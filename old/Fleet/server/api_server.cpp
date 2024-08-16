#include <sstream>

#include "../common/ConfigurationParser.h"
#include "../common/logger.h"
#include "RequestRouter.h"
#include "WebServer.h"
#include "api/stats.h"
#include "api/nodes.h"
#include "api/events.h"
#include "api/config.h"
#include "api/scheduled_query.h"
#include "api/dashboard.h"

#include "../3rdparty/jwtpp/include/export/jwtpp/jwtpp.hh"

bool authenticator(const std::string& path, HTTPMessage& request, std::shared_ptr<pqxx::connection> connection)
{
    bool result = false;

    if (request.type == RequestType::OPTIONS)
    {
        return true;
    }

    if (request.header.find("Authorization") != request.header.end())
    {
        try
        {
            connection->prepare("get_user_details", "select secret, token, issuer from api_users where username =  $1");
            std::time_t current_time = std::time(0);

            std::string jwt_token = request.header["Authorization"];

            jwtpp::sp_jws jws = jwtpp::jws::parse(jwt_token);
            jwtpp::claims claims = jws->claims();
            std::string i_at = claims.get().iat();
            std::string e_xp = claims.get().exp();
            long iat_long = stol(i_at);
            long exp_long = stol(e_xp);
            if(iat_long > current_time || exp_long < current_time){
                return false;
            }
            std::string user = claims.get().any("user");
            BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Authenticating user: " << user;

            BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "User: " << user << " for resource " << path;

            pqxx::work transaction{*connection};
            pqxx::result query_result = transaction.exec_prepared("get_user_details", user);

            if (query_result.size() == 1)
            {
                auto user_secret = query_result.at(0)["secret"].as<std::string>();
                auto user_token = query_result.at(0)["token"].as<std::string>();
                auto issuer = query_result.at(0)["issuer"].as<std::string>();

                jwtpp::sp_crypto h256 = std::make_shared<jwtpp::hmac>(user_secret.c_str(), jwtpp::alg_t::HS256);
                auto verifier = [issuer, user_token](jwtpp::sp_claims cl) {
                    return cl->check().iss(issuer) && cl->check().any("token", user_token);
                };

                if (jws->verify(h256, verifier))
                    result = true;
                else
                    BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Verification of authorization header for user [" << user << "] is unsuccessful.";
            }

            connection->unprepare("get_user_details");
        }
        catch (pqxx::sql_error const &e)
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
            return result;
        }
        catch (std::exception const &e)
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
            return result;
        }
        catch (...)
        {
            BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not add host into nodes table";
            return result;
        }
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Request for path [" << path << "] does not have any authorization header.";
    }

    return result;
}

std::string vector_to_string(const std::vector<std::string>& values)
{
    std::stringstream ss;

    if (!values.empty())
    {
        ss << values[0];
    }

    if (values.size() > 1)
    {
        for (auto i = 1; i < values.size(); ++i)
        {
            ss << ", " << values[i];
        }
    }

    return ss.str();
}

int main()
{
    std::unordered_map<std::string, std::string> prepared_queries;
    ConfigurationParser config_parser("config.json");
    if (!config_parser.parse())
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Cannot read configuration";
        return -1;
    }

    ApiServerConfiguration api = config_parser.GetApiServerConfiguration();
    LogConfiguration log_config = config_parser.GetLogConfiguration("api");
    AccessControlConfiguration access_control = api.access_control_configuration;

    std::filesystem::path log_path = std::filesystem::path(log_config.log_directory) / log_config.log_file;
    init_logging(log_path.string(), log_config.log_level, log_config.log_size);

    BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Starting connection pool";
    std::shared_ptr<ConnectionPool> connection_pool(
            new ConnectionPool(api.database.ConnectionString(), api.database_pool.maximum_connections, prepared_queries));

    RequestRouter router(default_req_handler,
                         [connection_pool](const std::string& destination, HTTPMessage& request) -> bool
                         {
                            auto conn = connection_pool->GetConnection();
                            bool result = authenticator(destination, request, conn);
                             connection_pool->ReturnConnection(conn);
                            return result;
                         },
                         [access_control](const std::string& destination, HTTPMessage& response) -> bool
                         {
                             if (!response.isRequest)
                             {
                                 response.header["Access-Control-Allow-Origin"] = access_control.allow_origin;

                                 if (!access_control.allow_headers.empty())
                                    response.header["Access-Control-Allow-Headers"] = vector_to_string(access_control.allow_headers);

                                 if (!access_control.expose_headers.empty())
                                    response.header["Access-Control-Expose-Headers"] = vector_to_string(access_control.expose_headers);
                                 response.header["Access-Control-Max-Age"] = std::to_string(access_control.max_age);

                                 if (access_control.allow_credentials)
                                     response.header["Access-Control-Allow-Credentials"] = "true";
                                 else
                                     response.header["Access-Control-Allow-Credentials"] = "false";
                             }
                             return true;
                         });
    router["/event/list"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = event_list(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/event/query"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = event_query(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/event/update"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = event_update(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/event/event_forward_process_tree"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                             {
                                 auto conn = connection_pool->GetConnection();
                                 auto response = event_forward_process_tree(req, conn);
                                 connection_pool->ReturnConnection(conn);
                                 return response;
                             });
    router["/node/list"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = nodes_list(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/node/query"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = nodes_query(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/node/reenroll"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = nodes_reenroll(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/node/health"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = node_health(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/node/memory_health"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = node_memory_health(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/node/historical_data"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = historical_data(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/node/ip"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = nodes_ip(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/stat/active_nodes_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = stat_nodes_active_count(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/node/config"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = nodes_config(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/stat/nodes_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = stat_nodes_count(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/stat/node_rule_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = node_rule_count(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/stat/max_severity_node_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = max_severity_node_count(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/node/delete"].delete_([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = nodes_delete(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/config/list"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = config_list(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/config/table_name"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = table_list(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/config/delete"].delete_([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = config_delete(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/config/update"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = config_update(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/config/add"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                              {
                                  auto conn = connection_pool->GetConnection();
                                  auto response = config_add(req, conn);
                                  connection_pool->ReturnConnection(conn);
                                  return response;
                              });
    router["/scheduled_queries/schedule"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                              {
                                                  auto conn = connection_pool->GetConnection();
                                                  auto response = schedule_query_send(req, conn);
                                                  connection_pool->ReturnConnection(conn);
                                                  return response;
                                              });
    router["/scheduled_queries/status"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                               {
                                                   auto conn = connection_pool->GetConnection();
                                                   auto response = schedule_query_status(req, conn);
                                                   connection_pool->ReturnConnection(conn);
                                                   return response;
                                               });
    router["/scheduled_queries/response"].post([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = schedule_query_response(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });
    router["/dashboard/nodes_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = nodes_count(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });
    router["/dashboard/events_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = events_count(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });
    router["/dashboard/os_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = os_count(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });
    router["/dashboard/weekly_alerts_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = weekly_alerts_count(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });
    router["/dashboard/severity_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = severity_count(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });
    router["/dashboard/mitre_analysis"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = mitre_analysis(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });
    router["/dashboard/alert_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = alert_count(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });
    router["/dashboard/high_risk_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = high_risk_count(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });
    router["/dashboard/node_alert_count"].get([connection_pool](const HTTPMessage& req) -> HTTPMessage
                                             {
                                                 auto conn = connection_pool->GetConnection();
                                                 auto response = node_alert_count(req, conn);
                                                 connection_pool->ReturnConnection(conn);
                                                 return response;
                                             });

    BOOST_LOG_TRIVIAL(info)<< __SRCREF__ << "Initializing API server";
    WebServer server(router, api.server.hostname, api.server.port);
    server.setTlsCertificates(api.ssl_configuration.ssl_certificate_file, api.ssl_configuration.ssl_private_key_file,
                              api.ssl_configuration.diffie_hellman_key_file, api.ssl_configuration.ssl_password);

    BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "API server initialization complete. Starting server";
    server.run();

    return 0;
}
