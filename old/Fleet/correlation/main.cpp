#include "../common/ConnectionPool.h"
#include "../common/ConfigurationParser.h"
#include "../common/logger.h"

int main()
{
    ConfigurationParser config_parser("config.json");
    if (!config_parser.parse())
    {
        return -1;
    }

    LogConfiguration log_config = config_parser.GetLogConfiguration("correlation");

    std::filesystem::path log_path = std::filesystem::path(log_config.log_directory) / log_config.log_file;
    init_logging(log_path.string(), log_config.log_level, log_config.log_size);

    BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Starting connection pool";

    FleetServerConfiguration fleet = config_parser.GetFleetConfiguration();
    std::shared_ptr<ConnectionPool> connection_pool(new ConnectionPool(fleet.database.ConnectionString(), fleet.database_pool.maximum_connections));
    return 0;
}