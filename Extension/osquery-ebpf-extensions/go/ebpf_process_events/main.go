package main

import (
	"context"
	"database/sql"
	"flag"
	"io/fs"
	"log"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"time"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	_ "github.com/mattn/go-sqlite3"
	"github.com/osquery/osquery-go"
	"github.com/osquery/osquery-go/plugin/table"
	"golang.org/x/sys/unix"
)

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang -cflags "-O2 -g -Wall" bpf bpf/process.c -- -I../headers

const (
	cgroupPathRoot string = "/sys/fs/cgroup"
)

var sqliteDb *sql.DB
var objs bpfObjects
var cidMap map[uint64]string

func main() {

	// socketPtr := flag.String("socket", "", "Path to the extensions UNIX domain socket")
	timeout := flag.Int("timeout", 10, "Timeout for Thrift client")
	interval := flag.Int("interval", 10, "Interval for Thrift client")
	socket := flag.String("socket", "", "Path to socket file")
	flag.Parse()


	if *socket == "" {
		log.Fatalf(`Usage: %s --socket SOCKET_PATH`, os.Args[0])
	}

	cidMap = make(map[uint64]string)

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

	tpExit, err := link.Tracepoint("syscalls", "sys_exit_execve", objs.TracepointSyscallsSysExitExecve, nil)
	if err != nil {
		log.Fatalf("opening tracepoint: %s", err)
	}
	defer tpExit.Close()


	serverTimeout := osquery.ServerTimeout(time.Duration(*timeout) * time.Second)
	serverInterval := osquery.ServerPingInterval(time.Duration(*interval) * time.Second)

 
	server, err := osquery.NewExtensionManagerServer("ebpf_process_events", *socket, serverTimeout, serverInterval)
	if err != nil {
		log.Fatalf("error creating extension: %s\n", err)
	}

	os.Remove("sqlite-db.db")
	file, err := os.Create("sqlite-db.db")
	if err != nil {
		log.Fatal(err.Error())
	}
	file.Close()

	sqliteDb, _ = sql.Open("sqlite3", "./sqlite-db.db")
	defer sqliteDb.Close()

	createTable(sqliteDb)

	server.RegisterPlugin(table.NewPlugin("ebpf_process_events", ebpfProcessEventsColumns(), ebpfProcessEventsGenerate))
	if err := server.Run(); err != nil {
		log.Fatalln(err)
	}
}

func createTable(db *sql.DB) {
	createQuery := `CREATE TABLE ebpf_process_events (
		"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
		"tid" INTEGER,
		"pid" INTEGER,
		"parent" INTEGER,
		"uid" INTEGER,
		"gid" INTEGER,
		"cid" INTEGER,
		"exit_code" TEXT,
		"syscall" TEXT,
		"path" TEXT,
		"duration" INTEGER,
		"ntime" TEXT,
		"cgroup_path" TEXT,
		"is_container_process" INTEGER
	);`

	stmt, err := db.Prepare(createQuery)
	if err != nil {
		log.Fatal(err.Error())
	}
	_, err = stmt.Exec()
	if err != nil {
		log.Fatal(err.Error())
	}
}

func insertEvent(db *sql.DB, event bpfEvent) {
	query := `INSERT INTO ebpf_process_events(
		tid, pid, parent, uid, 
		gid, cid, exit_code, syscall, 
		path, duration, ntime, cgroup_path, is_container_process
		) 
		VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);`
	stmt, err := db.Prepare(query)
	if err != nil {
		log.Fatal(err.Error())
	}

	var cgroupPath string
	if cidMap[event.Cid] == "" {
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
			if cgroupID == event.Cid {
				cgroupPath = strings.TrimPrefix(path, cgroupPathRoot)
				cidMap[event.Cid] = cgroupPath
			}
			return nil
		})
	} else {
		cgroupPath = cidMap[event.Cid]
	}

	isContainerProcess := 0
	if checkContainerProcess(cgroupPath) {
		isContainerProcess = 1
	}

	_, err = stmt.Exec(
		event.Tid,
		event.Pid,
		event.Ppid,
		event.Uid,
		event.Gid,
		event.Cid,
		event.Retcode,
		"exec",
		unix.ByteSliceToString(event.Filename[:]),
		event.Duration,
		event.Ntime,
		cgroupPath,
		isContainerProcess)

	if err != nil {
		log.Fatal(err.Error())
	}
}

func insertMapContentsToDb(db *sql.DB, m *ebpf.Map) {
	var event bpfEvent

	for m.LookupAndDelete(nil, &event) == nil {
		insertEvent(db, event)
	}
}

func checkContainerProcess(cgroupPath string) bool {
	containerCgroupPatterns := []string{"docker-", "libpod-"}
	for _, pattern := range containerCgroupPatterns {
		if strings.Contains(cgroupPath, pattern) {
			return true
		}
	}
	return false
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
		table.TextColumn("syscall"),
		table.TextColumn("path"),
		table.IntegerColumn("duration"),
		table.TextColumn("ntime"),
		table.TextColumn("cgroup_path"),
		table.IntegerColumn("is_container_process"),
	}
}

func ebpfProcessEventsGenerate(ctx context.Context, queryContext table.QueryContext) ([]map[string]string, error) {
	insertMapContentsToDb(sqliteDb, objs.ExecEvents)
	var processEvents []map[string]string
	var event map[string]string

	row, err := sqliteDb.Query("SELECT * FROM ebpf_process_events ORDER BY id")
	if err != nil {
		log.Fatal(err)
	}
	defer row.Close()

	for row.Next() {
		var (
			id                   int
			tid                  int
			pid                  int
			parent               int
			uid                  int
			gid                  int
			cid                  int
			exit_code            string
			syscall              string
			path                 string
			duration             int
			ntime                string
			cgroup_path          string
			is_container_process int
		)

		row.Scan(&id, &tid, &pid, &parent, &uid, &gid, &cid, &exit_code, &syscall, &path, &duration, &ntime, &cgroup_path, &is_container_process)

		event = map[string]string{
			"tid":                  strconv.Itoa(tid),
			"pid":                  strconv.Itoa(pid),
			"parent":               strconv.Itoa(parent),
			"uid":                  strconv.Itoa(uid),
			"gid":                  strconv.Itoa(gid),
			"cid":                  strconv.Itoa(cid),
			"exit_code":            exit_code,
			"syscall":              syscall,
			"path":                 path,
			"duration":             strconv.Itoa(duration),
			"ntime":                ntime,
			"cgroup_path":          cgroup_path,
			"is_container_process": strconv.Itoa(is_container_process),
		}
		processEvents = append(processEvents, event)
	}

	return processEvents, nil
}
