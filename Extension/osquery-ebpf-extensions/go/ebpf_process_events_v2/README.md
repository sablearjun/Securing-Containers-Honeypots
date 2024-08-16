# eBPF process events v2

eBPF process events v2 is a extension of [osquery](https://osquery.io/) bpf process events table. It adds a column `is_container_process` to the table. The column is set to `1` if the process is running inside a container, otherwise it is set to `0`.

## How to generate vmlinux.h
```bash
bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
```
## How to build

To build the extension, run the following command:

```bash
go generate
cd build
go build -o ebpf_process_events_v2.ext
```

## How to use

Make sure that the executable is owned by root, you can do that by running:

```bash
sudo chown root ebpf_process_events_v2.ext
```

Then, you can load the extension by running:

```bash
sudo osqueryi --disable_events=false --enable_bpf_events=true  --extension ./ebpf_process_events_v2.ext
```

After successfully loading the extension, you can query the table by running:

```sql
select * from ebpf_process_events_v2;
```

## How it works

With this extension we are using the `bpf_process_events` table to get the process events. Then, adding a new column `is_container_process` to the table. The column is set to `1` if the process is running inside a container, otherwise it is set to `0`.

This extension loads eBPF code into kernel to keep track of cgroup id and cgroup name of the processes geting executed. when osquery requests for process events, the extensions gets the `bpf_process_events` table data and gets the cgroup name from the eBPF map. Then, it checks if the cgroup name is a container cgroup name or not by checking if the cgroup name prefix.


## The Idea

One of the approaches to detect whether a process runs in a container (or not) is by making use of the cgroup path of that process. Container management tools like Docker and Podman creates cgroup directories with a specific naming format. We can match the cgroup path we find with this format to classify the process as a container process.

The eBPF kernel space code written in C is responsible for getting the required information to find the cgroup path. The user space code written in Go will use this information to flag a process as container process (or non container process).

## eBPF kernel space code

The ebpf code which runs in the kernel space keeps a `cgroupid` to `cgroup_path_parts` map which will be used by the user space program. The ebpf program traces `sys_enter_execve` and `sys_enter_execve` tracepoints. When a process is executed, the corresponding handler function for the syscall runs and updates the map. Cgroup id and cgroup path parts can be found out by making use of bpf helper functions and by reading nested kernel structures. 

## User space code

User space code in our case is osquery extension written in Go.
On initialization, the extension will load the eBPF code into the kernel.

The extension will get the process events from the `bpf_process_events` table and then get the cgroup path from the eBPF map. Then, it will check if the cgroup path is a container cgroup path or not by checking if the cgroup path prefix matches with the container cgroup path prefix.

