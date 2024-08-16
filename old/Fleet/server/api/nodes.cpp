#include "../../3rdparty/json.hpp"
#include "nodes.h"
#include "scheduled_query.h"
#include "../../common/logger.h"
#include <chrono>
#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;

HTTPMessage nodes_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_list_json, params;
    std::string host_identifier, hostname, is_active, no_of_rows_per_page, pageno, str_start_time, str_end_time;
    int start_time = 0, end_time = 0;
    try
    {
        int row_to_skip, r1 = 1, r2 = 100, total_records;
        params = request.query;
        if (params.contains("hostname"))
        {
            hostname = params["hostname"];
        }
        if (params.contains("start_time"))
        {
            str_start_time = params["start_time"];
            start_time = (stoi(str_start_time));
        }
        if (params.contains("end_time"))
        {
            str_end_time = params["end_time"];
            end_time = (stoi(str_end_time));
        }
        if (params.contains("is_active"))
        {
            is_active = params["is_active"];
        }
        if (params.contains("pageno"))
        {
            pageno = params["pageno"];
            r1 = (stoi(pageno));
        }
        if (params.contains("rows"))
        {
            no_of_rows_per_page = params["rows"];
            r2 = (stoi(no_of_rows_per_page));
        }
        row_to_skip = (r1 - 1) * r2;
        pqxx::work transaction{*connection};
        pqxx::result result, result_count;
        // 000
        if (hostname.empty() && start_time == 0 && end_time == 0)
        {
            connection->prepare("get_nodes_list_count", "select count(id) from nodes");
            result_count = transaction.exec_prepared("get_nodes_list_count");
            connection->unprepare("get_nodes_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_nodes_list", "select * from nodes order by id desc offset $1 rows fetch next $2 rows only");
            result = transaction.exec_prepared("get_nodes_list", row_to_skip, r2);
            connection->unprepare("get_nodes_list");
        }
        // 001
        if (hostname.empty() && start_time == 0 && !end_time == 0)
        {

            connection->prepare("get_nodes_list_count", "select count(id) from nodes where enrolled_on <= $1");
            result_count = transaction.exec_prepared("get_nodes_list_count", end_time);
            connection->unprepare("get_nodes_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_nodes_list", "select * from nodes where enrolled_on <= $3 order by id desc offset $1 rows fetch next $2 rows only");
            result = transaction.exec_prepared("get_nodes_list", row_to_skip, r2, end_time);
            connection->unprepare("get_nodes_list");
        }
        // 010
        if (hostname.empty() && !start_time == 0 && end_time == 0)
        {

            connection->prepare("get_nodes_list_count", "select count(id) from nodes where enrolled_on >= $1");
            result_count = transaction.exec_prepared("get_nodes_list_count", start_time);
            connection->unprepare("get_nodes_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_nodes_list", "select * from nodes where enrolled_on >= $3 order by id desc offset $1 rows fetch next $2 rows only");
            result = transaction.exec_prepared("get_nodes_list", row_to_skip, r2, start_time);
            connection->unprepare("get_nodes_list");
        }
        // 011
        if (hostname.empty() && !start_time == 0 && !end_time == 0)
        {

            connection->prepare("get_nodes_list_count", "select count(id) from nodes where enrolled_on >= $1 and enrolled_on <= $2");
            result_count = transaction.exec_prepared("get_nodes_list_count", start_time, end_time);
            connection->unprepare("get_nodes_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_nodes_list", "select * from nodes where enrolled_on >= $3 and enrolled_on <= $4 order by id desc offset $1 rows fetch next $2 rows only");
            result = transaction.exec_prepared("get_nodes_list", row_to_skip, r2, start_time, end_time);
            connection->unprepare("get_nodes_list");
        }
        // 100
        if (!hostname.empty() && start_time == 0 && end_time == 0)
        {
            std::string query1 = "select count(id) from nodes where (host_identifier like '%" + hostname + "%' or hostname like '%" + hostname + "%')";
            connection->prepare("get_nodes_list_count", query1);
            result_count = transaction.exec_prepared("get_nodes_list_count");
            connection->unprepare("get_nodes_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from nodes where (host_identifier like '%" + hostname + "%' or hostname like '%" + hostname + "%') order by id desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_nodes_list", query2);
            result = transaction.exec_prepared("get_nodes_list");
            connection->unprepare("get_nodes_list");
        }
        // 101
        if (!hostname.empty() && start_time == 0 && !end_time == 0)
        {

            std::string query1 = "select count(id) from nodes where (host_identifier like '%" + hostname + "%' or hostname like '%" + hostname + "%') and enrolled_on <= '" + std::to_string(end_time) + "'";
            connection->prepare("get_nodes_list_count");
            result_count = transaction.exec_prepared("get_nodes_list_count", query1);
            connection->unprepare("get_nodes_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from nodes where (host_identifier like '%" + hostname + "%' or hostname like '%" + hostname + "%') and enrolled_on <= '" + std::to_string(end_time) + "' order by id desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_nodes_list", query2);
            result = transaction.exec_prepared("get_nodes_list");
            connection->unprepare("get_nodes_list");
        }
        // 110
        if (!hostname.empty() && !start_time == 0 && end_time == 0)
        {

            std::string query1 = "select count(id) from nodes where (host_identifier like '%" + hostname + "%' or hostname like '%" + hostname + "%') and enrolled_on >= '" + std::to_string(start_time) + "'";
            connection->prepare("get_nodes_list_count", query1);
            result_count = transaction.exec_prepared("get_nodes_list_count");
            connection->unprepare("get_nodes_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from nodes where (host_identifier like '%" + hostname + "%' or hostname like '%" + hostname + "%') and enrolled_on >= '" + std::to_string(start_time) + "' order by id desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_nodes_list", query2);
            result = transaction.exec_prepared("get_nodes_list");
            connection->unprepare("get_nodes_list");
        }

        // 111
        if (!hostname.empty() && !start_time == 0 && !end_time == 0)
        {

            std::string query1 = "select count(id) from nodes where (host_identifier like '%" + hostname + "%' or hostname like '%" + hostname + "%') and enrolled_on >= '" + std::to_string(start_time) + "' and enrolled_on <= '" + std::to_string(end_time) + "'";
            connection->prepare("get_nodes_list_count", query1);
            result_count = transaction.exec_prepared("get_nodes_list_count");
            connection->unprepare("get_nodes_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from nodes where (host_identifier like '%" + hostname + "%' or hostname like '%" + hostname + "%') and enrolled_on >= '" + std::to_string(start_time) + "' and enrolled_on <= '" + std::to_string(end_time) + "' order by id desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_nodes_list", query2);
            result = transaction.exec_prepared("get_nodes_list");
            connection->unprepare("get_nodes_list");
        }

        if (is_active == "true")
        {
            for (pqxx::result::const_iterator i = result.begin(); i != result.end(); ++i)
            {
                long lastseen = i["last_seen"].as<long>();
                std::time_t t = std::time(0);
                std::string s_current_time = std::to_string(t);
                int current_time = std::stoi(s_current_time);
                int dif = current_time - lastseen;
                std::string active_status;
                if (dif < 300)
                {
                    active_status = "Online";
                    nodes_list_json["nodes"] += {
                        {"host_identifier", i["host_identifier"].as<std::string>()},
                        {"os_arch", i["os_arch"].as<std::string>()},
                        {"os_build", i["os_build"].as<std::string>()},
                        {"os_major", i["os_major"].as<std::string>()},
                        {"os_minor", i["os_minor"].as<std::string>()},
                        {"os_name", i["os_name"].as<std::string>()},
                        {"os_platform", i["os_platform"].as<std::string>()},
                        {"osquery_version", i["osquery_version"].as<std::string>()},
                        {"node_key", i["node_key"].as<std::string>()},
                        {"node_invalid", i["node_invalid"].as<std::string>()},
                        {"reenroll_allowed", i["reenroll_allowed"].as<std::string>()},
                        {"hardware_vendor", i["hardware_vendor"].as<std::string>()},
                        {"hardware_model", i["hardware_model"].as<std::string>()},
                        {"hardware_version", i["hardware_version"].as<std::string>()},
                        {"hostname", i["hostname"].as<std::string>()},
                        {"enrolled_on", i["enrolled_on"].as<std::string>()},
                        {"status", active_status},
                        {"last_seen", lastseen},
                        {"hardware_cpu_logical_core", i["hardware_cpu_logical_core"].as<std::string>()},
                        {"hardware_cpu_type", i["hardware_cpu_type"].as<std::string>()},
                        {"hardware_physical_memory", i["hardware_physical_memory"].as<std::string>()}};
                }
            }
        }
        else if (is_active == "false")
        {
            for (pqxx::result::const_iterator i = result.begin(); i != result.end(); ++i)
            {
                long lastseen = i["last_seen"].as<long>();
                std::time_t t = std::time(0);
                std::string s_current_time = std::to_string(t);
                int current_time = std::stoi(s_current_time);
                int dif = current_time - lastseen;
                std::string active_status;
                if (dif > 300)
                {
                    active_status = "Offline";
                    nodes_list_json["nodes"] += {
                        {"host_identifier", i["host_identifier"].as<std::string>()},
                        {"os_arch", i["os_arch"].as<std::string>()},
                        {"os_build", i["os_build"].as<std::string>()},
                        {"os_major", i["os_major"].as<std::string>()},
                        {"os_minor", i["os_minor"].as<std::string>()},
                        {"os_name", i["os_name"].as<std::string>()},
                        {"os_platform", i["os_platform"].as<std::string>()},
                        {"osquery_version", i["osquery_version"].as<std::string>()},
                        {"node_key", i["node_key"].as<std::string>()},
                        {"node_invalid", i["node_invalid"].as<std::string>()},
                        {"reenroll_allowed", i["reenroll_allowed"].as<std::string>()},
                        {"hardware_vendor", i["hardware_vendor"].as<std::string>()},
                        {"hardware_model", i["hardware_model"].as<std::string>()},
                        {"hardware_version", i["hardware_version"].as<std::string>()},
                        {"hostname", i["hostname"].as<std::string>()},
                        {"enrolled_on", i["enrolled_on"].as<std::string>()},
                        {"status", active_status},
                        {"last_seen", lastseen},
                        {"hardware_cpu_logical_core", i["hardware_cpu_logical_core"].as<std::string>()},
                        {"hardware_cpu_type", i["hardware_cpu_type"].as<std::string>()},
                        {"hardware_physical_memory", i["hardware_physical_memory"].as<std::string>()}};
                }
            }
        }
        else
        {
            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
            {
                long lastseen = c["last_seen"].as<long>();
                std::time_t t = std::time(0);
                std::string s_current_time = std::to_string(t);
                int current_time = std::stoi(s_current_time);
                int dif = current_time - lastseen;
                std::string active_status;
                if (dif < 300)
                {
                    active_status = "Online";
                }
                else
                {
                    active_status = "Offline";
                }
                nodes_list_json["nodes"] += {
                    {"host_identifier", c["host_identifier"].as<std::string>()},
                    {"os_arch", c["os_arch"].as<std::string>()},
                    {"os_build", c["os_build"].as<std::string>()},
                    {"os_major", c["os_major"].as<std::string>()},
                    {"os_minor", c["os_minor"].as<std::string>()},
                    {"os_name", c["os_name"].as<std::string>()},
                    {"os_platform", c["os_platform"].as<std::string>()},
                    {"osquery_version", c["osquery_version"].as<std::string>()},
                    {"node_key", c["node_key"].as<std::string>()},
                    {"node_invalid", c["node_invalid"].as<std::string>()},
                    {"reenroll_allowed", c["reenroll_allowed"].as<std::string>()},
                    {"hardware_vendor", c["hardware_vendor"].as<std::string>()},
                    {"hardware_model", c["hardware_model"].as<std::string>()},
                    {"hardware_version", c["hardware_version"].as<std::string>()},
                    {"hostname", c["hostname"].as<std::string>()},
                    {"enrolled_on", c["enrolled_on"].as<std::string>()},
                    {"status", active_status},
                    {"last_seen", lastseen},
                    {"hardware_cpu_logical_core", c["hardware_cpu_logical_core"].as<std::string>()},
                    {"hardware_cpu_type", c["hardware_cpu_type"].as<std::string>()},
                    {"hardware_physical_memory", c["hardware_physical_memory"].as<std::string>()}

                };
            }
        }

        if (!nodes_list_json.contains("nodes"))
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
            response.body = nodes_list_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            return response;
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to get node list failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get node list";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail.\",\n"
                        "  \"message\":\"Failed to get node list\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage nodes_query(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_query_json;
    std::string host_identifier;
    int no_of_records = 3, start_time = 0, end_time = 0;;
    std::stringstream ss;
    try
    {
        pqxx::result result_count;
        nodes_query_json = request.query;
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        if (nodes_query_json.contains("start_time"))
        {
            start_time = nodes_query_json["start_time"];
        }
        if (nodes_query_json.contains("end_time"))
        {
            end_time = nodes_query_json["end_time"];
        }
        if (nodes_query_json.contains("no_of_records"))
        {
            no_of_records = nodes_query_json["no_of_records"];
        }
        pqxx::work transaction{*connection};
        pqxx::result result_cpu, result_memory, result, result_logged_user, result_config, result_ip;
        if (!host_identifier.empty() && start_time == 0 && end_time == 0)
        {
            connection->prepare("get_nodes_query", "select * from events where host_identifier = $1 order by id desc limit $2");
            result = transaction.exec_prepared("get_nodes_query", host_identifier, no_of_records);
        }
        else if (!host_identifier.empty() && !start_time == 0 && end_time == 0)
        {
            connection->prepare("get_nodes_query", "select * from events where host_identifier = $1 and unixtime >= $2 order by id desc limit $3");
            result = transaction.exec_prepared("get_nodes_query", host_identifier, start_time, no_of_records);
        }
        else if (!host_identifier.empty() && start_time == 0 && !end_time == 0)
        {
            connection->prepare("get_nodes_query", "select * from events where host_identifier = $1 and unixtime <= $2 order by id desc limit $3");
            result = transaction.exec_prepared("get_nodes_query", host_identifier, end_time, no_of_records);
        }
        else if (!host_identifier.empty() && !start_time == 0 && !end_time == 0)
        {
            connection->prepare("get_nodes_query", "select * from events where host_identifier = $1 and unixtime >= $2 and unixtime >= $3 order by id desc limit $4");
            result = transaction.exec_prepared("get_nodes_query", host_identifier, start_time, end_time, no_of_records);
        }
        
        for (pqxx::result::const_iterator i = result.begin(); i != result.end(); ++i)
        {
            nodes_query_json["events"] += {
                {"id", i["id"].as<std::string>()},
                {"host_identifier", i["host_identifier"].as<std::string>()},
                {"hostname", i["hostname"].as<std::string>()},
                {"unixtime", i["unixtime"].as<std::string>()},
                {"event_code", i["event_code"].as<std::string>()},
                {"metadata", i["metadata"].as<std::string>()},
                {"is_alert", i["is_alert"].as<std::string>()},
                {"is_open", i["is_open"].as<std::string>()},
                {"context_data", i["context_data"].as<std::string>()},
                {"severity", i["severity"].as<std::string>()},
                {"alert_type", i["alert_type"].as<std::string>()}
            };    
        }

        int count = 0, severity_level, severity_counts;
        std::string ip_address;

        if (!host_identifier.empty())
        {
            connection->prepare("nodes_active_count", "select severity,count(severity) from events where host_identifier = $1 group by severity");
            result_count = transaction.exec_prepared("nodes_active_count", host_identifier);
        }

        for (pqxx::result::const_iterator i = result_count.begin(); i != result_count.end(); ++i)
        {
            severity_level = i["severity"].as<int>();
            severity_counts = i["count"].as<int>();

            nodes_query_json["severity"] += {
                {"severity_count", std::to_string(severity_counts)},
                {"severity_level", std::to_string(severity_level)},
            };
        }

        if (!host_identifier.empty())
        {
            connection->prepare("logged_user", "select distinct logged_user from logged_in_users where type = 'user' and host_identifier = $1");
            result_logged_user = transaction.exec_prepared("logged_user", host_identifier);
        }
        if (!result_logged_user.empty())
        {
            for (pqxx::result::const_iterator i = result_logged_user.begin(); i != result_logged_user.end(); ++i)
            {
                nodes_query_json["users"] += {
                    {"logged_user", i["logged_user"].as<std::string>()}};
            }
        }

        if (!host_identifier.empty())
        {
            connection->prepare("node_config", "select config_id from nodes where host_identifier = $1");
            result_config = transaction.exec_prepared("node_config", host_identifier);
        }

        for (pqxx::result::const_iterator i = result_config.begin(); i != result_config.end(); ++i)
        {
            nodes_query_json["config"]=i["config_id"].as<std::string>();
        }

        if (!host_identifier.empty())
        {
            connection->prepare("node_ip", "select address from interface_addresses where host_identifier = $1 and broadcast <> '' and interface <> 'lo' order by id desc limit 1");
            result_ip = transaction.exec_prepared("node_ip", host_identifier);
        }

        for (pqxx::result::const_iterator i = result_ip.begin(); i != result_ip.end(); ++i)
        {
            nodes_query_json["address"]=i["address"].as<std::string>();
        }

        if (result.empty() && result_count.empty() && result_logged_user.empty())
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
            response.body = nodes_query_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            connection->unprepare("nodes_active_count");
            connection->unprepare("get_nodes_query");
            connection->unprepare("logged_user");
            connection->unprepare("node_config");
            connection->unprepare("node_ip");
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
    catch (...)
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

HTTPMessage nodes_reenroll(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_reenroll_json;
    bool reenroll_allowed;
    std::string host_identifier;
    try
    {
        std::string body = request.body;
        nodes_reenroll_json = nlohmann::json::parse(body);
        host_identifier = nodes_reenroll_json["host_identifier"];
        reenroll_allowed = nodes_reenroll_json["reenroll_allowed"];

        connection->prepare("nodes_reenroll", "update nodes set reenroll_allowed = $2 where host_identifier = $1");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("nodes_reenroll", host_identifier, reenroll_allowed);
        int status = result.affected_rows();
        transaction.commit();
        connection->unprepare("nodes_reenroll");

        if (status > 0)
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"Node re-enroll allowed status changed.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::bad_request;
            response.body = "{\n"
                            "  \"status\":\"Fail\",\n"
                            "  \"message\":\"Failed to change status.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to change the status of re-enroll node failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query to change the status of re-enroll node contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to change status.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage nodes_delete(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_delete_json;
    std::string host_identifier;
    try
    {
        std::string body = request.body;
        nodes_delete_json = nlohmann::json::parse(body);
        host_identifier = nodes_delete_json["host_identifier"];
        connection->prepare("delete_nodes", "delete from nodes where host_identifier = $1");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("delete_nodes", host_identifier);
        int status = result.affected_rows();
        transaction.commit();

        if (status > 0)
        {
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"Node Deleted.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Fail.\",\n"
                            "  \"message\":\"Failed to delete node\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }

        // response.status = boost::beast::http::status::ok;
        // response.body = nodes_delete_json.dump();
        // response.header["Content-Type"] = "application/json";
        // connection->unprepare("delete_nodes");
        // return response;
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to delete node failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not delete node";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail.\",\n"
                        "  \"message\":\"Failed to delete node\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage nodes_active_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_active_json;
    std::string is_active;
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
            if (dif < 300)
            {
                online++;
            }
            else
            {
                offline++;
            }

            nodes_active_json["nodes"] = {
                {"online", std::to_string(online)},
                {"offline", std::to_string(offline)},
            };
        }

        if (result.size() <= 0)
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
    catch (...)
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

HTTPMessage nodes_config(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_configid_json;
    std::string host_identifier;
    int status;
    nlohmann::json arr = nlohmann::json::array();
    try
    {
        std::string body = request.body;
        nodes_configid_json = nlohmann::json::parse(body);
        host_identifier = nodes_configid_json["host_identifier"];
        for (int i = 0; i < nodes_configid_json["config_id"].size(); i++)
        {
            arr.push_back(nodes_configid_json["config_id"][i]);
        }
        connection->prepare("nodes_config", "update nodes set config_id = $1 where host_identifier = $2");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("nodes_config", to_string(arr).substr(1,to_string(arr).size()-2), host_identifier);
        int status = result.affected_rows();
        transaction.commit();
        connection->unprepare("nodes_config");
        if (status > 0)
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"Node Configured.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::bad_request;
            response.body = "{\n"
                            "  \"status\":\"Fail\",\n"
                            "  \"message\":\"Failed to configure node.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to configure node failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query to configure node contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to configure node\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage node_health(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json node_health_json, node_request;
    bool reenroll_allowed;
    std::string host_identifier;
    int start_time, end_time, temp_end_time, time= 300;
    std::stringstream ss;
    try
    {
        pqxx::result result_cpu, result_memory;
        pqxx::work transaction{*connection};
        node_request = request.query;
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        if (node_request.contains("time"))
        {
            time = node_request["time"]; // 1hr, 3hr, 6hr ... 1hr = 3600sec
        }
        const auto p1 = std::chrono::system_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        end_time = t;
        int count = 0;
        std::string node_health[16];

        do
        {
            int temp_start_time = end_time - time; // 3600sec for 1hr
            temp_end_time = temp_start_time + 20;    // in how much interval we want to divide

            if (!host_identifier.empty())
            {

                connection->prepare("nodes_cpu_health", "select (SUM(users) + SUM(nice) + SUM(system) + SUM(idle) * 1.0) AS time_since_boot, SUM(COALESCE(idle, 0)) + SUM(COALESCE(iowait, 0)) AS idle_time_since_boot FROM cpu_time where host_identifier = $1 and unixTime >= $2 and unixTime <= $3");
                result_cpu = transaction.exec_prepared("nodes_cpu_health", host_identifier, temp_start_time, temp_end_time);
                connection->unprepare("nodes_cpu_health");
            }
            if (!result_cpu.empty())
            {
                double idle_time_since_boot, time_since_boot, average_CPU_load;

                for (pqxx::result::const_iterator i = result_cpu.begin(); i != result_cpu.end(); ++i)
                {
                    if (i["time_since_boot"].is_null())
                    {
                        average_CPU_load = 0.0;
                        node_health[count] = std::to_string(average_CPU_load);
                        count++;
                    }
                    else
                    {
                        time_since_boot = i["time_since_boot"].as<double>();
                        idle_time_since_boot = i["idle_time_since_boot"].as<double>();
                        average_CPU_load = (1 - (idle_time_since_boot / time_since_boot)) * 100;
                        node_health[count] = std::to_string(average_CPU_load);
                        count++;
                    }
                }
            }
            start_time = temp_end_time;
            time = time - 20;
        } while (temp_end_time <= end_time);
        node_health_json["cpu_health"] += {
            {"average_CPU_load", node_health}};
        if (result_cpu.empty())
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
            response.body = node_health_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            return response;
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to node health failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query to node health contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"No data available\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage node_memory_health(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json node_health_json, node_request;
    bool reenroll_allowed;
    std::string host_identifier;
    int start_time, end_time, temp_end_time, time =300;
    std::stringstream ss;
    try
    {
        pqxx::result result_cpu, result_memory;
        pqxx::work transaction{*connection};
        node_request = request.query;
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        if (node_request.contains("time"))
        {
            time = node_request["time"]; // 1hr, 3hr, 6hr ... 1hr = 3600sec
        }
        const auto p1 = std::chrono::system_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        end_time = t;
        int count = 0;
        std::string node_health[16];

        do
        {
            int temp_start_time = end_time - time; // 3600sec for 1hr
            temp_end_time = temp_start_time + 20;    // in how much interval we want to divide

            if (!host_identifier.empty())
            {
                connection->prepare("nodes_memory_health", "select * from memory_info where host_identifier = $1 and unixTime >= $2 and unixTime <= $3 order by id desc limit 1");
                result_memory = transaction.exec_prepared("nodes_memory_health", host_identifier, temp_start_time, temp_end_time);
                connection->unprepare("nodes_memory_health");
            }
            if (result_memory.empty())
            {
                {
                    node_health_json["memory_health"] += {
                        {"memory_total", 0},
                        {"memory_free", 0}};
                }
            }
            else
            {
                for (pqxx::result::const_iterator i = result_memory.begin(); i != result_memory.end(); ++i)
                {
                    node_health_json["memory_health"] += {
                        {"memory_total", i["memory_total"].as<std::string>()},
                        {"memory_free", i["memory_free"].as<std::string>()}};
                    count++;
                }
            }
            start_time = temp_end_time;
            time = time - 20;
        } while (temp_end_time <= end_time);

        response.status = boost::beast::http::status::ok;
        response.body = node_health_json.dump();
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
                        "  \"message\":\"Query to node health failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query to node health contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"No data available\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage historical_data(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    std::string query, random;
    std::string host_identifier;
    std::stringstream ss;
    try
    {
        pqxx::result historical_data_result;
        // nlohmann::json my_query = request.query;
        std::string body = request.body;
        nlohmann::json my_query = nlohmann::json::parse(body);
        std::string q = to_string(my_query);
        if (my_query.contains("query"))
        {
            query = my_query["query"];
            boost::replace_all(query, "%20", " ");
        }

        pqxx::work transaction{*connection};
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        connection->prepare("historical_data_query", query);
        historical_data_result = transaction.exec_prepared("historical_data_query");
        // std::cout << historical_data_result;
        transaction.commit();

        int const num_rows = std::size(historical_data_result);
        nlohmann::json parents_list = nlohmann::json::array();

        for (int rownum = 0; rownum < num_rows; ++rownum)
        {
            pqxx::row const row = historical_data_result[rownum];
            int const num_cols = std::size(row);
            nlohmann::json obj;

            for (int colnum = 0; colnum < num_cols; ++colnum)
            {
                pqxx::field const field = row[colnum];

                if (row[row[colnum].name()].is_null())
                {
                    obj[row[colnum].name()] = "NULL";
                }
                else
                {
                    obj[row[colnum].name()] = row[row[colnum].name()].as<std::string>();
                }
            }
            parents_list.push_back(obj);
        }

        response.status = boost::beast::http::status::ok;
        response.body = parents_list.dump();
        response.header["Content-Type"] = "application/json";
        transaction.commit();
        connection->unprepare("historical_data_query");
        return response;
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to historical data failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query to historical data contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"No data available\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage nodes_ip(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json nodes_list_json, params;
    std::string is_active, host_identifier;
    std::stringstream ss;
    try
    {
        params = request.query;
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        connection->prepare("get_nodes_ip", "select address from interface_addresses where host_identifier = $1 and broadcast <> '' and interface <> 'lo' order by id desc limit 1");
        pqxx::work transaction{*connection};
        pqxx::result result;
        result = transaction.exec_prepared("get_nodes_ip", host_identifier);

        for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
        {
            nodes_list_json["nodes"] += {
                {"address", c["address"].as<std::string>()}};
        };

        if (!nodes_list_json.contains("nodes"))
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
            response.body = nodes_list_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            connection->unprepare("get_nodes_ip");
            return response;
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to get node ip failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get node ip";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail.\",\n"
                        "  \"message\":\"Failed to get node ip\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}
