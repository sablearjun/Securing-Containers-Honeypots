#ifndef FLEET_EDRDATATYPES_H
#define FLEET_EDRDATATYPES_H

#include "../3rdparty/json.hpp"

#include <string>
#include <vector>

struct ProcessEntry
{
    long pid, ppid, parent;
    long long event_time, user_id, effective_user_id, group_id, event_id, effective_group_id, owner_user_id, owner_group_id, protocol, local_port, remote_port;
    std::string path, command_line, current_working_directory, syscall, uptime, file_path, flags, local_address, remote_address;

    ProcessEntry();
};

struct BpfProcessEntry
{
    long pid, ppid;
    long long tid, uid, gid, cid, probe_error, duration, time, eid, is_container_process, container_age, family, type, protocol, local_port, remote_port, timestamp, uptime, parent;
    std::string command_line, exit_code, current_working_directory, syscall, path, cwd, cmdline, md5, sha1, sha256,
        container_name, ntime, json_cmdline, fd, local_address, remote_address, file_path, hostname, flags, host_identifier, event_type;

    BpfProcessEntry();
};

struct BpfSocketEntry
{
    long pid, ppid;
    long long tid, uid, gid, cid, duration, protocol, local_port, remote_port, timestamp, uptime, parent;
    std::string exit_code, syscall, path, local_address, remote_address, hostname;

    BpfSocketEntry();
};

struct BpfFileEntry
{
    long pid, ppid;
    long long tid, uid, gid, cid, duration, protocol, local_port, remote_port, timestamp, uptime, parent;
    std::string exit_code, syscall, path, local_address, remote_address, host_identifier, hostname, flags, file_path;

    BpfFileEntry();
};

struct ProcessEvent
{
    ProcessEntry entry;
    std::vector<ProcessEntry> parents;

    ProcessEvent();
};

enum FileAccessPermissionType
{
    OPEN_PERMISSION,
    EXEC_PERMISSION
};

struct BlockingEvent
{
    int pid, fd;
    std::string file_path;
    FileAccessPermissionType permission_requested;
    ProcessEntry entry;
    std::vector<ProcessEntry> parents;

    BlockingEvent();
};

struct BlockingResponse
{
    BlockingEvent event;
    bool wasBlocked;
    std::string reason;
    std::string metadata;

    BlockingResponse();
};

enum SeverityLevel
{
    None,
    Information,
    Low,
    Medium,
    High
};

enum RemediationPolicy
{
    Allow,
    Deny,
    Log
};

struct Event
{
    std::string code;
    std::string metadata;
    SeverityLevel severity;
    bool isAlert;

    Event();
};

inline BlockingResponse::BlockingResponse()
{
    wasBlocked = false;
}

inline BlockingEvent::BlockingEvent()
{
    permission_requested = FileAccessPermissionType::OPEN_PERMISSION;
    pid = 0;
    fd = -1;
}

inline ProcessEvent::ProcessEvent()
{
    entry.pid = 0;
    entry.ppid = 0;
    entry.path = "";
    entry.command_line = "";
    entry.current_working_directory = "";
    entry.syscall = "";
    entry.parent = 0;
}

inline ProcessEntry::ProcessEntry()
{
    pid = 0;
    ppid = 0;
    parent = 0;
    event_time = -1;
    user_id = -1;
    group_id = -1;
    effective_group_id = -1;
    effective_user_id = -1;
    owner_user_id = -1;
    owner_group_id = -1;
    path = "";
    command_line = "";
    current_working_directory = "";
    syscall = "";
    event_id = -1;
    file_path = "";
    flags = "";
}

inline BpfProcessEntry::BpfProcessEntry()
{
    pid = 0;
    ppid = 0;
    tid = -1,
    uid = -1,
    gid = -1,
    cid = -1,
    probe_error = -1,
    duration = -1,
    time = -1,
    eid = -1,
    is_container_process = -1,
    container_age = -1,
    path = "",
    command_line = "",
    exit_code = "",
    current_working_directory = "",
    syscall = "",
    path = "",
    cwd = "",
    cmdline = "",
    md5 = "",
    sha1 = "",
    sha256 = "",
    container_name = "",
    ntime = "",
    json_cmdline = "",
    file_path = "",
    hostname ="",
    flags = "",
    event_type = "";
}

inline BpfSocketEntry::BpfSocketEntry()
{
    pid = 0;
    ppid = 0;
    parent = 0;
    tid = -1,   
    uid = -1,
    gid = -1,
    cid = -1,
    duration = -1, 
    protocol = -1,
    local_port = -1,
    remote_port = -1,
    timestamp = -1,
    uptime = -1,
    duration = -1,
    path = "",
    exit_code = "",
    syscall = "",
    path = "",
    remote_address = "",
    local_address = "";
}

inline BpfFileEntry::BpfFileEntry()
{
    pid = 0;
    ppid = 0;
    parent = 0;
    tid = -1,
    uid = -1,
    gid = -1,
    cid = -1,
    duration = -1, 
    protocol = -1,
    timestamp = -1,
    uptime = -1,
    duration = -1,
    path = "",
    exit_code = "",
    syscall = "",
    path = "",
    flags = "",
    file_path = "";

}

inline Event::Event()
{
    this->severity = SeverityLevel::None;
    this->isAlert = false;
}

inline nlohmann::json ProcessEntryListToJson(std::vector<ProcessEntry> list)
{
    nlohmann::json j = nlohmann::json::array();

    for (auto &entry : list)
    {
        nlohmann::json obj;
        obj["pid"] = entry.pid;
        obj["path"] = entry.path;
        obj["command_line"] = entry.command_line;
        obj["cwd"] = entry.current_working_directory;
        obj["ppid"] = entry.ppid;
        obj["parent"] = entry.parent;
        obj["syscall"] = entry.syscall;
        obj["event_time"] = entry.event_time;
        obj["user_id"] = entry.user_id;
        obj["group_id"] = entry.group_id;
        obj["effective_gid"] = entry.effective_group_id;
        obj["effective_uid"] = entry.effective_user_id;
        obj["owner_uid"] = entry.owner_user_id;
        obj["owner_gid"] = entry.owner_group_id;

        j.push_back(obj);
    }

    return j;
}

inline nlohmann::json BPFProcessEntryListToJson(std::vector<BpfProcessEntry> list)
{
    nlohmann::json j = nlohmann::json::array();

    for (auto &entry : list)
    {
        nlohmann::json obj;
        obj["pid"] = entry.pid;
        obj["parent"] = entry.ppid;
        obj["tid"] = entry.tid;
        obj["uid"] = entry.uid;
        obj["gid"] = entry.gid;
        obj["cid"] = entry.cid;
        obj["exit_code"] = entry.exit_code;
        obj["probe_error"] = entry.probe_error;
        obj["syscall"] = entry.syscall;
        obj["path"] = entry.path;
        obj["cwd"] = entry.cwd;
        obj["cmdline"] = entry.cmdline;
        obj["duration"] = entry.duration;
        obj["json_cmdline"] = entry.json_cmdline;
        obj["ntime"] = entry.ntime;
        obj["time"] = entry.time;
        obj["eid"] = entry.eid;
        obj["md5"] = entry.md5;
        obj["sha1"] = entry.sha1;
        obj["sha256"] = entry.sha256;
        obj["is_container_process"] = entry.is_container_process;
        obj["container_age"] = entry.container_age;
        obj["container_name"] = entry.container_name;
        obj["fd"] = entry.fd;
        obj["family"] = entry.family;
        obj["protocol"] = entry.protocol;
        obj["local_address"] = entry.local_address;
        obj["remote_address"] = entry.remote_address;
        obj["local_port"] = entry.local_port;
        obj["remote_port"] = entry.remote_port;
        obj["uptime"] = entry.uptime;
        obj["file_path"] = entry.file_path;

        j.push_back(obj);
    }

    return j;
}

inline std::vector<ProcessEntry> JsonToProcessEntryList(nlohmann::json process_tree)
{
    std::vector<ProcessEntry> process_list;

    if (process_tree.is_array())
    {
        for (auto &process : process_tree)
        {
            ProcessEntry entry;
            entry.pid = process.contains("pid") ? process["pid"].get<long>() : 0;
            entry.ppid = process.contains("ppid") ? process["ppid"].get<long>() : 0;
            entry.parent = process.contains("parent") ? process["parent"].get<long>() : 0;
            entry.event_time = process.contains("event_time") ? process["event_time"].get<long long>() : -1;
            entry.user_id = process.contains("user_id") ? process["user_id"].get<long long>() : -1;
            entry.group_id = process.contains("group_id") ? process["group_id"].get<long long>() : -1;
            entry.effective_group_id = process.contains("effective_gid") ? process["effective_gid"].get<long long>() : -1;
            entry.effective_user_id = process.contains("effective_uid") ? process["effective_uid"].get<long long>() : -1;
            entry.owner_user_id = process.contains("owner_uid") ? process["owner_uid"].get<long long>() : -1;
            entry.owner_group_id = process.contains("owner_gid") ? process["owner_gid"].get<long long>() : -1;
            entry.path = process.contains("path") ? process["path"].get<std::string>() : "";
            entry.command_line = process.contains("command_line") ? process["command_line"].get<std::string>() : "";
            entry.current_working_directory = process.contains("cwd") ? process["cwd"].get<std::string>() : "";
            entry.syscall = process.contains("syscall") ? process["syscall"].get<std::string>() : "";

            process_list.push_back(entry);
        }
    }

    return process_list;
}

#endif // FLEET_EDRDATATYPES_H
