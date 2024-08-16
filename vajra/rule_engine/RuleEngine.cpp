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
                       std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent &, Event &)>>> process_rules,
                       std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const WinProcessEvent &, Event &)>>> win_process_rules)
{
    this->pool = std::move(pool);
    this->process_event_rules = std::move(process_rules);
    this->win_process_event_rules = std::move(win_process_rules);
    
}

[[noreturn]] void RuleEngine::run_audit()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_pending_records", "select * from process_events where "
                                                   "is_processed is not true  "
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
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent process_event;
                    // process_event.parents = parents;
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
                    process_event.entry.cmdline = result.at(i)["cmdline"].as<std::string>();
                    process_event.entry.cwd = result.at(i)["cwd"].as<std::string>();
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
                        std::string os_name = std::get<4>(rule);
                        if(os_name == "Linux")
                        {
                            std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                            nlohmann::json context_list = nlohmann::json::array();
                            long event_pid = result.at(i)["pid"].as<long>();
                            long event_id = result.at(i)["id"].as<long>();
                            if (process(process_event, _event))
                            {
                                bool isAlert = std::get<6>(rule);
                                SeverityLevel severity = std::get<5>(rule);
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
                                event_metadata["cmdline"] = process_event.entry.cmdline;

                                nlohmann::json parents_list = nlohmann::json::array();

                                for (auto parent : process_event.parents)
                                {
                                    nlohmann::json obj;
                                    obj["pid"] = parent.pid;
                                    obj["path"] = parent.path;
                                    obj["cmdline"] = parent.cmdline;
                                    obj["cwd"] = parent.cwd;
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
                                event_metadata["cmdline"] = process_event.entry.cmdline;
                                event_metadata["cwd"] = process_event.entry.cwd;
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

                                connection->prepare("insert_alert", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid, action, event_id, os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Process Alert', $9, $10, $11, $12, 'None', $13, $14)");
                                transaction.exec_prepared("insert_alert", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid, event_id, os_name);
                                // transaction.commit();
                                connection->unprepare("insert_alert");
                            }
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

[[noreturn]] void RuleEngine::run_audit_socket()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_pending_records", "select * from socket_events where "
                                                   "is_processed is not true  "
                                                   "order by id asc limit 100");
        connection->prepare("set_processed", "update socket_events set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent process_event;
                    // process_event.parents = parents;
                    process_event.entry.action = result.at(i)["action"].as<std::string>();
                    process_event.entry.pid = result.at(i)["pid"].as<long>();
                    process_event.entry.path = result.at(i)["path"].as<std::string>();
                    process_event.entry.fd = result.at(i)["fd"].as<std::string>();
                    process_event.entry.auid = result.at(i)["auid"].as<long>();
                    process_event.entry.status = result.at(i)["status"].as<std::string>();
                    process_event.entry.family = result.at(i)["family"].as<int>();
                    process_event.entry.protocol = result.at(i)["protocol"].as<int>();
                    process_event.entry.local_address = result.at(i)["local_address"].as<std::string>();
                    process_event.entry.remote_address = result.at(i)["remote_address"].as<std::string>();
                    process_event.entry.local_port = result.at(i)["local_port"].as<int>();
                    process_event.entry.remote_port = result.at(i)["remote_port"].as<int>();
                    process_event.entry.socket = result.at(i)["socket"].as<std::string>();
                    process_event.entry.time = result.at(i)["time"].as<long long>();
                    process_event.entry.uptime = result.at(i)["uptime"].as<long long>();
                    process_event.entry.eid = result.at(i)["eid"].as<int>();
                    process_event.entry.success = result.at(i)["success"].as<int>();
                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string os_name = std::get<4>(rule);
                        if(os_name == "Linux")
                        {
                            std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                            nlohmann::json context_list = nlohmann::json::array();
                            long event_pid = result.at(i)["pid"].as<long>();
                            long event_id = result.at(i)["id"].as<long>();
                            if (process(process_event, _event))
                            {
                                bool isAlert = std::get<6>(rule);
                                SeverityLevel severity = std::get<5>(rule);
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

                                nlohmann::json parents_list = nlohmann::json::array();

                                for (auto parent : process_event.parents)
                                {
                                    nlohmann::json obj;
                                    obj["action"] = parent.action;
                                    obj["pid"] = parent.pid;
                                    obj["path"] = parent.path;
                                    obj["fd"] = parent.fd;
                                    obj["auid"] = parent.auid;
                                    obj["status"] = parent.status;
                                    obj["family"] = parent.family;
                                    obj["protocol"] = parent.protocol;
                                    obj["local_address"] = parent.local_address;
                                    obj["remote_address"] = parent.remote_address;
                                    obj["local_port"] = parent.local_port;
                                    obj["remote_port"] = parent.remote_port;
                                    obj["socket"] = parent.socket;
                                    obj["eid"] = parent.eid	;
                                    obj["success"] = parent.success;

                                    parents_list.push_back(obj);
                                }

                                event_metadata["parents"] = parents_list;
                                event_metadata["extra_metadata"] = _event.metadata;
                                event_metadata["action"] = process_event.entry.action;
                                event_metadata["pid"] = process_event.entry.pid;
                                event_metadata["path"] = process_event.entry.path;
                                event_metadata["fd"] = process_event.entry.fd;
                                event_metadata["auid"] = process_event.entry.auid;
                                event_metadata["status"] = process_event.entry.status;
                                event_metadata["family"] = process_event.entry.family;
                                event_metadata["protocol"] = process_event.entry.protocol;
                                event_metadata["local_address"] = process_event.entry.local_address;
                                event_metadata["remote_address"] = process_event.entry.remote_address;
                                event_metadata["local_port"] = process_event.entry.local_port;
                                event_metadata["remote_port"] = process_event.entry.remote_port;
                                event_metadata["socket"] = process_event.entry.socket;
                                event_metadata["eid"] = process_event.entry.eid;
                                event_metadata["success"] = process_event.entry.success;


                                if (isAlert)
                                {
                                    std::cout << "CODE: " << code << " (" << severity << ")\n";
                                    std::cout << "METADATA: " << _event.metadata << "\n\n"
                                            << std::flush;
                                }

                                connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid, action, event_id, os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Process Alert', $9, $10, $11, $12, 'None',$13,$14)");
                                transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid, event_id, os_name);
                                // transaction.commit();
                                connection->unprepare("insert_alerts");
                            }
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
                                                       "is_processed is not true "
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
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent process_event;
                    // process_event.parents = parents;
                    process_event.entry.unixtime = result.at(i)["unixtime"].as<long long>();
                    process_event.entry.tid = result.at(i)["tid"].as<long long>();
                    process_event.entry.pid = result.at(i)["pid"].as<long>();
                    process_event.entry.parent = result.at(i)["parent"].as<long>();
                    process_event.entry.uid = result.at(i)["uid"].as<long long>();
                    process_event.entry.gid = result.at(i)["gid"].as<long long>();
                    process_event.entry.cid = result.at(i)["cid"].as<int>();
                    process_event.entry.exit_code = result.at(i)["exit_code"].as<std::string>();
                    process_event.entry.probe_error = result.at(i)["probe_error"].as<int>();
                    process_event.entry.syscall = result.at(i)["syscall"].as<std::string>();
                    process_event.entry.path = result.at(i)["path"].as<std::string>();
                    process_event.entry.cwd = result.at(i)["cwd"].as<std::string>();
                    process_event.entry.cmdline = result.at(i)["cmdline"].as<std::string>();
                    process_event.entry.eid = result.at(i)["eid"].as<int>();
                    process_event.entry.md5 = result.at(i)["md5"].as<std::string>();
                    process_event.entry.sha1 = result.at(i)["sha1"].as<std::string>();
                    process_event.entry.sha256 = result.at(i)["sha256"].as<std::string>();
                    process_event.entry.is_container_process = result.at(i)["is_container_process"].as<int>();
                    process_event.entry.container_age = result.at(i)["container_age"].as<int>();
                    process_event.entry.container_name = result.at(i)["container_name"].as<std::string>();
                    process_event.entry.uptime = result.at(i)["uptime"].as<long long>();

                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string os_name = std::get<4>(rule);
                        if(os_name == "Linux")
                        {
                            std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                            nlohmann::json context_list = nlohmann::json::array();
                            // std::cout<<_event.code<<std::endl;

                            long event_pid = result.at(i)["pid"].as<long>();
                            long event_id = result.at(i)["id"].as<long>();
                            if (process(process_event, _event))
                            {
                                bool isAlert = std::get<6>(rule);
                                SeverityLevel severity = std::get<5>(rule);
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
                                event_metadata["cmdline"] = process_event.entry.cmdline;

                                nlohmann::json parents_list = nlohmann::json::array();

                                for (auto parent : process_event.parents)
                                {
                                    nlohmann::json obj;
                                    obj["pid"] = parent.pid;
                                    obj["parent"] = parent.parent;
                                    obj["uid"] = parent.uid;
                                    obj["gid"] = parent.gid;
                                    obj["cid"] = parent.cid;
                                    obj["exit_code"] = parent.exit_code;
                                    obj["probe_error"] = parent.gid;
                                    obj["syscall"] = parent.syscall;
                                    obj["path"] = parent.path;
                                    obj["cwd"] = parent.cwd;
                                    obj["cmdline"] = parent.cmdline;
                                    obj["eid"] = parent.eid;
                                    obj["md5"] = parent.md5;
                                    obj["sha1"] = parent.sha1;
                                    obj["sha256"] = parent.sha256;
                                    obj["is_container_process"] = parent.is_container_process;
                                    obj["container_age"] = parent.container_age;
                                    obj["container_name"] = parent.container_name;

                                    parents_list.push_back(obj);
                                }

                                event_metadata["parents"] = parents_list;
                                event_metadata["extra_metadata"] = _event.metadata;
                                event_metadata["pid"] = process_event.entry.pid;
                                event_metadata["uid"] = process_event.entry.uid;
                                event_metadata["gid"] = process_event.entry.gid;
                                event_metadata["cid"] = process_event.entry.cid;
                                event_metadata["exit_code"] = process_event.entry.exit_code;
                                event_metadata["probe_error"] = process_event.entry.probe_error;
                                event_metadata["syscall"] = process_event.entry.syscall;
                                event_metadata["path"] = process_event.entry.path;
                                event_metadata["cwd"] = process_event.entry.cwd;
                                event_metadata["cmdline"] = process_event.entry.cmdline;
                                //event_metadata["eid"] = process_event.entry.eid;
                                event_metadata["md5"] = process_event.entry.md5;
                                event_metadata["sha1"] = process_event.entry.sha1;
                                event_metadata["sha256"] = process_event.entry.sha256;
                                event_metadata["is_container_process"] = process_event.entry.is_container_process;
                                event_metadata["container_age"] = process_event.entry.container_age;
                                event_metadata["container_name"] = process_event.entry.container_name;
                                event_metadata["parent"] = process_event.entry.parent;


                                if (isAlert)
                                {
                                    std::cout << "CODE: " << code << " (" << severity << ")\n";
                                    std::cout << "METADATA: " << _event.metadata << "\n\n"
                                            << std::flush;
                                }

                                connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid, action, event_id, os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Process Alert', $9, $10, $11, $12, 'None', $13, $14)");

                                transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid, event_id, os_name);
                                // transaction.commit();
                                connection->unprepare("insert_alerts");
                            }
                        }
                    }

                    transaction.exec_prepared("set_bpf_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                    // auto currentTime = std::chrono::system_clock::now();
                    // std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                    // std::string timeString = std::ctime(&currentTimeT);
                    // std::cout << "Current system time: " << timeString;
                    // std::cout << "bpf process thread" << std::endl;



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
                                                   "(is_processed is not true) "
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
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent process_event;
                    // process_event.parents = parents;
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
                        std::string os_name = std::get<4>(rule);
                        if(os_name == "Linux")
                        {
                            std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                            nlohmann::json context_list = nlohmann::json::array();
                            long event_pid = result.at(i)["pid"].as<long>();
                            long event_id = result.at(i)["id"].as<long>();
                            if (process(process_event, _event))
                            {
                                bool isAlert = std::get<6>(rule);
                                SeverityLevel severity = std::get<5>(rule);
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
                                event_metadata["cmdline"] = process_event.entry.cmdline;

                                nlohmann::json parents_list = nlohmann::json::array();

                                for (auto parent : process_event.parents)
                                {
                                    nlohmann::json obj;
                                    obj["pid"] = parent.pid;
                                    obj["parent"] = parent.parent;
                                    obj["uid"] = parent.uid;
                                    obj["gid"] = parent.gid;
                                    obj["cid"] = parent.cid;
                                    obj["exit_code"] = parent.exit_code;
                                    obj["probe_error"] = parent.gid;
                                    obj["syscall"] = parent.syscall;
                                    obj["path"] = parent.path;
                                    obj["cwd"] = parent.cwd;
                                    obj["cmdline"] = parent.cmdline;
                                    obj["eid"] = parent.eid;
                                    obj["md5"] = parent.md5;
                                    obj["sha1"] = parent.sha1;
                                    obj["sha256"] = parent.sha256;
                                    obj["is_container_process"] = parent.is_container_process;
                                    obj["container_age"] = parent.container_age;
                                    obj["container_name"] = parent.container_name;

                                    parents_list.push_back(obj);
                                }

                                event_metadata["parents"] = parents_list;
                                event_metadata["extra_metadata"] = _event.metadata;
                                event_metadata["pid"] = process_event.entry.pid;
                                event_metadata["path"] = process_event.entry.path;
                                event_metadata["parent"] = process_event.entry.parent;
                                event_metadata["syscall"] = process_event.entry.syscall;
                                event_metadata["probe_error"] = process_event.entry.probe_error;
                                event_metadata["uid"] = process_event.entry.uid;
                                event_metadata["gid"] = process_event.entry.gid;
                                event_metadata["file_path"] = process_event.entry.file_path;
                                event_metadata["flags"] = process_event.entry.flags;
                                

                                if (isAlert)
                                {
                                    std::cout << "CODE: " << code << " (" << severity << ")\n";
                                    std::cout << "METADATA: " << _event.metadata << "\n\n"
                                            << std::flush;
                                }

                                connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid, action, event_id) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'File Alert', $9, $10, $11, $12, 'None', $13)");
                                transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid, event_id);
                                // transaction.commit();
                                connection->unprepare("insert_alerts");
                            }
                        }
                    }

                    transaction.exec_prepared("set_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                //     auto currentTime = std::chrono::system_clock::now();
                //     std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                //     std::string timeString = std::ctime(&currentTimeT);
                //     std::cout << "Current system time: " << timeString;
                //     std::cout << "bpf file thread" << std::endl;
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
                                                   "(is_processed is not true) "
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
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent process_event;
                    // process_event.parents = parents;
                    process_event.entry.pid = result.at(i)["pid"].as<long>();
                    process_event.entry.parent = result.at(i)["parent"].as<long>();
                    process_event.entry.event_time = result.at(i)["time"].as<long long>();
                    process_event.entry.uid = result.at(i)["uid"].as<long long>();
                    process_event.entry.gid = result.at(i)["gid"].as<long long>();
                    process_event.entry.path = result.at(i)["path"].as<std::string>();
                    process_event.entry.syscall = result.at(i)["syscall"].as<std::string>();
                    process_event.entry.protocol = result.at(i)["protocol"].as<int>();
                    process_event.entry.local_address = result.at(i)["local_address"].as<std::string>();
                    process_event.entry.remote_address = result.at(i)["remote_address"].as<std::string>();
                    process_event.entry.fd = result.at(i)["fd"].as<std::string>();
                    process_event.entry.local_port = result.at(i)["local_port"].as<int>();
                    process_event.entry.remote_port = result.at(i)["remote_port"].as<int>();
                    process_event.entry.family = result.at(i)["family"].as<std::string>();
                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string os_name = std::get<4>(rule);
                        if(os_name == "Linux")
                        {
                            std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                            nlohmann::json context_list = nlohmann::json::array();
                            long event_pid = result.at(i)["pid"].as<long>();
                            long event_id = result.at(i)["id"].as<long>();
                            if (process(process_event, _event))
                            {
                                bool isAlert = std::get<6>(rule);
                                SeverityLevel severity = std::get<5>(rule);
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
                                event_metadata["cmdline"] = process_event.entry.cmdline;

                                nlohmann::json parents_list = nlohmann::json::array();

                                for (auto parent : process_event.parents)
                                {
                                    nlohmann::json obj;
                                    obj["pid"] = parent.pid;
                                    obj["parent"] = parent.parent;
                                    obj["uid"] = parent.uid;
                                    obj["gid"] = parent.gid;
                                    obj["cid"] = parent.cid;
                                    obj["exit_code"] = parent.exit_code;
                                    obj["probe_error"] = parent.gid;
                                    obj["syscall"] = parent.syscall;
                                    obj["path"] = parent.path;
                                    obj["cwd"] = parent.cwd;
                                    obj["cmdline"] = parent.cmdline;
                                    obj["eid"] = parent.eid;
                                    obj["md5"] = parent.md5;
                                    obj["sha1"] = parent.sha1;
                                    obj["sha256"] = parent.sha256;
                                    obj["is_container_process"] = parent.is_container_process;
                                    obj["container_age"] = parent.container_age;
                                    obj["container_name"] = parent.container_name;
                                }

                                event_metadata["parents"] = parents_list;
                                event_metadata["extra_metadata"] = _event.metadata;
                                event_metadata["pid"] = process_event.entry.pid;
                                event_metadata["path"] = process_event.entry.path;
                                event_metadata["parent"] = process_event.entry.parent;
                                event_metadata["syscall"] = process_event.entry.syscall;
                                event_metadata["uid"] = process_event.entry.uid;
                                event_metadata["gid"] = process_event.entry.gid;
                                event_metadata["exit_code"] = process_event.entry.exit_code;
                                event_metadata["probe_error"] = process_event.entry.probe_error;
                                event_metadata["fd"] = process_event.entry.fd;
                                event_metadata["family"] = process_event.entry.family;
                                event_metadata["gid"] = process_event.entry.gid;

                                if (isAlert)
                                {
                                    std::cout << "CODE: " << code << " (" << severity << ")\n";
                                    std::cout << "METADATA: " << _event.metadata << "\n\n"
                                            << std::flush;
                                }

                                connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open,  alert_type, technique, technique_id, link, pid, action, event_id,os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Socket Alert', $9, $10, $11, $12, 'None', $13,$14)");
                                transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid, event_id,os_name);
                                // transaction.commit();
                                connection->unprepare("insert_alerts");
                            }
                        }
                    }

                    transaction.exec_prepared("set_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                    // auto currentTime = std::chrono::system_clock::now();
                    // std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                    // std::string timeString = std::ctime(&currentTimeT);
                    // std::cout << "Current system time: " << timeString;
                    // std::cout << "bpf socket thread" << std::endl;
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


// connection->prepare("get_win_pending_records", "select * from win_process_events where "
//                                                        "(is_processed is not true and (process_tree is not null and process_tree <> '' or process_tree = '[]')) "
//                                                        "order by id asc limit 100");
[[noreturn]] void RuleEngine::run_win()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_win_pending_records", "select * from win_process_events where "
                                                       "(is_processed is not true) "
                                                       "order by id asc limit 100");
        connection->prepare("set_win_processed", "update win_process_events set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_win_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    //std::cout << process_json << std::endl;
                    ProcessEvent win_process_event;
                    // win_process_event.parents = parents;
                    win_process_event.entry.action = result.at(i)["action"].as<std::string>();
                    win_process_event.entry.pid = result.at(i)["pid"].as<long>();
                    win_process_event.entry.process_guid = result.at(i)["process_guid"].as<std::string>();
                    win_process_event.entry.path = result.at(i)["path"].as<std::string>();
                    win_process_event.entry.cmdline = result.at(i)["cmdline"].as<std::string>();
                    win_process_event.entry.parent = result.at(i)["parent_pid"].as<long>();
                    win_process_event.entry.parent_path = result.at(i)["parent_path"].as<std::string>();
                    win_process_event.entry.eventid = result.at(i)["eventid"].as<int>();
                    win_process_event.entry.path = result.at(i)["path"].as<std::string>();
                    win_process_event.entry.cmdline = result.at(i)["cmdline"].as<std::string>();
                    win_process_event.entry.sha256 = result.at(i)["sha256"].as<std::string>();  
                    win_process_event.entry.uptime = result.at(i)["uptime"].as<long long>();
                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string os_name = std::get<4>(rule);
                        if(os_name == "Windows")
                        {
                            std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                            nlohmann::json context_list = nlohmann::json::array();
                            //std::cout<<_event.code<<std::endl;

                            long event_pid = result.at(i)["pid"].as<long>();
                            long event_id = result.at(i)["id"].as<long>();
                            if (process(win_process_event, _event))
                            {
                                long r_id;
                                bool isAlert = std::get<6>(rule);
                                SeverityLevel severity = std::get<5>(rule);
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
                                event_metadata["path"] = win_process_event.entry.path;
                                event_metadata["cmdline"] = win_process_event.entry.cmdline;

                                nlohmann::json parents_list = nlohmann::json::array();

                                for (auto parent : win_process_event.parents)
                                {
                                    nlohmann::json obj;
                                    obj["pid"] = parent.pid;
                                    obj["path"] = parent.path;
                                    obj["cmdline"] = parent.cmdline;
                                    obj["action"] = parent.action;
                                    obj["path"] = parent.path;
                                    obj["parent_pid"] = parent.parent;
                                    obj["parent_path"] = parent.parent_path;
                                    obj["eventid"] = parent.eventid;
                                    obj["sha256"] = parent.sha256;

                                    parents_list.push_back(obj);
                                }

                                event_metadata["parents"] = parents_list;
                                event_metadata["extra_metadata"] = _event.metadata;
                                event_metadata["pid"] = win_process_event.entry.pid;
                                event_metadata["path"] = win_process_event.entry.path;
                                event_metadata["cmdline"] = win_process_event.entry.cmdline;
                                event_metadata["action"] = win_process_event.entry.action;
                                event_metadata["parent_pid"] = win_process_event.entry.parent;
                                event_metadata["unixtime"] = win_process_event.entry.unixtime;
                                event_metadata["parent_path"] = win_process_event.entry.parent_path;
                                event_metadata["eventid"] = win_process_event.entry.eventid;
                                event_metadata["sha256"] = win_process_event.entry.sha256;

                                if (isAlert)
                                {
                                    std::cout << "CODE: " << code << " (" << severity << ")\n";
                                    std::cout << "METADATA: " << _event.metadata << "\n\n"
                                            << std::flush;
                                }

                                connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid, action, event_id,os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Process Alert', $9, $10, $11, $12, 'None', $13, $14)");

                                transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid, event_id,os_name);
                                // transaction.commit();
                                connection->unprepare("insert_alerts");
                            }
                        }
                    }

                    transaction.exec_prepared("set_win_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                    // auto currentTime = std::chrono::system_clock::now();
                    // std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                    // std::string timeString = std::ctime(&currentTimeT);
                    // std::cout << "Current system time: " << timeString;
                    // std::cout << "win process thread" << std::endl;
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
        connection->unprepare("get_win_pending_records");
        connection->unprepare("set_win_processed");

        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void RuleEngine::run_win_socket()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_win_pending_records", "select * from win_socket_events where "
                                                       "(is_processed is not true) "
                                                       "order by id asc limit 100");
        connection->prepare("set_win_processed", "update win_socket_events set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_win_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {

                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent win_process_event;
                    // win_process_event.parents = parents;
                    win_process_event.entry.pid = result.at(i)["pid"].as<long>();
                    win_process_event.entry.parent = result.at(i)["parent_pid"].as<long>();
                    win_process_event.entry.action = result.at(i)["action"].as<std::string>();
                    win_process_event.entry.process_name = result.at(i)["process_name"].as<std::string>();
                    win_process_event.entry.family = result.at(i)["family"].as<std::string>();
                    win_process_event.entry.local_address = result.at(i)["local_address"].as<std::string>();
                    win_process_event.entry.remote_address = result.at(i)["remote_address"].as<std::string>();
                    win_process_event.entry.local_port = result.at(i)["local_port"].as<int>();
                    win_process_event.entry.remote_port = result.at(i)["remote_port"].as<int>();
                    win_process_event.entry.uptime = result.at(i)["uptime"].as<long long>();
                    win_process_event.entry.protocol = result.at(i)["protocol"].as<int>();
                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string os_name = std::get<4>(rule);
                        if(os_name == "Windows")
                        {
                            std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                            nlohmann::json context_list = nlohmann::json::array();
                            //std::cout<<_event.code<<std::endl;

                            long event_pid = result.at(i)["pid"].as<long>();
                            long event_id = result.at(i)["id"].as<long>();
                            if (process(win_process_event, _event))
                            {
                                long r_id;
                                bool isAlert = std::get<6>(rule);
                                SeverityLevel severity = std::get<5>(rule);
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
                                event_metadata["path"] = win_process_event.entry.path;
                                event_metadata["cmdline"] = win_process_event.entry.cmdline;

                                nlohmann::json parents_list = nlohmann::json::array();

                                for (auto parent : win_process_event.parents)
                                {
                                    nlohmann::json obj;
                                    obj["pid"] = parent.pid;
                                    obj["path"] = parent.path;
                                    obj["cmdline"] = parent.cmdline;
                                    obj["action"] = parent.action;
                                    obj["path"] = parent.path;
                                    obj["parent_pid"] = parent.parent;
                                    obj["parent_path"] = parent.parent_path;
                                    obj["eventid"] = parent.eventid;
                                    obj["sha256"] = parent.sha256;

                                    parents_list.push_back(obj);
                                }

                                event_metadata["parents"] = parents_list;
                                event_metadata["extra_metadata"] = _event.metadata;
                                event_metadata["pid"] = win_process_event.entry.pid;
                                event_metadata["path"] = win_process_event.entry.path;
                                event_metadata["action"] = win_process_event.entry.action;
                                event_metadata["process_name"] = win_process_event.entry.process_name;
                                event_metadata["parent_pid"] = win_process_event.entry.parent;
                                event_metadata["family"] = win_process_event.entry.family;
                                event_metadata["local_address"] = win_process_event.entry.local_address;
                                event_metadata["remote_address"] = win_process_event.entry.remote_address;
                                event_metadata["local_port"] = win_process_event.entry.local_port;
                                event_metadata["remote_port"] = win_process_event.entry.remote_port;
                                event_metadata["uptime"] = win_process_event.entry.uptime;
                                event_metadata["protocol"] = win_process_event.entry.protocol;


                                if (isAlert)
                                {
                                    std::cout << "CODE: " << code << " (" << severity << ")\n";
                                    std::cout << "METADATA: " << _event.metadata << "\n\n"
                                            << std::flush;
                                }

                                connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid, action, event_id,os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Socket Alert', $9, $10, $11, $12, 'None', $13,$14)");

                                transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid, event_id, os_name);
                                // transaction.commit();
                                connection->unprepare("insert_alerts");
                            }
                        }
                    }

                    transaction.exec_prepared("set_win_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                    // auto currentTime = std::chrono::system_clock::now();
                    // std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                    // std::string timeString = std::ctime(&currentTimeT);
                    // std::cout << "Current system time: " << timeString;
                    // std::cout << "win socket thread" << std::endl;
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
        connection->unprepare("get_win_pending_records");
        connection->unprepare("set_win_processed");

        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void RuleEngine::run_win_file()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_win_pending_records", "select * from win_file_events where "
                                                       "(is_processed is not true) "
                                                       "order by id asc limit 100");
        connection->prepare("set_win_processed", "update win_file_events set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_win_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent win_process_event;
                    // win_process_event.parents = parents;
                    win_process_event.entry.pid = result.at(i)["pid"].as<long>();
                    win_process_event.entry.parent = result.at(i)["parent_pid"].as<long>();
                    win_process_event.entry.unixtime = result.at(i)["unixTime"].as<long long>();
                    win_process_event.entry.action = result.at(i)["action"].as<std::string>();
                    win_process_event.entry.process_name = result.at(i)["process_name"].as<std::string>();
                    win_process_event.entry.target_path = result.at(i)["target_path"].as<std::string>();
                    win_process_event.entry.md5 = result.at(i)["md5"].as<std::string>();
                    win_process_event.entry.sha256 = result.at(i)["sha256"].as<std::string>();
                    win_process_event.entry.hashed = result.at(i)["hashed"].as<int>();
                    win_process_event.entry.md5 = result.at(i)["md5"].as<std::string>();
                    win_process_event.entry.uptime = result.at(i)["uptime"].as<long long>();
                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string os_name = std::get<4>(rule);
                        if(os_name == "Windows")
                        {
                            std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                            nlohmann::json context_list = nlohmann::json::array();
                            //std::cout<<_event.code<<std::endl;

                            long event_pid = result.at(i)["pid"].as<long>();
                            long event_id = result.at(i)["id"].as<long>();
                            if (process(win_process_event, _event))
                            {
                                long r_id;
                                bool isAlert = std::get<6>(rule);
                                SeverityLevel severity = std::get<5>(rule);
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
                                event_metadata["path"] = win_process_event.entry.path;
                                event_metadata["cmdline"] = win_process_event.entry.cmdline;

                                nlohmann::json parents_list = nlohmann::json::array();

                                for (auto parent : win_process_event.parents)
                                {
                                    nlohmann::json obj;
                                    obj["pid"] = parent.pid;
                                    obj["path"] = parent.path;
                                    obj["cmdline"] = parent.cmdline;
                                    obj["action"] = parent.action;
                                    obj["path"] = parent.path;
                                    obj["parent_pid"] = parent.parent;
                                    obj["parent_path"] = parent.parent_path;
                                    obj["eventid"] = parent.eventid;
                                    obj["sha256"] = parent.sha256;

                                    parents_list.push_back(obj);
                                }

                                event_metadata["parents"] = parents_list;
                                event_metadata["extra_metadata"] = _event.metadata;
                                event_metadata["pid"] = win_process_event.entry.pid;
                                event_metadata["path"] = win_process_event.entry.path;
                                event_metadata["unixTime"] = win_process_event.entry.unixtime;
                                event_metadata["parent_pid"] = win_process_event.entry.parent;
                                event_metadata["action"] = win_process_event.entry.action;
                                event_metadata["process_name"] = win_process_event.entry.process_name;
                                event_metadata["target_path"] = win_process_event.entry.target_path;
                                event_metadata["md5"] = win_process_event.entry.effective_group_id;
                                event_metadata["sha256"] = win_process_event.entry.effective_user_id;
                                event_metadata["hashed"] = win_process_event.entry.owner_user_id;

                                if (isAlert)
                                {
                                    std::cout << "CODE: " << code << " (" << severity << ")\n";
                                    std::cout << "METADATA: " << _event.metadata << "\n\n"
                                            << std::flush;
                                }

                                connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid, action, event_id,os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'File Alert', $9, $10, $11, $12, 'None', $13, $14)");

                                transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid, event_id,os_name);
                                // transaction.commit();
                                connection->unprepare("insert_alerts");
                            }
                        }
                    }

                    transaction.exec_prepared("set_win_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                    // auto currentTime = std::chrono::system_clock::now();
                    // std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                    // std::string timeString = std::ctime(&currentTimeT);
                    // std::cout << "Current system time: " << timeString;
                    // std::cout << "win file thread" << std::endl;
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
        connection->unprepare("get_win_pending_records");
        connection->unprepare("set_win_processed");

        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void RuleEngine::run_chrome_ext()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_chrome_ext_pending_records", "select * from chrome_extensions where "
                                                       "is_processed is not true "
                                                       "order by id asc limit 100");
        connection->prepare("set_win_processed", "update chrome_extensions set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_chrome_ext_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent chrome_extension;
                    // chrome_extension.parents = parents;
                    chrome_extension.entry.browser_type = result.at(i)["browser_type"].as<std::string>();
                    chrome_extension.entry.uid = result.at(i)["uid"].as<long>();
                    chrome_extension.entry.name = result.at(i)["name"].as<std::string>();
                    chrome_extension.entry.profile = result.at(i)["profile"].as<std::string>();
                    chrome_extension.entry.profile_path = result.at(i)["profile_path"].as<std::string>();
                    chrome_extension.entry.referenced_identifier = result.at(i)["referenced_identifier"].as<std::string>();
                    chrome_extension.entry.identifier = result.at(i)["identifier"].as<std::string>();
                    chrome_extension.entry.version = result.at(i)["version"].as<std::string>();
                    chrome_extension.entry.description = result.at(i)["description"].as<std::string>();
                    chrome_extension.entry.default_locale = result.at(i)["default_locale"].as<std::string>();
                    chrome_extension.entry.current_locale = result.at(i)["current_locale"].as<std::string>();  
                    chrome_extension.entry.update_url = result.at(i)["update_url"].as<std::string>();
                    chrome_extension.entry.author = result.at(i)["author"].as<std::string>();
                    chrome_extension.entry.persistent = result.at(i)["persistent"].as<int>();
                    chrome_extension.entry.path = result.at(i)["path"].as<std::string>();
                    chrome_extension.entry.permissions = result.at(i)["permissions"].as<std::string>();
                    chrome_extension.entry.optional_permissions = result.at(i)["optional_permissions"].as<std::string>();
                    chrome_extension.entry.manifest_hash = result.at(i)["manifest_hash"].as<std::string>();
                    chrome_extension.entry.referenced = result.at(i)["referenced"].as<long long>();
                    chrome_extension.entry.from_webstore = result.at(i)["from_webstore"].as<std::string>();
                    chrome_extension.entry.state = result.at(i)["state"].as<std::string>();
                    chrome_extension.entry.install_time = result.at(i)["install_time"].as<std::string>();
                    chrome_extension.entry.install_timestamp = result.at(i)["install_timestamp"].as<long long>();
                    chrome_extension.entry.ext_key = result.at(i)["key"].as<std::string>();


                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string os_name = std::get<4>(rule);
                        std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                        nlohmann::json context_list = nlohmann::json::array();
                        //std::cout<<_event.code<<std::endl;

                        // long event_pid = result.at(i)["pid"].as<long>();
                        if (process(chrome_extension, _event))
                        {
                            bool isAlert = std::get<6>(rule);
                            SeverityLevel severity = std::get<5>(rule);
                            std::string code = std::get<0>(rule);
                            std::string technique = std::get<1>(rule);
                            std::string technique_id = std::get<2>(rule);
                            std::string link = std::get<3>(rule);
                            std::string host_identifier = result.at(i)["host_identifier"].as<std::string>();
                            std::string hostname = result.at(i)["hostname"].as<std::string>();
                            long unixtime = result.at(i)["unixtime"].as<long>();
                            
                            nlohmann::json event_metadata;
                            event_metadata["path"] = chrome_extension.entry.path;

                            // nlohmann::json parents_list = nlohmann::json::array();


                            // event_metadata["parents"] = parents_list;
                            event_metadata["extra_metadata"] = _event.metadata;
                            event_metadata["browser_type"] = chrome_extension.entry.browser_type;
                            event_metadata["uid"] = chrome_extension.entry.uid;
                            event_metadata["name"] = chrome_extension.entry.name;
                            event_metadata["profile"] = chrome_extension.entry.profile;
                            event_metadata["profile_path"] = chrome_extension.entry.profile_path;
                            event_metadata["referenced_identifier"] = chrome_extension.entry.referenced_identifier;
                            event_metadata["identifier"] = chrome_extension.entry.identifier;
                            event_metadata["version"] = chrome_extension.entry.version;
                            event_metadata["description"] = chrome_extension.entry.description;
                            event_metadata["default_locale"] = chrome_extension.entry.default_locale;
                            event_metadata["current_locale"] = chrome_extension.entry.current_locale;
                            event_metadata["update_url"] = chrome_extension.entry.update_url;
                            event_metadata["author"] = chrome_extension.entry.author;
                            event_metadata["persistent"] = chrome_extension.entry.persistent;
                            event_metadata["path"] = chrome_extension.entry.path;
                            event_metadata["permissions"] = chrome_extension.entry.permissions;
                            event_metadata["optional_permissions"] = chrome_extension.entry.optional_permissions;
                            event_metadata["manifest_hash"] = chrome_extension.entry.manifest_hash;
                            event_metadata["referenced"] = chrome_extension.entry.referenced;
                            event_metadata["from_webstore"] = chrome_extension.entry.from_webstore;
                            event_metadata["state"] = chrome_extension.entry.state;
                            event_metadata["install_time"] = chrome_extension.entry.install_time;
                            event_metadata["install_timestamp"] = chrome_extension.entry.install_timestamp;
                            event_metadata["key"] = chrome_extension.entry.ext_key;

                            if (isAlert)
                            {
                                std::cout << "CODE: " << code << " (" << severity << ")\n";
                                std::cout << "METADATA: " << _event.metadata << "\n\n"
                                          << std::flush;
                            }

                            connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, action,os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Other Alert', $9, $10, $11, 'None',$12)");

                            transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, os_name);
                            // transaction.commit();
                            connection->unprepare("insert_alerts");
                        }
                    }

                    transaction.exec_prepared("set_win_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                    // auto currentTime = std::chrono::system_clock::now();
                    // std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                    // std::string timeString = std::ctime(&currentTimeT);
                    // std::cout << "Current system time: " << timeString;
                    // std::cout << "chrome ext thread" << std::endl;
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
        connection->unprepare("get_chrome_ext_pending_records");
        connection->unprepare("set_win_processed");

        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void RuleEngine::run_file()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_file_pending_records", "select * from file where "
                                                       "is_processed is not true "
                                                       "order by id asc limit 100");
        connection->prepare("set_file_processed", "update file set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_file_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent file;
                    // chrome_extension.parents = parents;
                    file.entry.path = result.at(i)["path"].as<std::string>();    
                    file.entry.directory = result.at(i)["directory"].as<long>();
                    file.entry.filename = result.at(i)["filename"].as<std::string>();
                    file.entry.inode = result.at(i)["inode"].as<long>();
                    file.entry.uid = result.at(i)["uid"].as<long>();
                    file.entry.gid = result.at(i)["gid"].as<long>();
                    file.entry.mode = result.at(i)["mode"].as<std::string>();
                    file.entry.device = result.at(i)["device"].as<long>();
                    file.entry.size = result.at(i)["size"].as<long>();
                    file.entry.block_size = result.at(i)["block_size"].as<int>();
                    file.entry.atime = result.at(i)["atime"].as<long>();  
                    file.entry.mtime = result.at(i)["mtime"].as<long>();
                    file.entry.ctime = result.at(i)["ctime"].as<long>();
                    file.entry.btime = result.at(i)["btime"].as<long>();
                    file.entry.hard_links = result.at(i)["hard_links"].as<int>();
                    file.entry.symlink = result.at(i)["symlink"].as<int>();
                    file.entry.type = result.at(i)["type"].as<std::string>();
                    file.entry.attributes = result.at(i)["attributes"].as<std::string>();
                    file.entry.volume_serial = result.at(i)["volume_serial"].as<long long>();
                    file.entry.file_id = result.at(i)["file_id"].as<std::string>();
                    file.entry.file_version = result.at(i)["file_version"].as<std::string>();
                    file.entry.product_version = result.at(i)["product_version"].as<std::string>();
                    file.entry.original_filename = result.at(i)["original_filename"].as<long long>();
                    file.entry.bsd_flags = result.at(i)["bsd_flags"].as<std::string>();
                    file.entry.pid_with_namespace = result.at(i)["pid_with_namespace"].as<int>();
                    file.entry.mount_namespace_id = result.at(i)["mount_namespace_id"].as<std::string>();



                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string os_name = std::get<4>(rule);
                        std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                        nlohmann::json context_list = nlohmann::json::array();
                        //std::cout<<_event.code<<std::endl;

                        // long event_pid = result.at(i)["pid"].as<long>();
                        if (process(file, _event))
                        {
                            bool isAlert = std::get<6>(rule);
                            SeverityLevel severity = std::get<5>(rule);
                            std::string code = std::get<0>(rule);
                            std::string technique = std::get<1>(rule);
                            std::string technique_id = std::get<2>(rule);
                            std::string link = std::get<3>(rule);
                            std::string host_identifier = result.at(i)["host_identifier"].as<std::string>();
                            std::string hostname = result.at(i)["hostname"].as<std::string>();
                            long unixtime = result.at(i)["unixtime"].as<long>();
                            
                            nlohmann::json event_metadata;
                            event_metadata["path"] = file.entry.path;

                            // nlohmann::json parents_list = nlohmann::json::array();


                            // event_metadata["parents"] = parents_list;
                            event_metadata["extra_metadata"] = _event.metadata;
                            event_metadata["path"] = file.entry.path;
                            event_metadata["directory"] = file.entry.directory;
                            event_metadata["filename"] = file.entry.filename;
                            event_metadata["inode"] = file.entry.inode;
                            event_metadata["uid"] = file.entry.uid;
                            event_metadata["gid"] = file.entry.gid;
                            event_metadata["mode"] = file.entry.mode;
                            event_metadata["device"] = file.entry.device;
                            event_metadata["size"] = file.entry.size;
                            event_metadata["block_size"] = file.entry.block_size;
                            event_metadata["atime"] = file.entry.atime;
                            event_metadata["mtime"] = file.entry.mtime;
                            event_metadata["ctime"] = file.entry.ctime;
                            event_metadata["btime"] = file.entry.btime;
                            event_metadata["hard_links"] = file.entry.hard_links;
                            event_metadata["symlink"] = file.entry.symlink;
                            event_metadata["type"] = file.entry.type;
                            event_metadata["attributes"] = file.entry.attributes;
                            event_metadata["volume_serial"] = file.entry.volume_serial;
                            event_metadata["file_id"] = file.entry.file_id;
                            event_metadata["file_version"] = file.entry.file_version;
                            event_metadata["product_version"] = file.entry.product_version;
                            event_metadata["original_filename"] = file.entry.original_filename;
                            event_metadata["bsd_flags"] = file.entry.bsd_flags;
                            event_metadata["pid_with_namespace"] = file.entry.pid_with_namespace;
                            event_metadata["mount_namespace_id"] = file.entry.mount_namespace_id;

                            if (isAlert)
                            {
                                std::cout << "CODE: " << code << " (" << severity << ")\n";
                                std::cout << "METADATA: " << _event.metadata << "\n\n"
                                          << std::flush;
                            }

                            connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, action,os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Other Alert', $9, $10, $11, 'None',$12)");

                            transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link,os_name);
                            // transaction.commit();
                            connection->unprepare("insert_alerts");
                        }
                    }

                    transaction.exec_prepared("set_file_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                    // auto currentTime = std::chrono::system_clock::now();
                    // std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                    // std::string timeString = std::ctime(&currentTimeT);
                    // std::cout << "Current system time: " << timeString;
                    // std::cout << "FILE thread" << std::endl;
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
        connection->unprepare("get_file_pending_records");
        connection->unprepare("set_file_processed");

        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void RuleEngine::run_processes()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_processes_pending_records", "select * from processes where "
                                                       "is_processed is not true "
                                                       "order by id asc limit 100");
        connection->prepare("set_processed", "update processes set is_processed = true where id = $1");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("get_processes_pending_records");
        transaction.commit();

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                // std::string process_tree = result.at(i)["process_tree"].as<std::string>();

                try
                {
                    // auto process_json = nlohmann::json::parse(process_tree);
                    // auto parents = JsonToProcessEntryList(process_json);
                    ProcessEvent processes;
                    // processes.parents = parents;
                    processes.entry.pid = result.at(i)["pid"].as<long>();
                    processes.entry.name = result.at(i)["name"].as<std::string>();
                    processes.entry.path = result.at(i)["path"].as<std::string>();
                    processes.entry.cmdline = result.at(i)["cmdline"].as<std::string>();
                    processes.entry.state = result.at(i)["state"].as<std::string>();
                    processes.entry.cwd = result.at(i)["cwd"].as<std::string>();
                    processes.entry.root = result.at(i)["root"].as<std::string>();
                    processes.entry.uid = result.at(i)["uid"].as<long>();
                    processes.entry.gid = result.at(i)["gid"].as<long>();
                    processes.entry.egid = result.at(i)["egid"].as<long>();
                    processes.entry.suid = result.at(i)["suid"].as<long>();
                    processes.entry.sgid = result.at(i)["sgid"].as<long>();
                    processes.entry.on_disk = result.at(i)["on_disk"].as<int>();
                    processes.entry.wired_size = result.at(i)["wired_size"].as<long>();
                    processes.entry.resident_size = result.at(i)["resident_size"].as<long>();
                    processes.entry.total_size = result.at(i)["total_size"].as<long>();
                    processes.entry.user_time = result.at(i)["user_time"].as<long>();
                    processes.entry.disk_bytes_read = result.at(i)["disk_bytes_read"].as<long>();
                    processes.entry.disk_bytes_written = result.at(i)["disk_bytes_written"].as<long>();
                    processes.entry.start_time = result.at(i)["start_time"].as<long>();
                    processes.entry.parent = result.at(i)["parent"].as<long>();
                    processes.entry.pgroup = result.at(i)["pgroup"].as<long>();
                    processes.entry.threads = result.at(i)["threads"].as<int>();
                    processes.entry.nice = result.at(i)["nice"].as<int>();
                    processes.entry.elevated_token = result.at(i)["elevated_token"].as<long>();
                    processes.entry.secure_process = result.at(i)["secure_process"].as<long>();
                    processes.entry.protection_type = result.at(i)["protection_type"].as<std::string>();
                    processes.entry.virtual_process = result.at(i)["virtual_process"].as<int>();
                    processes.entry.elapsed_time = result.at(i)["elapsed_time"].as<long>();
                    processes.entry.handle_count = result.at(i)["handle_count"].as<long>();
                    processes.entry.percent_processor_time = result.at(i)["percent_processor_time"].as<long>();

                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string os_name = std::get<4>(rule);
                        std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                    
                    
                        nlohmann::json context_list = nlohmann::json::array();
                        //std::cout<<_event.code<<std::endl;

                        // long event_pid = result.at(i)["pid"].as<long>();
                        if (process(processes, _event))
                        {
                            bool isAlert = std::get<6>(rule);
                            SeverityLevel severity = std::get<5>(rule);
                            std::string code = std::get<0>(rule);
                            std::string technique = std::get<1>(rule);
                            std::string technique_id = std::get<2>(rule);
                            std::string link = std::get<3>(rule);
                            std::string host_identifier = result.at(i)["host_identifier"].as<std::string>();
                            std::string hostname = result.at(i)["hostname"].as<std::string>();
                            long unixtime = result.at(i)["unixtime"].as<long>();
                            
                            nlohmann::json event_metadata;
                            event_metadata["path"] = processes.entry.path;

                            // nlohmann::json parents_list = nlohmann::json::array();


                            // event_metadata["parents"] = parents_list;
                            event_metadata["extra_metadata"] = _event.metadata;
                            event_metadata["pid"] = processes.entry.pid;
                            event_metadata["name"] = processes.entry.name;
                            event_metadata["path"] = processes.entry.path;
                            event_metadata["cmdline"] = processes.entry.cmdline;
                            event_metadata["state"] = processes.entry.state;
                            event_metadata["cwd"] = processes.entry.cwd;
                            event_metadata["root"] = processes.entry.root;
                            event_metadata["uid"] = processes.entry.uid;
                            event_metadata["gid"] = processes.entry.gid;
                            event_metadata["egid"] = processes.entry.egid;
                            event_metadata["suid"] = processes.entry.suid;
                            event_metadata["sgid"] = processes.entry.sgid;
                            event_metadata["on_disk"] = processes.entry.on_disk;
                            event_metadata["wired_size"] = processes.entry.wired_size;
                            event_metadata["resident_size"] = processes.entry.resident_size;
                            event_metadata["total_size"] = processes.entry.total_size;
                            event_metadata["user_time"] = processes.entry.user_time;
                            event_metadata["system_time"] = processes.entry.system_time;
                            event_metadata["disk_bytes_read"] = processes.entry.disk_bytes_read;
                            event_metadata["disk_bytes_written"] = processes.entry.disk_bytes_written;
                            event_metadata["start_time"] = processes.entry.start_time;
                            event_metadata["parent"] = processes.entry.parent;
                            event_metadata["pgroup"] = processes.entry.pgroup;
                            event_metadata["threads"] = processes.entry.threads;
                            event_metadata["nice"] = processes.entry.nice;
                            event_metadata["elevated_token"] = processes.entry.elevated_token;
                            event_metadata["secure_process"] = processes.entry.secure_process;
                            event_metadata["protection_type"] = processes.entry.protection_type;
                            event_metadata["virtual_process"] = processes.entry.virtual_process;
                            event_metadata["elapsed_time"] = processes.entry.elapsed_time;
                            event_metadata["handle_count"] = processes.entry.handle_count;
                            event_metadata["percent_processor_time"] = processes.entry.percent_processor_time;

                            if (isAlert)
                            {
                                std::cout << "CODE: " << code << " (" << severity << ")\n";
                                std::cout << "METADATA: " << _event.metadata << "\n\n"
                                        << std::flush;
                            }

                            connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, action,os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Other Alert', $9, $10, $11, 'None',$12)");

                            transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link,os_name);
                            // transaction.commit();
                            connection->unprepare("insert_alerts");
                        }
                        
                    }

                    transaction.exec_prepared("set_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                    // auto currentTime = std::chrono::system_clock::now();
                    // std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                    // std::string timeString = std::ctime(&currentTimeT);
                    // std::cout << "Current system time: " << timeString;
                    // std::cout << "processes thread" << std::endl;
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
        connection->unprepare("get_processes_pending_records");
        connection->unprepare("set_processed");

        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void RuleEngine::run_bpf_process_tree()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_bpf_pending_records", "select * from bpf_process_events where "
                                                       "(is_processed_tree is not true and (process_tree is not null and process_tree <> '' or process_tree = '[]')) "
                                                       "order by id asc limit 100");
        connection->prepare("set_bpf_processed", "update bpf_process_events set is_processed_tree = true where id = $1");
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
                    process_event.entry.unixtime = result.at(i)["unixtime"].as<long long>();
                    process_event.entry.tid = result.at(i)["tid"].as<long long>();
                    process_event.entry.pid = result.at(i)["pid"].as<long>();
                    process_event.entry.parent = result.at(i)["parent"].as<long>();
                    process_event.entry.uid = result.at(i)["uid"].as<long long>();
                    process_event.entry.gid = result.at(i)["gid"].as<long long>();
                    process_event.entry.cid = result.at(i)["cid"].as<int>();
                    process_event.entry.exit_code = result.at(i)["exit_code"].as<std::string>();
                    process_event.entry.probe_error = result.at(i)["probe_error"].as<int>();
                    process_event.entry.syscall = result.at(i)["syscall"].as<std::string>();
                    process_event.entry.path = result.at(i)["path"].as<std::string>();
                    process_event.entry.cwd = result.at(i)["cwd"].as<std::string>();
                    process_event.entry.cmdline = result.at(i)["cmdline"].as<std::string>();
                    process_event.entry.eid = result.at(i)["eid"].as<int>();
                    process_event.entry.md5 = result.at(i)["md5"].as<std::string>();
                    process_event.entry.sha1 = result.at(i)["sha1"].as<std::string>();
                    process_event.entry.sha256 = result.at(i)["sha256"].as<std::string>();
                    process_event.entry.is_container_process = result.at(i)["is_container_process"].as<int>();
                    process_event.entry.container_age = result.at(i)["container_age"].as<int>();
                    process_event.entry.container_name = result.at(i)["container_name"].as<std::string>();
                    process_event.entry.uptime = result.at(i)["uptime"].as<long long>();

                    /*
                     * Now we will run this process event through all the supplied rules.
                     * Whenever there is a match, we have got an rule event.
                     *
                     * We will show rule events on screen only if they are flagged as alerts.
                     */

                    for (const auto &rule : process_event_rules)
                    {
                        Event _event;
                        std::string technique = std::get<1>(rule);
                        std::string os_name = std::get<4>(rule);
                        if(technique == "GTFOBINS" && os_name == "Linux")
                        {
                            std::function<bool(const ProcessEvent &, Event &)> process = std::get<7>(rule);
                            nlohmann::json context_list = nlohmann::json::array();
                            // std::cout<<_event.code<<std::endl;

                            long event_pid = result.at(i)["pid"].as<long>();
                            long event_id = result.at(i)["id"].as<long>();
                            if (process(process_event, _event))
                            {
                                bool isAlert = std::get<6>(rule);
                                SeverityLevel severity = std::get<5>(rule);
                                std::string code = std::get<0>(rule);
                                // std::string technique = std::get<1>(rule);
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
                                event_metadata["cmdline"] = process_event.entry.cmdline;

                                nlohmann::json parents_list = nlohmann::json::array();

                                for (auto parent : process_event.parents)
                                {
                                    nlohmann::json obj;
                                    obj["pid"] = parent.pid;
                                    obj["parent"] = parent.parent;
                                    obj["uid"] = parent.uid;
                                    obj["gid"] = parent.gid;
                                    obj["cid"] = parent.cid;
                                    obj["exit_code"] = parent.exit_code;
                                    obj["probe_error"] = parent.gid;
                                    obj["syscall"] = parent.syscall;
                                    obj["path"] = parent.path;
                                    obj["cwd"] = parent.cwd;
                                    obj["cmdline"] = parent.cmdline;
                                    obj["eid"] = parent.eid;
                                    obj["md5"] = parent.md5;
                                    obj["sha1"] = parent.sha1;
                                    obj["sha256"] = parent.sha256;
                                    obj["is_container_process"] = parent.is_container_process;
                                    obj["container_age"] = parent.container_age;
                                    obj["container_name"] = parent.container_name;

                                    parents_list.push_back(obj);
                                }

                                event_metadata["parents"] = parents_list;
                                event_metadata["extra_metadata"] = _event.metadata;
                                event_metadata["pid"] = process_event.entry.pid;
                                event_metadata["uid"] = process_event.entry.uid;
                                event_metadata["gid"] = process_event.entry.gid;
                                event_metadata["cid"] = process_event.entry.cid;
                                event_metadata["exit_code"] = process_event.entry.exit_code;
                                event_metadata["probe_error"] = process_event.entry.probe_error;
                                event_metadata["syscall"] = process_event.entry.syscall;
                                event_metadata["path"] = process_event.entry.path;
                                event_metadata["cwd"] = process_event.entry.cwd;
                                event_metadata["cmdline"] = process_event.entry.cmdline;
                                //event_metadata["eid"] = process_event.entry.eid;
                                event_metadata["md5"] = process_event.entry.md5;
                                event_metadata["sha1"] = process_event.entry.sha1;
                                event_metadata["sha256"] = process_event.entry.sha256;
                                event_metadata["is_container_process"] = process_event.entry.is_container_process;
                                event_metadata["container_age"] = process_event.entry.container_age;
                                event_metadata["container_name"] = process_event.entry.container_name;
                                event_metadata["parent"] = process_event.entry.parent;


                                if (isAlert)
                                {
                                    std::cout << "CODE: " << code << " (" << severity << ")\n";
                                    std::cout << "METADATA: " << _event.metadata << "\n\n"
                                            << std::flush;
                                }

                                connection->prepare("insert_alerts", "insert into alerts (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid, action, event_id,os_name) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Process Alert', $9, $10, $11, $12, 'None', $13, $14)");

                                transaction.exec_prepared("insert_alerts", host_identifier, hostname, context_list.dump(), unixtime, code, event_metadata.dump(), isAlert, std::to_string(severity), technique, technique_id, link, event_pid, event_id,os_name);
                                // transaction.commit();
                                connection->unprepare("insert_alerts");
                            }
                        }
                    }

                    transaction.exec_prepared("set_bpf_processed", record_id);
                    transaction.commit();
                    // below code is to calculate performance run rule engine like './rule_engine >> output.txt'
                    // auto currentTime = std::chrono::system_clock::now();
                    // std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
                    // std::string timeString = std::ctime(&currentTimeT);
                    // std::cout << "Current system time: " << timeString;
                    // std::cout << "process tree bpf_pro thread" << std::endl;
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


void RuleEngine::run()
{
    std::thread bpf_process_event_rule_engine_thread(&RuleEngine::run_bpf, this);
    std::thread bpf_file_event_rule_engine_thread(&RuleEngine::run_bpf_file, this);
    std::thread bpf_socket_event_rule_engine_thread(&RuleEngine::run_bpf_socket, this);
    std::thread win_process_event_rule_engine_thread(&RuleEngine::run_win, this);
    std::thread win_socket_event_rule_engine_thread(&RuleEngine::run_win_socket, this);
    std::thread win_file_event_rule_engine_thread(&RuleEngine::run_win_file, this);
    std::thread run_chrome_ext_thread(&RuleEngine::run_chrome_ext, this);
    std::thread run_processes_thread(&RuleEngine::run_processes, this);
    std::thread run_bpf_process_tree_thread(&RuleEngine::run_bpf_process_tree, this);
    std::thread audit_process_event_rule_engine_thread(&RuleEngine::run_audit, this);
    std::thread audit_socket_event_rule_engine_thread(&RuleEngine::run_audit_socket, this);


    win_process_event_rule_engine_thread.join();
    bpf_process_event_rule_engine_thread.join();
    bpf_file_event_rule_engine_thread.join();
    bpf_socket_event_rule_engine_thread.join();
    win_socket_event_rule_engine_thread.join();
    win_file_event_rule_engine_thread.join();
    run_chrome_ext_thread.join();
    run_processes_thread.join();
    run_bpf_process_tree_thread.join();
    audit_process_event_rule_engine_thread.join();
    audit_socket_event_rule_engine_thread.join();
    
}