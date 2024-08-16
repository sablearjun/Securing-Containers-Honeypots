package main

import (
	"C"
	"log"
	"time"

	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
	"golang.org/x/sys/unix"
	"unsafe"
)

// $BPF_CLANG and $BPF_CFLAGS are set by the Makefile.
//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang -cflags "-O2 -g -Wall" bpf ./bpf.c -- -I../headers

func main() {
	// Allow the current process to lock memory for eBPF resources.
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatal(err)
	}

	// Load pre-compiled programs and maps into the kernel.
	objs := bpfObjects{}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("loading objects: %v", err)
	}
	defer objs.Close()

	
	tpWakeup, err := link.Tracepoint("sched", "sched_wakeup", objs.HandleSchedWakeup, nil)
	if err != nil {
		log.Fatalf("opening tracepoint: %s", err)
	}
	defer tpWakeup.Close()

	ticker := time.NewTicker(2 * time.Second)
	defer ticker.Stop()

	log.Println("Waiting for events..")

	for range ticker.C {
		// log.Println("tick")
		mapIterator := objs.PidCgroupName.Iterate()
		var pid uint32
		var cgroupName [128]byte
		for mapIterator.Next(&pid, &cgroupName) {
			// log.Println(cgroupName)
			// reverse the byte array i.e change endianess

			myslice := C.GoBytes(unsafe.Pointer(&cgroupName), 128)
			
			cgroupNameStr := unix.ByteSliceToString(myslice[:])
			log.Printf("get pid %v for cgroup name %v", pid, cgroupNameStr)
		}
	}
}