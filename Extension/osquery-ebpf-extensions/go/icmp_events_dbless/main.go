package main

import (
	"context"
	"flag"
	"log"
	"net"
	"os"
	"strconv"

	"github.com/cilium/ebpf/link"
	"github.com/osquery/osquery-go"
	"github.com/osquery/osquery-go/plugin/table"
)

var objs bpfObjects

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang -cflags "-O2 -g -Wall" bpf ./bpf/xdp.c -- -I../headers

func main() {
	ifaceName := flag.String("iface", "lo", "Interface to attach the XDP program")
	socket := flag.String("socket", "", "Path to socket file")
	flag.Parse()

	iface, err := net.InterfaceByName(*ifaceName)
	if err != nil {
		log.Fatalf("lookup network iface %q: %s", *ifaceName, err)
	}

	objs = bpfObjects{}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("loading objects: %s", err)
	}
	defer objs.Close()

	l, err := link.AttachXDP(link.XDPOptions{
		Program:   objs.XdpProgFunc,
		Interface: iface.Index,
	})
	if err != nil {
		log.Fatalf("could not attach XPD program: %s", err)
	}
	defer l.Close()

	if *socket == "" {
		log.Fatalf(`Usage: %s --socket SOCKET_PATH`, os.Args[0])
	}

	server, err := osquery.NewExtensionManagerServer("icmp_events", *socket)
	if err != nil {
		log.Fatalf("Error creating extension: %s\n", err)
	}

	server.RegisterPlugin(table.NewPlugin("icmp_events", ICMPEventsColumns(), ICMPEventsGenerate))
	if err := server.Run(); err != nil {
		log.Fatalln(err)
	}

}

func ICMPEventsColumns() []table.ColumnDefinition {
	return []table.ColumnDefinition{
		table.TextColumn("source_ip"),
		table.TextColumn("packet_count"),
	}
}

// func displayMapContents(m *ebpf.Map) (string, error) {
// 	var (
// 		sb  strings.Builder
// 		key []byte
// 		val uint32
// 	)
// 	iter := m.Iterate()
// 	for iter.Next(&key, &val) {
// 		sourceIP := net.IP(key)
// 		packetCount := val
// 		sb.WriteString(fmt.Sprintf("\t%s => %d\n", sourceIP, packetCount))
// 	}
// 	return sb.String(), iter.Err()
// }

func ICMPEventsGenerate(ctx context.Context, queryContext table.QueryContext) ([]map[string]string, error) {
	// s, err := displayMapContents(objs.XdpStatsMap)
	// if err != nil {
	// 	log.Printf("Error reading map: %s", err)
	// }
	// log.Printf("Map contents:\n%s", s)

	var icmp_events []map[string]string

	iter := objs.XdpStatsMap.Iterate()
	var (
		key []byte
		val uint32
	)

	for iter.Next(&key, &val) {
		sourceIP := net.IP(key)
		packetCount := val
		icmp_events = append(icmp_events, map[string]string{"source_ip": sourceIP.String(), "packet_count": strconv.Itoa(int(packetCount))})
	}

	return icmp_events, nil
}
