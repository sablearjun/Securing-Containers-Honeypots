#include<boost/algorithm/string.hpp>
#include <fstream>
#include "OsqueryConfig.h"

OsqueryConfig::OsqueryConfig(std::string path)
{
    this->config_path = path;
    reload();
}

void OsqueryConfig::reload()
{
    std::ifstream file;
    file.open(this->config_path);

    std::string line;

    if (!file.is_open())
        return;

    while (std::getline(file, line))
    {
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of("="));

        if (tokens.size() < 2)
            break;

        std::string key = tokens[0];
        std::string value;

        for (int i = 1; i < tokens.size(); ++i)
        {
            value.append(tokens[i]);
        }

        this->configuration[key] = value;
    }

    file.close();
}

std::string OsqueryConfig::GetConfig(std::string key)
{
    return configuration[std::string("--") + key];
}