#ifndef FLEET_SCHEDULED_QUERY_H
#define FLEET_SCHEDULED_QUERY_H

#include "../http_common.h"
#include "../../common/ConnectionPool.h"

HTTPMessage schedule_query_send(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage schedule_query_status(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage schedule_query_response(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);

#endif //FLEET_SCHEDULED_QUERY_H
