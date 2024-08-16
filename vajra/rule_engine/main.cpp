#include "../3rdparty/json.hpp"
#include "../common/ConnectionPool.h"
#include "../common/ConfigurationParser.h"
#include "RuleEngine.h"
#include "PluginLoader.h"

#include <pqxx/pqxx>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

int main()
{
    PluginLoader loader("");
    //WinPluginLoader win_loader("");
    std::vector<std::string> loaded_plugins = loader.loaded_plugins();
    std::vector<std::string> failed_plugins = loader.failed_plugins();
    //std::vector<std::string> win_loaded_plugins = win_loader.loaded_plugins();
    //std::vector<std::string> win_failed_plugins = win_loader.failed_plugins();

    ConfigurationParser config_parser("config.json");
    if (!config_parser.parse())
    {
        return -1;
    }

    FleetServerConfiguration fleet = config_parser.GetFleetConfiguration();
    std::shared_ptr<ConnectionPool> connection_pool(new ConnectionPool(fleet.database.ConnectionString(), fleet.database_pool.maximum_connections));

    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>> process_event_rules;
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const WinProcessEvent&, Event&)>>> win_process_event_rules;

    for (const auto& plugin_name : loaded_plugins)
    {
        auto rules = loader.ProcessEventRules(plugin_name);
        process_event_rules.insert(process_event_rules.end(), rules.begin(), rules.end());

    }

    // for (const auto& win_plugin_name : win_loaded_plugins)
    // {
    //     auto win_rules = win_loader.WinProcessEventRules(win_plugin_name);
    //     win_process_event_rules.insert(win_process_event_rules.end(), win_rules.begin(), win_rules.end());
    // }


    RuleEngine engine(connection_pool, process_event_rules, win_process_event_rules);
    //engine.run_bpf();
    //engine.run_bpf_file(); 
    engine.run(); 

    return 0;
}