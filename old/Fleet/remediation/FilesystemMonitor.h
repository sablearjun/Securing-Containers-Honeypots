#ifndef FLEET_FILESYSTEMMONITOR_H
#define FLEET_FILESYSTEMMONITOR_H

#include <memory>
#include <mutex>
#include <deque>
#include <string>
#include <thread>
#include <vector>

#include "RemediationService.h"
#include "HTTPClient.h"

struct PermissionHook
{
    bool permission_exec;
    bool permission_open;

    PermissionHook();
};

extern PermissionHook __default_hook;

class CommunicationService
{
private:
    HTTPClient client;
    std::string remote_host;
    unsigned int batch_size;
    std::deque<BlockingResponse> message_queue;
    std::mutex message_mutex;
protected:
    [[noreturn]] void SendResponse();
public:
    CommunicationService(const std::string& host);
    void SendResponseAsync(BlockingResponse response);
    void run();
    ~CommunicationService();
};

class FilesystemMonitor {
private:
    std::string path;
    std::shared_ptr<RemediationService> remediation_service;
    std::thread* monitor;
    PermissionHook permission_hook;

    void handle_events(int fd);
    void run_monitor();
public:
    FilesystemMonitor(std::shared_ptr<RemediationService> remediation, std::string mount = "", PermissionHook hook = __default_hook);
    ~FilesystemMonitor();
    void run(bool isAsync = false);
};


#endif //FLEET_FILESYSTEMMONITOR_H
