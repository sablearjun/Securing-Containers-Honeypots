#ifndef FLEET_PLUGIN_BASE_H
#define FLEET_PLUGIN_BASE_H

#include "EdrDataTypes.h"

#include <functional>
#include <string>
#include <tuple>
#include <vector>

struct PluginInfo
{
    std::string name;
    std::string publisher;
    unsigned int version_major, version_minor, version_revision;
    std::string version_string;

    PluginInfo()
    {
        version_major = 0;
        version_minor = 0;
        version_revision = 0;
    }
};

struct WinPluginInfo
{
    std::string name;
    std::string publisher;
    unsigned int version_major, version_minor, version_revision;
    std::string version_string;

    WinPluginInfo()
    {
        version_major = 0;
        version_minor = 0;
        version_revision = 0;
    }
};

class AbstractDetectionPlugin
{
public:
    virtual ~AbstractDetectionPlugin() {}
    virtual int load() = 0;
    virtual int unload() = 0;
    virtual PluginInfo version() = 0;
    virtual std::vector<std::tuple<std::string,std::string,std::string,std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>> process_event_rules() = 0;
    //virtual std::vector<std::tuple<std::string,std::string,std::string,std::string, SeverityLevel, bool, std::function<bool(const WinProcessEvent&, Event&)>>> win_process_event_rules() = 0;

};

class WinAbstractDetectionPlugin
{
public:
    virtual ~WinAbstractDetectionPlugin() {}
    virtual int load() = 0;
    virtual int unload() = 0;
    virtual WinPluginInfo version() = 0;
    //virtual std::vector<std::tuple<std::string,std::string,std::string,std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent&, Event&)>>> process_event_rules() = 0;
    virtual std::vector<std::tuple<std::string,std::string,std::string,std::string, std::string, SeverityLevel, bool, std::function<bool(const WinProcessEvent&, Event&)>>> win_process_event_rules() = 0;

};

class AbstractBlockingPlugin
{
public:
    virtual ~AbstractBlockingPlugin() {}
    virtual int load() = 0;
    virtual int unload() = 0;
    virtual PluginInfo version() = 0;
    virtual std::vector<std::tuple<std::string,std::string,std::string,std::string, std::string, RemediationPolicy, std::function<bool(const BlockingEvent)>>> process_blocking_rules() = 0;
};

#endif //FLEET_PLUGIN_BASE_H
