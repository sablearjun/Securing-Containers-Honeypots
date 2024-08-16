#ifndef FLEET_PYTHON_RULE_H
#define FLEET_PYTHON_RULE_H

#include "../http_common.h"
#include "../../common/ConnectionPool.h"

#include <memory>

HTTPMessage rule_add(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage rule_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage rule_update(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage rule_delete(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage tables_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
#endif //FLEET_PYTHON_RULE_H