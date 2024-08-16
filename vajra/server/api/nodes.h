
#ifndef FLEET_NODES_H
#define FLEET_NODES_H

#include "../http_common.h"
#include "../../common/ConnectionPool.h"

#include <memory>

HTTPMessage nodes_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage nodes_query(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage nodes_reenroll(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage nodes_delete(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage nodes_active_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage nodes_config(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage node_health(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage node_memory_health(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage historical_data(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage nodes_ip(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage node_log_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);


#endif //FLEET_NODES_H
