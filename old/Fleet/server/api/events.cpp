#include "../../3rdparty/json.hpp"
#include "events.h"
#include "../../common/logger.h"
#include "../../event_processor/EventProcessor.h"
#include "../../event_processor/EventProcessor.cpp"

HTTPMessage event_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json events_list_json, response_json;
    std::string host_identifier, is_open, start_time, end_time, pageno, no_of_rows_per_page;
    std::stringstream ss;
    try
    {
        int row_to_skip, r1 = 1, r2 = 100, total_records;
        events_list_json = request.query;
        if (events_list_json.contains("host_identifier"))
        {
            host_identifier = events_list_json["host_identifier"];
        }
        if (events_list_json.contains("is_open"))
        {
            is_open = events_list_json["is_open"];
        }
        if (events_list_json.contains("start_time"))
        {
            start_time = events_list_json["start_time"];
        }
        if (events_list_json.contains("end_time"))
        {
            end_time = events_list_json["end_time"];
        }
        if (events_list_json.contains("pageno"))
        {
            pageno = events_list_json["pageno"];
            r1 = (stoi(pageno));
            // r1 = r1--;
        }
        if (events_list_json.contains("rows"))
        {
            no_of_rows_per_page = events_list_json["rows"];
            r2 = (stoi(no_of_rows_per_page));
        }

        row_to_skip = (r1 - 1) * r2;
        pqxx::work transaction{*connection};
        pqxx::result result, result_count, result_ip;
        std::string ip_address, os_name, os_platform, osquery_version;

        // 0000
        if (host_identifier.empty() && is_open.empty() && start_time.empty() && end_time.empty())
        {
            connection->prepare("get_events_list_count", "select count(id) from events");
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select * from events order by unixtime desc offset $1 rows fetch next $2 rows only");
            result = transaction.exec_prepared("get_events_list", row_to_skip, r2);
            connection->unprepare("get_events_list");
        }
        // 0001
        else if (host_identifier.empty() && is_open.empty() && start_time.empty() && !end_time.empty())
        {
            connection->prepare("get_events_list_count", "select count(id) from events where unixtime <= $1");
            result_count = transaction.exec_prepared("get_events_list_count", end_time);
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select * from events where unixtime <= $3 order by unixtime desc offset $1 rows fetch next $2 rows only");
            result = transaction.exec_prepared("get_events_list", row_to_skip, r2, end_time);
            connection->unprepare("get_events_list");
        }
        // 0010
        else if (host_identifier.empty() && is_open.empty() && !start_time.empty() && end_time.empty())
        {
            connection->prepare("get_events_list_count", "select count(id) from events where unixtime >= $1");
            result_count = transaction.exec_prepared("get_events_list_count", start_time);
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select * from events where unixtime >= $1 order by unixtime desc offset $2 rows fetch next $3 rows only");
            result = transaction.exec_prepared("get_events_list", start_time, row_to_skip, r2);
            connection->unprepare("get_events_list");
        }
        // 0011
        else if (host_identifier.empty() && is_open.empty() && !start_time.empty() && !end_time.empty())
        {
            connection->prepare("get_events_list_count", "select count(id) from events where unixtime >= $1 and unixtime <= $2");
            result_count = transaction.exec_prepared("get_events_list_count", start_time, end_time);
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select * from events where unixtime >= $1 and unixtime <= $2 order by unixtime desc offset $3 rows fetch next $4 rows only");
            result = transaction.exec_prepared("get_events_list", start_time, end_time, row_to_skip, r2);
            connection->unprepare("get_events_list");
        }
        // 0100
        else if (host_identifier.empty() && !is_open.empty() && start_time.empty() && end_time.empty())
        {
            connection->prepare("get_events_list_count", "select count(id) from events where is_open = $1");
            result_count = transaction.exec_prepared("get_events_list_count", is_open);
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select * from events where is_open = $1 order by unixtime desc offset $2 rows fetch next $3 rows only");
            result = transaction.exec_prepared("get_events_list", is_open, row_to_skip, r2);
            connection->unprepare("get_events_list");
        }
        // 0101
        else if (host_identifier.empty() && !is_open.empty() && start_time.empty() && !end_time.empty())
        {
            connection->prepare("get_events_list_count", "select count(id) from events where is_open = $1 and unixtime <=  $2");
            result_count = transaction.exec_prepared("get_events_list_count", is_open, end_time);
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select * from events where is_open = $1 and unixtime <=  $2 order by unixtime desc offset $3 rows fetch next $4 rows only");
            result = transaction.exec_prepared("get_events_list", is_open, end_time, row_to_skip, r2);
            connection->unprepare("get_events_list");
        }
        // 0110
        else if (host_identifier.empty() && !is_open.empty() && !start_time.empty() && end_time.empty())
        {
            connection->prepare("get_events_list_count", "select count(id) from events where is_open = $1 and unixtime >= $2 ");
            result_count = transaction.exec_prepared("get_events_list_count", is_open, start_time);
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select * from events where is_open = $1 and unixtime >= $2 order by unixtime desc offset $3 rows fetch next $4 rows only");
            result = transaction.exec_prepared("get_events_list", is_open, start_time, row_to_skip, r2);
            connection->unprepare("get_events_list");
        }
        // 0111
        else if (host_identifier.empty() && !is_open.empty() && !start_time.empty() && !end_time.empty())
        {
            connection->prepare("get_events_list_count", "select count(id) from events where is_open = $1 and unixtime >= $2 and unixtime <= $3");
            result_count = transaction.exec_prepared("get_events_list_count", is_open, start_time, end_time);
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select * from events where is_open = $1 and unixtime >= $2 and unixtime <= $3 order by unixtime desc offset $4 rows fetch next $5 rows only");
            result = transaction.exec_prepared("get_events_list", is_open, start_time, end_time, row_to_skip, r2);
            connection->unprepare("get_events_list");
        }
        // 1000
        else if (!host_identifier.empty() && is_open.empty() && start_time.empty() && end_time.empty())
        {
            std::string query1 = "select count(id) from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%')";
            connection->prepare("get_events_list_count", query1);
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') order by unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_events_list", query2);
            result = transaction.exec_prepared("get_events_list");
            connection->unprepare("get_events_list");
        }
        // 1001
        else if (!host_identifier.empty() && is_open.empty() && start_time.empty() && !end_time.empty())
        {
            std::string query1 = "select count(id) from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and unixtime <= '" + end_time + "'";
            connection->prepare("get_events_list_count", query1);
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and unixtime <= '" + end_time + "' order by unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_events_list", query2);
            result = transaction.exec_prepared("get_events_list");
            connection->unprepare("get_events_list");
        }
        // 1010
        else if (!host_identifier.empty() && is_open.empty() && !start_time.empty() && end_time.empty())
        {
            std::string query1 = "select count(id) from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and unixtime >= '" + start_time + "'";
            connection->prepare("get_events_list_count", query1);
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and unixtime >= '" + start_time + "' order by unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_events_list", query2);
            result = transaction.exec_prepared("get_events_list");
            connection->unprepare("get_events_list");
        }
        // 1011
        else if (!host_identifier.empty() && is_open.empty() && !start_time.empty() && !end_time.empty())
        {
            std::string query1 = "select count(id) from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and unixtime >= '" + start_time + "' and unixtime <= '" + end_time + "'";
            connection->prepare("get_events_list_count", query1);
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and unixtime >= '" + start_time + "' and unixtime <= '" + end_time + "' order by unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_events_list", query2);
            result = transaction.exec_prepared("get_events_list");
            connection->unprepare("get_events_list");
        }
        // 1100
        else if (!host_identifier.empty() && !is_open.empty() && start_time.empty() && end_time.empty())
        {
            std::string query1 = "select count(id) from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "'";
            connection->prepare("get_events_list_count", query1);
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "'  order by unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_events_list", query2);
            result = transaction.exec_prepared("get_events_list");
            connection->unprepare("get_events_list");
        }
        // 1101
        else if (!host_identifier.empty() && !is_open.empty() && start_time.empty() && !end_time.empty())
        {
            std::string query1 = "select count(id) from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' and unixtime <= '" + end_time + "'";
            connection->prepare("get_events_list_count", query1);
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' and unixtime <= '" + end_time + "' order by unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_events_list", query2);
            result = transaction.exec_prepared("get_events_list");
            connection->unprepare("get_events_list");
        }
        // 1110
        else if (!host_identifier.empty() && !is_open.empty() && !start_time.empty() && end_time.empty())
        {
            std::string query1 = "select count(id) from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' and unixtime >= '" + start_time + "'";
            connection->prepare("get_events_list_count", query1);
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' and unixtime >= '" + start_time + "' order by unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_events_list", query2);
            result = transaction.exec_prepared("get_events_list");
            connection->unprepare("get_events_list");
        }
        // 1111
        else if (!host_identifier.empty() && !is_open.empty() && !start_time.empty() && !end_time.empty())
        {
            std::string query1 = "select count(id) from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' and unixtime >= '" + start_time + "' and unixtime <= '" + end_time + "'";
            connection->prepare("get_events_list_count", query1);
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            std::string query2 = "select * from events where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' and unixtime >= '" + start_time + "' and unixtime <= '" + end_time + "' order by unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
            connection->prepare("get_events_list", query2);
            result = transaction.exec_prepared("get_events_list");
            connection->unprepare("get_events_list");
        }
        else
        {
            connection->prepare("get_events_list_count", "select count(id) from events");
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select * from events order by unixtime desc offset $1 rows fetch next $2 rows only");
            result = transaction.exec_prepared("get_events_list", row_to_skip, r2);
            connection->unprepare("get_events_list");
        }

        connection->prepare("get_event_nodes_ip", "select events.*, interface_addresses.address from events inner join interface_addresses on events.host_identifier = interface_addresses.host_identifier where interface_addresses.broadcast <> '' and interface_addresses.interface <> 'lo' order by events.id desc offset $1 rows fetch next $2 rows only");
        result_ip = transaction.exec_prepared("get_event_nodes_ip", row_to_skip, r2);
        for (pqxx::result::const_iterator c = result_ip.begin(); c != result_ip.end(); ++c)
        {
            ip_address = c["address"].as<std::string>();
        }
        connection->unprepare("get_event_nodes_ip");

        connection->prepare("get_nodes_os", "select events.*, nodes.os_name, nodes.os_platform, nodes.osquery_version from events inner join nodes on events.host_identifier = nodes.host_identifier order by events.id desc offset $1 rows fetch next $2 rows only");
        result_ip = transaction.exec_prepared("get_nodes_os", row_to_skip, r2);
        for (pqxx::result::const_iterator c = result_ip.begin(); c != result_ip.end(); ++c)
        {
            os_name = c["os_name"].as<std::string>();
            os_platform = c["os_platform"].as<std::string>();
            osquery_version = c["osquery_version"].as<std::string>();
        }
        connection->unprepare("get_nodes_os");
        // All empty
        //  else if(host_identifier.empty() && is_open.empty() && event_time.empty())
        //  {
        //      connection->prepare("get_default_events_list", "select * from events where id between 1 and 100");
        //      result = transaction.exec_prepared("get_default_events_list");
        //  }

        // connection->prepare("get_events_list", "select * from events");
        // pqxx::work transaction{*connection};
        // pqxx::result result;
        // result = transaction.exec_prepared("get_events_list", "host_identifier", host_identifier, "is_open", is_open, event_time);
        // total_records = result.size();
        response_json["no_of_records"] = total_records;

        for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
        {
            response_json["events"] += {
                {"id", c["id"].as<std::string>()},
                {"host_identifier", c["host_identifier"].as<std::string>()},
                {"hostname", c["hostname"].as<std::string>()},
                {"unixtime", c["unixtime"].as<std::string>()},
                {"event_code", c["event_code"].as<std::string>()},
                {"metadata", c["metadata"].as<std::string>()},
                {"is_alert", c["is_alert"].as<std::string>()},
                {"is_open", c["is_open"].as<std::string>()},
                {"context_data", c["context_data"].as<std::string>()},
                {"severity", c["severity"].as<std::string>()},
                {"address", ip_address},
                {"os_name", os_name},
                {"os_platform", os_platform},
                {"osquery_version", osquery_version},
                {"technique", c["technique"].as<std::string>()},
                {"technique_id", c["technique_id"].as<std::string>()},
                {"link", c["link"].as<std::string>()}};
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
            transaction.commit();
            response.status = boost::beast::http::status::ok;
            response.body = response_json.dump();
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
                        "  \"message\":\"Query to get list of all events failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (std::exception &e)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: " << e.what();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail.\",\n"
                        "  \"message\":\"Failed to get event list\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get event list";
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail.\",\n"
                        "  \"message\":\"Failed to get event list\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage event_query(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json query_list_json;
    std::string event_id;
    std::stringstream ss;
    try
    {
        for (const auto &it : request.query)
        {
            event_id = it.second;

            ss << it.first << " : " << it.second << std::endl;
        }
        pqxx::work transaction{*connection};
        pqxx::result result;

        if (!event_id.empty())
        {

            connection->prepare("get_events_list", "select events.*, interface_addresses.address from events inner join interface_addresses on events.host_identifier = interface_addresses.host_identifier where events.id = $1 and interface_addresses.broadcast <> '' and interface_addresses.interface <> 'lo' order by interface_addresses.id desc limit 1");
            result = transaction.exec_prepared("get_events_list", event_id);
            connection->unprepare("get_events_list");
        }
        else
        {

            connection->prepare("get_events_list", "select * from events");
            result = transaction.exec_prepared("get_events_list");
            connection->unprepare("get_events_list");
        }
        for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
        {
            query_list_json["events"] += {
                {"id", c["id"].as<std::string>()},
                {"host_identifier", c["host_identifier"].as<std::string>()},
                {"unixtime", c["unixtime"].as<std::string>()},
                {"event_code", c["event_code"].as<std::string>()},
                {"metadata", c["metadata"].as<std::string>()},
                {"is_alert", c["is_alert"].as<std::string>()},
                {"is_open", c["is_open"].as<std::string>()},
                {"context_data", c["context_data"].as<std::string>()},
                {"severity", c["severity"].as<std::string>()},
                {"address", c["address"].as<std::string>()},
                {"technique", c["technique"].as<std::string>()},
                {"technique_id", c["technique_id"].as<std::string>()},
                {"link", c["link"].as<std::string>()}};
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
            transaction.commit();
            response.status = boost::beast::http::status::ok;
            response.body = query_list_json.dump();
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
                        "  \"message\":\"Query to get an event failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get event";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get event\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage event_update(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json parameters;
    nlohmann::json reader;
    int event_id;
    nlohmann::json arr = nlohmann::json::array();

    bool is_open;
    std::string remarks = "";
    std::stringstream ss;
    std::string body;
    int status;
    try
    {
        body = request.body;

        parameters = nlohmann::json::parse(body);
        is_open = parameters["is_open"];
        remarks = parameters["remarks"];

        for (int i = 0; i < parameters["event_id"].size(); i++)
        {
            arr.push_back(parameters["event_id"][i]);
            event_id = arr[i];
            connection->prepare("update_event_open", "update events set is_open = $1, remarks = $2 where id = $3");
            pqxx::work transaction{*connection};
            pqxx::result result;
            result = transaction.exec_prepared("update_event_open", is_open, remarks, event_id);
            connection->unprepare("update_event_open");
            status = result.affected_rows();
            transaction.commit();
        }

        // for(int i=0; i < parameters.size(); i++)
        // {

        //}
        if (status > 0)
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"Event Updated Sucessfully.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::bad_request;
            response.body = "{\n"
                            "  \"status\":\"Fail.\",\n"
                            "  \"message\":\"Failed to update event\"\n"
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
                        "  \"message\":\"Query to update an event failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not update event";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail.\",\n"
                        "  \"message\":\"Failed to update event\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage event_forward_process_tree(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json parameters;
    nlohmann::json forward_tree;
    int event_id;
    nlohmann::json arr = nlohmann::json::array();

    int pid;
    std::string remarks = "";
    std::stringstream ss;
    std::string body;
    std::string host_identifier;
    bool soc_event, file_event;
    int status;
    nlohmann::json process_tree;
    try
    {
        body = request.body;
        parameters = nlohmann::json::parse(body);
        pid = parameters["pid"];
        host_identifier = parameters["host_identifier"];
        soc_event = parameters["socket_event"];
        file_event = parameters["file_event"];

        {
            connection->prepare("get_bpf_process_event_id", "select id from bpf_process_events where pid = $1 and host_identifier = $2 order by id asc limit 1");

            pqxx::work transaction{*connection};
            pqxx::result result = transaction.exec_prepared("get_bpf_process_event_id", pid, host_identifier);
            connection->unprepare("get_bpf_process_event_id");
            std::queue<long> parent_queue;
            if (!result.empty())
            {
                long record_id = result.at(0)["id"].as<long>();
                {
                    std::vector<BpfProcessEntry> process_list;
                    connection->prepare("get_bpf_process_data_from_id", "select * from bpf_process_events where id = $1");

                    /*
                     * Since events can arrive out of order, and at some delay (in case osquery cannot reach fleet server for some reason, we cannot rely upon record ID in database.
                     * If we want to get latest record of event, we should order by time column instead, which records timestamp at which the event occurred. Since it comes a
                     * monotonic clock, it will never be out of order.
                     */

                    connection->prepare("get_bpf_process_data_execve", "select * from bpf_process_events where cmdline <> '' and "
                                                                       "parent = $1 and host_identifier = $2 and unixtime between $3 and $4 order by time asc limit 10");
                    connection->prepare("get_bpf_process_data_non_execve", "select * from bpf_process_events where cmdline = '' and "
                                                                           "parent = $1 and host_identifier = $2 and unixtime between $3 and $4 order by time asc limit 10");

                    pqxx::result result;

                    {
                        result = transaction.exec_prepared("get_bpf_process_data_from_id", record_id);
                    }

                    if (result.empty())
                    {
                        std::cerr << "No process found with record ID bpf" << record_id << std::endl;
                    }
                    else if (result.size() != 1)
                    {
                        std::cerr << "Multiple processes found with record ID " << record_id << std::endl;
                    }
                    else
                    {
                        // we have only one process entry now.
                        bool loop = true;
                        long ppid = result.at(0)["parent"].as<long>();
                        long pid = result.at(0)["pid"].as<long>();

                        std::string path, cmdline, host_identifier;

                        path = result.at(0)["path"].as<std::string>();
                        cmdline = result.at(0)["cmdline"].as<std::string>();
                        host_identifier = result.at(0)["host_identifier"].as<std::string>();

                        long timestamp = result.at(0)["time"].as<long>();
                        long uptime = result.at(0)["uptime"].as<long>();
                        std::time_t t = std::time(0);
                        std::string s_current_time = std::to_string(t);

                        /*
                         * First step: we prefer entries from process_events.
                         */

                        parent_queue.push(pid);
                        while (!parent_queue.empty())
                        {
                            result = transaction.exec_prepared("get_bpf_process_data_execve", parent_queue.front(), host_identifier, timestamp, s_current_time);

                            BpfProcessEntry entry;
                            BpfProcessEntry entry_soc;
                            BpfProcessEntry entry_file;
                            entry_soc.event_type = "Socket Event";
                            entry_file.event_type = "File Event";
                            entry.event_type = "Process Event";

                            if (result.empty())
                            {
                                result = transaction.exec_prepared("get_bpf_process_data_non_execve", parent_queue.front(), host_identifier, timestamp, s_current_time);
                            }

                            if (!result.empty())
                            {
                                /*
                                 * The uptime value of uptime of system when process was created. This is not a monotonically increasing clock.
                                 * The timestamp value is from a monotonic clock, when process was created.
                                 *
                                 * if process and its apparent parent process are from same session (i.e. system did not reboot between two entries),
                                 * difference is timestamp should be same as difference in uptime.
                                 *
                                 */
                                long child_pid;
                                for (unsigned int i = 0; i < result.size(); ++i)
                                {
                                    long new_timestamp = result.at(i)["time"].as<long>();
                                    long new_uptime = result.at(i)["uptime"].as<long>();

                                    long timestamp_delta = timestamp - new_timestamp;
                                    long uptime_delta = uptime - new_uptime;

                                    /*
                                     * timestamp and uptime are measured in sequence; and due to rounding, delta may not be exactly same.
                                     * A time gap of 3 seconds is chosen here
                                     */
                                    // if (std::abs(timestamp_delta - uptime_delta) > 3)
                                    // {
                                    //     // system rebooted somewhere between these two
                                    //     loop = false;

                                    //     // we need to discard the result, as it is from different session
                                    //     continue;
                                    // }
                                    // else
                                    // {
                                    //     timestamp = new_timestamp;
                                    //     uptime = new_uptime;
                                    // }

                                    path = result.at(i)["path"].as<std::string>();
                                    cmdline = result.at(i)["cmdline"].as<std::string>();
                                    child_pid = result.at(i)["pid"].as<long>();
                                    parent_queue.push(result.at(i)["pid"].as<long>());
                                    timestamp = result.at(i)["time"].as<long>();

                                        // entry.ppid = pid;

                                        entry.pid = child_pid;
                                        entry.tid = result.at(i)["tid"].as<long long>();
                                        entry.uid = result.at(i)["uid"].as<long long>();
                                        entry.gid = result.at(i)["gid"].as<long long>();
                                        entry.cid = result.at(i)["cid"].as<long long>();
                                        entry.cmdline = result.at(i)["cmdline"].as<std::string>();
                                        entry.exit_code = result.at(i)["exit_code"].as<std::string>();
                                        entry.probe_error = result.at(i)["probe_error"].as<long long>();
                                        entry.syscall = result.at(i)["syscall"].as<std::string>();
                                        entry.path = path;
                                        entry.cwd = result.at(i)["cwd"].as<std::string>();
                                        entry.duration = result.at(i)["duration"].as<long long>();
                                        entry.json_cmdline = result.at(i)["json_cmdline"].as<std::string>();
                                        entry.ntime = result.at(i)["ntime"].as<std::string>();
                                        entry.time = timestamp;
                                        entry.eid = result.at(i)["eid"].as<long long>();
                                        entry.md5 = result.at(i)["md5"].as<std::string>();
                                        entry.sha1 = result.at(i)["sha1"].as<std::string>();
                                        entry.sha256 = result.at(i)["sha256"].as<std::string>();
                                        entry.is_container_process = result.at(i)["is_container_process"].as<long long>();
                                        entry.container_age = result.at(i)["container_age"].as<long long>();
                                        entry.container_name = result.at(i)["container_name"].as<std::string>();
                                        entry.uptime = result.at(i)["uptime"].as<long>();
                                        entry.parent = parent_queue.front();

                                        // pid = tmp_pid;
                                        process_list.push_back(entry);
                                    }
                                    if (soc_event)
                                    {

                                        connection->prepare("get_bpf_socket_event_id", "select * from bpf_socket_events where pid = $1 and host_identifier = $2 order by id asc limit 1");
                                        pqxx::result result_socket = transaction.exec_prepared("get_bpf_socket_event_id", child_pid, host_identifier);
                                        if (!result_socket.empty())
                                        {
                                            entry_soc.pid = result_socket.at(0)["pid"].as<long>();

                                            entry_soc.path = result_socket.at(0)["path"].as<std::string>();
                                            entry_soc.host_identifier = result_socket.at(0)["host_identifier"].as<std::string>();
                                            entry_soc.hostname = result_socket.at(0)["hostname"].as<std::string>();
                                            entry_soc.parent = result_socket.at(0)["parent"].as<long>();
                                            entry_soc.uid = result_socket.at(0)["uid"].as<long>();
                                            entry_soc.gid = result_socket.at(0)["gid"].as<long>();
                                            entry_soc.cid = result_socket.at(0)["cid"].as<long>();
                                            entry_soc.exit_code = result_socket.at(0)["exit_code"].as<std::string>();
                                            entry_soc.syscall = result_socket.at(0)["syscall"].as<std::string>();
                                            entry_soc.protocol = result_socket.at(0)["protocol"].as<long>();
                                            entry_soc.local_address = result_socket.at(0)["local_address"].as<std::string>();
                                            entry_soc.remote_address = result_socket.at(0)["remote_address"].as<std::string>();
                                            entry_soc.local_port = result_socket.at(0)["local_port"].as<long>();
                                            entry_soc.remote_port = result_socket.at(0)["remote_port"].as<long>();
                                            entry_soc.duration = result_socket.at(0)["duration"].as<long>();

                                            entry_soc.timestamp = result_socket.at(0)["time"].as<long>();
                                            entry_soc.uptime = result_socket.at(0)["uptime"].as<long>();
                                            process_list.push_back(entry_soc);
                                        }
                                    }
                                    if (file_event)
                                    {

                                        connection->prepare("get_bpf_file_event_id", "select * from bpf_file_events where pid = $1 and host_identifier = $2 order by id asc limit 1");
                                        pqxx::result result_file = transaction.exec_prepared("get_bpf_file_event_id", child_pid, host_identifier);
                                        if (!result_file.empty())
                                        {
                                            entry_file.pid = result_file.at(0)["pid"].as<long>();

                                            entry_file.path = result_file.at(0)["path"].as<std::string>();
                                            entry_file.host_identifier = result_file.at(0)["host_identifier"].as<std::string>();
                                            entry_file.hostname = result_file.at(0)["hostname"].as<std::string>();
                                            entry_file.parent = result_file.at(0)["parent"].as<long>();
                                            entry_file.uid = result_file.at(0)["uid"].as<long>();
                                            entry_file.gid = result_file.at(0)["gid"].as<long>();
                                            entry_file.cid = result_file.at(0)["cid"].as<long>();
                                            entry_file.exit_code = result_file.at(0)["exit_code"].as<std::string>();
                                            entry_file.syscall = result_file.at(0)["syscall"].as<std::string>();
                                            entry_file.file_path = result_file.at(0)["file_path"].as<std::string>();
                                            entry_file.flags = result_file.at(0)["flags"].as<std::string>();
                                            entry_file.duration = result_file.at(0)["duration"].as<long>();

                                            entry_file.timestamp = result_file.at(0)["time"].as<long>();
                                            entry_file.uptime = result_file.at(0)["uptime"].as<long>();
                                            process_list.push_back(entry_file);
                                        }
                                }
                                parent_queue.pop();
                            }
                            else
                            {
                                loop = false;
                                parent_queue.pop();
                            }
                        }
                    }

                    connection->unprepare("get_bpf_process_data_from_id");
                    connection->unprepare("get_bpf_process_data_execve");
                    connection->unprepare("get_bpf_process_data_non_execve");
                    nlohmann::json j = nlohmann::json::array();

                    for (auto &entry : process_list)
                    {
                        nlohmann::json obj;
                        obj["pid"] = entry.pid;
                        obj["ppid"] = entry.ppid;
                        obj["tid"] = entry.tid;
                        obj["uid"] = entry.uid;
                        obj["gid"] = entry.gid;
                        obj["cid"] = entry.cid;
                        obj["exit_code"] = entry.exit_code;
                        obj["probe_error"] = entry.probe_error;
                        obj["syscall"] = entry.syscall;
                        obj["path"] = entry.path;
                        obj["cwd"] = entry.cwd;
                        obj["cmdline"] = entry.cmdline;
                        obj["duration"] = entry.duration;
                        obj["json_cmdline"] = entry.json_cmdline;
                        obj["ntime"] = entry.ntime;
                        obj["time"] = entry.time;
                        obj["eid"] = entry.eid;
                        obj["md5"] = entry.md5;
                        obj["sha1"] = entry.sha1;
                        obj["sha256"] = entry.sha256;
                        obj["is_container_process"] = entry.is_container_process;
                        obj["container_age"] = entry.container_age;
                        obj["container_name"] = entry.container_name;
                        obj["fd"] = entry.fd;
                        obj["family"] = entry.family;
                        obj["protocol"] = entry.protocol;
                        obj["local_address"] = entry.local_address;
                        obj["remote_address"] = entry.remote_address;
                        obj["local_port"] = entry.local_port;
                        obj["remote_port"] = entry.remote_port;
                        obj["uptime"] = entry.uptime;
                        obj["file_path"] = entry.file_path;
                        obj["flags"] = entry.flags;
                        obj["event_type"] = entry.event_type;
                        obj["parent"] = entry.parent;

                        j.push_back(obj);
                    }

                    process_tree = j;
                }
                if (result.size() > 0)
                {
                    response.status = boost::beast::http::status::ok;
                    response.body = process_tree.dump();
                    response.header["Content-Type"] = "application/json";
                    return response;
                }
                else
                {
                    response.status = boost::beast::http::status::bad_request;
                    response.body = "{\n"
                                    "  \"status\":\"Fail\",\n"
                                    "  \"message\":\"Failed to Generate Forward Process Tree.\"\n"
                                    "}";
                    response.header["Content-Type"] = "application/json";
                    return response;
                }
            }
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to Generate Forward Process Tree failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not Generate Forward Process Tree.";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail.\",\n"
                        "  \"message\":\"Failed to Generate Forward Process Tree.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}
