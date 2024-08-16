#ifndef FLEET_RULE_BUILDER_RULES_H
#define FLEET_RULE_BUILDER_RULES_H

#include "../http_common.h"
#include "../../common/ConnectionPool.h"

#include <memory>

HTTPMessage rule_builder_rule_add(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage rule_builder_rule_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage rule_builder_rule_update(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage rule_builder_rule_delete(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
// HTTPMessage tables_builder_rule_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
#endif //FLEET_RULE_BUILDER_RULES_H