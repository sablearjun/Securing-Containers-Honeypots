#ifndef FLEET_CONFIG_H
#define FLEET_CONFIG_H

#include "../http_common.h"
#include "../../common/ConnectionPool.h"

#include <memory>

HTTPMessage config_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage table_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage config_delete(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage config_update(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage config_add(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);

#endif //FLEET_CONFIG_H
