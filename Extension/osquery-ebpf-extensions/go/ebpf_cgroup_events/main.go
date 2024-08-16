package main

import (
	"C"
	"context"
	"flag"
	"log"
	"os"
	// "strconv"

	// "strconv"

	"github.com/cilium/ebpf/link"
	"github.com/osquery/osquery-go"
	"github.com/osquery/osquery-go/plugin/table"
	"golang.org/x/sys/unix"
)
import "fmt"

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang -cflags "-O2 -g -Wall" bpf bpf/cgroups.bpf.c -- -I../headers

var objs bpfObjects

func main() {
	socket := flag.String("socket", "", "Path to socket file")
	flag.Parse()

	if *socket == "" {
		log.Fatalf(`Usage: %s --socket SOCKET_PATH`, os.Args[0])
	}

	objs = bpfObjects{}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("loading objects: %s", err)
	}
	defer objs.Close()

	tpEnter, err := link.Tracepoint("cgroup", "cgroup_mkdir", objs.TracepointCgroupCgroupMkdir, nil)
	if err != nil {
		log.Fatalf("opening tracepoint: %s", err)
	}
	defer tpEnter.Close()


	server, err := osquery.NewExtensionManagerServer("ebpf_cgroup_events", *socket)
	if err != nil {
		log.Fatalf("error creating extension: %s\n", err)
	}

	server.RegisterPlugin(table.NewPlugin("ebpf_cgroup_events", ebpfCgroupEventsColumns(), ebpfCgroupEventsGenerate))
	if err := server.Run(); err != nil {
		log.Fatalln(err)
	}
}

func ebpfCgroupEventsColumns() []table.ColumnDefinition {
	return []table.ColumnDefinition{
		table.BigIntColumn("cid"),
		table.TextColumn("cgroup_path"),
	}
}

func ebpfCgroupEventsGenerate(ctx context.Context, queryContext table.QueryContext) ([]map[string]string, error) {
	var processEvents []map[string]string
	iter := objs.CgroupMap.Iterate()
	var (
		key uint64
		val [400]byte
	)
	for iter.Next(&key, &val) {
		cid := key
		cgroupPath := unix.ByteSliceToString(val[:])
		fmt.Println(val)
		fmt.Printf("cid: %d, cgroup_path : %s\n", cid, cgroupPath)
	}
	return processEvents, nil
}
