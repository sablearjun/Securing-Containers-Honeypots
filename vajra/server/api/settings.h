#ifndef FLEET_SETTINGS_H
#define FLEET_SETTINGS_H

#include "../http_common.h"
#include "../../common/ConnectionPool.h"

#include <memory>

HTTPMessage manual_purge(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage data_retention(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage data_retention_days(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
// HTTPMessage cron_job_exists(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);


#endif //FLEET_SETTINGS_H