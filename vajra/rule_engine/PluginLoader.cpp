#include "../common/plugin_base.h"
#include "PluginLoader.h"

#include <dlfcn.h>
#include <exception>
#include <functional>
#include <iostream>

typedef void*(*module_init)();
typedef void(*module_exit)(void*);

PluginLoader::PluginLoader(std::string path)
{
    if (path == "") {
        this->plugin_root = "./";
    }
    else {
        this->plugin_root = path;
    }

    this->load();
}

PluginLoader::~PluginLoader()
{
    this->unload();
}

std::vector<std::string> PluginLoader::failed_plugins()
{
    return failed_plugin_list;
}

std::vector<std::string> PluginLoader::loaded_plugins()
{
    return loaded_plugin_list;
}

// std::map<std::string, AbstractDetectionPlugin> PluginLoader::loaded_plugin_map() {
// return plugin_map;
// }

void PluginLoader::load()
{
    for (const auto &entry : std::filesystem::directory_iterator(this->plugin_root))
    {
        if (entry.path().extension() == ".so")
        {
            void* handle = dlopen(entry.path().c_str(), RTLD_LAZY);

            if (!handle)
            {
                std::cerr << "dlerror: " << dlerror() << std::endl;
                failed_plugin_list.push_back(entry.path().string());
                // we do not close handle here, because it was not acquired at first place
                continue;
            }
            else {
                module_init minit = reinterpret_cast<module_init> (dlsym(handle, "module_init"));
                if (!minit)
                {
                    dlclose(handle);
                    failed_plugin_list.push_back(entry.path().string());
                    continue;
                }
                AbstractDetectionPlugin *plugin = reinterpret_cast<AbstractDetectionPlugin*>(minit());
                //WinAbstractDetectionPlugin *plugin_win = reinterpret_cast<WinAbstractDetectionPlugin*>(minit());
                // plugin_win->flag = 1;
                if (plugin->load() == 0)
                {
                    std::string plugin_name = plugin->version().name;
                    //std::string plugin_name_win = plugin_win->version().name;
                    this->event_processors[plugin_name] = plugin->process_event_rules();
                    //this->win_event_processors[plugin_name_win] = plugin_win->win_process_event_rules();

                    loaded_plugin_list.push_back(plugin_name);
                    //loaded_plugin_list.push_back(plugin_name_win);
                    handles.push_back(handle);
                    plugins.push_back(plugin);
                    //plugins.push_back(plugin_win);
                }
                else
                {
                    plugin->unload();
                    //plugin_win->unload();
                    dlclose(handle);
                    failed_plugin_list.push_back(entry.path().string());
                }
            }
        }
    }
}

void PluginLoader::unload()
{
    for (unsigned int i = 0; i < handles.size(); ++i)
    {
        module_exit mexit = reinterpret_cast<module_exit> (dlsym(handles[i], "module_exit"));
        if (mexit)
        {
            AbstractDetectionPlugin *plugin = reinterpret_cast<AbstractDetectionPlugin*>(plugins[i]);
            plugin->unload();
            mexit(plugins[i]);
        }
        dlclose(handles[i]);
    }

    handles.clear();
    plugins.clear();
    loaded_plugin_list.clear();
    failed_plugin_list.clear();
}

std::vector<std::tuple<std::string, std::string,std::string,std::string,std::string,SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>> PluginLoader::ProcessEventRules(std::string plugin_name)
{
    std::vector<std::tuple<std::string,std::string,std::string,std::string,std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>> event_rules;
    if (event_processors.count(plugin_name) != 0)
        event_rules = event_processors[plugin_name];

    return event_rules;
}

// std::vector<std::tuple<std::string, std::string,std::string,std::string,SeverityLevel, bool, std::function<bool(const WinProcessEvent&, Event&)>>> PluginLoader::WinProcessEventRules(std::string plugin_name)
// {
//     std::vector<std::tuple<std::string,std::string,std::string,std::string, SeverityLevel, bool, std::function<bool(const WinProcessEvent&, Event&)>>> win_event_rules;
//     if (win_event_processors.count(plugin_name) != 0)
//         win_event_rules = win_event_processors[plugin_name];

//     return win_event_rules;
// }