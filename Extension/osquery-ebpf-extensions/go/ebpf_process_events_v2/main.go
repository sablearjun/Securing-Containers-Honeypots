package main

import (
	"C"
	"context"
	"flag"
	"fmt"
	"io/fs"
	"log"
	"path/filepath"
	"strconv"
	"strings"
	"time"

	"github.com/cilium/ebpf/link"
	"github.com/osquery/osquery-go"
	"github.com/osquery/osquery-go/plugin/table"
	"golang.org/x/sys/unix"
)

//go:generate sh -c "mkdir -p build && cd build && go run github.com/cilium/ebpf/cmd/bpf2go -cc clang -cflags '-O2 -g -Wall' bpf ../bpf/process_exec.bpf.c -- -I../headers"

// global client variable
var client *osquery.ExtensionManagerClient

var err error
var objs bpfObjects
var cidCgroupPathMap map[uint64]string

const (
	cgroupPathRoot string = "/sys/fs/cgroup"
)

func checkContainerProcess(cgroupPath string) bool {
	if strings.Contains(cgroupPath, "docker") {
		return true
	}
	if strings.Contains(cgroupPath, "libpod-") && !strings.Contains(cgroupPath, "libpod-conmon") {
		return true
	}
	return false
}

func getCgroupPathUserSpace(cid uint64) string {
	var cgroupPath string
	if cidCgroupPathMap[cid] == "" {
		filepath.WalkDir(cgroupPathRoot, func(path string, entry fs.DirEntry, err error) error {
			if err != nil {
				return err
			}
			if !entry.IsDir() {
				return nil
			}
			fstat := unix.Stat_t{}
			unix.Stat(path, &fstat)
			var cgroupID uint64 = fstat.Ino
			if cgroupID == cid {
				cgroupPath = strings.TrimPrefix(path, cgroupPathRoot)
				cidCgroupPathMap[cid] = cgroupPath
			}
			return nil
		})
	} else {
		cgroupPath = cidCgroupPathMap[cid]
	}
	return cgroupPath
}

func ebpfProcessEventsColumns() []table.ColumnDefinition {
	return []table.ColumnDefinition{
		table.BigIntColumn("tid"),
		table.BigIntColumn("pid"),
		table.BigIntColumn("parent"),
		table.BigIntColumn("uid"),
		table.BigIntColumn("gid"),
		table.BigIntColumn("cid"),
		table.TextColumn("exit_code"),
		table.IntegerColumn("probe_error"),
		table.TextColumn("syscall"),
		table.TextColumn("path"),
		table.TextColumn("cwd"),
		table.TextColumn("cmdline"),
		table.IntegerColumn("duration"),
		table.TextColumn("json_cmdline"),
		table.TextColumn("ntime"),
		table.BigIntColumn("time"),
		table.IntegerColumn("eid"),
		table.IntegerColumn("is_container_process"),
	}
}

// func getSqlQuery(queryContext table.QueryContext) string {
// 	query := "SELECT *, json_cmdline, time, eid FROM bpf_process_events"
// 	isWhere := false
// 	if len(queryContext.Constraints["path"].Constraints) > 0 {
// 		for _, constraint := range queryContext.Constraints["path"].Constraints {
// 			if constraint.Operator == table.OperatorEquals {
// 				if !isWhere {
// 					query += " WHERE "
// 					isWhere = true
// 				} else {
// 					query += " OR "
// 				}
// 				query += fmt.Sprintf("path = \"%s\"", constraint.Expression)
// 			}
// 		}
// 	}
// 	query += ";"
// 	fmt.Printf("query: %s\n", query)
// 	return query
// }

func checkContainerConstraint(queryContext table.QueryContext, is_container_process int) bool {
	if len(queryContext.Constraints["is_container_process"].Constraints) == 0 {
		return true
	}
	if len(queryContext.Constraints["is_container_process"].Constraints) > 0 {
		for _, constraint := range queryContext.Constraints["is_container_process"].Constraints {
			if constraint.Operator == table.OperatorEquals {
				if constraint.Expression == strconv.Itoa(is_container_process) {
					return true
				}
			}
		}
	}
	return false
}

func ebpfProcessEventsGenerate(ctx context.Context, queryContext table.QueryContext) ([]map[string]string, error) {
	var results []map[string]string

	// Query the table
	response, err := client.Query("SELECT *, json_cmdline, time, eid FROM bpf_process_events;")
	if err != nil {
		return nil, fmt.Errorf("error running query: %v", err)
	}

	for _, r := range response.Response {
		cid, err := strconv.ParseUint(r["cid"], 10, 64)
		if err != nil {
			return nil, fmt.Errorf("error converting cid to int: %v", err)
		}

		// get cgroup name
		var cgroupNames bpfCgroupPathParts
		objs.CidCgroupPathParts.Lookup(cid, &cgroupNames)

		cgroupNameStr := unix.ByteSliceToString(cgroupNames.Name[:])
		parentCgroupNameStr := unix.ByteSliceToString(cgroupNames.ParentName[:])

		cgroupPath := cgroupNameStr
		if parentCgroupNameStr != "" {
			cgroupPath = parentCgroupNameStr + "/" + cgroupNameStr
		}

		// fmt.Printf("cid: %d, cgroup_path: %s\n", cid, cgroupPath)

		isContainerProcess := 0
		if checkContainerProcess(cgroupPath) {
			isContainerProcess = 1
		} else if r["uid"] != "0" {
			cgroupPath = getCgroupPathUserSpace(cid)
			if checkContainerProcess(cgroupPath) {
				isContainerProcess = 1
			}
		}
		// fmt.Printf("is_container_process: %d\n", isContainerProcess)

		r["is_container_process"] = strconv.Itoa(isContainerProcess)
		if checkContainerConstraint(queryContext, isContainerProcess) {
			results = append(results, r)
		}
	}

	return results, nil
}

func main() {

	socketPtr := flag.String("socket", "", "Path to the extensions UNIX domain socket")
	timeout := flag.Int("timeout", 10, "Timeout for Thrift client")
	interval := flag.Int("interval", 10, "Interval for Thrift client")

	flag.Parse()

	cidCgroupPathMap = make(map[uint64]string)

	socket := *socketPtr
	if socket == "" {
		log.Fatal("Missing required --socket argument")
	}

	objs = bpfObjects{}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("loading objects: %s", err)
	}
	defer objs.Close()

	tpEnter, err := link.Tracepoint("syscalls", "sys_enter_execve", objs.TracepointSyscallsSysEnterExecve, nil)
	if err != nil {
		log.Fatalf("opening tracepoint: %s", err)
	}
	defer tpEnter.Close()

	serverTimeout := osquery.ServerTimeout(time.Duration(*timeout) * time.Second)
	serverInterval := osquery.ServerPingInterval(time.Duration(*interval) * time.Second)

	client, err = osquery.NewClient(socket, time.Duration(*timeout)*time.Second)
	if err != nil {
		log.Fatalf("Error creating Thrift client: %v", err)
	}
	defer client.Close()

	server, err := osquery.NewExtensionManagerServer("ebpf_process_events", socket, serverTimeout, serverInterval)
	if err != nil {
		log.Fatalf("Error creating extension: %v", err)
	}

	// Register the table we created above
	server.RegisterPlugin(table.NewPlugin("ebpf_process_events_v2", ebpfProcessEventsColumns(), ebpfProcessEventsGenerate))
	if err := server.Run(); err != nil {
		log.Fatalf("Error running extension: %v", err)
	}
}
