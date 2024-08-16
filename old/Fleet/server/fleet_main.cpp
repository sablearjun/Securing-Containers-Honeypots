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
                                           "(host_identifier, unixTime, time, uptime, action, auid, family, fd, "
                                           "local_address, local_port, remote_address, remote_port, pid, path) "
                                           "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14)";
    prepared_queries["user_event_add"] = "insert into user_events "
                                         "(host_identifier, unixTime, time, uptime, auid, pid, type, uid, "
                                         "path, address, message, terminal) "
                                         "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";
    prepared_queries["process_file_events_add"] = "insert into process_file_events "
                                                  "(host_identifier, operation, pid, ppid, time, executable, partial, cwd, path, dest_path, uid, gid, auid, euid, egid, fsuid, fsgid, suid, sgid, uptime, eid)"
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
                                             "(host_identifier, hostname, unixTime, tid, pid, parent, uid, gid, cid, exit_code, probe_error, syscall, path, cwd, cmdline, duration, json_cmdline, ntime, time, eid, md5, sha1, sha256, is_container_process, container_age, container_name, uptime)"
                                             "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27)";
    prepared_queries["bpf_socket_events"] = "insert into bpf_socket_events "
                                            "(host_identifier, hostname, unixTime, tid, pid, parent, uid, gid, cid, exit_code, probe_error, syscall, path, fd, family, type, protocol, local_address, remote_address, local_port, remote_port, duration, ntime, time, eid, uptime)"
                                            "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26)";
    prepared_queries["bpf_file_events"] = "insert into bpf_file_events "
                                          "(host_identifier, hostname, unixTime, tid, pid, parent, uid, gid, cid, exit_code, probe_error, syscall, path, file_path, flags, ntime, duration, time, uptime)"
                                          "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19)";
    prepared_queries["docker_events"] = "insert into docker_events "
                                          "(host_identifier, hostname, action, from_column, status, scope, time, type, path, cmdline, pid, container_id, container_image, container_name, privileged, added_capabilities, dropped_capabilities, port_binding)"
                                          "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18)";

    prepared_queries["docker_details"] = "insert into docker_details "
                                          "(host_identifier, created, path, args, image, resolvconfpath, hostnamepath, hostspath, logpath, name, restartcount, pid, driver, platform, mountlabel, processlabel, apparmorprofile, execids, sizerw, sizerootfs)"
                                          "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20)";
    prepared_queries["docker_network"] = "insert into docker_network "
                                          "(host_identifier, bridge, sandboxid, hairpinmode, linklocalipv6address, linklocalipv6prefixlen, ports, sandboxkey, secondaryipaddresses, secondaryipv6addresses, pid)"
                                          "values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";

    prepared_queries["interface_addresses"] = "insert into interface_addresses "
                                              "(host_identifier, unixtime, interface, address, mask, broadcast, point_to_point, type)"
                                              "values ($1, $2, $3, $4, $5, $6, $7, $8)";
    prepared_queries["snapshot_result_add"] = "insert into snapshot_result "
                                              "(host_identifier, unixTime, query_name, result) "
                                              "values ($1, $2, $3, $4)";
    prepared_queries["query_result_add"] = "insert into query_result "
                                           "(host_identifier, unixTime, query_name, result) "
                                           "values ($1, $2, $3, $4)";
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
