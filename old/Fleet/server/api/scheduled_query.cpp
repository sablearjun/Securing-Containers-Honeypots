#include <boost/algorithm/string.hpp>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../../3rdparty/json.hpp"
#include "../../common/logger.h"
#include "scheduled_query.h"

HTTPMessage schedule_query_send(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    std::string query;
    std::vector<std::string> hosts;

    try
    {
        nlohmann::json scheduled_query = nlohmann::json::parse(request.body);

        if (scheduled_query.contains("query"))
            query = scheduled_query["query"];
        if (scheduled_query.contains("hosts"))
        {
            auto host_list = scheduled_query["hosts"];
            if (host_list.is_array())
            {
                for (auto host : host_list)
                {
                    if (host.contains("host_identifier"))
                    {
                        hosts.push_back(host["host_identifier"]);
                    }
                }
            }
        }
    }
    catch (const nlohmann::json::parse_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Query scheduling failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (const nlohmann::json::type_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Query scheduling failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query scheduling request contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Query scheduling failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }

    if (!query.empty() && !hosts.empty())
    {
        std::unordered_map<std::string, unsigned long long> host_query_map;

        try
        {
            pqxx::work transaction{*connection};
            connection->prepare("schedule_query",
                                "insert into query_queue (host_identifier, query, sent) values ($1, $2, $3) returning id");

            for (const auto& host : hosts)
            {
                pqxx::result result = transaction.exec_prepared("schedule_query", host, query, false);
                transaction.commit();

                if (!result.empty())
                {
                    for (unsigned int index = 0; index < result.size(); ++index)
                    {
                        host_query_map[host + ":" + std::to_string(index)] = result.at(index)["id"].as<unsigned long long>();
                    }
                }
            }
            connection->unprepare("schedule_query");

            nlohmann::json message;
            for (auto &it : host_query_map)
            {
                message[it.first] = it.second;
            }
            response.status = boost::beast::http::status::ok;
            response.body = message.dump();
            response.header["Content-Type"] = "application/json";
        }
        catch (pqxx::sql_error const &e)
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
            response.status = boost::beast::http::status::internal_server_error;
            response.body = "{\n"
                            "  \"message\":\"Query scheduling failed due to database error.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        catch (std::exception const &e)
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
            response.status = boost::beast::http::status::internal_server_error;
            response.body = "{\n"
                            "  \"message\":\"Query scheduling failed due to database error.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        catch (...)
        {
            BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not add scheduled query into query_queue";
            response.status = boost::beast::http::status::internal_server_error;
            response.body = "{\n"
                            "  \"message\":\"Query scheduling failed due to database error.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }

    return response;
}

HTTPMessage schedule_query_status(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    std::vector<std::string> config_ids;

    try
    {
        nlohmann::json scheduled_query = nlohmann::json::parse(request.body);

        if (scheduled_query.contains("config_id_list"))
        {
            std::string config_list = scheduled_query["config_id_list"].get<std::string>();
            boost::split(config_ids, config_list, boost::is_any_of(","));
        }
    }
    catch (const nlohmann::json::parse_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Query status check failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (const nlohmann::json::type_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Query status check failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query scheduling request contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Query status check failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }

    if (!config_ids.empty())
    {
        std::unordered_map<unsigned long long, std::tuple<bool, unsigned long long>> query_status_map;
        try
        {
            pqxx::work transaction{*connection};
            connection->prepare("schedule_query_status",
                                "select sent, send_time from query_queue where id = $1");

            for (const auto& id : config_ids)
            {
                unsigned long long __id = std::stoull(id);
                pqxx::result result = transaction.exec_prepared("schedule_query_status", __id);

                if (!result.empty())
                {
                    for (unsigned int index = 0; index < result.size(); ++index)
                    {
                        bool isSent = result.at(index)["sent"].as<bool>();
                        unsigned long long sent_time = 0;
                        if (!result.at(index)["send_time"].is_null())
                        {
                            sent_time = result.at(index)["send_time"].as<unsigned long long>();
                        }

                        query_status_map[__id] = std::tuple<bool, unsigned long long>(isSent, sent_time);
                    }
                }
            }
            connection->unprepare("schedule_query_status");

            nlohmann::json message;
            for (auto &it : query_status_map)
            {
                message[std::to_string(it.first)]["sent"] = std::get<0>(it.second);
                message[std::to_string(it.first)]["sent_time"] = std::get<1>(it.second);
            }
            response.status = boost::beast::http::status::ok;
            response.body = message.dump();
            response.header["Content-Type"] = "application/json";
        }
        catch (pqxx::sql_error const &e)
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
            response.status = boost::beast::http::status::internal_server_error;
            response.body = "{\n"
                            "  \"message\":\"Query status check failed due to database error.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        catch (std::exception const &e)
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
            response.status = boost::beast::http::status::internal_server_error;
            response.body = "{\n"
                            "  \"message\":\"Query status check failed due to database error.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        catch (...)
        {
            BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not add scheduled query into query_queue";
            response.status = boost::beast::http::status::internal_server_error;
            response.body = "{\n"
                            "  \"message\":\"Query status check failed due to database error.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }

    return response;
}

HTTPMessage schedule_query_response(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    std::vector<std::string> config_ids;

    try
    {
        nlohmann::json scheduled_query = nlohmann::json::parse(request.body);

        if (scheduled_query.contains("config_id_list"))
        {
            std::string config_list = scheduled_query["config_id_list"].get<std::string>();
            boost::split(config_ids, config_list, boost::is_any_of(","));
        }
    }
    catch (const nlohmann::json::parse_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Query response fetch failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (const nlohmann::json::type_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Query response fetch failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query scheduling request contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Query response fetch failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }

    if (!config_ids.empty())
    {
        std::unordered_map<std::string, std::string> query_result_map;
        try
        {
            pqxx::work transaction{*connection};
            connection->prepare("get_query_response",
                                "select * from query_result where query_name = $1");

            for (const auto& id : config_ids)
            {
                pqxx::result result = transaction.exec_prepared("get_query_response", id);

                if (!result.empty())
                {
                    auto query_result = result.at(0)["result"].as<std::string>();
                    query_result_map[id] = query_result;
                }
            }
            connection->unprepare("get_query_response");

            nlohmann::json message;
            for (auto &it : query_result_map)
            {
                message[it.first] = it.second;
            }
            response.status = boost::beast::http::status::ok;
            response.body = message.dump();
            response.header["Content-Type"] = "application/json";
        }
        catch (pqxx::sql_error const &e)
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
            response.status = boost::beast::http::status::internal_server_error;
            response.body = "{\n"
                            "  \"message\":\"Query response fetch failed due to database error.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        catch (std::exception const &e)
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
            response.status = boost::beast::http::status::internal_server_error;
            response.body = "{\n"
                            "  \"message\":\"Query response fetch failed due to database error.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        catch (...)
        {
            BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not add scheduled query into query_queue";
            response.status = boost::beast::http::status::internal_server_error;
            response.body = "{\n"
                            "  \"message\":\"Query response fetch failed due to database error.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }

    return response;
}