package main

import (
        "context"
        "database/sql"
        "fmt"
        "github.com/docker/docker/api/types"
        "github.com/docker/docker/api/types/filters"
        "github.com/docker/docker/client"
        _ "github.com/lib/pq"
        "io"
        "os"
        "sync"
        "io/ioutil"
    "encoding/json"
    "time"
    "strings"
)

type SlackRequestBody struct {
    Text string `json:"text"`
}

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

var global_mutex sync.Mutex
var docker_events []DockerEvent

const (
        host     = "localhost"
        port     = 5432
        user     = "honeypot"
        password = "honeypot"
        dbname   = "honeypotdb"
)

func isError(err error) bool {
    if err != nil {
        fmt.Println(err.Error())
    }
    return (err != nil)
}

func GetProcessCmdline(pid int) {

        if pid != 0 {
                var path = fmt.Sprintf("/proc/%d/cmdline",pid)
            fmt.Println("Opening a file ")
            var file, err = os.Open(path)
            b, err := ioutil.ReadAll(file)

            fmt.Println(string(b))
            if isError(err) {
                return
            }
            defer file.Close()

        }
}

func GetProcessPath(pid int) string {

        var file string

        if pid != 0 {
                var path = fmt.Sprintf("/proc/%d/exe",pid)
            fmt.Println("READING")
            var file, err = os.Readlink(path)
            if isError(err){

            }

            return file
        }

        return file
}


func GetCurrentTime() string{
        time1:=time.Now()
        time2:=fmt.Sprintf("%s",time1)
        time_string:=strings.Split(time2," ")
        return time_string[0]
}

func CreateLogFile(datetime_string string) *os.File{
        filename := fmt.Sprintf("/home/sablearjun/honeypot/logs/%s-honeypot.log", datetime_string)
        file, err := os.OpenFile(filename, os.O_APPEND | os.O_CREATE | os.O_WRONLY, 0644)
        if isError(err) {
            fmt.Println("Failed to Create File\n")
        }
        return file
}

func WriteLog(file_obj *os.File, payload map[string]string) {

        jsonStr, err := json.Marshal(payload)
    if err != nil {
        fmt.Printf("Error: %s", err.Error())
    } else {
        file_obj.WriteString(fmt.Sprint(string(jsonStr), "\n"))
    }

}

func DockerEventReader() {
        ctx := context.Background()
        cli, err := client.NewClientWithOpts(client.FromEnv, client.WithHost("http://192.168.56.11:2375"), client.WithAPIVersionNegotiation())
        if err != nil {
                panic(err)
        }

        psqlconn := fmt.Sprintf("host=%s port=%d user=%s password=%s dbname=%s sslmode=disable", host, port, user, password, dbname)

    // open database
    db, err := sql.Open("postgres", psqlconn)
    if err != nil {
        panic(err)
    }

    defer db.Close()

    // check db
    err = db.Ping()
    if err != nil {
        panic(err)
    }

    stmt, err := db.Prepare("insert into docker_events " +
                "(action, actor, event_from, status, scope, time, type, container_id, image, name, path, cmdline, pid, privileged, " +
                "added_capabilities, dropped_capabilities, port_bindings) " +
                "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17)")
    if err != nil {
        panic(err)
    }

        var file *os.File
        datetime_string := GetCurrentTime()
        start_date := datetime_string
        for {
                event_channel, error_channel := cli.Events(ctx, types.EventsOptions{
                        Since:   "",
                        Until:   "",
                        Filters: filters.Args{},
                })

                for {
                        select {
                        case docker_event := <-event_channel:
                                t := time.Now()
                                fmt.Printf("\nAction: %s", docker_event.Action)
                                fmt.Printf("\nActor: %s", docker_event.Actor.Attributes)
                                fmt.Printf("\nFrom: %s", docker_event.From)
                                fmt.Printf("\nStatus: %s", docker_event.Status)
                                fmt.Printf("\nScope: %s", docker_event.Scope)
                                fmt.Printf("\nTime: %d", docker_event.TimeNano)
                                fmt.Printf("\nType: %s", docker_event.Type)
                                fmt.Printf("\nID: %s", docker_event.ID)
                                fmt.Println("")

                                file = CreateLogFile(start_date)

                                current_date := GetCurrentTime()

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

                                                fmt.Println("Is container privileged: ", container_details.HostConfig.Privileged)
                                                fmt.Println("Added capabilities: ", container_details.HostConfig.CapAdd)
                                                fmt.Println("Dropped capabilities: ", container_details.HostConfig.CapDrop)
                                                fmt.Println("Cmdline: ", strings.Join(container_details.Config.Cmd[:], "\n"))



                                        entry.Privileged = container_details.HostConfig.Privileged
                                        entry.AddedCapabilities = fmt.Sprint(container_details.HostConfig.CapAdd)
                                        entry.DroppedCapabilities = fmt.Sprint(container_details.HostConfig.CapDrop)
                                        entry.Path = container_details.Path
                                        entry.CmdLine = fmt.Sprint(strings.Join(container_details.Config.Cmd[:], "\n"))
                                        entry.PID = container_details.State.Pid
                                        entry.PortBindings = fmt.Sprint(container_details.HostConfig.PortBindings)
                                        entry.Path = GetProcessPath(entry.PID)

                                        logmap := map[string]string{
                                                "Time": t.String(),
                                                "Action": docker_event.Action,
                                                "Actor": entry.Actor,
                                                "From": docker_event.From,
                                                "Status": docker_event.Status,
                                                "Scope": docker_event.Scope,
                                                "Type": docker_event.Type,
                                                "ID": docker_event.ID,
                                                "Privileged": fmt.Sprint(entry.Privileged),
                                                "Added_Capabilities": entry.AddedCapabilities,
                                                "Drop_Capabilities": entry.DroppedCapabilities,
                                                "Path": entry.Path,
                                                "CmdLine": entry.CmdLine,
                                                "Binding": entry.PortBindings,
                                        }

                        if start_date == current_date {
                                WriteLog(file, logmap)
                        }else{
                                file = CreateLogFile(current_date)
                                start_date = current_date
                        }
                                }

                                stmt.Exec(entry.Action, entry.Actor, entry.From, entry.Status, entry.Scope, entry.Time, entry.Type,
                                        entry.ID, entry.Image, entry.Name, entry.Path, entry.CmdLine, entry.PID, entry.Privileged,
                                        entry.AddedCapabilities, entry.DroppedCapabilities, entry.PortBindings)
                                global_mutex.Lock()
                                docker_events = append(docker_events, entry)
                                global_mutex.Unlock()

                        case err = <-error_channel:
                                if err == io.EOF {
                                        break
                                } else {
                                    fmt.Println(err)
                                    break
                                }
                        }
                }
        }
}

func main() {
        for {
            DockerEventReader()
            time.Sleep(2 * time.Second)
        }
}

