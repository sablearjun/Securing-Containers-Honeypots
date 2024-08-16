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
    std::vector<std::string> loaded_plugins = loader.loaded_plugins();
    std::vector<std::string> failed_plugins = loader.failed_plugins();

    ConfigurationParser config_parser("config.json");
    if (!config_parser.parse())
    {
        return -1;
    }

    FleetServerConfiguration fleet = config_parser.GetFleetConfiguration();
    std::shared_ptr<ConnectionPool> connection_pool(new ConnectionPool(fleet.database.ConnectionString(), fleet.database_pool.maximum_connections));

    std::vector<std::tuple<std::string, std::string, std::string, std::string,  SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>> process_event_rules;

    for (const auto& plugin_name : loaded_plugins)
    {
        auto rules = loader.ProcessEventRules(plugin_name);
        process_event_rules.insert(process_event_rules.end(), rules.begin(), rules.end());
    }

    RuleEngine engine(connection_pool, process_event_rules);
    //engine.run_bpf();
    //engine.run_bpf_file(); 
    engine.run(); 

    return 0;
}