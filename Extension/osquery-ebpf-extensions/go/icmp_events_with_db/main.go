package main

import (
	"context"
	"database/sql"
	"encoding/binary"
	"flag"
	"log"
	"net"
	"os"
	"strconv"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	_ "github.com/mattn/go-sqlite3"
	"github.com/osquery/osquery-go"
	"github.com/osquery/osquery-go/plugin/table"
)

var sqliteDb *sql.DB
var objs bpfObjects

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang -cflags "-O2 -g -Wall" bpf bpf/xdp.c -- -I../headers

func main() {
	ifaceName := flag.String("iface", "lo", "Interface to attach the XDP program")
	socket := flag.String("socket", "", "Path to socket file")
	flag.Parse()

	if *socket == "" {
		log.Fatalf(`Usage: %s --socket SOCKET_PATH`, os.Args[0])
	}

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

	server, err := osquery.NewExtensionManagerServer("icmp_events", *socket)
	if err != nil {
		log.Fatalf("Error creating extension: %s\n", err)
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

	server.RegisterPlugin(table.NewPlugin("icmp_events", ICMPEventsColumns(), ICMPEventsGenerate))
	if err := server.Run(); err != nil {
		log.Fatalln(err)
	}

}

func createTable(db *sql.DB) {
	createQuery := `CREATE TABLE icmp_events (
		"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
		"source_ip" TEXT,
		"seq_no" INTEGER,
		"type" INTEGER,
		"ttl" INTEGER
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

func insertICMPEvent(db *sql.DB, sourceIP string, seqNo uint16, msgType uint8, ttl uint8) {
	query := `INSERT INTO icmp_events(source_ip, seq_no, type, ttl) VALUES (?, ?, ?, ?);`
	stmt, err := db.Prepare(query)
	if err != nil {
		log.Fatal(err.Error())
	}

	_, err = stmt.Exec(sourceIP, seqNo, msgType, ttl)

	if err != nil {
		log.Fatal(err.Error())
	}
}

func insertMapContentsToDb(db *sql.DB, m *ebpf.Map) {
	var icmp bpfIcmpInfo

	for m.LookupAndDelete(nil, &icmp) == nil {
		ip := make(net.IP, 4)
		binary.LittleEndian.PutUint32(ip, icmp.Ip)
		seqNo := make([]byte, 2)
		binary.LittleEndian.PutUint16(seqNo, icmp.SeqNo)
		icmp.SeqNo = binary.BigEndian.Uint16(seqNo)
		insertICMPEvent(db, ip.String(), icmp.SeqNo, icmp.Type, icmp.Ttl)
	}
}

func ICMPEventsColumns() []table.ColumnDefinition {
	return []table.ColumnDefinition{
		table.TextColumn("source_ip"),
		table.TextColumn("seq_no"),
		table.TextColumn("type"),
		table.TextColumn("ttl"),
	}
}

func ICMPEventsGenerate(ctx context.Context, queryContext table.QueryContext) ([]map[string]string, error) {
	insertMapContentsToDb(sqliteDb, objs.XdpStatsMap)
	var icmpEvents []map[string]string
	var icmpEvent map[string]string

	row, err := sqliteDb.Query("SELECT * FROM icmp_events ORDER BY id")
	if err != nil {
		log.Fatal(err)
	}
	defer row.Close()

	for row.Next() {
		var (
			id       int
			sourceIP string
			seqNo    int
			msgType  int
			ttl      int
		)
		row.Scan(&id, &sourceIP, &seqNo, &msgType, &ttl)
		icmpEvent = map[string]string{
			"source_ip": sourceIP,
			"seq_no":    strconv.Itoa(seqNo),
			"type":      strconv.Itoa(msgType),
			"ttl":       strconv.Itoa(ttl),
		}
		icmpEvents = append(icmpEvents, icmpEvent)
	}

	return icmpEvents, nil
}
