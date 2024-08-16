#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

#define MAX_LEN 128

struct cgroup_path_parts
{
    u8 name[MAX_LEN];
    u8 parent_name[MAX_LEN];
};

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 10240);
    __type(key, u64);
    __type(value, struct cgroup_path_parts);
} cid_cgroup_path_parts SEC(".maps");

static __always_inline int sys_enter_exec_common()
{
    struct task_struct *task = (void *)bpf_get_current_task();

    struct css_set *cgroups;
    struct cgroup_subsys_state *subsys[14];
    struct cgroup *cg;
    struct kernfs_node *kn;
    struct kernfs_node *parent;

    bpf_probe_read(&cgroups, sizeof(cgroups), &task->cgroups);
    if (!cgroups) {
        return 0;
    }

    bpf_probe_read(&subsys, sizeof(subsys), &cgroups->subsys);
    if (!subsys[1]) {
        return 0;
    }

    bpf_probe_read(&cg, sizeof(cg), &subsys[1]->cgroup);
    if (!cg) {
        return 0;
    }

    bpf_probe_read(&kn, sizeof(kn), &cg->kn);
    if (!kn) {
        return 0;
    }

    bpf_probe_read(&parent, sizeof(parent), &kn->parent);
    if (!parent) {
        return 0;
    }

    const char *name, *parent_name;

    bpf_probe_read(&name, sizeof(name), &kn->name);
    if (!name) {
        return 0;
    }

    bpf_probe_read(&parent_name, sizeof(parent_name), &parent->name);
    if (!parent_name) {
        return 0;
    }

    struct cgroup_path_parts cpp;
    if (bpf_probe_read_str(&(cpp.name), sizeof(cpp.name), name) < 0) {
        return 0;
    }

    if (bpf_probe_read_str(&(cpp.parent_name), sizeof(cpp.parent_name), parent_name) < 0) {
        return 0;
    } 

    // get cgroup id using bpf helper
    u64 cid;
    cid = bpf_get_current_cgroup_id();

    // update bpf map with cid as key and cpp (cgroup_path_parts) as value 
    if (bpf_map_update_elem(&cid_cgroup_path_parts, &cid, &cpp, BPF_ANY) < 0) {
        return 0;
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_enter_execve")
int tracepoint__syscalls__sys_enter_execve(struct trace_event_raw_sys_enter *ctx)
{
    return sys_enter_exec_common();
}

SEC("tracepoint/syscalls/sys_enter_execveat")
int tracepoint__syscalls__sys_enter_execveat(struct trace_event_raw_sys_enter *ctx)
{

    return sys_enter_exec_common();
}

// licence
char _license[] SEC("license") = "GPL";
