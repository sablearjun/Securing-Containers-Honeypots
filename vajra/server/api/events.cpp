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
        std::string ip_address, os_name, os_platform, osquery_version, created;
        std::vector<std::string> os_names, os_platforms, osquery_versions;

        // 0000
        if (host_identifier.empty() && is_open.empty() && start_time.empty() && end_time.empty())
        {
            connection->prepare("get_events_list_count", "select count(id) from events");
            result_count = transaction.exec_prepared("get_events_list_count");
            connection->unprepare("get_events_list_count");
            total_records = result_count.at(0)["count"].as<int>();

            connection->prepare("get_events_list", "select eve.*, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve LEFT JOIN python_rules as pr ON eve.event_code = pr.name order by eve.unixtime desc offset $1 rows fetch next $2 rows only");
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

            connection->prepare("get_events_list", "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where eve.unixtime <= $3 order by eve.unixtime desc offset $1 rows fetch next $2 rows only");
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

            connection->prepare("get_events_list", "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where unixtime >= $1 order by eve.unixtime desc offset $2 rows fetch next $3 rows only");
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

            connection->prepare("get_events_list", "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where unixtime >= $1 and unixtime <= $2 order by eve.unixtime desc offset $3 rows fetch next $4 rows only");
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

            connection->prepare("get_events_list", "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where is_open = $1 order by eve.unixtime desc offset $2 rows fetch next $3 rows only");
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

            connection->prepare("get_events_list", "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where is_open = $1 and unixtime <=  $2 order by eve.unixtime desc offset $3 rows fetch next $4 rows only");
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

            connection->prepare("get_events_list", "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where is_open = $1 and unixtime >= $2 order by eve.unixtime desc offset $3 rows fetch next $4 rows only");
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
            connection->prepare("get_events_list", "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where is_open = $1 and unixtime >= $2 and unixtime <= $3 order by eve.unixtime desc offset $4 rows fetch next $5 rows only");
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
            std::string query2 = "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') order by eve.unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
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
            std::string query2 = "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and unixtime <= '" + end_time + "' order by eve.unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
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
            std::string query2 = "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and unixtime >= '" + start_time + "'  order by eve.unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
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
            std::string query2 = "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and unixtime >= '" + start_time + "' and unixtime <= '" + end_time + "' order by eve.unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
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
            std::string query2 = "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' order by eve.unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
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
            std::string query2 = "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' and unixtime <= '" + end_time + "' order by eve.unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
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
            std::string query2 = "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' and unixtime >= '" + start_time + "' order by eve.unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
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
            std::string query2 = "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name where (host_identifier like '%" + host_identifier + "%' or hostname like '%" + host_identifier + "%') and is_open = '" + is_open + "' and unixtime >= '" + start_time + "' and unixtime <= '" + end_time + "' order by eve.unixtime desc offset " + std::to_string(row_to_skip) + " rows fetch next " + std::to_string(r2) + " rows only";
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

            connection->prepare("get_events_list", "select eve.*, case when (pr.os_name IS NULL) then 'NULL' else os_name end as os_name, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from events as eve FULL JOIN python_rules as pr ON eve.event_code = pr.name order by eve.unixtime desc offset $1 rows fetch next $2 rows only");
            result = transaction.exec_prepared("get_events_list", row_to_skip, r2);
            connection->unprepare("get_events_list");
        }

        // connection->prepare("get_event_nodes_ip", "select events.*, interface_addresses.address from events inner join interface_addresses on events.host_identifier = interface_addresses.host_identifier where interface_addresses.broadcast <> '' and interface_addresses.interface <> 'lo' order by events.id desc offset $1 rows fetch next $2 rows only");
        // result_ip = transaction.exec_prepared("get_event_nodes_ip", row_to_skip, r2);
        // for (pqxx::result::const_iterator c = result_ip.begin(); c != result_ip.end(); ++c)
        // {
        //     ip_address = c["address"].as<std::string>();
        // }
        // connection->unprepare("get_event_nodes_ip");

        // connection->prepare("get_nodes_os", "select events.*, nodes.os_name, nodes.os_platform, nodes.osquery_version from events inner join nodes on events.host_identifier = nodes.host_identifier order by events.id desc offset $1 rows fetch next $2 rows only");
        // result_ip = transaction.exec_prepared("get_nodes_os", row_to_skip, r2);
        // for (pqxx::result::const_iterator c = result_ip.begin(); c != result_ip.end(); ++c)
        // {
        //     os_name = c["os_name"].as<std::string>();
        //     os_platform = c["os_platform"].as<std::string>();
        //     osquery_version = c["osquery_version"].as<std::string>();

        //     os_names.push_back(os_name);
        //     os_platforms.push_back(os_platform);
        //     osquery_versions.push_back(osquery_version);
        // }
        // connection->unprepare("get_nodes_os");
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
        // for (auto const &row: result)
        // {
        // for (auto const &field: row) std::cout << field.c_str() << '\t';
        // std::cout << std::endl;
        // }
        // int os_index = 0;
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
                // {"address", ip_address},
                // {"os_name", os_names[os_index]},
                // {"os_platform", os_platforms[os_index]},
                // {"osquery_version", osquery_versions[os_index]},
                {"technique", c["technique"].as<std::string>()},
                {"technique_id", c["technique_id"].as<std::string>()},
                {"link", c["link"].as<std::string>()},
                {"alert_type", c["alert_type"].as<std::string>()},
                //{"os", c["os"].as<std::string>()},
                {"created", c["created"].as<long>()},
                {"last_modified", c["last_modified"].as<long>()},
                {"action", c["action"].as<std::string>()},
                {"rule_written_by", c["rule_written_by"].as<std::string>()},
                {"event_id", c["event_id"].as<long>()},
                {"os_name", c["os_name"].as<std::string>()},
                {"pid", c["pid"].as<std::string>()}};

            // if (os_index < result_ip.size())
            // {
            //     os_index = os_index + 1;
            // }
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
    nlohmann::json query_list_json, event_query_json;
    std::string event_id, host_identifier, address = "";
    std::stringstream ss;
    try
    {
        event_query_json = request.query;
        if (event_query_json.contains("event_id"))
        {
            event_id = event_query_json["event_id"];
        }
        if (event_query_json.contains("host_identifier"))
        {
            host_identifier = event_query_json["host_identifier"];
        }

        pqxx::work transaction{*connection};
        pqxx::result result_ip, result;
        std::string ip_address, os_name, os_platform, osquery_version;
        std::vector<std::string> os_names, os_platforms, osquery_versions;

        if (!event_id.empty() && !host_identifier.empty())
        {

            connection->prepare("get_ip_events_list", "select events.*, interface_addresses.address from events inner join interface_addresses on events.host_identifier = interface_addresses.host_identifier where events.id = $1 and events.host_identifier = $2 and interface_addresses.broadcast <> '' and interface_addresses.interface <> 'lo' and (interface_addresses.interface like '%wlx%' or interface_addresses.interface like 'en%' or interface_addresses.interface like 'eth%') order by interface_addresses.id desc limit 1");
            result = transaction.exec_prepared("get_ip_events_list", event_id, host_identifier);
            connection->unprepare("get_ip_events_list");
            connection->prepare("get_nodes_os", "select events.*, nodes.os_name, nodes.os_platform, nodes.osquery_version from events inner join nodes on events.host_identifier = nodes.host_identifier where events.id = $1 and events.host_identifier = $2 order by events.id desc");
            result_ip = transaction.exec_prepared("get_nodes_os", event_id, host_identifier);
            for (pqxx::result::const_iterator c = result_ip.begin(); c != result_ip.end(); ++c)
            {
                os_name = c["os_name"].as<std::string>();
                os_platform = c["os_platform"].as<std::string>();
                osquery_version = c["osquery_version"].as<std::string>();

                os_names.push_back(os_name);
                os_platforms.push_back(os_platform);
                osquery_versions.push_back(osquery_version);
            }
            connection->unprepare("get_nodes_os");
        }
        int os_index = 0;
        for (pqxx::result::const_iterator c = result_ip.begin(); c != result_ip.end(); ++c)
        {
            if (!result.empty())
            {
                address = result.at(0)["address"].as<std::string>();
            }
            query_list_json = {
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
                {"address", address},
                {"os_name", os_names[os_index]},
                {"os_platform", os_platforms[os_index]},
                {"osquery_version", osquery_versions[os_index]},
                {"technique", c["technique"].as<std::string>()},
                {"technique_id", c["technique_id"].as<std::string>()},
                {"pid", c["pid"].as<std::string>()},
                {"link", c["link"].as<std::string>()},
                {"action", c["action"].as<std::string>()},
                {"event_id", c["event_id"].as<long>()},
                {"os_name", c["os_name"].as<std::string>()},
                {"alert_type", c["alert_type"].as<std::string>()}};
            if (os_index < result_ip.size())
            {
                os_index = os_index + 1;
            }
        }
        if (result_ip.size() <= 0)
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
    std::string remarks = "", action = "None";
    std::stringstream ss;
    std::string body;
    int status;
    try
    {
        body = request.body;

        parameters = nlohmann::json::parse(body);
        if (parameters.contains("is_open"))
        {
            is_open = parameters["is_open"];
        }
        if (parameters.contains("remarks"))
        {
            remarks = parameters["remarks"];
        }
        if (parameters.contains("action"))
        {
            action = parameters["action"];
        }
        int x = parameters["event_id"].size();

        for (int i = 0; i < parameters["event_id"].size(); i++)
        {
            arr.push_back(parameters["event_id"][i]);
            event_id = arr[i];
            pqxx::work transaction{*connection};
            pqxx::result result;
            if (!parameters["action"].empty())
            {
                connection->prepare("update_event_open", "update events set action = $1 where id = $2");
                result = transaction.exec_prepared("update_event_open", action, event_id);
                connection->unprepare("update_event_open");
                status = result.affected_rows();
                transaction.commit();
            }
            else
            {
                connection->prepare("update_event_open", "update events set is_open = $1, remarks = $2 where id = $3");
                result = transaction.exec_prepared("update_event_open", is_open, remarks, event_id);
                connection->unprepare("update_event_open");
                status = result.affected_rows();
                transaction.commit();
            }
        }

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

BpfProcessEntry get_parent_from_snapshot(const nlohmann::json &processes, long pid)
{
    try
    {
        long ppid = 0;

        BpfProcessEntry entry;

        if (processes.is_array())
        {
            for (auto process : processes)
            {
                if (process.contains("pid") && process.contains("parent") && process.contains("cmdline") && process.contains("path"))
                {
                    if (process["pid"] == std::to_string(pid))
                    {
                        ppid = atol(process["parent"].get<std::string>().c_str());

                        entry.pid = pid;
                        entry.ppid = ppid;
                        entry.path = process["path"].get<std::string>();
                        entry.cmdline = process["cmdline"].get<std::string>();
                        entry.cwd = process["cwd"].get<std::string>();
                        entry.root_pid = pid;
                        entry.event_type = "Process Event";
                    }
                }
            }
        }

        return entry;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error";
    }
}

WinProcessEntry get_win_parent_from_snapshot(const nlohmann::json &processes, long pid)
{
    try
    {
        long ppid = 0;

        WinProcessEntry entry;

        if (processes.is_array())
        {
            for (auto process : processes)
            {
                if (process.contains("pid") && process.contains("parent") && process.contains("cmdline") && process.contains("path"))
                {
                    if (process["pid"] == std::to_string(pid))
                    {
                        ppid = atol(process["parent"].get<std::string>().c_str());

                        entry.pid = pid;
                        entry.ppid = ppid;
                        entry.path = process["path"].get<std::string>();
                        entry.command_line = process["cmdline"].get<std::string>();
                        entry.current_working_directory = process["cwd"].get<std::string>();
                        entry.root_pid = pid;
                        entry.event_type = "Process Event";
                    }
                }
            }
        }

        return entry;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error";
    }
}

std::vector<BpfProcessEntry> snapshot_process_tree(std::shared_ptr<pqxx::connection> connection, long ppid,
                                                   long timestamp, long uptime, std::string host_identifier)
{
    try
    {
        std::vector<BpfProcessEntry> process_list;
        std::vector<long> root;

        connection->prepare("get_snapshot_data_before", "select * from snapshot_result where unixTime < $1 and query_name = $2 "
                                                        "and host_identifier = $3 order by id desc limit 1");
        connection->prepare("get_snapshot_data_after", "select * from snapshot_result where unixTime > $1 and query_name = $2 "
                                                       "and host_identifier = $3 order by id asc limit 1");
        pqxx::work transaction{*connection};

        pqxx::result result = transaction.exec_prepared("get_snapshot_data_before", timestamp, "processes", host_identifier);
        transaction.commit();

        if (result.empty())
        {
            result = transaction.exec_prepared("get_snapshot_data_after", timestamp, "processes", host_identifier);
            transaction.commit();
        }
        else
        {
            /*
             * We need to ensure that previous snapshot record is from current sessions. Which means, that unixTime of
             * snapshot record must be greater than timestamp - uptime (i.e. timestamp of system startup).
             */

            long snapshot_unitxTime = result.at(0)["unixTime"].as<long>();
            if (snapshot_unitxTime < (timestamp - uptime))
            {
                // the snapshot result is from previous sesssion. We need to check immediate future entry.
                result = transaction.exec_prepared("get_snapshot_data_after", timestamp, "processes", host_identifier);
                transaction.commit();
            }
        }

        if (!result.empty())
        {
            std::string raw_process_list = result.at(0)["result"].as<std::string>();

            nlohmann::json processes = nlohmann::json::parse(raw_process_list);
            BpfProcessEntry entry = get_parent_from_snapshot(processes, ppid);
            long _tmp_pid = entry.ppid;
            long r_id;

            if (entry.pid)
                process_list.push_back(entry);

            while (_tmp_pid != 0)
            {
                entry = get_parent_from_snapshot(processes, _tmp_pid);
                if (!entry.pid == 0)
                {
                    r_id = entry.pid;
                }

                if (!r_id == 0)
                {
                    root.push_back(r_id);
                }
                entry.root_pid = root.back();
                process_list.push_back(entry);
                _tmp_pid = entry.ppid;
            }
        }

        connection->unprepare("get_snapshot_data_before");
        connection->unprepare("get_snapshot_data_after");

        return process_list;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error";
    }
}

std::vector<WinProcessEntry> win_snapshot_process_tree(std::shared_ptr<pqxx::connection> connection, long ppid,
                                                       long timestamp, long uptime, std::string host_identifier)
{
    try
    {
        std::vector<WinProcessEntry> process_list;
        std::vector<long> root;

        connection->prepare("get_snapshot_data_before", "select * from snapshot_result where unixTime < $1 and query_name = $2 "
                                                        "and host_identifier = $3 order by id desc limit 1");
        connection->prepare("get_snapshot_data_after", "select * from snapshot_result where unixTime > $1 and query_name = $2 "
                                                       "and host_identifier = $3 order by id asc limit 1");
        pqxx::work transaction{*connection};

        pqxx::result result = transaction.exec_prepared("get_snapshot_data_before", timestamp, "processes", host_identifier);
        transaction.commit();

        if (result.empty())
        {
            result = transaction.exec_prepared("get_snapshot_data_after", timestamp, "processes", host_identifier);
            transaction.commit();
        }
        else
        {
            /*
             * We need to ensure that previous snapshot record is from current sessions. Which means, that unixTime of
             * snapshot record must be greater than timestamp - uptime (i.e. timestamp of system startup).
             */

            long snapshot_unitxTime = result.at(0)["unixTime"].as<long>();
            if (snapshot_unitxTime < (timestamp - uptime))
            {
                // the snapshot result is from previous sesssion. We need to check immediate future entry.
                result = transaction.exec_prepared("get_snapshot_data_after", timestamp, "processes", host_identifier);
                transaction.commit();
            }
        }

        if (!result.empty())
        {
            std::string raw_process_list = result.at(0)["result"].as<std::string>();
            // std::cout << raw_process_list << std::endl;
            nlohmann::json processes = nlohmann::json::parse(raw_process_list);
            WinProcessEntry entry = get_win_parent_from_snapshot(processes, ppid);
            long _tmp_pid = entry.ppid;
            long r_id;

            if (entry.pid)
                process_list.push_back(entry);

            while (_tmp_pid != 0)
            {
                entry = get_win_parent_from_snapshot(processes, _tmp_pid);
                if (!entry.pid == 0 && !entry.parent == 0)
                {
                    r_id = entry.pid;
                }

                if (!r_id == 0)
                {
                    root.push_back(r_id);
                }
                entry.root_pid = root.back();

                process_list.push_back(entry);
                _tmp_pid = entry.ppid;
            }
        }

        connection->unprepare("get_snapshot_data_before");
        connection->unprepare("get_snapshot_data_after");

        return process_list;
        root.clear();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error";
    }
}

std::vector<BpfProcessEntry> event_bpf_backtrack_process_tree(std::shared_ptr<pqxx::connection> connection, long record_id)
{
    HTTPMessage response;
    nlohmann::json parameters;
    nlohmann::json forward_tree;
    int event_id;
    nlohmann::json arr = nlohmann::json::array();

    std::string remarks = "", process_guid, parent_process_guid;
    std::stringstream ss;
    std::string body;
    int status;
    nlohmann::json process_tree;

    try
    {
        connection->prepare("get_bpf_process_data_from_id", "select * from bpf_process_events where id = $1");

        /*
         * Since events can arrive out of order, and at some delay (in case osquery cannot reach fleet server for some reason, we cannot rely upon record ID in database.
         * If we want to get latest record of event, we should order by time column instead, which records timestamp at which the event occurred. Since it comes a
         * monotonic clock, it will never be out of order.
         */

        connection->prepare("get_bpf_process_data_execve", "select * from bpf_process_events where cmdline <> '' and "
                                                           "pid = $1 and host_identifier = $2 order by time desc limit 1");
        connection->prepare("get_bpf_process_data_non_execve", "select * from bpf_process_events where cmdline = '' and "
                                                               "pid = $1 and host_identifier = $2 order by time desc limit 1");
        connection->prepare("get_docker_events", "select * from docker_events where status = 'start' and time between $1 and $2 and "
                                                 "pid = $3 and host_identifier = $4");
        std::vector<BpfProcessEntry> process_list;
        pqxx::result result, container_result;
        BpfProcessEntry entry;

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_bpf_process_data_from_id", record_id);

            entry.pid = result.at(0)["pid"].as<long>();
            entry.ppid = result.at(0)["parent"].as<long>();
            entry.tid = result.at(0)["tid"].as<long long>();
            entry.uid = result.at(0)["uid"].as<long long>();
            entry.gid = result.at(0)["gid"].as<long long>();
            entry.cid = result.at(0)["cid"].as<long long>();
            entry.cmdline = result.at(0)["cmdline"].as<std::string>();
            entry.exit_code = result.at(0)["exit_code"].as<std::string>();
            entry.probe_error = result.at(0)["probe_error"].as<long long>();
            entry.syscall = result.at(0)["syscall"].as<std::string>();
            entry.path = result.at(0)["path"].as<std::string>();
            entry.cwd = result.at(0)["cwd"].as<std::string>();
            entry.duration = result.at(0)["duration"].as<long long>();
            entry.json_cmdline = result.at(0)["json_cmdline"].as<std::string>();
            entry.ntime = result.at(0)["ntime"].as<std::string>();
            entry.time = result.at(0)["time"].as<long>();
            entry.eid = result.at(0)["eid"].as<long long>();
            entry.md5 = result.at(0)["md5"].as<std::string>();
            entry.sha1 = result.at(0)["sha1"].as<std::string>();
            entry.sha256 = result.at(0)["sha256"].as<std::string>();
            entry.is_container_process = result.at(0)["is_container_process"].as<long long>();
            entry.container_age = result.at(0)["container_age"].as<long long>();
            entry.container_name = result.at(0)["container_name"].as<std::string>();
            entry.uptime = result.at(0)["uptime"].as<long>();
            entry.root_pid = result.at(0)["pid"].as<long>();
            entry.event_type = "Process Event";
            process_list.push_back(entry);

            transaction.commit();
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

            std::string path, cmdline, host_identifier, container_name, container_id;
            long container_pid;
            path = result.at(0)["path"].as<std::string>();
            cmdline = result.at(0)["cmdline"].as<std::string>();
            host_identifier = result.at(0)["host_identifier"].as<std::string>();

            long timestamp = result.at(0)["time"].as<long>();
            long uptime = result.at(0)["uptime"].as<long>();

            // Checking for container process
            pqxx::work transaction{*connection};
            container_result = transaction.exec_prepared("get_docker_events", timestamp - 3600, timestamp + 3600, ppid, host_identifier);
            transaction.commit();
            if (!container_result.empty())
            {
                container_pid = container_result.at(0)["pid"].as<long>();
                container_name = container_result.at(0)["container_name"].as<std::string>();
                container_id = container_result.at(0)["container_id"].as<std::string>();
                if (pid == container_pid or ppid == container_pid)
                {
                    connection->prepare("update_container_process", "update bpf_process_events set is_container_process = '1' where id = $1");
                    transaction.exec_prepared("update_container_process", record_id);
                    transaction.commit();
                    connection->unprepare("update_container_process");
                }
            }
            /*
             * First step: we prefer entries from process_events.
             */
            while (loop)
            {
                result = transaction.exec_prepared("get_bpf_process_data_execve", ppid, host_identifier);
                transaction.commit();

                BpfProcessEntry entry;

                if (result.empty())
                {
                    result = transaction.exec_prepared("get_bpf_process_data_non_execve", ppid, host_identifier);
                    transaction.commit();
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

                    long new_timestamp = result.at(0)["time"].as<long>();
                    long new_uptime = result.at(0)["uptime"].as<long>();

                    long timestamp_delta = timestamp - new_timestamp;
                    long uptime_delta = uptime - new_uptime;

                    /*
                     * timestamp and uptime are measured in sequence; and due to rounding, delta may not be exactly same.
                     * A time gap of 3 seconds is chosen here
                     */
                    if (std::abs(timestamp_delta - uptime_delta) > 3)
                    {
                        // system rebooted somewhere between these two
                        loop = false;

                        // we need to discard the result, as it is from different session
                        continue;
                    }
                    else
                    {
                        timestamp = new_timestamp;
                        uptime = new_uptime;
                    }

                    path = result.at(0)["path"].as<std::string>();
                    cmdline = result.at(0)["cmdline"].as<std::string>();
                    long tmp_ppid = result.at(0)["parent"].as<long>();

                    timestamp = result.at(0)["time"].as<long>();

                    entry.pid = ppid;
                    entry.ppid = tmp_ppid;

                    entry.tid = result.at(0)["tid"].as<long long>();
                    entry.uid = result.at(0)["uid"].as<long long>();
                    entry.gid = result.at(0)["gid"].as<long long>();
                    entry.cid = result.at(0)["cid"].as<long long>();
                    entry.exit_code = result.at(0)["exit_code"].as<std::string>();
                    entry.probe_error = result.at(0)["probe_error"].as<long long>();
                    entry.syscall = result.at(0)["syscall"].as<std::string>();
                    entry.path = path;
                    entry.cwd = result.at(0)["cwd"].as<std::string>();
                    entry.cmdline = cmdline;
                    entry.duration = result.at(0)["duration"].as<long long>();
                    entry.json_cmdline = result.at(0)["json_cmdline"].as<std::string>();
                    entry.ntime = result.at(0)["ntime"].as<std::string>();
                    entry.time = timestamp;
                    entry.eid = result.at(0)["eid"].as<long long>();
                    entry.md5 = result.at(0)["md5"].as<std::string>();
                    entry.sha1 = result.at(0)["sha1"].as<std::string>();
                    entry.sha256 = result.at(0)["sha256"].as<std::string>();
                    entry.is_container_process = result.at(0)["is_container_process"].as<long long>();
                    entry.container_age = result.at(0)["container_age"].as<long long>();
                    entry.container_name = result.at(0)["container_name"].as<std::string>();
                    entry.root_pid = result.at(0)["pid"].as<long long>();
                    entry.event_type = "Process Event";

                    ppid = tmp_ppid;

                    process_list.push_back(entry);
                }
                else
                    loop = false;
            }
            /*
             * Since we did not find more entries in process_events, we check in snapshot_result table.
             *
             * We first pick the previous snapshot result, because if there is no process event, the process must be
             * alive before it can spawn any other process.
             *
             * If the previous entry does not exist, we check the immediate future entry with a hope that it will have
             * process entry (for example, event from processes started before osquery generated an event after osquery,
             * but before first snapshot result arrived.
             */

            std::vector<BpfProcessEntry> _tmp_process_list = snapshot_process_tree(connection, ppid, timestamp, uptime, host_identifier);
            process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());

            connection->unprepare("get_bpf_process_data_from_id");
            connection->unprepare("get_bpf_process_data_execve");
            connection->unprepare("get_bpf_process_data_non_execve");
            connection->unprepare("get_docker_events");
            return process_list;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error";
    }
}

std::vector<BpfProcessEntry> get_bpf_socket_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
    try
    {
        connection->prepare("get_socket_data_from_id", "select * from bpf_socket_events where id = $1");
        connection->prepare("get_process_data", "select * from bpf_process_events where pid = $1 "
                                                "and host_identifier = $2 and time <= $3 and path = $4"
                                                "order by id desc limit 1");

        std::vector<BpfProcessEntry> process_list;
        pqxx::result result;

        {
            BpfProcessEntry entry;
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_socket_data_from_id", record_id);
            transaction.commit();

            entry.pid = result.at(0)["pid"].as<long>();

            entry.path = result.at(0)["path"].as<std::string>();
            entry.host_identifier = result.at(0)["host_identifier"].as<std::string>();
            entry.hostname = result.at(0)["hostname"].as<std::string>();
            // entry.parent = result.at(0)["parent"].as<long>();
            entry.ppid = result.at(0)["parent"].as<long>();
            entry.uid = result.at(0)["uid"].as<long>();
            entry.gid = result.at(0)["gid"].as<long>();
            entry.cid = result.at(0)["cid"].as<long>();
            entry.exit_code = result.at(0)["exit_code"].as<std::string>();
            entry.syscall = result.at(0)["syscall"].as<std::string>();
            entry.protocol = result.at(0)["protocol"].as<int>();
            entry.local_address = result.at(0)["local_address"].as<std::string>();
            entry.remote_address = result.at(0)["remote_address"].as<std::string>();
            entry.local_port = result.at(0)["local_port"].as<int>();
            entry.remote_port = result.at(0)["remote_port"].as<int>();
            entry.duration = result.at(0)["duration"].as<long>();
            entry.event_type = "Socket Event";
            entry.timestamp = result.at(0)["time"].as<long>();
            entry.uptime = result.at(0)["uptime"].as<long>();
            entry.root_pid = result.at(0)["pid"].as<long>();
            process_list.push_back(entry);
        }

        if (result.empty())
        {
            std::cerr << "No process found with record ID " << record_id << std::endl;
        }
        else if (result.size() != 1)
        {
            std::cerr << "Multiple processes found with record ID " << record_id << std::endl;
        }
        else
        {
            long pid = result.at(0)["pid"].as<long>();

            std::string path, host_identifier;

            path = result.at(0)["path"].as<std::string>();
            host_identifier = result.at(0)["host_identifier"].as<std::string>();

            long timestamp = result.at(0)["time"].as<long>();
            long uptime = result.at(0)["uptime"].as<long>();

            {
                pqxx::work transaction{*connection};
                result = transaction.exec_prepared("get_process_data", pid, host_identifier, timestamp, path);
                transaction.commit();
            }

            if (!result.empty())
            {
                long process_record_id = result.at(0)["id"].as<long>();
                std::vector<BpfProcessEntry> _tmp_process_list = event_bpf_backtrack_process_tree(connection, process_record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
            }
            else
            {
                //     /*
                //     * It seems that socket event is coming from a process that was started *before* osquery was started.
                //     * We have to pick data from snapshot.
                //     *
                //     * Since we have PID, instead of PPID, we have to remove first entry in process list.
                //     */
                std::vector<BpfProcessEntry> _tmp_process_list = snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
                // if (process_list.size() > 1)
                //     process_list.erase(process_list.begin());

    
            }
        }

        connection->unprepare("get_socket_data_from_id");
        connection->unprepare("get_process_data");

        return process_list;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get max node severity count";
    }
}

std::vector<BpfProcessEntry> get_bpf_file_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
    try
    {
        connection->prepare("get_file_data_from_id", "select * from bpf_file_events where id = $1");
        connection->prepare("get_process_data", "select * from bpf_process_events where pid = $1 "
                                                "and host_identifier = $2 and time <= $3 and path = $4"
                                                "order by id desc limit 1");

        std::vector<BpfProcessEntry> process_list;
        pqxx::result result;

        {
            BpfProcessEntry entry_file;
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_file_data_from_id", record_id);
            transaction.commit();

            entry_file.pid = result.at(0)["pid"].as<long>();
            entry_file.path = result.at(0)["path"].as<std::string>();
            entry_file.host_identifier = result.at(0)["host_identifier"].as<std::string>();
            entry_file.hostname = result.at(0)["hostname"].as<std::string>();
            // entry_file.parent = result.at(0)["parent"].as<long>();
            entry_file.ppid = result.at(0)["parent"].as<long>();
            entry_file.uid = result.at(0)["uid"].as<long>();
            entry_file.gid = result.at(0)["gid"].as<long>();
            entry_file.cid = result.at(0)["cid"].as<long>();
            entry_file.exit_code = result.at(0)["exit_code"].as<std::string>();
            entry_file.syscall = result.at(0)["syscall"].as<std::string>();
            entry_file.file_path = result.at(0)["file_path"].as<std::string>();
            entry_file.flags = result.at(0)["flags"].as<std::string>();
            entry_file.duration = result.at(0)["duration"].as<long>();
            entry_file.root_pid = result.at(0)["pid"].as<long>();
            entry_file.timestamp = result.at(0)["time"].as<long>();
            entry_file.uptime = result.at(0)["uptime"].as<long>();
            entry_file.event_type = "File Event";
            process_list.push_back(entry_file);
        }

        if (result.empty())
        {
            std::cerr << "No process found with record ID " << record_id << std::endl;
        }
        else if (result.size() != 1)
        {
            std::cerr << "Multiple processes found with record ID " << record_id << std::endl;
        }
        else
        {
            long pid = result.at(0)["pid"].as<long>();

            std::string path, host_identifier;

            path = result.at(0)["path"].as<std::string>();
            host_identifier = result.at(0)["host_identifier"].as<std::string>();

            long timestamp = result.at(0)["time"].as<long>();
            long uptime = result.at(0)["uptime"].as<long>();

            {
                pqxx::work transaction{*connection};
                result = transaction.exec_prepared("get_process_data", pid, host_identifier, timestamp, path);
                transaction.commit();
            }

            if (!result.empty())
            {
                long process_record_id = result.at(0)["id"].as<long>();               
                std::vector<BpfProcessEntry> _tmp_process_list = event_bpf_backtrack_process_tree(connection, process_record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
            }
            else
            {
                /*
                 * It seems that socket event is coming from a process that was started *before* osquery was started.
                 * We have to pick data from snapshot.
                 *
                 * Since we have PID, instead of PPID, we have to remove first entry in process list.
                 */
                std::vector<BpfProcessEntry> _tmp_process_list = snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
                // if (file_process_list.size() > 1)
                //     file_process_list.erase(file_process_list.begin());
            }
        }

        connection->unprepare("get_file_data_from_id");
        connection->unprepare("get_process_data");

        return process_list;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get max node severity count";
    }
}

std::vector<WinProcessEntry> event_win_backtrack_process_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
    try
    {
        connection->prepare("get_win_process_data_from_id", "select * from win_process_events where id = $1");

        /*
         * Since events can arrive out of order, and at some delay (in case osquery cannot reach fleet server for some reason, we cannot rely upon record ID in database.
         * If we want to get latest record of event, we should order by time column instead, which records timestamp at which the event occurred. Since it comes a
         * monotonic clock, it will never be out of order.
         */

        connection->prepare("get_win_process_data_execve", "select * from win_process_events where cmdline <> '' and "
                                                           "process_guid = $1 and host_identifier = $2 order by time desc limit 1");
        connection->prepare("get_win_process_data_non_execve", "select * from win_process_events where cmdline = '' and "
                                                               "process_guid = $1 and host_identifier = $2 order by time desc limit 1");
        connection->prepare("get_docker_events", "select * from docker_events where status = 'start' and time between $1 and $2 and "
                                                 "pid = $3 and host_identifier = $4");
        std::vector<WinProcessEntry> process_list;
        pqxx::result result, container_result;
        WinProcessEntry entry;

        {
            pqxx::work transaction{*connection};

            result = transaction.exec_prepared("get_win_process_data_from_id", record_id);

            entry.process_guid = result.at(0)["process_guid"].as<std::string>();
            entry.action = result.at(0)["action"].as<std::string>();
            entry.eid = result.at(0)["eid"].as<std::string>();
            entry.pid = result.at(0)["pid"].as<long>();
            entry.path = entry.path = result.at(0)["path"].as<std::string>();
            entry.cmdline = entry.cmdline = result.at(0)["cmdline"].as<std::string>();
            entry.ppid = result.at(0)["parent_pid"].as<long>();
            entry.parent_process_guid = result.at(0)["parent_process_guid"].as<std::string>();
            entry.parent_path = result.at(0)["parent_path"].as<std::string>();
            entry.owner_uid = result.at(0)["owner_uid"].as<std::string>();
            entry.time = result.at(0)["time"].as<long long>();
            entry.utc_time = result.at(0)["utc_time"].as<std::string>();
            entry.eventid = result.at(0)["eventid"].as<int>();
            entry.sha256 = result.at(0)["sha256"].as<std::string>();
            entry.uptime = result.at(0)["uptime"].as<long>();
            entry.root_pid = result.at(0)["pid"].as<long>();
            entry.event_type = "Process Event";

            process_list.push_back(entry);

            transaction.commit();
        }

        if (result.empty())
        {
            std::cerr << "No process found with record ID" << record_id << std::endl;
        }
        else if (result.size() != 1)
        {
            std::cerr << "Multiple processes found with record ID " << record_id << std::endl;
        }
        else
        {
            // we have only one process entry now.
            bool loop = true;
            long ppid = result.at(0)["parent_pid"].as<long>();
            long pid = result.at(0)["pid"].as<long>();

            std::string path, cmdline, host_identifier, container_name, container_id, process_guid, parent_process_guid, tmp_process_guid, tmp_parent_process_guid;
            long container_pid;
            path = result.at(0)["path"].as<std::string>();
            cmdline = result.at(0)["cmdline"].as<std::string>();
            host_identifier = result.at(0)["host_identifier"].as<std::string>();
            process_guid = result.at(0)["process_guid"].as<std::string>();
            parent_process_guid = result.at(0)["parent_process_guid"].as<std::string>();

            long timestamp = result.at(0)["time"].as<long>();
            long uptime = result.at(0)["uptime"].as<long>();

            // Checking for container process
            pqxx::work transaction{*connection};
            container_result = transaction.exec_prepared("get_docker_events", timestamp - 3600, timestamp + 3600, ppid, host_identifier);
            transaction.commit();
            if (!container_result.empty())
            {
                container_pid = container_result.at(0)["pid"].as<long>();
                container_name = container_result.at(0)["container_name"].as<std::string>();
                container_id = container_result.at(0)["container_id"].as<std::string>();
                if (pid == container_pid or ppid == container_pid)
                {
                    connection->prepare("update_container_process", "update win_process_events set is_container_process = '1' where id = $1");
                    transaction.exec_prepared("update_container_process", record_id);
                    transaction.commit();
                    connection->unprepare("update_container_process");
                }
            }
            /*
             * First step: we prefer entries from process_events.
             */
            while (loop)
            {
                result = transaction.exec_prepared("get_win_process_data_execve", parent_process_guid, host_identifier);
                transaction.commit();

                WinProcessEntry entry;

                if (result.empty())
                {
                    result = transaction.exec_prepared("get_win_process_data_non_execve", parent_process_guid, host_identifier);
                    transaction.commit();
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

                    long new_timestamp = result.at(0)["time"].as<long>();
                    long new_uptime = result.at(0)["uptime"].as<long>();

                    long timestamp_delta = timestamp - new_timestamp;
                    long uptime_delta = uptime - new_uptime;

                    /*
                     * timestamp and uptime are measured in sequence; and due to rounding, delta may not be exactly same.
                     * A time gap of 3 seconds is chosen here
                     */
                    if (std::abs(timestamp_delta - uptime_delta) > 3)
                    {
                        // system rebooted somewhere between these two
                        loop = false;

                        // we need to discard the result, as it is from different session
                        continue;
                    }
                    else
                    {
                        timestamp = new_timestamp;
                        uptime = new_uptime;
                    }

                    path = result.at(0)["path"].as<std::string>();
                    cmdline = result.at(0)["cmdline"].as<std::string>();
                    long tmp_ppid = result.at(0)["parent_pid"].as<long>();
                    tmp_process_guid = result.at(0)["process_guid"].as<std::string>();

                    timestamp = result.at(0)["time"].as<long>();

                    entry.pid = ppid;
                    entry.ppid = tmp_ppid;
                    entry.process_guid = parent_process_guid;
                    entry.parent_process_guid = tmp_parent_process_guid;

                    entry.action = result.at(0)["action"].as<std::string>();
                    entry.eid = result.at(0)["eid"].as<std::string>();
                    entry.process_guid = result.at(0)["process_guid"].as<std::string>();
                    // entry.parent_process_guid = result.at(0)["parent_process_guid"].as<std::string>();
                    // entry.parent_path  = result.at(0)["parent_path"].as<std::string>();
                    entry.owner_uid = result.at(0)["owner_uid"].as<std::string>();
                    entry.utc_time = result.at(0)["utc_time"].as<std::string>();
                    entry.path = path;
                    entry.eventid = result.at(0)["eventid"].as<long long>();
                    entry.command_line = cmdline;
                    entry.sha256 = result.at(0)["sha256"].as<std::string>();
                    ppid = tmp_ppid;
                    entry.root_pid = result.at(0)["pid"].as<long>();
                    parent_process_guid = tmp_parent_process_guid;
                    entry.event_type = "Process Event";

                    process_list.push_back(entry);
                }
                else
                    loop = false;
            }
            /*
             * Since we did not find more entries in process_events, we check in snapshot_result table.
             *
             * We first pick the previous snapshot result, because if there is no process event, the process must be
             * alive before it can spawn any other process.
             *
             * If the previous entry does not exist, we check the immediate future entry with a hope that it will have
             * process entry (for example, event from processes started before osquery generated an event after osquery,
             * but before first snapshot result arrived.
             */

            std::vector<WinProcessEntry> _tmp_process_list = win_snapshot_process_tree(connection, ppid, timestamp, uptime, host_identifier);
            process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
        }

        connection->unprepare("get_win_process_data_from_id");
        connection->unprepare("get_win_process_data_execve");
        connection->unprepare("get_win_process_data_non_execve");
        connection->unprepare("get_docker_events");

        return process_list;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get max node severity count";
    }
}

std::vector<WinProcessEntry> get_win_socket_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
    try
    {
        connection->prepare("get_socket_data_from_id", "select * from win_socket_events where id = $1");
        connection->prepare("get_process_data", "select * from win_process_events where process_guid = $1 "
                                                "and host_identifier = $2 and time <= $3 and path = $4"
                                                "order by id desc limit 1");

        std::vector<WinProcessEntry> process_list;
        pqxx::result result;

        {
            WinProcessEntry entry_soc;
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_socket_data_from_id", record_id);
            transaction.commit();

            entry_soc.pid = result.at(0)["pid"].as<long>();
            entry_soc.host_identifier = result.at(0)["host_identifier"].as<std::string>();
            entry_soc.hostname = result.at(0)["hostname"].as<std::string>();
            entry_soc.parent = result.at(0)["parent_pid"].as<long>();
            entry_soc.action = result.at(0)["action"].as<std::string>();
            entry_soc.eid = result.at(0)["eid"].as<std::string>();
            entry_soc.timestamp = result.at(0)["time"].as<long>();
            entry_soc.utc_time = result.at(0)["utc_time"].as<std::string>();
            entry_soc.process_guid = result.at(0)["process_guid"].as<std::string>();
            entry_soc.process_name = result.at(0)["process_name"].as<std::string>();
            entry_soc.family = result.at(0)["family"].as<std::string>();
            entry_soc.protocol = result.at(0)["protocol"].as<int>();
            entry_soc.local_address = result.at(0)["local_address"].as<std::string>();
            entry_soc.remote_address = result.at(0)["remote_address"].as<std::string>();
            entry_soc.local_port = result.at(0)["local_port"].as<int>();
            entry_soc.remote_port = result.at(0)["remote_port"].as<int>();
            entry_soc.eventid = result.at(0)["eventid"].as<int>();
            entry_soc.unixtime = result.at(0)["unixTime"].as<long>();
            entry_soc.uptime = result.at(0)["uptime"].as<long>();
            entry_soc.event_type = "Socket Event";
            entry_soc.root_pid = result.at(0)["pid"].as<long>();
            process_list.push_back(entry_soc);
        }

        if (result.empty())
        {
            std::cerr << "No process found with record ID " << record_id << std::endl;
        }
        else if (result.size() != 1)
        {
            std::cerr << "Multiple processes found with record ID " << record_id << std::endl;
        }
        else
        {
            long pid = result.at(0)["pid"].as<long>();

            std::string process_name, path, host_identifier, process_guid;

            path = result.at(0)["process_name"].as<std::string>();
            host_identifier = result.at(0)["host_identifier"].as<std::string>();

            long timestamp = result.at(0)["time"].as<long>();
            long uptime = result.at(0)["uptime"].as<long>();
            process_guid = result.at(0)["process_guid"].as<std::string>();

            {
                pqxx::work transaction{*connection};
                result = transaction.exec_prepared("get_process_data", process_guid, host_identifier, timestamp, path);
                transaction.commit();
            }

            if (!result.empty())
            {
                long process_record_id = result.at(0)["id"].as<long>();
                std::vector<WinProcessEntry> _tmp_process_list = event_win_backtrack_process_tree(connection, process_record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
            }
            else
            {
                //     /*
                //     * It seems that socket event is coming from a process that was started *before* osquery was started.
                //     * We have to pick data from snapshot.
                //     *
                //     * Since we have PID, instead of PPID, we have to remove first entry in process list.
                //     */
                std::vector<WinProcessEntry> _tmp_process_list = win_snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());

                // if (socket_process_list.size() > 1)
                //     socket_process_list.erase(socket_process_list.begin());
            }
        }

        connection->unprepare("get_socket_data_from_id");
        connection->unprepare("get_process_data");

        return process_list;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get max node severity count";
    }
}

std::vector<WinProcessEntry> get_win_file_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
    try
    {
        connection->prepare("get_file_data_from_id", "select * from win_file_events where id = $1");
        connection->prepare("get_process_data", "select * from win_process_events where process_guid = $1 "
                                                "and host_identifier = $2 and time <= $3 and path = $4"
                                                "order by id desc limit 1");

        std::vector<WinProcessEntry> process_list;
        pqxx::result result;

        {
            WinProcessEntry entry_file;
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_file_data_from_id", record_id);
            transaction.commit();
            entry_file.pid = result.at(0)["pid"].as<long>();
            entry_file.host_identifier = result.at(0)["host_identifier"].as<std::string>();
            entry_file.hostname = result.at(0)["hostname"].as<std::string>();
            entry_file.parent = result.at(0)["parent_pid"].as<long>();
            entry_file.action = result.at(0)["action"].as<std::string>();
            entry_file.eid = result.at(0)["eid"].as<std::string>();
            entry_file.target_path = result.at(0)["target_path"].as<std::string>();
            entry_file.md5 = result.at(0)["md5"].as<std::string>();
            entry_file.sha256 = result.at(0)["sha256"].as<std::string>();
            entry_file.hashed = result.at(0)["hashed"].as<int>();
            entry_file.uid = result.at(0)["uid"].as<std::string>();
            entry_file.utc_time = result.at(0)["utc_time"].as<std::string>();
            entry_file.unixtime = result.at(0)["unixTime"].as<long>();
            entry_file.timestamp = result.at(0)["time"].as<long>();
            entry_file.uptime = result.at(0)["uptime"].as<long>();
            entry_file.process_guid = result.at(0)["process_guid"].as<std::string>();
            entry_file.process_name = result.at(0)["process_name"].as<std::string>();
            entry_file.pe_file = result.at(0)["pe_file"].as<std::string>();
            entry_file.eventid = result.at(0)["eventid"].as<int>();
            entry_file.root_pid= result.at(0)["pid"].as<long>();
            entry_file.event_type = "File Event";

            process_list.push_back(entry_file);
        }

        if (result.empty())
        {
            std::cerr << "No process found with record ID " << record_id << std::endl;
        }
        else if (result.size() != 1)
        {
            std::cerr << "Multiple processes found with record ID " << record_id << std::endl;
        }
        else
        {
            long pid = result.at(0)["pid"].as<long>();

            std::string path, host_identifier, process_guid;

            path = result.at(0)["target_path"].as<std::string>();
            host_identifier = result.at(0)["host_identifier"].as<std::string>();

            long timestamp = result.at(0)["time"].as<long>();
            long uptime = result.at(0)["uptime"].as<long>();
            process_guid = result.at(0)["process_guid"].as<std::string>();

            {
                pqxx::work transaction{*connection};
                result = transaction.exec_prepared("get_process_data", process_guid, host_identifier, timestamp, path);
                transaction.commit();
            }

            if (!result.empty())
            {
                long process_record_id = result.at(0)["id"].as<long>();
                std::vector<WinProcessEntry> _tmp_process_list = event_win_backtrack_process_tree(connection, process_record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
            }
            else
            {
                /*
                 * It seems that file event is coming from a process that was started *before* osquery was started.
                 * We have to pick data from snapshot.
                 *
                 * Since we have PID, instead of PPID, we have to remove first entry in process list.
                 */
                std::vector<WinProcessEntry> _tmp_process_list = win_snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());

                // if (file_process_list.size() > 1)
                //     file_process_list.erase(file_process_list.begin());
            }
        }

        connection->unprepare("get_file_data_from_id");
        connection->unprepare("get_process_data");

        return process_list;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get max node severity count";
    }
}

HTTPMessage event_process_tree(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json parameters;
    nlohmann::json forward_tree;
    int event_id;
    nlohmann::json arr = nlohmann::json::array();

    long pid, record_id;
    std::string remarks = "", os_name, process_guid, parent_process_guid;
    std::stringstream ss;
    std::string body;
    std::string host_identifier, alert_type;
    bool soc_event, file_event;
    int status;
    nlohmann::json process_tree;
    try
    {
        body = request.body;
        parameters = nlohmann::json::parse(body);
        if (parameters.contains("os_name"))
        {
            os_name = parameters["os_name"];
        }
        if (parameters.contains("host_identifier"))
        {
            host_identifier = parameters["host_identifier"];
        }
        if (parameters.contains("id"))
        {
            record_id = parameters["id"];
        }
        if (parameters.contains("alert_type"))
        {
            alert_type = parameters["alert_type"];
        }
        if (os_name == "Linux")
        {

            soc_event = parameters["socket_event"];
            file_event = parameters["file_event"];
            std::vector<BpfProcessEntry> process_list;
            // below code is to make backward process tree
            if (alert_type == "File Alert")
            {

                std::vector<BpfProcessEntry> _tmp_process_list = get_bpf_file_tree(connection, record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
                std::cout << process_list.size() << std::endl;
            }
            else if (alert_type == "Socket Alert")
            {
                std::vector<BpfProcessEntry> _tmp_process_list = get_bpf_socket_tree(connection, record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
                std::cout << process_list.size() << std::endl;
            }
            else
            {
                std::vector<BpfProcessEntry> _tmp_process_list = event_bpf_backtrack_process_tree(connection, record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
                std::cout << process_list.size() << std::endl;
            }
            // From here we will make forward process tree

            // how to get pid or process_guid ?????????????????????????????????????????????????
            // we have id and alert_type now we'll check as per alert_type in which table we have to look into to get pid or process_guid.
            {
                pqxx::result result;
                BpfProcessEntry entry;
                BpfProcessEntry entry_soc;
                BpfProcessEntry entry_file;
                entry_soc.event_type = "Socket Event";
                entry_file.event_type = "File Event";
                entry.event_type = "Process Event";
                pqxx::work transaction{*connection};
                if(alert_type == "Socket Alert")
                {
                    connection->prepare("get_bpf_socket_pid", "select pid from bpf_socket_events where id = $1");
                    result = transaction.exec_prepared("get_bpf_socket_pid", record_id);
                    connection->unprepare("get_bpf_socket_pid");
                    pid = result.at(0)["pid"].as<long>();
                }
                else if (alert_type == "File Alert")
                {
                    connection->prepare("get_bpf_file_pid", "select pid from bpf_file_events where id = $1");
                    result = transaction.exec_prepared("get_bpf_file_pid", record_id);
                    connection->unprepare("get_bpf_file_pid");
                    pid = result.at(0)["pid"].as<long>();
                }
                else
                {
                    connection->prepare("get_bpf_process_pid", "select pid from bpf_process_events where id = $1");
                    result = transaction.exec_prepared("get_bpf_process_pid", record_id);
                    connection->unprepare("get_bpf_process_pid");
                    pid = result.at(0)["pid"].as<long>();
                }
                
                connection->prepare("get_bpf_process_event_id", "select id from bpf_process_events where pid = $1 and host_identifier = $2 order by id asc limit 1");
                result = transaction.exec_prepared("get_bpf_process_event_id", pid, host_identifier);
                connection->unprepare("get_bpf_process_event_id");
                std::queue<long> parent_queue;
                if (!result.empty())
                {
                    long record_id = result.at(0)["id"].as<long>();
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

                    result = transaction.exec_prepared("get_bpf_process_data_from_id", record_id);
                }
                if (result.empty())
                {
                    std::cerr << "No process found with record ID bpf " << record_id << std::endl;
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
                                if (std::abs(timestamp_delta - uptime_delta) > 3)
                                {
                                    // system rebooted somewhere between these two
                                    loop = false;

                                    // we need to discard the result, as it is from different session
                                    continue;
                                }
                                else
                                {
                                    timestamp = new_timestamp;
                                    uptime = new_uptime;
                                }

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
                                entry.ppid = result.at(i)["parent"].as<long>();
                                entry.root_pid = result.at(i)["pid"].as<long>();
                                entry.event_type = "Process Event";
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
                                    entry_soc.ppid = result_socket.at(0)["parent"].as<long>();
                                    entry_soc.uid = result_socket.at(0)["uid"].as<long>();
                                    entry_soc.gid = result_socket.at(0)["gid"].as<long>();
                                    entry_soc.cid = result_socket.at(0)["cid"].as<long>();
                                    entry_soc.exit_code = result_socket.at(0)["exit_code"].as<std::string>();
                                    entry_soc.syscall = result_socket.at(0)["syscall"].as<std::string>();
                                    entry_soc.protocol = result_socket.at(0)["protocol"].as<int>();
                                    entry_soc.local_address = result_socket.at(0)["local_address"].as<std::string>();
                                    entry_soc.remote_address = result_socket.at(0)["remote_address"].as<std::string>();
                                    entry_soc.local_port = result_socket.at(0)["local_port"].as<int>();
                                    entry_soc.remote_port = result_socket.at(0)["remote_port"].as<int>();
                                    entry_soc.duration = result_socket.at(0)["duration"].as<long>();
                                    entry_soc.event_type = "Socket Event";
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
                                    entry_file.ppid = result_file.at(0)["parent"].as<long>();
                                    entry_file.uid = result_file.at(0)["uid"].as<long>();
                                    entry_file.gid = result_file.at(0)["gid"].as<long>();
                                    entry_file.cid = result_file.at(0)["cid"].as<long>();
                                    entry_file.exit_code = result_file.at(0)["exit_code"].as<std::string>();
                                    entry_file.syscall = result_file.at(0)["syscall"].as<std::string>();
                                    entry_file.file_path = result_file.at(0)["file_path"].as<std::string>();
                                    entry_file.flags = result_file.at(0)["flags"].as<std::string>();
                                    entry_file.duration = result_file.at(0)["duration"].as<long>();
                                    entry_file.event_type = "File Event";
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
                long smallest_root_pid;
                if (!process_list.size() <= 1)
                {
                    long min_root_pid = process_list[0].root_pid;
                    for (int i = 1; i < process_list.size(); i++)
                    {

                        if (process_list[i].root_pid < min_root_pid)
                        {
                            min_root_pid = process_list[i].root_pid;
                        }
                    }

                    // Store the smallest value in entry.pid
                    smallest_root_pid = min_root_pid;
                }
                else
                {
                    smallest_root_pid = 0;
                    response.status = boost::beast::http::status::not_found;
                    response.body = "{\n"
                                "  \"status\":\"Fail\",\n"
                                "  \"message\":\"No Process Tree.\"\n"
                                "}";
                    response.header["Content-Type"] = "application/json";
                    return response;
                }
                connection->unprepare("get_bpf_process_data_from_id");
                connection->unprepare("get_bpf_process_data_execve");
                connection->unprepare("get_bpf_process_data_non_execve");
                nlohmann::json j;
                nlohmann::json graph;

                for (auto &entry : process_list)
                {
                    nlohmann::json obj;
                    obj["pid"] = entry.pid;
                    obj["parent"] = entry.ppid;
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
                    // obj["parent"] = entry.parent;
                    obj["root_pid"] = smallest_root_pid;

                    graph.push_back(obj);
                }

                j["root_pid"] = smallest_root_pid;
                j["graph"] = graph;

                process_tree = j;
            }
            if (process_list.size() > 1)
            {
                response.status = boost::beast::http::status::ok;
                response.body = process_tree.dump();
                response.header["Content-Type"] = "application/json";
                return response;
            }
            else
            {
                response.status = boost::beast::http::status::not_found;
                response.body = "{\n"
                                "  \"status\":\"Fail\",\n"
                                "  \"message\":\"No Forward Process Tree.\"\n"
                                "}";
                response.header["Content-Type"] = "application/json";
                return response;
            }
        }
        if (os_name == "Windows")
        {
            soc_event = parameters["socket_event"];
            file_event = parameters["file_event"];
            std::vector<WinProcessEntry> process_list;
            if (alert_type == "File Alert")
            {

                std::vector<WinProcessEntry> _tmp_process_list = get_win_file_tree(connection, record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
            }
            else if (alert_type == "Socket Alert")
            {
                std::vector<WinProcessEntry> _tmp_process_list = get_win_socket_tree(connection, record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
            }
            else
            {
                std::vector<WinProcessEntry> _tmp_process_list = event_win_backtrack_process_tree(connection, record_id);
                process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
            }
            {
                pqxx::result result;
                WinProcessEntry entry;
                WinProcessEntry entry_soc;
                WinProcessEntry entry_file;
                entry_soc.event_type = "Socket Event";
                entry_file.event_type = "File Event";
                entry.event_type = "Process Event";
                pqxx::work transaction{*connection};
                if(alert_type == "Socket Alert")
                {
                    connection->prepare("get_win_socket_pid", "select process_guid from win_socket_events where id = $1");
                    result = transaction.exec_prepared("get_win_socket_pid", record_id);
                    connection->unprepare("get_win_socket_pid");
                    process_guid = result.at(0)["process_guid"].as<std::string>();
                    transaction.commit();
                }
                else if (alert_type == "File Alert")
                {
                    connection->prepare("get_win_file_pid", "select process_guid from win_file_events where id = $1");
                    result = transaction.exec_prepared("get_win_file_pid", record_id);
                    connection->unprepare("get_win_file_pid");
                    process_guid = result.at(0)["process_guid"].as<std::string>();
                    transaction.commit();
                }
                else
                {
                    connection->prepare("get_win_process_pid", "select process_guid from win_process_events where id = $1");
                    result = transaction.exec_prepared("get_win_process_pid", record_id);
                    connection->unprepare("get_win_process_pid");
                    process_guid = result.at(0)["process_guid"].as<std::string>();
                }
                connection->prepare("get_win_process_event_id", "select id from win_process_events where process_guid = $1 and host_identifier = $2 order by id asc limit 1");

                
                result = transaction.exec_prepared("get_win_process_event_id", process_guid, host_identifier);
                connection->unprepare("get_win_process_event_id");
                std::queue<std::string> parent_queue;
                if (!result.empty())
                {
                    long record_id = result.at(0)["id"].as<long>();

                    connection->prepare("get_win_process_data_from_id", "select * from win_process_events where id = $1");

                    /*
                     * Since events can arrive out of order, and at some delay (in case osquery cannot reach fleet server for some reason, we cannot rely upon record ID in database.
                     * If we want to get latest record of event, we should order by time column instead, which records timestamp at which the event occurred. Since it comes a
                     * monotonic clock, it will never be out of order.
                     */

                    connection->prepare("get_win_process_data_execve", "select * from win_process_events where cmdline <> '' and "
                                                                       "parent_process_guid = $1 and host_identifier = $2 and unixtime between $3 and $4 order by time asc limit 10");
                    connection->prepare("get_win_process_data_non_execve", "select * from win_process_events where cmdline = '' and "
                                                                           "parent_process_guid = $1 and host_identifier = $2 and unixtime between $3 and $4 order by time asc limit 10");

                    result = transaction.exec_prepared("get_win_process_data_from_id", record_id);
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
                    long ppid = result.at(0)["parent_pid"].as<long>();
                    long pid = result.at(0)["pid"].as<long>();

                    std::string path, cmdline, host_identifier;

                    parent_process_guid = result.at(0)["parent_process_guid"].as<std::string>();
                    process_guid = result.at(0)["process_guid"].as<std::string>();
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

                    parent_queue.push(process_guid);
                    while (!parent_queue.empty())
                    {
                        result = transaction.exec_prepared("get_win_process_data_execve", parent_queue.front(), host_identifier, timestamp, s_current_time);

                        if (result.empty())
                        {
                            result = transaction.exec_prepared("get_win_process_data_non_execve", parent_queue.front(), host_identifier, timestamp, s_current_time);
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
                            std::string child_process_guid;
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
                                if (std::abs(timestamp_delta - uptime_delta) > 3)
                                {
                                    // system rebooted somewhere between these two
                                    loop = false;

                                    // we need to discard the result, as it is from different session
                                    continue;
                                }
                                else
                                {
                                    timestamp = new_timestamp;
                                    uptime = new_uptime;
                                }

                                path = result.at(i)["path"].as<std::string>();
                                cmdline = result.at(i)["cmdline"].as<std::string>();
                                child_process_guid = result.at(i)["process_guid"].as<std::string>();
                                parent_queue.push(result.at(i)["process_guid"].as<std::string>());
                                timestamp = result.at(i)["time"].as<long>();

                                // entry.ppid = pid;

                                // entry.pid = child_pid;
                                entry.process_guid = child_process_guid;
                                entry.action = result.at(i)["action"].as<std::string>();
                                entry.eid = result.at(i)["eid"].as<std::string>();
                                entry.pid = result.at(i)["pid"].as<long>();
                                entry.path = path;
                                entry.cmdline = cmdline;
                                entry.ppid = result.at(i)["parent_pid"].as<long>();
                                entry.parent_process_guid = result.at(i)["parent_process_guid"].as<std::string>();
                                entry.parent_path = result.at(i)["parent_path"].as<std::string>();
                                entry.owner_uid = result.at(i)["owner_uid"].as<std::string>();
                                entry.time = result.at(i)["time"].as<long long>();
                                entry.utc_time = result.at(i)["utc_time"].as<std::string>();
                                entry.eventid = result.at(i)["eventid"].as<int>();
                                entry.sha256 = result.at(i)["sha256"].as<std::string>();
                                entry.uptime = result.at(i)["uptime"].as<long>();
                                entry.event_type = "Process Event";
                                entry.root_pid = result.at(i)["pid"].as<long>();
                                entry.parent_process_guid = parent_queue.front();

                                // pid = tmp_pid;
                                process_list.push_back(entry);
                            }
                            if (soc_event)
                            {

                                connection->prepare("get_win_socket_event_id", "select * from win_socket_events where process_guid = $1 and host_identifier = $2 order by id asc limit 1");
                                pqxx::result result_socket = transaction.exec_prepared("get_win_socket_event_id", child_process_guid, host_identifier);
                                if (!result_socket.empty())
                                {
                                    entry_soc.pid = result_socket.at(0)["pid"].as<long>();

                                    entry_soc.event_type = result_socket.at(0)["event_type"].as<std::string>();
                                    entry_soc.host_identifier = result_socket.at(0)["host_identifier"].as<std::string>();
                                    entry_soc.hostname = result_socket.at(0)["hostname"].as<std::string>();
                                    entry_soc.parent = result_socket.at(0)["parent_pid"].as<long>();
                                    entry_soc.action = result_socket.at(0)["action"].as<long>();
                                    entry_soc.eid = result_socket.at(0)["eid"].as<std::string>();
                                    entry_soc.timestamp = result_socket.at(0)["time"].as<long>();
                                    entry_soc.utc_time = result_socket.at(0)["utc_time"].as<std::string>();
                                    entry_soc.process_guid = result_socket.at(0)["process_guid"].as<std::string>();
                                    entry_soc.process_name = result_socket.at(0)["process_name"].as<std::string>();
                                    entry_soc.family = result_socket.at(0)["family"].as<long>();
                                    entry_soc.protocol = result_socket.at(0)["protocol"].as<int>();
                                    entry_soc.local_address = result_socket.at(0)["local_address"].as<std::string>();
                                    entry_soc.remote_address = result_socket.at(0)["remote_address"].as<std::string>();
                                    entry_soc.local_port = result_socket.at(0)["local_port"].as<int>();
                                    entry_soc.remote_port = result_socket.at(0)["remote_port"].as<int>();
                                    entry_soc.eventid = result_socket.at(0)["eventid"].as<int>();
                                    entry_soc.event_type = "Socket Event";
                                    entry_soc.unixtime = result_socket.at(0)["unixTime"].as<long>();
                                    entry_soc.root_pid = result.at(0)["pid"].as<long>();
                                    entry_soc.uptime = result_socket.at(0)["uptime"].as<long>();
                                    process_list.push_back(entry_soc);
                                }
                            }
                            if (file_event)
                            {

                                connection->prepare("get_win_file_event_id", "select * from win_file_events where process_guid = $1 and host_identifier = $2 order by id asc limit 1");
                                pqxx::result result_file = transaction.exec_prepared("get_win_file_event_id", child_process_guid, host_identifier);
                                if (!result_file.empty())
                                {
                                    entry_file.pid = result_file.at(0)["pid"].as<long>();

                                    entry_file.host_identifier = result_file.at(0)["host_identifier"].as<std::string>();
                                    entry_file.hostname = result_file.at(0)["hostname"].as<std::string>();
                                    entry_file.parent = result_file.at(0)["parent_pid"].as<long>();
                                    entry_file.action = result_file.at(0)["action"].as<std::string>();
                                    entry_file.eid = result_file.at(0)["eid"].as<std::string>();
                                    entry_file.target_path = result_file.at(0)["target_path"].as<std::string>();
                                    entry_file.md5 = result_file.at(0)["md5"].as<std::string>();
                                    entry_file.sha256 = result_file.at(0)["sha256"].as<std::string>();
                                    entry_file.hashed = result_file.at(0)["hashed"].as<int>();
                                    entry_file.uid = result_file.at(0)["uid"].as<std::string>();
                                    entry_file.utc_time = result_file.at(0)["utc_time"].as<std::string>();
                                    entry_file.unixtime = result_file.at(0)["unixTime"].as<long>();
                                    entry_file.timestamp = result_file.at(0)["time"].as<long>();
                                    entry_file.uptime = result_file.at(0)["uptime"].as<long>();
                                    entry_file.process_guid = result_file.at(0)["process_guid"].as<std::string>();
                                    entry_file.process_name = result_file.at(0)["process_name"].as<std::string>();
                                    entry_file.pe_file = result_file.at(0)["pe_file"].as<std::string>();
                                    entry_file.root_pid = result.at(0)["pid"].as<long>();
                                    entry_file.eventid = result_file.at(0)["eventid"].as<int>();
                                    entry_file.event_type = "File Event";

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
                long smallest_root_pid;
                if (!process_list.size() <= 1)
                {
                    long min_root_pid = process_list[0].root_pid;
                    for (int i = 1; i < process_list.size(); i++)
                    {

                        if (process_list[i].root_pid < min_root_pid)
                        {
                            min_root_pid = process_list[i].root_pid;
                        }
                    }

                    // Store the smallest value in entry.pid
                    smallest_root_pid = min_root_pid;
                }
                else
                {
                    smallest_root_pid = 0;
                    response.status = boost::beast::http::status::not_found;
                    response.body = "{\n"
                                "  \"status\":\"Fail\",\n"
                                "  \"message\":\"No Process Tree.\"\n"
                                "}";
                    response.header["Content-Type"] = "application/json";
                    return response;
                }
                connection->unprepare("get_win_process_data_from_id");
                connection->unprepare("get_win_process_data_execve");
                connection->unprepare("get_win_process_data_non_execve");
                nlohmann::json j;
                nlohmann::json graph;

                for (auto &entry : process_list)
                {
                    nlohmann::json obj;

                    int eventid, hashed;
                    long long unixtime, pid, ppid, event_time, tid, gid, cid, probe_error, time, is_container_process, container_age, family, type, protocol, local_port, remote_port, timestamp, uptime, parent;
                    std::string eid, action, process_guid, parent_process_guid, parent_path, owner_uid, utc_time, command_line, exit_code, current_working_directory, syscall, path, cwd, cmdline, md5, sha1, sha256,
                        container_name, ntime, json_cmdline, fd, local_address, remote_address, file_path, hostname, flags, host_identifier, event_type, process_name, target_path, uid;
                    obj["pid"] = entry.pid;
                    obj["parent_pid"] = entry.ppid;
                    obj["action"] = entry.action;
                    // obj["eid"] = entry.eid;
                    obj["process_guid"] = entry.process_guid;
                    obj["path"] = entry.path;
                    obj["cmdline"] = entry.cmdline;
                    obj["parent_process_guid"] = entry.parent_process_guid;
                    obj["parent_path"] = entry.parent_path;
                    obj["owner_uid"] = entry.owner_uid;
                    // obj["eventid"] = entry.eventid;
                    obj["sha256"] = entry.sha256;
                    obj["family"] = entry.family;
                    obj["protocol"] = entry.protocol;
                    obj["local_address"] = entry.local_address;
                    obj["remote_address"] = entry.remote_address;
                    obj["local_port"] = entry.local_port;
                    obj["remote_port"] = entry.remote_port;
                    obj["target_path"] = entry.target_path;
                    obj["hashed"] = entry.hashed;
                    obj["pe_file"] = entry.pe_file;
                    // obj["parent_pid"] = entry.parent;
                    obj["event_type"] = entry.event_type;
                    obj["root_pid"] = smallest_root_pid;

                    graph.push_back(obj);
                }

                j["root_pid"] = smallest_root_pid;
                j["graph"] = graph;

                process_tree = j;
            }
            if (process_list.size() > 1)
            {
                response.status = boost::beast::http::status::ok;
                response.body = process_tree.dump();
                response.header["Content-Type"] = "application/json";
                return response;
            }
            else
            {
                response.status = boost::beast::http::status::not_found;
                response.body = "{\n"
                                "  \"status\":\"Fail\",\n"
                                "  \"message\":\"No Forward Process Tree.\"\n"
                                "}";
                response.header["Content-Type"] = "application/json";
                return response;
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

    response.status = boost::beast::http::status::not_found;
    response.body = "{\n"
                    "  \"status\":\"Fail\",\n"
                    "  \"message\":\"No Process Tree.\"\n"
                    "}";
    response.header["Content-Type"] = "application/json";
    return response;
}