//go:build ignore

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

char __license[] SEC("license") = "Dual MIT/GPL";

#define MAX_ENTRIES 10240

struct trace_event_sys_enter_execve {
    u64 __unused1;
    s32 syscall_nr;
    u32 __unused2;
    const char *filename;
    const char *const *argv;
    const char *const *envp;
};

struct trace_event_sys_exit_execve {
    u64 __unused1;
    s32 syscall_nr;
    u32 __unused2;
    s64 ret;
};

struct event {
    pid_t pid;
    pid_t tid;
    pid_t ppid;
    uid_t uid;
    gid_t gid;
    u64 cid;
    long retcode;
    u8 filename[100];
    u64 duration;
    u64 ntime;
};


struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, MAX_ENTRIES);
	__type(key, pid_t);
	__type(value, struct event);
} execs SEC(".maps");

struct {
        __uint(type, BPF_MAP_TYPE_QUEUE);
        __uint(max_entries, MAX_ENTRIES);
        __type(value, struct event);
} exec_events SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_execve")
int tracepoint_syscalls__sys_enter_execve(struct trace_event_sys_enter_execve *ctx) {
    u64 id;
    pid_t pid, tgid;
    struct event *event;
    struct task_struct *task, *parent_task;

    id = bpf_get_current_uid_gid();
    uid_t uid = (uid_t)id;
    gid_t gid = id >> 32;

    id = bpf_get_current_pid_tgid();
    pid = (pid_t)id;
    tgid = id >> 32;
    

    struct event empty_event = {};
    if (bpf_map_update_elem(&execs, &pid, &empty_event, BPF_NOEXIST) != 0) {
        return 0;
    }
    event = bpf_map_lookup_elem(&execs, &pid);
    if (!event) {
        return 0;
    }

    event->pid = tgid;
    event->tid = pid;
    event->uid = uid;
    event->gid = gid;
    event->cid = bpf_get_current_cgroup_id();
    event->ntime = bpf_ktime_get_ns();
    bpf_probe_read_str(event->filename, sizeof(event->filename), ctx->filename);

    task = (struct task_struct *)bpf_get_current_task();
    bpf_probe_read(&parent_task, sizeof(struct task_struct *), &task->real_parent);
    bpf_probe_read(&event->ppid, sizeof(pid_t), &parent_task->tgid);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_execve")
int tracepoint_syscalls__sys_exit_execve(struct trace_event_sys_exit_execve *ctx) {
    pid_t pid;
    struct event *event;

    pid = (pid_t)bpf_get_current_pid_tgid();
    event = bpf_map_lookup_elem(&execs, &pid);
    if (!event) {
        return 0;
    }
    if (ctx->ret < 0) {
        goto cleanup;
    }

    u64 ntime_now = bpf_ktime_get_ns();
    event->duration = ntime_now - event->ntime;
    event->retcode = ctx->ret;
    bpf_map_push_elem(&exec_events, event, BPF_EXIST);

cleanup:
    bpf_map_delete_elem(&execs, &pid);
    return 0;
}