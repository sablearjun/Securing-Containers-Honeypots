DO $$ DECLARE
r RECORD;
BEGIN
FOR r IN (SELECT tablename FROM pg_tables WHERE schemaname = current_schema()) LOOP
        EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident(r.tablename) || ' CASCADE';
END LOOP;
END $$;

CREATE DATABASE nodejs_users;

CREATE TABLE nodes (
    id bigserial,
    config_id TEXT,
    host_identifier varchar(40) PRIMARY KEY,
    os_arch TEXT,
    os_build    TEXT,
    os_major    TEXT,
    os_minor    TEXT,
    os_name TEXT,
    os_platform TEXT,
    osquery_version TEXT,
    node_key    varchar(40) UNIQUE,
    node_invalid boolean,
    reenroll_allowed boolean,
    hardware_vendor TEXT,
    hardware_model TEXT,
    hardware_version TEXT,
    hostname TEXT,
    enrolled_on bigint,
    last_seen bigint,
    hardware_cpu_logical_core TEXT, 
    hardware_cpu_type TEXT, 
    hardware_physical_memory TEXT,
    isolated boolean,
    kernel_version TEXT
);

CREATE TABLE process_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
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
    cwd TEXT,
    cmdline TEXT,
    path    TEXT,
    syscall TEXT,
    process_tree TEXT,
    is_processed boolean
);

CREATE TABLE socket_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    action TEXT,
    time   bigint,
    uptime bigint,
    auid        bigint,
    status      TEXT,
    family      bigint,
    fd          bigint,
    local_address TEXT,
    local_port  bigint,
    remote_address TEXT,
    remote_port bigint,
    socket      TEXT,
    pid       bigint,
    path    TEXT,
    eid     TEXT,
    success integer,
    process_tree TEXT,
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
    path    TEXT,
    address TEXT,
    message TEXT,
    terminal TEXT,
    process_tree TEXT,
    is_processed boolean
);

CREATE TABLE query_result (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    query_name TEXT,
    result TEXT
);

CREATE TABLE snapshot_result (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    query_name TEXT,
    result TEXT
);

CREATE TABLE alerts (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    event_code TEXT,
    metadata TEXT,
    is_alert boolean,
    is_open boolean,
    remarks TEXT,
    context_data TEXT,
    severity TEXT,
    alert_type TEXT,
    technique TEXT,
    technique_id TEXT,
    link TEXT,
    pid bigint,
    event_id bigint,
    action varchar (5),
    os_name TEXT
);

CREATE TABLE whitelists (
    id bigserial PRIMARY KEY,
    rule_code TEXT,
    whitelist TEXT
);

CREATE TABLE query_queue (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    query TEXT,
    sent boolean,
    send_time bigint
);

CREATE TABLE config_queries (
    id bigserial PRIMARY KEY,
    query TEXT,
    query_name TEXT,
    os_name TEXT,
    kernel_version TEXT,
    os_arch TEXT
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
    key TEXT,
    value TEXT,
    action TEXT
);

CREATE TABLE process_open_pipes (
    id              bigserial PRIMARY KEY,
    host_identifier varchar(40),
    pid             bigint,                                                                                                                                                                                                                                                                                                                                                                                                                                       
    fd              bigint,
    mode            TEXT,
    inode           bigint,
    type            TEXT,
    partner_pid     bigint,
    partner_fd      bigint,
    partner_mode    TEXT,
    unixTime bigint,
    action TEXT
);

CREATE TABLE process_open_files (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    pid bigint,
    fd bigint,
    path TEXT,
    unixTime bigint,
    action TEXT
);

CREATE TABLE listening_ports (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    pid bigint,
    port bigint,
    protocol bigint,
    family bigint,
    address TEXT,
    fd bigint,
    socket bigint,
    path TEXT,
    net_namespace TEXT,
    action TEXT
);

CREATE TABLE routes (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    destination TEXT,
    netmask bigint,
    gateway TEXT,
    source TEXT,
    flags bigint,
    interface TEXT,
    mtu bigint,
    metric bigint,
    type TEXT,
    hopcount bigint,
    action TEXT
);

CREATE TABLE logged_in_users (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    type TEXT,
    logged_user TEXT,
    tty TEXT,
    host TEXT,
    time bigint,
    pid bigint,
    sid TEXT,
    registry_hive TEXT,
    unixTime bigint,
    action TEXT
);

CREATE TABLE arp_cache (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    address TEXT,
    mac TEXT,
    interface TEXT,
    permanent TEXT,
    action TEXT
);

CREATE TABLE process_file_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    operation TEXT,
    pid bigint,
    ppid bigint,
    time bigint,
    executable TEXT,
    partial TEXT,
    cwd TEXT,
    path TEXT,
    dest_path TEXT,
    uid TEXT,
    gid TEXT,
    auid TEXT,
    euid TEXT,
    egid TEXT,
    fsuid TEXT,
    fsgid TEXT,
    suid TEXT,
    sgid TEXT,
    uptime bigint,
    eid TEXT    
);

CREATE TABLE suid_bin (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    path TEXT,
    username TEXT,
    groupname TEXT,
    permissions TEXT,
    unixTime bigint,
    action TEXT
);

CREATE TABLE process_open_sockets (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    pid bigint,
    fd bigint,
    socket bigint,
    family bigint,
    protocol bigint,
    local_address TEXT,
    remote_address TEXT,
    local_port bigint,
    remote_port bigint,
    path TEXT,
    state TEXT,
    net_namespace TEXT,
    unixTime bigint,
    action TEXT
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
    action TEXT
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
    action TEXT
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
    hostname TEXT,
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
    process_tree TEXT,
    uptime bigint,
    is_processed boolean,
    is_processed_py boolean,
    is_processed_rule_builder boolean,
    is_processed_tree boolean
);
 
CREATE TABLE bpf_socket_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
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
    process_tree TEXT,
    uptime bigint,
    is_processed boolean,
    is_processed_py boolean,
    is_processed_rule_builder boolean
);

CREATE TABLE bpf_file_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT, 
    unixTime bigint,
    tid bigint,
    pid bigint,
    parent bigint,
    uid bigint,
    gid bigint,
    cid bigint,
    exit_code TEXT,
    probe_error integer,
    syscall TEXT,
    path TEXT,
    file_path TEXT,
    flags text,
    ntime TEXT,
    duration bigint,
    time bigint,
    process_tree TEXT,
    uptime bigint,
    is_processed boolean,
    is_processed_py boolean,
    is_processed_rule_builder boolean
);

CREATE TABLE interface_addresses (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    unixTime bigint,
    interface text,
    address TEXT,
    mask TEXT,
    broadcast TEXT,
    point_to_point TEXT,
    type text
);

CREATE TABLE docker_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT, 
    action TEXT, 
    from_column TEXT, 
    status TEXT, 
    scope TEXT, 
    time TEXT,
    type TEXT, 
    path TEXT, 
    cmdline TEXT, 
    pid TEXT, 
    container_id TEXT,
    container_image TEXT, 
    container_name TEXT, 
    privileged TEXT, 
    added_capabilities TEXT, 
    dropped_capabilities TEXT, 
    port_binding TEXT
);


CREATE TABLE win_process_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    action TEXT,
    eid TEXT,
    pid bigint,
    process_guid TEXT,
    path text,
    cmdline text,
    parent_pid bigint,
    parent_process_guid TEXT,
    parent_path text,
    owner_uid TEXT,
    time bigint,
    utc_time TEXT,
    eventid integer,
    sha256 TEXT,
    process_tree text,
    uptime bigint,
    is_processed boolean,
    is_processed_py boolean,
    is_processed_rule_builder boolean
);

CREATE TABLE win_socket_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    action text,
    eid text,
    time TEXT,
    utc_time text,
    pid bigint,
    process_guid text,
    process_name text,
    family text,
    protocol integer,
    local_address text,
    remote_address text,
    local_port integer,
    remote_port integer,
    eventid integer,
    process_tree text,
    parent_pid bigint,
    uptime bigint,
    is_processed boolean,
    is_processed_py boolean,
    is_processed_rule_builder boolean
);

CREATE TABLE win_file_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    action TEXT,
    eid text,
    target_path text,
    md5 text,
    sha256 text,
    hashed integer,
    uid text,
    utc_time text,
    time bigint,
    pe_file text,
    pid bigint,
    process_guid text,
    process_name text,
    eventid integer,
    process_tree text,
    parent_pid bigint,
    uptime bigint,
    is_processed boolean,
    is_processed_py boolean,
    is_processed_rule_builder boolean
);

CREATE TABLE config_table_name (
    id bigserial PRIMARY KEY,
    table_name TEXT,
    os_name TEXT,
    os_arch TEXT,
    kernel_version TEXT
);

CREATE TABLE python_rules (
    id bigserial PRIMARY KEY,
    name text,
    rule text,
    severity integer,
    technique TEXT,
    technique_id TEXT,
    link text,
    comment text,
    os_name text,
    created bigint,
    last_modified bigint,
    rule_written_by text
);

CREATE TABLE rule_builder_rules (
    id bigserial PRIMARY KEY,
    name text,
    description TEXT,
    link TEXT,
    platform TEXT,
    severity integer,
    tactics TEXT,
    technique_id TEXT,
    type TEXT,
    rule_written_by TEXT,
    alerters TEXT,
    rule_builder text,
    created bigint,
    last_modified bigint
);

CREATE TABLE chrome_extensions (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    browser_type text,
    uid text,
    name text,
    profile text,
    profile_path text,
    referenced_identifier text,
    identifier text,
    version text,
    description text,
    default_locale text,
    current_locale text,
    update_url text,
    author text,
    persistent integer,
    path text,
    permissions text,
    permissions_json text,
    optional_permissions text,
    manifest_hash text,
    referenced bigint,
    from_webstore text,
    state text,
    install_time text,
    install_timestamp bigint,
    key text,
    is_processed boolean,
    is_processed_py boolean
);

CREATE TABLE file (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    path text,
    directory text,
    filename text,
    inode bigint,
    uid bigint,
    gid bigint,
    mode text,
    device bigint,
    size bigint,
    block_size integer,
    atime bigint,
    mtime bigint,
    ctime bigint,
    btime bigint,
    hard_links integer,
    symlink integer,
    type text,
    attributes text,
    volume_serial text,
    file_id text,
    file_version text,
    product_version text,
    original_filename text,
    bsd_flags text,
    pid_with_namespace integer,
    mount_namespace_id text,
    is_processed boolean,
    is_processed_py boolean
);


CREATE TABLE wmi_cli_event_consumers (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    name text,
    command_line_template text,
    executable_path text,
    class text,
    relative_path text,
    is_processed boolean,
    is_processed_py boolean
);

CREATE TABLE osquery_schedule(
    host_identifier TEXT, 
    hostname TEXT, 
    name TEXT, 
    query TEXT,
    interval bigint, 
    executions bigint, 
    last_executed bigint, 
    denylisted bigint, 
    output_size bigint, 
    wall_time bigint, 
    wall_time_ms bigint, 
    last_wall_time_ms bigint, 
    user_time bigint, 
    last_user_time bigint, 
    system_time bigint, 
    last_system_time bigint, 
    average_memory bigint, 
    last_memory bigint
);

CREATE TABLE etc_hosts (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    address TEXT,
    hostnames TEXT,
    pid_with_namespace integer
);

CREATE TABLE uptime (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    days integer,
    hours integer,
    minutes integer,
    seconds integer,
    total_seconds bigint
);

CREATE TABLE startup_items (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    name TEXT,
    path TEXT,
    args TEXT,
    type TEXT,
    source TEXT,
    status TEXT,
    username TEXT
);

CREATE TABLE patches (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    csname TEXT,
    hotfix_id TEXT,
    caption TEXT,
    description TEXT,
    fix_comments TEXT,
    installed_by TEXT,
    install_date TEXT,
    installed_on TEXT
);

CREATE TABLE processes (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    pid bigint, 
    name TEXT,
    path TEXT, 
    cmdline TEXT, 
    state TEXT, 
    cwd TEXT, 
    root TEXT, 
    uid bigint, 
    gid bigint, 
    egid bigint, 
    suid bigint, 
    sgid bigint, 
    on_disk integer, 
    wired_size bigint,
    resident_size bigint, 
    total_size bigint, 
    user_time bigint, 
    system_time bigint, 
    disk_bytes_read bigint, 
    disk_bytes_written bigint, 
    start_time bigint, 
    parent bigint, 
    pgroup bigint, 
    threads integer, 
    nice integer, 
    elevated_token integer, 
    secure_process integer, 
    protection_type TEXT,
    virtual_process integer,
    elapsed_time bigint, 
    handle_count bigint, 
    percent_processor_time bigint,
    is_processed boolean,
    is_processed_py boolean
);

CREATE TABLE programs (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    name TEXT,
    version TEXT,
    install_location TEXT,
    install_source TEXT,
    language TEXT,
    publisher TEXT,
    uninstall_string TEXT,
    install_date TEXT,
    identifying_number TEXT,
    cpe_string TEXT,
    cve_match TEXT,
    is_processed boolean,
    is_cve_processed boolean
);

CREATE TABLE file_events (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    target_path TEXT,
    category TEXT,
    action TEXT,
    transaction_id bigint,
    inode bigint,
    uid bigint,
    gid bigint,
    mode TEXT,
    size bigint,
    atime bigint,
    mtime bigint,
    ctime bigint,
    md5 TEXT,
    sha1 TEXT,
    sha256 TEXT,
    hashed integer,
    time bigint,
    eid TEXT
);

CREATE TABLE rules(
    id bigserial PRIMARY KEY,
    tactic TEXT,
    technique TEXT,
    technique_id TEXT,
    link TEXT,
    severity TEXT,
    os_name TEXT,
    query TEXT
);

CREATE TABLE kernel_info(
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    version TEXT,
    arguments TEXT,
    path TEXT,
    device TEXT
);

CREATE TABLE deb_packages(
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    name TEXT,
    version TEXT,
    source TEXT,
    size bigint,
    arch TEXT,
    revision TEXT,
    status TEXT,
    maintainer TEXT,
    section	TEXT,
    priority TEXT,
    admindir TEXT,
    pid_with_namespace integer,
    mount_namespace_id TEXT
);

CREATE TABLE rpm_packages(
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    name TEXT,
    version TEXT,
    release TEXT,
    source TEXT,
    size bigint,
    sha1 TEXT,
    arch TEXT,
    epoch integer,
    install_time integer,
    vendor TEXT,
    package_group TEXT,
    pid_with_namespace integer,
    mount_namespace_id TEXT
);

CREATE TABLE chocolatey_packages(
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    name TEXT,
    version TEXT,
    summary TEXT,
    author TEXT,
    license bigint,
    path TEXT
);

CREATE TABLE interface_details (
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime bigint,
    interface TEXT,
    mac TEXT,
    type integer,
    mtu integer,
    metric integer,
    flags integer,
    ipackets bigint,
    opackets bigint,
    ibytes bigint,
    obytes bigint,
    ierrors bigint,
    oerrors bigint,
    idrops bigint,
    odrops bigint,
    collisions bigint,
    last_change bigint,
    link_speed bigint,
    pci_slot TEXT,
    friendly_name TEXT,
    description TEXT,
    manufacturer TEXT,
    connection_id TEXT,
    connection_status TEXT,
    enabled integer,
    physical_adapter integer,
    speed integer,
    service TEXT,
    dhcp_enabled integer,
    dhcp_lease_expires TEXT,
    dhcp_lease_obtained TEXT,
    dhcp_server TEXT,
    dns_domain TEXT,
    dns_domain_suffix_search_order TEXT,
    dns_host_name TEXT,
    dns_server_search_order TEXT
);

CREATE TABLE nvd_cves(
    id bigserial PRIMARY KEY,
    cve_id TEXT,
    published TEXT,
    lastModified TEXT,
    vulnStatus TEXT,
    descriptions TEXT,
    metrics TEXT,
    weaknesses TEXT,
    configurations TEXT,
    reference TEXT,
    cpe_string TEXT,
    baseScore bigint,
    baseSeverity TEXT,
    exploitabilityScore bigint,
    impactScore bigint
);

CREATE TABLE ie_extensions(
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    name TEXT,
    registry_path TEXT,
    version TEXT,
    path TEXT,
    cpe_string TEXT,
    cve_match TEXT,
    is_processed boolean,
    is_cve_processed boolean
);

CREATE TABLE firefox_addons(
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    uid bigint,
    name TEXT,
    identifier TEXT,
    creator TEXT,
    type TEXT,
    version TEXT,
    description TEXT,
    source_url	TEXT,
    visible	integer,
    active integer,
    disabled integer,
    autoupdate integer,
    location TEXT,
    path TEXT,
    cpe_string TEXT,
    cve_match TEXT,
    is_processed boolean,
    is_cve_processed boolean
);

CREATE TABLE npm_packages(
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    name TEXT,
    version TEXT,
    description TEXT,
    author TEXT,
    license	TEXT,
    homepage TEXT,
    path TEXT,
    directory TEXT,
    pid_with_namespace integer,
    mount_namespace_id TEXT,
    cpe_string TEXT,
    cve_match TEXT,
    is_processed boolean,
    is_cve_processed boolean
);

CREATE TABLE portage_packages(
    id bigserial PRIMARY KEY,
    host_identifier varchar(40),
    hostname TEXT,
    unixTime TEXT,
    package TEXT,
    version TEXT,
    slot TEXT,
    build_time bigint,
    repository	TEXT,
    eapi bigint,
    size bigint,
    world integer,
    cpe_string TEXT,
    cve_match TEXT,
    is_processed boolean,
    is_cve_processed boolean
);

-- CREATE TABLE common_result (
--     id bigserial PRIMARY KEY,
--     host_identifier varchar(40),
--     hostname TEXT,
--     unixTime bigint,
--     query_name TEXT,
--     query TEXT,
--     query_result TEXT
-- );

CREATE TABLE data_retention_settings(
    days bigint
); 
----------------CONFIG QUERIES------------------------------------------------------------
----------------Add new insert query at the bottom.

-- insert into config_queries (query, query_name) values ('{"schedule":{"process_envs":{"interval":2,"query":"select * from process_envs", "snapshot": false},"process_open_pipes":{"interval":2,"query":"select * from process_open_pipes", "snapshot": false},"process_open_files":{"interval":2,"query":"select * from process_open_files", "snapshot": false},"listening_ports":{"interval":2,"query":"select * from listening_ports", "snapshot": false},"routes":{"interval":5,"query":"select * from routes", "snapshot": false},"logged_in_users":{"interval":5,"query":"select * from logged_in_users", "snapshot": false},"arp_cache":{"interval":5,"query":"select * from arp_cache", "snapshot": false},"suid_bin":{"interval":20,"query":"select * from suid_bin", "snapshot": false}, "process_open_sockets":{"interval":20,"query":"select * from process_open_sockets", "snapshot": false}, "users":{"interval":20,"query":"select * from users", "snapshot": false},"groups":{"interval":20,"query":"select * from groups", "snapshot": false}, "process_events":{"interval":10,"query":"select * from process_events", "snapshot": false}, "user_events":{"interval":10,"query":"select * from user_events", "snapshot": false}, "socket_events":{"interval":10,"query":"select * from socket_events", "snapshot": false}}},', 'all queries');

insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"process_open_pipes":{"interval":240,"query":"select * from process_open_pipes","snapshot":false}}}', 'process_open_pipes', 'Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"process_open_files":{"interval":240,"query":"select * from process_open_files","snapshot":false}}}', 'process_open_files','Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"listening_ports":{"interval":3600,"query":"select * from listening_ports"}}}', 'listening_ports','Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"routes":{"interval":3600,"query":"select * from routes"}}}', 'routes','Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"logged_in_users":{"interval":3600,"query":"select * from logged_in_users"}}}', 'logged_in_users', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"arp_cache":{"interval":86400,"query":"select * from arp_cache"}}}', 'arp_cache', 'Linux, Windows', '', ''); 
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"suid_bin":{"interval":3600,"query":"select * from suid_bin"}}}', 'suid_bin', 'Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"process_open_sockets":{"interval":600,"query":"select * from process_open_sockets","snapshot":false}}}', 'process_open_sockets', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"users":{"interval":86400,"query":"select * from users"}}}', 'users', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"groups":{"interval":3600,"query":"select * from groups"}}}', 'groups', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"process_events":{"interval":30,"query":"select * from process_events","snapshot":false}}}', 'process_events', 'Linux', '<4.18', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"processes":{"interval":3600,"query":"select * from processes","snapshot":true}}}', 'processes', 'Linux, Windows', '', '64');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"user_events":{"interval":240,"query":"select * from user_events","snapshot":false}}}', 'user_events', 'Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"process_file_events":{"interval":240,"query":"select * from process_file_events","snapshot":false}},"file_paths":{"etc":["/etc/%%"],"homes":["/root/.ssh/%%","/home/%/.ssh/%%"],"tmp":["/tmp/%%"]}}', 'process_file_events', 'Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"file_events":{"interval":240,"query":"select * from file_events","snapshot":false}},"file_paths":{"etc":["/etc/%%"],"homes":["/root/.ssh/%%","/home/%/.ssh/%%"],"tmp":["/tmp/%%"]},"file_accesses": ["homes", "etc"]}', 'file_events','Linux', '<4.18', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"socket_events":{"interval":30,"query":"select * from socket_events","snapshot":false}}}', 'socket_events', 'Linux', '<4.18', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"cpu_time":{"interval":30,"query":"select * from cpu_time"}}}', 'cpu_time', 'Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"memory_info":{"interval":30,"query":"select * from memory_info"}}}', 'memory_info', 'Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"process_envs":{"interval":240,"query":"select * from process_envs"}}}', 'process_envs', 'Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"bpf_socket_events":{"interval":30,"query":"select * from bpf_socket_events", "snapshot": false}}}', 'bpf_socket_events', 'Linux', '>4.17', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"bpf_process_events":{"interval":30,"query":"SELECT * FROM bpf_process_events where path not like ''%snap%'' and path not like ''%code%'' and path not like ''%postgres%'' and path not like ''%chrome%'';","snapshot": false}}}', 'bpf_process_events', 'Linux', '>4.17', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"bpf_file_events":{"interval":30,"query":"SELECT * FROM bpf_file_events;","snapshot":false}},"file_paths":{"etc":["/etc/%%"],"homes":["/root/.ssh/%%","/home/%/.ssh/%%"],"tmp":["/tmp/%%"]}}', 'bpf_file_events', 'Linux', '>4.17', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"interface_addresses":{"interval":30,"query":"select * from interface_addresses","snapshot": false}}}', 'interface_addresses', 'Linux, Windows', '', '');					
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"docker_events":{"interval":30,"query":"select * from docker_events","snapshot": false}}}', 'docker_events', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"win_process_events":{"interval":30,"query":"select * from win_process_events","snapshot": false}}}', 'win_process_events', 'Windows', '', '64');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"win_file_events":{"interval":30,"query":"select * from win_file_events","snapshot": false}}}', 'win_file_events', 'Windows', '', '64');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"win_socket_events":{"interval":30,"query":"select * from win_socket_events","snapshot": false}}}', 'win_socket_events', 'Windows', '', '64');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"chrome_extensions":{"interval":86400,"query":"SELECT * FROM chrome_extensions WHERE chrome_extensions.uid IN (SELECT uid FROM users);","snapshot": false}}}', 'chrome_extensions', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"file":{"interval":30,"query":"select * from file where (path like \"A:%\" or path like \"B:%\" or path like \"D:%\" or path like \"E:%\" or path like \"F:%\" or path like \"G:%\" or path like \"H:%\" or path like \"I:%\" or path like \"J:%\" or path like \"K:%\" or path like \"L:%\" or path like \"M:%\" or path like \"N:%\" or path like \"O:%\" or path like \"P:%\" or path like \"Q:%\" or path like \"R:%\" or path like \"S:%\" or path like \"T:%\" or path like \"U:%\" or path like \"V:%\" or path like \"W:%\" or path like \"X:%\" or path like \"Y:%\" or path like \"Z:%\") or (path like \"/media/%%\" or path like \"/run/media/%%\" or path like \"/mnt/%%\")","snapshot":false}}}', 'file', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"etc_hosts":{"interval":30,"query":"select * from etc_hosts"}}}', 'etc_hosts', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"uptime":{"interval":30,"query":"select * from uptime"}}}', 'uptime', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"startup_items":{"interval":30,"query":"select * from startup_items"}}}', 'startup_items', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"patches":{"interval":30,"query":"select * from patches"}}}', 'patches', 'Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"processes32":{"interval":30,"query":"select * from processes","snapshot":true}}}', 'processes32', 'Windows', '', '32');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"programs":{"interval":60,"query":"select * from programs","snapshot":false}}}', 'programs', 'Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"kernel_info":{"interval":84600,"query":"select * from kernel_info"}}}', 'kernel_info', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"deb_packages":{"interval":30,"query":"select * from deb_packages"}}}', 'deb_packages', 'Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"rpm_packages":{"interval":30,"query":"select * from rpm_packages"}}}', 'rpm_packages', 'Linux', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"chocolatey_packages":{"interval":30,"query":"select * from chocolatey_packages"}}}', 'chocolatey_packages', 'Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"interface_details":{"interval":30,"query":"select * from interface_details"}}}', 'interface_details', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"ie_extensions":{"interval":30,"query":"select * from ie_extensions"}}}', 'ie_extensions', 'Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"firefox_addons":{"interval":30,"query":"SELECT * FROM firefox_addons WHERE firefox_addons.uid IN (SELECT uid FROM users)"}}}', 'firefox_addons', 'Linux, Windows', '', '');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"npm_packages":{"interval":30,"query":"select * from npm_packages"}}}', 'npm_packages', 'Linux, Windows', '', '');


-- insert into config_queries (query, query_name) values ('{"schedule":{"common":{"interval":60,"query":"select * from kernel_info","snapshot":false}}}', 'common');

insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"win_include_paths":{"all_files":["*"]},"plgx_event_filters":{"win_ssl_events":{"process_name":{"exclude":{"values":["*\\Program Files\\plgx_osquery\\plgx_*"]}}},"win_file_events":{"target_path":{"exclude":{"values":["C:\\Windows\\system32\\DriverStore\\Temp\\*","C:\\Windows\\system32\\wbem\\Performance*","C:\\$WINDOWS.~BT\\Sources\\*","C:\\Windows\\Installer\\*","*WRITABLE.TST","C:\\Windows\\System32\\Tasks\\Adobe Acrobat Update Task*","C:\\Windows\\System32\\Tasks\\Adobe Flash Player Updater*","C:\\Windows\\System32\\Tasks\\OfficeSoftwareProtectionPlatform\\SvcRestartTask*","C:\\Program Files\\osquery\\*"]},"include":{"values":["*\\Start Menu*","*\\Startup\\*","*\\Content.Outlook\\*","*\\Downloads\\*","*.application","*.appref-ms","*.bat","*.chm","*.cmd","*.cmdline","*.docm","*.exe","*.jar","*.jnlp","*.jse","*.hta","*.pptm","*.ps1","*.sys","*.scr","*.vbe","*.vbs","*.xlsm","*.proj","*.sln","C:\\Users\\Default*","C:\\Windows\\system32\\Drivers*","C:\\Windows\\SysWOW64\\Drivers*","C:\\Windows\\system32\\GroupPolicy\\Machine\\Scripts*","C:\\Windows\\system32\\GroupPolicy\\User\\Scripts*","C:\\Windows\\system32\\Wbem*","C:\\Windows\\SysWOW64\\Wbem*","C:\\Windows\\system32\\WindowsPowerShell*","C:\\Windows\\SysWOW64\\WindowsPowerShell*","C:\\Windows\\Tasks\\*","C:\\Windows\\system32\\Tasks*","C:\\Windows\\AppPatch\\Custom*","*VirtualStore*","*.xls","*.ppt","*.rtf"]}},"process_name":{"exclude":{"values":["*\\Program Files\\plgx_osquery\\plgx_*","C:\\Program Files (x86)\\EMET *\\EMET_Service.exe","C:\\Program Files\\Common Files\\Microsoft Shared\\ClickToRun\\OfficeC2RClient.exe","C:\\Windows\\system32\\smss.exe","C:\\Windows\\system32\\CompatTelRunner.exe","\\\\?\\C:\\Windows\\system32\\wbem\\WMIADAP.EXE","C:\\Windows\\system32\\wbem\\WMIADAP.EXE","C:\\Windows\\system32\\mobsync.exe","C:\\Program Files (x86)\\Dell\\CommandUpdate\\InvColPC.exe","C:\\Windows\\system32\\igfxCUIService.exe","C:\\Windows\\system32\\igfxCUIService.exe"]}}},"win_socket_events":{"remote_port":{"include":{"values":["80","443","445","3389","3540","22","23","25","139","5800","5900","1194","1701","1723","1293","4500","1080","8080","3128","9001","9030","4443","2448","8143","1777","1443","243","65535","13506","3360","200","198","49180","13507","3360","6625","4444","4438","1904","13505","13504","12102","9631","5445","2443","777","13394","13145","12103","5552","3939","3675","666","473","5649","4455","4433","1817","100","65520","1960","1515","743","700","14154","14103","14102","12322","10101","7210","4040","9943","7777","9943","666"]}},"process_name":{"exclude":{"values":["*\\Users\\*\\Spotify.exe","*AppData\\Roaming\\Dropbox\\bin\\Dropbox.exe","*\\Users\\*\\g2ax_comm_expert.exe","*\\Users\\*\\g2mcomm.exe","*\\Users\\*\\OneDrive.exe","*\\Users\\*\\OneDriveStandaloneUpdater.exe","*\\AppData\\Local\\Microsoft\\Teams\\current\\Teams.exe","*\\Program Files\\plgx_osquery\\plgx_*"]},"include":{"values":["C:\\Users*","C:\\ProgramData*","C:\\Windows\\Temp*","*\\at.exe","*\\certutil.exe","*\\cmd.exe","*\\cmstp.exe","*\\cscript.exe","*\\driverquery.exe","*\\dnsquery.exe","*\\hh.exe","*\\infDefaultInstall.exe","*\\java.exe","*\\javaw.exe","*\\javaws.exe","*\\mmc.exe","*\\msbuild.exe","*\\mshta.exe","*\\msiexec.exe","*\\nbtstat.exe","*\\net.exe","*\\net1.exe","*\\notepad.exe","*\\nslookup.exe","*\\powershell.exe","*\\qprocess.exe","*\\qwinsta.exe","*\\reg.exe","*\\regsvcs.exe","*\\regsvr32.exe","*\\rundll32.exe","*\\rwinsta.exe","*\\sc.exe","*\\schtasks.exe","*\\taskkill.exe","*\\tasklist.exe","*\\ncat.exe","*\\wmic.exe","*\\wscript.exe","*\\nc.exe","*\\psexec.exe","*\\psexesvc.exe","*\\tor.exe","*\\vnc.exe","*\\vncservice.exe","*\\vncviewer.exe","*\\winexesvc.exe","*\\nmap.exe","*\\psinfo.exe","*\\python.exe"]}}},"win_process_events":{"path":{"exclude":{"values":["C:\\Windows\\system32\\CompatTelRunner.exe","C:\\Windows\\system32\\audiodg.exe","C:\\Windows\\system32\\conhost.exe","C:\\Windows\\system32\\musNotification.exe","C:\\Windows\\system32\\musNotificationUx.exe","C:\\Windows\\system32\\powercfg.exe","C:\\Windows\\system32\\sndVol.exe","C:\\Windows\\system32\\sppsvc.exe","C:\\Windows\\system32\\wbem\\WmiApSrv.exe","C:\\Windows\\System32\\plasrv.exe","C:\\Windows\\System32\\wifitask.exe","C:\\Program Files (x86)\\Common Files\\microsoft shared\\ink\\TabTip32.exe","C:\\Windows\\System32\\TokenBrokerCookies.exe","C:\\Windows\\system32\\mobsync.exe","C:\\Windows\\system32\\SppExtComObj.Exe","C:\\Windows\\system32\\PrintIsolationHost.exe","C:\\Program Files\\Windows Defender*","C:\\Windows\\system32\\MpSigStub.exe","C:\\Windows\\SoftwareDistribution\\Download\\Install\\AM_*","C:\\Windows\\Microsoft.NET\\Framework64\\*\\mscorsvw.exe","C:\\Windows\\Microsoft.NET\\Framework\\*\\mscorsvw.exe","C:\\Windows\\Microsoft.Net\\Framework64\\*\\WPF\\PresentationFontCache.exe","C:\\Program Files\\Microsoft Office\\Office*\\MSOSYNC.EXE","C:\\Program Files (x86)\\Microsoft Office\\Office*\\MSOSYNC.EXE","C:\\Program Files\\Common Files\\Microsoft Shared\\OfficeSoftwareProtectionPlatform\\OSPPSVC.EXE","C:\\Program Files\\Microsoft Office\\Office*\\msoia.exe","C:\\Program Files (x86)\\Microsoft Office\\root\\Office*\\officebackgroundtaskhandler.exe","C:\\Program Files\\Common Files\\Microsoft Shared\\ClickToRun\\OfficeC2RClient.exe","C:\\Program Files\\Windows Media Player\\wmpnscfg.exe","C:\\Program Files (x86)\\Google\\Update\\*","C:\\Program Files (x86)\\Adobe\\Acrobat DC\\Acrobat\\AcroCEF\\AcroCEF.exe","C:\\Program Files (x86)\\Adobe\\Acrobat DC\\Acrobat\\LogTransport2.exe","C:\\Program Files (x86)\\Adobe\\Acrobat 2015\\Acrobat\\AcroCEF\\AcroCEF.exe","C:\\Program Files (x86)\\Adobe\\Acrobat 2015\\Acrobat\\LogTransport2.exe","C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader\\AcroCEF\\RdrCEF.exe","C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader\\LogTransport2.exe","C:\\Windows\\SysWOW64\\Macromed\\Flash\\FlashPlayerUpdateService.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\ARM\\1.0\\AdobeARM.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\ARM\\1.0\\armsvc.exe","C:\\Program Files (x86)\\Adobe\\Acrobat DC\\Acrobat\\AdobeCollabSync.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\Adobe Desktop Common\\HEX\\Adobe CEF Helper.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\AdobeGCClient\\AdobeGCClient.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\OOBE\\PDApp\\P6\\adobe_licutil.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\OOBE\\PDApp\\P7\\adobe_licutil.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\OOBE\\PDApp\\UWA\\updaterstartuputility.exe","C:\\Program Files (x86)\\Adobe\\Adobe Creative Cloud\\ACC\\Creative Cloud.exe","C:\\Program Files (x86)\\Dropbox\\Update\\DropboxUpdate.exe","C:\\Program Files\\Dell\\SupportAssist\\pcdrcui.exe","C:\\Program Files\\Dell\\SupportAssist\\koala.exe","C:\\Program Files\\osquery\\osqueryd\\osqueryd.exe"]}},"cmdline":{"exclude":{"values":["C:\\Windows\\system32\\DllHost.exe /Processid*","C:\\Windows\\system32\\SearchIndexer.exe /Embedding","C:\\windows\\system32\\wermgr.exe -queuereporting","\"C:\\Windows\\system32\\wermgr.exe\" \"-queuereporting_svc\"*","C:\\WINDOWS\\system32\\wermgr.exe -upload","\\SystemRoot\\System32\\smss.exe","\\??\\C:\\WINDOWS\\system32\\autochk.exe *","C:\\Windows\\system32\\wbem\\wmiprvse.exe -Embedding*","C:\\Windows\\system32\\wbem\\wmiprvse.exe -secured -Embedding*","C:\\Windows\\system32\\svchost.exe -k appmodel -s StateRepository","C:\\Windows\\system32\\svchost.exe -k appmodel","C:\\WINDOWS\\system32\\svchost.exe -k appmodel -p -s tiledatamodelsvc","C:\\Windows\\system32\\svchost.exe -k camera -s FrameServer","C:\\Windows\\system32\\svchost.exe -k dcomlaunch -s LSM","C:\\Windows\\system32\\svchost.exe -k dcomlaunch -s PlugPlay","C:\\Windows\\system32\\svchost.exe -k defragsvc","C:\\Windows\\system32\\svchost.exe -k devicesflow -s DevicesFlowUserSvc","C:\\Windows\\system32\\svchost.exe -k imgsvc","C:\\Windows\\system32\\svchost.exe -k localService -s EventSystem","C:\\Windows\\system32\\svchost.exe -k localService -s bthserv","C:\\Windows\\system32\\svchost.exe -k localService -s nsi","C:\\Windows\\system32\\svchost.exe -k localService -s w32Time","C:\\Windows\\system32\\svchost.exe -k localServiceAndNoImpersonation","C:\\Windows\\system32\\svchost.exe -k localServiceNetworkRestricted -s Dhcp","C:\\Windows\\system32\\svchost.exe -k localServiceNetworkRestricted -s EventLog","C:\\Windows\\system32\\svchost.exe -k localServiceNetworkRestricted -s TimeBrokerSvc","C:\\Windows\\system32\\svchost.exe -k localServiceNetworkRestricted -s WFDSConMgrSvc","C:\\Windows\\system32\\svchost.exe -k localServiceNetworkRestricted","C:\\Windows\\system32\\svchost.exe -k localServiceAndNoImpersonation -s SensrSvc","C:\\Windows\\system32\\svchost.exe -k localServiceNoNetwork","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted -p -s WPDBusEnum","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted -p -s fhsvc","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted -s DeviceAssociationService","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted -s NcbService","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted -s SensorService","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted -s TabletInputService","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted -s UmRdpService","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted -s WPDBusEnum","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted -s WdiSystemHost","C:\\WINDOWS\\System32\\svchost.exe -k LocalSystemNetworkRestricted -p -s WdiSystemHost","C:\\Windows\\system32\\svchost.exe -k localSystemNetworkRestricted","C:\\WINDOWS\\system32\\svchost.exe -k netsvcs -p -s wlidsvc","C:\\Windows\\system32\\svchost.exe -k netsvcs -p -s ncaSvc","C:\\Windows\\system32\\svchost.exe -k netsvcs -s BDESVC","C:\\Windows\\system32\\svchost.exe -k netsvcs -s BITS","C:\\Windows\\system32\\svchost.exe -k netsvcs -s CertPropSvc","C:\\Windows\\system32\\svchost.exe -k netsvcs -s DsmSvc","C:\\Windows\\system32\\svchost.exe -k netsvcs -s Gpsvc","C:\\Windows\\system32\\svchost.exe -k netsvcs -s ProfSvc","C:\\Windows\\system32\\svchost.exe -k netsvcs -s SENS","C:\\Windows\\system32\\svchost.exe -k netsvcs -s SessionEnv","C:\\Windows\\system32\\svchost.exe -k netsvcs -s Themes","C:\\Windows\\system32\\svchost.exe -k netsvcs -s Winmgmt","C:\\Windows\\system32\\svchost.exe -k netsvcs","C:\\Windows\\system32\\svchost.exe -k networkService -p -s DoSvc","C:\\Windows\\system32\\svchost.exe -k networkService -s Dnscache","C:\\Windows\\system32\\svchost.exe -k networkService -s LanmanWorkstation","C:\\Windows\\system32\\svchost.exe -k networkService -s NlaSvc","C:\\Windows\\system32\\svchost.exe -k networkService -s TermService","C:\\Windows\\system32\\svchost.exe -k networkService","C:\\Windows\\system32\\svchost.exe -k networkServiceNetworkRestricted","C:\\Windows\\system32\\svchost.exe -k rPCSS","C:\\Windows\\system32\\svchost.exe -k secsvcs","C:\\Windows\\system32\\svchost.exe -k swprv","C:\\Windows\\system32\\svchost.exe -k unistackSvcGroup","C:\\Windows\\system32\\svchost.exe -k utcsvc","C:\\Windows\\system32\\svchost.exe -k wbioSvcGroup","C:\\Windows\\system32\\svchost.exe -k werSvcGroup","C:\\WINDOWS\\System32\\svchost.exe -k wsappx -p -s ClipSVC","C:\\WINDOWS\\system32\\svchost.exe -k wsappx -p -s AppXSvc","C:\\Windows\\system32\\svchost.exe -k wsappx -s ClipSVC","C:\\Windows\\system32\\svchost.exe -k wsappx","C:\\Windows\\Microsoft.NET\\Framework\\v*\\ngen.exe","\"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe\" --type=*","\"C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe\" --type=*","\"C:\\Program Files\\Mozilla Firefox\\plugin-container.exe\" --channel*","\"C:\\Program Files (x86)\\Mozilla Firefox\\plugin-container.exe\" --channel*","*AcroRd32.exe\" /CR*","*AcroRd32.exe\" --channel=*","\"C:\\Program Files\\DellTPad\\ApMsgFwd.exe\" -s{*","C:\\Windows\\system32\\igfxsrvc.exe -Embedding","C:\\Program Files\\osquery\\osqueryd\\osqueryd.exe"]}},"parent_path":{"exclude":{"values":["*\\Program Files\\plgx_osquery\\plgx_*","C:\\Windows\\system32\\SearchIndexer.exe","C:\\Windows\\Microsoft.NET\\Framework64\\v*\\mscorsvw.exe","C:\\Windows\\Microsoft.NET\\Framework64\\v*\\ngentask.exe","C:\\Windows\\Microsoft.NET\\Framework\\v*\\mscorsvw.exe","C:\\Windows\\Microsoft.NET\\Framework\\v*\\ngentask.exe","C:\\Program Files\\Common Files\\Microsoft Shared\\ClickToRun\\OfficeClickToRun.exe","C:\\Program Files\\Common Files\\Microsoft Shared\\ClickToRun\\OfficeC2RClient.exe","C:\\Program Files (x86)\\Google\\Update\\*","C:\\Program Files (x86)\\Common Files\\Adobe\\AdobeGCClient\\AGSService.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\ARM\\1.0\\AdobeARM.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\OOBE\\PDApp\\P7\\adobe_licutil.exe","C:\\Program Files (x86)\\Common Files\\Adobe\\OOBE\\PDApp\\UWA\\updaterstartuputility.exe","C:\\Program Files (x86)\\Adobe\\Adobe Creative Cloud\\ACC\\Creative Cloud.exe","C:\\Program Files (x86)\\Adobe\\Adobe Creative Cloud\\CCXProcess\\CCXProcess.exe","C:\\Program Files (x86)\\Adobe\\Adobe Creative Cloud\\CoreSync\\CoreSync.exe","C:\\Program Files (x86)\\Cisco\\Cisco AnyConnect Secure Mobility Client\\vpnagent.exe","C:\\Program Files\\DellTPad\\HidMonitorSvc.exe","C:\\Program Files\\Realtek\\Audio\\HDA\\RtkAudioService64.exe","C:\\Program Files (x86)\\Dropbox\\Update\\DropboxUpdate.exe","C:\\Program Files (x86)\\Dell\\CommandUpdate\\InvColPC.exe","C:\\Program Files\\osquery\\osqueryd\\osqueryd.exe"]}}},"win_registry_events":{"action":{"exclude":{"values":["REG_CREATE"]}},"target_name":{"exclude":{"values":["*Toolbar\\WebBrowser","*Toolbar\\WebBrowser\\ITBar7Height","*Toolbar\\WebBrowser\\ITBar7Layout","*Toolbar\\ShellBrowser\\ITBar7Layout","*Internet Explorer\\Toolbar\\Locked","*Toolbar\\WebBrowser\\{47833539-D0C5-4125-9FA8-0819E2EAAC93}","*ShellBrowser","*\\CurrentVersion\\App Paths","*\\CurrentVersion\\Shell Extensions\\Cached","*\\CurrentVersion\\Shell Extensions\\Approved","*}\\PreviousPolicyAreas","*\\Control\\WMI\\Autologger\\","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Services\\UsoSvc\\Start","*\\Lsa\\OfflineJoin\\CurrentValue","*\\Components\\TrustedInstaller\\Events","*\\Components\\TrustedInstaller","*\\Components\\Wlansvc","*\\Components\\Wlansvc\\Events","\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\UserData\\S-1-5-18\\*","*\\Directory\\shellex","*\\Directory\\shellex\\DragDropHandlers","*\\Drive\\shellex","*\\Drive\\shellex\\DragDropHandlers","*_Classes\\AppX*","\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\WINEVT\\Publishers\\*","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Lsa\\Audit","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Lsa\\Audit\\AuditPolicy","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Lsa\\Audit\\PerUserAuditing\\System","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Lsa\\LsaPid","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Lsa\\SspiCache","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Lsa\\Kerberos\\Domains","\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\Audit","*\\services\\bits\\Start","*\\services\\clr_optimization_v*\\Start","*\\services\\deviceAssociationService\\Start","*\\services\\fhsvc\\Start","*\\services\\nal\\Start","*\\services\\trustedInstaller\\Start","*\\services\\tunnel\\Start","*\\services\\usoSvc\\Start","*\\OpenWithProgids","*\\OpenWithList","*\\UserChoice","*\\UserChoice\\ProgId","*\\UserChoice\\Hash","*\\OpenWithList\\MRUList","*} 0xFFFF","\\REGISTRY\\MACHINE\\System\\ControlSet001\\Control\\Lsa\\Audit\\SpecialGroups","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\PSScriptOrder","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\SOM-ID","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\GPO-ID","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\0\\IsPowershell","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\0\\ExecTime","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown\\0","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown\\0\\PSScriptOrder","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown\\0\\SOM-ID","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown\\0\\GPO-ID","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown\\0\\0\\IsPowershell","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown\\0\\0\\ExecTime","*\\safer\\codeidentifiers\\0\\HASHES\\{*","*\\VLC.*","*\\iTunes."]},"include":{"values":["*\\CurrentVersion\\Run","*\\CurrentVersion\\RunOnce","*\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Cryptography\\OID\\EncodingType 0\\CryptSIPDllVerifyIndirectData\\*","*\\CurrentVersion\\Image File Execution Options\\*","*SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SilentProcessExit\\*","\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Windows Defender\\Exclusions\\Paths*","\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows*","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Session Manager\\kernel\\MitigationOptions*","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\MUI\\CallbackDlls\\Microsoft-Windows-BootEnvironment\\name*","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Session Manager\\kernel\\MitigationOptions*","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Services\\DHCPServer\\Parameters*","*SOFTWARE\\Microsoft\\Windows Script\\Settings\\AmsiEnable*","*SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Notifications\\Settings\\Windows.SystemToast.SecurityAndMaintenance\\Enabled*","\\REGISTRY\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ICM\\Calibration\\DisplayCalibrator","*SOFTWARE\\Microsoft\\Office test\\Special\\Perf*","*InprocServer32*","*Environment*","\\REGISTRY\\MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\AppKey","*Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\AppKey","*LocalServer32*","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Lsa\\RunAsPPL","*Software\\Microsoft\\Office\\*\\Word\\Security\\Level*","*Software\\Microsoft\\Office\\*\\*\\Security\\ProtectedView*","*Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Attachments\\HideZoneInfoOnProperties*","*Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Attachments\\ScanWithAntiVirus*","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\Session Manager\\SafeDllSearchMode","*Control Panel\\Desktop\\ScreenSaveActive*","*Control Panel\\Desktop\\ScreenSaverTimeout*","*Control Panel\\Desktop\\SCRNSAVE.EXE*","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\SecureBoot","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\services\\DNS\\Parameters\\ServerLevelPluginDll","*\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run*","*\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run*","*\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\Run*","*\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce","*\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx*","*\\Software\\Microsoft\\Windows\\CurrentVersion\\RunServices*","*\\Software\\Microsoft\\Windows\\CurrentVersion\\RunServicesOnce*","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Services\\LanmanServer\\Parameters\\SMB1","\\REGISTRY\\MACHINE\\SYSTEM\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\EnableUA","\\REGISTRY\\MACHINE\\SYSTEM\\ControlSet001\\Control\\CrashControl\\LogEvent","*Software\\Microsoft\\Office Test\\Special\\Perf*","*Software\\Microsoft\\Office\\*\\Word*","\\REGISTRY\\MACHINE\\Software\\Classes\\MSCFile\\shell\\open\\command*","*Software\\Classes\\exefile\\shell\\runas\\command\\*","*Software\\Classes\\MSCFile\\shell\\open\\command\\*","*Software\\Microsoft\\Cryptography\\providers\\trust\\FinalPolicy\\*"]}},"process_name":{"exclude":{"values":["*\\Program Files\\plgx_osquery\\plgx_*"]}}},"win_image_load_process_map":{"image_path":{"exclude":{"values":["*"]}}},"win_image_load_events":{"image_path":{"exclude":{"values":["*.exe","C:\\Windows\\system32\\*","C:\\WINDOWS\\SYSWOW64\\*","C:\\Program Files\\*","C:\\WINDOWS\\WINSXS\\*","C:\\WINDOWS\\ASSEMBLY\\*","C:\\Program Files (x86)\\*"]}}},"win_process_open_events":{"src_path":{"exclude":{"values":["C:\\Windows\\system32\\wbem\\WmiPrvSE.exe","*\\Program Files\\plgx_osquery\\plgx_*"]}},"target_path":{"include":{"values":["*lsass.exe"]}}},"win_remote_thread_events":{"src_path":{"exclude":{"values":["System","C:\\Windows\\system32\\wbem\\WmiPrvSE.exe","C:\\Windows\\system32\\svchost.exe","C:\\Windows\\system32\\wininit.exe","C:\\Windows\\system32\\csrss.exe","C:\\Windows\\system32\\services.exe","C:\\Windows\\system32\\winlogon.exe","C:\\Windows\\system32\\audiodg.exe","C:\\Program Files (x86)\\Webroot\\WRSA.exe"]}},"target_path":{"exclude":{"values":["*Google\\Chrome\\Application\\chrome.exe"]}}}},"options":{"custom_plgx_EnableLogging":"true","custom_plgx_LogFileName":"C:\\Program Files\\osquery\\plgx-win-extension.log","custom_plgx_LogLevel":"3","custom_plgx_LogModeQuiet":"1","schedule_splay_percent":10,"custom_plgx_EnableDns":"true","custom_plgx_EnableSSL":"true","custom_plgx_EnableHttp":"true","custom_plgx_EnableAmsiStreamEventData":"true"},"decorators":{"always":["SELECT total_seconds AS uptime FROM uptime;"]}}','win_config', 'Windows', '', '64');
insert into config_queries (query, query_name,os_name,kernel_version,os_arch) values ('{"schedule":{"osquery_schedule":{"interval":30,"query":"select * from osquery_schedule","snapshot": false}}}', 'osquery_schedule', 'Linux, Windows', '', '');


------- ADD new config queries above--



insert into data_retention_settings (days) values (0);

insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('process_open_files', 'Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('process_open_pipes', 'Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('listening_ports', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('routes', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('logged_in_users', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('arp_cache', 'Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('suid_bin', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('process_open_sockets', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('users', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('groups', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('process_events', 'Linux', '<4.18', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('processes', 'Linux, Windows', '', '32/64');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('user_events', 'Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('process_file_events','Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('file_events','Linux', '<4.18', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('socket_events', 'Linux', '<4.18', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('cpu_time','Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('memory_info', 'Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('process_envs', 'Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('bpf_socket_events', 'Linux', '>4.17', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('bpf_process_events', 'Linux', '>4.17', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('bpf_file_events', 'Linux', '>4.17', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('interface_addresses', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('docker_events', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('win_process_events', 'Windows', '', '64');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('win_file_events', 'Windows', '', '64');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('win_socket_events', 'Windows', '', '64');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('chrome_extensions', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('file', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('etc_hosts', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('uptime', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('startup_items', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('patches', 'Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('programs', 'Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('kernel_info', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('deb_packages', 'Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('rpm_packages', 'Linux', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('chocolatey_packages', 'Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('interface_details', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('ie_extensions', 'Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('firefox_addons', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('npm_packages', 'Linux, Windows', '', '');
insert into config_table_name (table_name,os_name,kernel_version,os_arch) values ('portage_packages', 'Linux', '', '');


-- BPF Process Events columns description

COMMENT ON COLUMN bpf_process_events.id IS 'ID';
COMMENT ON COLUMN bpf_process_events.host_identifier IS 'Host Identifier';
COMMENT ON COLUMN bpf_process_events.hostname IS 'Hostname';
COMMENT ON COLUMN bpf_process_events.unixTime IS 'Unixtime';
COMMENT ON COLUMN bpf_process_events.tid IS 'Thread ID';
COMMENT ON COLUMN bpf_process_events.pid IS 'Process ID';
COMMENT ON COLUMN bpf_process_events.parent IS 'Parent Process ID';
COMMENT ON COLUMN bpf_process_events.uid IS 'User ID';
COMMENT ON COLUMN bpf_process_events.gid IS 'Group ID';
COMMENT ON COLUMN bpf_process_events.cid IS 'Cgroup ID';
COMMENT ON COLUMN bpf_process_events.exit_code IS 'Exit code';
COMMENT ON COLUMN bpf_process_events.probe_error IS 'Probe Error';
COMMENT ON COLUMN bpf_process_events.syscall IS 'Syscall';
COMMENT ON COLUMN bpf_process_events.path IS 'Path';
COMMENT ON COLUMN bpf_process_events.cwd IS 'Current working directory';
COMMENT ON COLUMN bpf_process_events.cmdline IS 'Commandine';
COMMENT ON COLUMN bpf_process_events.duration IS 'Duration';
COMMENT ON COLUMN bpf_process_events.json_cmdline IS 'JSON command line';
COMMENT ON COLUMN bpf_process_events.ntime IS 'Nanoseconds uptime';
COMMENT ON COLUMN bpf_process_events.time IS 'Time';
COMMENT ON COLUMN bpf_process_events.eid IS 'Event ID';
COMMENT ON COLUMN bpf_process_events.md5 IS 'MD5';
COMMENT ON COLUMN bpf_process_events.sha1 IS 'SHA1';
COMMENT ON COLUMN bpf_process_events.sha256 IS 'SHA256';
COMMENT ON COLUMN bpf_process_events.is_container_process IS 'Is Container Process';
COMMENT ON COLUMN bpf_process_events.container_age IS 'Container Age';
COMMENT ON COLUMN bpf_process_events.container_name IS 'Container Name';
COMMENT ON COLUMN bpf_process_events.process_tree IS 'Process Tree';
COMMENT ON COLUMN bpf_process_events.uptime IS 'Uptime';
COMMENT ON COLUMN bpf_process_events.is_processed IS 'Is Processed';




-- BPF Socket Events columns description

COMMENT ON COLUMN bpf_socket_events.id IS 'ID';
COMMENT ON COLUMN bpf_socket_events.host_identifier IS 'Host Identifier';
COMMENT ON COLUMN bpf_socket_events.hostname IS 'Hostname';
COMMENT ON COLUMN bpf_socket_events.unixTime IS 'Unixtime';
COMMENT ON COLUMN bpf_socket_events.tid IS 'Thread ID';
COMMENT ON COLUMN bpf_socket_events.pid IS 'Process ID';
COMMENT ON COLUMN bpf_socket_events.parent IS 'Parent Process ID';
COMMENT ON COLUMN bpf_socket_events.uid IS 'User ID';
COMMENT ON COLUMN bpf_socket_events.gid IS 'Group ID';
COMMENT ON COLUMN bpf_socket_events.cid IS 'Cgroup ID';
COMMENT ON COLUMN bpf_socket_events.exit_code IS 'Exit code';
COMMENT ON COLUMN bpf_socket_events.probe_error IS 'Probe Error';
COMMENT ON COLUMN bpf_socket_events.syscall IS 'Syscall';
COMMENT ON COLUMN bpf_socket_events.path IS 'Path';
COMMENT ON COLUMN bpf_socket_events.fd IS 'File Descriptor';
COMMENT ON COLUMN bpf_socket_events.family IS 'Internet Protocol Family ID';
COMMENT ON COLUMN bpf_socket_events.type IS 'Socket Type';
COMMENT ON COLUMN bpf_socket_events.protocol IS 'Internet Protocol ID';
COMMENT ON COLUMN bpf_socket_events.local_address IS 'Local Address';
COMMENT ON COLUMN bpf_socket_events.remote_address IS 'Remote Address';
COMMENT ON COLUMN bpf_socket_events.local_port IS 'Local Port';
COMMENT ON COLUMN bpf_socket_events.remote_port IS 'Remote Port';
COMMENT ON COLUMN bpf_socket_events.duration IS 'Duration';
COMMENT ON COLUMN bpf_socket_events.ntime IS 'Nanoseconds uptime';
COMMENT ON COLUMN bpf_socket_events.time IS 'Time';
COMMENT ON COLUMN bpf_socket_events.eid IS 'Event ID';
COMMENT ON COLUMN bpf_socket_events.process_tree IS 'Process Tree';
COMMENT ON COLUMN bpf_socket_events.uptime IS 'Uptime';
COMMENT ON COLUMN bpf_socket_events.is_processed IS 'Is Processed';




-- BPF Socket File columns description

COMMENT ON COLUMN bpf_file_events.id IS 'ID';
COMMENT ON COLUMN bpf_file_events.host_identifier IS 'Host Identifier';
COMMENT ON COLUMN bpf_file_events.hostname IS 'Hostname';
COMMENT ON COLUMN bpf_file_events.unixTime IS 'Unixtime';
COMMENT ON COLUMN bpf_file_events.tid IS 'Thread ID';
COMMENT ON COLUMN bpf_file_events.pid IS 'Process ID';
COMMENT ON COLUMN bpf_file_events.parent IS 'Parent Process ID';
COMMENT ON COLUMN bpf_file_events.uid IS 'User ID';
COMMENT ON COLUMN bpf_file_events.gid IS 'Group ID';
COMMENT ON COLUMN bpf_file_events.cid IS 'Cgroup ID';
COMMENT ON COLUMN bpf_file_events.exit_code IS 'Exit code';
COMMENT ON COLUMN bpf_file_events.probe_error IS 'Probe Error';
COMMENT ON COLUMN bpf_file_events.syscall IS 'Syscall';
COMMENT ON COLUMN bpf_file_events.path IS 'Path';
COMMENT ON COLUMN bpf_file_events.file_path IS 'File Path';
COMMENT ON COLUMN bpf_file_events.flags IS 'Flags';
COMMENT ON COLUMN bpf_file_events.duration IS 'Duration';
COMMENT ON COLUMN bpf_file_events.ntime IS 'Nanoseconds uptime';
COMMENT ON COLUMN bpf_file_events.time IS 'Time';
COMMENT ON COLUMN bpf_file_events.process_tree IS 'Process Tree';
COMMENT ON COLUMN bpf_file_events.uptime IS 'Uptime';
COMMENT ON COLUMN bpf_file_events.is_processed IS 'Is Processed';



-- Windows Socket Events columns description

COMMENT ON COLUMN win_socket_events.id IS 'ID';
COMMENT ON COLUMN win_socket_events.host_identifier IS 'Host Identifier';
COMMENT ON COLUMN win_socket_events.hostname IS 'Hostname';
COMMENT ON COLUMN win_socket_events.unixTime IS 'Unixtime';
COMMENT ON COLUMN win_socket_events.action IS 'Action';
COMMENT ON COLUMN win_socket_events.eid IS 'EID';
COMMENT ON COLUMN win_socket_events.time IS 'Time';
COMMENT ON COLUMN win_socket_events.utc_time IS 'UTC Time';
COMMENT ON COLUMN win_socket_events.pid IS 'Process ID';
COMMENT ON COLUMN win_socket_events.process_guid IS 'Process GUID';
COMMENT ON COLUMN win_socket_events.process_name IS 'Process Name';
COMMENT ON COLUMN win_socket_events.family IS 'Family';
COMMENT ON COLUMN win_socket_events.protocol IS 'Protocol';
COMMENT ON COLUMN win_socket_events.local_address IS 'Local Address';
COMMENT ON COLUMN win_socket_events.remote_address IS 'Remote Address';
COMMENT ON COLUMN win_socket_events.local_port IS 'Local Port';
COMMENT ON COLUMN win_socket_events.eventid IS 'EventID';
COMMENT ON COLUMN win_socket_events.parent_pid IS 'Parent Process ID';

-- Window Process Events columns description

COMMENT ON COLUMN win_process_events.id IS 'ID';
COMMENT ON COLUMN win_process_events.host_identifier IS 'Host Identifier';
COMMENT ON COLUMN win_process_events.hostname IS 'Hostname';
COMMENT ON COLUMN win_process_events.unixTime IS 'Unixtime';
COMMENT ON COLUMN win_process_events.eid IS 'EID';
COMMENT ON COLUMN win_process_events.pid IS 'Process ID';
COMMENT ON COLUMN win_process_events.parent_pid IS 'Parent Process ID';
COMMENT ON COLUMN win_process_events.process_guid IS 'Process GUID';
COMMENT ON COLUMN win_process_events.cmdline IS 'Commandine';
COMMENT ON COLUMN win_process_events.parent_process_guid IS 'Parent Process GUID';
COMMENT ON COLUMN win_process_events.owner_uid IS 'Owner UID';
COMMENT ON COLUMN win_process_events.utc_time IS 'UTC Time';
COMMENT ON COLUMN win_process_events.sha256 IS 'SHA256';


-- Windows File Events columns description

COMMENT ON COLUMN win_file_events.id IS 'ID';
COMMENT ON COLUMN win_file_events.host_identifier IS 'Host Identifier';
COMMENT ON COLUMN win_file_events.hostname IS 'Hostname';
COMMENT ON COLUMN win_file_events.unixTime IS 'Unixtime';
COMMENT ON COLUMN win_file_events.eid IS 'EID';
COMMENT ON COLUMN win_file_events.md5 IS 'MD5';
COMMENT ON COLUMN win_file_events.sha256 IS 'SHA256';
COMMENT ON COLUMN win_file_events.utc_time IS 'UTC Time';
COMMENT ON COLUMN win_file_events.pe_file IS 'PE File';
COMMENT ON COLUMN win_file_events.process_guid IS 'Process GUID';
COMMENT ON COLUMN win_file_events.eventid IS 'EventID';
