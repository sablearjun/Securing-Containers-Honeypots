#include "RuleEngine.h"
#include "../3rdparty/json.hpp"

#include <iostream>
#include <thread>
#include <utility>
#include "../common/hostinfo.h"
#include "../common/logger.h"

#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
RuleEngine::RuleEngine(std::shared_ptr<ConnectionPool> pool,
                       std::vector<std::tuple<std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent &, Event &)>>> process_rules)
{
    this->pool = std::move(pool);
    this->process_event_rules = std::move(process_rules);
}

[[noreturn]] void RuleEngine::run_audit()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_pending_records", "select * from process_events where "
                                                   "(is_processed is not true and (process_tree is not null and process_tree <> '' or process_tree = '[]')) "
                                                   "order by id asc limit 100");
        connection->prepare("set_processed", "update process_events set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    auto process_json = nlohmann::json::parse(process_tree);
                    auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent process_event;
                    process_event.parents = parents;
                    process_event.entry.pid = result.at(i)["pid"].as<long>();
                    process_event.entry.ppid = result.at(i)["ppid"].as<long>();
                    process_event.entry.event_time = result.at(i)["time"].as<long long>();
                    process_event.entry.user_id = result.at(i)["uid"].as<long long>();
                    process_event.entry.group_id = result.at(i)["gid"].as<long long>();
                    process_event.entry.effective_group_id = result.at(i)["egid"].as<long long>();
                    process_event.entry.effective_user_id = result.at(i)["euid"].as<long long>();
                    process_event.entry.owner_user_id = result.at(i)["owner_uid"].as<long long>();
                    process_event.entry.owner_group_id = result.at(i)["owner_gid"].as<long long>();
                    process_event.entry.path = result.at(i)["path"].as<std::string>();
                    process_event.entry.command_line = result.at(i)["cmdline"].as<std::string>();
                    process_event.entry.current_working_directory = result.at(i)["cwd"].as<std::string>();
                    process_event.entry.syscall = result.at(i)["syscall"].as<std::string>();
                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::function<bool(const ProcessEvent &, Event &)> process = std::get<6>(rule);
                        nlohmann::json context_list = nlohmann::json::array();
                        if (process(process_event, _event))
                        {
                            bool isAlert = std::get<5>(rule);
                            SeverityLevel severity = std::get<4>(rule);
                            std::string code = std::get<0>(rule);
                            std::string technique = std::get<1>(rule);
                            std::string technique_id = std::get<2>(rule);
                            std::string link = std::get<3>(rule);
                            std::string host_identifier = result.at(i)["host_identifier"].as<std::string>();
                            std::string hostname = result.at(i)["hostname"].as<std::string>();
                            long unixtime = result.at(i)["unixtime"].as<long>();
                            long start_time = unixtime - 60;
                            long end_time = unixtime + 60;

                            long long pid = result.at(i)["pid"].as<long long>();
                            std::string process_files_open = process_files_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_files_open == "") && !(process_files_open == "null"))
                            {
                                context_list.push_back(process_files_open);
                            }

                            std::string process_sockets_open = process_sockets_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_sockets_open == "") && !(process_sockets_open == "null"))
                            {
                                context_list.push_back(process_sockets_open);
                            }

                            std::string process_pipes_open = process_pipes_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_pipes_open == "") && !(process_pipes_open == "null"))
                            {
                                context_list.push_back(process_pipes_open);
                            }

                            std::string process_ports_open = process_ports_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_ports_open == "") && !(process_ports_open == "null"))
                            {
                                context_list.push_back(process_ports_open);
                            }

                            nlohmann::json event_metadata;
                            event_metadata["path"] = process_event.entry.path;
                            event_metadata["command_line"] = process_event.entry.command_line;

                            nlohmann::json parents_list = nlohmann::json::array();

                            for (auto parent : process_event.parents)
                            {
                                nlohmann::json obj;
                                obj["pid"] = parent.pid;
                                obj["path"] = parent.path;
                                obj["command_line"] = parent.command_line;
                                obj["cwd"] = parent.current_working_directory;
                                obj["ppid"] = parent.ppid;
                                obj["syscall"] = parent.syscall;
                                obj["event_time"] = parent.event_time;
                                obj["user_id"] = parent.user_id;
                                obj["group_id"] = parent.group_id;
                                obj["effective_gid"] = parent.effective_group_id;
                                obj["effective_uid"] = parent.effective_user_id;
                                obj["owner_uid"] = parent.owner_user_id;
                                obj["owner_gid"] = parent.owner_group_id;

                                parents_list.push_back(obj);
                            }

                            event_metadata["parents"] = parents_list;
                            event_metadata["extra_metadata"] = _event.metadata;
                            event_metadata["pid"] = process_event.entry.pid;
                            event_metadata["path"] = process_event.entry.path;
                            event_metadata["command_line"] = process_event.entry.command_line;
                            event_metadata["cwd"] = process_event.entry.current_working_directory;
                            event_metadata["ppid"] = process_event.entry.ppid;
                            event_metadata["syscall"] = process_event.entry.syscall;
                            event_metadata["event_time"] = process_event.entry.event_time;
                            event_metadata["user_id"] = process_event.entry.user_id;
                            event_metadata["group_id"] = process_event.entry.group_id;
                            event_metadata["effective_gid"] = process_event.entry.effective_group_id;
                            event_metadata["effective_uid"] = process_event.entry.effective_user_id;
                            event_metadata["owner_uid"] = process_event.entry.owner_user_id;
                            event_metadata["owner_gid"] = process_event.entry.owner_group_id;

                            if (isAlert)
                            {
                                std::cout << "CODE: " << code << " (" << severity << ")\n";
                                std::cout << "METADATA: " << _event.metadata << "\n\n"
                                          << std::flush;
                            }

                            connection->prepare("insert_events", "insert into events (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Process Alert', $9, $10, $11, $12)");
                            transaction.exec_prepared("insert_events", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, process_event.entry.pid);
                            // transaction.commit();
                            connection->unprepare("insert_events");
                        }
                    }

                    transaction.exec_prepared("set_processed", record_id);
                    transaction.commit();
                }
                catch (...)
                {
                    continue;
                }
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        connection->unprepare("get_pending_records");
        connection->unprepare("set_processed");

        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void RuleEngine::run_bpf()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_bpf_pending_records", "select * from bpf_process_events where "
                                                       "(is_processed is not true and (process_tree is not null and process_tree <> '' or process_tree = '[]')) "
                                                       "order by id asc limit 100");
        connection->prepare("set_bpf_processed", "update bpf_process_events set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_bpf_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    auto process_json = nlohmann::json::parse(process_tree);
                    auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent process_event;
                    process_event.parents = parents;
                    process_event.entry.pid = result.at(i)["pid"].as<long>();
                    process_event.entry.parent = result.at(i)["parent"].as<long>();
                    process_event.entry.event_time = result.at(i)["time"].as<long long>();
                    process_event.entry.user_id = result.at(i)["uid"].as<long long>();
                    process_event.entry.group_id = result.at(i)["gid"].as<long long>();
                    // process_event.entry.effective_group_id = result.at(i)["egid"].as<long long>();
                    // process_event.entry.effective_user_id = result.at(i)["euid"].as<long long>();
                    // process_event.entry.owner_user_id = result.at(i)["owner_uid"].as<long long>();
                    // process_event.entry.owner_group_id = result.at(i)["owner_gid"].as<long long>();
                    process_event.entry.path = result.at(i)["path"].as<std::string>();
                    process_event.entry.command_line = result.at(i)["cmdline"].as<std::string>();
                    process_event.entry.current_working_directory = result.at(i)["cwd"].as<std::string>();
                    process_event.entry.syscall = result.at(i)["syscall"].as<std::string>();
                    process_event.entry.uptime = result.at(i)["uptime"].as<std::string>();

                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::function<bool(const ProcessEvent &, Event &)> process = std::get<6>(rule);
                        nlohmann::json context_list = nlohmann::json::array();
                        // std::cout<<_event.code<<std::endl;

                        long event_pid = result.at(i)["pid"].as<long>();
                        if (process(process_event, _event))
                        {
                            bool isAlert = std::get<5>(rule);
                            SeverityLevel severity = std::get<4>(rule);
                            std::string code = std::get<0>(rule);
                            std::string technique = std::get<1>(rule);
                            std::string technique_id = std::get<2>(rule);
                            std::string link = std::get<3>(rule);
                            std::string host_identifier = result.at(i)["host_identifier"].as<std::string>();
                            std::string hostname = result.at(i)["hostname"].as<std::string>();
                            long unixtime = result.at(i)["unixtime"].as<long>();
                            long start_time = unixtime - 60;
                            long end_time = unixtime + 60;

                            long long pid = result.at(i)["pid"].as<long long>();
                            std::string process_files_open = process_files_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_files_open == "") && !(process_files_open == "null"))
                            {
                                context_list.push_back(process_files_open);
                            }

                            std::string process_sockets_open = process_sockets_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_sockets_open == "") && !(process_sockets_open == "null"))
                            {
                                context_list.push_back(process_sockets_open);
                            }

                            std::string process_pipes_open = process_pipes_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_pipes_open == "") && !(process_pipes_open == "null"))
                            {
                                context_list.push_back(process_pipes_open);
                            }

                            std::string process_ports_open = process_ports_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_ports_open == "") && !(process_ports_open == "null"))
                            {
                                context_list.push_back(process_ports_open);
                            }

                            nlohmann::json event_metadata;
                            event_metadata["path"] = process_event.entry.path;
                            event_metadata["command_line"] = process_event.entry.command_line;

                            nlohmann::json parents_list = nlohmann::json::array();

                            for (auto parent : process_event.parents)
                            {
                                nlohmann::json obj;
                                obj["pid"] = parent.pid;
                                obj["path"] = parent.path;
                                obj["command_line"] = parent.command_line;
                                obj["cwd"] = parent.current_working_directory;
                                obj["parent"] = parent.parent;
                                obj["syscall"] = parent.syscall;
                                obj["event_time"] = parent.event_time;
                                obj["user_id"] = parent.user_id;
                                obj["group_id"] = parent.group_id;
                                obj["effective_gid"] = parent.effective_group_id;
                                obj["effective_uid"] = parent.effective_user_id;
                                obj["owner_uid"] = parent.owner_user_id;
                                obj["owner_gid"] = parent.owner_group_id;

                                parents_list.push_back(obj);
                            }

                            event_metadata["parents"] = parents_list;
                            event_metadata["extra_metadata"] = _event.metadata;
                            event_metadata["pid"] = process_event.entry.pid;
                            event_metadata["path"] = process_event.entry.path;
                            event_metadata["command_line"] = process_event.entry.command_line;
                            event_metadata["cwd"] = process_event.entry.current_working_directory;
                            event_metadata["parent"] = process_event.entry.parent;
                            event_metadata["syscall"] = process_event.entry.syscall;
                            event_metadata["event_time"] = process_event.entry.event_time;
                            event_metadata["user_id"] = process_event.entry.user_id;
                            event_metadata["group_id"] = process_event.entry.group_id;
                            event_metadata["effective_gid"] = process_event.entry.effective_group_id;
                            event_metadata["effective_uid"] = process_event.entry.effective_user_id;
                            event_metadata["owner_uid"] = process_event.entry.owner_user_id;
                            event_metadata["owner_gid"] = process_event.entry.owner_group_id;

                            if (isAlert)
                            {
                                std::cout << "CODE: " << code << " (" << severity << ")\n";
                                std::cout << "METADATA: " << _event.metadata << "\n\n"
                                          << std::flush;
                            }

                            connection->prepare("insert_events", "insert into events (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Process Alert', $9, $10, $11, $12)");

                            transaction.exec_prepared("insert_events", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid);
                            // transaction.commit();
                            connection->unprepare("insert_events");
                        }
                    }

                    transaction.exec_prepared("set_bpf_processed", record_id);
                    transaction.commit();
                }
                catch (const nlohmann::json::parse_error &error)
                {
                    BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
                    
                }
                catch (const nlohmann::json::type_error &error)
                {
                    BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
                    
                }
                catch (pqxx::sql_error const &e)
                {
                    BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
                }
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        connection->unprepare("get_bpf_pending_records");
        connection->unprepare("set_bpf_processed");

        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void RuleEngine::run_bpf_file()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_pending_records", "select * from bpf_file_events where "
                                                   "(is_processed is not true and (process_tree is not null and process_tree <> '' or process_tree = '[]')) "
                                                   "order by id asc limit 100");
        connection->prepare("set_processed", "update bpf_file_events set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    auto process_json = nlohmann::json::parse(process_tree);
                    auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent process_event;
                    process_event.parents = parents;
                    process_event.entry.pid = result.at(i)["pid"].as<long>();
                    process_event.entry.parent = result.at(i)["parent"].as<long>();
                    process_event.entry.event_time = result.at(i)["time"].as<long long>();
                    process_event.entry.user_id = result.at(i)["uid"].as<long long>();
                    process_event.entry.group_id = result.at(i)["gid"].as<long long>();
                    process_event.entry.path = result.at(i)["path"].as<std::string>();
                    process_event.entry.syscall = result.at(i)["syscall"].as<std::string>();
                    process_event.entry.file_path = result.at(i)["file_path"].as<std::string>();
                    process_event.entry.flags = result.at(i)["flags"].as<std::string>();
                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::function<bool(const ProcessEvent &, Event &)> process = std::get<6>(rule);
                        nlohmann::json context_list = nlohmann::json::array();
                        if (process(process_event, _event))
                        {
                            bool isAlert = std::get<5>(rule);
                            SeverityLevel severity = std::get<4>(rule);
                            std::string code = std::get<0>(rule);
                            std::string technique = std::get<1>(rule);
                            std::string technique_id = std::get<2>(rule);
                            std::string link = std::get<3>(rule);
                            std::string host_identifier = result.at(i)["host_identifier"].as<std::string>();
                            std::string hostname = result.at(i)["hostname"].as<std::string>();
                            long unixtime = result.at(i)["unixtime"].as<long>();
                            long start_time = unixtime - 60;
                            long end_time = unixtime + 60;

                            long long pid = result.at(i)["pid"].as<long long>();
                            std::string process_files_open = process_files_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_files_open == "") && !(process_files_open == "null"))
                            {
                                context_list.push_back(process_files_open);
                            }

                            std::string process_sockets_open = process_sockets_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_sockets_open == "") && !(process_sockets_open == "null"))
                            {
                                context_list.push_back(process_sockets_open);
                            }

                            std::string process_pipes_open = process_pipes_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_pipes_open == "") && !(process_pipes_open == "null"))
                            {
                                context_list.push_back(process_pipes_open);
                            }

                            std::string process_ports_open = process_ports_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_ports_open == "") && !(process_ports_open == "null"))
                            {
                                context_list.push_back(process_ports_open);
                            }

                            nlohmann::json event_metadata;
                            event_metadata["path"] = process_event.entry.path;
                            event_metadata["command_line"] = process_event.entry.command_line;

                            nlohmann::json parents_list = nlohmann::json::array();

                            for (auto parent : process_event.parents)
                            {
                                nlohmann::json obj;
                                obj["pid"] = parent.pid;
                                obj["path"] = parent.path;
                                obj["command_line"] = parent.command_line;
                                obj["cwd"] = parent.current_working_directory;
                                obj["parent"] = parent.parent;
                                obj["syscall"] = parent.syscall;
                                obj["event_time"] = parent.event_time;
                                obj["uid"] = parent.user_id;
                                obj["gid"] = parent.group_id;

                                parents_list.push_back(obj);
                            }

                            event_metadata["parents"] = parents_list;
                            event_metadata["extra_metadata"] = _event.metadata;
                            event_metadata["pid"] = process_event.entry.pid;
                            event_metadata["path"] = process_event.entry.path;
                            event_metadata["command_line"] = process_event.entry.command_line;
                            event_metadata["cwd"] = process_event.entry.current_working_directory;
                            event_metadata["parent"] = process_event.entry.parent;
                            event_metadata["syscall"] = process_event.entry.syscall;
                            event_metadata["event_time"] = process_event.entry.event_time;
                            event_metadata["uid"] = process_event.entry.user_id;
                            event_metadata["gid"] = process_event.entry.group_id;

                            if (isAlert)
                            {
                                std::cout << "CODE: " << code << " (" << severity << ")\n";
                                std::cout << "METADATA: " << _event.metadata << "\n\n"
                                          << std::flush;
                            }

                            connection->prepare("insert_events", "insert into events (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'File Alert', $9, $10, $11, $12)");
                            transaction.exec_prepared("insert_events", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, process_event.entry.pid);
                            // transaction.commit();
                            connection->unprepare("insert_events");
                        }
                    }

                    transaction.exec_prepared("set_processed", record_id);
                    transaction.commit();
                }
                catch (...)
                {
                    continue;
                }
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        connection->unprepare("get_pending_records");
        connection->unprepare("set_processed");

        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void RuleEngine::run_bpf_socket()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_pending_records", "select * from bpf_socket_events where "
                                                   "(is_processed is not true and (process_tree is not null and process_tree <> '' or process_tree = '[]')) "
                                                   "order by id asc limit 100");
        connection->prepare("set_processed", "update bpf_socket_events set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    auto process_json = nlohmann::json::parse(process_tree);
                    auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent process_event;
                    process_event.parents = parents;
                    process_event.entry.pid = result.at(i)["pid"].as<long>();
                    process_event.entry.parent = result.at(i)["parent"].as<long>();
                    process_event.entry.event_time = result.at(i)["time"].as<long long>();
                    process_event.entry.user_id = result.at(i)["uid"].as<long long>();
                    process_event.entry.group_id = result.at(i)["gid"].as<long long>();
                    process_event.entry.path = result.at(i)["path"].as<std::string>();
                    process_event.entry.syscall = result.at(i)["syscall"].as<std::string>();
                    process_event.entry.protocol = result.at(i)["protocol"].as<long long>();
                    process_event.entry.local_address = result.at(i)["local_address"].as<std::string>();
                    process_event.entry.remote_address = result.at(i)["remote_address"].as<std::string>();
                    process_event.entry.local_port = result.at(i)["local_port"].as<long long>();
                    process_event.entry.remote_port = result.at(i)["remote_port"].as<long long>();
                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::function<bool(const ProcessEvent &, Event &)> process = std::get<6>(rule);
                        nlohmann::json context_list = nlohmann::json::array();
                        if (process(process_event, _event))
                        {
                            bool isAlert = std::get<5>(rule);
                            SeverityLevel severity = std::get<4>(rule);
                            std::string code = std::get<0>(rule);
                            std::string technique = std::get<1>(rule);
                            std::string technique_id = std::get<2>(rule);
                            std::string link = std::get<3>(rule);
                            std::string host_identifier = result.at(i)["host_identifier"].as<std::string>();
                            std::string hostname = result.at(i)["hostname"].as<std::string>();
                            long unixtime = result.at(i)["unixtime"].as<long>();
                            long start_time = unixtime - 60;
                            long end_time = unixtime + 60;

                            long long pid = result.at(i)["pid"].as<long long>();
                            std::string process_files_open = process_files_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_files_open == "") && !(process_files_open == "null"))
                            {
                                context_list.push_back(process_files_open);
                            }

                            std::string process_sockets_open = process_sockets_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_sockets_open == "") && !(process_sockets_open == "null"))
                            {
                                context_list.push_back(process_sockets_open);
                            }

                            std::string process_pipes_open = process_pipes_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_pipes_open == "") && !(process_pipes_open == "null"))
                            {
                                context_list.push_back(process_pipes_open);
                            }

                            std::string process_ports_open = process_ports_opened(host_identifier, pid, start_time, end_time, connection);
                            if (!(process_ports_open == "") && !(process_ports_open == "null"))
                            {
                                context_list.push_back(process_ports_open);
                            }

                            nlohmann::json event_metadata;
                            event_metadata["path"] = process_event.entry.path;
                            event_metadata["command_line"] = process_event.entry.command_line;

                            nlohmann::json parents_list = nlohmann::json::array();

                            for (auto parent : process_event.parents)
                            {
                                nlohmann::json obj;
                                obj["pid"] = parent.pid;
                                obj["path"] = parent.path;
                                obj["command_line"] = parent.command_line;
                                obj["cwd"] = parent.current_working_directory;
                                obj["parent"] = parent.parent;
                                obj["syscall"] = parent.syscall;
                                obj["event_time"] = parent.event_time;
                                obj["uid"] = parent.user_id;
                                obj["gid"] = parent.group_id;
                                parents_list.push_back(obj);
                            }

                            event_metadata["parents"] = parents_list;
                            event_metadata["extra_metadata"] = _event.metadata;
                            event_metadata["pid"] = process_event.entry.pid;
                            event_metadata["path"] = process_event.entry.path;
                            event_metadata["command_line"] = process_event.entry.command_line;
                            event_metadata["cwd"] = process_event.entry.current_working_directory;
                            event_metadata["parent"] = process_event.entry.parent;
                            event_metadata["syscall"] = process_event.entry.syscall;
                            event_metadata["event_time"] = process_event.entry.event_time;
                            event_metadata["uid"] = process_event.entry.user_id;
                            event_metadata["gid"] = process_event.entry.group_id;
                            if (isAlert)
                            {
                                std::cout << "CODE: " << code << " (" << severity << ")\n";
                                std::cout << "METADATA: " << _event.metadata << "\n\n"
                                          << std::flush;
                            }

                            connection->prepare("insert_events", "insert into events (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open,  alert_type, technique, technique_id, link, pid) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Socket Alert', $9, $10, $11, $12)");
                            transaction.exec_prepared("insert_events", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, process_event.entry.pid);
                            // transaction.commit();
                            connection->unprepare("insert_events");
                        }
                    }

                    transaction.exec_prepared("set_processed", record_id);
                    transaction.commit();
                }
                catch (...)
                {
                    continue;
                }
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        connection->unprepare("get_pending_records");
        connection->unprepare("set_processed");

        pool->ReturnConnection(connection);
    }
}

void RuleEngine::run()
{
    std::thread bpf_process_event_rule_engine_thread(&RuleEngine::run_bpf, this);
    std::thread bpf_file_event_rule_engine_thread(&RuleEngine::run_bpf_file, this);
    std::thread bpf_socket_event_rule_engine_thread(&RuleEngine::run_bpf_socket, this);

    bpf_process_event_rule_engine_thread.join();
    bpf_file_event_rule_engine_thread.join();
    bpf_socket_event_rule_engine_thread.join();
}