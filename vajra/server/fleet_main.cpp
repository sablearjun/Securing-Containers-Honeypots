#include "WebServer.h"
#include "request_handler.h"
#include "RequestRouter.h"
#include "../3rdparty/json.hpp"
#include "../common/ConnectionPool.h"
#include "../common/ConfigurationParser.h"
#include "../common/logger.h"

#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>

#include <pqxx/pqxx>

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

HTTPMessage default_handler(std::string path, HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;

    std::stringstream ss;
    ss << "Requested path [" + path + "] does not exist.\n\nQuery parameters:\n";

    for (const auto &it : request.query)
    {
        ss << it.first << " : " << it.second << std::endl;
    }

    ss << "\nHeaders:\n";
    for (const auto &it : request.header)
    {
        ss << it.first << " : " << it.second << std::endl;
    }

    std::string reply = ss.str();
    response.status = boost::beast::http::status::not_found;
    response.body = reply;
    response.header["Content-Type"] = "text/plain";
    return response;
}

std::string enrollment_secret;

int main(int argc, char *argv[])
{
    std::unordered_map<std::string, std::string> prepared_queries;

    prepared_queries["validate_node_key"] = "select * from nodes where node_key = $1";
    prepared_queries["process_event_add"] = "insert into process_events "
                                            "(host_identifier, hostname, unixtime, atime, btime, ctime, mtime, time, uptime, "
                                            "auid, egid, euid, fsgid, fsuid, gid, mode, owner_gid, owner_uid, pid, "
                                            "ppid, sgid, suid, uid, cwd, cmdline, path, syscall) "
                                            "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, "
                                            "$15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27)";
    prepared_queries["socket_event_add"] = "insert into socket_events "
                                           "(host_identifier, hostname, unixTime, time, uptime, action, auid, family, fd, "
                                           "local_address, local_port, remote_address, remote_port, pid, path) "
                                           "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15)";
    prepared_queries["user_event_add"] = "insert into user_events "
                                         "(host_identifier, unixTime, time, uptime, auid, pid, type, uid, "
                                         "path, address, message, terminal) "
                                         "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";
    prepared_queries["process_file_events_add"] = "insert into process_file_events "
                                                  "(host_identifier, operation, pid, ppid, time, executable, partial, cwd, path, dest_path, uid, gid, auid, euid, egid, fsuid, fsgid, suid, sgid, uptime, eid)"
                                                  "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21)";
    prepared_queries["file_events"] = "insert into file_events "
                                                  "(host_identifier, hostname, unixTime, target_path, category, action, transaction_id, inode, uid, gid, mode, size, atime, mtime, ctime, md5, sha1, sha256, hashed, time, eid)"
                                                  "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21)";


    prepared_queries["process_envs_add"] = "insert into process_envs "
                                           "(host_identifier, pid, key, value, action)"
                                           "values ($1, $2, $3, $4, $5)";

    prepared_queries["process_open_pipes_add"] = "insert into process_open_pipes "
                                                 "(host_identifier, unixTime, pid, fd, mode, inode, type, partner_pid, partner_fd, partner_mode, action)"
                                                 "values ($1, $2, $3, $4, $5, $6, $7, $8,$9, $10, $11)";
    prepared_queries["process_open_files_add"] = "insert into process_open_files "
                                                 "(host_identifier, unixTime, pid, fd, path, action)"
                                                 "values ($1, $2, $3, $4, $5, $6)";
    prepared_queries["listening_ports_add"] = "insert into listening_ports "
                                              "(host_identifier, pid, port, protocol, family, address, fd, socket, path, net_namespace, action)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8,$9, $10, $11)";
    prepared_queries["routes_add"] = "insert into routes "
                                     "(host_identifier, destination, netmask, gateway, source, flags, interface, mtu, metric, type, hopcount, action)"
                                     "values ($1, $2, $3, $4, $5, $6, $7, $8,$9, $10, $11, $12)";
    prepared_queries["logged_in_users_add"] = "insert into logged_in_users "
                                              "(host_identifier, unixTime, type, logged_user, tty, host, time, pid, sid, registry_hive, action)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8,$9, $10, $11)";
    prepared_queries["arp_cache_add"] = "insert into arp_cache "
                                        "(host_identifier, address, mac, interface, permanent, action)"
                                        "values ($1, $2, $3, $4, $5, $6)";

    prepared_queries["suid_bin_add"] = "insert into suid_bin "
                                       "(host_identifier, unixTime, path, username, groupname, permissions, action)"
                                       "values ($1, $2, $3, $4, $5, $6, $7)";
    prepared_queries["process_open_sockets_add"] = "insert into process_open_sockets "
                                                   "(host_identifier, unixTime, pid, fd, socket, family, protocol, local_address, remote_address, local_port, remote_port, path, state, net_namespace, action)"
                                                   "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15)";
    prepared_queries["users_add"] = "insert into users "
                                    "(host_identifier, uid, gid, uid_signed, gid_signed, is_hidden, username, description, directory, shell, uuid, type, action)"
                                    "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)";
    prepared_queries["groups_add"] = "insert into groups "
                                     "(host_identifier, gid, gid_signed, is_hidden, groupname, group_sid, comment, action)"
                                     "values ($1, $2, $3, $4, $5, $6, $7, $8)";
    prepared_queries["cpu_time"] = "insert into cpu_time "
                                   "(host_identifier, unixTime, core, users, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice)"
                                   "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)";
    prepared_queries["memory_info"] = "insert into memory_info "
                                      "(host_identifier, unixTime, memory_total, memory_free, buffers, cached, swap_cached, active,  inactive, swap_total, swap_free)"
                                      "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";
    prepared_queries["bpf_process_events"] = "insert into bpf_process_events "
                                             "(host_identifier, hostname, unixTime, tid, pid, parent, uid, gid, cid, exit_code, probe_error, syscall, path, cwd, cmdline, duration, json_cmdline, ntime, time, eid, md5, sha1, sha256, is_container_process, container_age, container_name, uptime, is_processed, is_processed_rule_builder, is_processed_py)"
                                             "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, 'f', 'f', 'f')";
    prepared_queries["bpf_socket_events"] = "insert into bpf_socket_events "
                                            "(host_identifier, hostname, unixTime, tid, pid, parent, uid, gid, cid, exit_code, probe_error, syscall, path, fd, family, type, protocol, local_address, remote_address, local_port, remote_port, duration, ntime, time, eid, uptime, is_processed, is_processed_rule_builder, is_processed_py)"
                                            "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, 'f', 'f', 'f')";
    prepared_queries["bpf_file_events"] = "insert into bpf_file_events "
                                          "(host_identifier, hostname, unixTime, tid, pid, parent, uid, gid, cid, exit_code, probe_error, syscall, path, file_path, flags, ntime, duration, time, uptime, is_processed, is_processed_rule_builder, is_processed_py)"
                                          "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, 'f', 'f', 'f')";
    prepared_queries["docker_events"] = "insert into docker_events "
                                          "(host_identifier, hostname, action, from_column, status, scope, time, type, path, cmdline, pid, container_id, container_image, container_name, privileged, added_capabilities, dropped_capabilities, port_binding)"
                                          "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18)";
    prepared_queries["interface_addresses"] = "insert into interface_addresses "
                                              "(host_identifier, unixtime, interface, address, mask, broadcast, point_to_point, type)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8)";
    prepared_queries["win_process_events"] = "insert into win_process_events "
                                              "(host_identifier, hostname, unixTime, action, eid, pid, process_guid, path, cmdline, parent_pid, parent_process_guid, parent_path, owner_uid, time, uptime, utc_time, eventid, sha256, is_processed, is_processed_rule_builder, is_processed_py)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, 'f', 'f', 'f')";
    prepared_queries["win_socket_events"] = "insert into win_socket_events "
                                              "(host_identifier, hostname, unixTime, action, eid, time, utc_time, pid, process_guid, process_name, family, protocol, local_address, remote_address, local_port, remote_port, eventid, parent_pid, uptime, is_processed, is_processed_rule_builder, is_processed_py)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, 'f', 'f', 'f')";
    prepared_queries["win_file_events"] = "insert into win_file_events "
                                              "(host_identifier, hostname, unixTime, action, eid, target_path, md5, sha256, hashed, uid, utc_time, time, pe_file, pid, process_guid, process_name, eventid, parent_pid, uptime, is_processed, is_processed_rule_builder, is_processed_py)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, 'f', 'f', 'f')";
    prepared_queries["chrome_extensions"] = "insert into chrome_extensions "
                                              "(host_identifier, hostname, unixTime, browser_type, uid, name, profile, profile_path, referenced_identifier, identifier, version, description, default_locale, current_locale, update_url, author, persistent, path, permissions, optional_permissions, manifest_hash, referenced, from_webstore, state, install_time, install_timestamp, key, is_processed, is_processed_py)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, 'f', 'f')";
    prepared_queries["file"] = "insert into file "
                                              "(host_identifier, hostname, unixTime, path, directory, filename, inode, uid, gid, mode, device, size, block_size, atime, mtime, ctime, btime, hard_links, symlink, type, attributes, volume_serial, file_id, file_version, product_version, original_filename, bsd_flags, pid_with_namespace, mount_namespace_id, is_processed, is_processed_py)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, $28, $29, 'f', 'f')";
    prepared_queries["wmi_cli_event_consumers"] = "insert into wmi_cli_event_consumers "
                                              "(host_identifier, hostname, unixTime, name, command_line_template, executable_path, class, relative_path,  is_processed, is_processed_py)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, 'f', 'f')";
    prepared_queries["snapshot_result_add"] = "insert into snapshot_result "
                                              "(host_identifier, unixTime, query_name, result) "
                                              "values ($1, $2, $3, $4)";
    prepared_queries["query_result_add"] = "insert into query_result "
                                           "(host_identifier, unixTime, query_name, result) "
                                           "values ($1, $2, $3, $4)";
    prepared_queries["osquery_schedule"] = "insert into osquery_schedule "
                                            "(host_identifier, hostname, name, query, interval, executions, last_executed, denylisted, output_size, wall_time, wall_time_ms, last_wall_time_ms, user_time, last_user_time, system_time, last_system_time, average_memory, last_memory)"
                                            "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18)";
    prepared_queries["etc_hosts"] = "insert into etc_hosts "
                                      "(host_identifier, hostname, unixTime, address, hostnames, pid_with_namespace)"
                                      "values ($1, $2, $3, $4, $5, $6)";
    prepared_queries["uptime"] = "insert into uptime "
                                      "(host_identifier, hostname, unixTime, days, hours, minutes, seconds, total_seconds)"
                                      "values ($1, $2, $3, $4, $5, $6, $7, $8)";
    prepared_queries["startup_items"] = "insert into startup_items "
                                      "(host_identifier, hostname, unixTime, name, path, args, type, source, status, username)"
                                      "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";
    prepared_queries["patches"] = "insert into patches "
                                      "(host_identifier, hostname, unixTime, csname, hotfix_id, caption, description, fix_comments, installed_by, install_date, installed_on)"
                                      "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";
    prepared_queries["processes32"] = "insert into processes "
                                      "(host_identifier, hostname, unixTime, pid, name, path, cmdline, state, cwd, root, uid, gid, egid, suid, sgid, on_disk, wired_size, resident_size, total_size, user_time, system_time, disk_bytes_read, disk_bytes_written, start_time, parent, pgroup, threads, nice, elevated_token, secure_process, protection_type, virtual_process, elapsed_time, handle_count, percent_processor_time)"
                                      "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, $28, $29, $30, $31, $32, $33, $34, $35)";
    prepared_queries["programs"] = "insert into programs "
                                      "(host_identifier, hostname, unixTime, name, version, install_location, install_source, language, publisher, uninstall_string, install_date, identifying_number)"
                                      "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";
    prepared_queries["common_result_add"] = "insert into common_result "
                                            "(host_identifier, hostname, unixTime, query_name, query, query_result) "
                                            "values ($1, $2, $3, $4, $5, $6)";
    prepared_queries["kernel_info"] = "insert into kernel_info "
                                      "(host_identifier, hostname, unixTime, version, arguments, path, device)"
                                      "values ($1, $2, $3, $4, $5, $6, $7)";
    prepared_queries["deb_packages"] = "insert into deb_packages "
                                      "(host_identifier, hostname, unixTime, name, version, source, size, arch, revision, status, maintainer, section, priority, admindir, pid_with_namespace, mount_namespace_id)"
                                      "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16)";
    prepared_queries["rpm_packages"] = "insert into rpm_packages "
                                      "(host_identifier, hostname, unixTime, name, version, release, source, size, sha1, arch, epoch, install_time, vendor, package_group, pid_with_namespace, mount_namespace_id)"
                                      "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16)";
    prepared_queries["chocolatey_packages"] = "insert into chocolatey_packages "
                                              "(host_identifier, hostname, unixTime, name, version, summary, author, license, path)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9)";
    prepared_queries["interface_details"] = "insert into interface_details "
                                              "(host_identifier, hostname, unixTime, interface, mac, type, mtu, metric, flags, ipackets, opackets, ibytes, obytes, ierrors, oerrors, idrops, odrops, collisions, last_change, link_speed, pci_slot, friendly_name, description, manufacturer, connection_id, connection_status, enabled, physical_adapter, speed, service, dhcp_enabled, dhcp_lease_expires, dhcp_lease_obtained, dhcp_server, dns_domain, dns_domain_suffix_search_order, dns_host_name, dns_server_search_order)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, $28, $29, $30, $31, $32, $33, $34, $35, $36, $37, $38)";
    prepared_queries["ie_extensions"] = "insert into ie_extensions "
                                              "(host_identifier, hostname, unixtime, name, registry_path, version, path)"
                                              "values ($1, $2, $3, $4, $5, $6, $7)";
    prepared_queries["firefox_addons"] = "insert into firefox_addons "
                                              "(host_identifier, hostname, unixTime, uid, name, identifier, creator, type, version, description, source_url, visible, active, disabled, autoupdate, location, path)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17)";
    prepared_queries["npm_packages"] = "insert into npm_packages "
                                              "(host_identifier, hostname, unixTime, name, version, description, author, license, homepage, path, directory, pid_with_namespace, mount_namespace_id)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)";
    prepared_queries["portage_packages"] = "insert into portage_packages "
                                              "(host_identifier, hostname, unixTime, package, version, slot, build_time, repository, eapi, size, world)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";
    prepared_queries["get_scheduled_query"] = "select * from query_queue where host_identifier = $1 and sent = false";
    prepared_queries["stop_scheduled_query"] = "update query_queue set sent = true, send_time = $2 where id = $1";

    ConfigurationParser config_parser("config.json");
    if (!config_parser.parse())
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Cannot read configuration";
        return -1;
    }

    FleetServerConfiguration fleet = config_parser.GetFleetConfiguration();
    enrollment_secret = fleet.node.enrollment_secret;
    LogConfiguration log_config = config_parser.GetLogConfiguration("fleet");

    std::filesystem::path log_path = std::filesystem::path(log_config.log_directory) / log_config.log_file;

    init_logging(log_path.string(), log_config.log_level, log_config.log_size);
    // init_logging();

    BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Starting connection pool";
    std::shared_ptr<ConnectionPool> connection_pool(new ConnectionPool(fleet.database.ConnectionString(), fleet.database_pool.maximum_connections, prepared_queries));

    RequestRouter router;
    router["/enroll"].post([connection_pool](const HTTPMessage &req) -> HTTPMessage
                           {
                                auto conn = connection_pool->GetConnection();
                                auto response = enroll(req, conn);
                               connection_pool->ReturnConnection(conn);
                                return response; });
    router["/config"].post([connection_pool](const HTTPMessage &req) -> HTTPMessage
                           {
                               auto conn = connection_pool->GetConnection();
                               auto response = config(req, conn);
                               connection_pool->ReturnConnection(conn);
                               return response; });
    router["/log"].post([connection_pool](const HTTPMessage &req) -> HTTPMessage
                        {
                               auto conn = connection_pool->GetConnection();
                               auto response = logger(req, conn);
                               connection_pool->ReturnConnection(conn);
                               return response; });
    router["/read"].post([connection_pool](const HTTPMessage &req) -> HTTPMessage
                         {
                               auto conn = connection_pool->GetConnection();
                               auto response = distributed_read(req, conn);
                               connection_pool->ReturnConnection(conn);
                               return response; });
    router["/write"].post([connection_pool](const HTTPMessage &req) -> HTTPMessage
                          {
                               auto conn = connection_pool->GetConnection();
                               auto response = distributed_write(req, conn);
                               connection_pool->ReturnConnection(conn);
                               return response; });

    BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Initializing fleet HTTPS server";
    WebServer server(router, fleet.server.hostname, fleet.server.port);
    server.setTlsCertificates(fleet.ssl_configuration.ssl_certificate_file, fleet.ssl_configuration.ssl_private_key_file,
                              fleet.ssl_configuration.diffie_hellman_key_file, fleet.ssl_configuration.ssl_password);

    BOOST_LOG_TRIVIAL(info) << __SRCREF__ << "Fleet server initialization complete. Starting server";
    server.run();

    return 0;
}
