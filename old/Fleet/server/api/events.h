
#ifndef FLEET_EVENTS_H
#define FLEET_EVENTS_H

#include "../http_common.h"
#include "../../common/ConnectionPool.h"

HTTPMessage event_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage event_query(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage event_update(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage event_forward_process_tree(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);


#endif //FLEET_EVENTS_H
