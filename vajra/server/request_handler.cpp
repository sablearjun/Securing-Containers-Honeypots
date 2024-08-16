#include "../3rdparty/json.hpp"
#include "request_handler.h"
#include "../common/logger.h"

#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <ctime>
#include <string>
#include "../common/ConfigurationParser.h"

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

std::string random_string(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

HTTPMessage enroll(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json enrollment_response;

    try
    {
        auto enrollment_request = nlohmann::json::parse(request.body);
        //std::cout << request.body << std::endl;
        //BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Enrollment request is" << request.body;
        if (enrollment_request.contains("enroll_secret") && enrollment_request.contains("host_identifier") && enrollment_request.contains("host_details"))
        {
            std::string secret = enrollment_request["enroll_secret"];
            std::string host_identifier = enrollment_request["host_identifier"];
            std::cout << "Enroll request " << host_identifier << std::endl;
            /*
             * TODO: Replace with check from some other data-store
             */
            if (secret == enrollment_secret)
            {
                /*
                 * Since the enrollment secret is valid, we will generate a unique node key, and store host information
                 * in database. The node key will be used as key to identify which node the data is coming from.
                 */
                std::string node_key = random_string(20);
                bool node_unique = false;
                auto host_details = enrollment_request["host_details"];
                // std::cout << host_details << std::endl;

                pqxx::work transaction{*connection};
                connection->prepare("validate_node", "select * from nodes where host_identifier = $1;");
                pqxx::result result = transaction.exec_prepared("validate_node", host_identifier);
                transaction.commit();
                connection->unprepare("validate_node");
                if (result.size() == 1)
                {
                    bool unique_node = false;
                    bool reenroll_allowed = result.at(0)["reenroll_allowed"].as<bool>();
                    if (reenroll_allowed)
                    {
                        while (unique_node == false)
                        {
                            pqxx::result result = transaction.exec_prepared("validate_node_key", node_key);
                            transaction.commit();
                            // connection->unprepare("validate_node_key");
                            if (result.size() == 1)
                            {
                                node_key = random_string(20);
                                unique_node = false;
                            }
                            else
                            {
                                unique_node = true;
                            }
                        }
                        //////
                        // connection->prepare("delete_node", "delete from nodes where host_identifier = $1;");
                        // pqxx::result result = transaction.exec_prepared("delete_node", host_identifier);
                        // connection->unprepare("delete_node");
                        //     if (result.size() == 1)
                        //     {
                        //         // enrollment_response["node_key"] = node_key;
                        //         // enrollment_response["node_invalid"] = false;
                        //         // response.status = http::status::ok;
                        //         // response.body = enrollment_response.dump();

                        //         // response.header["Content-Type"] = "application/json";
                        //         // BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Successfully enrolled host " << host_identifier;
                        //         // return response;
                        //         std::cout<<"Node deleted";
                        //     }
                        //     else
                        //     {

                        //     }
                        /////
                    }
                }
                else
                {
                }

                BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Enrollment request received from host-identifier " << host_identifier;

                std::string os_arch, os_build, os_major, os_minor, os_name, os_platform, osquery_version, hardware_vendor, hardware_model, hardware_version, hostname, hardware_cpu_logical_core, hardware_cpu_type, hardware_physical_memory;
                int enrolled_on, last_seen;
                std::time_t t = std::time(0);
                enrolled_on = t;
                last_seen = t;
                if (host_details.contains("os_version"))
                {
                    auto os_version = host_details["os_version"];

                    os_arch = os_version.contains("arch") ? os_version["arch"] : "unknown";
                    os_build = os_version.contains("build") ? os_version["build"] : "unknown";
                    os_major = os_version.contains("major") ? os_version["major"] : "unknown";
                    os_minor = os_version.contains("minor") ? os_version["minor"] : "unknown";
                    os_name = os_version.contains("name") ? os_version["name"] : "unknown";
                    os_platform = os_version.contains("platform") ? os_version["platform"] : "unknown";
                }
                else
                {
                    BOOST_LOG_TRIVIAL(debug) << __SRCREF__ << "Enrollment request does not contain os_version object. Offending host-identifier: " << host_identifier;
                }

                if (host_details.contains("osquery_info"))
                {
                    auto osquery_info = host_details["osquery_info"];

                    osquery_version = osquery_info.contains("version") ? osquery_info["version"] : "unknown";
                }
                else
                {
                    BOOST_LOG_TRIVIAL(debug) << __SRCREF__ << "Enrollment request does not contain osquery_info object. Offending host-identifier: " << host_identifier;
                }

                if (host_details.contains("system_info"))
                {
                    auto system_info = host_details["system_info"];
                    hardware_model = system_info.contains("hardware_model") ? system_info["hardware_model"] : "unknown";
                    hardware_version = system_info.contains("hardware_version") ? system_info["hardware_version"] : "unknown";
                    hardware_vendor = system_info.contains("hardware_vendor") ? system_info["hardware_vendor"] : "unknown";
                    hostname = system_info.contains("hostname") ? system_info["hostname"] : "unknown";
                    hardware_cpu_logical_core = system_info.contains("cpu_logical_cores") ? system_info["cpu_logical_cores"] : "unknown";
                    hardware_cpu_type = system_info.contains("cpu_type") ? system_info["cpu_type"] : "unknown";
                    hardware_physical_memory = system_info.contains("physical_memory") ? system_info["physical_memory"] : "unknown";
                }

                try
                {
                    // pqxx::work transaction{*connection};
                    while (node_unique == false)
                    {
                        pqxx::result result = transaction.exec_prepared("validate_node_key", node_key);
                        // connection->unprepare("validate_node_key");
                        if (result.size() == 1)
                        {
                            node_key = random_string(20);
                            node_unique = false;
                        }
                        else
                        {
                            node_unique = true;
                        }
                    }
                    std::string node_invalid = "false", reenroll_allowed = "true", config_id = "1";

                    connection->prepare("validate_node", "select * from nodes where host_identifier = $1;");
                    pqxx::result result = transaction.exec_prepared("validate_node", host_identifier);
                    if (result.size() == 1)
                    {
                        connection->prepare("update_node_key", "update nodes set node_key = $1 where host_identifier = $2;");
                        pqxx::result queries = transaction.exec_prepared("update_node_key", node_key, host_identifier);
                        connection->unprepare("update_node_key");
                    }
                    else
                    {
                        bool isolated = false;
                        connection->prepare("host_add", "insert into nodes (host_identifier, os_arch, os_build, os_major, os_minor, os_name, os_platform, osquery_version, node_key, node_invalid, reenroll_allowed, config_id, hardware_vendor, hardware_model, hardware_version, hostname, hardware_cpu_logical_core, hardware_cpu_type, hardware_physical_memory, enrolled_on, last_seen, isolated) values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22)");
                        transaction.exec_prepared("host_add", host_identifier, os_arch, os_build, os_major, os_minor, os_name, os_platform, osquery_version, node_key, node_invalid, reenroll_allowed, config_id, hardware_vendor, hardware_model, hardware_version, hostname, hardware_cpu_logical_core, hardware_cpu_type, hardware_physical_memory, enrolled_on, last_seen, isolated);
                        transaction.commit();
                        connection->unprepare("host_add");
                    }
                }
                catch (pqxx::sql_error const &e)
                {
                    BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
                    response.status = http::status::internal_server_error;
                    response.body = "{\n"
                                    "  \"message\":\"Enrollment failed due to database error.\",\n"
                                    "  \"node_key\":\"\",\n"
                                    "  \"node_invalid\":\"true\"\n"
                                    "}";
                    response.header["Content-Type"] = "application/json";
                    return response;
                }
                catch (std::exception const &e)
                {
                    BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
                    response.status = http::status::internal_server_error;
                    response.body = "{\n"
                                    "  \"message\":\"Enrollment failed due to database error.\",\n"
                                    "  \"node_key\":\"\",\n"
                                    "  \"node_invalid\":\"true\"\n"
                                    "}";
                    response.header["Content-Type"] = "application/json";
                    return response;
                }
                catch (...)
                {
                    BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not add host into nodes table";
                    response.status = http::status::internal_server_error;
                    response.body = "{\n"
                                    "  \"message\":\"Enrollment failed due to database error.\",\n"
                                    "  \"node_key\":\"\",\n"
                                    "  \"node_invalid\":\"true\"\n"
                                    "}";
                    response.header["Content-Type"] = "application/json";
                    return response;
                }

                enrollment_response["node_key"] = node_key;
                enrollment_response["node_invalid"] = false;
                response.status = http::status::ok;
                response.body = enrollment_response.dump();

                response.header["Content-Type"] = "application/json";
                BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Successfully enrolled host " << host_identifier;
                return response;
            }
            else
            {
                BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Enrollment of host failed due to bad secret";
                response.status = http::status::unauthorized;
                response.body = "{\n"
                                "  \"message\":\"Enrollment failed due to bad secret\",\n"
                                "  \"node_key\":\"\",\n"
                                "  \"node_invalid\":\"true\"\n"
                                "}";
                response.header["Content-Type"] = "application/json";
                return response;
            }
        }
        else
        {
            BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Could not process enrollment request due to incomplete fields";
            response.status = http::status::bad_request;
            response.body = "{\n"
                            "  \"message\":\"Enrollment failed due to bad request\",\n"
                            "  \"node_key\":\"\",\n"
                            "  \"node_invalid\":\"true\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }
    catch (const nlohmann::json::parse_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
        response.status = http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Enrollment failed due to bad request\",\n"
                        "  \"node_key\":\"\",\n"
                        "  \"node_invalid\":\"true\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (const nlohmann::json::type_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
        response.status = http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Enrollment failed due to bad request\",\n"
                        "  \"node_key\":\"\",\n"
                        "  \"node_invalid\":\"true\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Enrollment request contained malformed JSON";
        response.status = http::status::bad_request;
        response.body = "{\n"
                        "  \"message\":\"Enrollment failed due to bad request\",\n"
                        "  \"node_key\":\"\",\n"
                        "  \"node_invalid\":\"true\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage config(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;

    nlohmann::json osquery_configuration, scheduled_queries;

    try
    {
        auto request_data = nlohmann::json::parse(request.body);

        if (request_data.contains("node_key"))
        {
            std::string key = request_data["node_key"];
            std::string host_identifier;
            bool isKeyValid;

            try
            {
                pqxx::work transaction{*connection};
                connection->prepare("con_id", "select config_id from nodes where node_key = $1");
                connection->prepare("config_i", "select query from config_queries where id = $1");
                connection->prepare("update_last_seen", "update nodes set last_seen = $1 where host_identifier = $2;");
                
                pqxx::result result = transaction.exec_prepared("validate_node_key", key);
                if (result.size() == 1)
                {
                    isKeyValid = true;
                    host_identifier = result.at(0)["host_identifier"].as<std::string>();
                    std::cout << "Config request " << host_identifier << std::endl;
                    osquery_configuration["node_invalid"] = false;
                    int last_seen;
                    std::time_t t = std::time(0);
                    last_seen = t;
                    pqxx::result queries = transaction.exec_prepared("update_last_seen", last_seen, host_identifier);
                    transaction.commit();
                }
                else
                {
                    isKeyValid = false;
                    osquery_configuration["node_invalid"] = true;
                }
                connection->prepare("max_start_time", "select max(start_time) from processes where host_identifier = $1");
                
                pqxx::result result_time = transaction.exec_prepared("max_start_time", host_identifier);
                transaction.commit();
                long max_start_time = 0;
            
                auto temp = result_time.at(0)["max"];
                if (temp.size() > 1)
                {
                    max_start_time = temp.as<long>();
                }

                pqxx::result ids = transaction.exec_prepared("con_id", key);
                std::string node_id;

                if (!ids.empty())
                {
                    node_id = ids.at(0)["config_id"].as<std::string>();
                }
                std::vector<std::string> config_id_list;
                boost::split(config_id_list, node_id, boost::is_any_of(","));

                for (int i = 0; i < config_id_list.size(); i++)
                {
                    int node_ids = std::stoi(config_id_list[i]);
                    pqxx::result queries = transaction.exec_prepared("config_i", node_ids);
                    nlohmann::json query;
                    std::string node_query, str_query;

                    if (!queries.empty())
                    {
                        if (!queries.at(0)["query"].is_null())
                        {
                            node_query = queries.at(0)["query"].as<std::string>();;
                        }
                        query = nlohmann::json::parse(node_query);
                        if(query["schedule"].contains("file"))
                        {
                            str_query = query["schedule"]["file"]["query"].get<std::string>();
                            for (char& c : str_query) {
                                    if (c == '\"') {
                                        c = '\'';
                                    }
                                }
                            query["schedule"]["file"]["query"] = str_query;
                        }
                        if (isKeyValid)
                        {
                            if (query["schedule"].contains("process_events"))
                            {
                                scheduled_queries["process_events"]["query"] = query["schedule"]["process_events"]["query"];
                                scheduled_queries["process_events"]["interval"] = query["schedule"]["process_events"]["interval"];
                            }
                            if (query["schedule"].contains("user_events"))
                            {
                                scheduled_queries["user_events"]["query"] = query["schedule"]["user_events"]["query"];
                                scheduled_queries["user_events"]["interval"] = query["schedule"]["user_events"]["interval"];
                            }
                            if (query["schedule"].contains("socket_events"))
                            {
                                scheduled_queries["socket_events"]["query"] = query["schedule"]["socket_events"]["query"];
                                scheduled_queries["socket_events"]["interval"] = query["schedule"]["socket_events"]["interval"];
                            }
                            if (query["schedule"].contains("process_file_events"))
                            {
                                scheduled_queries["process_file_events"]["query"] = query["schedule"]["process_file_events"]["query"];
                                scheduled_queries["process_file_events"]["interval"] = query["schedule"]["process_file_events"]["interval"];
                            }
                            if (query["schedule"].contains("file_events"))
                            {
                                scheduled_queries["file_events"]["query"] = query["schedule"]["file_events"]["query"];
                                scheduled_queries["file_events"]["interval"] = query["schedule"]["file_events"]["interval"];
                            }
                            if (query["schedule"].contains("processes"))
                            {
                                scheduled_queries["processes"]["query"] = query["schedule"]["processes"]["query"];
                                scheduled_queries["processes"]["interval"] = query["schedule"]["processes"]["interval"];
                                scheduled_queries["processes"]["snapshot"] = query["schedule"]["processes"]["snapshot"];
                            }
                            if (query["schedule"].contains("process_envs"))
                            {
                                scheduled_queries["process_envs"]["query"] = query["schedule"]["process_envs"]["query"];
                                scheduled_queries["process_envs"]["interval"] = query["schedule"]["process_envs"]["interval"];
                                scheduled_queries["process_envs"]["snapshot"] = query["schedule"]["process_envs"]["snapshot"];
                            }
                            if (query["schedule"].contains("process_open_pipes"))
                            {
                                scheduled_queries["process_open_pipes"]["query"] = query["schedule"]["process_open_pipes"]["query"];
                                scheduled_queries["process_open_pipes"]["interval"] = query["schedule"]["process_open_pipes"]["interval"];
                                scheduled_queries["process_open_pipes"]["snapshot"] = query["schedule"]["process_open_pipes"]["snapshot"];
                            }
                            if (query["schedule"].contains("process_open_files"))
                            {
                                scheduled_queries["process_open_files"]["query"] = query["schedule"]["process_open_files"]["query"];
                                scheduled_queries["process_open_files"]["interval"] = query["schedule"]["process_open_files"]["interval"];
                                scheduled_queries["process_open_files"]["snapshot"] = query["schedule"]["process_open_files"]["snapshot"];
                            }
                            if (query["schedule"].contains("listening_ports"))
                            {
                                scheduled_queries["listening_ports"]["query"] = query["schedule"]["listening_ports"]["query"];
                                scheduled_queries["listening_ports"]["interval"] = query["schedule"]["listening_ports"]["interval"];
                                scheduled_queries["listening_ports"]["snapshot"] = query["schedule"]["listening_ports"]["snapshot"];
                            }
                            if (query["schedule"].contains("routes"))
                            {
                                scheduled_queries["routes"]["query"] = query["schedule"]["routes"]["query"];
                                scheduled_queries["routes"]["interval"] = query["schedule"]["routes"]["interval"];
                                scheduled_queries["routes"]["snapshot"] = query["schedule"]["routes"]["snapshot"];
                            }
                            if (query["schedule"].contains("logged_in_users"))
                            {
                                scheduled_queries["logged_in_users"]["query"] = query["schedule"]["logged_in_users"]["query"];
                                scheduled_queries["logged_in_users"]["interval"] = query["schedule"]["logged_in_users"]["interval"];
                                scheduled_queries["logged_in_users"]["snapshot"] = query["schedule"]["logged_in_users"]["snapshot"];
                            }
                            if (query["schedule"].contains("arp_cache"))
                            {
                                scheduled_queries["arp_cache"]["query"] = query["schedule"]["arp_cache"]["query"];
                                scheduled_queries["arp_cache"]["interval"] = query["schedule"]["arp_cache"]["interval"];
                                scheduled_queries["arp_cache"]["snapshot"] = query["schedule"]["arp_cache"]["snapshot"];
                            }
                            if (query["schedule"].contains("suid_bin"))
                            {
                                scheduled_queries["suid_bin"]["query"] = query["schedule"]["suid_bin"]["query"];
                                scheduled_queries["suid_bin"]["interval"] = query["schedule"]["suid_bin"]["interval"];
                                scheduled_queries["suid_bin"]["snapshot"] = query["schedule"]["suid_bin"]["snapshot"];
                            }
                            if (query["schedule"].contains("process_open_sockets"))
                            {
                                scheduled_queries["process_open_sockets"]["query"] = query["schedule"]["process_open_sockets"]["query"];
                                scheduled_queries["process_open_sockets"]["interval"] = query["schedule"]["process_open_sockets"]["interval"];
                                scheduled_queries["process_open_sockets"]["snapshot"] = query["schedule"]["process_open_sockets"]["snapshot"];
                            }
                            if (query["schedule"].contains("users"))
                            {
                                scheduled_queries["users"]["query"] = query["schedule"]["users"]["query"];
                                scheduled_queries["users"]["interval"] = query["schedule"]["users"]["interval"];
                                scheduled_queries["users"]["snapshot"] = query["schedule"]["users"]["snapshot"];
                            }
                            if (query["schedule"].contains("groups"))
                            {
                                scheduled_queries["groups"]["query"] = query["schedule"]["groups"]["query"];
                                scheduled_queries["groups"]["interval"] = query["schedule"]["groups"]["interval"];
                                scheduled_queries["groups"]["snapshot"] = query["schedule"]["groups"]["snapshot"];
                            }
                            if (query["schedule"].contains("cpu_time"))
                            {
                                scheduled_queries["cpu_time"]["query"] = query["schedule"]["cpu_time"]["query"];
                                scheduled_queries["cpu_time"]["interval"] = query["schedule"]["cpu_time"]["interval"];
                            }
                            if (query["schedule"].contains("memory_info"))
                            {
                                scheduled_queries["memory_info"]["query"] = query["schedule"]["memory_info"]["query"];
                                scheduled_queries["memory_info"]["interval"] = query["schedule"]["memory_info"]["interval"];
                            }
                            if (query["schedule"].contains("bpf_process_events"))
                            {
                                scheduled_queries["bpf_process_events"]["query"] = query["schedule"]["bpf_process_events"]["query"];
                                scheduled_queries["bpf_process_events"]["interval"] = query["schedule"]["bpf_process_events"]["interval"];
                                scheduled_queries["bpf_process_events"]["snapshot"] = query["schedule"]["bpf_process_events"]["snapshot"];
                            }
                            if (query["schedule"].contains("bpf_socket_events"))
                            {
                                scheduled_queries["bpf_socket_events"]["query"] = query["schedule"]["bpf_socket_events"]["query"];
                                scheduled_queries["bpf_socket_events"]["interval"] = query["schedule"]["bpf_socket_events"]["interval"];
                                scheduled_queries["bpf_socket_events"]["snapshot"] = query["schedule"]["bpf_socket_events"]["snapshot"];
                            }
                            if (query["schedule"].contains("bpf_file_events"))
                            {
                                scheduled_queries["bpf_file_events"]["query"] = query["schedule"]["bpf_file_events"]["query"];
                                scheduled_queries["bpf_file_events"]["interval"] = query["schedule"]["bpf_file_events"]["interval"];
                                scheduled_queries["bpf_file_events"]["snapshot"] = query["schedule"]["bpf_file_events"]["snapshot"];
                            }
                            if (query["schedule"].contains("docker_events"))
                            {
                                scheduled_queries["docker_events"]["query"] = query["schedule"]["docker_events"]["query"];
                                scheduled_queries["docker_events"]["interval"] = query["schedule"]["docker_events"]["interval"];
                            }
                            if (query["schedule"].contains("interface_addresses"))
                            {
                                scheduled_queries["interface_addresses"]["query"] = query["schedule"]["interface_addresses"]["query"];
                                scheduled_queries["interface_addresses"]["interval"] = query["schedule"]["interface_addresses"]["interval"];
                                scheduled_queries["interface_addresses"]["snapshot"] = query["schedule"]["interface_addresses"]["snapshot"];
                            }
                            if (query["schedule"].contains("win_process_events"))
                            {
                                scheduled_queries["win_process_events"]["query"] = query["schedule"]["win_process_events"]["query"];
                                scheduled_queries["win_process_events"]["interval"] = query["schedule"]["win_process_events"]["interval"];
                                scheduled_queries["win_process_events"]["snapshot"] = query["schedule"]["win_process_events"]["snapshot"];
                            }
                            if (query["schedule"].contains("win_socket_events"))
                            {
                                scheduled_queries["win_socket_events"]["query"] = query["schedule"]["win_socket_events"]["query"];
                                scheduled_queries["win_socket_events"]["interval"] = query["schedule"]["win_socket_events"]["interval"];
                                scheduled_queries["win_socket_events"]["snapshot"] = query["schedule"]["win_socket_events"]["snapshot"];
                            }
                            if (query["schedule"].contains("win_file_events"))
                            {
                                scheduled_queries["win_file_events"]["query"] = query["schedule"]["win_file_events"]["query"];
                                scheduled_queries["win_file_events"]["interval"] = query["schedule"]["win_file_events"]["interval"];
                                scheduled_queries["win_file_events"]["snapshot"] = query["schedule"]["win_file_events"]["snapshot"];
                            }
                            if (query["schedule"].contains("chrome_extensions"))
                            {
                                scheduled_queries["chrome_extensions"]["query"] = query["schedule"]["chrome_extensions"]["query"];
                                scheduled_queries["chrome_extensions"]["interval"] = query["schedule"]["chrome_extensions"]["interval"];
                                scheduled_queries["chrome_extensions"]["snapshot"] = query["schedule"]["chrome_extensions"]["snapshot"];
                            }
                            if (query["schedule"].contains("file"))
                            {
                                scheduled_queries["file"]["query"] = query["schedule"]["file"]["query"];
                                scheduled_queries["file"]["interval"] = query["schedule"]["file"]["interval"];
                                scheduled_queries["file"]["snapshot"] = query["schedule"]["file"]["snapshot"];
                            }
                            if (query["schedule"].contains("wmi_cli_event_consumers"))
                            {
                                scheduled_queries["wmi_cli_event_consumers"]["query"] = query["schedule"]["wmi_cli_event_consumers"]["query"];
                                scheduled_queries["wmi_cli_event_consumers"]["interval"] = query["schedule"]["wmi_cli_event_consumers"]["interval"];
                                scheduled_queries["wmi_cli_event_consumers"]["snapshot"] = query["schedule"]["wmi_cli_event_consumers"]["snapshot"];
                            }
                            if (query["schedule"].contains("osquery_schedule"))
                            {
                                scheduled_queries["osquery_schedule"]["query"] = query["schedule"]["osquery_schedule"]["query"];
                                scheduled_queries["osquery_schedule"]["interval"] = query["schedule"]["osquery_schedule"]["interval"];
                                scheduled_queries["osquery_schedule"]["snapshot"] = query["schedule"]["osquery_schedule"]["snapshot"];
                            }
                            if (query["schedule"].contains("etc_hosts"))
                            {
                                scheduled_queries["etc_hosts"]["query"] = query["schedule"]["etc_hosts"]["query"];
                                scheduled_queries["etc_hosts"]["interval"] = query["schedule"]["etc_hosts"]["interval"];
                                scheduled_queries["etc_hosts"]["snapshot"] = query["schedule"]["etc_hosts"]["snapshot"];
                            }
                            if (query["schedule"].contains("uptime"))
                            {
                                scheduled_queries["uptime"]["query"] = query["schedule"]["uptime"]["query"];
                                scheduled_queries["uptime"]["interval"] = query["schedule"]["uptime"]["interval"];
                                scheduled_queries["uptime"]["snapshot"] = query["schedule"]["uptime"]["snapshot"];
                            }
                            if (query["schedule"].contains("startup_items"))
                            {
                                scheduled_queries["startup_items"]["query"] = query["schedule"]["startup_items"]["query"];
                                scheduled_queries["startup_items"]["interval"] = query["schedule"]["startup_items"]["interval"];
                                scheduled_queries["startup_items"]["snapshot"] = query["schedule"]["startup_items"]["snapshot"];
                            }
                            if (query["schedule"].contains("patches"))
                            {
                                scheduled_queries["patches"]["query"] = query["schedule"]["patches"]["query"];
                                scheduled_queries["patches"]["interval"] = query["schedule"]["patches"]["interval"];
                                scheduled_queries["patches"]["snapshot"] = query["schedule"]["patches"]["snapshot"];
                            }
                            if (query["schedule"].contains("processes32"))
                            {
                                std::string update_query = query["schedule"]["processes32"]["query"];
                                std::string updated_query = update_query + " where start_time > " + std::to_string(max_start_time);
                                scheduled_queries["processes32"]["query"] = updated_query;
                                scheduled_queries["processes32"]["interval"] = query["schedule"]["processes32"]["interval"];
                                scheduled_queries["processes32"]["snapshot"] = query["schedule"]["processes32"]["snapshot"];
                            }
                            if (query["schedule"].contains("programs"))
                            {
                                scheduled_queries["programs"]["query"] = query["schedule"]["programs"]["query"];
                                scheduled_queries["programs"]["interval"] = query["schedule"]["programs"]["interval"];
                                scheduled_queries["programs"]["snapshot"] = query["schedule"]["programs"]["snapshot"];
                            }
                            if (query["schedule"].contains("kernel_info"))
                            {
                                scheduled_queries["kernel_info"]["query"] = query["schedule"]["kernel_info"]["query"];
                                scheduled_queries["kernel_info"]["interval"] = query["schedule"]["kernel_info"]["interval"];
                            }
                            if (query["schedule"].contains("deb_packages"))
                            {
                                scheduled_queries["deb_packages"]["query"] = query["schedule"]["deb_packages"]["query"];
                                scheduled_queries["deb_packages"]["interval"] = query["schedule"]["deb_packages"]["interval"];
                            }
                            if (query["schedule"].contains("rpm_packages"))
                            {
                                scheduled_queries["rpm_packages"]["query"] = query["schedule"]["rpm_packages"]["query"];
                                scheduled_queries["rpm_packages"]["interval"] = query["schedule"]["rpm_packages"]["interval"];
                            }
                            if (query["schedule"].contains("chocolatey_packages"))
                            {
                                scheduled_queries["chocolatey_packages"]["query"] = query["schedule"]["chocolatey_packages"]["query"];
                                scheduled_queries["chocolatey_packages"]["interval"] = query["schedule"]["chocolatey_packages"]["interval"];
                            }
                            if (query["schedule"].contains("interface_details"))
                            {
                                scheduled_queries["interface_details"]["query"] = query["schedule"]["interface_details"]["query"];
                                scheduled_queries["interface_details"]["interval"] = query["schedule"]["interface_details"]["interval"];
                            }
                            if (query["schedule"].contains("ie_extensions"))
                            {
                                scheduled_queries["ie_extensions"]["query"] = query["schedule"]["ie_extensions"]["query"];
                                scheduled_queries["ie_extensions"]["interval"] = query["schedule"]["ie_extensions"]["interval"];
                            }
                            if (query["schedule"].contains("firefox_addons"))
                            {
                                scheduled_queries["firefox_addons"]["query"] = query["schedule"]["firefox_addons"]["query"];
                                scheduled_queries["firefox_addons"]["interval"] = query["schedule"]["firefox_addons"]["interval"];
                            }
                            if (query["schedule"].contains("npm_packages"))
                            {
                                scheduled_queries["npm_packages"]["query"] = query["schedule"]["npm_packages"]["query"];
                                scheduled_queries["npm_packages"]["interval"] = query["schedule"]["npm_packages"]["interval"];
                            }
                            if (query["schedule"].contains("portage_packages"))
                            {
                                scheduled_queries["portage_packages"]["query"] = query["schedule"]["portage_packages"]["query"];
                                scheduled_queries["portage_packages"]["interval"] = query["schedule"]["portage_packages"]["interval"];
                            }
                            osquery_configuration["schedule"] = scheduled_queries;
                            if (query.contains("file_paths"))
                            {
                                osquery_configuration["file_paths"] = query["file_paths"];
                            }
                            if (query.contains("file_accesses"))
                            {
                                osquery_configuration["file_accesses"] = query["file_accesses"];
                            }
                            if (query.contains("win_include_paths"))
                            {
                                osquery_configuration["win_include_paths"] = query["win_include_paths"];
                            }
                            if (query.contains("plgx_event_filters"))
                            {
                                osquery_configuration["plgx_event_filters"] = query["plgx_event_filters"];
                            }
                            if (query.contains("options"))
                            {
                                osquery_configuration["options"] = query["options"];
                            }
                            if (query.contains("decorators"))
                            {
                                osquery_configuration["decorators"] = query["decorators"];
                            }


                            //std::cout << query.dump() << std::endl;
                        }
                    }
                }
                connection->unprepare("con_id");
                connection->unprepare("config_i");
                connection->unprepare("update_last_seen");
            }
            catch (pqxx::sql_error const &e)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
            }
            catch (std::exception const &e)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
            }
            catch (...)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Unknown error";
            }
        }
    }
    catch (const nlohmann::json::parse_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
    }
    catch (const nlohmann::json::type_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
    }
    catch (std::exception const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Unknown error";
    }

    std::string reply = osquery_configuration.dump();

    response.status = http::status::ok;
    response.body = reply;
    // std::cout<<response.body<<std::endl;
    response.header["Content-Type"] = "application/json";
    return response;
}

HTTPMessage logger(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json log_response;
    // std::unordered_set<std::string> set_tables;
    // bool fl=true;
    // if(fl)
    // {
    //     set_tables.insert({
    //         "process_events", "socket_events", "user_events", "process_envs", "process_open_pipes", "process_open_files", "listening_ports",
    //         "routes", "logged_in_users", "arp_cache", "process_file_events", "suid_bin", "process_open_sockets", "users", "groups", "cpu_time", "memory_info",
    //         "bpf_process_events", "bpf_socket_events", "bpf_file_events", "docker_events", "interface_addresses", "win_process_events", "win_socket_events",
    //         "win_file_events", "chrome_extensions", "osquery_schedule", "etc_hosts", "uptime", "startup_items", "patches", "file", "wmi_cli_event_consumers", "processes32", "programs"
    //     });
    //     fl=false;
    // }
    try
    {
        auto logger_data = nlohmann::json::parse(request.body);
        // std::cout<<request.body<<std::endl;
        if (logger_data.contains("log_type") && logger_data.contains("node_key"))
        {
            std::string key = logger_data["node_key"];
            std::string host_identifier, hostname;
            pqxx::work transaction{*connection};
            connection->prepare("get_hostname", "select hostname from nodes where node_key = $1");
            pqxx::result host = transaction.exec_prepared("get_hostname", key);
            hostname = host.at(0)["hostname"].as<std::string>();
            connection->unprepare("get_hostname");
            bool isKeyValid;

            try
            {
                // pqxx::work transaction{*connection};

                pqxx::result result = transaction.exec_prepared("validate_node_key", key);
                if (result.size() == 1)
                {
                    isKeyValid = true;
                    host_identifier = result.at(0)["host_identifier"].as<std::string>();
                    log_response["node_invalid"] = false;
                    connection->prepare("update_last_seen", "update nodes set last_seen = $1 where host_identifier = $2;");
                    int last_seen;
                    std::time_t t = std::time(0);
                    last_seen = t;
                    pqxx::result queries = transaction.exec_prepared("update_last_seen", last_seen, host_identifier);
                    transaction.commit();
                    connection->unprepare("update_last_seen");
                }
                else
                {
                    isKeyValid = false;
                    log_response["node_invalid"] = true;
                }

                if (isKeyValid && logger_data["log_type"] == "result" && logger_data.contains("data"))
                {
                    auto entries = logger_data["data"];
                    // std::cout << entries <<std::endl;
                    if (entries.is_array())
                    {
                        for (auto entry : entries)
                        {
                            // std::cout << entry["name"] << std::endl;
                            // std::cout << entry << std::endl;
                            // if (entry.contains("hostIdentifier") && entry.contains("name") && entry.contains("columns") && (set_tables.find(entry["name"])!=set_tables.end()))
                            if (entry.contains("hostIdentifier") && entry.contains("name") && entry.contains("columns"))
                            {
                                std::cout << "Log request " << host_identifier << std::endl;
                                std::cout << entry["name"] << std::endl;
                                std::string action = "";
                                if (entry.contains("action"))
                                {
                                    action = entry["action"];
                                }
                                if (host_identifier != entry["hostIdentifier"])
                                {
                                    continue;
                                }

                                if (entry["name"] == "process_events")
                                {
                                    long unixTime, time, atime, btime, ctime, mtime, uptime;
                                    long auid, egid, euid, fsgid, fsuid, gid, mode, owner_gid, owner_uid, pid, ppid, sgid, suid, uid;
                                    std::string cwd, cmdline, path, syscall;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    atime = column.contains("atime") ? atol(column["atime"].get<std::string>().c_str()) : 0;
                                    btime = column.contains("btime") ? atol(column["btime"].get<std::string>().c_str()) : 0;
                                    ctime = column.contains("ctime") ? atol(column["ctime"].get<std::string>().c_str()) : 0;
                                    mtime = column.contains("mtime") ? atol(column["mtime"].get<std::string>().c_str()) : 0;
                                    uptime = column.contains("uptime") ? atol(column["uptime"].get<std::string>().c_str()) : 0;

                                    auid = column.contains("auid") ? atol(column["auid"].get<std::string>().c_str()) : 0;
                                    egid = column.contains("egid") ? atol(column["egid"].get<std::string>().c_str()) : 0;
                                    euid = column.contains("euid") ? atol(column["euid"].get<std::string>().c_str()) : 0;
                                    fsgid = column.contains("fsgid") ? atol(column["fsgid"].get<std::string>().c_str()) : 0;
                                    fsuid = column.contains("fsuid") ? atol(column["fsuid"].get<std::string>().c_str()) : 0;
                                    gid = column.contains("gid") ? atol(column["gid"].get<std::string>().c_str()) : 0;
                                    mode = column.contains("mode") ? atol(column["mode"].get<std::string>().c_str()) : 0;
                                    owner_gid = column.contains("owner_gid") ? atol(column["owner_gid"].get<std::string>().c_str()) : 0;
                                    owner_uid = column.contains("owner_uid") ? atol(column["owner_uid"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    ppid = column.contains("parent") ? atol(column["parent"].get<std::string>().c_str()) : 0;
                                    sgid = column.contains("sgid") ? atol(column["sgid"].get<std::string>().c_str()) : 0;
                                    suid = column.contains("suid") ? atol(column["suid"].get<std::string>().c_str()) : 0;
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;

                                    cwd = column.contains("cwd") ? column["cwd"] : "";
                                    cmdline = column.contains("cmdline") ? column["cmdline"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    syscall = column.contains("syscall") ? column["syscall"] : "";

                                    transaction.exec_prepared("process_event_add", host_identifier, hostname, unixTime, atime, btime, ctime, mtime, time, uptime,
                                                              auid, egid, euid, fsgid, fsuid, gid, mode, owner_gid, owner_uid, pid, ppid, sgid, suid, uid,
                                                              cwd, cmdline, path, syscall);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "socket_events")
                                {
                                    long unixTime, time, uptime, auid, family, fd, local_port, remote_port, pid;
                                    std::string action, local_address, remote_address, path;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;

                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    uptime = column.contains("uptime") ? atol(column["uptime"].get<std::string>().c_str()) : 0;

                                    auid = column.contains("auid") ? atol(column["auid"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    family = column.contains("family") ? atol(column["family"].get<std::string>().c_str()) : 0;
                                    fd = column.contains("fd") ? atol(column["fd"].get<std::string>().c_str()) : 0;
                                    local_port = column.contains("local_port") ? atol(column["local_port"].get<std::string>().c_str()) : 0;
                                    remote_port = column.contains("remote_port") ? atol(column["remote_port"].get<std::string>().c_str()) : 0;

                                    action = column.contains("action") ? column["action"] : "";
                                    local_address = column.contains("local_address") ? column["local_address"] : "";
                                    remote_address = column.contains("remote_address") ? column["remote_address"] : "";
                                    path = column.contains("path") ? column["path"] : "";

                                    transaction.exec_prepared("socket_event_add", host_identifier, hostname, unixTime, time, uptime, action, auid, family, fd,
                                                              local_address, local_port, remote_address, remote_port, pid, path);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "user_events")
                                {
                                    long unixTime, time, uptime, auid, pid, type, uid;
                                    std::string path, address, message, terminal;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;

                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    uptime = column.contains("uptime") ? atol(column["uptime"].get<std::string>().c_str()) : 0;

                                    auid = column.contains("auid") ? atol(column["auid"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    type = column.contains("type") ? atol(column["type"].get<std::string>().c_str()) : 0;
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;

                                    path = column.contains("path") ? column["path"] : "";
                                    address = column.contains("address") ? column["address"] : "";
                                    message = column.contains("message") ? column["message"] : "";
                                    terminal = column.contains("terminal") ? column["terminal"] : "";

                                    transaction.exec_prepared("user_event_add", host_identifier, unixTime, time, uptime, auid, pid, type, uid,
                                                              path, address, message, terminal);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "process_envs")
                                {
                                    long pid;
                                    std::string key, value;

                                    auto column = entry["columns"];

                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    key = column.contains("key") ? column["key"] : "";
                                    value = column.contains("value") ? column["value"] : "";

                                    transaction.exec_prepared("process_envs_add", host_identifier, pid, key, value, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "process_open_pipes")
                                {
                                    long pid, fd, inode, partner_pid, partner_fd, unixTime;
                                    std::string mode, type, partner_mode;

                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    fd = column.contains("fd") ? atol(column["fd"].get<std::string>().c_str()) : 0;
                                    inode = column.contains("inode") ? atol(column["inode"].get<std::string>().c_str()) : 0;
                                    partner_pid = column.contains("partner_pid") ? atol(column["partner_pid"].get<std::string>().c_str()) : 0;
                                    partner_fd = column.contains("partner_fd") ? atol(column["partner_fd"].get<std::string>().c_str()) : 0;
                                    mode = column.contains("mode") ? column["mode"] : "";
                                    type = column.contains("type") ? column["type"] : "";
                                    partner_mode = column.contains("partner_mode") ? column["partner_mode"] : "";

                                    transaction.exec_prepared("process_open_pipes_add", host_identifier, unixTime, pid, fd, mode, inode, type, partner_pid, partner_fd, partner_mode, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "process_open_files")
                                {
                                    long pid, fd, unixTime;
                                    std::string path;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    fd = column.contains("fd") ? atol(column["fd"].get<std::string>().c_str()) : 0;
                                    path = column.contains("path") ? column["path"] : "";

                                    transaction.exec_prepared("process_open_files_add", host_identifier, unixTime, pid, fd, path, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "listening_ports")
                                {
                                    long pid, port, protocol, family, fd, socket;
                                    std::string address, path, net_namespace;

                                    auto column = entry["columns"];

                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    port = column.contains("port") ? atol(column["port"].get<std::string>().c_str()) : 0;
                                    protocol = column.contains("protocol") ? atol(column["protocol"].get<std::string>().c_str()) : 0;
                                    family = column.contains("family") ? atol(column["family"].get<std::string>().c_str()) : 0;
                                    socket = column.contains("socket") ? atol(column["socket"].get<std::string>().c_str()) : 0;
                                    fd = column.contains("fd") ? atol(column["fd"].get<std::string>().c_str()) : 0;
                                    path = column.contains("path") ? column["path"] : "";
                                    address = column.contains("address") ? column["address"] : "";
                                    net_namespace = column.contains("net_namespace") ? column["net_namespace"] : "";

                                    transaction.exec_prepared("listening_ports_add", host_identifier, pid, port, protocol, family, address, fd, socket, path, net_namespace, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "routes")
                                {
                                    long netmask, flags, mtu, metric, hopcount;
                                    std::string destination, gateway, source, interface, type;

                                    auto column = entry["columns"];

                                    netmask = column.contains("netmask") ? atol(column["netmask"].get<std::string>().c_str()) : 0;
                                    flags = column.contains("flags") ? atol(column["flags"].get<std::string>().c_str()) : 0;
                                    mtu = column.contains("mtu") ? atol(column["mtu"].get<std::string>().c_str()) : 0;
                                    metric = column.contains("metric") ? atol(column["metric"].get<std::string>().c_str()) : 0;
                                    hopcount = column.contains("hopcount") ? atol(column["hopcount"].get<std::string>().c_str()) : 0;
                                    destination = column.contains("destination") ? column["destination"] : "";
                                    gateway = column.contains("gateway") ? column["gateway"] : "";
                                    source = column.contains("source") ? column["source"] : "";
                                    interface = column.contains("interface") ? column["interface"] : "";
                                    type = column.contains("type") ? column["type"] : "";

                                    transaction.exec_prepared("routes_add", host_identifier, destination, netmask, gateway, source, flags, interface, mtu, metric, type, hopcount, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "logged_in_users")
                                {
                                    long time, pid, unixTime;
                                    std::string type, logged_user, tty, host, sid, registry_hive;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    type = column.contains("type") ? column["type"] : "";
                                    logged_user = column.contains("user") ? column["user"] : "";
                                    tty = column.contains("tty") ? column["tty"] : "";
                                    host = column.contains("host") ? column["host"] : "";
                                    sid = column.contains("sid") ? column["sid"] : "";
                                    registry_hive = column.contains("registry_hive") ? column["registry_hive"] : "";

                                    transaction.exec_prepared("logged_in_users_add", host_identifier, unixTime, type, logged_user, tty, host, time, pid, sid, registry_hive, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "arp_cache")
                                {
                                    std::string address, mac, interface, permanent;

                                    auto column = entry["columns"];

                                    address = column.contains("address") ? column["address"] : "";
                                    mac = column.contains("mac") ? column["mac"] : "";
                                    interface = column.contains("interface") ? column["interface"] : "";
                                    permanent = column.contains("permanent") ? column["permanent"] : "";

                                    transaction.exec_prepared("arp_cache_add", host_identifier, address, mac, interface, permanent, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "process_file_events")
                                {
                                    long pid, ppid, time, uptime;
                                    std::string operation, executable, partial, cwd, path, dest_path, uid, gid, auid, euid, egid, fsuid, fsgid, suid, sgid, eid;

                                    auto column = entry["columns"];

                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    uptime = column.contains("uptime") ? atol(column["uptime"].get<std::string>().c_str()) : 0;
                                    ppid = column.contains("ppid") ? atol(column["ppid"].get<std::string>().c_str()) : 0;
                                    operation = column.contains("operation") ? column["operation"] : "";
                                    executable = column.contains("executable") ? column["executable"] : "";
                                    partial = column.contains("partial") ? column["partial"] : "";
                                    cwd = column.contains("cwd") ? column["cwd"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    dest_path = column.contains("dest_path") ? column["dest_path"] : "";
                                    uid = column.contains("uid") ? column["uid"] : "";
                                    gid = column.contains("gid") ? column["gid"] : "";
                                    auid = column.contains("auid") ? column["auid"] : "";
                                    euid = column.contains("euid") ? column["euid"] : "";
                                    egid = column.contains("egid") ? column["egid"] : "";
                                    fsuid = column.contains("fsuid") ? column["fsuid"] : "";
                                    fsgid = column.contains("fsgid") ? column["fsgid"] : "";
                                    suid = column.contains("suid") ? column["suid"] : "";
                                    sgid = column.contains("sgid") ? column["sgid"] : "";
                                    eid = column.contains("eid") ? column["eid"] : "";

                                    transaction.exec_prepared("process_file_events_add", host_identifier, operation, pid, ppid, time, executable, partial, cwd, path, dest_path, uid, gid, auid, euid, egid, fsuid, fsgid, suid, sgid, uptime, eid);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "file_events")
                                {
                                    int hashed;
                                    std::string target_path, category, action, md5, sha1, sha256, eid, mode;
                                    long unixTime, transaction_id, inode, uid, gid, size, atime, mtime, ctime, time;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    target_path = column.contains("target_path") ? column["target_path"] : "";
                                    category = column.contains("category") ? column["category"] : "";
                                    action = column.contains("action") ? column["action"] : "";
                                    transaction_id = column.contains("transaction_id") ? atol(column["transaction_id"].get<std::string>().c_str()) : 0;
                                    inode = column.contains("inode") ? atol(column["inode"].get<std::string>().c_str()) : 0;
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;
                                    gid = column.contains("gid") ? atol(column["gid"].get<std::string>().c_str()) : 0;
                                    mode = column.contains("mode") ? column["mode"] : "";
                                    size = column.contains("size") ? atol(column["size"].get<std::string>().c_str()) : 0;
                                    atime = column.contains("atime") ? atol(column["atime"].get<std::string>().c_str()) : 0;
                                    mtime = column.contains("mtime") ? atol(column["mtime"].get<std::string>().c_str()) : 0;
                                    ctime = column.contains("ctime") ? atol(column["ctime"].get<std::string>().c_str()) : 0;
                                    md5 = column.contains("md5") ? column["md5"] : "";
                                    sha1 = column.contains("sha1") ? column["sha1"] : "";
                                    sha256 = column.contains("sha256") ? column["sha256"] : "";
                                    hashed = entry.contains("hashed") ? entry["hashed"].get<int>() : 0;
                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    eid = column.contains("eid") ? column["eid"] : "";
                                    
                                    transaction.exec_prepared("file_events", host_identifier, hostname, unixTime, target_path, category, action, transaction_id, inode, uid, gid, mode, size, atime, mtime, ctime, md5, sha1, sha256, hashed, time, eid);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "suid_bin")
                                {
                                    std::string path, username, groupname, permissions;
                                    long unixTime;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    path = column.contains("path") ? column["path"] : "";
                                    username = column.contains("username") ? column["username"] : "";
                                    groupname = column.contains("groupname") ? column["groupname"] : "";
                                    permissions = column.contains("permissions") ? column["permissions"] : "";

                                    transaction.exec_prepared("suid_bin_add", host_identifier, unixTime, path, username, groupname, permissions, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "process_open_sockets")
                                {

                                    long pid, unixTime, fd, socket, family, protocol, local_port, remote_port;
                                    std::string local_address, remote_address, path, state, net_namespace;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    fd = column.contains("fd") ? atol(column["fd"].get<std::string>().c_str()) : 0;
                                    socket = column.contains("socket") ? atol(column["socket"].get<std::string>().c_str()) : 0;
                                    family = column.contains("family") ? atol(column["family"].get<std::string>().c_str()) : 0;
                                    protocol = column.contains("protocol") ? atol(column["protocol"].get<std::string>().c_str()) : 0;
                                    local_port = column.contains("local_port") ? atol(column["local_port"].get<std::string>().c_str()) : 0;
                                    remote_port = column.contains("remote_port") ? atol(column["remote_port"].get<std::string>().c_str()) : 0;
                                    local_address = column.contains("local_address") ? column["local_address"] : "";
                                    remote_address = column.contains("remote_address") ? column["remote_address"] : "";
                                    state = column.contains("state") ? column["state"] : "";
                                    net_namespace = column.contains("net_namespace") ? column["net_namespace"] : "";
                                    state = column.contains("state") ? column["state"] : "";

                                    transaction.exec_prepared("process_open_sockets_add", host_identifier, unixTime, pid, fd, socket, family, protocol, local_address, remote_address, local_port, remote_port, path, state, net_namespace, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "users")
                                {
                                    std::string username, description, directory, shell, uuid, type;
                                    long uid, gid, uid_signed, gid_signed, is_hidden;

                                    auto column = entry["columns"];
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;
                                    gid = column.contains("gid") ? atol(column["gid"].get<std::string>().c_str()) : 0;
                                    uid_signed = column.contains("uid_signed") ? atol(column["uid_signed"].get<std::string>().c_str()) : 0;
                                    gid_signed = column.contains("gid_signed") ? atol(column["gid_signed"].get<std::string>().c_str()) : 0;
                                    is_hidden = column.contains("is_hidden") ? atol(column["is_hidden"].get<std::string>().c_str()) : 0;
                                    description = column.contains("description") ? column["description"] : "";
                                    username = column.contains("username") ? column["username"] : "";
                                    directory = column.contains("directory") ? column["directory"] : "";
                                    shell = column.contains("shell") ? column["shell"] : "";
                                    uuid = column.contains("uuid") ? column["uuid"] : "";
                                    type = column.contains("type") ? column["type"] : "";

                                    transaction.exec_prepared("users_add", host_identifier, uid, gid, uid_signed, gid_signed, is_hidden, username, description, directory, shell, uuid, type, action);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "groups")
                                {
                                    std::string groupname, group_sid, comment;
                                    long gid, gid_signed, is_hidden;

                                    auto column = entry["columns"];
                                    gid = column.contains("gid") ? atol(column["gid"].get<std::string>().c_str()) : 0;
                                    gid_signed = column.contains("gid_signed") ? atol(column["gid_signed"].get<std::string>().c_str()) : 0;
                                    is_hidden = column.contains("is_hidden") ? atol(column["is_hidden"].get<std::string>().c_str()) : 0;
                                    groupname = column.contains("groupname") ? column["groupname"] : "";
                                    group_sid = column.contains("group_sid") ? column["group_sid"] : "";
                                    comment = column.contains("comment") ? column["comment"] : "";

                                    transaction.exec_prepared("groups_add", host_identifier, gid, gid_signed, is_hidden, groupname, group_sid, comment, action);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "cpu_time")
                                {
                                    long core, users, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice, unixTime;

                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    core = column.contains("core") ? atol(column["core"].get<std::string>().c_str()) : 0;
                                    users = column.contains("users") ? atol(column["users"].get<std::string>().c_str()) : 0;
                                    nice = column.contains("nice") ? atol(column["nice"].get<std::string>().c_str()) : 0;
                                    system = column.contains("system") ? atol(column["system"].get<std::string>().c_str()) : 0;
                                    idle = column.contains("idle") ? atol(column["idle"].get<std::string>().c_str()) : 0;
                                    iowait = column.contains("iowait") ? atol(column["iowait"].get<std::string>().c_str()) : 0;
                                    irq = column.contains("irq") ? atol(column["irq"].get<std::string>().c_str()) : 0;
                                    softirq = column.contains("softirq") ? atol(column["softirq"].get<std::string>().c_str()) : 0;
                                    steal = column.contains("steal") ? atol(column["steal"].get<std::string>().c_str()) : 0;
                                    guest = column.contains("guest") ? atol(column["guest"].get<std::string>().c_str()) : 0;
                                    guest_nice = column.contains("guest_nice") ? atol(column["guest_nice"].get<std::string>().c_str()) : 0;

                                    transaction.exec_prepared("cpu_time", host_identifier, unixTime, core, users, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "memory_info")
                                {
                                    long memory_total, memory_free, buffers, cached, swap_cached, active, inactive, swap_total, swap_free, unixTime;

                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    memory_total = column.contains("memory_total") ? atol(column["memory_total"].get<std::string>().c_str()) : 0;
                                    memory_free = column.contains("memory_free") ? atol(column["memory_free"].get<std::string>().c_str()) : 0;
                                    buffers = column.contains("buffers") ? atol(column["buffers"].get<std::string>().c_str()) : 0;
                                    cached = column.contains("cached") ? atol(column["cached"].get<std::string>().c_str()) : 0;
                                    swap_cached = column.contains("swap_cached") ? atol(column["swap_cached"].get<std::string>().c_str()) : 0;
                                    active = column.contains("active") ? atol(column["active"].get<std::string>().c_str()) : 0;
                                    inactive = column.contains("inactive") ? atol(column["inactive"].get<std::string>().c_str()) : 0;
                                    swap_total = column.contains("swap_total") ? atol(column["swap_total"].get<std::string>().c_str()) : 0;
                                    swap_free = column.contains("swap_free") ? atol(column["swap_free"].get<std::string>().c_str()) : 0;

                                    transaction.exec_prepared("memory_info", host_identifier, unixTime, memory_total, memory_free, buffers, cached, swap_cached, active, inactive, swap_total, swap_free);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "bpf_process_events")
                                {
                                    std::string exit_code, syscall, path, cwd, cmdline, json_cmdline, ntime, md5, sha1, sha256, container_name;
                                    long unixTime, tid, pid, parent, uid, gid, cid, probe_error, duration, time, eid, is_container_process, container_age, uptime;

                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    tid = column.contains("tid") ? atol(column["tid"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    parent = column.contains("parent") ? atol(column["parent"].get<std::string>().c_str()) : 0;
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;
                                    gid = column.contains("gid") ? atol(column["gid"].get<std::string>().c_str()) : 0;
                                    cid = column.contains("cid") ? atol(column["cid"].get<std::string>().c_str()) : 0;
                                    exit_code = column.contains("exit_code") ? column["exit_code"] : "";
                                    probe_error = column.contains("probe_error") ? atol(column["probe_error"].get<std::string>().c_str()) : 0;
                                    syscall = column.contains("syscall") ? column["syscall"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    cwd = column.contains("cwd") ? column["cwd"] : "";
                                    cmdline = column.contains("cmdline") ? column["cmdline"] : "";
                                    duration = column.contains("duration") ? atol(column["duration"].get<std::string>().c_str()) : 0;
                                    json_cmdline = column.contains("json_cmdline") ? column["json_cmdline"] : "";
                                    ntime = column.contains("ntime") ? column["ntime"] : "";
                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    eid = column.contains("eid") ? atol(column["eid"].get<std::string>().c_str()) : 0;
                                    md5 = column.contains("md5") ? column["md5"] : "";
                                    sha1 = column.contains("sha1") ? column["sha1"] : "";
                                    sha256 = column.contains("sha256") ? column["sha256"] : "";
                                    is_container_process = column.contains("is_container_processid") ? atol(column["is_container_process"].get<std::string>().c_str()) : 0;
                                    container_age = column.contains("container_age") ? atol(column["container_age"].get<std::string>().c_str()) : 0;
                                    container_name = column.contains("container_name") ? column["container_name"] : "";
                                    // uptime = column.contains("uptime") ? atol(column["uptime"].get<std::string>().c_str()) : 0;
                                    long _ntime = stol(ntime);
                                    uptime = _ntime / 1000000000;
                                    transaction.exec_prepared("bpf_process_events", host_identifier, hostname, unixTime, tid, pid, parent, uid, gid, cid,
                                                              exit_code, probe_error, syscall, path, cwd, cmdline, duration, json_cmdline, ntime, time, eid, md5, sha1, sha256, is_container_process, container_age, container_name, uptime);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "bpf_socket_events")
                                {

                                    std::string exit_code, syscall, path, fd, local_address, remote_address, cwd, cmdline, ntime;
                                    long unixTime, tid, pid, parent, uid, gid, cid, probe_error, family, type, protocol, local_port, remote_port, duration, time, eid, uptime;

                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    tid = column.contains("tid") ? atol(column["tid"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    parent = column.contains("parent") ? atol(column["parent"].get<std::string>().c_str()) : 0;
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;
                                    gid = column.contains("gid") ? atol(column["gid"].get<std::string>().c_str()) : 0;
                                    cid = column.contains("cid") ? atol(column["cid"].get<std::string>().c_str()) : 0;
                                    exit_code = column.contains("exit_code") ? column["exit_code"] : "";
                                    probe_error = column.contains("probe_error") ? atol(column["probe_error"].get<std::string>().c_str()) : 0;
                                    syscall = column.contains("syscall") ? column["syscall"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    fd = column.contains("fd") ? column["fd"] : "";
                                    family = column.contains("family") ? atol(column["family"].get<std::string>().c_str()) : 0;
                                    type = column.contains("type") ? atol(column["type"].get<std::string>().c_str()) : 0;
                                    protocol = column.contains("protocol") ? atol(column["protocol"].get<std::string>().c_str()) : 0;
                                    local_address = column.contains("local_address") ? column["local_address"] : "";
                                    remote_address = column.contains("remote_address") ? column["remote_address"] : "";
                                    local_port = column.contains("local_port") ? atol(column["local_port"].get<std::string>().c_str()) : 0;
                                    remote_port = column.contains("remote_port") ? atol(column["remote_port"].get<std::string>().c_str()) : 0;
                                    duration = column.contains("duration") ? atol(column["duration"].get<std::string>().c_str()) : 0;
                                    ntime = column.contains("ntime") ? column["ntime"] : "";
                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    eid = column.contains("eid") ? atol(column["eid"].get<std::string>().c_str()) : 0;
                                    uptime = column.contains("uptime") ? atol(column["uptime"].get<std::string>().c_str()) : 0;

                                    transaction.exec_prepared("bpf_socket_events", host_identifier, hostname, unixTime, tid, pid, parent, uid, gid, cid,
                                                              exit_code, probe_error, syscall, path, fd, family, type, protocol, local_address, remote_address, local_port, remote_port, duration, ntime, time, eid, uptime);
                                    transaction.commit();
                                }

                                else if (entry["name"] == "bpf_file_events")
                                {

                                    std::string exit_code, syscall, path, file_path, ntime, flags;
                                    long tid, pid, parent, uid, gid, cid, probe_error, duration, time, uptime, unixTime;

                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    tid = column.contains("tid") ? atol(column["tid"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    parent = column.contains("parent") ? atol(column["parent"].get<std::string>().c_str()) : 0;
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;
                                    gid = column.contains("gid") ? atol(column["gid"].get<std::string>().c_str()) : 0;
                                    cid = column.contains("cid") ? atol(column["cid"].get<std::string>().c_str()) : 0;
                                    exit_code = column.contains("exit_code") ? column["exit_code"] : "";
                                    probe_error = column.contains("probe_error") ? atol(column["probe_error"].get<std::string>().c_str()) : 0;
                                    syscall = column.contains("syscall") ? column["syscall"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    duration = column.contains("duration") ? atol(column["duration"].get<std::string>().c_str()) : 0;
                                    ntime = column.contains("ntime") ? column["ntime"] : "";
                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    file_path = column.contains("file_path") ? column["file_path"] : "";
                                    uptime = column.contains("uptime") ? atol(column["uptime"].get<std::string>().c_str()) : 0;
                                    flags = column.contains("flags") ? column["flags"] : "";

                                    transaction.exec_prepared("bpf_file_events", host_identifier, hostname, unixTime, tid, pid, parent, uid, gid, cid, exit_code, probe_error, syscall, path, file_path, flags, ntime, duration, time, uptime);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "docker_events")
                                {

                                    std::string action, from, status, scope, time, type, path, cmdline, pid, container_id, container_image, container_name,
                                        privileged, added_capabilities, dropped_capabilities, port_binding;

                                    auto column = entry["columns"];
                                    action = column.contains("action") ? column["action"] : "";
                                    from = column.contains("from") ? column["from"] : "";
                                    status = column.contains("status") ? column["status"] : "";
                                    scope = column.contains("scope") ? column["scope"] : "";
                                    time = column.contains("time") ? column["time"] : "";
                                    type = column.contains("type") ? column["type"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    cmdline = column.contains("cmdline") ? column["cmdline"] : "";
                                    pid = column.contains("pid") ? column["pid"] : "";
                                    container_id = column.contains("container_id") ? column["container_id"] : "";
                                    container_image = column.contains("container_image") ? column["container_image"] : "";
                                    container_name = column.contains("container_name") ? column["container_name"] : "";
                                    privileged = column.contains("privileged") ? column["privileged"] : "";
                                    added_capabilities = column.contains("added_capabilities") ? column["added_capabilities"] : "";
                                    dropped_capabilities = column.contains("dropped_capabilities") ? column["dropped_capabilities"] : "";
                                    port_binding = column.contains("port_binding") ? column["port_binding"] : "";

                                    transaction.exec_prepared("docker_events", host_identifier, hostname, action, from, status, scope, time, type, path, cmdline, pid, container_id, container_image, container_name, privileged, added_capabilities, dropped_capabilities, port_binding);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "interface_addresses")
                                {

                                    std::string interface, address, mask, broadcast, point_to_point, type;
                                    long unixTime;

                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    interface = column.contains("interface") ? column["interface"] : "";
                                    address = column.contains("address") ? column["address"] : "";
                                    mask = column.contains("mask") ? column["mask"] : "";
                                    broadcast = column.contains("broadcast") ? column["broadcast"] : "";
                                    point_to_point = column.contains("point_to_point") ? column["point_to_point"] : "";
                                    type = column.contains("type") ? column["type"] : "";

                                    transaction.exec_prepared("interface_addresses", host_identifier, unixTime, interface, address, mask, broadcast, point_to_point, type);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "win_process_events")
                                {
                                    long unixTime, time, uptime, eventid, pid, ppid;
                                    std::string cmdline, path, action, eid, process_guid, parent_process_guid, parent_path, owner_uid, utc_time, sha256;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    action = column.contains("action") ? column["action"] : "";
                                    eid = column.contains("eid") ? column["eid"] : "";
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    process_guid = column.contains("process_guid") ? column["process_guid"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    cmdline = column.contains("cmdline") ? column["cmdline"] : "";
                                    ppid = column.contains("parent_pid") ? atol(column["parent_pid"].get<std::string>().c_str()) : 0;
                                    parent_process_guid = column.contains("parent_process_guid") ? column["parent_process_guid"] : "";
                                    parent_path = column.contains("parent_path") ? column["parent_path"] : "";
                                    owner_uid = column.contains("owner_uid") ? column["owner_uid"] : "";
                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    uptime = atol(entry["decorations"]["uptime"].get<std::string>().c_str());
                                    utc_time = column.contains("utc_time") ? column["utc_time"] : "";
                                    eventid = column.contains("eventid") ? atol(column["eventid"].get<std::string>().c_str()) : 0;
                                    sha256 = column.contains("sha256") ? column["sha256"] : "";

                                    transaction.exec_prepared("win_process_events", host_identifier, hostname, unixTime, action, eid, pid, process_guid, path, cmdline, ppid,
                                                                                     parent_process_guid, parent_path, owner_uid, time, uptime, utc_time, eventid, sha256);
                                    transaction.commit();

                                }
                                else if (entry["name"] == "win_socket_events")
                                {
                                    long unixTime, time, eventid, pid, ppid, protocol, local_port, remote_port, uptime;
                                    std::string action, eid, process_guid, process_name, family, local_address, remote_address, utc_time;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    uptime = atol(entry["decorations"]["uptime"].get<std::string>().c_str());
                                    action = column.contains("action") ? column["action"] : "";
                                    eid = column.contains("eid") ? column["eid"] : "";
                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    utc_time = column.contains("utc_time") ? column["utc_time"] : "";
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    ppid = column.contains("parent_pid") ? atol(column["parent_pid"].get<std::string>().c_str()) : 0;
                                    process_guid = column.contains("process_guid") ? column["process_guid"] : "";
                                    process_name = column.contains("process_name") ? column["process_name"] : "";
                                    family = column.contains("family") ? column["family"] : "";
                                    protocol = column.contains("protocol") ? atol(column["protocol"].get<std::string>().c_str()) : 0;
                                    local_address = column.contains("local_address") ? column["local_address"] : "";
                                    remote_address = column.contains("remote_address") ? column["remote_address"] : "";
                                    local_port = column.contains("local_port") ? atol(column["local_port"].get<std::string>().c_str()) : 0;
                                    remote_port = column.contains("remote_port") ? atol(column["remote_port"].get<std::string>().c_str()) : 0;
                                    eventid = column.contains("eventid") ? atol(column["eventid"].get<std::string>().c_str()) : 0;

                                    transaction.exec_prepared("win_socket_events", host_identifier, hostname, unixTime, action, eid, time, utc_time, pid, process_guid, process_name, family, protocol, 
                                                                                local_address, remote_address, local_port, remote_port, eventid, ppid, uptime);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "win_file_events")
                                {
                                    long unixTime, time, uptime, eventid, pid, ppid, hashed;
                                    std::string action, eid, process_guid, process_name, utc_time, target_path, md5, sha256, uid, pe_file;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    uptime = atol(entry["decorations"]["uptime"].get<std::string>().c_str());
                                    action = column.contains("action") ? column["action"] : "";
                                    eid = column.contains("eid") ? column["eid"] : "";
                                    target_path = column.contains("target_path") ? column["target_path"] : "";
                                    md5 = column.contains("md5") ? column["md5"] : "";
                                    sha256 = column.contains("sha256") ? column["sha256"] : "";
                                    hashed = column.contains("hashed") ? atol(column["hashed"].get<std::string>().c_str()) : 0;
                                    uid = column.contains("uid") ? column["uid"] : "";
                                    utc_time = column.contains("utc_time") ? column["utc_time"] : "";
                                    time = column.contains("time") ? atol(column["time"].get<std::string>().c_str()) : 0;
                                    pe_file = column.contains("pe_file") ? column["pe_file"] : "";
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    ppid = column.contains("parent_pid") ? atol(column["parent_pid"].get<std::string>().c_str()) : 0;
                                    process_guid = column.contains("process_guid") ? column["process_guid"] : "";
                                    process_name = column.contains("process_name") ? column["process_name"] : "";
                                    eventid = column.contains("eventid") ? atol(column["eventid"].get<std::string>().c_str()) : 0;

                                    transaction.exec_prepared("win_file_events", host_identifier, hostname, unixTime, action, eid, target_path, md5, sha256, hashed,
                                                              uid, utc_time, time, pe_file, pid, process_guid, process_name, eventid, ppid, uptime);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "chrome_extensions")
                                {
                                    long unixTime, uid, persistent, referenced, install_timestamp;
                                    std::string browser_type, name, profile, profile_path, referenced_identifier, identifier, version, description, default_locale, current_locale, update_url
                                    , author, path, permissions, optional_permissions, manifest_hash, from_webstore, state, install_time, ext_key;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    browser_type = column.contains("browser_type") ? column["browser_type"] : "";
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;
                                    name = column.contains("name") ? column["name"] : "";
                                    profile = column.contains("profile") ? column["profile"] : "";
                                    profile_path = column.contains("profile_path") ? column["profile_path"] : "";
                                    referenced_identifier = column.contains("referenced_identifier") ? column["referenced_identifier"] : "";
                                    identifier = column.contains("identifier") ? column["identifier"] : "";
                                    version = column.contains("version") ? column["version"] : "";
                                    description = column.contains("description") ? column["description"] : "";
                                    default_locale = column.contains("default_locale") ? column["default_locale"] : "";
                                    current_locale = column.contains("current_locale") ? column["current_locale"] : "";
                                    update_url = column.contains("update_url") ? column["update_url"] : "";
                                    author = column.contains("author") ? column["author"] : "";
                                    persistent = column.contains("persistent") ? atol(column["persistent"].get<std::string>().c_str()) : 0;
                                    path = column.contains("path") ? column["path"] : "";
                                    permissions = column.contains("permissions") ? atol(column["permissions"].get<std::string>().c_str()) : 0;
                                    optional_permissions = column.contains("optional_permissions") ? column["optional_permissions"] : "";
                                    manifest_hash = column.contains("manifest_hash") ? column["manifest_hash"] : "";
                                    referenced = column.contains("referenced") ? atol(column["referenced"].get<std::string>().c_str()) : 0;
                                    from_webstore = column.contains("from_webstore") ? column["from_webstore"] : "";
                                    state = column.contains("state") ? column["state"] : "";
                                    install_time = column.contains("install_time") ? column["install_time"] : "";
                                    install_timestamp = column.contains("install_timestamp") ? atol(column["install_timestamp"].get<std::string>().c_str()) : 0;
                                    ext_key = column.contains("key") ? column["key"] : "";


                                    transaction.exec_prepared("chrome_extensions", host_identifier, hostname, unixTime, browser_type, uid, name, profile, profile_path, referenced_identifier, identifier, version, description, default_locale, current_locale, update_url
                                                            , author, persistent, path, permissions, optional_permissions, manifest_hash, referenced, from_webstore, state, install_time, install_timestamp, ext_key);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "osquery_schedule")

                                {
                                    std::string last_executed, denylisted, output_size, wall_time, wall_time_ms, last_wall_time_ms, user_time, last_user_time, system_time, last_system_time, average_memory, last_memory ;
                                    std::string name, query, interval, executions;

                                    auto column = entry["columns"];
                                    interval = column.contains("interval") ? column["interval"] : "";
                                    executions = column.contains("executions") ? column["executions"] : "";
                                    last_executed = column.contains("last_executed") ? column["last_executed"] : "";
                                    denylisted = column.contains("denylisted") ? column["denylisted"] : "";
                                    output_size = column.contains("output_size") ? column["output_size"] : "";
                                    wall_time = column.contains("wall_time") ? column["wall_time"] : "";
                                    wall_time_ms = column.contains("wall_time_ms") ? column["wall_time_ms"] : "";
                                    last_wall_time_ms = column.contains("last_wall_time_ms") ? column["last_wall_time_ms"] : "";
                                    user_time = column.contains("user_time") ? column["user_time"] : "";
                                    last_user_time = column.contains("last_user_time") ? column["last_user_time"] : "";
                                    system_time = column.contains("system_time") ? column["system_time"] : "";
                                    last_system_time = column.contains("last_system_time") ? column["last_system_time"] : "";
                                    average_memory = column.contains("average_memory") ? column["average_memory"] : "";
                                    last_memory = column.contains("last_memory") ? column["last_memory"] : "";
                                    name = column.contains("name") ? column["name"] : "";
                                    query = column.contains("query") ? column["query"] : "";

                                    transaction.exec_prepared("osquery_schedule", host_identifier, hostname, name, query, interval, executions, last_executed, denylisted, output_size, wall_time, wall_time_ms, last_wall_time_ms, user_time, last_user_time, system_time, last_system_time, average_memory, last_memory);
                                    transaction.commit();

                                }
                                else if (entry["name"] == "etc_hosts")
                                {
                                    long unixTime;
                                    int pid_with_namespace;
                                    std::string hostnames, address;
                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    address = column.contains("address") ? column["address"] : "";
                                    hostnames = column.contains("hostnames") ? column["hostnames"] : "";
                                    pid_with_namespace = entry.contains("pid_with_namespace") ? entry["pid_with_namespace"].get<int>() : 0;
                        
                                    transaction.exec_prepared("etc_hosts", host_identifier, hostname, unixTime, address, hostnames, pid_with_namespace);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "uptime")
                                {
                                    long unixTime, total_seconds;
                                    int days, hours, minutes, seconds;
                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    days = column.contains("days") ? atol(column["days"].get<std::string>().c_str()) : 0;
                                    // days = column.contains("days") ? column["days"].get<long>() : 0;
                                    hours = column.contains("hours") ? atol(column["hours"].get<std::string>().c_str()) : 0;
                                    minutes = column.contains("minutes") ? atol(column["minutes"].get<std::string>().c_str()) : 0;
                                    seconds = column.contains("seconds") ? atol(column["seconds"].get<std::string>().c_str()) : 0;
                                    total_seconds = column.contains("total_seconds") ? atol(column["total_seconds"].get<std::string>().c_str()) : 0;
                        
                                    transaction.exec_prepared("uptime", host_identifier, hostname, unixTime, days, hours, minutes, seconds, total_seconds);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "startup_items")
                                {
                                    long unixTime;
                                    std::string name, path, args, type, source, status, username;
                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    name = column.contains("name") ? column["name"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    args = column.contains("args") ? column["args"] : "";
                                    type = column.contains("type") ? column["type"] : "";
                                    source = column.contains("source") ? column["source"] : "";
                                    status = column.contains("status") ? column["status"] : "";
                                    username = column.contains("username") ? column["username"] : "";
                                    transaction.exec_prepared("startup_items", host_identifier, hostname, unixTime, name, path, args, type, source, status, username);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "patches")
                                {
                                    long unixTime;
                                    std::string csname, hotfix_id, caption, description, fix_comments, installed_by, install_date, installed_on;
                                    auto column = entry["columns"];
                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    csname = column.contains("csname") ? column["csname"] : "";
                                    hotfix_id = column.contains("hotfix_id") ? column["hotfix_id"] : "";
                                    caption = column.contains("caption") ? column["caption"] : "";
                                    description = column.contains("description") ? column["description"] : "";
                                    fix_comments = column.contains("fix_comments") ? column["fix_comments"] : "";
                                    installed_by = column.contains("installed_by") ? column["installed_by"] : "";
                                    install_date = column.contains("install_date") ? column["install_date"] : "";
                                    installed_on = column.contains("installed_on") ? column["installed_on"] : "";
                                    transaction.exec_prepared("patches", host_identifier, hostname, unixTime, csname, hotfix_id, caption, description, fix_comments, installed_by, install_date, installed_on);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "file")
                                {
                                    long unixTime, uid, inode, gid, device, size, block_size, atime, mtime, ctime, btime, hard_links, symlink, pid_with_namespace;
                                    std::string type,attributes,volume_serial, file_id, file_version, product_version, original_filename, bsd_flags, mount_namespace_id, mode,filename,directory,path;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    path = column.contains("path") ? column["path"] : "";
                                    directory = column.contains("directory") ? column["directory"] : "";
                                    filename = column.contains("filename") ? column["filename"] : "";
                                    inode = column.contains("inode") ? atol(column["inode"].get<std::string>().c_str()) : 0;
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;
                                    gid = column.contains("gid") ? atol(column["gid"].get<std::string>().c_str()) : 0;
                                    mode = column.contains("mode") ? column["mode"] : "";
                                    device = column.contains("device") ? atol(column["device"].get<std::string>().c_str()) : 0;
                                    size = column.contains("size") ? atol(column["size"].get<std::string>().c_str()) : 0;
                                    block_size = column.contains("block_size") ? atol(column["block_size"].get<std::string>().c_str()) : 0;
                                    atime = column.contains("atime") ? atol(column["atime"].get<std::string>().c_str()) : 0;
                                    mtime = column.contains("mtime") ? atol(column["mtime"].get<std::string>().c_str()) : 0;
                                    ctime = column.contains("ctime") ? atol(column["ctime"].get<std::string>().c_str()) : 0;
                                    btime = column.contains("btime") ? atol(column["btime"].get<std::string>().c_str()) : 0;
                                    hard_links = column.contains("hard_links") ? atol(column["hard_links"].get<std::string>().c_str()) : 0;
                                    symlink = column.contains("symlink") ? atol(column["symlink"].get<std::string>().c_str()) : 0;
                                    type = column.contains("type") ? column["type"] : "";
                                    attributes = column.contains("attributes") ? column["attributes"] : "";
                                    volume_serial = column.contains("volume_serial") ? column["volume_serial"] : "";
                                    file_id = column.contains("file_id") ? column["file_id"] : "";
                                    file_version = column.contains("file_version") ? column["file_version"] : "";
                                    product_version = column.contains("product_version") ? column["product_version"] : "";
                                    original_filename = column.contains("original_filename") ? column["original_filename"] : "";
                                    bsd_flags = column.contains("bsd_flags") ? column["bsd_flags"] : "";
                                    pid_with_namespace = column.contains("pid_with_namespace") ? atol(column["pid_with_namespace"].get<std::string>().c_str()) : 0;
                                    mount_namespace_id = column.contains("mount_namespace_id") ? column["mount_namespace_id"] : "";


                                    transaction.exec_prepared("file", host_identifier, hostname, unixTime, path, directory, filename, inode, uid, gid, mode, device, size, block_size, atime, mtime
                                                            , ctime, btime, hard_links, symlink, type, attributes, volume_serial, file_id, file_version, product_version, original_filename, bsd_flags, pid_with_namespace, mount_namespace_id);
                                    transaction.commit();  
                                }
                                else if (entry["name"] == "wmi_cli_event_consumers")
                                {
                                    std::string name, command_line_template, executable_path, class_name, relative_path;
                                    long unixTime;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    name = column.contains("name") ? column["name"] : "";
                                    command_line_template = column.contains("command_line_template") ? column["command_line_template"] : "";
                                    executable_path = column.contains("executable_path") ? column["executable_path"] : "";
                                    class_name = column.contains("class") ? column["class"] : "";
                                    relative_path = column.contains("relative_path") ? column["relative_path"] : "";
                                    
                                    transaction.exec_prepared("wmi_cli_event_consumers", host_identifier, hostname, unixTime);
                                    transaction.commit();  
                                }
                                else if (entry["name"] == "processes32")
                                {
                                    int on_disk, threads, nice, virtual_process;
                                    long unixTime, pid, uid, gid,egid, suid, sgid, wired_size, resident_size, total_size, user_time, system_time, disk_bytes_read, disk_bytes_written,
                                    start_time, parent, pgroup, elevated_token, secure_process, elapsed_time, handle_count, percent_processor_time;
                                    std::string name, cmdline, state, cwd, root, protection_type, path;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    name = column.contains("name") ? column["name"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    cmdline = column.contains("cmdline") ? column["cmdline"] : "";
                                    state = column.contains("state") ? column["state"] : "";
                                    cwd = column.contains("cwd") ? column["cwd"] : "";
                                    root = column.contains("root") ? column["root"] : "";
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;
                                    gid = column.contains("gid") ? atol(column["gid"].get<std::string>().c_str()) : 0;
                                    egid = column.contains("egid") ? atol(column["egid"].get<std::string>().c_str()) : 0;
                                    suid = column.contains("suid") ? atol(column["suid"].get<std::string>().c_str()) : 0;
                                    sgid = column.contains("sgid") ? atol(column["sgid"].get<std::string>().c_str()) : 0;
                                    on_disk = column.contains("on_disk") ? atol(column["on_disk"].get<std::string>().c_str()) : 0;
                                    wired_size = column.contains("wired_size") ? atol(column["wired_size"].get<std::string>().c_str()) : 0;
                                    resident_size = column.contains("resident_size") ? atol(column["resident_size"].get<std::string>().c_str()) : 0;
                                    total_size = column.contains("total_size") ? atol(column["total_size"].get<std::string>().c_str()) : 0;
                                    user_time = column.contains("user_time") ? atol(column["user_time"].get<std::string>().c_str()) : 0;
                                    system_time = column.contains("system_time") ? atol(column["system_time"].get<std::string>().c_str()) : 0;
                                    disk_bytes_read = column.contains("disk_bytes_read") ? atol(column["disk_bytes_read"].get<std::string>().c_str()) : 0;
                                    disk_bytes_written = column.contains("disk_bytes_written") ? atol(column["disk_bytes_written"].get<std::string>().c_str()) : 0;
                                    start_time = column.contains("start_time") ? atol(column["start_time"].get<std::string>().c_str()) : 0;
                                    parent = column.contains("parent") ? atol(column["parent"].get<std::string>().c_str()) : 0;
                                    pgroup = column.contains("pgroup") ? atol(column["pgroup"].get<std::string>().c_str()) : 0;
                                    threads = column.contains("threads") ? atol(column["threads"].get<std::string>().c_str()) : 0;
                                    nice = column.contains("nice") ? atol(column["nice"].get<std::string>().c_str()) : 0;
                                    elevated_token = column.contains("elevated_token") ? atol(column["elevated_token"].get<std::string>().c_str()) : 0;
                                    secure_process = column.contains("secure_process") ? atol(column["secure_process"].get<std::string>().c_str()) : 0;
                                    protection_type = column.contains("protection_type") ? column["protection_type"] : "";
                                    virtual_process = column.contains("virtual_process") ? atol(column["virtual_process"].get<std::string>().c_str()) : 0;
                                    elapsed_time = column.contains("elapsed_time") ? atol(column["elapsed_time"].get<std::string>().c_str()) : 0;
                                    handle_count = column.contains("handle_count") ? atol(column["handle_count"].get<std::string>().c_str()) : 0;
                                    percent_processor_time = column.contains("percent_processor_time") ? atol(column["percent_processor_time"].get<std::string>().c_str()) : 0;

                                    transaction.exec_prepared("processes32", host_identifier, hostname, unixTime, pid, name, path, cmdline, state, cwd, root, uid, gid, egid, suid, sgid, on_disk, wired_size,
                                    resident_size, total_size, user_time, system_time, disk_bytes_read, disk_bytes_written, start_time, parent, pgroup, threads, nice, elevated_token, 
                                    secure_process, protection_type, virtual_process, elapsed_time, handle_count, percent_processor_time);
                                    transaction.commit();  
                                }
                                else if (entry["name"] == "programs")
                                {
                                    std::string name, version, install_location, install_source, language, publisher, uninstall_string, install_date, identifying_number;
                                    long unixTime;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    name = column.contains("query_name") ? column["query_name"] : "";
                                    version = column.contains("version") ? column["version"] : "";
                                    install_location = column.contains("install_location") ? column["install_location"] : "";
                                    install_source = column.contains("install_source") ? column["install_source"] : "";
                                    language = column.contains("language") ? column["language"] : "";
                                    publisher = column.contains("publisher") ? column["publisher"] : "";
                                    uninstall_string = column.contains("uninstall_string") ? column["uninstall_string"] : "";
                                    install_date = column.contains("install_date") ? column["install_date"] : "";
                                    identifying_number = column.contains("identifying_number") ? column["identifying_number"] : "";

                                    
                                    transaction.exec_prepared("programs", host_identifier, hostname, unixTime, name, version, install_location, install_source, language, publisher, uninstall_string, install_date, identifying_number);
                                    transaction.commit();  
                                }
                                else if (entry["name"] == "kernel_info")
                                {
                                    long unixTime;
                                    std::string version, arguments, path, device;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;

                                    version = column.contains("version") ? column["version"] : "";
                                    arguments = column.contains("arguments") ? column["arguments"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    device = column.contains("device") ? column["device"] : "";

                                    connection->prepare("update_kernel_to_nodes", "update nodes set kernel_version = $1 where host_identifier = $2");
                                    pqxx::work transaction{*connection};
                                    pqxx::result result;
                                    result = transaction.exec_prepared("update_kernel_to_nodes",version, host_identifier);
                                    connection->unprepare("update_kernel_to_nodes");


                                    transaction.exec_prepared("kernel_info", host_identifier, hostname, unixTime, version, arguments, path, device);
                                    transaction.commit();
                                }else if (entry["name"] == "deb_packages")
                                {
                                    long unixTime, size;
                                    int pid_with_namespace;
                                    std::string name, version, source, arch, revision, status, maintainer, section, priority, admindir, mount_namespace_id;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;

                                    name = column.contains("name") ? column["name"] : "";
                                    version = column.contains("version") ? column["version"] : "";
                                    source = column.contains("source") ? column["source"] : "";
                                    size = column.contains("size") ? atol(column["size"].get<std::string>().c_str()) : 0;
                                    arch = column.contains("arch") ? column["arch"] : "";
                                    revision = column.contains("revision") ? column["revision"] : "";
                                    status = column.contains("status") ? column["status"] : "";
                                    maintainer = column.contains("maintainer") ? column["maintainer"] : "";
                                    section = column.contains("section") ? column["section"] : "";
                                    priority = column.contains("priority") ? column["priority"] : "";
                                    admindir = column.contains("admindir") ? column["admindir"] : "";
                                    pid_with_namespace = column.contains("pid_with_namespace") ? atoi(column["pid_with_namespace"].get<std::string>().c_str()) : 0;
                                    mount_namespace_id = column.contains("mount_namespace_id") ? column["mount_namespace_id"] : "";

                                    transaction.exec_prepared("deb_packages", host_identifier, hostname, unixTime, name, version, source, size, arch, revision, status, maintainer, section, priority, admindir, pid_with_namespace, mount_namespace_id);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "rpm_packages")
                                {
                                    long unixTime, size;
                                    int pid_with_namespace, epoch, install_time;
                                    std::string name, version, release, source, sha1, arch, vendor, package_group, mount_namespace_id;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;

                                    name = column.contains("name") ? column["name"] : "";
                                    version = column.contains("version") ? column["version"] : "";
                                    release = column.contains("release") ? column["release"] : "";
                                    source = column.contains("source") ? column["source"] : "";
                                    size = column.contains("size") ? atol(column["size"].get<std::string>().c_str()) : 0;
                                    sha1 = column.contains("sha1") ? column["sha1"] : "";
                                    arch = column.contains("arch") ? column["arch"] : "";
                                    epoch = column.contains("epoch") ? atoi(column["epoch"].get<std::string>().c_str()) : 0;
                                    install_time = column.contains("install_time") ? atoi(column["install_time"].get<std::string>().c_str()) : 0;
                                    vendor = column.contains("vendor") ? column["vendor"] : "";
                                    package_group = column.contains("package_group") ? column["package_group"] : "";
                                    pid_with_namespace = column.contains("pid_with_namespace") ? atoi(column["pid_with_namespace"].get<std::string>().c_str()) : 0;
                                    mount_namespace_id = column.contains("mount_namespace_id") ? column["mount_namespace_id"] : "";

                                    transaction.exec_prepared("rpm_packages", host_identifier, hostname, unixTime, name, version, release, source, size, sha1, arch, epoch, install_time, vendor, package_group, pid_with_namespace, mount_namespace_id);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "chocolatey_packages")
                                {
                                    long unixTime, size;
                                    int pid_with_namespace, epoch, install_time;
                                    std::string name, version, summary, author, license, path;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;

                                    name = column.contains("name") ? column["name"] : "";
                                    version = column.contains("version") ? column["version"] : "";
                                    summary = column.contains("summary") ? column["summary"] : "";
                                    author = column.contains("author") ? column["author"] : "";
                                    license = column.contains("license") ? column["license"] : "";
                                    path = column.contains("path") ? column["path"] : "";

                                    transaction.exec_prepared("chocolatey_packages", host_identifier, hostname, unixTime, name, version, summary, author, license, path);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "interface_details")
                                {
                                    long unixTime, size;
                                    int type, mtu, metric, flags, enabled, physical_adapter, speed, dhcp_enabled;
                                    std::string interface, mac, pci_slot, friendly_name	, description, manufacturer, connection_id, connection_status, service, dhcp_lease_expires, dhcp_lease_obtained, dhcp_server, dns_domain, dns_domain_suffix_search_order, dns_host_name, dns_server_search_order;
                                    long ipackets, opackets, ibytes, obytes, ierrors, oerrors, idrops, odrops, collisions, last_change, link_speed;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    interface = column.contains("interface") ? column["interface"] : "";
                                    mac = column.contains("mac") ? column["mac"] : "";
                                    type = column.contains("type") ? atoi(column["type"].get<std::string>().c_str()) : 0;
                                    mtu = column.contains("mtu") ? atoi(column["mtu"].get<std::string>().c_str()) : 0;
                                    metric = column.contains("metric") ? atoi(column["metric"].get<std::string>().c_str()) : 0;
                                    flags = column.contains("flags") ? atoi(column["flags"].get<std::string>().c_str()) : 0;
                                    ipackets = column.contains("ipackets") ? atol(column["ipackets"].get<std::string>().c_str()) : 0;
                                    opackets = column.contains("opackets") ? atol(column["opackets"].get<std::string>().c_str()) : 0;
                                    ibytes = column.contains("ibytes") ? atol(column["ibytes"].get<std::string>().c_str()) : 0;
                                    obytes = column.contains("obytes") ? atol(column["obytes"].get<std::string>().c_str()) : 0;
                                    ierrors = column.contains("ierrors") ? atol(column["ierrors"].get<std::string>().c_str()) : 0;
                                    oerrors = column.contains("oerrors") ? atol(column["oerrors"].get<std::string>().c_str()) : 0;
                                    idrops = column.contains("idrops") ? atol(column["idrops"].get<std::string>().c_str()) : 0;
                                    odrops = column.contains("odrops") ? atol(column["odrops"].get<std::string>().c_str()) : 0;
                                    collisions = column.contains("collisions") ? atol(column["collisions"].get<std::string>().c_str()) : 0;
                                    last_change = column.contains("last_change") ? atol(column["last_change"].get<std::string>().c_str()) : 0;
                                    link_speed = column.contains("link_speed") ? atol(column["link_speed"].get<std::string>().c_str()) : 0;
                                    pci_slot = column.contains("pci_slot") ? column["pci_slot"] : "";
                                    friendly_name = column.contains("friendly_name") ? column["friendly_name"] : "";
                                    description = column.contains("description") ? column["description"] : "";
                                    manufacturer = column.contains("manufacturer") ? column["manufacturer"] : "";
                                    connection_id = column.contains("connection_id") ? column["connection_id"] : "";
                                    connection_status = column.contains("connection_status") ? column["connection_status"] : "";
                                    enabled = column.contains("enabled") ? atoi(column["enabled"].get<std::string>().c_str()) : 0;
                                    physical_adapter = column.contains("physical_adapter") ? atoi(column["physical_adapter"].get<std::string>().c_str()) : 0;
                                    speed = column.contains("speed") ? atoi(column["speed"].get<std::string>().c_str()) : 0;
                                    service = column.contains("service") ? column["service"] : "";
                                    dhcp_enabled = column.contains("dhcp_enabled") ? atoi(column["dhcp_enabled"].get<std::string>().c_str()) : 0;
                                    dhcp_lease_expires = column.contains("dhcp_lease_expires") ? column["dhcp_lease_expires"] : "";
                                    dhcp_lease_obtained = column.contains("dhcp_lease_obtained") ? column["dhcp_lease_obtained"] : "";
                                    dhcp_server = column.contains("dhcp_server") ? column["dhcp_server"] : "";
                                    dns_domain = column.contains("dns_domain") ? column["dns_domain"] : "";
                                    dns_domain_suffix_search_order = column.contains("dns_domain_suffix_search_order") ? column["dns_domain_suffix_search_order"] : "";
                                    dns_host_name = column.contains("dns_host_name") ? column["dns_host_name"] : "";
                                    dns_server_search_order = column.contains("dns_server_search_order") ? column["dns_server_search_order"] : "";

                                    transaction.exec_prepared("interface_details", host_identifier, hostname, unixTime, interface, mac, type, mtu, metric, flags, ipackets, opackets, ibytes, obytes, ierrors, oerrors, idrops, odrops, collisions, last_change, link_speed, pci_slot, friendly_name	, description, manufacturer, connection_id, connection_status, enabled, physical_adapter, speed, service, dhcp_enabled, dhcp_lease_expires, dhcp_lease_obtained, dhcp_server, dns_domain, dns_domain_suffix_search_order, dns_host_name, dns_server_search_order);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "ie_extensions")
                                {
                                    long unixTime;
                                    std::string name, registry_path, version, path;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;

                                    name = column.contains("name") ? column["name"] : "";
                                    registry_path = column.contains("registry_path") ? column["registry_path"] : "";
                                    version = column.contains("version") ? column["version"] : "";
                                    path = column.contains("path") ? column["path"] : "";

                                    transaction.exec_prepared("ie_extensions", host_identifier, hostname, unixTime, name, registry_path, version, path);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "firefox_addons")
                                {
                                    long unixTime, uid;
                                    std::string name, identifier, creator, type, version, description, source_url, location, path;
                                    int visible, active, disabled, autoupdate;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    uid = column.contains("uid") ? atol(column["uid"].get<std::string>().c_str()) : 0;
                                    name = column.contains("name") ? column["name"] : "";
                                    identifier = column.contains("identifier") ? column["identifier"] : "";
                                    creator  = column.contains("creator") ? column["creator"] : "";
                                    type  = column.contains("type") ? column["type"] : "";
                                    version = column.contains("version") ? column["type"] : "";
                                    description = column.contains("description") ? column["description"] : "";
                                    source_url = column.contains("source_url") ? column["source_url"] : "";
                                    visible = column.contains("visible") ? atoi(column["visible"].get<std::string>().c_str()) : 0;
                                    active = column.contains("active") ? atoi(column["active"].get<std::string>().c_str()) : 0;
                                    disabled = column.contains("disabled") ? atoi(column["disabled"].get<std::string>().c_str()) : 0;
                                    autoupdate = column.contains("autoupdate") ? atoi(column["autoupdate"].get<std::string>().c_str()) : 0;
                                    location = column.contains("location") ? column["location"] : "";
                                    path = column.contains("path") ? column["path"] : "";

                                    transaction.exec_prepared("firefox_addons", host_identifier, hostname, unixTime, uid, name, identifier, creator, type, version, description, source_url, visible, active, disabled, autoupdate, location, path);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "npm_packages")
                                {
                                    long unixTime;
                                    std::string name, version, description, author, license, homepage, path, directory, mount_namespace_id;
                                    int pid_with_namespace;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    
                                    name = column.contains("name") ? column["name"] : "";
                                    version = column.contains("version") ? column["type"] : "";
                                    author = column.contains("author") ? column["author"] : "";
                                    license = column.contains("license") ? column["license"] : "";
                                    homepage = column.contains("homepage") ? column["homepage"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    directory = column.contains("directory") ? column["directory"] : "";
                                    pid_with_namespace = column.contains("pid_with_namespace") ? atoi(column["pid_with_namespace"].get<std::string>().c_str()) : 0;
                                    mount_namespace_id = column.contains("mount_namespace_id") ? column["mount_namespace_id"] : "";


                                    transaction.exec_prepared("npm_packages", host_identifier, hostname, unixTime, name, version, description, author, license, homepage, path, directory, pid_with_namespace, mount_namespace_id);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "portage_packages")
                                {
                                    long unixTime, build_time, eapi, size;
                                    std::string package, version, slot, repository;
                                    int world;

                                    auto column = entry["columns"];

                                    unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                    
                                    package = column.contains("package") ? column["package"] : "";
                                    version = column.contains("version") ? column["version"] : "";
                                    slot = column.contains("slot") ? column["slot"] : "";
                                    build_time = column.contains("build_time") ? atol(column["build_time"].get<std::string>().c_str()) : 0;
                                    repository = column.contains("repository") ? column["repository"] : "";
                                    eapi = column.contains("eapi") ? atol(column["eapi"].get<std::string>().c_str()) : 0;
                                    size = column.contains("size") ? atol(column["size"].get<std::string>().c_str()) : 0;
                                    world = column.contains("world") ? atoi(column["world"].get<std::string>().c_str()) : 0;

                                    transaction.exec_prepared("portage_packages", host_identifier, hostname, unixTime, package, version, slot, build_time, repository, eapi, size, world);
                                    transaction.commit();
                                }
    
                            }
                            else if (entry.contains("hostIdentifier") && entry.contains("name") && entry.contains("snapshot"))
                            {
                                if (host_identifier != entry["hostIdentifier"])
                                {
                                    continue;
                                }

                                long unixTime = entry.contains("unixTime") ? entry["unixTime"].get<long>() : 0;
                                std::string query_name = entry["name"];
                                std::string query_result = entry["snapshot"].dump();

                                transaction.exec_prepared("snapshot_result_add", host_identifier, unixTime, query_name, query_result);
                                transaction.commit();
                            }
                            
                        }
                    }
                }
            }
            catch (pqxx::sql_error const &e)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
            }
            catch (std::exception const &e)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
            }
            catch (...)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Unknown error";
            }
        }
    }
    catch (const nlohmann::json::parse_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
    }
    catch (const nlohmann::json::type_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
    }
    catch (std::exception const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Unknown error";
    }

    std::string reply = log_response.dump();
    response.status = http::status::ok;
    response.body = reply;
    response.header["Content-Type"] = "application/json";
    return response;
}

HTTPMessage distributed_read(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;

    nlohmann::json log_response;

    try
    {
        auto logger_data = nlohmann::json::parse(request.body);
        if (logger_data.contains("node_key"))
        {
            std::string key = logger_data["node_key"];
            std::string host_identifier;
            bool isKeyValid;

            try
            {
                pqxx::work transaction{*connection};

                pqxx::result result = transaction.exec_prepared("validate_node_key", key);
                if (result.size() == 1)
                {
                    isKeyValid = true;
                    host_identifier = result.at(0)["host_identifier"].as<std::string>();
                    log_response["node_invalid"] = false;
                    result = transaction.exec_prepared("get_scheduled_query", host_identifier);
                    connection->prepare("update_last_seen", "update nodes set last_seen = $1 where host_identifier = $2;");
                    int last_seen;
                    std::time_t t = std::time(0);
                    last_seen = t;
                    pqxx::result queries = transaction.exec_prepared("update_last_seen", last_seen, host_identifier);
                    transaction.commit();
                    connection->unprepare("update_last_seen");

                    for (unsigned int i = 0; i < result.size(); ++i)
                    {
                        unsigned long long query_key = result.at(i)["id"].as<unsigned long long>();
                        std::string query = result.at(i)["query"].as<std::string>();
                        log_response["queries"][std::to_string(query_key)] = query;

                        long unixTime = std::time(nullptr);

                        transaction.exec_prepared("stop_scheduled_query", query_key, unixTime);
                        transaction.commit();
                    }
                }
                else
                {
                    isKeyValid = false;
                    log_response["node_invalid"] = true;
                }
            }
            catch (pqxx::sql_error const &e)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
            }
            catch (std::exception const &e)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
            }
            catch (...)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Unknown error";
            }
        }
    }
    catch (const nlohmann::json::parse_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
    }
    catch (const nlohmann::json::type_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
    }
    catch (std::exception const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Unknown error";
    }

    std::string reply = log_response.dump();
    response.status = http::status::ok;
    response.body = reply;
    response.header["Content-Type"] = "application/json";
    return response;
}

HTTPMessage distributed_write(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;

    nlohmann::json log_response;

    try
    {
        auto logger_data = nlohmann::json::parse(request.body);
        if (logger_data.contains("node_key"))
        {
            std::string key = logger_data["node_key"];
            std::string host_identifier;
            bool isKeyValid;

            try
            {
                pqxx::work transaction{*connection};

                pqxx::result result = transaction.exec_prepared("validate_node_key", key);
                if (result.size() == 1)
                {
                    isKeyValid = true;
                    host_identifier = result.at(0)["host_identifier"].as<std::string>();
                    log_response["node_invalid"] = false;
                    connection->prepare("update_last_seen", "update nodes set last_seen = $1 where host_identifier = $2;");
                    int last_seen;
                    std::time_t t = std::time(0);
                    last_seen = t;
                    pqxx::result queries = transaction.exec_prepared("update_last_seen", last_seen, host_identifier);
                    transaction.commit();
                    connection->unprepare("update_last_seen");
                }
                else
                {
                    isKeyValid = false;
                    log_response["node_invalid"] = true;
                }

                if (isKeyValid && logger_data.contains("queries"))
                {
                    auto entries = logger_data["queries"];
                    if (!entries.is_array())
                    {
                        for (auto entry : entries.items())
                        {
                            std::string query_name = entry.key();
                            std::string query_result = entry.value().dump();

                            long unixTime = std::time(nullptr);

                            transaction.exec_prepared("query_result_add", host_identifier, unixTime, query_name, query_result);
                            transaction.commit();
                        }
                    }
                }
            }
            catch (pqxx::sql_error const &e)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
            }
            catch (std::exception const &e)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
            }
            catch (...)
            {
                BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Unknown error";
            }
        }
    }
    catch (const nlohmann::json::parse_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
    }
    catch (const nlohmann::json::type_error &error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
    }
    catch (std::exception const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: " << e.what();
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Unknown error";
    }

    std::string reply = log_response.dump();
    response.status = http::status::ok;
    response.body = reply;
    response.header["Content-Type"] = "application/json";
    return response;
}