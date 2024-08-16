#ifndef FLEET_OSQUERYCONFIG_H
#define FLEET_OSQUERYCONFIG_H

#include <string>
#include <unordered_map>

class OsqueryConfig
{
private:
    std::string config_path;
    std::unordered_map<std::string, std::string> configuration;
public:
    OsqueryConfig(std::string path = "/etc/osquery/osquery.flags");
    void reload();
    std::string GetConfig(std::string key);
};


#endif //FLEET_OSQUERYCONFIG_H
