package main

import (
	"context"
	"flag"
	"fmt"
	"io"
	"log"
	"os"
	"sync"

	"github.com/docker/docker/api/types"
	"github.com/docker/docker/api/types/filters"
	"github.com/docker/docker/client"
	_ "github.com/lib/pq"
	"github.com/osquery/osquery-go"
	"github.com/osquery/osquery-go/plugin/table"
)

type DockerEvent struct {
	Action              string
	Actor               string
	From                string
	Status              string
	Scope               string
	Time                int64
	Type                string
	ID                  string
	Image               string
	Name                string
	Path                string
	CmdLine             string
	PID                 int
	Privileged          bool
	AddedCapabilities   string
	DroppedCapabilities string
	PortBindings        string
}

type ContainerDetails struct {
	ID              string
	Created         string
	Path            string
	Args            []string
	Image           string
	ResolvConfPath  string
	HostnamePath    string
	HostsPath       string
	LogPath         string
	Name            string
	RestartCount    int
	PID             int
	Driver          string
	Platform        string
	MountLabel      string
	ProcessLabel    string
	AppArmorProfile string
	ExecIDs         []string
	SizeRw          *int64
	SizeRootFs      *int64
}

type NetworkDetails struct {
	ID                     string
	Bridge                 string // Bridge is the Bridge name the network uses(e.g. `docker0`)
	SandboxID              string // SandboxID uniquely represents a container's network stack
	HairpinMode            bool   // HairpinMode specifies if hairpin NAT should be enabled on the virtual interface
	LinkLocalIPv6Address   string // LinkLocalIPv6Address is an IPv6 unicast address using the link-local prefix
	LinkLocalIPv6PrefixLen int    // LinkLocalIPv6PrefixLen is the prefix length of an IPv6 unicast address
	Ports                  string // Ports is a collection of PortBinding indexed by Port
	SandboxKey             string // SandboxKey identifies the sandbox
	SecondaryIPAddresses   string
	SecondaryIPv6Addresses string
	PID                    int
}

var global_mutex sync.Mutex
var docker_events []DockerEvent
var docker_details []ContainerDetails
var docker_network []NetworkDetails

const (
	hostname      = "localhost"
	port          = 5432
	username      = "yogesh"
	password      = "honeypot"
	database_name = "honeypotdb"
)

// FoobarColumns returns the columns that our table will return.
func DockerEventColumns() []table.ColumnDefinition {
	return []table.ColumnDefinition{
		table.TextColumn("action"),
		table.TextColumn("from"),
		table.TextColumn("status"),
		table.TextColumn("scope"),
		table.TextColumn("time"),
		table.TextColumn("type"),
		table.TextColumn("path"),
		table.TextColumn("cmdline"),
		table.TextColumn("pid"),
		table.TextColumn("container_id"),
		table.TextColumn("container_image"),
		table.TextColumn("container_name"),
		table.TextColumn("privileged"),
		table.TextColumn("added_capabilities"),
		table.TextColumn("dropped_capabilities"),
		table.TextColumn("port_binding"),
	}
}

// FoobarGenerate will be called whenever the table is queried. It should return
// a full table scan.
func DockerEventGenerate(ctx context.Context, queryContext table.QueryContext) ([]map[string]string, error) {
	var records []map[string]string

	global_mutex.Lock()
	for _, entry := range docker_events {
		record := make(map[string]string)
		record["action"] = entry.Action
		record["from"] = entry.From
		record["status"] = entry.Status
		record["scope"] = entry.Scope
		record["time"] = fmt.Sprint(entry.Time)
		record["type"] = entry.Type
		record["cmdline"] = entry.CmdLine
		record["pid"] = fmt.Sprint(entry.PID)
		record["container_id"] = entry.ID
		record["container_image"] = entry.Image
		record["container_name"] = entry.Name
		record["privileged"] = fmt.Sprint(entry.Privileged)
		record["added_capabilities"] = entry.AddedCapabilities
		record["dropped_capabilities"] = entry.DroppedCapabilities
		record["port_binding"] = entry.PortBindings

		records = append(records, record)
	}
	global_mutex.Unlock()

	return records, nil
}

func DockerEventReader() {
	ctx := context.Background()
	cli, err := client.NewClientWithOpts(client.FromEnv, client.WithAPIVersionNegotiation())
	if err != nil {
		panic(err)
	}

	for {
		event_channel, error_channel := cli.Events(ctx, types.EventsOptions{
			Since:   "",
			Until:   "",
			Filters: filters.Args{},
		})

		for {
			select {
			case docker_event := <-event_channel:
				/*
					fmt.Printf("\nAction: %s", docker_event.Action)
					fmt.Printf("\nActor: %s", docker_event.Actor.Attributes)
					fmt.Printf("\nFrom: %s", docker_event.From)
					fmt.Printf("\nStatus: %s", docker_event.Status)
					fmt.Printf("\nScope: %s", docker_event.Scope)
					fmt.Printf("\nTime: %d", docker_event.TimeNano)
					fmt.Printf("\nType: %s", docker_event.Type)
					fmt.Printf("\nID: %s", docker_event.ID)
					fmt.Println("")
				*/

				var entry DockerEvent
				entry.ID = docker_event.ID
				entry.Type = docker_event.Type
				entry.From = docker_event.From
				entry.Scope = docker_event.Scope
				entry.Status = docker_event.Status
				entry.Action = docker_event.Action
				entry.Actor = fmt.Sprint(docker_event.Actor.Attributes)

				if docker_event.Action == "die" || docker_event.Action == "create" || docker_event.Action == "attach" || docker_event.Action == "start" {
					if image, found := docker_event.Actor.Attributes["image"]; found {
						entry.Image = image
					}

					if name, found := docker_event.Actor.Attributes["name"]; found {
						entry.Name = name
					}
				} else if docker_event.Action == "connect" || docker_event.Action == "disconnect" {
					if container_id, found := docker_event.Actor.Attributes["container"]; found {
						entry.ID = container_id
					}
				}

				entry.Time = docker_event.Time
				entry.Privileged = false

				container_details, err := cli.ContainerInspect(ctx, docker_event.ID)
				if err == nil {
					/*
						fmt.Println("Is container privileged: ", container_details.HostConfig.Privileged)
						fmt.Println("Added capabilities: ", container_details.HostConfig.CapAdd)
						fmt.Println("Dropped capabilities: ", container_details.HostConfig.CapDrop)

					*/

					entry.Privileged = container_details.HostConfig.Privileged
					entry.AddedCapabilities = fmt.Sprint(container_details.HostConfig.CapAdd)
					entry.DroppedCapabilities = fmt.Sprint(container_details.HostConfig.CapDrop)
					entry.Path = container_details.Path
					entry.CmdLine = fmt.Sprint(container_details.Args)
					entry.PID = container_details.State.Pid
					entry.PortBindings = fmt.Sprint(container_details.HostConfig.PortBindings)
				}

				global_mutex.Lock()
				docker_events = append(docker_events, entry)
				global_mutex.Unlock()

			case err = <-error_channel:
				if err == io.EOF {
					break
				}
			}
		}
	}
}

func DockerDetailsColumns() []table.ColumnDefinition {
	return []table.ColumnDefinition{
		table.TextColumn("ID"),
		table.TextColumn("Created"),
		table.TextColumn("Path"),
		table.TextColumn("Args"),
		table.TextColumn("Image"),
		table.TextColumn("ResolvConfPath"),
		table.TextColumn("HostnamePath"),
		table.TextColumn("HostsPath"),
		table.TextColumn("LogPath"),
		table.TextColumn("Name"),
		table.TextColumn("RestartCount"),
		table.TextColumn("Driver"),
		table.TextColumn("pid"),
		table.TextColumn("Platform"),
		table.TextColumn("MountLabel"),
		table.TextColumn("ProcessLabel"),
		table.TextColumn("AppArmorProfile"),
		table.TextColumn("ExecIDs"),
		table.TextColumn("SizeRw"),
		table.TextColumn("SizeRootFs"),
	}
}

func DockerDetailsGenerate(ctx context.Context, queryContext table.QueryContext) ([]map[string]string, error) {
	var records []map[string]string

	global_mutex.Lock()
	for _, entry := range docker_details {
		record := make(map[string]string)
		record["ID"] = entry.ID
		record["Created"] = entry.Created
		record["Path"] = entry.Path
		record["Args"] = fmt.Sprint(entry.Args)
		record["Image"] = entry.Image
		record["ResolvConfPath"] = entry.ResolvConfPath
		record["HostnamePath"] = entry.HostnamePath
		record["HostsPath"] = entry.HostsPath
		record["LogPath"] = entry.LogPath
		record["Name"] = entry.Name
		record["pid"] = fmt.Sprint(entry.PID)
		record["RestartCount"] = fmt.Sprint(entry.RestartCount)
		record["Driver"] = entry.Driver
		record["Platform"] = entry.Platform
		record["MountLabel"] = entry.MountLabel
		record["ProcessLabel"] = entry.ProcessLabel
		record["AppArmorProfile"] = entry.AppArmorProfile
		record["ExecIDs"] = fmt.Sprint(entry.ExecIDs)
		record["SizeRw"] = fmt.Sprint(entry.SizeRw)
		record["SizeRootFs"] = fmt.Sprint(entry.SizeRootFs)

		records = append(records, record)
	}
	global_mutex.Unlock()

	return records, nil
}

func DockerDetailsReader() {
	ctx := context.Background()
	cli, err := client.NewClientWithOpts(client.FromEnv, client.WithAPIVersionNegotiation())
	if err != nil {
		panic(err)
	}

	for {
		event_channel, error_channel := cli.Events(ctx, types.EventsOptions{
			Since:   "",
			Until:   "",
			Filters: filters.Args{},
		})

		for {
			select {
			case docker_event := <-event_channel:
				/*
					fmt.Printf("\nAction: %s", docker_event.Action)
					fmt.Printf("\nActor: %s", docker_event.Actor.Attributes)
					fmt.Printf("\nFrom: %s", docker_event.From)
					fmt.Printf("\nStatus: %s", docker_event.Status)
					fmt.Printf("\nScope: %s", docker_event.Scope)
					fmt.Printf("\nTime: %d", docker_event.TimeNano)
					fmt.Printf("\nType: %s", docker_event.Type)
					fmt.Printf("\nID: %s", docker_event.ID)
					fmt.Println("")
				*/

				var entry ContainerDetails
				entry.ID = docker_event.ID
				container_details, err := cli.ContainerInspect(ctx, docker_event.ID)
				if err == nil {
					/*
						fmt.Println("Is container privileged: ", container_details.HostConfig.Privileged)
						fmt.Println("Added capabilities: ", container_details.HostConfig.CapAdd)
						fmt.Println("Dropped capabilities: ", container_details.HostConfig.CapDrop)

					*/
					entry.Created = container_details.Created
					entry.Path = container_details.Path
					entry.Args = container_details.Args
					entry.Image = container_details.Image
					entry.ResolvConfPath = container_details.ResolvConfPath
					entry.HostnamePath = container_details.HostnamePath
					entry.HostsPath = container_details.HostsPath
					entry.LogPath = container_details.LogPath
					entry.Name = container_details.Name
					entry.RestartCount = container_details.RestartCount
					entry.Driver = container_details.Driver
					entry.PID = container_details.State.Pid
					entry.Platform = container_details.Platform
					entry.MountLabel = container_details.MountLabel
					entry.ProcessLabel = container_details.ProcessLabel
					entry.AppArmorProfile = container_details.AppArmorProfile
					entry.ExecIDs = container_details.ExecIDs
					entry.SizeRw = container_details.SizeRw
					entry.SizeRootFs = container_details.SizeRootFs
				}

				global_mutex.Lock()
				docker_details = append(docker_details, entry)
				global_mutex.Unlock()

			case err = <-error_channel:
				if err == io.EOF {
					break
				}
			}
		}
	}
}

func DockerNetworkColumns() []table.ColumnDefinition {
	return []table.ColumnDefinition{
		table.TextColumn("ID"),
		table.TextColumn("Bridge"),
		table.TextColumn("SandboxID"),
		table.TextColumn("HairpinMode"),
		table.TextColumn("LinkLocalIPv6Address"),
		table.TextColumn("LinkLocalIPv6PrefixLen"),
		table.TextColumn("Ports"),
		table.TextColumn("pid"),
		table.TextColumn("SandboxKey"),
		table.TextColumn("SecondaryIPAddresses"),
		table.TextColumn("SecondaryIPv6Addresses"),
	}
}

func DockerNetworkGenerate(ctx context.Context, queryContext table.QueryContext) ([]map[string]string, error) {
	var records []map[string]string

	global_mutex.Lock()
	for _, entry := range docker_network {
		record := make(map[string]string)
		record["ID"] = entry.ID
		record["Bridge"] = entry.Bridge
		record["SandboxID"] = entry.SandboxID
		record["HairpinMode"] = fmt.Sprint(entry.HairpinMode)
		record["LinkLocalIPv6Address"] = entry.LinkLocalIPv6Address
		record["LinkLocalIPv6PrefixLen"] = fmt.Sprint(entry.LinkLocalIPv6PrefixLen)
		record["Ports"] = entry.Ports
		record["pid"] = fmt.Sprint(entry.PID)
		record["SandboxKey"] = entry.SandboxKey
		record["SecondaryIPAddresses"] = entry.SecondaryIPAddresses
		record["SecondaryIPv6Addresses"] = entry.SecondaryIPv6Addresses

		records = append(records, record)
	}
	global_mutex.Unlock()

	return records, nil
}

func DockerNetworkReader() {
	ctx := context.Background()
	cli, err := client.NewClientWithOpts(client.FromEnv, client.WithAPIVersionNegotiation())
	if err != nil {
		panic(err)
	}

	for {
		event_channel, error_channel := cli.Events(ctx, types.EventsOptions{
			Since:   "",
			Until:   "",
			Filters: filters.Args{},
		})

		for {
			select {
			case docker_event := <-event_channel:
				/*
					fmt.Printf("\nAction: %s", docker_event.Action)
					fmt.Printf("\nActor: %s", docker_event.Actor.Attributes)
					fmt.Printf("\nFrom: %s", docker_event.From)
					fmt.Printf("\nStatus: %s", docker_event.Status)
					fmt.Printf("\nScope: %s", docker_event.Scope)
					fmt.Printf("\nTime: %d", docker_event.TimeNano)
					fmt.Printf("\nType: %s", docker_event.Type)
					fmt.Printf("\nID: %s", docker_event.ID)
					fmt.Println("")
				*/

				var entry NetworkDetails
				entry.ID = docker_event.ID
				container_details, err := cli.ContainerInspect(ctx, docker_event.ID)
				if err == nil {
					/*
						fmt.Println("Is container privileged: ", container_details.HostConfig.Privileged)
						fmt.Println("Added capabilities: ", container_details.HostConfig.CapAdd)
						fmt.Println("Dropped capabilities: ", container_details.HostConfig.CapDrop)

					*/
					entry.Bridge = container_details.NetworkSettings.NetworkSettingsBase.Bridge
					entry.SandboxID = container_details.NetworkSettings.NetworkSettingsBase.SandboxID
					entry.HairpinMode = container_details.NetworkSettings.NetworkSettingsBase.HairpinMode
					entry.LinkLocalIPv6Address = container_details.NetworkSettings.NetworkSettingsBase.LinkLocalIPv6Address
					entry.LinkLocalIPv6PrefixLen = container_details.NetworkSettings.NetworkSettingsBase.LinkLocalIPv6PrefixLen
					entry.Ports = fmt.Sprint(container_details.NetworkSettings.NetworkSettingsBase.Ports)
					entry.SandboxKey = container_details.NetworkSettings.NetworkSettingsBase.SandboxKey
					entry.PID = container_details.State.Pid
					entry.SecondaryIPAddresses = fmt.Sprint(container_details.NetworkSettings.NetworkSettingsBase.SecondaryIPAddresses)
					entry.SecondaryIPv6Addresses = fmt.Sprint(container_details.NetworkSettings.NetworkSettingsBase.SecondaryIPv6Addresses)
				}

				global_mutex.Lock()
				docker_network = append(docker_network, entry)
				global_mutex.Unlock()

			case err = <-error_channel:
				if err == io.EOF {
					break
				}
			}
		}
	}
}

func main() {

	socket := flag.String("socket", "/var/osquery/osquery.em", "Path to osquery socket file")
	flag.Parse()
	if *socket == "" {
		log.Fatalf(`Usage: %s --socket SOCKET_PATH`, os.Args[0])
	}

	server, err := osquery.NewExtensionManagerServer("Container Details 2.0", *socket)
	if err != nil {
		log.Fatalf("Error creating extension: %s\n", err)
	}

	// Create and register a new table plugin with the server.
	// table.NewPlugin requires the table plugin name,
	// a slice of Columns and a Generate function.
	server.RegisterPlugin(table.NewPlugin("docker_events", DockerEventColumns(), DockerEventGenerate))
	server.RegisterPlugin(table.NewPlugin("docker_details", DockerDetailsColumns(), DockerDetailsGenerate))
	server.RegisterPlugin(table.NewPlugin("docker_network", DockerNetworkColumns(), DockerNetworkGenerate))

	go DockerEventReader()
	go DockerDetailsReader()
	go DockerNetworkReader()

	if err := server.Run(); err != nil {
		log.Fatalln(err)
	}
}
