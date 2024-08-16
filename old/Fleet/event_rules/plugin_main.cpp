#include "../common/plugin_base.h"
#include "../common/EdrDataTypes.h"
#include "detection_rules.h"

class 
ProcessEventProcessor : public AbstractDetectionPlugin
{
private:
        std::vector<std::tuple<std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent &, Event &)>>> event_rules;

public:
        ProcessEventProcessor()
        {
                event_rules.emplace_back(
                    std::make_tuple("WGET_DOWNLOAD_IN_TMP","","","", SeverityLevel::Medium, true, wget_download_tmp));
                event_rules.emplace_back(
                    std::make_tuple("CPULIMIT_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, cpulimit_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("DMESG_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, dmesg_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("DOCKER_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, docker_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("DPKG_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, dpkg_spawn_shell));
                // event_rules.emplace_back(
                //     std::make_tuple("ENV_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, env_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("FIND_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, find_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("FLOCK_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, flock_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("APT_GET_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, apt_get_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("APT_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, apt_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("ASH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ash_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("AWK_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, awk_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("BUNDLER_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, bundler_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("BUSCTL_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, busctl_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("BYEBUG_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, byebug_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("CPAN_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, cpan_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("IONICE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ionice_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("JOURNALCTL_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, journalctl_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("KSH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ksh_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("LESS_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, less_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("LOGSAVE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, logsave_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("LTRACE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ltrace_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("LUA_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, lua_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("MAN_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, man_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("MAWK_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, mawk_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("MYSQL_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, mysql_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("NANO_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, nano_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("NICE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, nice_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("NMAP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, nmap_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("NOHUP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, nohup_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("NROFF_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, nroff_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("NSENTER_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, nsenter_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("PERL_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, perl_spawn_shell));

                event_rules.emplace_back(
                    std::make_tuple("FTP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ftp_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("GIT_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, git_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("GAWK_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, gawk_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("GCC_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, gcc_spawn_shell));
                // event_rules.emplace_back(
                //     std::make_tuple("GDB_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, gdb_spawn_shell));

                event_rules.emplace_back(
                    std::make_tuple("TAR_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, tar_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("TASKSET_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, taskset_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("TIME_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, time_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("TIMEOUT_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, timeout_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("TMUX_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, tmux_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("WATCH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, watch_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("VI_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, vi_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("VIM_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, vim_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("VIEW_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, view_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("XARGS_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, xargs_spawn_shell));

                event_rules.emplace_back(
                    std::make_tuple("ZIP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, zip_spawn_shell));

                event_rules.emplace_back(
                    std::make_tuple("STRACE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, strace_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("STDBUF_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, stdbuf_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("STARTSTOPDAEMON_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, startstopdaemon_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("SSH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ssh_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("SPLIT_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, split_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("SQLITE3_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, sqlite3_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("SETARCH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, setarch_spawn_shell));
                event_rules.emplace_back(std::make_tuple("SED_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, sed_spawn_shell));
                event_rules.emplace_back(std::make_tuple("SLSH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, slsh_spawn_shell));
                event_rules.emplace_back(std::make_tuple("SOCAT_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, socat_spawn_shell));
                event_rules.emplace_back(std::make_tuple("SCRIPT_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, script_spawn_shell));
                event_rules.emplace_back(std::make_tuple("SCREEN_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, screen_spawn_shell));
                event_rules.emplace_back(std::make_tuple("PUPPET_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, puppet_spawn_shell));
                event_rules.emplace_back(std::make_tuple("RUNMAILCAP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, runmailcap_spawn_shell));
                event_rules.emplace_back(std::make_tuple("RAKE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, rake_spawn_shell));
                event_rules.emplace_back(std::make_tuple("RLWRAP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, rlwrap_spawn_shell));
                event_rules.emplace_back(std::make_tuple("RPM_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, rpm_spawn_shell));
                event_rules.emplace_back(std::make_tuple("PIC_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, pic_spawn_shell));
                event_rules.emplace_back(std::make_tuple("SCP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, scp_spawn_shell));
                event_rules.emplace_back(std::make_tuple("PHP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, php_spawn_shell));
                event_rules.emplace_back(std::make_tuple("UNSHARE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, unshare_spawn_shell));

                // Arjun
                event_rules.emplace_back(
                    std::make_tuple("AT_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, at_spawn_shell));
                // event_rules.emplace_back(
                //         std::make_tuple("BASH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, bash_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("CAPSH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, capsh_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("COWSAY_SPAWNS_SHELL","GTFOBINS","Cowsay","https://gtfobins.github.io/gtfobins/cowsay/", SeverityLevel::Medium, true, cowsay_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("CRASH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, crash_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("CSH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, csh_spawn_shell));
                // event_rules.emplace_back(
                //         std::make_tuple("DASH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, dash_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("ED_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ed_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("EMAC_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, emacs_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("EX_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ex_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("EXPECT_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, expect_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("FACTER_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, facter_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("GHC_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ghc_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("GHCI_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, ghci_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("HPING_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, hping_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("GTESTER_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, gtester_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("GIMP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, gimp_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("IRB_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, irb_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("JJS_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, jjs_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("MAIL_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, mail_spawn_shell));
                // event_rules.emplace_back(
                //     std::make_tuple("MAKE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, make_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("MORE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, more_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("NAWK_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, nawk_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("NODE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, node_spawn_shell));
                // event_rules.emplace_back(

                //     std::make_tuple("PICO_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, pico_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("ZYPPER_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, zypper_spawn_shell));
                // event_rules.emplace_back(
                //     std::make_tuple("ZSH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, zsh_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("VALGRIND_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, valgrind_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("TOP_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, top_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("TCLSH_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, tclsh_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("SERVICE_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, service_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("RVIEW_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, rview_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("RUN-PARTS_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, run_parts_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("PYTHON_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, python_spawn_shell));
                event_rules.emplace_back(
                    std::make_tuple("PSQL_SPAWNS_SHELL","","","", SeverityLevel::Medium, true, psql_spawn_shell));

                // Sunil

                event_rules.emplace_back(
                    std::make_tuple("KERNAL_MODULES_EXTENSIONS_MODPROBE","PRIVILEGE ESCALATION, PERSISTENCE","T1547.006","https://attack.mitre.org/techniques/T1547/006", SeverityLevel::High, true, kernel_modules_extensions_modprobe));

                event_rules.emplace_back(
                    std::make_tuple("KERNAL_MODULES_EXTENSIONS_INSMOD","PRIVILEGE ESCALATION, PERSISTENCE","T1547.006","https://attack.mitre.org/techniques/T1547/006", SeverityLevel::High, true, kernel_modules_extensions_insmod));

                event_rules.emplace_back(
                    std::make_tuple("KERNAL_MODULES_EXTENSIONS_LSMOD","PRIVILEGE ESCALATION, PERSISTENCE","T1547.006","https://attack.mitre.org/techniques/T1547/006", SeverityLevel::High, true, kernel_modules_extensions_lsmod));

                event_rules.emplace_back(
                    std::make_tuple("KERNAL_MODULES_EXTENSIONS_RMMOD","PRIVILEGE ESCALATION, PERSISTENCE","T1547.006","https://attack.mitre.org/techniques/T1547/006", SeverityLevel::High, true, kernel_modules_extensions_rmmod));

                event_rules.emplace_back(
                    std::make_tuple("CREATE_LOCAL_ACCOUNT_USERADD","PERSISTENCE","T1136.001","https://attack.mitre.org/techniques/T1136/001", SeverityLevel::High, true, create_local_account_useradd));

                event_rules.emplace_back(
                    std::make_tuple("CREATE_LOCAL_ACCOUNT_ADDUSER","PERSISTENCE","T1136.001","https://attack.mitre.org/techniques/T1136/001", SeverityLevel::High, true, create_local_account_adduser));

                event_rules.emplace_back(
                    std::make_tuple("CREATE_DOMAIN_ACCOUNT","PERSISTENCE","T1136.002","https://attack.mitre.org/techniques/T1136/002", SeverityLevel::High, true, create_domain_account));

                event_rules.emplace_back(
                    std::make_tuple("EVENT_TRIGGERED_EXECUTION_TRAP","PRIVILEGE ESCALATION, PERSISTENCE","T1546.005","https://attack.mitre.org/techniques/T1546/005", SeverityLevel::High, true, event_triggered_execution_trap));

                // event_rules.emplace_back(
                //     std::make_tuple("DYNAMIC_LINKER_HIJACKING","DEFENCE EVASION, PRIVILEGE ESCALATION, PERSISTENCE","T1574.006","https://attack.mitre.org/techniques/T1574/006", SeverityLevel::High, true, dynamic_linker_hijacking));

                event_rules.emplace_back(
                    std::make_tuple("SCHEDULING_TASK_AT","PRIVILEGE ESCALATION, PERSISTENCE, EXECUTION","T1053.002","https://attack.mitre.org/techniques/T1053/002", SeverityLevel::High, true, scheduled_task_at));

                event_rules.emplace_back(
                    std::make_tuple("SCHEDULING_TASK_ATRM","PRIVILEGE ESCALATION, PERSISTENCE, EXECUTION","T1053.002","https://attack.mitre.org/techniques/T1053/002", SeverityLevel::High, true, scheduled_task_atrm));

                event_rules.emplace_back(
                    std::make_tuple("SCHEDULING_TASK_CRONTAB","PRIVILEGE ESCALATION, PERSISTENCE","T1053.003","https://attack.mitre.org/techniques/T1053/003", SeverityLevel::High, true, scheduled_task_cron));

                event_rules.emplace_back(
                    std::make_tuple("SCHEDULING_TASK_SYSTEMD_TIMERS","PRIVILEGE ESCALATION, PERSISTENCE, EXECUTION","T1053.006","https://attack.mitre.org/techniques/T1053/006", SeverityLevel::High, true, scheduled_task_systemd_timers));

                // event_rules.emplace_back(
                //     std::make_tuple("SSH_AUTHORISED_KEYS","PERSISTENCE","T1098.004","https://attack.mitre.org/techniques/T1098/004", SeverityLevel::High, true, ssh_authorized_keys));

                event_rules.emplace_back(
                    std::make_tuple("ABUSE_ELEVATION_CONTROL_SETUID","DEFENCE EVASION, PRIVILEGE ESCALATION","T1548.001","https://attack.mitre.org/techniques/T1548/001", SeverityLevel::High, true, abuse_elevation_control_setuid));

                event_rules.emplace_back(
                    std::make_tuple("ABUSE_ELEVATION_CONTROL_SETUID1","DEFENCE EVASION, PRIVILEGE ESCALATION","T1548.001","https://attack.mitre.org/techniques/T1548/001", SeverityLevel::High, true, abuse_elevation_control_setuid1));

                event_rules.emplace_back(
                    std::make_tuple("ABUSE_ELEVATION_CONTROL_SUDO_CACHING","DEFENCE EVASION, PRIVILEGE ESCALATION","T1548.003","https://attack.mitre.org/techniques/T1548/003", SeverityLevel::High, true, abuse_elevation_control_sudo_caching));

                event_rules.emplace_back(
                    std::make_tuple("CREATE_MODIFY_SYSTEMD_SERVICE","PERSISTENCE","T1543.002","https://attack.mitre.org/techniques/T1543/002", SeverityLevel::High, true, create_modify_system_systemd_service));

                event_rules.emplace_back(
                    std::make_tuple("TERMINAL_DOWNLAODS_CURL","","","", SeverityLevel::High, true, terminal_downloads_curl));

                event_rules.emplace_back(
                    std::make_tuple("IMPAIR_COMMAND_HISTORY_LOGGING","","","", SeverityLevel::High, true, impair_command_history_logging));

                event_rules.emplace_back(
                    std::make_tuple("TERMINAL_DOWNLAODS_PYTHON","","","", SeverityLevel::High, true, terminal_downloads_python));

                event_rules.emplace_back(
                    std::make_tuple("BASH_HISTORY_COMMANDLINE","","","", SeverityLevel::High, true, bash_history_commandLine));

                event_rules.emplace_back(
                    std::make_tuple("PING_PRIVILEGED_COMMANDLINE","","","", SeverityLevel::High, true, ping_privileged_commandLine));

                event_rules.emplace_back(
                    std::make_tuple("MOUNT_PRIVILEGED_COMMANDLINE","","","", SeverityLevel::High, true, mount_privileged_commandLine));

                event_rules.emplace_back(
                    std::make_tuple("UMOUNT_PRIVILEGED_COMMANDLINE","","","", SeverityLevel::High, true, umount_privileged_commandLine));

                event_rules.emplace_back(
                    std::make_tuple("CHGRP_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, chgrp_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("PAM_TIMESTAMP_CHECK_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, pam_timestamp_check_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("UNIX_CHKPWD_CHECK_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, unix_chkpwd_check_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("PWCK_CHECK_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, pwck_check_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("USERHELPER_CHECK_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, userhelper_check_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("XORG_CHECK_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, Xorg_check_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("RLOGIN_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, rlogin_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("SUDOEDIT_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, sudoedit_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("RSH_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, rsh_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("GPASSWD_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, gpasswd_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("SUDO_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, sudo_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("STAPRUN_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, staprun_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("RCP_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, rcp_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("PASSWD_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, passwd_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("CHSH_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, chsh_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("CHFN_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, chfn_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("CHAGE_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, chage_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("SETFACL_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, setfacl_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("CHACL_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, chacl_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("CHCON_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, chcon_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("NEWGRP_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, newgrp_privileged_command));

                // event_rules.emplace_back(
                //         std::make_tuple("SLEEP_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, sleep_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("PGREP_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, pgrep_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("GREP_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, grep_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("LSPCI_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, lspci_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("UDEVADM_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, udevadm_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("FINDMNT_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, findmnt_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("NETSTAT_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, netstat_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("AWK_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, awk_privileged_command));

                // event_rules.emplace_back(
                //         std::make_tuple("SED_PRIVILEGED_COMMAND","","","", SeverityLevel::High, true, sed_privileged_command));

                event_rules.emplace_back(
                    std::make_tuple("SYSTED DETECT VIRTUAL ENV","DEFENCE EVASION, DISCOVERY","T1497.001","https://attack.mitre.org/techniques/T1497", SeverityLevel::High, true, virtualization_evasion_system_checks_systemd_detect_virt));

                event_rules.emplace_back(
                    std::make_tuple("SYSTED DETECT VIRTUAL ENV DMIDECODE","DEFENCE EVASION","T1497.001","https://attack.mitre.org/techniques/T1497/001", SeverityLevel::High, true, virtualization_evasion_system_checks_dmidecode));

                event_rules.emplace_back(
                    std::make_tuple("SET FILE ACCESS TIMESTAMP","DEFENCE EVASION","T1070.006","https://attack.mitre.org/techniques/T1070/006", SeverityLevel::High, true, set_file_access_timestamp));

                // event_rules.emplace_back(
                //         std::make_tuple("SET FILE MODIFICATION TIMESTAMP","","","", SeverityLevel::High, true, set_file_modification_timestamp));

                event_rules.emplace_back(
                    std::make_tuple("MODIFY FILE TIMESTAMP USING REFERENCE FILE","DEFENCE EVASION","T1070.006","https://attack.mitre.org/techniques/T1070/006", SeverityLevel::High, true, modify_file_timestamp_using_reference_file));

                event_rules.emplace_back(
                    std::make_tuple("SUDO CACHING SUDO USAGE","DEFENCE EVASION","T1070.006","https://attack.mitre.org/techniques/T1070/006", SeverityLevel::High, true, sudo_and_sudo_caching_sudo_usage));

                event_rules.emplace_back(
                    std::make_tuple("SUDO CACHING UNLIMITED SUDO TIMEOUT","DEFENCE EVASION","T1548.003","https://attack.mitre.org/techniques/T1548/003", SeverityLevel::High, true, sudo_and_sudo_caching_unlimited_sudo_timeout));
                //
                event_rules.emplace_back(
                    std::make_tuple("SUDO CACHING UNLIMITED SUDO TIMEOUT","DEFENCE EVASION, PRIVILEGE ESCALATION","T1548.003","https://attack.mitre.org/techniques/T1548/003", SeverityLevel::High, true, sudo_and_sudo_caching_disable_tty_tickets_sudo_caching));

                event_rules.emplace_back(
                    std::make_tuple("IMPAIR COMMAND HISTORY LOGGING DISABLE HISTORY COLLECTION","DEFENCE EVASION","T1562.003","https://attack.mitre.org/techniques/T1562/003", SeverityLevel::High, true, impair_cmd_history_logging_disable_history_collection));

                event_rules.emplace_back(
                    std::make_tuple("IMPAIR COMMAND HISTORY LOGGING Mac HISTCONTROLL","DEFENCE EVASION","T1562.003","https://attack.mitre.org/techniques/T1562/003", SeverityLevel::High, true, impair_cmd_history_logging_mac_hist_control));

                event_rules.emplace_back(
                    std::make_tuple("FILE DELETION SINGLE FILE","","","", SeverityLevel::High, true, file_deletion_single_file));

                event_rules.emplace_back(
                    std::make_tuple("FILE DELETION ENTIRE FOLDER","","","", SeverityLevel::High, true, file_deletion_entire_folder));

                event_rules.emplace_back(
                    std::make_tuple("OVERWRITE AND DELETE FILE WITH SHRED","","","", SeverityLevel::High, true, overwrite_and_delete_file_with_shred));

                event_rules.emplace_back(
                    std::make_tuple("DELETE FILESYSTEM","","","", SeverityLevel::High, true, delete_filesystem_root));

                event_rules.emplace_back(
                    std::make_tuple("MODIFYING_CRON_FILE_REFERENCE","","","", SeverityLevel::High, true, modifying_cron_file_reference));

                event_rules.emplace_back(
                    std::make_tuple("MODIFYING_CRON_DAILY_FILE","","","", SeverityLevel::High, true, modifying_cron_daily_file));

                event_rules.emplace_back(
                    std::make_tuple("MODIFYING_CRON_HOURLY_FILE","","","", SeverityLevel::High, true, modifying_cron_hourly_file));

                event_rules.emplace_back(
                    std::make_tuple("MODIFYING_CRON_MONTHLY_FILE","","","", SeverityLevel::High, true, modifying_cron_monthly_file));

                event_rules.emplace_back(
                    std::make_tuple("MODIFYING_CRON_WEEKLY_FILE","","","", SeverityLevel::High, true, modifying_cron_weekly_file));

                event_rules.emplace_back(
                    std::make_tuple("MODIFYING_CRON_VAR_FILE","","","", SeverityLevel::High, true, modifying_cron_var_file));

                event_rules.emplace_back(
                    std::make_tuple("SHARED_LIBRARY_INJECTION","","","", SeverityLevel::High, true, shared_library_injection));

                event_rules.emplace_back(
                    std::make_tuple("SHARED_LIBRARY_INJECTION_LD_PRELOAD","","","", SeverityLevel::High, true, shared_library_injection_ld_preload));

                event_rules.emplace_back(
                    std::make_tuple("LOAD_KERNEL_MODULE_INSMOD","","","", SeverityLevel::High, true, load_kernel_module_insmod));

                event_rules.emplace_back(
                    std::make_tuple("PAM_CONFIG_FILE_MODIFY","DEFENCE EVASION, PERSISTENCE, CREDENTIAL ACCESS","T1556.003","https://attack.mitre.org/techniques/T1556/003", SeverityLevel::High, true, pam_config_file_modify));

                event_rules.emplace_back(
                    std::make_tuple("RULE_ADD_PAM_CONFIG","","","", SeverityLevel::High, true, rule_add_pam_config));

                event_rules.emplace_back(
                    std::make_tuple("RC_SCRIPT_COMMON_MIDIFY","PRIVILEGE ESCALATION, PERSISTENCE","T1037.004","https://attack.mitre.org/techniques/T1037/004", SeverityLevel::High, true, rc_script_common_midify));

                event_rules.emplace_back(
                    std::make_tuple("RC_SCRIPT_LOCAL_MIDIFY","PRIVILEGE ESCALATION","T1037.004","https://attack.mitre.org/techniques/T1037/004", SeverityLevel::High, true, rc_script_local_midify));

                event_rules.emplace_back(
                    std::make_tuple("SSH_AUTHORIZED_KEYS","","","", SeverityLevel::High, true, ssh_authorized_keys_midify));

                event_rules.emplace_back(
                    std::make_tuple("CREATE_SYSTEMD_SERVICE_PATH","","","", SeverityLevel::High, true, create_systemd_service_path));

                event_rules.emplace_back(
                    std::make_tuple("CREATE_SYSTEMD_SERVICE_FILE","PRIVILEGE ESCALATION","T1543.002","https://attack.mitre.org/techniques/T1543/002", SeverityLevel::High, true, create_systemd_service_file));

                event_rules.emplace_back(
                    std::make_tuple("CREATE_SYSTEMD_SERVICE__TIMER_SERVICE","","","", SeverityLevel::High, true, create_systemd_service__timer_service));

                event_rules.emplace_back(
                    std::make_tuple("CREATE_SYSTEMD_SERVICE__TIMER_FILE","","","", SeverityLevel::High, true, create_systemd_service__timer_file));

                // event_rules.emplace_back(
                //     std::make_tuple("LATERAL_MOVEMENT_WITH_SECURE_SHELL","","","", SeverityLevel::High, true, lateral_movement_with_secure_shell));

                event_rules.emplace_back(
                    std::make_tuple("LATERAL_MOVEMENT_WITH_SSH_RSA","","","", SeverityLevel::High, true, lateral_movement_with_ssh_rsa));

                event_rules.emplace_back(
                    std::make_tuple("LATERAL_TOOL_TRANSFER_FILES","","","", SeverityLevel::High, true, lateral_tool_transfer_files));

                // event_rules.emplace_back(
                //     std::make_tuple("DYNAMIC_LINKER_HIJACKING_LD_SO_PRELOAD","","","", SeverityLevel::High, true, shared_library_injection_ld_so_preload));
                
                event_rules.emplace_back(
                        std::make_tuple("DISABLE_SYSLOG","DEFENCE EVASION","T1562.001","https://attack.mitre.org/techniques/T1562/001", SeverityLevel::High, true, disable_syslog));
                
                event_rules.emplace_back(
                        std::make_tuple("DISABLE_CB_RESPONSE","DEFENCE EVASION","T1562.001","https://attack.mitre.org/techniques/T1562/001", SeverityLevel::High, true, disable_cb_response));
                
                event_rules.emplace_back(
                        std::make_tuple("STOP_CROWDSTRIKE_FALCON","DEFENCE EVASION","T1562.001","https://attack.mitre.org/techniques/T1562/001", SeverityLevel::High, true, stop_crowdstrike_falcon));
                
                event_rules.emplace_back(
                        std::make_tuple("STOP_START_UFW_FIREWALL","DEFENCE EVASION","T1562.004","https://attack.mitre.org/techniques/T1562/004", SeverityLevel::High, true, stop_start_ufw_firewall));
                
                event_rules.emplace_back(
                        std::make_tuple("STOP_START_UFW_FIREWALL_SYSTEMCTL","DEFENCE EVASION","T1562.004","https://attack.mitre.org/techniques/T1562/004", SeverityLevel::High, true, stop_start_ufw_firewall_systemctl));
                
                event_rules.emplace_back(
                        std::make_tuple("TURN_OFF_UFW_LOGGING","DEFENCE EVASION","T1562.004","https://attack.mitre.org/techniques/T1562/004", SeverityLevel::High, true, turn_off_ufw_logging));
                
                event_rules.emplace_back(
                        std::make_tuple("ADD_DELETE_UFW_RULES","DEFENCE EVASION","T1562.004","https://attack.mitre.org/techniques/T1562/004", SeverityLevel::High, true, add_delete_ufw_rules));
                
                event_rules.emplace_back(
                        std::make_tuple("EDIT_UFW_RULES_USER_DOT_RULES_FILE","DEFENCE EVASION","T1562.004","https://attack.mitre.org/techniques/T1562/004", SeverityLevel::High, true, edit_ufw_user_rules_file));
                
                event_rules.emplace_back(
                        std::make_tuple("EDIT_UFW_RULES_UFW_DOT_CONF_FILE","DEFENCE EVASION","T1562.004","https://attack.mitre.org/techniques/T1562/004", SeverityLevel::High, true, edit_ufw_conf_file));
                
                event_rules.emplace_back(
                        std::make_tuple("EDIT_UFW_RULES_SYSCTL_DOT_CONF_FILE","DEFENCE EVASION","T1562.004","https://attack.mitre.org/techniques/T1562/004", SeverityLevel::High, true, edit_ufw_rules_sysctl_conf_file));
                
                event_rules.emplace_back(
                        std::make_tuple("EDIT_UFW_FIREWALL_MAIN_CONFIG_FILE","DEFENCE EVASION","T1562.004","https://attack.mitre.org/techniques/T1562/004", SeverityLevel::High, true, edit_ufw_firewall_main_config_file));
                
                event_rules.emplace_back(
                        std::make_tuple("TAIL_UFW_FIREWALL_LOG_FILE","DEFENCE EVASION","T1562.004","https://attack.mitre.org/techniques/T1562/004", SeverityLevel::High, true, tail_ufw_firewall_log_file));
                
                event_rules.emplace_back(
                        std::make_tuple("BASE_64_DECODING_WITH_PYTHON","DEFENCE EVASION","T1140","https://attack.mitre.org/techniques/T1140", SeverityLevel::High, true, base64_decoding_python));
                
                event_rules.emplace_back(
                        std::make_tuple("BASE_64_DECODING_WITH_PERL","DEFENCE EVASION","T1140","https://attack.mitre.org/techniques/T1140", SeverityLevel::High, true, base64_decoding_perl));
                
                event_rules.emplace_back(
                        std::make_tuple("BASE_64_DECODING_WITH_SHELL_UTILITIES","DEFENCE EVASION","T1140","https://attack.mitre.org/techniques/T1140", SeverityLevel::High, true, base64_decoding_shell_utilities));
                
                event_rules.emplace_back(
                        std::make_tuple("HEX_DECODING_WITH_SHELL_UTILITIES","DEFENCE EVASION","T1140","https://attack.mitre.org/techniques/T1140", SeverityLevel::High, true, hex_decoding_shell_utilities));
                
                // event_rules.emplace_back(
                //         std::make_tuple("COMPILE_AFTER_DELIVERY_C_COMPILE","DEFENCE EVASION","T1027.004","https://attack.mitre.org/techniques/T1027/004", SeverityLevel::High, true, compile_after_delivery_c_compile));
                
                event_rules.emplace_back(
                        std::make_tuple("COMPILE_AFTER_DELIVERY_GO_COMPILE","DEFENCE EVASION","T1027.004","https://attack.mitre.org/techniques/T1027/004", SeverityLevel::High, true, compile_after_delivery_go_compile));
                
                event_rules.emplace_back(
                        std::make_tuple("CREATING_GCP_SERVICE_ACCOUNT_AND_KEY","","","", SeverityLevel::High, true, creating_gcp_service_account_and_key));
                
                event_rules.emplace_back(
                        std::make_tuple("CLEAR_LINUX_LOGS","DEFENCE EVASION","T1070.002","https://attack.mitre.org/techniques/T1070/002", SeverityLevel::High, true, clear_linux_logs_rm_rf));
                
                event_rules.emplace_back(
                        std::make_tuple("OVERWRITE_LINUX_MAIL_SPOOL_AND_LOGS","DEFENCE EVASION","T1070.002","https://attack.mitre.org/techniques/T1070/002", SeverityLevel::High, true, overwrite_linux_mail_spool_and_logs));
                        
                // event_rules.emplace_back(
                //         std::make_tuple("SCHEDULE_A_JOB_LINUX_AT","","","", SeverityLevel::High, true, at_schedule_a_job));
                        
                // event_rules.emplace_back(
                //         std::make_tuple("SYSTEM_OWNER_USER_DISCOVERY","DISCOVERY","T1033","https://attack.mitre.org/techniques/T1033", SeverityLevel::High, true, system_owner_user_discovery));
                
                event_rules.emplace_back(
                        std::make_tuple("ENUMERATE_ALL_ACCOUNTS_LOCAL","DISCOVERY","T1087.001","https://attack.mitre.org/techniques/T1087/001", SeverityLevel::High, true, enumerate_all_accounts_local));
                
                event_rules.emplace_back(
                        std::make_tuple("VIEW_SUDOERS_ACCESS","DISCOVERY","T1087.001","https://attack.mitre.org/techniques/T1087/001", SeverityLevel::High, true, view_sudoers_file));
                
                event_rules.emplace_back(
                        std::make_tuple("VIEW_ACCOUNTS_WITH_UID_0","DISCOVERY","T1087.001","https://attack.mitre.org/techniques/T1087/001", SeverityLevel::High, true, view_accounts_with_uid_0));
                
                event_rules.emplace_back(
                        std::make_tuple("LIST_OPENED_FILES_BY_USER","DISCOVERY","T1087.001","https://attack.mitre.org/techniques/T1087/001", SeverityLevel::High, true, list_opened_files_by_user));
                
                event_rules.emplace_back(
                        std::make_tuple("SHOW_IF_A_USER_ACCOUNT_HAS_EVER_LOGGED_IN_REMOTELY","DISCOVERY","T1087.001","https://attack.mitre.org/techniques/T1087/001", SeverityLevel::High, true, show_if_a_user_account_has_ever_logged_in_remotely));
                
                event_rules.emplace_back(
                        std::make_tuple("ENUMERATE_USERS_AND_GROUPS","DISCOVERY","T1087.001","https://attack.mitre.org/techniques/T1087/001", SeverityLevel::High, true, enumerate_users_and_groups));
                
                event_rules.emplace_back(
                        std::make_tuple("SYSTEM_SERVICE_DISCOVERY_SYSTEMCTL","DISCOVERY","T1007","https://attack.mitre.org/techniques/T1007", SeverityLevel::High, true, system_service_discovery_systemctl));
                
                event_rules.emplace_back(
                        std::make_tuple("PACKET_CAPTURE_LINUX","DISCOVERY","T1040","https://attack.mitre.org/techniques/T1040", SeverityLevel::High, true, packet_capture_linux));
                
                event_rules.emplace_back(
                        std::make_tuple("NETWORK_SHARE_DISCOVERY","DISCOVERY","T1135","https://attack.mitre.org/techniques/T1135", SeverityLevel::High, true, network_share_discovery));
                
                event_rules.emplace_back(
                        std::make_tuple("EXAMINE_PASSWORD_COMPLEXITY_POLICY","DISCOVERY","T1201","https://attack.mitre.org/techniques/T1201", SeverityLevel::High, true, examine_password_complexity_policy));

                event_rules.emplace_back(
                        std::make_tuple("LIST_OS_INFORMATION","DISCOVERY","T1082","https://attack.mitre.org/techniques/T1082", SeverityLevel::High, true, list_os_information));
                
                event_rules.emplace_back(
                        std::make_tuple("LINUX_VM_CHECK_VIA_HARWARE","DISCOVERY","T1082","https://attack.mitre.org/techniques/T1082", SeverityLevel::High, true, linux_vm_check_via_hardware));
                
                event_rules.emplace_back(
                        std::make_tuple("LINUX_VM_CHECK_VIA_KERNEL_MODULES","DISCOVERY","T1082","https://attack.mitre.org/techniques/T1082", SeverityLevel::High, true, linux_vm_check_via_kernel_modules));
                
                // event_rules.emplace_back(
                //         std::make_tuple("HOSTNAME_DISCOVERY","DISCOVERY","T1082","https://attack.mitre.org/techniques/T1082", SeverityLevel::High, true, hostname_discovery));
                
                event_rules.emplace_back(
                        std::make_tuple("ENVIRONMENT_VARIABLES_DISCOVERY","DISCOVERY","T1082","https://attack.mitre.org/techniques/T1082", SeverityLevel::High, true, environment_variables_discovery));
                
                event_rules.emplace_back(
                        std::make_tuple("LIST_MOZILLA_FIREFOX_BOOKMARK_DATABASE_FILES","DISCOVERY","T1217","https://attack.mitre.org/techniques/T1217", SeverityLevel::High, true, list_mozilla_firefox_bookmark_database_files));
                
                event_rules.emplace_back(
                        std::make_tuple("SYSTEM_NETWORK_CONFIGURATION_DISCOVERY","DISCOVERY","T1016","https://attack.mitre.org/techniques/T1016", SeverityLevel::High, true, system_network_configuration_discovery));
                
                // event_rules.emplace_back(
                //         std::make_tuple("NIX_FILE_AND_DIRECTORY_DISCOVERY","DISCOVERY","T1083","https://attack.mitre.org/techniques/T1083", SeverityLevel::High, true, nix_file_and_directory_discovery));
                
                event_rules.emplace_back(
                        std::make_tuple("SYSTEM_NETWORK_CONNECTIONS_DISCOVERY","DISCOVERY","T1049","https://attack.mitre.org/techniques/T1049", SeverityLevel::High, true, system_network_connections_discovery));
                
                // event_rules.emplace_back(
                //         std::make_tuple("PROCESS_DISCOVERY","DISCOVERY","T1057","https://attack.mitre.org/techniques/T1057", SeverityLevel::Low, true, process_discovery));
                
                // event_rules.emplace_back(
                //         std::make_tuple("PERMISSION_GROUPS_DISCOVERY_LOCAL","DISCOVERY","T1069.001","https://attack.mitre.org/techniques/T1069/001", SeverityLevel::High, true, permission_groups_discovery_local));
                
                event_rules.emplace_back(
                        std::make_tuple("SECURITY_SOFTWARE_DISCOVERY","DISCOVERY","T1518.001","https://attack.mitre.org/techniques/T1518/001", SeverityLevel::High, true, security_software_discovery));
                
                event_rules.emplace_back(
                        std::make_tuple("REMOTE_SYSTEM_DISCOVERY_IPNEIGHBOUR","DISCOVERY","T1018","https://attack.mitre.org/techniques/T1018", SeverityLevel::High, true, remote_system_discovery_ipneighbour));
                
                event_rules.emplace_back(
                        std::make_tuple("PORT_SCAN","DISCOVERY","T1046","https://attack.mitre.org/techniques/T1046", SeverityLevel::High, true, port_scan));
                
                event_rules.emplace_back(
                        std::make_tuple("ENCRYPT_FILE_USING_GPG","IMPACT","T1486","https://attack.mitre.org/techniques/T1486", SeverityLevel::High, true, encrypt_file_using_gpg));
                
                event_rules.emplace_back(
                        std::make_tuple("ENCRYPT_FILE_USING_7Z","IMPACT","T1486","https://attack.mitre.org/techniques/T1486", SeverityLevel::High, true, encrypt_file_using_7z));
                
                event_rules.emplace_back(
                        std::make_tuple("ENCRYPT_FILE_USING_OPENSSL","IMPACT","T1486","https://attack.mitre.org/techniques/T1486", SeverityLevel::High, true, encrypt_file_using_openssl));
                
                event_rules.emplace_back(
                        std::make_tuple("OVERWRITE_FILE_WITH_DD","IMPACT","T1485","https://attack.mitre.org/techniques/T1485", SeverityLevel::High, true, overwrite_file_with_dd));
                
                event_rules.emplace_back(
                        std::make_tuple("SYSTEM_SHUTDOWN_REBOOT","IMPACT","T1529","https://attack.mitre.org/techniques/T1529", SeverityLevel::High, true, system_shutdown_reboot));
                
                event_rules.emplace_back(
                        std::make_tuple("EXFILTRATE_DATA_HTTPS_USING_CURL","","","", SeverityLevel::High, true, exfiltrate_data_https_using_curl));

                event_rules.emplace_back(
                    std::make_tuple("DATA_COMPRESSED_ZIP","COLLECTION","T1560.001","https://attack.mitre.org/techniques/T1560/001", SeverityLevel::High, true, data_compressed_zip));
                
                event_rules.emplace_back(
                        std::make_tuple("DATA_COMPRESSED_ZIP_NIX_GZIP","COLLECTION","T1560.001","https://attack.mitre.org/techniques/T1560/001", SeverityLevel::High, true, data_compressed_zip_nix_gzip));
                
                event_rules.emplace_back(
                        std::make_tuple("DATA_COMPRESSED_ZIP_NIX_TAR","COLLECTION","T1560.001","https://attack.mitre.org/techniques/T1560/001", SeverityLevel::High, true, data_compressed_zip_nix_tar));
                
                event_rules.emplace_back(
                        std::make_tuple("X_WINDOWS_CAPTURE","COLLECTION","T1113","https://attack.mitre.org/techniques/T1113", SeverityLevel::High, true, x_windows_capture));
                
                event_rules.emplace_back(
                        std::make_tuple("CAPTURE_LINUX_DESKTOP_USING_IMPORT_TOOL","COLLECTION","T1113","https://attack.mitre.org/techniques/T1113", SeverityLevel::High, true, capture_linux_desktop_using_import_tool));
                
                event_rules.emplace_back(
                        std::make_tuple("TERMINAL_INPUT_CAPTURE_LINUX_WITH_PAM_D","COLLECTION","T1056.001","https://attack.mitre.org/techniques/T1056/001", SeverityLevel::High, true, terminal_input_capture_linux_with_pam_d));
                
                event_rules.emplace_back(
                        std::make_tuple("LOGGING_BASH_HISTORY_TO_SYSLOG","COLLECTION","T1056.001","https://attack.mitre.org/techniques/T1056/001", SeverityLevel::High, true, logging_bash_history_to_syslog));
                
                event_rules.emplace_back(
                        std::make_tuple("SSHD_PAM_KEYLOGGER","COLLECTION","T1056.001","https://attack.mitre.org/techniques/T1056/001", SeverityLevel::High, true, sshd_pam_keylogger));
                
                event_rules.emplace_back(
                        std::make_tuple("STAGE_DATA_FROM_DISCOVERY_SH","COLLECTION","T1074.001","https://attack.mitre.org/techniques/T1074/001", SeverityLevel::High, true, stage_data_from_discovery_sh));

                event_rules.emplace_back(
                        std::make_tuple("ADD_OR_COPY_CONTENT_TO_CLIPBOARD_WITH_XCLIP","COLLECTION","T1115","https://attack.mitre.org/techniques/T1115", SeverityLevel::High, true, add_or_copy_content_to_clipboard_with_xclip));
                
                event_rules.emplace_back(
                        std::make_tuple("COMPRESSING_DATA_USING_GZIP_IN_PYTHON","COLLECTION","T1560.002","https://attack.mitre.org/techniques/T1560/002", SeverityLevel::High, true, compressing_data_using_gzip_in_python));
                
                event_rules.emplace_back(
                        std::make_tuple("COMPRESSING_DATA_USING_BZ2_IN_PYTHON","COLLECTION","T1560.002","https://attack.mitre.org/techniques/T1560/002", SeverityLevel::High, true, compressing_data_using_bz2_in_python));
                
                event_rules.emplace_back(
                        std::make_tuple("COMPRESSING_DATA_USING_ZIPFILE_IN_PYTHON","COLLECTION","T1560.002","https://attack.mitre.org/techniques/T1560/002", SeverityLevel::High, true, compressing_data_using_zipfile_in_python));
                // event_rules.emplace_back(
                //        std::make_tuple("NON_STANDARD_PORT_COMMAND_CONTROL","","","", SeverityLevel::High, true, non_standard_port_command_control));
                
                event_rules.emplace_back(
                        std::make_tuple("HARDWARE_ADDITIONS","INITIAL ACCESS","T1200","https://attack.mitre.org/techniques/T1200/", SeverityLevel::High, true, hardware_additions));
                 
                event_rules.emplace_back(                       
                        std::make_tuple("INJECT_LD_PRELOAD","INITIAL ACCESS","T1190","https://attack.mitre.org/tactics/TA0001/", SeverityLevel::High, true, inject_ld_preload));

                event_rules.emplace_back(
                        std::make_tuple("MANIPULATION_SSH_AUTHORIZED_KEY","PERSISTENCE","T1098-004","https://attack.mitre.org/techniques/T1098/004/", SeverityLevel::High, true, manipulation_ssh_authorized_key));
                 
                event_rules.emplace_back(                       
                        std::make_tuple("SHELL_CONFIG_MODIFY","PERSISTENCE","T1546-004","https://attack.mitre.org/techniques/T1546/004/", SeverityLevel::High, true, shell_config_modify));

                // event_rules.emplace_back(                       
                //         std::make_tuple("DYNAMIC_LINKER_HIJACKING","PERSISTENCE","T1574.006","https://attack.mitre.org/techniques/T1574/006/", SeverityLevel::High, true, dynamic_linker_hijacking));
                    
                event_rules.emplace_back(                       
                        std::make_tuple("PLUGGABLE_AUTHENTICATION_MODULES_FILE","PERSISTENCE","T1556.003","https://attack.mitre.org/techniques/T1556/003/", SeverityLevel::High, true, pluggable_authentication_modules_file));

                event_rules.emplace_back(                       
                        std::make_tuple("SCHEDULED_TASK_AT_FILE","PERSISTENCE","T1053.002","https://attack.mitre.org/techniques/T1053/002/", SeverityLevel::High, true, scheduled_task_at_file));

                event_rules.emplace_back(                       
                        std::make_tuple("SCHEDULED_TASK_CRON_FILE","PERSISTENCE","T1053.003","https://attack.mitre.org/techniques/T1053/003/", SeverityLevel::High, true, scheduled_task_cron_file));

                event_rules.emplace_back(                       
                        std::make_tuple("SCHEDULED_TASK_SYSTEMD_TIMERS_FILE","PERSISTENCE","T1053.006","https://attack.mitre.org/techniques/T1053/006/", SeverityLevel::High, true, scheduled_task_systemd_timers_file));

                event_rules.emplace_back(
                        std::make_tuple("MALICIOUS_PAM_RULES","Credential Access","T1556.003","https://attack.mitre.org/techniques/T1556/003", SeverityLevel::High, true, malicious_pam_rules));
                
                event_rules.emplace_back(
                       std::make_tuple("INPUT_CAPTURE_KEYLOGGING","Credential Access","T1056.001","https://attack.mitre.org/techniques/T1056/001", SeverityLevel::High, true, input_capture_keylogging));
                
                event_rules.emplace_back(
                       std::make_tuple("SSHD PAM KEYLOGGER","Credential Access","T1056.001","https://attack.mitre.org/techniques/T1056/001", SeverityLevel::High, true, sshd_pam_keylogger_keylogging));
                
                event_rules.emplace_back(
                       std::make_tuple("DUMP_CREDENTIALS_FROM_WEB_BROWSERS","Credential Access","T1555.003","https://attack.mitre.org/techniques/T1555/003", SeverityLevel::High, true, dump_credentials_from_web_browsers));
                
                event_rules.emplace_back(
                       std::make_tuple("DISCOVER_PRIVATE_SSH_KEYS","Credential Access","T1552.004","https://attack.mitre.org/techniques/T1552/004", SeverityLevel::High, true, discover_private_ssh_keys));
                
                event_rules.emplace_back(
                       std::make_tuple("COPY_PRIVATE_SSH_KEYS_WITH_CP","Credential Access","T1552.004","https://attack.mitre.org/techniques/T1552/004", SeverityLevel::High, true, copy_private_ssh_keys_with_cp));
                
                event_rules.emplace_back(
                       std::make_tuple("EXTRACT_PASSWORDS_WITH_GREP","Credential Access","T1552.001","https://attack.mitre.org/techniques/T1552/001", SeverityLevel::High, true, extract_passwords_with_grep));
                
                event_rules.emplace_back(
                       std::make_tuple("EXTRACT_PASSWORDS_WITH_GREP","Credential Access","T1003.008","https://attack.mitre.org/techniques/T1003/008", SeverityLevel::High, true, access_etc_shadow_passwd));
                
              
        }

        ~ProcessEventProcessor() override
        {
                event_rules.clear();
        }

        int load() override
        {
                return 0;
        }

        int unload() override
        {
                return 0;
        }

        PluginInfo version() override
        {
                PluginInfo info;

                info.name = "Process Event Processor";
                info.version_revision = 0;
                info.version_minor = 1;
                info.version_major = 0;
                info.publisher = "self";
                info.version_string = "-dev";

                return info;
        }

        std::vector<std::tuple<std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent &, Event &)>>> process_event_rules() override{
                return this->event_rules;
        }
};

extern "C"
{
        void *module_init()
        {
                ProcessEventProcessor *obj = new ProcessEventProcessor();
                return reinterpret_cast<void *>(obj);
        }

        void module_exit(void *ptr)
        {
                if (ptr)
                        delete reinterpret_cast<ProcessEventProcessor *>(ptr);
        }
}
