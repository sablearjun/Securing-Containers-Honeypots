#ifndef FLEET_CONFIGURATIONPARSER_H
#define FLEET_CONFIGURATIONPARSER_H

#include "../3rdparty/json.hpp"
#include <filesystem>
#include <string>

struct ServerConfiguration
{
    std::string hostname;
    unsigned int port;

    ServerConfiguration();
};

struct LogConfiguration
{
    std::string log_directory;
    std::string log_file;
    char log_level;
    long long log_size;

    LogConfiguration();
};

struct AccessControlConfiguration
{
    std::string allow_origin;
    std::vector<std::string> expose_headers;
    unsigned int max_age;
    bool allow_credentials;
    std::vector<std::string> allow_methods;
    std::vector<std::string> allow_headers;

    AccessControlConfiguration();
};

struct SslConfiguration
{
    std::string ssl_certificate_file;
    std::string ssl_private_key_file;
    std::string ssl_password;
    std::string diffie_hellman_key_file;
};

struct NodeConfiguration
{
    std::string enrollment_secret;
};

struct DatabaseConfiguration
{
    std::string hostname;
    unsigned int port;
    std::string username;
    std::string password;
    std::string database_name;

    std::string ConnectionString();
    DatabaseConfiguration();
};

struct DatabasePoolConfiguration
{
    unsigned int maximum_connections;
    DatabasePoolConfiguration();
};

struct FleetServerConfiguration
{
    ServerConfiguration server;
    NodeConfiguration node;
    DatabaseConfiguration database;
    DatabasePoolConfiguration database_pool;
    SslConfiguration ssl_configuration;
};

struct ApiServerConfiguration
{
    ServerConfiguration server;
    DatabaseConfiguration database;
    DatabasePoolConfiguration database_pool;
    SslConfiguration ssl_configuration;
    AccessControlConfiguration access_control_configuration;
};

class ConfigurationParser {
private:
    std::filesystem::path configuration_file;
    FleetServerConfiguration fleet_server;
    LogConfiguration log_configuration;
    ApiServerConfiguration api_server;

    nlohmann::json config_data;
protected:
    //bool isValid();
    bool parse_fleet_config();
    bool parse_api_config();
    bool parse_log_config();
public:
    bool parse();
    FleetServerConfiguration GetFleetConfiguration();
    ApiServerConfiguration GetApiServerConfiguration();
    LogConfiguration GetLogConfiguration(std::string service_name);
    ConfigurationParser(std::string path);
};


#endif //FLEET_CONFIGURATIONPARSER_H
