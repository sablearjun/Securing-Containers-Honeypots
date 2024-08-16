
#ifndef FLEET_DASHBOARD_H
#define FLEET_DASHBOARD_H

#include "../http_common.h"
#include "../../common/ConnectionPool.h"

#include <memory>

HTTPMessage nodes_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage events_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage os_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage weekly_alerts_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage severity_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage alert_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage high_risk_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage node_alert_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage mitre_analysis(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);

#endif //FLEET_DASHBOARD_H
