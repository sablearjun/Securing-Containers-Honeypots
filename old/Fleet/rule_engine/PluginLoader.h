#ifndef FLEET_PLUGINLOADER_H
#define FLEET_PLUGINLOADER_H

#include "../common/EdrDataTypes.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

class PluginLoader
{
private:
    std::string plugin_root;
    std::vector<std::string> loaded_plugin_list;
    std::vector<void*> handles;
    std::vector<void*> plugins;
    std::vector<std::string> failed_plugin_list;

    std::unordered_map<std::string, std::vector<std::tuple<std::string,std::string,std::string,std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>>> event_processors;
    void load();
    void unload();
public:
    PluginLoader(std::string path);
    ~PluginLoader();
    std::vector<std::string> loaded_plugins();
    std::vector<std::string> failed_plugins();
    std::vector<std::tuple<std::string,std::string,std::string,std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>> ProcessEventRules(std::string plugin_name);
};

#endif //FLEET_PLUGINLOADER_H
