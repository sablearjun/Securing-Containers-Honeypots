#include "../../3rdparty/json.hpp"
#include "settings.h"
#include "../../common/logger.h"
#include "../../common/EdrDataTypes.h"
#include "../../common/ConfigurationParser.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>   
std::filesystem::path cwd = std::filesystem::current_path() / "data_retention.sh";
std::string path = cwd.string();

bool cron_job_exists() {
    // Define the command to check if the cron job exists
    std::string command = "crontab -l | grep '"+path+"'";

    // Open a pipe to execute the command and read its output
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error executing command" << std::endl;
        return false;
    }

    // Read the output of the command
    char buffer[128];
    std::string output = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }

    // Close the pipe
    pclose(pipe);

    // Check if the output contains the cron job
    return (output.find(path) != std::string::npos);
}

HTTPMessage manual_purge(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json parameters;
    std::string days;
    try
    {
        parameters = request.query;
        if(parameters.contains("days"))
        {
            days = parameters["days"];
        }
        if (!days.empty())
        {   
            int affect = 0;
            pqxx::result result;
            std::time_t t = std::time(0);
            int now = t;
            int seconds = stol(days) * 86400;
            seconds = now - seconds;
            std::array <std::string, 23> tables_lists = { "process_events", "socket_events", "user_events", "query_result", "snapshot_result", "process_open_pipes", "process_open_files", "logged_in_users", "suid_bin", "process_open_sockets", "cpu_time", "memory_info", "interface_addresses", "chrome_extensions", "file", "wmi_cli_event_consumers", "bpf_process_events", "bpf_socket_events", "bpf_file_events", "win_process_events", "win_file_events", "win_socket_events", "events" };
            int size = tables_lists.size();
            for(int i = 0; i < size; i++){
                std::string query = "delete from "+tables_lists[i]+ " where unixtime < "+std::to_string(seconds);
                connection->prepare("manual_purge_x_days", query);
                pqxx::work transaction{*connection};
                result = transaction.exec_prepared("manual_purge_x_days");
                if(result.affected_rows()>0){
                    affect = result.affected_rows();
                }
                connection->unprepare("manual_purge_x_days");
                transaction.commit();
            }

            if(affect<=0)
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
                response.body = "{\n"
                                "  \"status\":\"Success\",\n"
                                "  \"message\":\"Data purged successfully\"\n"
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
                        "  \"message\":\"Query to purge data failed.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not purge";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to purge data\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    } 
}

HTTPMessage data_retention(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json parameters;
    std::string days;
    std::string script = "";
    std::ofstream outfile(path);
    try
    {
        parameters = request.query;
        if(parameters.contains("days"))
        {
            days = parameters["days"];
        }
        if (!days.empty())
        {

                connection->prepare("data_retention_days", "update data_retention_settings set days = $1");
                pqxx::work transaction{*connection};
                pqxx::result result;
                result = transaction.exec_prepared("data_retention_days", std::to_string(stol(days)*86400));
                connection->unprepare("data_retention_days");
                transaction.commit();
                nlohmann::json configs;
                std::ifstream ifs("config.json");
                configs = nlohmann::json::parse(ifs);
                
                std::string username = configs["fleet_server"]["db_configuration"]["username"];
                std::string password = configs["fleet_server"]["db_configuration"]["password"];


                std::time_t t = std::time(0);
            int now = t;
            int seconds = stol(days) * 86400;
            seconds = now - seconds;
            std::array <std::string, 23> tables_lists = { "process_events", "socket_events", "user_events", "query_result", "snapshot_result", "process_open_pipes", "process_open_files", "logged_in_users", "suid_bin", "process_open_sockets", "cpu_time", "memory_info", "interface_addresses", "chrome_extensions", "file", "wmi_cli_event_consumers", "bpf_process_events", "bpf_socket_events", "bpf_file_events", "win_process_events", "win_file_events", "win_socket_events", "events" };
            int size = tables_lists.size();
            for(int i = 0; i < size; i++){
                script = script + "PGPASSWORD="+password+" psql -U "+username+" -d fleet -c \"delete from "+tables_lists[i]+ " where unixtime < '$timestamp';\"\n";
            }
            std::string bin_bash = "#!/bin/bash \n";
            std::string time = "ts=$(date +%s) \n";
            // std::string cmd1 = "days=$(sudo -u postgres bash -c \"psql -d fleet -t -c 'select days from data_retention_settings;'\")\n";
            std::string cmd1 = "days=$(PGPASSWORD="+password+" psql -U "+username+" -d fleet -t -c 'select days from data_retention_settings;' | tr -d ' ' | sed 's/^0*//')\n";
            std::string time2 = "timestamp=$((ts-days)) \n";
            std::string chmod = "chmod +x "+path;
            system(chmod.c_str());
            outfile << bin_bash;
            outfile << time;
            outfile << cmd1;
            outfile << time2;
            // outfile << "echo $timestamp >> /home/sablearjun/Desktop/output.txt\n";
            outfile << script;

            outfile.close();


                if (!cron_job_exists()) {
                    // Define the command to add the cron job

                    std::string command = "echo '0 0 * * * "+path+" | crontab -";

                    // Execute the command using system()
                    int status = system(command.c_str());

                    // Check if the command executed successfully
                    if (status != 0) {
                        std::cerr << "Error adding cron job" << std::endl;
                                BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Error adding cron job";
                                response.status = boost::beast::http::status::bad_request;
                                response.body = "{\n"
                                                "  \"status\":\"Fail\",\n"
                                                "  \"message\":\"Error adding cron job\"\n"
                                                "}";
                                response.header["Content-Type"] = "application/json";
                                return response;
                    }

                    // Success
                    std::cout << "Cron job added successfully" << std::endl;
                } else {
                    // The cron job already exists
                    std::cout << "Cron job already exists" << std::endl;
                }


        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to purge data failed.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not purge";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to purge data\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    } 

        response.status = boost::beast::http::status::ok;
        response.body = "{\n"
                        "  \"status\":\"Success\",\n"
                        "  \"message\":\"Successfully added cron job\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
}

HTTPMessage data_retention_days(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json rule_list_json, rule_list;
    long days;
    try
    {
            pqxx::result result;
            pqxx::work transaction{*connection};
            connection->prepare("get_retenion_days", "select days from data_retention_settings");
            result = transaction.exec_prepared("get_retenion_days");
            connection->unprepare("get_retenion_days");

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
            days = result.at(0)["days"].as<long>();
            days = days/86400;
            rule_list_json["retention_days"] = { \
            {"days", std::to_string(days)}
            };
            response.status = boost::beast::http::status::ok;
            response.body = rule_list_json.dump();
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
                        "  \"message\":\"Query to get list of rule failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get rule listretention days";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get rule list\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}