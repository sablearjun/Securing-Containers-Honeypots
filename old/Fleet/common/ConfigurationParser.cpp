#include "ConfigurationParser.h"
#include "../3rdparty/json.hpp"

#include <boost/algorithm/string.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

DatabaseConfiguration::DatabaseConfiguration() {
    this->port = 0;
}

std::string DatabaseConfiguration::ConnectionString() {
    std::stringstream connection_string;

    connection_string << "postgresql://" << this->username;

    if (this->password != "") {
        connection_string << ":" << this->password;
    }

    connection_string << "@" << this->hostname;

    if (this->port > 0)
        connection_string << ":" << std::to_string(this->port);

    connection_string << "/" << this->database_name;

    return connection_string.str();
}

DatabasePoolConfiguration::DatabasePoolConfiguration() {
    this->maximum_connections = 0;
}

ConfigurationParser::ConfigurationParser(std::string path) {
    this->configuration_file = path;
}

ServerConfiguration::ServerConfiguration() {
    this->port = 1234;
}

LogConfiguration::LogConfiguration() {
    this->log_size = -1;
    this->log_level = 3; // default to warning
}

AccessControlConfiguration::AccessControlConfiguration()
{
    this->allow_credentials = true;
    this->max_age = 86400;
    this->allow_origin = "*";
}

bool ConfigurationParser::parse_api_config() {
    nlohmann::json fleet_config;

    if (config_data.contains("api_server"))
    {
        fleet_config = config_data["api_server"];
    }
    else
    {
        std::cerr << "Fleet server configuration parameters are missing in configuration file" << std::endl;
        return false;
    }

    if (fleet_config.contains("server_configuration"))
    {
        auto server_config = fleet_config["server_configuration"];

        if (server_config.contains("hostname"))
        {
            api_server.server.hostname = server_config["hostname"].get<std::string>();
        }
        else
        {
            api_server.server.hostname = "0.0.0.0";
        }

        if (server_config.contains("port"))
        {
            api_server.server.port = server_config["port"].get<unsigned int>();
        }
        else
        {
            api_server.server.port = 1234;
        }

        if (server_config.contains("ssl_certificate"))
        {
            api_server.ssl_configuration.ssl_certificate_file = server_config["ssl_certificate"].get<std::string>();
        }
        if (server_config.contains("ssl_private_key"))
        {
            api_server.ssl_configuration.ssl_private_key_file = server_config["ssl_private_key"].get<std::string>();
        }
        if (server_config.contains("dh_key"))
        {
            api_server.ssl_configuration.diffie_hellman_key_file = server_config["dh_key"].get<std::string>();
        }
        if (server_config.contains("ssl_password"))
        {
            api_server.ssl_configuration.ssl_password = server_config["ssl_password"].get<std::string>();
        }
    }

    if (fleet_config.contains("db_configuration"))
    {
        auto db_config = fleet_config["db_configuration"];
        if (db_config.contains("hostname") && db_config.contains("port") && db_config.contains("username") && db_config.contains("database_name"))
        {
            api_server.database.hostname = db_config["hostname"].get<std::string>();
            api_server.database.username = db_config["username"].get<std::string>();
            api_server.database.database_name = db_config["database_name"].get<std::string>();

            if (db_config.contains("password"))
            {
                api_server.database.password = db_config["password"].get<std::string>();
            }

            if (db_config.contains("port"))
            {
                if (db_config["port"] != "")
                {
                    api_server.database.port = db_config["port"].get<unsigned int>();
                }
                else
                {
                    api_server.database.port = 0;
                }
            }
        }
        else
        {
            std::cerr << "Database connection parameters are missing in configuration file" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "Database connection parameters are missing in configuration file" << std::endl;
        return false;
    }

    if (fleet_config.contains("db_pool_configuration"))
    {
        auto pool_config = fleet_config["db_pool_configuration"];
        if (pool_config.contains("max_connections"))
        {
            if (pool_config["max_connections"] != "")
            {
                api_server.database_pool.maximum_connections = pool_config["max_connections"].get<unsigned int>();
            }
            else
            {
                api_server.database_pool.maximum_connections = 10;
            }
        }
        else
        {
            api_server.database_pool.maximum_connections = 10;
        }
    }

    if (fleet_config.contains("access_control_configuration"))
    {
        auto access_control_config = fleet_config["access_control_configuration"];

        if (access_control_config.contains("allow_origin"))
        {
            api_server.access_control_configuration.allow_origin = access_control_config["allow_origin"].get<std::string>();
        }
        if (access_control_config.contains("allow_methods"))
        {
            auto allowed_methods = access_control_config["allow_methods"];

            if (allowed_methods.is_array())
            {
                for (const auto& method : allowed_methods)
                {
                    api_server.access_control_configuration.allow_methods.push_back(method);
                }
            }
            else
                return false;
        }
        if (access_control_config.contains("allow_headers"))
        {
            auto allowed_headers = access_control_config["allow_headers"];

            if (allowed_headers.is_array())
            {
                for (const auto& method : allowed_headers)
                {
                    api_server.access_control_configuration.allow_headers.push_back(method);
                }
            }
            else
                return false;
        }
        if (access_control_config.contains("allow_credentials"))
        {
            api_server.access_control_configuration.allow_credentials = access_control_config["allow_credentials"].get<bool>();
        }
        if (access_control_config.contains("expose_headers"))
        {
            auto exposed_headers = access_control_config["expose_headers"];

            if (exposed_headers.is_array())
            {
                for (const auto& method : exposed_headers)
                {
                    api_server.access_control_configuration.expose_headers.push_back(method);
                }
            }
            else
                return false;
        }
        if (access_control_config.contains("max_age"))
        {
            api_server.access_control_configuration.max_age = access_control_config["max_age"].get<unsigned int>();
        }
    }

    return true;
}

bool ConfigurationParser::parse_fleet_config() {
    nlohmann::json fleet_config;

    if (config_data.contains("fleet_server"))
    {
        fleet_config = config_data["fleet_server"];
    }
    else
    {
        std::cerr << "Fleet server configuration parameters are missing in configuration file" << std::endl;
        return false;
    }

    if (fleet_config.contains("server_configuration"))
    {
        auto server_config = fleet_config["server_configuration"];

        if (server_config.contains("hostname"))
        {
            fleet_server.server.hostname = server_config["hostname"].get<std::string>();
        }
        else
        {
            fleet_server.server.hostname = "0.0.0.0";
        }

        if (server_config.contains("port"))
        {
            fleet_server.server.port = server_config["port"].get<unsigned int>();
        }
        else
        {
            fleet_server.server.port = 1234;
        }

        if (server_config.contains("ssl_certificate"))
        {
            fleet_server.ssl_configuration.ssl_certificate_file = server_config["ssl_certificate"].get<std::string>();
        }
        if (server_config.contains("ssl_private_key"))
        {
            fleet_server.ssl_configuration.ssl_private_key_file = server_config["ssl_private_key"].get<std::string>();
        }
        if (server_config.contains("dh_key"))
        {
            fleet_server.ssl_configuration.diffie_hellman_key_file = server_config["dh_key"].get<std::string>();
        }
        if (server_config.contains("ssl_password"))
        {
            fleet_server.ssl_configuration.ssl_password = server_config["ssl_password"].get<std::string>();
        }
    }

    if (fleet_config.contains("db_configuration"))
    {
        auto db_config = fleet_config["db_configuration"];
        if (db_config.contains("hostname") && db_config.contains("port") && db_config.contains("username") && db_config.contains("database_name"))
        {
            fleet_server.database.hostname = db_config["hostname"].get<std::string>();
            fleet_server.database.username = db_config["username"].get<std::string>();
            fleet_server.database.database_name = db_config["database_name"].get<std::string>();

            if (db_config.contains("password"))
            {
                fleet_server.database.password = db_config["password"].get<std::string>();
            }

            if (db_config.contains("port"))
            {
                if (db_config["port"] != "")
                {
                    fleet_server.database.port = db_config["port"].get<unsigned int>();
                }
                else
                {
                    fleet_server.database.port = 0;
                }
            }
        }
        else
        {
            std::cerr << "Database connection parameters are missing in configuration file" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "Database connection parameters are missing in configuration file" << std::endl;
        return false;
    }

    if (fleet_config.contains("db_pool_configuration"))
    {
        auto pool_config = fleet_config["db_pool_configuration"];
        if (pool_config.contains("max_connections"))
        {
            if (pool_config["max_connections"] != "")
            {
                fleet_server.database_pool.maximum_connections = pool_config["max_connections"].get<unsigned int>();
            }
            else
            {
                fleet_server.database_pool.maximum_connections = 10;
            }
        }
        else
        {
            fleet_server.database_pool.maximum_connections = 10;
        }
    }
     if(fleet_config.contains("node_configuration"))
    {
         auto node_config = fleet_config["node_configuration"];
        if (node_config.contains("enrollment_secret"))
        {
            fleet_server.node.enrollment_secret = node_config["enrollment_secret"].get<std::string>();  
        }
        else
        {
            std::cerr << "Secret key parameters are missing from configuration." << std::endl;
            return false;
        }
    }
    return true;
}

bool ConfigurationParser::parse_log_config() {
    nlohmann::json log_config;

    if (config_data.contains("log_configuration"))
    {
        log_config = config_data["log_configuration"];

        if (log_config.contains("log_directory"))
        {
            log_configuration.log_directory = log_config["log_directory"].get<std::string>();
        }
        if (log_config.contains("log_file"))
        {
            log_configuration.log_file = log_config["log_file"].get<std::string>();
        }
        if (log_config.contains("log_level"))
        {
            std::string level = log_config["log_level"].get<std::string>();

            if (level == "trace")
                log_configuration.log_level = 0;
            else if (level == "debug")
                log_configuration.log_level = 1;
            else if (level == "info")
                log_configuration.log_level = 2;
            else if (level == "warning")
                log_configuration.log_level = 3;
            else if (level == "error")
                log_configuration.log_level = 4;
            else if (level == "fatal")
                log_configuration.log_level = 5;
            else
            {
                std::cerr<< "Error: unknown log level. Defaulting to \"warning\".";
            }
        }
        if (log_config.contains("max_log_size"))
        {
            log_configuration.log_size = log_config["max_log_size"].get<long long>();
        }
    }

    return true;
}

bool ConfigurationParser::parse() {

    /*
     * Before we even attempt reading data, we need to check the following:
     * 1. that the given path exists
     * 2. that the given path is a regular file (not special files like sockets, device files etc)
     * 3. that the given path does not point to an empty file
     *
     * If given path is a symlink, we resolve the symlink and run both checks on resolved path.
     */

    try
    {
        if (!std::filesystem::exists(this->configuration_file))
        {
            std::cerr << "Error: configuration file does not exist." << std::endl;
            return false;
        }
        if (std::filesystem::is_symlink(this->configuration_file))
        {
            this->configuration_file = std::filesystem::read_symlink(this->configuration_file);

            if (!std::filesystem::exists(this->configuration_file))
            {
                std::cerr << "Error: configuration file is a symlink resolving to non-existing path." << std::endl;
                return false;
            }
        }
        if (!std::filesystem::is_regular_file(this->configuration_file))
        {
            std::cerr << "Error: configuration file is not a regular file." << std::endl;
            return false;
        }
        if (std::filesystem::is_empty(this->configuration_file))
        {
            std::cerr << "Error: configuration file is empty." << std::endl;
            return false;
        }
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cerr << "Error (" << error.code() << ") while validating configuration file path: " << error.what() << std::endl;
        return false;
    }

    std::ifstream config_file(this->configuration_file);
    std::string str((std::istreambuf_iterator<char>(config_file)), std::istreambuf_iterator<char>());

    try {
        this->config_data = nlohmann::json::parse(str);
    }
    catch (const nlohmann::json::parse_error& error)
    {
        std::cerr << "Error: could not parse JSON. Parse error: " << error.what() << std::endl;
        return false;
    }
    catch (const nlohmann::json::type_error& error)
    {
        std::cerr << "Error: invalid type encountered in JSON. Parse error: " << error.what() << std::endl;
        return false;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error: some unknown error occurred. Parse error: " << ex.what() << std::endl;
        return false;
    }

    if (parse_fleet_config() && parse_log_config() && parse_api_config())
        return true;
    else
        return false;
}

FleetServerConfiguration ConfigurationParser::GetFleetConfiguration() {
    return this->fleet_server;
}

LogConfiguration ConfigurationParser::GetLogConfiguration(std::string service_name) {
    boost::replace_all(log_configuration.log_file, "{%service_name%}", service_name);
    return this->log_configuration;
}

ApiServerConfiguration ConfigurationParser::GetApiServerConfiguration() {
    return this->api_server;
}