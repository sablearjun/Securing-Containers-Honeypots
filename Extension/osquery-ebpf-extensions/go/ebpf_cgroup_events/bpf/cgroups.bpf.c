//go:build ignore

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
 #include <bpf/bpf_core_read.h>

char __license[] SEC("license") = "Dual MIT/GPL";

#define MAX_ENTRIES 102

// struct event {
//     u64 cid;
//     char cgroup_path[100];
// };


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, MAX_ENTRIES);
    __type(key, u64);
	__type(value, char[400]);
} cgroup_map SEC(".maps");

// struct trace_event_cgroup_mkdir
// {
//     u64 unused;
//     int root;
//     int level;
//     u64 id;
//     u32 data;
// };


SEC("tracepoint/cgroup/cgroup_mkdir")
int tracepoint_cgroup__cgroup_mkdir(struct trace_event_raw_cgroup_event *ctx) {

    char cgroup_path[400];
    bpf_printk("cgroup_mkdir: %d\n", ctx->__data_loc_path);
    bpf_core_read_str(&cgroup_path, 400, &ctx->__data);
    if (cgroup_path[0] == '\0') {
        return 0;
    }

    bpf_printk("cgroup_mkdir: %d , %d\n", cgroup_path[0], cgroup_path[1]);

    u64 id = ctx->id;

    bpf_map_update_elem(&cgroup_map, &id, &cgroup_path, BPF_ANY);


    return 0;
}
