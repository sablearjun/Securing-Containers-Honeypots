
#ifndef FLEET_STATS_H
#define FLEET_STATS_H

#include "../http_common.h"
#include "../../common/ConnectionPool.h"

#include <memory>

HTTPMessage stat_nodes_active_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage stat_nodes_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage node_rule_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage max_severity_node_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);


#endif //FLEET_STATS_H