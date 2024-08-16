#include "vmlinux.h"
    #include <bpf/bpf_helpers.h>
#define MAX_SIZE 128
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 10240);
    __type(key, u32);
    __type(value, char[MAX_SIZE]);
} pid_cgroup_name SEC(".maps");
static __always_inline
int trace_enqueue(struct task_struct *task)
{
    u32 pid;

    struct css_set *cgroups;
    struct cgroup_subsys_state *subsys[14];
    struct cgroup *cg;
    struct kernfs_node *kn;
    
    // char cgroup_name[MAX_SIZE];
    long name_len;
    
    bpf_probe_read(&cgroups, sizeof(cgroups), &task->cgroups);
    bpf_probe_read(&subsys, sizeof(subsys), &cgroups->subsys);
    bpf_probe_read(&cg, sizeof(cg), &subsys[1]->cgroup);
    bpf_probe_read(&kn, sizeof(kn), &cg->kn);
    const char *name;
    name_len = bpf_probe_read(&name, sizeof(name), &kn->name);
    //bpf_printk("name_len %ld\n", name_len);
    if (name_len < 0)
        return 0;

    char cgroup_name[MAX_SIZE];
    bpf_probe_read_str(cgroup_name, MAX_SIZE, name);
    bpf_printk("cgroup_name: %c\n", cgroup_name[1]);
    bpf_probe_read(&pid, sizeof(pid), &task->tgid);

    long err = bpf_map_update_elem(&pid_cgroup_name, &pid, &cgroup_name, BPF_ANY);
    if (err) {
        bpf_printk("update elem failed %ld\n", err);
    }
    return 0;
}

    SEC("tp/sched/sched_wakeup")
    int handle__sched_wakeup(struct trace_event_raw_cgroup_event *ctx)
    {
        bpf_printk("sched_wakeup\n");
        struct task_struct *task = (void *)bpf_get_current_task();

        return trace_enqueue(task);
    }

    // liscence
    char _license[] SEC("license") = "GPL";
