#include "../3rdparty/json.hpp"
#include "../common/ConnectionPool.h"
#include "../common/ConfigurationParser.h"
#include "../common/logger.h"
#include <chrono>
#include <ctime>
#include <boost/algorithm/string.hpp>

#include "EventProcessor.h"

#include <fstream>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <pqxx/pqxx>

ProcessEntry get_parent_from_snapshot(const nlohmann::json &processes, long pid)
{
    try
    {
        long ppid = 0;

        ProcessEntry entry;

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

BpfProcessEntry get_bpf_parent_from_snapshot(const nlohmann::json &processes, long pid)
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

std::vector<ProcessEntry> snapshot_process_tree(std::shared_ptr<pqxx::connection> connection, long ppid,
                                                long timestamp, long uptime, std::string host_identifier)
{
    try
    {
        std::vector<ProcessEntry> process_list;

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
            ProcessEntry entry = get_parent_from_snapshot(processes, ppid);
            long _tmp_pid = entry.ppid;

            if (entry.pid)
                process_list.push_back(entry);

            while (_tmp_pid != 0)
            {
                entry = get_parent_from_snapshot(processes, _tmp_pid);
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

std::vector<BpfProcessEntry> bpf_snapshot_process_tree(std::shared_ptr<pqxx::connection> connection, long ppid,
                                                       long timestamp, long uptime, std::string host_identifier)
{
    try
    {
        std::vector<BpfProcessEntry> process_list;

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
            BpfProcessEntry entry = get_bpf_parent_from_snapshot(processes, ppid);
            long _tmp_pid = entry.ppid;

            if (entry.pid)
                process_list.push_back(entry);

            while (_tmp_pid != 0)
            {
                entry = get_bpf_parent_from_snapshot(processes, _tmp_pid);
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

std::vector<ProcessEntry> get_process_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
    try
    {
        connection->prepare("get_process_data_from_id", "select * from process_events where id = $1");

        /*
         * Since events can arrive out of order, and at some delay (in case osquery cannot reach fleet server for some reason, we cannot rely upon record ID in database.
         * If we want to get latest record of event, we should order by time column instead, which records timestamp at which the event occurred. Since it comes a
         * monotonic clock, it will never be out of order.
         */

        connection->prepare("get_process_data_execve", "select * from process_events where cmdline <> '' and "
                                                       "pid = $1 and host_identifier = $2 order by time desc limit 1");
        connection->prepare("get_process_data_non_execve", "select * from process_events where cmdline = '' and "
                                                           "pid = $1 and host_identifier = $2 order by time desc limit 1");

        std::vector<ProcessEntry> process_list;
        pqxx::result result;

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_process_data_from_id", record_id);
            transaction.commit();
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
            // we have only one process entry now.
            bool loop = true;
            long ppid = result.at(0)["ppid"].as<long>();
            long pid = result.at(0)["pid"].as<long>();
            if (pid <= ppid)
            {
                return std::vector<ProcessEntry>({});
            }

            std::string path, cmdline, host_identifier;

            path = result.at(0)["path"].as<std::string>();
            cmdline = result.at(0)["cmdline"].as<std::string>();
            host_identifier = result.at(0)["host_identifier"].as<std::string>();

            long timestamp = result.at(0)["time"].as<long>();
            long uptime = result.at(0)["uptime"].as<long>();

            /*
             * First step: we prefer entries from process_events.
             */
            while (loop)
            {
                pqxx::work transaction{*connection};
                result = transaction.exec_prepared("get_process_data_execve", ppid, host_identifier);
                transaction.commit();

                ProcessEntry entry;

                if (result.empty())
                {
                    result = transaction.exec_prepared("get_process_data_non_execve", ppid, host_identifier);
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
                    long tmp_ppid = result.at(0)["ppid"].as<long>();

                    timestamp = result.at(0)["time"].as<long>();

                    entry.pid = ppid;
                    entry.ppid = tmp_ppid;
                    entry.cmdline = cmdline;
                    entry.path = path;
                    entry.cwd = result.at(0)["cwd"].as<std::string>();
                    ;
                    entry.syscall = result.at(0)["syscall"].as<std::string>();
                    entry.event_time = result.at(0)["time"].as<long long>();
                    entry.user_id = result.at(0)["uid"].as<long long>();
                    entry.group_id = result.at(0)["gid"].as<long long>();
                    entry.effective_group_id = result.at(0)["egid"].as<long long>();
                    entry.effective_user_id = result.at(0)["euid"].as<long long>();
                    entry.owner_user_id = result.at(0)["owner_uid"].as<long long>();
                    entry.owner_group_id = result.at(0)["owner_gid"].as<long long>();

                    ppid = tmp_ppid;

                    if (!result.at(0)["process_tree"].is_null())
                    {
                        std::string process_tree = result.at(0)["process_tree"].as<std::string>();
                        if (process_tree != "")
                        {
                            auto process_json = nlohmann::json::parse(process_tree);
                            auto parents_process_tree = JsonToProcessEntryList(process_json);
                            parents_process_tree.insert(parents_process_tree.begin(), entry);
                            return parents_process_tree;
                        }
                    }
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

            // std::vector<ProcessEntry> _tmp_process_list = snapshot_process_tree(connection, ppid, timestamp, uptime, host_identifier);
            // process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
        }

        connection->unprepare("get_process_data_from_id");
        connection->unprepare("get_process_data_execve");
        connection->unprepare("get_process_data_non_execve");

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

std::vector<ProcessEntry> print_user_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
    try
    {
        connection->prepare("get_user_data_from_id", "select * from user_events where id = $1");
        connection->prepare("get_process_data", "select * from process_events where pid = $1 "
                                                "and host_identifier = $2 and time <= $3 and path = $4"
                                                "order by id desc limit 1");

        std::vector<ProcessEntry> user_process_list;
        pqxx::result result;

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_user_data_from_id", record_id);
            transaction.commit();
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
                user_process_list = get_process_tree(connection, process_record_id);
            }
            // else
            // {
            //     /*
            //     * It seems that user event is coming from a process that was started *before* osquery was started.
            //     * We have to pick data from snapshot.
            //     *
            //     * Since we have PID, instead of PPID, we have to remove first entry in process list.
            //     */
            //     // user_process_list = snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);

            //     if (user_process_list.size() > 1)
            //         user_process_list.erase(user_process_list.begin());
            // }
        }

        connection->unprepare("get_user_data_from_id");
        connection->unprepare("get_process_data");

        return user_process_list;
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

std::vector<ProcessEntry> print_socket_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
    try
    {
        connection->prepare("get_socket_data_from_id", "select * from socket_events where id = $1");
        connection->prepare("get_process_data", "select * from process_events where pid = $1 "
                                                "and host_identifier = $2 and time <= $3 and path = $4"
                                                "order by id desc limit 1");

        std::vector<ProcessEntry> socket_process_list;
        pqxx::result result;

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_socket_data_from_id", record_id);
            transaction.commit();
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
                socket_process_list = get_process_tree(connection, process_record_id);
            }
            // else
            // {
            //     /*
            //     * It seems that socket event is coming from a process that was started *before* osquery was started.
            //     * We have to pick data from snapshot.
            //     *
            //     * Since we have PID, instead of PPID, we have to remove first entry in process list.
            //     */
            //     socket_process_list = snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);

            //     if (socket_process_list.size() > 1)
            //         socket_process_list.erase(socket_process_list.begin());
            // }
        }

        connection->unprepare("get_socket_data_from_id");
        connection->unprepare("get_process_data");

        return socket_process_list;
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

std::vector<BpfProcessEntry> get_bpf_process_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
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

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_bpf_process_data_from_id", record_id);
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
            if (pid <= ppid)
            {
                return std::vector<BpfProcessEntry>({});
            }

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

                    ppid = tmp_ppid;

                    if (!result.at(0)["process_tree"].is_null())
                    {
                        std::string process_tree = result.at(0)["process_tree"].as<std::string>();
                        if (process_tree != "")
                        {
                            auto process_json = nlohmann::json::parse(process_tree);
                            auto parents_process_tree = JsonToBpfProcessEntryList(process_json);
                            parents_process_tree.insert(parents_process_tree.begin(), entry);
                            return parents_process_tree;
                        }
                    }
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

            // std::vector<BpfProcessEntry> _tmp_process_list = bpf_snapshot_process_tree(connection, ppid, timestamp, uptime, host_identifier);
            // process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
        }

        connection->unprepare("get_bpf_process_data_from_id");
        connection->unprepare("get_bpf_process_data_execve");
        connection->unprepare("get_bpf_process_data_non_execve");
        connection->unprepare("get_docker_events");

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

std::vector<BpfProcessEntry> get_bpf_socket_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
{
    try
    {
        connection->prepare("get_socket_data_from_id", "select * from bpf_socket_events where id = $1");
        connection->prepare("get_process_data", "select * from bpf_process_events where pid = $1 "
                                                "and host_identifier = $2 and time <= $3 and path = $4"
                                                "order by id desc limit 1");

        std::vector<BpfProcessEntry> socket_process_list;
        pqxx::result result;

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_socket_data_from_id", record_id);
            transaction.commit();
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
                socket_process_list = get_bpf_process_tree(connection, process_record_id);
            }
            // else
            // {
            //     /*
            //     * It seems that socket event is coming from a process that was started *before* osquery was started.
            //     * We have to pick data from snapshot.
            //     *
            //     * Since we have PID, instead of PPID, we have to remove first entry in process list.
            //     */
            //     // socket_process_list = bpf_snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);

            //     if (socket_process_list.size() > 1)
            //         socket_process_list.erase(socket_process_list.begin());
            // }
        }

        connection->unprepare("get_socket_data_from_id");
        connection->unprepare("get_process_data");

        return socket_process_list;
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

        std::vector<BpfProcessEntry> file_process_list;
        pqxx::result result;

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_file_data_from_id", record_id);
            transaction.commit();
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
                file_process_list = get_bpf_process_tree(connection, process_record_id);
            }
            // else
            // {
            //     /*
            //     * It seems that socket event is coming from a process that was started *before* osquery was started.
            //     * We have to pick data from snapshot.
            //     *
            //     * Since we have PID, instead of PPID, we have to remove first entry in process list.
            //     */
            //     file_process_list = bpf_snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);

            //     if (file_process_list.size() > 1)
            //         file_process_list.erase(file_process_list.begin());
            // }
        }

        connection->unprepare("get_file_data_from_id");
        connection->unprepare("get_process_data");

        return file_process_list;
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

std::vector<WinProcessEntry> get_win_process_tree(std::shared_ptr<pqxx::connection> connection, unsigned long long record_id)
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
        // connection->prepare("get_docker_events", "select * from docker_events where status = 'start' and time between $1 and $2 and "
        //                                          "pid = $3 and host_identifier = $4");
        std::vector<WinProcessEntry> process_list;
        pqxx::result result, container_result;

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_win_process_data_from_id", record_id);
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
            // container_result = transaction.exec_prepared("get_docker_events", timestamp - 3600, timestamp + 3600, ppid, host_identifier);
            // transaction.commit();
            // if (!container_result.empty())
            // {
            //     container_pid = container_result.at(0)["pid"].as<long>();
            //     container_name = container_result.at(0)["container_name"].as<std::string>();
            //     container_id = container_result.at(0)["container_id"].as<std::string>();
            //     if (pid == container_pid or ppid == container_pid)
            //     {
            //         connection->prepare("update_container_process", "update win_process_events set is_container_process = '1' where id = $1");
            //         transaction.exec_prepared("update_container_process", record_id);
            //         transaction.commit();
            //         connection->unprepare("update_container_process");
            //     }
            // }
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
                    entry.root_pid = entry.pid;
                    ppid = tmp_ppid;
                    parent_process_guid = tmp_parent_process_guid;

                    if (!result.at(0)["process_tree"].is_null())
                    {
                        std::string process_tree = result.at(0)["process_tree"].as<std::string>();
                        if (process_tree != "")
                        {
                            auto process_json = nlohmann::json::parse(process_tree);
                            auto parents_process_tree = JsonToWinProcessEntryList(process_json);
                            parents_process_tree.insert(parents_process_tree.begin(), entry);
                            return parents_process_tree;
                        }
                    }
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

            // std::vector<WinProcessEntry> _tmp_process_list = win_snapshot_process_tree(connection, ppid, timestamp, uptime, host_identifier);
            // process_list.insert(process_list.end(), _tmp_process_list.begin(), _tmp_process_list.end());
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

        std::vector<WinProcessEntry> socket_process_list;
        pqxx::result result;

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_socket_data_from_id", record_id);
            transaction.commit();
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
                socket_process_list = get_win_process_tree(connection, process_record_id);
            }
            // else
            // {
            //     /*
            //     * It seems that socket event is coming from a process that was started *before* osquery was started.
            //     * We have to pick data from snapshot.
            //     *
            //     * Since we have PID, instead of PPID, we have to remove first entry in process list.
            //     */
            //     socket_process_list = win_snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);

            //     if (socket_process_list.size() > 1)
            //         socket_process_list.erase(socket_process_list.begin());
            // }
        }

        connection->unprepare("get_socket_data_from_id");
        connection->unprepare("get_process_data");

        return socket_process_list;
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

        std::vector<WinProcessEntry> file_process_list;
        pqxx::result result;

        {
            pqxx::work transaction{*connection};
            result = transaction.exec_prepared("get_file_data_from_id", record_id);
            transaction.commit();
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
                file_process_list = get_win_process_tree(connection, process_record_id);
            }
            // else
            // {
            //     /*
            //     * It seems that file event is coming from a process that was started *before* osquery was started.
            //     * We have to pick data from snapshot.
            //     *
            //     * Since we have PID, instead of PPID, we have to remove first entry in process list.
            //     */
            //     file_process_list = win_snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);

            //     if (file_process_list.size() > 1)
            //         file_process_list.erase(file_process_list.begin());
            // }
        }

        connection->unprepare("get_file_data_from_id");
        connection->unprepare("get_process_data");

        return file_process_list;
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

int main(int argc, char **argv)
{
    ConfigurationParser config_parser("config.json");
    if (!config_parser.parse())
    {
        return -1;
    }

    FleetServerConfiguration fleet = config_parser.GetFleetConfiguration();
    std::shared_ptr<ConnectionPool> connection_pool(new ConnectionPool(fleet.database.ConnectionString(), fleet.database_pool.maximum_connections));

    EventProcessor event_processor(connection_pool);
    event_processor.setProcessEventProcessor(get_process_tree);
    event_processor.setUserEventProcessor(print_user_tree);
    event_processor.setSocketEventProcessor(print_socket_tree);
    event_processor.setBPFProcessEventProcessor(get_bpf_process_tree);
    event_processor.setBPFSocketEventProcessor(get_bpf_socket_tree);
    event_processor.setBPFFileEventProcessor(get_bpf_file_tree);
    event_processor.setWinProcessEventProcessor(get_win_process_tree);
    event_processor.setWinSocketEventProcessor(get_win_socket_tree);
    event_processor.setWinFileEventProcessor(get_win_file_tree);
    event_processor.run();

    return 0;
}