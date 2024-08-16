#include "../../3rdparty/json.hpp"
#include "nodes.h"
#include "scheduled_query.h"
#include "../../common/logger.h"
#include <chrono>
#include <ctime>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <map>
using namespace boost::algorithm;

HTTPMessage nodes_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
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
            if (dif < 60)
            {
                online++;
            }
            else
            {
                offline++;
            }

            nodes_active_json["nodes"] = {
                {"online", (online)},
                {"offline", (offline)},
                {"total", (offline+online)},
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

HTTPMessage events_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json events_count;
    std::string event_id, host_identifier;
    std::stringstream ss;
    try
    {
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        pqxx::work transaction{*connection};
        pqxx::result result;
        int events_total , events_open , events_closed;

        if(host_identifier.empty()){
            connection->prepare("get_events_count", "select count(id) from events");
            result = transaction.exec_prepared("get_events_count");
            connection->unprepare("get_events_count");
            events_total = result.at(0)["count"].as<int>();


            connection->prepare("get_open_events_count", "select count(id) from events where is_open = 't'");
            result = transaction.exec_prepared("get_open_events_count");
            connection->unprepare("get_open_events_count");
            events_open = result.at(0)["count"].as<int>();


            connection->prepare("get_close_events_count", "select count(id) from events where is_open = 'f'");
            result = transaction.exec_prepared("get_close_events_count");
            connection->unprepare("get_close_events_count");
            events_closed = result.at(0)["count"].as<int>();
        }
        else{
            connection->prepare("get_events_count", "select count(id) from events where host_identifier = $1");
            result = transaction.exec_prepared("get_events_count", host_identifier);
            connection->unprepare("get_events_count");
            events_total = result.at(0)["count"].as<int>();


            connection->prepare("get_open_events_count", "select count(id) from events where is_open = 't' and host_identifier = $1");
            result = transaction.exec_prepared("get_open_events_count", host_identifier);
            connection->unprepare("get_open_events_count");
            events_open = result.at(0)["count"].as<int>();


            connection->prepare("get_close_events_count", "select count(id) from events where is_open = 'f' and host_identifier = $1");
            result = transaction.exec_prepared("get_close_events_count", host_identifier);
            connection->unprepare("get_close_events_count");
            events_closed = result.at(0)["count"].as<int>();
        }
        events_count["events"] = {
            {"total", (events_total)},
            {"close", (events_closed)},
            {"open", (events_open)},
        };

        if (events_total<=0)
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
            response.body = events_count.dump();
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

HTTPMessage os_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json os_count;
    std::string event_id;
    std::stringstream ss;
    int unique_os;
    try
    {
        for (const auto &it : request.query)
        {
            event_id = it.second;

            ss << it.first << " : " << it.second << std::endl;
        }
        pqxx::work transaction{*connection};
        pqxx::result result;
        // int events_total , events_open , events_closed;
        connection->prepare("get_os_count", "SELECT count(*), CASE WHEN (os_name like ('%Windows%') or os_name like ('%windows%')) THEN 'Windows' WHEN (os_name like ('%Kali%') or os_name like ('%kali%') or os_name like ('%ubuntu%') or os_name like ('%Ubuntu%') or os_name like ('%Linux%') or os_name like ('%Arch%')) THEN 'Linux' WHEN (os_name like ('%Darvin%') or os_name like ('%darvin%')) THEN 'MacOS' END os FROM nodes group by os");
        result = transaction.exec_prepared("get_os_count");
        connection->unprepare("get_os_count");
        // unique_os = result.size();


        for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
        {
            os_count["os_count"] += {
                {"os", c["os"].as<std::string>()},
                {"count", c["count"].as<int>()}
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
            transaction.commit();
            response.status = boost::beast::http::status::ok;
            response.body = os_count.dump();
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

HTTPMessage weekly_alerts_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::ordered_json alert_count, response_json;
    std::string event_id, host_identifier; 
    std::string weekday = "";
    std::stringstream ss;
    std::vector<std::string> day={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    int unique_os;
    try
    {
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        pqxx::work transaction{*connection};
        pqxx::result result;
        std::time_t current_time = std::time(0);
        tm *now = localtime(&current_time);
        time_t daystart_time = time(0) - ((now->tm_hour * 60 * 60) + (now->tm_min * 60) + (now->tm_sec));
        weekday = std::to_string(now->tm_mday)+"/"+ std::to_string(1+(now->tm_mon)) + "/"+std::to_string(1900+(now->tm_year));
        if(host_identifier.empty()){
            for (int i = 1; i <=7;)
            {
                connection->prepare("get_weekly_alerts_count", "select alert_type, count(*) from events where unixtime between $1 and $2 group by alert_type");
                result = transaction.exec_prepared("get_weekly_alerts_count", daystart_time, current_time);
                connection->unprepare("get_weekly_alerts_count");
                alert_count[weekday]["File Alert"]=0;
                alert_count[weekday]["Process Alert"]=0;
                alert_count[weekday]["Socket Alert"]=0;
                for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
                {
                    alert_count[weekday][c["alert_type"].as<std::string>()]=c["count"].as<int>();
                }
                current_time = daystart_time;
                daystart_time = daystart_time - 86400;
                i++;
                tm *now = localtime(&current_time);
                weekday = std::to_string((now)->tm_mday)+"/"+ std::to_string(1+(now->tm_mon)) + "/"+std::to_string(1900+(now->tm_year));
                // weekday--;
                // weekday=weekday<0?weekday=6:weekday;
            }
        }
        else{
            for (int i = 1; i <=7;)
                {
                    connection->prepare("get_weekly_alerts_count", "select alert_type, count(*) from events where host_identifier = $1 and unixtime between $2 and $3 group by alert_type");
                    result = transaction.exec_prepared("get_weekly_alerts_count", host_identifier, daystart_time, current_time);
                    connection->unprepare("get_weekly_alerts_count");
                    alert_count[weekday]["File Alert"]=0;
                    alert_count[weekday]["Process Alert"]=0;
                    alert_count[weekday]["Socket Alert"]=0;
                    for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
                    {
                        alert_count[weekday][c["alert_type"].as<std::string>()]=c["count"].as<int>();
                    }
                    current_time = daystart_time;
                    daystart_time = daystart_time - 86400;
                    i++;
                    tm *now = localtime(&current_time);
                    weekday = std::to_string((now)->tm_mday)+"/"+ std::to_string(1+(now->tm_mon)) + "/"+std::to_string(1900+(now->tm_year));
                    // weekday--;
                    // weekday=weekday<0?weekday=6:weekday;
                }
        }
        // unique_os = result.size();

        
        if (alert_count.size() <= 0)
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
            response.body = alert_count.dump();
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


HTTPMessage severity_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json os_count;
    std::string event_id, host_identifier;
    std::stringstream ss;
    int unique_os;
    try
    {
        for (const auto &it : request.query)
        {
            event_id = it.second;

            ss << it.first << " : " << it.second << std::endl;
        }

        os_count = request.query;
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        pqxx::work transaction{*connection};
        pqxx::result result;
        // int events_total , events_open , events_closed;

        if (!host_identifier.empty())
        {
            connection->prepare("get_severity_count", "select severity,count(severity) from events where host_identifier = $1 group by severity");
            result = transaction.exec_prepared("get_severity_count", host_identifier);
            connection->unprepare("get_severity_count");
        }
        else
        {
            connection->prepare("get_severity_count", "select severity, count(*) from events group by severity");
            result = transaction.exec_prepared("get_severity_count");
            connection->unprepare("get_severity_count");
        }
        // unique_os = result.size();


        for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
        {
            os_count["severity_count"] += {
                {"severity", c["severity"].as<int>()},
                {"count", c["count"].as<int>()}
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
            transaction.commit();
            response.status = boost::beast::http::status::ok;
            response.body = os_count.dump();
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

HTTPMessage alert_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json os_count;
    std::string event_id, host_identifier;
    std::stringstream ss;
    int unique_os;
    try
    {
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        pqxx::work transaction{*connection};
        pqxx::result result;
        // int events_total , events_open , events_closed;

        if(host_identifier.empty()){
            connection->prepare("get_alert_count", "select event_code, count(*) from events group by event_code order by count desc limit 10");
            result = transaction.exec_prepared("get_alert_count");
            connection->unprepare("get_alert_count");
            // unique_os = result.size();


            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
            {
                os_count["alert_count"] += {
                    {"event_code", c["event_code"].as<std::string>()},
                    {"count", c["count"].as<int>()}
                    };
            }
        }
        else{
            connection->prepare("get_alert_count", "select event_code, count(*) from events where host_identifier = $1 group by event_code order by count desc limit 10");
            result = transaction.exec_prepared("get_alert_count", host_identifier);
            connection->unprepare("get_alert_count");
            // unique_os = result.size();


            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
            {
                os_count["alert_count"] += {
                    {"event_code", c["event_code"].as<std::string>()},
                    {"count", c["count"].as<int>()}
                    };
            }
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
            response.body = os_count.dump();
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

HTTPMessage high_risk_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json high_risk_count, params;
    try
    {
        pqxx::work transaction{*connection};
        pqxx::result result_count;
        int count = 0,total_records;
        std::string level = "desc", topn = "1", severity = "4";
        params = request.query;
        if(params.contains("level"))
        {
            level = params["level"];
        }
        if(params.contains("topn"))
        {
            topn = params["topn"];
        }
        if(params.contains("severity"))
        {
            severity = params["severity"];
        }

            std::string query = "select host_identifier, hostname, count(severity) from events where severity = '"+severity+"' group by host_identifier,hostname order by count "+level+" limit "+topn+"";
            connection->prepare("high_risk_count", query);
            result_count = transaction.exec_prepared("high_risk_count");
            connection->unprepare("high_risk_count");

            std::string host_identifier, hostnames;
            int counts = 0;
            for (pqxx::result::const_iterator j = result_count.begin(); j != result_count.end(); ++j)
                {
                    std::string host_identifier = j["host_identifier"].as<std::string>();
                    std::string hostname = j["hostname"].as<std::string>();
                    std::string count = j["count"].as<std::string>();

                    high_risk_count["high_risk_count"] += { \

                        {"host_identifier", j["host_identifier"].as<std::string>()},
                        {"hostname", j["hostname"].as<std::string>()},
                        {"severity", severity},
                        {"count", j["count"].as<std::string>()}

                    };

                }

            
        if (result_count.size() <= 0)
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Fail\",\n"
                            "  \"message\":\"No data available\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else{
            response.status = boost::beast::http::status::ok;
            response.body = high_risk_count.dump();
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

HTTPMessage node_alert_count(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json node_alert_count, params;
    try
    {
        pqxx::work transaction{*connection};
        pqxx::result result_count;
        int count = 0,total_records;

            std::string query = "select * from (select n.*,count(e.*) from nodes n left join events e on n.host_identifier=e.host_identifier group by n.id,n.config_id,n.host_identifier) as p  where not count = 0 order by count desc limit 5;";
            connection->prepare("node_alert_count", query);
            result_count = transaction.exec_prepared("node_alert_count");
            connection->unprepare("node_alert_count");

            int const num_rows = std::size(result_count);
            nlohmann::json node_alert_count = nlohmann::json::array();

            for (int rownum = 0; rownum < num_rows; ++rownum)
            {
                pqxx::row const row = result_count[rownum];
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
                node_alert_count.push_back(obj);
            }

            if (result_count.size() <= 0)
            {
                response.status = boost::beast::http::status::ok;
                response.body = "{\n"
                    "  \"status\":\"Fail\",\n"
                    "  \"message\":\"No data available\"\n"
                    "}";
                response.header["Content-Type"] = "application/json";
                return response;
            }
            else{
            response.status = boost::beast::http::status::ok;
            response.body = node_alert_count.dump();
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


HTTPMessage mitre_analysis(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json mitre_analysis;
    std::unordered_map<std::string,int> mitr;
    std::vector<std::unordered_map<std::string,int>> res;
    std::string event_id, host_identifier;
    std::stringstream ss;
    int unique_os;
    try
    {
        for (const auto &it : request.query)
        {
            if (it.first == "host_identifier")
            {
                host_identifier = it.second;
            }
            ss << it.first << " : " << it.second << std::endl;
        }
        pqxx::work transaction{*connection};
        pqxx::result result;
        // int events_total , events_open , events_closed;

        if(host_identifier.empty()){
            connection->prepare("get_mitre_analysis", "select technique, count(*) from events group by technique");
            result = transaction.exec_prepared("get_mitre_analysis");
            connection->unprepare("get_mitre_analysis");
            // unique_os = result.size();


            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
            {
                std::string technique_name = c["technique"].as<std::string>();
                int no = c["count"].as<int>();
                if(!technique_name.empty())
                {
                    std::vector<std::string> strs;
                    boost::split(strs, technique_name, boost::is_any_of(","));

                    for ( auto it : strs)
                    {
                        boost::trim(it);
                        mitr[it]=c["count"].as<int>()+mitr[it];
                    }
                }
            }
            mitre_analysis=mitr;
        }


        else {
            connection->prepare("get_mitre_analysis", "select technique, count(*) from events where host_identifier = $1 group by technique");
            result = transaction.exec_prepared("get_mitre_analysis", host_identifier);
            connection->unprepare("get_mitre_analysis");
            // unique_os = result.size();


            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
            {
                std::string technique_name = c["technique"].as<std::string>();
                int no = c["count"].as<int>();
                if(!technique_name.empty())
                {
                    std::vector<std::string> strs;
                    boost::split(strs, technique_name, boost::is_any_of(","));

                    for ( auto it : strs)
                    {
                        boost::trim(it);
                        mitr[it]=c["count"].as<int>()+mitr[it];
                    }
                }
            }
            mitre_analysis=mitr;
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
            response.body = mitre_analysis.dump();
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
                        "  \"message\":\"Query to get mitre analysis failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get mitre analysis";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get mitre analysis\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}
