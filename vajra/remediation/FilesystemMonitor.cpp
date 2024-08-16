#define _GNU_SOURCE     /* Needed to get O_LARGEFILE definition */
#include <cerrno>
#include <fcntl.h>
#include <climits>
#include <poll.h>
#include <cstdio>
#include <utility>
#include <sys/fanotify.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <thread>

#include <proc/readproc.h>

#include "FilesystemMonitor.h"
#include "RemediationService.h"

#include "../common/EdrDataTypes.h"

PermissionHook __default_hook;

ProcessEntry GetProcessFromPid(long pid)
{
    ProcessEntry entry;
    char procfd_path[PATH_MAX];
    char path[PATH_MAX];
    ssize_t path_len;

    snprintf(procfd_path, sizeof(procfd_path),
             "/proc/%d/exe", pid);
    path_len = readlink(procfd_path, path,
                        sizeof(path) - 1);
    if (path_len == -1) {
        return entry;
    }

    path[path_len] = '\0';
    proc_t process_info;
    memset(&process_info, 0, sizeof(process_info));

    PROCTAB *pt_ptr = openproc(PROC_FILLSTATUS | PROC_PID, &pid);
    if(readproc(pt_ptr, &process_info) != 0)
    {
        entry.pid = pid;
        entry.ppid = process_info.ppid;
        entry.path = std::string(path);

        char **cmd = process_info.cmdline;
        std::string cmdline;
        int index = 0;
        std::string arg;

        while ((cmd != NULL) && (cmd[index] != NULL))
        {
            arg = std::string(cmd[index]);
            cmdline.append(arg);
            cmdline.append(" ");
            ++index;
        }

        entry.cmdline = cmdline;
    }

    closeproc(pt_ptr);
    return entry;
}

std::vector<ProcessEntry> GetParentsFromPid(long pid)
{
    std::vector<ProcessEntry> entries;

    auto entry = GetProcessFromPid(pid);

    while(entry.pid != 1 and entry.pid != 0)
    {
        entry = GetProcessFromPid(entry.ppid);
        entries.push_back(entry);
    }

    return entries;
}

void FilesystemMonitor::handle_events(int fd)
{
    const struct fanotify_event_metadata *metadata;
    struct fanotify_event_metadata buf[200];
    ssize_t len;
    char path[PATH_MAX];
    ssize_t path_len;
    char procfd_path[PATH_MAX];
    struct fanotify_response response;

    for (;;)
    {
        len = read(fd, buf, sizeof(buf));
        if (len == -1 && errno != EAGAIN)
        {
            return;
        }

        if (len <= 0)
            break;
        metadata = buf;

        while (FAN_EVENT_OK(metadata, len))
        {
            if (metadata->vers != FANOTIFY_METADATA_VERSION)
            {
                return;
            }

            if (metadata->fd >= 0)
            {
                if (metadata->mask & FAN_OPEN_EXEC_PERM)
                {
                    snprintf(procfd_path, sizeof(procfd_path),
                             "/proc/self/fd/%d", metadata->fd);
                    path_len = readlink(procfd_path, path,
                                        sizeof(path) - 1);
                    if (path_len == -1) {
                        return;
                    }

                    path[path_len] = '\0';

                    std::string hash = GetIndicatorFromFd(metadata->fd).sha256;

                    switch (remediation_service->decision(hash))
                    {
                        case RemediationPolicy::Allow:
                            response.response = FAN_ALLOW;
                            break;
                        case RemediationPolicy::Deny:
                            response.response = FAN_DENY;
                            break;
                        case RemediationPolicy::Log:
                            response.response = FAN_ALLOW;
                        default:
                            response.response = FAN_DENY;
                    }

                    response.fd = metadata->fd;

                    BlockingEvent block;
                    block.permission_requested = FileAccessPermissionType::EXEC_PERMISSION;
                    block.fd = metadata->fd;
                    block.pid = metadata->pid;
                    block.entry = GetProcessFromPid(block.pid);
                    block.parents = GetParentsFromPid(block.pid);
                    block.file_path = std::string(path);

                    write(fd, &response, sizeof(response));
                }
                if (metadata->mask & FAN_OPEN_PERM)
                {
                    response.response = FAN_ALLOW;
                    response.fd = metadata->fd;

                    std::cerr << "FD access: " << metadata->fd << std::endl;

                    write(fd, &response, sizeof(response));
                }

                close(metadata->fd);
            }

            metadata = FAN_EVENT_NEXT(metadata, len);
        }
    }
}

void FilesystemMonitor::run_monitor()
{
    char buf;
    int fd, poll_num;
    nfds_t nfds;
    struct pollfd fds[2];

    fd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT | FAN_NONBLOCK,O_RDONLY | O_LARGEFILE);
    if (fd == -1) {
        std::cerr << "Fanotify init failed" << std::endl;
        return;
    }

    uint64_t mask = FAN_OPEN_EXEC_PERM;

    if (this->permission_hook.permission_exec && this->permission_hook.permission_open)
    {
        mask = FAN_OPEN_PERM | FAN_OPEN_EXEC_PERM;
    }
    else if (this->permission_hook.permission_open)
    {
        mask = FAN_OPEN_PERM;
    }
    else if (this->permission_hook.permission_exec)
    {
        mask = FAN_OPEN_EXEC_PERM;
    }

    if (fanotify_mark(fd, FAN_MARK_ADD | FAN_MARK_MOUNT, mask, AT_FDCWD,
                      this->path.c_str()) == -1)
    {
        std::cerr << "Fanotify mark failed" << std::endl;
        return;
    }

    nfds = 2;

    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    fds[1].fd = fd;
    fds[1].events = POLLIN;

    while (true) {
        poll_num = poll(fds, nfds, -1);
        if (poll_num == -1)
        {
            if (errno == EINTR)
                continue;
            std::cerr << "Polling failed" << std::endl;
            return;
        }

        if (poll_num > 0) {
            if (fds[0].revents & POLLIN)
            {
                while (read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n')
                    continue;
                std::cerr << "Fanotify read failed" << std::endl;
                break;
            }

            if (fds[1].revents & POLLIN)
            {
                handle_events(fd);
            }
        }
    }
}

void FilesystemMonitor::run(bool isAsync)
{
    monitor = new std::thread([this] {this->run_monitor();});
    if (!isAsync)
        monitor->join();
}

FilesystemMonitor::FilesystemMonitor(std::shared_ptr<RemediationService> remediation, std::string mount, PermissionHook hook)
{
    this->path = std::move(mount);
    this->remediation_service = std::move(remediation);
    this->permission_hook = std::move(hook);
}

FilesystemMonitor::~FilesystemMonitor()
{
}

PermissionHook::PermissionHook()
{
    this->permission_exec = true;
    this->permission_open = false;
}