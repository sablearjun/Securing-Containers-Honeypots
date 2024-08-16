#ifndef FLEET_REQUEST_HANDLER_H
#define FLEET_REQUEST_HANDLER_H

#include "WebServer.h"

extern std::string enrollment_secret;
HTTPMessage enroll(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage config(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage logger(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage distributed_read(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);
HTTPMessage distributed_write(HTTPMessage request, std::shared_ptr<pqxx::connection> connection);

#endif //FLEET_REQUEST_HANDLER_H
