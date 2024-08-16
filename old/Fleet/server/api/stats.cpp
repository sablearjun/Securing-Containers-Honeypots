#include "../../3rdparty/json.hpp"
#include "stats.h"
#include "../../common/logger.h"
HTTPMessage stat_nodes_active_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_active_json;
    std::string is_active ;
    std::stringstream ss;

    try
    {
        pqxx::work transaction{*connection};
        pqxx::result result;
        int online = 0, offline = 0;

        connection->prepare("get_nodes_active_count", "select last_seen from nodes");
        result = transaction.exec_prepared("get_nodes_active_count");
        
        for (pqxx::result::const_iterator i = result.begin(); i != result.end(); ++i)
        {
            std::string s_lastseen = i["last_seen"].as<std::string>();
            int lastseen = std::stoi(s_lastseen);
            std::time_t t = std::time(0);
            std::string s_current_time = std::to_string(t);
            int current_time = std::stoi(s_current_time);
            int dif = current_time - lastseen;
            std::string active_status;
            if(dif<300)
            {
                online++;
            }
            else
            {
                offline++;
            }

            nodes_active_json["nodes"] = { \
            {"online", std::to_string(online)},
            {"offline", std::to_string(offline)},
            };
        }

        if(result.size()<=0)
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Fail\",\n"
                            "  \"message\":\"No data available\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::ok;
            response.body = nodes_active_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            connection->unprepare("get_nodes_active_count");
            return response;
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to the particular node failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch(...)
    {   
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get node";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get node\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage stat_nodes_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_active_json;
    try
    {
        pqxx::work transaction{*connection};
        pqxx::result result_count;
        int count = 0,total_records;

        connection->prepare("nodes_active_count", "select count(host_identifier) from nodes");
        result_count = transaction.exec_prepared("nodes_active_count");
        total_records = result_count.at(0)["count"].as<int>();

            nodes_active_json["nodes"] = { \
            {
                "count", std::to_string(total_records)
            }
            };
            response.status = boost::beast::http::status::ok;
            response.body = nodes_active_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            connection->unprepare("nodes_active_count");
            return response;
        
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to get node count failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch(...)
    {   
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get node";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get node count\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage node_rule_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_rule_json = nlohmann::json::array(), result_json;;
    try
    {
        pqxx::work transaction{*connection};
        pqxx::result result_count,result;
        int count = 0,severity_level, severity_counts;
        std::string host_ident;

        connection->prepare("nodes_active", "select distinct host_identifier from events");
        result = transaction.exec_prepared("nodes_active");
        for (size_t i = 0; i < result.size(); i++)
        {
            host_ident = result.at(i)["host_identifier"].as<std::string>();
            connection->prepare("nodes_active_count", "select severity,count(severity) from events where host_identifier = $1 group by severity");
            result_count = transaction.exec_prepared("nodes_active_count", host_ident);
            
               result_json = { \
                    {"host_identifier", (host_ident)},
                };
            for (size_t i = 0; i < result_count.size(); i++)
            {
               severity_level = result_count.at(i)["severity"].as<int>();
               severity_counts = result_count.at(i)["count"].as<int>();
               
               result_json["severity"] += { \
        
                    {"severity", std::to_string(severity_level)},
                    {"severity_count", std::to_string(severity_counts)}
                
                };
            }
            nodes_rule_json.push_back(result_json);
        }
        response.status = boost::beast::http::status::ok;
        response.body = nodes_rule_json.dump();
        response.header["Content-Type"] = "application/json";
        transaction.commit();
        connection->unprepare("nodes_active_count");
        transaction.commit();
        connection->unprepare("nodes_active");
        return response;
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to get node severity count failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch(...)
    {   
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get node";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get node severity count.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    EXIT_SUCCESS;
}

HTTPMessage max_severity_node_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json max_severity_node_count_json, params;
    try
    {
        pqxx::work transaction{*connection};
        pqxx::result result_count;
        int count = 0,total_records;
        std::string level = "desc", topn = "1";
        params = request.query;
        if(params.contains("level"))
        {
            level = params["level"];
        }
        if(params.contains("topn"))
        {
            topn = params["topn"];
        }

        for(int i=0; i<=4; i++)
        {
            std::string query = "select host_identifier, hostname, count(severity) from events where severity = '"+std::to_string(i)+"' group by host_identifier,hostname order by count "+level+" limit "+topn+"";
            connection->prepare("max_severity_node_count", query);
            result_count = transaction.exec_prepared("max_severity_node_count");
            connection->unprepare("max_severity_node_count");

            std::string host_identifier, hostnames;
            int counts = 0;
            for (pqxx::result::const_iterator j = result_count.begin(); j != result_count.end(); ++j)
                {
                    std::string host_identifier = j["host_identifier"].as<std::string>();
                    std::string hostname = j["hostname"].as<std::string>();
                    std::string count = j["count"].as<std::string>();

                    max_severity_node_count_json["node_severity_count"] += { \

                        {"host_identifier", j["host_identifier"].as<std::string>()},
                        {"hostname", j["hostname"].as<std::string>()},
                        {"severity", i},
                        {"count", j["count"].as<std::string>()}

                    };
                    // int count = i["count"].as<int>();
                    // if (count > counts){
                    //    host_ids = host_id;
                    //    hostnames = hostname;
                    //    counts = count;
                    // }

                }
            
                // max_severity_node_count_json["node_severity_count"] += { \
                //     // {
                //     //     "count", counts,
                //     //     "severity" , i,
                //     //     "host_identifier", host_ids,
                //     //     "hostname", hostnames
                //     // }
                //     {

                //     }
                // };
        }
            response.status = boost::beast::http::status::ok;
            response.body = max_severity_node_count_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            return response;
        
        
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to get max node severity count failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch(...)
    {   
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get max node severity count";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get max node severity count\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    return response;
}