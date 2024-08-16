
#ifndef FLEET_EVENTPROCESSOR_H
#define FLEET_EVENTPROCESSOR_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../common/ConnectionPool.h"
#include "../common/EdrDataTypes.h"

class EventProcessor {
private:
    std::shared_ptr<ConnectionPool> pool;
    std::function<std::vector<ProcessEntry>(std::shared_ptr<pqxx::connection>, unsigned long long)> process_event_processor,
        socket_event_processor, user_event_processor;
    std::function<std::vector<BpfProcessEntry>(std::shared_ptr<pqxx::connection>, unsigned long long)> bpf_process_event_processor, bpf_socket_event_processor, bpf_file_event_processor;
    std::function<std::vector<WinProcessEntry>(std::shared_ptr<pqxx::connection>, unsigned long long)> win_process_event_processor, win_socket_event_processor, win_file_event_processor;
    [[noreturn]] void ProcessEventProcessor();
    [[noreturn]] void SocketEventProcessor();
    [[noreturn]] void UserEventProcessor();
    [[noreturn]] void BPFProcessEventProcessor();
    [[noreturn]] void BPFSocketEventProcessor();
    [[noreturn]] void BPFFileEventProcessor();
    [[noreturn]] void WinProcessEventProcessor();
    [[noreturn]] void WinSocketEventProcessor();
    [[noreturn]] void WinFileEventProcessor();
public:
    explicit EventProcessor(std::shared_ptr<ConnectionPool>);
    void setProcessEventProcessor(std::function<std::vector<ProcessEntry>(const std::shared_ptr<pqxx::connection>&, unsigned long long)>);
    void setSocketEventProcessor(std::function<std::vector<ProcessEntry>(const std::shared_ptr<pqxx::connection>&, unsigned long long)>);
    void setUserEventProcessor(std::function<std::vector<ProcessEntry>(const std::shared_ptr<pqxx::connection>&, unsigned long long)>);
    void setBPFProcessEventProcessor(std::function<std::vector<BpfProcessEntry>(const std::shared_ptr<pqxx::connection>&, unsigned long long)>);
    void setBPFSocketEventProcessor(std::function<std::vector<BpfProcessEntry>(const std::shared_ptr<pqxx::connection>&, unsigned long long)>);
    void setBPFFileEventProcessor(std::function<std::vector<BpfProcessEntry>(const std::shared_ptr<pqxx::connection>&, unsigned long long)>);
    void setWinProcessEventProcessor(std::function<std::vector<WinProcessEntry>(const std::shared_ptr<pqxx::connection>&, unsigned long long)>);
    void setWinSocketEventProcessor(std::function<std::vector<WinProcessEntry>(const std::shared_ptr<pqxx::connection>&, unsigned long long)>);
    void setWinFileEventProcessor(std::function<std::vector<WinProcessEntry>(const std::shared_ptr<pqxx::connection>&, unsigned long long)>);

    void run();
};


#endif //FLEET_EVENTPROCESSOR_H
