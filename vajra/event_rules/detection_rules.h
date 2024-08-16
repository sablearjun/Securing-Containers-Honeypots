#ifndef FLEET_DETECTION_RULES_H
#define FLEET_DETECTION_RULES_H

#include "../common/EdrDataTypes.h"

// New Rules Mitre Attacks

// bool kernel_modules_extensions(const BpfProcessEntry& bpf_process_event, Event& rule_event);

// bool kernel_modules_extensions(const ProcessEvent& process_event, Event& rule_event);

bool kernel_modules_extensions_modprobe(const ProcessEvent &process_event, Event &rule_event);

bool kernel_modules_extensions_insmod(const ProcessEvent &process_event, Event &rule_event);

bool kernel_modules_extensions_lsmod(const ProcessEvent &process_event, Event &rule_event);

bool kernel_modules_extensions_rmmod(const ProcessEvent &process_event, Event &rule_event);

bool create_local_account_useradd(const ProcessEvent &process_event, Event &rule_event);

bool create_local_account_adduser(const ProcessEvent &process_event, Event &rule_event);

bool create_domain_account(const ProcessEvent &process_event, Event &rule_event);

bool event_triggered_execution_trap(const ProcessEvent &process_event, Event &rule_event);

bool dynamic_linker_hijacking(const ProcessEvent &process_event, Event &rule_event);

bool scheduled_task_at(const ProcessEvent &process_event, Event &rule_event);

bool scheduled_task_atrm(const ProcessEvent &process_event, Event &rule_event);

bool scheduled_task_cron(const ProcessEvent &process_event, Event &rule_event);

bool scheduled_task_systemd_timers(const ProcessEvent &process_event, Event &rule_event);

bool ssh_authorized_keys(const ProcessEvent &process_event, Event &rule_event);

bool abuse_elevation_control_setuid(const ProcessEvent &process_event, Event &rule_event);

bool abuse_elevation_control_setuid1(const ProcessEvent &process_event, Event &rule_event);

bool abuse_elevation_control_sudo_caching(const ProcessEvent &process_event, Event &rule_event);

bool create_modify_system_systemd_service(const ProcessEvent &process_event, Event &rule_event);

bool terminal_downloads_curl(const ProcessEvent &process_event, Event &rule_event);

bool impair_command_history_logging(const ProcessEvent &process_event, Event &rule_event);

bool terminal_downloads_python(const ProcessEvent &process_event, Event &rule_event);

bool bash_history_commandLine(const ProcessEvent &process_event, Event &rule_event);

bool ping_privileged_commandLine(const ProcessEvent &process_event, Event &rule_event);

// Date 8-4-2022

bool mount_privileged_commandLine(const ProcessEvent &process_event, Event &rule_event);

bool umount_privileged_commandLine(const ProcessEvent &process_event, Event &rule_event);

bool chgrp_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool pam_timestamp_check_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool unix_chkpwd_check_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool pwck_check_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool userhelper_check_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool Xorg_check_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool rlogin_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool sudoedit_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool rsh_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool gpasswd_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool sudo_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool staprun_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool rcp_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool passwd_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool chsh_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool chfn_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool chage_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool setfacl_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool chacl_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool chcon_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool newgrp_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool sleep_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool pgrep_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool grep_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool lspci_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool udevadm_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool findmnt_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool netstat_privileged_command(const ProcessEvent &process_event, Event &rule_event);

bool awk_privileged_command(const ProcessEvent &process_event, Event &rule_event);

// bool sed_privileged_command(const ProcessEvent& process_event, Event& rule_event);

bool virtualization_evasion_system_checks_systemd_detect_virt(const ProcessEvent &process_event, Event &rule_event);

bool virtualization_evasion_system_checks_dmidecode(const ProcessEvent &process_event, Event &rule_event);

bool set_file_access_timestamp(const ProcessEvent &process_event, Event &rule_event);

// bool set_file_modification_timestamp(const ProcessEvent& process_event, Event& rule_event);

bool modify_file_timestamp_using_reference_file(const ProcessEvent &process_event, Event &rule_event);

bool sudo_and_sudo_caching_sudo_usage(const ProcessEvent &process_event, Event &rule_event);

bool sudo_and_sudo_caching_unlimited_sudo_timeout(const ProcessEvent &process_event, Event &rule_event);

bool sudo_and_sudo_caching_disable_tty_tickets_sudo_caching(const ProcessEvent &process_event, Event &rule_event);

bool impair_cmd_history_logging_disable_history_collection(const ProcessEvent &process_event, Event &rule_event);

bool impair_cmd_history_logging_mac_hist_control(const ProcessEvent &process_event, Event &rule_event);

bool file_deletion_single_file(const ProcessEvent &process_event, Event &rule_event);

bool file_deletion_entire_folder(const ProcessEvent &process_event, Event &rule_event);

bool overwrite_and_delete_file_with_shred(const ProcessEvent &process_event, Event &rule_event);

bool delete_filesystem_root(const ProcessEvent &process_event, Event &rule_event);

bool modifying_cron_file_reference(const ProcessEvent &process_event, Event &rule_event);

bool modifying_cron_daily_file(const ProcessEvent &process_event, Event &rule_event);

bool modifying_cron_hourly_file(const ProcessEvent &process_event, Event &rule_event);

bool modifying_cron_monthly_file(const ProcessEvent &process_event, Event &rule_event);

bool modifying_cron_weekly_file(const ProcessEvent &process_event, Event &rule_event);

bool modifying_cron_var_file(const ProcessEvent &process_event, Event &rule_event);

bool shared_library_injection(const ProcessEvent &process_event, Event &rule_event);

bool shared_library_injection_ld_preload(const ProcessEvent &process_event, Event &rule_event);

bool load_kernel_module_insmod(const ProcessEvent &process_event, Event &rule_event);

bool pam_config_file_modify(const ProcessEvent &process_event, Event &rule_event);

bool rule_add_pam_config(const ProcessEvent &process_event, Event &rule_event);

bool rc_script_common_midify(const ProcessEvent &process_event, Event &rule_event);

bool rc_script_local_midify(const ProcessEvent &process_event, Event &rule_event);

bool ssh_authorized_keys_midify(const ProcessEvent &process_event, Event &rule_event);

bool create_systemd_service_path(const ProcessEvent &process_event, Event &rule_event);

bool create_systemd_service_file(const ProcessEvent &process_event, Event &rule_event);

bool create_systemd_service__timer_service(const ProcessEvent &process_event, Event &rule_event);

bool create_systemd_service__timer_file(const ProcessEvent &process_event, Event &rule_event);

bool lateral_movement_with_secure_shell(const ProcessEvent &process_event, Event &rule_event);

bool lateral_movement_with_ssh_rsa(const ProcessEvent &process_event, Event &rule_event);

bool lateral_tool_transfer_files(const ProcessEvent &process_event, Event &rule_event);

bool shared_library_injection_ld_so_preload(const ProcessEvent& process_event, Event& rule_event);

bool disable_syslog(const ProcessEvent& process_event, Event& rule_event);

bool disable_cb_response(const ProcessEvent& process_event, Event& rule_event);

bool stop_crowdstrike_falcon(const ProcessEvent& process_event, Event& rule_event);

bool stop_start_ufw_firewall(const ProcessEvent& process_event, Event& rule_event);

bool stop_start_ufw_firewall_systemctl(const ProcessEvent& process_event, Event& rule_event);

bool turn_off_ufw_logging(const ProcessEvent& process_event, Event& rule_event);

bool add_delete_ufw_rules(const ProcessEvent& process_event, Event& rule_event);

// TODO add file_events_rule_for below
bool edit_ufw_user_rules_file(const ProcessEvent& process_event, Event& rule_event);

bool edit_ufw_conf_file(const ProcessEvent& process_event, Event& rule_event);

bool edit_ufw_rules_sysctl_conf_file(const ProcessEvent& process_event, Event& rule_event);

bool edit_ufw_firewall_main_config_file(const ProcessEvent& process_event, Event& rule_event);

bool tail_ufw_firewall_log_file(const ProcessEvent& process_event, Event& rule_event);

bool base64_decoding_python(const ProcessEvent& process_event, Event& rule_event);

bool base64_decoding_perl(const ProcessEvent& process_event, Event& rule_event);

bool base64_decoding_shell_utilities(const ProcessEvent& process_event, Event& rule_event);

bool hex_decoding_shell_utilities(const ProcessEvent& process_event, Event& rule_event);

bool compile_after_delivery_c_compile(const ProcessEvent& process_event, Event& rule_event);

bool compile_after_delivery_go_compile(const ProcessEvent& process_event, Event& rule_event);

bool creating_gcp_service_account_and_key(const ProcessEvent& process_event, Event& rule_event);

bool clear_linux_logs_rm_rf(const ProcessEvent& process_event, Event& rule_event);

bool overwrite_linux_mail_spool_and_logs(const ProcessEvent& process_event, Event& rule_event);

bool at_schedule_a_job(const ProcessEvent& process_event, Event& rule_event);

bool system_owner_user_discovery(const ProcessEvent& process_event, Event& rule_event);

bool enumerate_all_accounts_local(const ProcessEvent& process_event, Event& rule_event);

bool view_sudoers_file(const ProcessEvent& process_event, Event& rule_event);

bool view_accounts_with_uid_0(const ProcessEvent& process_event, Event& rule_event);

bool list_opened_files_by_user(const ProcessEvent& process_event, Event& rule_event);

bool show_if_a_user_account_has_ever_logged_in_remotely(const ProcessEvent& process_event, Event& rule_event);

bool enumerate_users_and_groups(const ProcessEvent& process_event, Event& rule_event);

bool system_service_discovery_systemctl(const ProcessEvent& process_event, Event& rule_event);

bool packet_capture_linux(const ProcessEvent& process_event, Event& rule_event);

bool network_share_discovery(const ProcessEvent& process_event, Event& rule_event);

// bool examine_password_complexity_policy(const ProcessEvent& process_event, Event& rule_event);

bool list_os_information(const ProcessEvent& process_event, Event& rule_event);

bool linux_vm_check_via_hardware(const ProcessEvent& process_event, Event& rule_event);

bool linux_vm_check_via_kernel_modules(const ProcessEvent& process_event, Event& rule_event);

bool hostname_discovery(const ProcessEvent& process_event, Event& rule_event);

bool environment_variables_discovery(const ProcessEvent& process_event, Event& rule_event);

bool list_mozilla_firefox_bookmark_database_files(const ProcessEvent& process_event, Event& rule_event);

bool system_network_configuration_discovery(const ProcessEvent& process_event, Event& rule_event);

bool nix_file_and_directory_discovery(const ProcessEvent& process_event, Event& rule_event);

bool system_network_connections_discovery(const ProcessEvent& process_event, Event& rule_event);

bool process_discovery(const ProcessEvent& process_event, Event& rule_event);

bool permission_groups_discovery_local(const ProcessEvent& process_event, Event& rule_event);

bool security_software_discovery(const ProcessEvent& process_event, Event& rule_event);

bool remote_system_discovery_ipneighbour(const ProcessEvent& process_event, Event& rule_event);

bool port_scan(const ProcessEvent& process_event, Event& rule_event);

bool encrypt_file_using_gpg(const ProcessEvent& process_event, Event& rule_event);

bool encrypt_file_using_7z(const ProcessEvent& process_event, Event& rule_event);

bool encrypt_file_using_openssl(const ProcessEvent& process_event, Event& rule_event);

bool overwrite_file_with_dd(const ProcessEvent& process_event, Event& rule_event);

bool system_shutdown_reboot(const ProcessEvent& process_event, Event& rule_event);





bool hardware_additions(const ProcessEvent& process_event, Event& rule_event);
 
bool inject_ld_preload(const ProcessEvent& process_event, Event& rule_event);

bool manipulation_ssh_authorized_key(const ProcessEvent& process_event, Event& rule_event);

bool shell_config_modify(const ProcessEvent& process_event, Event& rule_event);

bool dynamic_linker_hijacking_file(const ProcessEvent& process_event, Event& rule_event);

// bool pluggable_authentication_modules_file(const ProcessEvent& process_event, Event& rule_event);

bool scheduled_task_at_file(const ProcessEvent& process_event, Event& rule_event);

bool scheduled_task_cron_file(const ProcessEvent& process_event, Event& rule_event);

bool scheduled_task_systemd_timers_file(const ProcessEvent& process_event, Event& rule_event);





bool data_transfer_size_limits(const ProcessEvent& process_event, Event& rule_event);

bool exfil_compressed_archive_to_s3_via_aws_cli(const ProcessEvent& process_event, Event& rule_event);

bool exfil_compressed_archive_to_s3_via_aws_golang(const ProcessEvent& process_event, Event& rule_event);

bool multi_hop_proxy(const ProcessEvent& process_event, Event& rule_event);
 
bool ingress_tool_transfer(const ProcessEvent& process_event, Event& rule_event);
 
bool Abuse_elevation_control_mechanism_file(const ProcessEvent& process_event, Event& rule_event);
 
bool abuse_elevation_control_mechanism_sudo_caching_file(const ProcessEvent& process_event, Event& rule_event);
 
bool modify_system_process_systemd_service_file(const ProcessEvent& process_event, Event& rule_event);
 
bool process_injection_proc_memory_file(const ProcessEvent& process_event, Event& rule_event);
 
bool boot_logon_initialization_scripts_rc_scripts_file(const ProcessEvent& process_event, Event& rule_event);

bool chaos_malware_infection(const ProcessEvent& process_event, Event& rule_event);

bool rule_builder_rule(const ProcessEvent& process_event, Event& rule_event);

bool log4j_spawn_shell(const ProcessEvent& process_event, Event& rule_event);

std::string get_context_data(std::string host_identifier, long pid, long start_time, long end_time);

std::string get_process_tree(long pid, std::string table_name);
#endif // FLEET_DETECTION_RULES_H
