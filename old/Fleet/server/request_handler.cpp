#include "../3rdparty/json.hpp"
#include "request_handler.h"
#include "../common/logger.h"

#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <ctime>
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
        // std::cout<<request.body<<std::endl;
        // BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Enrollment request is" << request.body;
        if (enrollment_request.contains("enroll_secret") && enrollment_request.contains("host_identifier") && enrollment_request.contains("host_details"))
        {
            std::string secret = enrollment_request["enroll_secret"];
            std::string host_identifier = enrollment_request["host_identifier"];
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
                        connection->prepare("host_add", "insert into nodes (host_identifier, os_arch, os_build, os_major, os_minor, os_name, os_platform, osquery_version, node_key, node_invalid, reenroll_allowed, config_id, hardware_vendor, hardware_model, hardware_version, hostname, hardware_cpu_logical_core, hardware_cpu_type, hardware_physical_memory, enrolled_on, last_seen) values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21)");
                        transaction.exec_prepared("host_add", host_identifier, os_arch, os_build, os_major, os_minor, os_name, os_platform, osquery_version, node_key, node_invalid, reenroll_allowed, config_id, hardware_vendor, hardware_model, hardware_version, hostname, hardware_cpu_logical_core, hardware_cpu_type, hardware_physical_memory, enrolled_on, last_seen);
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
                    osquery_configuration["node_invalid"] = false;
                    int last_seen;
                    std::time_t t = std::time(0);
                    last_seen = t;
                    pqxx::result queries = transaction.exec_prepared("update_last_seen", last_seen, host_identifier);
                }
                else
                {
                    isKeyValid = false;
                    osquery_configuration["node_invalid"] = true;
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
                    std::string node_query;

                    if (!queries.empty())
                    {
                        if (!queries.at(0)["query"].is_null())
                        {
                            node_query = queries.at(0)["query"].as<std::string>();
                        }
                        query = nlohmann::json::parse(node_query);
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
                            if (query["schedule"].contains("docker_details"))
                            {
                                scheduled_queries["docker_details"]["query"] = query["schedule"]["docker_details"]["query"];
                                scheduled_queries["docker_details"]["interval"] = query["schedule"]["docker_details"]["interval"];
                                scheduled_queries["docker_details"]["snapshot"] = query["schedule"]["docker_details"]["snapshot"];
                            }
                            if (query["schedule"].contains("docker_network"))
                            {
                                scheduled_queries["docker_network"]["query"] = query["schedule"]["docker_network"]["query"];
                                scheduled_queries["docker_network"]["interval"] = query["schedule"]["docker_network"]["interval"];
                                scheduled_queries["docker_network"]["snapshot"] = query["schedule"]["docker_network"]["snapshot"];
                            }
                            osquery_configuration["schedule"] = scheduled_queries;
                            if (query.contains("file_paths"))
                            {
                                osquery_configuration["file_paths"] = query["file_paths"];
                            }

                            // std::cout << query.dump() << std::endl;
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
                    if (entries.is_array())
                    {
                        for (auto entry : entries)
                        {
                            // std::cout << entry << std::endl;
                            if (entry.contains("hostIdentifier") && entry.contains("name") && entry.contains("columns"))
                            {
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

                                    transaction.exec_prepared("socket_event_add", host_identifier, unixTime, time, uptime, action, auid, family, fd,
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
                                    interface = column.contains("gateway") ? column["gateway"] : "";
                                    type = column.contains("source") ? column["source"] : "";

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
                                    //uptime = column.contains("uptime") ? atol(column["uptime"].get<std::string>().c_str()) : 0;
                                    long _ntime = stol(ntime);
                                    uptime = _ntime/1000000000;
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
                                else if (entry["name"] == "docker_details")
                                {

                                    std::string created, path, args, image, resolveconfpath, hostnamepath, hostspath, logpath, name, driver, platform, mountlabel, 
                                    process_label, app_armor_profile, execid;
                                    long restartcount, pid, sizerw, sizerootfs;

                                    auto column = entry["columns"];
                                    created = column.contains("created") ? column["created"] : "";
                                    path = column.contains("path") ? column["path"] : "";
                                    args = column.contains("args") ? column["args"] : "";
                                    image = column.contains("image") ? column["image"] : "";
                                    resolveconfpath = column.contains("resolveconfpath") ? column["resolveconfpath"] : "";
                                    hostnamepath = column.contains("hostnamepath") ? column["hostnamepath"] : "";
                                    hostspath = column.contains("hostspath") ? column["hostspath"] : "";
                                    logpath = column.contains("logpath") ? column["logpath"] : "";
                                    name = column.contains("name") ? column["name"] : "";
                                    driver = column.contains("driver") ? column["driver"] : "";
                                    platform = column.contains("platform") ? column["platform"] : "";
                                    mountlabel = column.contains("mountlabel") ? column["mountlabel"] : "";
                                    process_label = column.contains("process_label") ? column["process_label"] : "";
                                    app_armor_profile = column.contains("app_armor_profile") ? column["app_armor_profile"] : "";
                                    execid = column.contains("execid") ? column["execid"] : "";

                                    restartcount = column.contains("restartcount") ? atol(column["restartcount"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;
                                    sizerw = column.contains("sizerw") ? atol(column["sizerw"].get<std::string>().c_str()) : 0;
                                    sizerootfs = column.contains("sizerootfs") ? atol(column["sizerootfs"].get<std::string>().c_str()) : 0;
                                    
                                    transaction.exec_prepared("docker_details", host_identifier, created, path, args, image, resolveconfpath, hostnamepath, hostspath, logpath, name, restartcount, pid, driver, platform, mountlabel, process_label, app_armor_profile, execid, sizerw, sizerootfs);
                                    transaction.commit();
                                }
                                else if (entry["name"] == "network_details")
                                {

                                    std::string bridge, sandboxid, hairpinmode, linklocalipv6address, ports, sandboxkey, secondaryipaddresses, secondaryipv6addresses;
                                    long linklocalipv6prefixlen, pid;
                                    auto column = entry["columns"];
                                    bridge = column.contains("bridge") ? column["bridge"] : "";
                                    sandboxid = column.contains("sandboxid") ? column["sandboxid"] : "";
                                    hairpinmode = column.contains("hairpinmode") ? column["hairpinmode"] : "";
                                    linklocalipv6address = column.contains("linklocalipv6address") ? column["linklocalipv6address"] : "";
                                    ports = column.contains("ports") ? column["ports"] : "";
                                    sandboxkey = column.contains("sandboxkey") ? column["sandboxkey"] : "";
                                    secondaryipaddresses = column.contains("secondaryipaddresses") ? column["secondaryipaddresses"] : "";
                                    secondaryipv6addresses = column.contains("secondaryipv6addresses") ? column["secondaryipv6addresses"] : "";
                                    linklocalipv6prefixlen = column.contains("linklocalipv6prefixlen") ? atol(column["linklocalipv6prefixlen"].get<std::string>().c_str()) : 0;
                                    pid = column.contains("pid") ? atol(column["pid"].get<std::string>().c_str()) : 0;

                                    
                                    transaction.exec_prepared("docker_events",host_identifier, bridge, sandboxid, hairpinmode, linklocalipv6address, linklocalipv6prefixlen, ports, sandboxkey, secondaryipaddresses, secondaryipv6addresses, pid);
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