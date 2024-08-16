
#ifndef FLEET_RULEENGINE_H
#define FLEET_RULEENGINE_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../3rdparty/json.hpp"
#include "../common/EdrDataTypes.h"
#include "../common/ConnectionPool.h"

class RuleEngine {
private:
    std::shared_ptr<ConnectionPool> pool;
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>> process_event_rules;
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const WinProcessEvent&, Event&)>>> win_process_event_rules;
public:
    RuleEngine(std::shared_ptr<ConnectionPool>, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>>, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const WinProcessEvent&, Event&)>>>);

    void run();
    [[noreturn]] void run_audit();
    [[noreturn]] void run_audit_socket();
    [[noreturn]] void run_bpf();
    [[noreturn]] void run_bpf_file();
    [[noreturn]] void run_bpf_socket();
    [[noreturn]] void run_win();
    [[noreturn]] void run_win_socket();
    [[noreturn]] void run_win_file();
    [[noreturn]] void run_chrome_ext();
    [[noreturn]] void run_file();
    [[noreturn]] void run_processes();
    [[noreturn]] void run_bpf_process_tree();
};


#endif //FLEET_RULEENGINE_H
