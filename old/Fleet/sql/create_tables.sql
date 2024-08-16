DO $$ DECLARE
r RECORD;
BEGIN
FOR r IN (SELECT tablename FROM pg_tables WHERE schemaname = current_schema()) LOOP
        EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident(r.tablename) || ' CASCADE';
END LOOP;
END $$;

CREATE TABLE nodes (
    id bigserial,
    config_id varchar (100),
    host_identifier varchar(40) PRIMARY KEY,
    os_arch varchar(15),
    os_build    varchar(15),
    os_major    varchar(5),
    os_minor    varchar(5),
    os_name varchar(20),
    os_platform varchar(20),
    osquery_version varchar(15),
    node_key    varchar(40) UNIQUE,
    node_invalid boolean,
    reenroll_allowed boolean,
    hardware_vendor varchar(40),
    hardware_model varchar(40),
    hardware_version varchar(40),
    hostname varchar(40),
    enrolled_on bigint,
    last_seen bigint,
    hardware_cpu_logical_core varchar (40), 
    hardware_cpu_type varchar (40), 
    hardware_physical_memory varchar(20)
);

CREATE TABLE process_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname varchar(40),
    unixTime bigint,
    atime  bigint,
    btime  bigint,
    ctime  bigint,
    mtime  bigint,
    time   bigint,
    uptime bigint,
    auid      bigint,
    egid      bigint,
    euid      bigint,
    fsgid     bigint,
    fsuid     bigint,
    gid       bigint,
    mode      bigint,
    owner_gid bigint,
    owner_uid bigint,
    pid       bigint,
    ppid      bigint,
    sgid      bigint,
    suid      bigint,
    uid       bigint,
    cwd varchar(300),
    cmdline varchar(2000),
    path    varchar(300),
    syscall varchar(20),
    process_tree varchar(10000000),
    is_processed boolean
);

CREATE TABLE socket_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    action varchar(20),
    time   bigint,
    uptime bigint,
    auid        bigint,
    family      bigint,
    fd          bigint,
    local_address varchar(100),
    local_port  bigint,
    remote_address varchar(100),
    remote_port bigint,
    pid       bigint,
    path    varchar(300),
    process_tree varchar(10000000),
    is_processed boolean
);

CREATE TABLE user_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    time   bigint,
    uptime bigint,
    auid        bigint,
    pid       bigint,
    type      bigint,
    uid       bigint,
    path    varchar(300),
    address varchar(100),
    message varchar(400),
    terminal varchar(50),
    process_tree varchar(10000000),
    is_processed boolean
);

CREATE TABLE query_result (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    query_name varchar(50),
    result varchar(10000000)
);

CREATE TABLE snapshot_result (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    query_name varchar(50),
    result varchar(10000000)
);

CREATE TABLE events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname varchar(40),
    unixTime bigint,
    event_code varchar (200),
    metadata varchar(10000000),
    is_alert boolean,
    is_open boolean,
    remarks varchar (10000),
    context_data varchar(10000000),
    severity varchar (10),
    alert_type varchar (20),
    technique varchar (80),
    technique_id varchar (30),
    link varchar (100),
    pid bigint
);

CREATE TABLE whitelists (
    id bigserial PRIMARY KEY,
    rule_code varchar(200),
    whitelist varchar(1000000)
);

CREATE TABLE query_queue (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    query varchar (1000),
    sent boolean,
    send_time bigint
);

CREATE TABLE config_queries (
    id bigserial PRIMARY KEY,
    query varchar (10000),
    query_name varchar (40)
);

CREATE TABLE api_users (
    username    varchar(40) PRIMARY KEY,
    secret      varchar(256),
    token       varchar(256),
    issuer      varchar(256),
    iat         bigint,
    exp         bigint
);

CREATE TABLE process_envs (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    pid bigint,
    key varchar (1000),
    value varchar (2000),
    action varchar(50)
);

CREATE TABLE process_open_pipes (
    id              bigserial PRIMARY KEY,
    host_identifier varchar(40),
    pid             bigint,                                                                                                                                                                                                                                                                                                                                                                                                                                       
    fd              bigint,
    mode            varchar(1000),
    inode           bigint,
    type            varchar(1000),
    partner_pid     bigint,
    partner_fd      bigint,
    partner_mode    varchar(1000),
    unixTime bigint,
    action varchar(50)
);

CREATE TABLE process_open_files (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    pid bigint,
    fd bigint,
    path varchar (1000),
    unixTime bigint,
    action varchar(50)
);

CREATE TABLE listening_ports (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    pid bigint,
    port bigint,
    protocol bigint,
    family bigint,
    address varchar (1000),
    fd bigint,
    socket bigint,
    path varchar(300),
    net_namespace varchar(1000),
    action varchar(50)
);

CREATE TABLE routes (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    destination varchar(40),
    netmask bigint,
    gateway varchar(40),
    source varchar(40),
    flags bigint,
    interface varchar(300),
    mtu bigint,
    metric bigint,
    type varchar(100),
    hopcount bigint,
    action varchar(50)
);

CREATE TABLE logged_in_users (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    type varchar(40),
    logged_user varchar(40),
    tty varchar(100),
    host varchar(80),
    time bigint,
    pid bigint,
    sid varchar(100),
    registry_hive varchar(100),
    unixTime bigint,
    action varchar(50)
);

CREATE TABLE arp_cache (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    address varchar(100),
    mac varchar(100),
    interface varchar(100),
    permanent varchar(100),
    action varchar(50)
);

CREATE TABLE process_file_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    operation varchar(100),
    pid bigint,
    ppid bigint,
    time bigint,
    executable varchar(300),
    partial varchar(100),
    cwd varchar(300),
    path varchar(300),
    dest_path varchar(300),
    uid varchar(300),
    gid varchar(300),
    auid varchar(300),
    euid varchar(300),
    egid varchar(300),
    fsuid varchar(300),
    fsgid varchar(300),
    suid varchar(300),
    sgid varchar(300),
    uptime bigint,
    eid varchar(300)    
);

CREATE TABLE suid_bin (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    path varchar(100),
    username varchar(100),
    groupname varchar(100),
    permissions varchar(100),
    unixTime bigint,
    action varchar(50)
);

CREATE TABLE process_open_sockets (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    pid bigint,
    fd bigint,
    socket bigint,
    family bigint,
    protocol bigint,
    local_address varchar(100),
    remote_address varchar(100),
    local_port bigint,
    remote_port bigint,
    path varchar(300),
    state varchar(100),
    net_namespace varchar(1000),
    unixTime bigint,
    action varchar(50)
);

CREATE TABLE users (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    uid	bigint,
    gid	bigint,
    uid_signed	bigint,
    gid_signed	bigint,
    username	text,
    description	text,
    directory	text,
    shell	text,
    uuid	text,
    type	text,
    is_hidden bigint,
    action varchar(50)
);

CREATE TABLE groups (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    gid	bigint,
    gid_signed	bigint,
    groupname	text,
    group_sid	text,
    comment	text,
    is_hidden	integer,
    action varchar(50)
);

CREATE TABLE cpu_time (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    core integer,
    users bigint,
    nice bigint,
    system bigint,
    idle bigint,
    iowait bigint,
    irq bigint,
    softirq bigint,
    steal bigint,
    guest bigint,
    guest_nice bigint
);

CREATE TABLE memory_info (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    memory_total bigint,
    memory_free bigint,
    buffers bigint,
    cached bigint,
    swap_cached bigint,
    active bigint,
    inactive bigint,
    swap_total bigint,
    swap_free bigint
);

CREATE TABLE bpf_process_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname varchar(40),
    unixTime bigint,
    tid bigint,
    pid bigint,
    parent  bigint,
    uid bigint,
    gid bigint,
    cid integer,
    exit_code text,
    probe_error integer,
    syscall text,
    path    text,
    cwd text,
    cmdline text,
    duration    integer,
    json_cmdline    text,
    ntime   text,
    time    bigint,
    eid integer,
    md5 text,
    sha1 text,
    sha256 text,
    is_container_process integer,
    container_age integer, 
    container_name text,
    process_tree varchar(10000000),
    uptime bigint,
    forward_process_tree varchar(10000000),
    is_processed boolean
);
 
CREATE TABLE bpf_socket_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname varchar(40),
    unixTime bigint,
    tid bigint,
    pid bigint,
    parent  bigint,
    uid bigint,
    gid bigint,
    cid integer,
    exit_code text,
    probe_error integer,
    syscall text,
    path    text,
    fd  text,
    family integer,
    type integer,
    protocol integer,
    local_address text,
    remote_address text,
    local_port integer,
    remote_port integer,
    duration    integer,
    ntime   text,
    time    bigint,
    eid integer,
    process_tree varchar(10000000),
    uptime bigint,
    is_processed boolean
);

CREATE TABLE bpf_file_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname varchar(40), 
    unixTime bigint,
    tid bigint,
    pid bigint,
    parent bigint,
    uid bigint,
    gid bigint,
    cid bigint,
    exit_code varchar(40),
    probe_error bigint,
    syscall varchar(40),
    path varchar(100),
    file_path varchar(100),
    flags text,
    ntime varchar(40),
    duration bigint,
    time bigint,
    process_tree varchar(10000000),
    uptime bigint,
    is_processed boolean
);

CREATE TABLE interface_addresses (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    interface text,
    address varchar(100),
    mask varchar(100),
    broadcast varchar  (60),
    point_to_point varchar (40),
    type text
);

CREATE TABLE docker_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname varchar(40), 
    action varchar(40), 
    from_column varchar(40), 
    status varchar(40), 
    scope varchar(40), 
    time varchar(40),
    type varchar(40), 
    path varchar(200), 
    cmdline varchar(500), 
    pid varchar(40), 
    container_id varchar(200),
    container_image varchar(200), 
    container_name varchar(200), 
    privileged varchar(40), 
    added_capabilities varchar(40), 
    dropped_capabilities varchar(40), 
    port_binding varchar(40)
);

CREATE TABLE docker_details (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    created varchar(40),
    path varchar(40), 
    args varchar(40), 
    image varchar(40), 
    resolvconfpath varchar(40), 
    hostnamepath varchar(40), 
    hostspath varchar(40),
    logpath varchar(40), 
    name varchar(200), 
    restartcount bigint, 
    pid bigint, 
    driver varchar(200),
    platform varchar(200), 
    mountlabel varchar(200), 
    processlabel varchar(40), 
    apparmorprofile varchar(40), 
    execids varchar(40), 
    sizerw bigint,
    sizerootfs bigint
);

CREATE TABLE docker_network (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    bridge varchar(40),
    sandboxid varchar(40), 
    hairpinmode boolean, 
    linklocalipv6address varchar(40), 
    linklocalipv6prefixlen bigint, 
    ports varchar(40), 
    sandboxkey varchar(40),
    secondaryipaddresses varchar(40), 
    secondaryipv6addresses varchar(200), 
    pid bigint
);


CREATE TABLE config_table_name (
    id bigserial PRIMARY KEY,
    table_name varchar (100)
);


insert into config_queries (query, query_name) values ('{"schedule":{"process_envs":{"interval":2,"query":"select * from process_envs", "snapshot": false},"process_open_pipes":{"interval":2,"query":"select * from process_open_pipes", "snapshot": false},"process_open_files":{"interval":2,"query":"select * from process_open_files", "snapshot": false},"listening_ports":{"interval":2,"query":"select * from listening_ports", "snapshot": false},"routes":{"interval":5,"query":"select * from routes", "snapshot": false},"logged_in_users":{"interval":5,"query":"select * from logged_in_users", "snapshot": false},"arp_cache":{"interval":5,"query":"select * from arp_cache", "snapshot": false},"suid_bin":{"interval":20,"query":"select * from suid_bin", "snapshot": false}, "process_open_sockets":{"interval":20,"query":"select * from process_open_sockets", "snapshot": false}, "users":{"interval":20,"query":"select * from users", "snapshot": false},"groups":{"interval":20,"query":"select * from groups", "snapshot": false}, "process_events":{"interval":10,"query":"select * from process_events", "snapshot": false}, "user_events":{"interval":10,"query":"select * from user_events", "snapshot": false}, "socket_events":{"interval":10,"query":"select * from socket_events", "snapshot": false}}},', 'all queries');

insert into config_queries (query, query_name) values ('{"schedule":{"process_open_pipes":{"interval":2,"query":"select * from process_open_pipes","snapshot":false}}}', 'process_open_pipes');
insert into config_queries (query, query_name) values ('{"schedule":{"process_open_files":{"interval":2,"query":"select * from process_open_files","snapshot":false}}}', 'process_open_files');
insert into config_queries (query, query_name) values ('{"schedule":{"listening_ports":{"interval":2,"query":"select * from listening_ports"}}}', 'listening_ports');
insert into config_queries (query, query_name) values ('{"schedule":{"routes":{"interval":5,"query":"select * from routes"}}}', 'routes');
insert into config_queries (query, query_name) values ('{"schedule":{"logged_in_users":{"interval":5,"query":"select * from logged_in_users"}}}', 'logged_in_users');
insert into config_queries (query, query_name) values ('{"schedule":{"arp_cache":{"interval":5,"query":"select * from arp_cache"}}}', 'arp_cache'); 
insert into config_queries (query, query_name) values ('{"schedule":{"suid_bin":{"interval":20,"query":"select * from suid_bin"}}}', 'suid_bin');
insert into config_queries (query, query_name) values ('{"schedule":{"process_open_sockets":{"interval":20,"query":"select * from process_open_sockets","snapshot":false}}}', 'process_open_sockets');
insert into config_queries (query, query_name) values ('{"schedule":{"users":{"interval":20,"query":"select * from users"}}}', 'users');
insert into config_queries (query, query_name) values ('{"schedule":{"groups":{"interval":20,"query":"select * from groups"}}}', 'groups');
insert into config_queries (query, query_name) values ('{"schedule":{"process_events":{"interval":10,"query":"select * from process_events","snapshot":false}}}', 'process_events');
insert into config_queries (query, query_name) values ('{"schedule":{"processes":{"interval":10,"query":"select * from processes","snapshot":true}}}', 'processes');
insert into config_queries (query, query_name) values ('{"schedule":{"user_events":{"interval":10,"query":"select * from user_events","snapshot":false}}}', 'user_events');
insert into config_queries (query, query_name) values ('{"schedule":{"process_file_events":{"interval":10,"query":"select * from process_file_events","snapshot":false}}}', 'process_file_events');
insert into config_queries (query, query_name) values ('{"schedule":{"socket_events":{"interval":10,"query":"select * from socket_events","snapshot":false}}}', 'socket_events');
insert into config_queries (query, query_name) values ('{"schedule":{"cpu_time":{"interval":2,"query":"select * from cpu_time"}}}', 'cpu_time');
insert into config_queries (query, query_name) values ('{"schedule":{"memory_info":{"interval":2,"query":"select * from memory_info"}}}', 'memory_info');
insert into config_queries (query, query_name) values ('{"schedule":{"process_envs":{"interval":2,"query":"select * from groups"}}}', 'groups');
insert into config_queries (query, query_name) values ('{"schedule":{"bpf_socket_events":{"interval":2,"query":"select * from bpf_socket_events", "snapshot": false}}}', 'bpf_socket_events');
insert into config_queries (query, query_name) values ('{"schedule":{"bpf_process_events":{"interval":2,"query":"select * from bpf_process_events", "snapshot": false}}}', 'bpf_process_events');
insert into config_queries (query, query_name) values ('{"schedule":{"bpf_file_events":{"interval":2,"query":"SELECT * FROM bpf_file_events;","snapshot":false}},"file_paths":{"etc":["/etc/%%"],"homes":["/root/.ssh/%%","/home/%/.ssh/%%"],"tmp":["/tmp/%%"]}}', 'bpf_file_events');
insert into config_queries (query, query_name) values ('{"schedule":{"interface_addresses":{"interval":2,"query":"select * from interface_addresses","snapshot": false}}}', 'interface_addresses');					
insert into config_queries (query, query_name) values ('{"schedule":{"docker_events":{"interval":2,"query":"select * from docker_events","snapshot": false}}}', 'docker_events');



insert into config_table_name (table_name) values ('process_open_pipes');
insert into config_table_name (table_name) values ('process_open_files');
insert into config_table_name (table_name) values ('listening_ports');
insert into config_table_name (table_name) values ('routes');
insert into config_table_name (table_name) values ('logged_in_users');
insert into config_table_name (table_name) values ('arp_cache');
insert into config_table_name (table_name) values ('suid_bin');
insert into config_table_name (table_name) values ('process_open_sockets');
insert into config_table_name (table_name) values ('users');
insert into config_table_name (table_name) values ('groups');
insert into config_table_name (table_name) values ('process_events');
insert into config_table_name (table_name) values ('user_events');
insert into config_table_name (table_name) values ('processes');
insert into config_table_name (table_name) values ('process_file_events');
insert into config_table_name (table_name) values ('socket_events');
insert into config_table_name (table_name) values ('cpu_time');
insert into config_table_name (table_name) values ('memory_info');
insert into config_table_name (table_name) values ('process_envs');
insert into config_table_name (table_name) values ('bpf_socket_events');
insert into config_table_name (table_name) values ('bpf_process_events');
insert into config_table_name (table_name) values ('bpf_file_events');
insert into config_table_name (table_name) values ('interface_addresses');
insert into config_table_name (table_name) values ('docker_events');

insert into config_table_name (table_name) values ('docker_events');