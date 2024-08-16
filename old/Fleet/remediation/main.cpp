#include <iostream>
#include "HTTPClient.h"
#include "FilesystemMonitor.h"
#include "RemediationService.h"
#include "../common/ThreadPool.h"
#include "OsqueryConfig.h"
#include "PluginLoader.h"

int main()
{
    OsqueryConfig config;

    PluginLoader loader("");
    std::vector<std::string> loaded_plugins = loader.loaded_plugins();

    std::vector<std::tuple<std::string, std::string,std::string,std::string, RemediationPolicy, std::function<bool(const BlockingEvent)>>> process_blocking_rules;

    for (const auto& plugin_name : loaded_plugins)
    {
        auto rules = loader.ProcessBlockingRules(plugin_name);
        process_blocking_rules.insert(process_blocking_rules.end(), rules.begin(), rules.end());
    }

    std::vector<std::string> block_list;
    block_list.push_back("f8b09fba9fda9ffebae86611261cf628bd71022fb4348d876974f7c48ddcc6d5");

    IOCSource source;
    source.indicators = block_list;
    source.policy = RemediationPolicy::Deny;
    source.type = IOCType::Hash;

    std::vector<IOCSource> ioc_source_list;
    ioc_source_list.push_back(source);

    std::shared_ptr<RemediationService> remediationService(new RemediationService(ioc_source_list));

    PermissionHook hook;
    hook.permission_exec = true;
    FilesystemMonitor monitor(remediationService, std::string("/"), hook);
    monitor.run();

    return 0;
}