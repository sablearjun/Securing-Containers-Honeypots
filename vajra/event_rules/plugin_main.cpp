#include "../common/plugin_base.h"
#include "../common/EdrDataTypes.h"
#include "detection_rules.h"
#include "win_detection_rules.h"
#include "gtfobins_rules.h"
#include "collection_rules.h"
#include "command_control_rules.h"
#include "credential_access_rules.h"
#include "exfiltration_rules.h"
#include "win_impact_rules.h"
#include "win_execution_rules.h"
#include "win_defence_evasion_rules.h"
#include "win_privilege_escalation_rules.h"
#include "win_collection_rules.h"
#include "win_discovery_rules.h"
#include "win_command_control_rules.h"
#include "win_credential_access_rules.h"
#include "win_persistence_rules.h"
#include "win_initial_access_rules.h"
#include "win_exfilteration_rules.h"
#include "win_lateral_movement.h"

class
    ProcessEventProcessor : public AbstractDetectionPlugin
{
private:
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent &, Event &)>>> event_rules;
    // std::vector<std::tuple<std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const WinProcessEvent &, Event &)>>> win_event_rules;
public:
    ProcessEventProcessor()
    {
         // Linux
        event_rules.emplace_back(
            std::make_tuple("WGET_DOWNLOAD_IN_TMP", "", "", "", "Linux", SeverityLevel::Medium, true, wget_download_tmp));
        event_rules.emplace_back(
            std::make_tuple("GET_DOWNLOAD_IN_TMP", "", "", "", "Linux", SeverityLevel::Medium, true, get_download_tmp));
        event_rules.emplace_back(
            std::make_tuple("CPULIMIT_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, cpulimit_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("DMESG_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, dmesg_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("DOCKER_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, docker_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("DPKG_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, dpkg_spawn_shell));
        // event_rules.emplace_back(
        //     std::make_tuple("ENV_SPAWNS_SHELL","GTFOBINS","","","Linux", SeverityLevel::Medium, true, env_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("FIND_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, find_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("FLOCK_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, flock_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("APT_GET_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, apt_get_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("APT_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, apt_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("ASH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ash_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("AWK_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, awk_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("BUNDLER_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, bundler_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("BUSCTL_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, busctl_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("BYEBUG_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, byebug_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("CPAN_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, cpan_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("IONICE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ionice_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("JOURNALCTL_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, journalctl_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("KSH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ksh_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("LESS_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, less_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("LOGSAVE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, logsave_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("LTRACE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ltrace_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("LUA_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, lua_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("MAN_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, man_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("MAWK_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, mawk_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("MYSQL_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, mysql_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("NANO_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, nano_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("NICE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, nice_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("NMAP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, nmap_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("NOHUP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, nohup_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("NROFF_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, nroff_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("NSENTER_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, nsenter_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("PERL_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, perl_spawn_shell));

        event_rules.emplace_back(
            std::make_tuple("FTP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ftp_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("GIT_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, git_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("GAWK_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, gawk_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("GCC_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, gcc_spawn_shell));
        // event_rules.emplace_back(
        //     std::make_tuple("GDB_SPAWNS_SHELL","GTFOBINS","","","Linux", SeverityLevel::Medium, true, gdb_spawn_shell));

        event_rules.emplace_back(
            std::make_tuple("TAR_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, tar_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("TASKSET_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, taskset_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("TIME_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, time_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("TIMEOUT_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, timeout_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("TMUX_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, tmux_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("WATCH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, watch_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("VI_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, vi_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("VIM_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, vim_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("VIEW_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, view_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("XARGS_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, xargs_spawn_shell));

        event_rules.emplace_back(
            std::make_tuple("ZIP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, zip_spawn_shell));

        event_rules.emplace_back(
            std::make_tuple("STRACE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, strace_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("STDBUF_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, stdbuf_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("STARTSTOPDAEMON_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, startstopdaemon_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("SSH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ssh_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("SPLIT_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, split_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("SQLITE3_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, sqlite3_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("SETARCH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, setarch_spawn_shell));
        event_rules.emplace_back(std::make_tuple("SED_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, sed_spawn_shell));
        event_rules.emplace_back(std::make_tuple("SLSH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, slsh_spawn_shell));
        event_rules.emplace_back(std::make_tuple("SOCAT_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, socat_spawn_shell));
        event_rules.emplace_back(std::make_tuple("SCRIPT_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, script_spawn_shell));
        event_rules.emplace_back(std::make_tuple("SCREEN_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, screen_spawn_shell));
        event_rules.emplace_back(std::make_tuple("PUPPET_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, puppet_spawn_shell));
        event_rules.emplace_back(std::make_tuple("RUNMAILCAP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, runmailcap_spawn_shell));
        event_rules.emplace_back(std::make_tuple("RAKE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, rake_spawn_shell));
        event_rules.emplace_back(std::make_tuple("RLWRAP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, rlwrap_spawn_shell));
        event_rules.emplace_back(std::make_tuple("RPM_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, rpm_spawn_shell));
        event_rules.emplace_back(std::make_tuple("PIC_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, pic_spawn_shell));
        event_rules.emplace_back(std::make_tuple("SCP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, scp_spawn_shell));
        event_rules.emplace_back(std::make_tuple("PHP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, php_spawn_shell));
        event_rules.emplace_back(std::make_tuple("UNSHARE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, unshare_spawn_shell));

        // Arjun
        event_rules.emplace_back(
            std::make_tuple("AT_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, at_spawn_shell));
        // event_rules.emplace_back(
        //         std::make_tuple("BASH_SPAWNS_SHELL","GTFOBINS","","Linux", SeverityLevel::Medium, true, bash_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("CAPSH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, capsh_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("COWSAY_SPAWNS_SHELL", "GTFOBINS", "Cowsay", "https://gtfobins.github.io/gtfobins/cowsay/", "Linux", SeverityLevel::Medium, true, cowsay_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("CRASH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, crash_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("CSH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, csh_spawn_shell));
        // event_rules.emplace_back(
        //         std::make_tuple("DASH_SPAWNS_SHELL","GTFOBINS","","","Linux", SeverityLevel::Medium, true, dash_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("ED_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ed_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("EMAC_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, emacs_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("EX_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ex_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("EXPECT_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, expect_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("FACTER_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, facter_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("GHC_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ghc_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("GHCI_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, ghci_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("HPING_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, hping_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("GTESTER_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, gtester_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("GIMP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, gimp_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("IRB_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, irb_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("JJS_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, jjs_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("MAIL_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, mail_spawn_shell));
        // event_rules.emplace_back(
        //     std::make_tuple("MAKE_SPAWNS_SHELL","GTFOBINS","","","Linux", SeverityLevel::Medium, true, make_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("MORE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, more_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("NAWK_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, nawk_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("NODE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, node_spawn_shell));
        // event_rules.emplace_back(

        //     std::make_tuple("PICO_SPAWNS_SHELL","GTFOBINS","","","Linux", SeverityLevel::Medium, true, pico_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("ZYPPER_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, zypper_spawn_shell));
        // event_rules.emplace_back(
        //     std::make_tuple("ZSH_SPAWNS_SHELL","GTFOBINS","","","Linux", SeverityLevel::Medium, true, zsh_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("VALGRIND_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, valgrind_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("TOP_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, top_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("TCLSH_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, tclsh_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("SERVICE_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, service_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("RVIEW_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, rview_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("RUN-PARTS_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, run_parts_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("PYTHON_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, python_spawn_shell));
        event_rules.emplace_back(
            std::make_tuple("PSQL_SPAWNS_SHELL", "GTFOBINS", "", "", "Linux", SeverityLevel::Medium, true, psql_spawn_shell));

        // Sunil

        event_rules.emplace_back(
            std::make_tuple("KERNAL_MODULES_EXTENSIONS_MODPROBE", "PRIVILEGE ESCALATION, PERSISTENCE", "T1547.006", "https://attack.mitre.org/techniques/T1547/006", "Linux", SeverityLevel::Medium, true, kernel_modules_extensions_modprobe));

        event_rules.emplace_back(
            std::make_tuple("KERNAL_MODULES_EXTENSIONS_INSMOD", "PRIVILEGE ESCALATION, PERSISTENCE", "T1547.006", "https://attack.mitre.org/techniques/T1547/006", "Linux", SeverityLevel::Medium, true, kernel_modules_extensions_insmod));

        event_rules.emplace_back(
            std::make_tuple("KERNAL_MODULES_EXTENSIONS_LSMOD", "PRIVILEGE ESCALATION, PERSISTENCE", "T1547.006", "https://attack.mitre.org/techniques/T1547/006", "Linux", SeverityLevel::Medium, true, kernel_modules_extensions_lsmod));

        event_rules.emplace_back(
            std::make_tuple("KERNAL_MODULES_EXTENSIONS_RMMOD", "PRIVILEGE ESCALATION, PERSISTENCE", "T1547.006", "https://attack.mitre.org/techniques/T1547/006", "Linux", SeverityLevel::Medium, true, kernel_modules_extensions_rmmod));

        event_rules.emplace_back(
            std::make_tuple("CREATE_LOCAL_ACCOUNT_USERADD", "PERSISTENCE", "T1136.001", "https://attack.mitre.org/techniques/T1136/001", "Linux", SeverityLevel::High, true, create_local_account_useradd));

        event_rules.emplace_back(
            std::make_tuple("CREATE_LOCAL_ACCOUNT_ADDUSER", "PERSISTENCE", "T1136.001", "https://attack.mitre.org/techniques/T1136/001", "Linux", SeverityLevel::High, true, create_local_account_adduser));

        event_rules.emplace_back(
            std::make_tuple("CREATE_DOMAIN_ACCOUNT", "PERSISTENCE", "T1136.002", "https://attack.mitre.org/techniques/T1136/002", "Linux", SeverityLevel::Low, true, create_domain_account));

        event_rules.emplace_back(
            std::make_tuple("EVENT_TRIGGERED_EXECUTION_TRAP", "PRIVILEGE ESCALATION, PERSISTENCE", "T1546.005", "https://attack.mitre.org/techniques/T1546/005", "Linux", SeverityLevel::High, true, event_triggered_execution_trap));

        event_rules.emplace_back(
            std::make_tuple("DYNAMIC_LINKER_HIJACKING", "DEFENCE EVASION, PRIVILEGE ESCALATION, PERSISTENCE", "T1574.006", "https://attack.mitre.org/techniques/T1574/006", "Linux", SeverityLevel::High, true, dynamic_linker_hijacking));

        event_rules.emplace_back(
            std::make_tuple("SCHEDULING_TASK_AT", "PRIVILEGE ESCALATION, PERSISTENCE, EXECUTION", "T1053.002", "https://attack.mitre.org/techniques/T1053/002", "Linux", SeverityLevel::High, true, scheduled_task_at));

        event_rules.emplace_back(
            std::make_tuple("SCHEDULING_TASK_ATRM", "PRIVILEGE ESCALATION, PERSISTENCE, EXECUTION", "T1053.002", "https://attack.mitre.org/techniques/T1053/002", "Linux", SeverityLevel::High, true, scheduled_task_atrm));

        // event_rules.CHEDULING_TASK_CRONTAB","PRIVILEGE ESCALATION, PERSISTENCE","T1053.003","https://attack.mitre.org/techniques/T1053/003","Linux", SeverityLevel::Medium, true, scheduled_task_cron));emplace_back(
        //     std::make_tuple("S

        event_rules.emplace_back(
            std::make_tuple("SCHEDULING_TASK_SYSTEMD_TIMERS", "PRIVILEGE ESCALATION, PERSISTENCE, EXECUTION", "T1053.006", "https://attack.mitre.org/techniques/T1053/006", "Linux", SeverityLevel::Low, true, scheduled_task_systemd_timers));

        // event_rules.emplace_back(
        //     std::make_tuple("SSH_AUTHORISED_KEYS","PERSISTENCE","T1098.004","https://attack.mitre.org/techniques/T1098/004","Linux", SeverityLevel::High, true, ssh_authorized_keys));

        event_rules.emplace_back(
            std::make_tuple("ABUSE_ELEVATION_CONTROL_SETUID", "DEFENCE EVASION, PRIVILEGE ESCALATION", "T1548.001", "https://attack.mitre.org/techniques/T1548/001", "Linux", SeverityLevel::High, true, abuse_elevation_control_setuid));

        event_rules.emplace_back(
            std::make_tuple("ABUSE_ELEVATION_CONTROL_SETUID1", "DEFENCE EVASION, PRIVILEGE ESCALATION", "T1548.001", "https://attack.mitre.org/techniques/T1548/001", "Linux", SeverityLevel::High, true, abuse_elevation_control_setuid1));

        event_rules.emplace_back(
            std::make_tuple("ABUSE_ELEVATION_CONTROL_SUDO_CACHING", "DEFENCE EVASION, PRIVILEGE ESCALATION", "T1548.003", "https://attack.mitre.org/techniques/T1548/003", "Linux", SeverityLevel::High, true, abuse_elevation_control_sudo_caching));

        event_rules.emplace_back(
            std::make_tuple("CREATE_MODIFY_SYSTEMD_SERVICE", "PERSISTENCE", "T1543.002", "https://attack.mitre.org/techniques/T1543/002", "Linux", SeverityLevel::High, true, create_modify_system_systemd_service));

        event_rules.emplace_back(
            std::make_tuple("TERMINAL_DOWNLAODS_CURL", "", "", "", "Linux", SeverityLevel::High, true, terminal_downloads_curl));

        event_rules.emplace_back(
            std::make_tuple("IMPAIR_COMMAND_HISTORY_LOGGING", "", "", "", "Linux", SeverityLevel::High, true, impair_command_history_logging));

        event_rules.emplace_back(
            std::make_tuple("TERMINAL_DOWNLAODS_PYTHON", "", "", "", "Linux", SeverityLevel::High, true, terminal_downloads_python));

        event_rules.emplace_back(
            std::make_tuple("BASH_HISTORY_COMMANDLINE", "", "", "", "Linux", SeverityLevel::High, true, bash_history_commandLine));

        event_rules.emplace_back(
            std::make_tuple("PING_PRIVILEGED_COMMANDLINE", "", "", "", "Linux", SeverityLevel::High, true, ping_privileged_commandLine));

        event_rules.emplace_back(
            std::make_tuple("MOUNT_PRIVILEGED_COMMANDLINE", "", "", "", "Linux", SeverityLevel::High, true, mount_privileged_commandLine));

        event_rules.emplace_back(
            std::make_tuple("UMOUNT_PRIVILEGED_COMMANDLINE", "", "", "", "Linux", SeverityLevel::High, true, umount_privileged_commandLine));

        event_rules.emplace_back(
            std::make_tuple("CHGRP_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, chgrp_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("PAM_TIMESTAMP_CHECK_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, pam_timestamp_check_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("UNIX_CHKPWD_CHECK_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, unix_chkpwd_check_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("PWCK_CHECK_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, pwck_check_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("USERHELPER_CHECK_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, userhelper_check_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("XORG_CHECK_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, Xorg_check_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("RLOGIN_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, rlogin_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("SUDOEDIT_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, sudoedit_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("RSH_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, rsh_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("GPASSWD_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, gpasswd_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("SUDO_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, sudo_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("STAPRUN_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, staprun_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("RCP_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, rcp_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("PASSWD_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, passwd_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("CHSH_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, chsh_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("CHFN_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, chfn_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("CHAGE_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, chage_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("SETFACL_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, setfacl_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("CHACL_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, chacl_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("CHCON_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, chcon_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("NEWGRP_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, newgrp_privileged_command));

        // event_rules.emplace_back(
        //         std::make_tuple("SLEEP_PRIVILEGED_COMMAND","","","","Linux", SeverityLevel::High, true, sleep_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("PGREP_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, pgrep_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("GREP_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, grep_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("LSPCI_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, lspci_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("UDEVADM_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, udevadm_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("FINDMNT_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, findmnt_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("NETSTAT_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, netstat_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("AWK_PRIVILEGED_COMMAND", "", "", "", "Linux", SeverityLevel::High, true, awk_privileged_command));

        // event_rules.emplace_back(
        //         std::make_tuple("SED_PRIVILEGED_COMMAND","","","","Linux", SeverityLevel::High, true, sed_privileged_command));

        event_rules.emplace_back(
            std::make_tuple("SYSTED DETECT VIRTUAL ENV", "DEFENCE EVASION, DISCOVERY", "T1497.001", "https://attack.mitre.org/techniques/T1497", "Linux", SeverityLevel::High, true, virtualization_evasion_system_checks_systemd_detect_virt));

        event_rules.emplace_back(
            std::make_tuple("SYSTED DETECT VIRTUAL ENV DMIDECODE", "DEFENCE EVASION", "T1497.001", "https://attack.mitre.org/techniques/T1497/001", "Linux", SeverityLevel::High, true, virtualization_evasion_system_checks_dmidecode));

        event_rules.emplace_back(
            std::make_tuple("SET FILE ACCESS TIMESTAMP", "DEFENCE EVASION", "T1070.006", "https://attack.mitre.org/techniques/T1070/006", "Linux", SeverityLevel::Medium, true, set_file_access_timestamp));

        // event_rules.emplace_back(
        //         std::make_tuple("SET FILE MODIFICATION TIMESTAMP","","","","Linux", SeverityLevel::High, true, set_file_modification_timestamp));

        event_rules.emplace_back(
            std::make_tuple("MODIFY FILE TIMESTAMP USING REFERENCE FILE", "DEFENCE EVASION", "T1070.006", "https://attack.mitre.org/techniques/T1070/006", "Linux", SeverityLevel::Medium, true, modify_file_timestamp_using_reference_file));

        event_rules.emplace_back(
            std::make_tuple("SUDO CACHING SUDO USAGE", "DEFENCE EVASION", "T1070.006", "https://attack.mitre.org/techniques/T1070/006", "Linux", SeverityLevel::Medium, true, sudo_and_sudo_caching_sudo_usage));

        event_rules.emplace_back(
            std::make_tuple("SUDO CACHING UNLIMITED SUDO TIMEOUT", "DEFENCE EVASION", "T1548.003", "https://attack.mitre.org/techniques/T1548/003", "Linux", SeverityLevel::Medium, true, sudo_and_sudo_caching_unlimited_sudo_timeout));
        //
        event_rules.emplace_back(
            std::make_tuple("SUDO CACHING UNLIMITED SUDO TIMEOUT", "DEFENCE EVASION, PRIVILEGE ESCALATION", "T1548.003", "https://attack.mitre.org/techniques/T1548/003", "Linux", SeverityLevel::Medium, true, sudo_and_sudo_caching_disable_tty_tickets_sudo_caching));

        event_rules.emplace_back(
            std::make_tuple("IMPAIR COMMAND HISTORY LOGGING DISABLE HISTORY COLLECTION", "DEFENCE EVASION", "T1562.003", "https://attack.mitre.org/techniques/T1562/003", "Linux", SeverityLevel::High, true, impair_cmd_history_logging_disable_history_collection));

        event_rules.emplace_back(
            std::make_tuple("IMPAIR COMMAND HISTORY LOGGING Mac HISTCONTROLL", "DEFENCE EVASION", "T1562.003", "https://attack.mitre.org/techniques/T1562/003", "Linux", SeverityLevel::High, true, impair_cmd_history_logging_mac_hist_control));

        event_rules.emplace_back(
            std::make_tuple("FILE DELETION SINGLE FILE", "", "", "", "Linux", SeverityLevel::High, true, file_deletion_single_file));

        event_rules.emplace_back(
            std::make_tuple("FILE DELETION ENTIRE FOLDER", "", "", "", "Linux", SeverityLevel::High, true, file_deletion_entire_folder));

        event_rules.emplace_back(
            std::make_tuple("OVERWRITE AND DELETE FILE WITH SHRED", "", "", "", "Linux", SeverityLevel::High, true, overwrite_and_delete_file_with_shred));

        event_rules.emplace_back(
            std::make_tuple("DELETE FILESYSTEM", "", "", "", "Linux", SeverityLevel::High, true, delete_filesystem_root));

        event_rules.emplace_back(
            std::make_tuple("MODIFYING_CRON_FILE_REFERENCE", "", "", "", "Linux", SeverityLevel::High, true, modifying_cron_file_reference));

        event_rules.emplace_back(
            std::make_tuple("MODIFYING_CRON_DAILY_FILE", "", "", "", "Linux", SeverityLevel::High, true, modifying_cron_daily_file));

        event_rules.emplace_back(
            std::make_tuple("MODIFYING_CRON_HOURLY_FILE", "", "", "", "Linux", SeverityLevel::High, true, modifying_cron_hourly_file));

        event_rules.emplace_back(
            std::make_tuple("MODIFYING_CRON_MONTHLY_FILE", "", "", "", "Linux", SeverityLevel::High, true, modifying_cron_monthly_file));

        event_rules.emplace_back(
            std::make_tuple("MODIFYING_CRON_WEEKLY_FILE", "", "", "", "Linux", SeverityLevel::High, true, modifying_cron_weekly_file));

        event_rules.emplace_back(
            std::make_tuple("MODIFYING_CRON_VAR_FILE", "", "", "", "Linux", SeverityLevel::High, true, modifying_cron_var_file));

        event_rules.emplace_back(
            std::make_tuple("SHARED_LIBRARY_INJECTION", "", "", "", "Linux", SeverityLevel::High, true, shared_library_injection));

        event_rules.emplace_back(
            std::make_tuple("SHARED_LIBRARY_INJECTION_LD_PRELOAD", "", "", "", "Linux", SeverityLevel::High, true, shared_library_injection_ld_preload));

        event_rules.emplace_back(
            std::make_tuple("LOAD_KERNEL_MODULE_INSMOD", "", "", "", "Linux", SeverityLevel::High, true, load_kernel_module_insmod));

        event_rules.emplace_back(
            std::make_tuple("PAM_CONFIG_FILE_MODIFY", "DEFENCE EVASION, PERSISTENCE, CREDENTIAL ACCESS", "T1556.003", "https://attack.mitre.org/techniques/T1556/003", "Linux", SeverityLevel::High, true, pam_config_file_modify));

        event_rules.emplace_back(
            std::make_tuple("RULE_ADD_PAM_CONFIG", "", "", "", "Linux", SeverityLevel::High, true, rule_add_pam_config));

        event_rules.emplace_back(
            std::make_tuple("RC_SCRIPT_COMMON_MIDIFY", "PRIVILEGE ESCALATION, PERSISTENCE", "T1037.004", "https://attack.mitre.org/techniques/T1037/004", "Linux", SeverityLevel::Medium, true, rc_script_common_midify));

        event_rules.emplace_back(
            std::make_tuple("RC_SCRIPT_LOCAL_MIDIFY", "PRIVILEGE ESCALATION", "T1037.004", "https://attack.mitre.org/techniques/T1037/004", "Linux", SeverityLevel::Medium, true, rc_script_local_midify));

        event_rules.emplace_back(
            std::make_tuple("SSH_AUTHORIZED_KEYS", "", "", "", "Linux", SeverityLevel::High, true, ssh_authorized_keys_midify));

        event_rules.emplace_back(
            std::make_tuple("CREATE_SYSTEMD_SERVICE_PATH", "", "", "", "Linux", SeverityLevel::High, true, create_systemd_service_path));

        event_rules.emplace_back(
            std::make_tuple("CREATE_SYSTEMD_SERVICE_FILE", "PRIVILEGE ESCALATION", "T1543.002", "https://attack.mitre.org/techniques/T1543/002", "Linux", SeverityLevel::Medium, true, create_systemd_service_file));

        event_rules.emplace_back(
            std::make_tuple("CREATE_SYSTEMD_SERVICE__TIMER_SERVICE", "", "", "", "Linux", SeverityLevel::High, true, create_systemd_service__timer_service));

        event_rules.emplace_back(
            std::make_tuple("CREATE_SYSTEMD_SERVICE__TIMER_FILE", "", "", "", "Linux", SeverityLevel::High, true, create_systemd_service__timer_file));

        // event_rules.emplace_back(
        //     std::make_tuple("LATERAL_MOVEMENT_WITH_SECURE_SHELL","","","","Linux", SeverityLevel::High, true, lateral_movement_with_secure_shell));

        event_rules.emplace_back(
            std::make_tuple("LATERAL_MOVEMENT_WITH_SSH_RSA", "", "", "", "Linux", SeverityLevel::High, true, lateral_movement_with_ssh_rsa));

        event_rules.emplace_back(
            std::make_tuple("LATERAL_TOOL_TRANSFER_FILES", "", "", "", "Linux", SeverityLevel::High, true, lateral_tool_transfer_files));

        event_rules.emplace_back(
            std::make_tuple("DYNAMIC_LINKER_HIJACKING_LD_SO_PRELOAD", "", "", "", "Linux", SeverityLevel::High, true, shared_library_injection_ld_so_preload));

        event_rules.emplace_back(
            std::make_tuple("DISABLE_SYSLOG", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001", "Linux", SeverityLevel::Low, true, disable_syslog));

        event_rules.emplace_back(
            std::make_tuple("DISABLE_CB_RESPONSE", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001", "Linux", SeverityLevel::Medium, true, disable_cb_response));

        event_rules.emplace_back(
            std::make_tuple("STOP_CROWDSTRIKE_FALCON", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001", "Linux", SeverityLevel::Medium, true, stop_crowdstrike_falcon));

        event_rules.emplace_back(
            std::make_tuple("STOP_START_UFW_FIREWALL", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Linux", SeverityLevel::High, true, stop_start_ufw_firewall));

        event_rules.emplace_back(
            std::make_tuple("STOP_START_UFW_FIREWALL_SYSTEMCTL", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Linux", SeverityLevel::High, true, stop_start_ufw_firewall_systemctl));

        event_rules.emplace_back(
            std::make_tuple("TURN_OFF_UFW_LOGGING", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Linux", SeverityLevel::High, true, turn_off_ufw_logging));

        event_rules.emplace_back(
            std::make_tuple("ADD_DELETE_UFW_RULES", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Linux", SeverityLevel::High, true, add_delete_ufw_rules));

        event_rules.emplace_back(
            std::make_tuple("EDIT_UFW_RULES_USER_DOT_RULES_FILE", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Linux", SeverityLevel::High, true, edit_ufw_user_rules_file));

        event_rules.emplace_back(
            std::make_tuple("EDIT_UFW_RULES_UFW_DOT_CONF_FILE", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Linux", SeverityLevel::High, true, edit_ufw_conf_file));

        event_rules.emplace_back(
            std::make_tuple("EDIT_UFW_RULES_SYSCTL_DOT_CONF_FILE", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Linux", SeverityLevel::High, true, edit_ufw_rules_sysctl_conf_file));

        event_rules.emplace_back(
            std::make_tuple("EDIT_UFW_FIREWALL_MAIN_CONFIG_FILE", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Linux", SeverityLevel::High, true, edit_ufw_firewall_main_config_file));

        event_rules.emplace_back(
            std::make_tuple("TAIL_UFW_FIREWALL_LOG_FILE", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Linux", SeverityLevel::High, true, tail_ufw_firewall_log_file));

        event_rules.emplace_back(
            std::make_tuple("BASE_64_DECODING_WITH_PYTHON", "DEFENCE EVASION", "T1140", "https://attack.mitre.org/techniques/T1140", "Linux", SeverityLevel::Low, true, base64_decoding_python));

        event_rules.emplace_back(
            std::make_tuple("BASE_64_DECODING_WITH_PERL", "DEFENCE EVASION", "T1140", "https://attack.mitre.org/techniques/T1140", "Linux", SeverityLevel::Low, true, base64_decoding_perl));

        event_rules.emplace_back(
            std::make_tuple("BASE_64_DECODING_WITH_SHELL_UTILITIES", "DEFENCE EVASION", "T1140", "https://attack.mitre.org/techniques/T1140", "Linux", SeverityLevel::Low, true, base64_decoding_shell_utilities));

        event_rules.emplace_back(
            std::make_tuple("HEX_DECODING_WITH_SHELL_UTILITIES", "DEFENCE EVASION", "T1140", "https://attack.mitre.org/techniques/T1140", "Linux", SeverityLevel::Low, true, hex_decoding_shell_utilities));

        // event_rules.emplace_back(
        //         std::make_tuple("COMPILE_AFTER_DELIVERY_C_COMPILE","DEFENCE EVASION","T1027.004","https://attack.mitre.org/techniques/T1027/004","Linux", SeverityLevel::High, true, compile_after_delivery_c_compile));

        event_rules.emplace_back(
            std::make_tuple("COMPILE_AFTER_DELIVERY_GO_COMPILE", "DEFENCE EVASION", "T1027.004", "https://attack.mitre.org/techniques/T1027/004", "Linux", SeverityLevel::High, true, compile_after_delivery_go_compile));

        event_rules.emplace_back(
            std::make_tuple("CREATING_GCP_SERVICE_ACCOUNT_AND_KEY", "", "", "", "Linux", SeverityLevel::High, true, creating_gcp_service_account_and_key));

        event_rules.emplace_back(
            std::make_tuple("CLEAR_LINUX_LOGS", "DEFENCE EVASION", "T1070.002", "https://attack.mitre.org/techniques/T1070/002", "Linux", SeverityLevel::High, true, clear_linux_logs_rm_rf));

        event_rules.emplace_back(
            std::make_tuple("OVERWRITE_LINUX_MAIL_SPOOL_AND_LOGS", "DEFENCE EVASION", "T1070.002", "https://attack.mitre.org/techniques/T1070/002", "Linux", SeverityLevel::High, true, overwrite_linux_mail_spool_and_logs));

        // event_rules.emplace_back(
        //         std::make_tuple("SCHEDULE_A_JOB_LINUX_AT","","","","Linux", SeverityLevel::High, true, at_schedule_a_job));

        // event_rules.emplace_back(
        //         std::make_tuple("SYSTEM_OWNER_USER_DISCOVERY","DISCOVERY","T1033","https://attack.mitre.org/techniques/T1033","Linux", SeverityLevel::High, true, system_owner_user_discovery));

        event_rules.emplace_back(
            std::make_tuple("ENUMERATE_ALL_ACCOUNTS_LOCAL", "DISCOVERY", "T1087.001", "https://attack.mitre.org/techniques/T1087/001", "Linux", SeverityLevel::High, true, enumerate_all_accounts_local));

        event_rules.emplace_back(
            std::make_tuple("VIEW_SUDOERS_ACCESS", "DISCOVERY", "T1087.001", "https://attack.mitre.org/techniques/T1087/001", "Linux", SeverityLevel::High, true, view_sudoers_file));

        event_rules.emplace_back(
            std::make_tuple("VIEW_ACCOUNTS_WITH_UID_0", "DISCOVERY", "T1087.001", "https://attack.mitre.org/techniques/T1087/001", "Linux", SeverityLevel::High, true, view_accounts_with_uid_0));

        event_rules.emplace_back(
            std::make_tuple("LIST_OPENED_FILES_BY_USER", "DISCOVERY", "T1087.001", "https://attack.mitre.org/techniques/T1087/001", "Linux", SeverityLevel::High, true, list_opened_files_by_user));

        event_rules.emplace_back(
            std::make_tuple("SHOW_IF_A_USER_ACCOUNT_HAS_EVER_LOGGED_IN_REMOTELY", "DISCOVERY", "T1087.001", "https://attack.mitre.org/techniques/T1087/001", "Linux", SeverityLevel::High, true, show_if_a_user_account_has_ever_logged_in_remotely));

        // event_rules.emplace_back(
        //         std::make_tuple("ENUMERATE_USERS_AND_GROUPS","DISCOVERY","T1087.001","https://attack.mitre.org/techniques/T1087/001","Linux", SeverityLevel::High, true, enumerate_users_and_groups));

        event_rules.emplace_back(
            std::make_tuple("SYSTEM_SERVICE_DISCOVERY_SYSTEMCTL", "DISCOVERY", "T1007", "https://attack.mitre.org/techniques/T1007", "Linux", SeverityLevel::High, true, system_service_discovery_systemctl));

        event_rules.emplace_back(
            std::make_tuple("PACKET_CAPTURE_LINUX", "DISCOVERY", "T1040", "https://attack.mitre.org/techniques/T1040", "Linux", SeverityLevel::High, true, packet_capture_linux));

        event_rules.emplace_back(
            std::make_tuple("NETWORK_SHARE_DISCOVERY", "DISCOVERY", "T1135", "https://attack.mitre.org/techniques/T1135", "Linux", SeverityLevel::High, true, network_share_discovery));

        // event_rules.emplace_back(
        //         std::make_tuple("EXAMINE_PASSWORD_COMPLEXITY_POLICY","DISCOVERY","T1201","https://attack.mitre.org/techniques/T1201","Linux", SeverityLevel::High, true, examine_password_complexity_policy));

        event_rules.emplace_back(
            std::make_tuple("LIST_OS_INFORMATION", "DISCOVERY", "T1082", "https://attack.mitre.org/techniques/T1082", "Linux", SeverityLevel::High, true, list_os_information));

        event_rules.emplace_back(
            std::make_tuple("LINUX_VM_CHECK_VIA_HARWARE", "DISCOVERY", "T1082", "https://attack.mitre.org/techniques/T1082", "Linux", SeverityLevel::High, true, linux_vm_check_via_hardware));

        event_rules.emplace_back(
            std::make_tuple("LINUX_VM_CHECK_VIA_KERNEL_MODULES", "DISCOVERY", "T1082", "https://attack.mitre.org/techniques/T1082", "Linux", SeverityLevel::High, true, linux_vm_check_via_kernel_modules));

        // event_rules.emplace_back(
        //         std::make_tuple("HOSTNAME_DISCOVERY","DISCOVERY","T1082","https://attack.mitre.org/techniques/T1082","Linux", SeverityLevel::High, true, hostname_discovery));

        // event_rules.emplace_back(
        //         std::make_tuple("ENVIRONMENT_VARIABLES_DISCOVERY","DISCOVERY","T1082","https://attack.mitre.org/techniques/T1082","Linux", SeverityLevel::High, true, environment_variables_discovery));

        event_rules.emplace_back(
            std::make_tuple("LIST_MOZILLA_FIREFOX_BOOKMARK_DATABASE_FILES", "DISCOVERY", "T1217", "https://attack.mitre.org/techniques/T1217", "Linux", SeverityLevel::High, true, list_mozilla_firefox_bookmark_database_files));

        event_rules.emplace_back(
            std::make_tuple("SYSTEM_NETWORK_CONFIGURATION_DISCOVERY", "DISCOVERY", "T1016", "https://attack.mitre.org/techniques/T1016", "Linux", SeverityLevel::High, true, system_network_configuration_discovery));

        // event_rules.emplace_back(
        //         std::make_tuple("NIX_FILE_AND_DIRECTORY_DISCOVERY","DISCOVERY","T1083","https://attack.mitre.org/techniques/T1083","Linux", SeverityLevel::High, true, nix_file_and_directory_discovery));

        event_rules.emplace_back(
            std::make_tuple("SYSTEM_NETWORK_CONNECTIONS_DISCOVERY", "DISCOVERY", "T1049", "https://attack.mitre.org/techniques/T1049", "Linux", SeverityLevel::High, true, system_network_connections_discovery));

        // event_rules.emplace_back(
        //         std::make_tuple("PROCESS_DISCOVERY","DISCOVERY","T1057","https://attack.mitre.org/techniques/T1057","Linux", SeverityLevel::Low, true, process_discovery));

        // event_rules.emplace_back(
        //         std::make_tuple("PERMISSION_GROUPS_DISCOVERY_LOCAL","DISCOVERY","T1069.001","https://attack.mitre.org/techniques/T1069/001","Linux", SeverityLevel::High, true, permission_groups_discovery_local));

        event_rules.emplace_back(
            std::make_tuple("SECURITY_SOFTWARE_DISCOVERY", "DISCOVERY", "T1518.001", "https://attack.mitre.org/techniques/T1518/001", "Linux", SeverityLevel::High, true, security_software_discovery));

        event_rules.emplace_back(
            std::make_tuple("REMOTE_SYSTEM_DISCOVERY_IPNEIGHBOUR", "DISCOVERY", "T1018", "https://attack.mitre.org/techniques/T1018", "Linux", SeverityLevel::High, true, remote_system_discovery_ipneighbour));

        event_rules.emplace_back(
            std::make_tuple("PORT_SCAN", "DISCOVERY", "T1046", "https://attack.mitre.org/techniques/T1046", "Linux", SeverityLevel::Medium, true, port_scan));

        event_rules.emplace_back(
            std::make_tuple("ENCRYPT_FILE_USING_GPG", "IMPACT", "T1486", "https://attack.mitre.org/techniques/T1486", "Linux", SeverityLevel::High, true, encrypt_file_using_gpg));

        event_rules.emplace_back(
            std::make_tuple("ENCRYPT_FILE_USING_7Z", "IMPACT", "T1486", "https://attack.mitre.org/techniques/T1486", "Linux", SeverityLevel::High, true, encrypt_file_using_7z));

        event_rules.emplace_back(
            std::make_tuple("ENCRYPT_FILE_USING_OPENSSL", "IMPACT", "T1486", "https://attack.mitre.org/techniques/T1486", "Linux", SeverityLevel::High, true, encrypt_file_using_openssl));

        event_rules.emplace_back(
            std::make_tuple("OVERWRITE_FILE_WITH_DD", "IMPACT", "T1485", "https://attack.mitre.org/techniques/T1485", "Linux", SeverityLevel::High, true, overwrite_file_with_dd));

        event_rules.emplace_back(
            std::make_tuple("SYSTEM_SHUTDOWN_REBOOT", "IMPACT", "T1529", "https://attack.mitre.org/techniques/T1529", "Linux", SeverityLevel::High, true, system_shutdown_reboot));

        // event_rules.emplace_back(
        //         std::make_tuple("EXFILTRATE_DATA_HTTPS_USING_CURL","EXFILTRATE","T1048","https://attack.mitre.org/techniques/T1048","Linux", SeverityLevel::High, true, exfiltrate_data_https_using_curl));

        event_rules.emplace_back(
            std::make_tuple("DATA_COMPRESSED_ZIP", "COLLECTION", "T1560.001", "https://attack.mitre.org/techniques/T1560/001", "Linux", SeverityLevel::Medium, true, data_compressed_zip));

        event_rules.emplace_back(
            std::make_tuple("DATA_COMPRESSED_ZIP_NIX_GZIP", "COLLECTION", "T1560.001", "https://attack.mitre.org/techniques/T1560/001", "Linux", SeverityLevel::Medium, true, data_compressed_zip_nix_gzip));

        event_rules.emplace_back(
            std::make_tuple("DATA_COMPRESSED_ZIP_NIX_TAR", "COLLECTION", "T1560.001", "https://attack.mitre.org/techniques/T1560/001", "Linux", SeverityLevel::Medium, true, data_compressed_zip_nix_tar));

        event_rules.emplace_back(
            std::make_tuple("X_WINDOWS_CAPTURE", "COLLECTION", "T1113", "https://attack.mitre.org/techniques/T1113", "Linux", SeverityLevel::High, true, x_windows_capture));

        event_rules.emplace_back(
            std::make_tuple("CAPTURE_LINUX_DESKTOP_USING_IMPORT_TOOL", "COLLECTION", "T1113", "https://attack.mitre.org/techniques/T1113", "Linux", SeverityLevel::High, true, capture_linux_desktop_using_import_tool));

        event_rules.emplace_back(
            std::make_tuple("TERMINAL_INPUT_CAPTURE_LINUX_WITH_PAM_D", "COLLECTION", "T1056.001", "https://attack.mitre.org/techniques/T1056/001", "Linux", SeverityLevel::High, true, terminal_input_capture_linux_with_pam_d));

        event_rules.emplace_back(
            std::make_tuple("LOGGING_BASH_HISTORY_TO_SYSLOG", "COLLECTION", "T1056.001", "https://attack.mitre.org/techniques/T1056/001", "Linux", SeverityLevel::High, true, logging_bash_history_to_syslog));

        event_rules.emplace_back(
            std::make_tuple("SSHD_PAM_KEYLOGGER", "COLLECTION", "T1056.001", "https://attack.mitre.org/techniques/T1056/001", "Linux", SeverityLevel::High, true, sshd_pam_keylogger));

        event_rules.emplace_back(
            std::make_tuple("STAGE_DATA_FROM_DISCOVERY_SH", "COLLECTION", "T1074.001", "https://attack.mitre.org/techniques/T1074/001", "Linux", SeverityLevel::High, true, stage_data_from_discovery_sh));

        event_rules.emplace_back(
            std::make_tuple("ADD_OR_COPY_CONTENT_TO_CLIPBOARD_WITH_XCLIP", "COLLECTION", "T1115", "https://attack.mitre.org/techniques/T1115", "Linux", SeverityLevel::High, true, add_or_copy_content_to_clipboard_with_xclip));

        event_rules.emplace_back(
            std::make_tuple("COMPRESSING_DATA_USING_GZIP_IN_PYTHON", "COLLECTION", "T1560.002", "https://attack.mitre.org/techniques/T1560/002", "Linux", SeverityLevel::High, true, compressing_data_using_gzip_in_python));

        event_rules.emplace_back(
            std::make_tuple("COMPRESSING_DATA_USING_BZ2_IN_PYTHON", "COLLECTION", "T1560.002", "https://attack.mitre.org/techniques/T1560/002", "Linux", SeverityLevel::High, true, compressing_data_using_bz2_in_python));

        event_rules.emplace_back(
            std::make_tuple("COMPRESSING_DATA_USING_ZIPFILE_IN_PYTHON", "COLLECTION", "T1560.002", "https://attack.mitre.org/techniques/T1560/002", "Linux", SeverityLevel::High, true, compressing_data_using_zipfile_in_python));
        // event_rules.emplace_back(
        //        std::make_tuple("NON_STANDARD_PORT_COMMAND_CONTROL","","","","Linux", SeverityLevel::High, true, non_standard_port_command_control));

        event_rules.emplace_back(
            std::make_tuple("HARDWARE_ADDITIONS", "INITIAL ACCESS", "T1200", "https://attack.mitre.org/techniques/T1200/", "Linux", SeverityLevel::High, true, hardware_additions));

        event_rules.emplace_back(
            std::make_tuple("INJECT_LD_PRELOAD", "INITIAL ACCESS", "T1190", "https://attack.mitre.org/tactics/TA0001/", "Linux", SeverityLevel::High, true, inject_ld_preload));

        event_rules.emplace_back(
            std::make_tuple("MANIPULATION_SSH_AUTHORIZED_KEY", "PERSISTENCE", "T1098.004", "https://attack.mitre.org/techniques/T1098/004/", "Linux", SeverityLevel::High, true, manipulation_ssh_authorized_key));

        event_rules.emplace_back(
            std::make_tuple("SHELL_CONFIG_MODIFY", "PERSISTENCE", "T1546-004", "https://attack.mitre.org/techniques/T1546/004/", "Linux", SeverityLevel::High, true, shell_config_modify));

        // event_rules.emplace_back(
        //         std::make_tuple("DYNAMIC_LINKER_HIJACKING_FILE","PERSISTENCE","T1574.006","https://attack.mitre.org/techniques/T1574/006/","Linux", SeverityLevel::High, true, dynamic_linker_hijacking_file));

        // event_rules.emplace_back(
        //         std::make_tuple("PLUGGABLE_AUTHENTICATION_MODULES_FILE","PERSISTENCE","T1556.003","https://attack.mitre.org/techniques/T1556/003/","Linux", SeverityLevel::High, true, pluggable_authentication_modules_file));

        event_rules.emplace_back(
            std::make_tuple("SCHEDULED_TASK_AT_FILE", "PERSISTENCE", "T1053.002", "https://attack.mitre.org/techniques/T1053/002/", "Linux", SeverityLevel::High, true, scheduled_task_at_file));

        event_rules.emplace_back(
            std::make_tuple("SCHEDULED_TASK_CRON_FILE", "PERSISTENCE", "T1053.003", "https://attack.mitre.org/techniques/T1053/003/", "Linux", SeverityLevel::Medium, true, scheduled_task_cron_file));

        event_rules.emplace_back(
            std::make_tuple("SCHEDULED_TASK_SYSTEMD_TIMERS_FILE", "PERSISTENCE", "T1053.006", "https://attack.mitre.org/techniques/T1053/006/", "Linux", SeverityLevel::Low, true, scheduled_task_systemd_timers_file));

        event_rules.emplace_back(
            std::make_tuple("MALICIOUS_PAM_RULES", "Credential Access", "T1556.003", "https://attack.mitre.org/techniques/T1556/003", "Linux", SeverityLevel::High, true, malicious_pam_rules));

        event_rules.emplace_back(
            std::make_tuple("INPUT_CAPTURE_KEYLOGGING", "Credential Access", "T1056.001", "https://attack.mitre.org/techniques/T1056/001", "Linux", SeverityLevel::High, true, input_capture_keylogging));

        event_rules.emplace_back(
            std::make_tuple("SSHD PAM KEYLOGGER", "Credential Access", "T1056.001", "https://attack.mitre.org/techniques/T1056/001", "Linux", SeverityLevel::High, true, sshd_pam_keylogger_keylogging));

        event_rules.emplace_back(
            std::make_tuple("DUMP_CREDENTIALS_FROM_WEB_BROWSERS", "Credential Access", "T1555.003", "https://attack.mitre.org/techniques/T1555/003", "Linux", SeverityLevel::High, true, dump_credentials_from_web_browsers));

        event_rules.emplace_back(
            std::make_tuple("DISCOVER_PRIVATE_SSH_KEYS", "Credential Access", "T1552.004", "https://attack.mitre.org/techniques/T1552/004", "Linux", SeverityLevel::High, true, discover_private_ssh_keys));

        event_rules.emplace_back(
            std::make_tuple("COPY_PRIVATE_SSH_KEYS_WITH_CP", "Credential Access", "T1552.004", "https://attack.mitre.org/techniques/T1552/004", "Linux", SeverityLevel::High, true, copy_private_ssh_keys_with_cp));

        // event_rules.emplace_back(
        //        std::make_tuple("EXTRACT_PASSWORDS_WITH_GREP","Credential Access","T1552.001","https://attack.mitre.org/techniques/T1552/001","Linux", SeverityLevel::High, true, extract_passwords_with_grep));

        // event_rules.emplace_back(
        //        std::make_tuple("EXTRACT_PASSWORDS_WITH_GREP","Credential Access","T1003.008","https://attack.mitre.org/techniques/T1003/008","Linux", SeverityLevel::High, true, access_etc_shadow_passwd));

        event_rules.emplace_back(
            std::make_tuple("EXFILTRATE_DATA_ALTERNATE_PROTOCOL", "EXFILTRATION", "T1048", "https://attack.mitre.org/techniques/T1048", "Linux", SeverityLevel::High, true, exfiltrate_data_alternate_protocol));

        event_rules.emplace_back(
            std::make_tuple("DATA_TRANSFER_SIZE_LIMITS", "EXFILTRATION", "T1030", "https://attack.mitre.org/techniques/T1030", "Linux", SeverityLevel::High, true, data_transfer_size_limits));

        event_rules.emplace_back(
            std::make_tuple("EXFIL_COMPRESSED_ARCHIVE_TO_S3_VIA_AWS_CLI", "EXFILTRATION", "T1567", "https://attack.mitre.org/techniques/T1567/002", "Linux", SeverityLevel::High, true, exfil_compressed_archive_to_s3_via_aws_cli));

        event_rules.emplace_back(
            std::make_tuple("EXFIL_COMPRESSED_ARCHIVE_TO_S3_VIA_AWS_GOLANG", "EXFILTRATION", "T1567", "https://attack.mitre.org/techniques/T1567/002", "Linux", SeverityLevel::High, true, exfil_compressed_archive_to_s3_via_aws_golang));

        event_rules.emplace_back(
            std::make_tuple("MULTI_HOP_PROXY", "Command and Control", "T1090.003", "https://attack.mitre.org/techniques/T1090/003/", "Linux", SeverityLevel::High, true, multi_hop_proxy));

        event_rules.emplace_back(
            std::make_tuple("INGRESS_TOOL_TRANSFER", "Command and Control", "T1108", "https://attack.mitre.org/techniques/T1105/", "Linux", SeverityLevel::High, true, ingress_tool_transfer));

        event_rules.emplace_back(
            std::make_tuple("ABUSE_ELEVATION_CONTROL_MECHANISM_FILE", "Privilege Escalation", "T1548.001", "https://attack.mitre.org/techniques/T1548/001/", "Linux", SeverityLevel::High, true, Abuse_elevation_control_mechanism_file));

        event_rules.emplace_back(
            std::make_tuple("ABUSE_ELEVATION_CONTROL_MECHANISM_SUDO_CACHING_FILE", "Privilege Escalation", "T1548.003", "https://attack.mitre.org/techniques/T1548/003/", "Linux", SeverityLevel::Medium, true, abuse_elevation_control_mechanism_sudo_caching_file));

        event_rules.emplace_back(
            std::make_tuple("MODIFY_SYSTEM_PROCESS_SYSTEMD_SERVICE_FILE", "Privilege Escalation", "T1543.002", "https://attack.mitre.org/techniques/T1543/002/", "Linux", SeverityLevel::Medium, true, abuse_elevation_control_mechanism_sudo_caching_file));

        event_rules.emplace_back(
            std::make_tuple("PROCESS_INJECTION_PROC_MEMORY_FILE", "Privilege Escalation", "T1055.009", "https://attack.mitre.org/techniques/T1055/009/", "Linux", SeverityLevel::High, true, process_injection_proc_memory_file));

        event_rules.emplace_back(
            std::make_tuple("BOOT_LOGON_INITIALIZATION_SCRIPTS_RC_SCRIPTS_FILE", "Privilege Escalation", "T1037.004", "https://attack.mitre.org/techniques/T1037/004/", "Linux", SeverityLevel::Medium, true, boot_logon_initialization_scripts_rc_scripts_file));

        event_rules.emplace_back(
            std::make_tuple("CHAOS_MALWARE_INFECTION", "User Execution", "T1204.002", "https://attack.mitre.org/techniques/T1204/002/", "Linux", SeverityLevel::High, true, chaos_malware_infection));

        event_rules.emplace_back(
            std::make_tuple("Attempt to stop a service", "Impact", "T1489", "https://attack.mitre.org/techniques/T1489/", "Linux", SeverityLevel::High, true, service_stop_one));

        event_rules.emplace_back(
            std::make_tuple("Attempt to stop a service", "Impact", "T1489", "https://attack.mitre.org/techniques/T1489/", "Linux", SeverityLevel::High, true, service_stop_two));

        event_rules.emplace_back(
            std::make_tuple("Attempt to stop a service", "Impact", "T1489", "https://attack.mitre.org/techniques/T1489/", "Linux", SeverityLevel::High, true, service_stop_three));

        event_rules.emplace_back(
            std::make_tuple("Defacement: Internal Defacement", "Impact", "T1489", "https://attack.mitre.org/techniques/T1489/", "Linux", SeverityLevel::High, true, internal_defacement_one));

        event_rules.emplace_back(
            std::make_tuple("Defacement: Internal Defacement", "Impact", "T1491.001", "https://attack.mitre.org/techniques/T1489/", "Linux", SeverityLevel::High, true, internal_defacement_two));

        event_rules.emplace_back(
            std::make_tuple("Data Encrypted for Impact", "Impact", "T1486", "https://attack.mitre.org/techniques/T1486/", "Linux", SeverityLevel::High, true, data_encrypted_impact));

        event_rules.emplace_back(
            std::make_tuple("SCHEDULED_TASK_JOB", "Execution", "T1053.005", "https://attack.mitre.org/techniques/T1053/005/", "Windows", SeverityLevel::High, true, scheduled_task_job));

        event_rules.emplace_back(
            std::make_tuple("NATIVE_API", "Execution", "T1106", "https://attack.mitre.org/techniques/T1106/", "Windows", SeverityLevel::High, true, native_api));

        event_rules.emplace_back(
            std::make_tuple("COMMAND_AND_SCRIPTING_INTERPRETER", "Execution", "T1059", "https://attack.mitre.org/techniques/T1059/", "Windows", SeverityLevel::High, true, command_and_scripting_interpreter));

        event_rules.emplace_back(
            std::make_tuple("SERVICE_EXECUTION", "Execution", "T1569.002", "https://attack.mitre.org/techniques/T1569/002/", "Windows", SeverityLevel::High, true, service_execution));

        event_rules.emplace_back(
            std::make_tuple("COMMAND_AND_SCRIPTING_INTERPRETER: JAVASCRIPT", "Execution", "T1059.007", "https://attack.mitre.org/techniques/T1059/007/", "Windows", SeverityLevel::High, true, command_scripting_interpreter_javascript));

        event_rules.emplace_back(
            std::make_tuple("COMMAND_AND_SCRIPTING_INTERPRETER: POWERSHELL", "Execution", "T1059.001", "https://attack.mitre.org/techniques/T1059/001/", "Windows", SeverityLevel::High, true, command_scripting_interpreter_powershell));

        event_rules.emplace_back(
            std::make_tuple("COMMAND_AND_SCRIPTING_INTERPRETER: WINDOWS COMMAND SHELL", "Execution", "T1059.003", "https://attack.mitre.org/techniques/T1059/003/", "Windows", SeverityLevel::High, true, command_scripting_interpreter_win_command_shell));

        event_rules.emplace_back(
            std::make_tuple("COMMAND_AND_SCRIPTING_INTERPRETER: VISUAL BASIC", "Execution", "T1059.005", "https://attack.mitre.org/techniques/T1059/005/", "Windows", SeverityLevel::High, true, command_scripting_interpreter_visual_basic));

        event_rules.emplace_back(
            std::make_tuple("User Execution: Malicious File", "Execution", "T1204.002", "https://attack.mitre.org/techniques/T1204/002", "Windows", SeverityLevel::High, true, malicious_file_user_execution));

        event_rules.emplace_back(
            std::make_tuple("BITS_jobs", "Defence Evasion", "T1197", "https://attack.mitre.org/techniques/T1197/", "Windows", SeverityLevel::High, true, BITS_jobs));

        event_rules.emplace_back(
            std::make_tuple("Valid accounts: Default accounts", "Defence Evasion", "T1078.001", "https://attack.mitre.org/techniques/T1078/001/", "Windows", SeverityLevel::High, true, escalate_guest));

        // event_rules.emplace_back(
        // std::make_tuple("Modify Registry", "Defence Evasion", "T1112", "https://attack.mitre.org/techniques/T1112/","Windows", SeverityLevel::High, true, registry_modification));

        event_rules.emplace_back(
            std::make_tuple("Indirect Command Execution", "Defence Evasion", "T1202", "https://attack.mitre.org/techniques/T1202/", "Windows", SeverityLevel::High, true, indirect_command_execution));

        event_rules.emplace_back(
            std::make_tuple("Use Alternate Authentication Material", "Defence Evasion", "T1550.003", "https://attack.mitre.org/techniques/T1550/003/", "Windows", SeverityLevel::High, true, alternate_authentication));

        event_rules.emplace_back(
            std::make_tuple("Indicator Removal", "Defence Evasion", "T1070", "https://attack.mitre.org/techniques/T1070/", "Windows", SeverityLevel::High, true, indicator_removal));

        event_rules.emplace_back(
            std::make_tuple("Impair Defenses: Indicator Blocking", "Defence Evasion", "T1562.006", "https://attack.mitre.org/techniques/T1562/006/", "Windows", SeverityLevel::High, true, disable_powershell_etw));

        event_rules.emplace_back(
            std::make_tuple("Create process with token", "Defence Evasion", "T1134.002", "https://attack.mitre.org/techniques/T1134/002/", "Windows", SeverityLevel::High, true, create_process_with_token));

        event_rules.emplace_back(
            std::make_tuple("Process Hollowing", "Defence Evasion", "T1055.012", "https://attack.mitre.org/techniques/T1055/012/", "Windows", SeverityLevel::High, true, process_hollowing));

        event_rules.emplace_back(
            std::make_tuple("File and Directory Permissions Modification: Windows File and Directory Permissions Modification", "", "Defence Evasion", "T1222.001", "https://attack.mitre.org/techniques/T1222/001/", SeverityLevel::High, true, grant_access_to_C));

        event_rules.emplace_back(
            std::make_tuple("Hide Artifacts: Hidden Files and Directories", "Defence Evasion", "T1564.001", "https://attack.mitre.org/techniques/T1564/001/", "Windows", SeverityLevel::High, true, hide_artifacts));

        event_rules.emplace_back(
            std::make_tuple("Masquerading", "Defence Evasion", "T1036", "https://attack.mitre.org/techniques/T1036/", "Windows", SeverityLevel::High, true, masquerading));

        event_rules.emplace_back(
            std::make_tuple("Trusted Developer Utilities Proxy Execution", "Defence Evasion", "T1127", "https://attack.mitre.org/techniques/T1127/", "Windows", SeverityLevel::High, true, proxy_execution));

        event_rules.emplace_back(
            std::make_tuple("Abuse Elevation Control Mechanism: Bypass User Account Control", "Defence Evasion", "T1548.002", "https://attack.mitre.org/techniques/T1548/002", "Windows", SeverityLevel::High, true, bypass_user_account_control));

        event_rules.emplace_back(
            std::make_tuple("XSL Script Processing", "Defence Evasion", "T1220", "https://attack.mitre.org/techniques/T1220", "Windows", SeverityLevel::High, true, xsl_script_processing));

        event_rules.emplace_back(
            std::make_tuple("System Binary Proxy Execution: Mshta", "Defence Evasion", "T1218.005", "https://attack.mitre.org/techniques/T1218/005", "Windows", SeverityLevel::High, true, mshta));

        event_rules.emplace_back(
            std::make_tuple("System Binary Proxy Execution: Regsvr32", "Defence Evasion", "T1218.010", "https://attack.mitre.org/techniques/T1218/010", "Windows", SeverityLevel::High, true, system_binary_proxy_execution_regsvr32));

        event_rules.emplace_back(
            std::make_tuple("System Binary Proxy Execution: Msiexec", "Defence Evasion", "T1218.007", "https://attack.mitre.org/techniques/T1218/007", "Windows", SeverityLevel::High, true, system_binary_proxy_execution_msiexec));

        event_rules.emplace_back(
            std::make_tuple("Trusted Developer Utilities Proxy Execution: MSBuild", "Defence Evasion", "T1127.001", "https://attack.mitre.org/techniques/T1127/001", "Windows", SeverityLevel::High, true, proxy_execution_msbuild));

        event_rules.emplace_back(
            std::make_tuple("System Binary Proxy Execution", "Defence Evasion", "T1218", "https://attack.mitre.org/techniques/T1218/", "Windows", SeverityLevel::High, true, system_binary_proxy_execution));

        event_rules.emplace_back(
            std::make_tuple("System Binary Proxy Execution: Rundll32", "Defence Evasion", "T1218.011", "https://attack.mitre.org/techniques/T1218/011/", "Windows", SeverityLevel::High, true, system_binary_proxy_execution_rundll32));

        event_rules.emplace_back(
            std::make_tuple("Hijack Execution Flow: DLL Side-Loading", "DEFENCE EVASION", "T1574.002", "https://attack.mitre.org/techniques/T1574/002/", "Windows", SeverityLevel::High, true, dll_side_loading));

        event_rules.emplace_back(
            std::make_tuple("Hide Artifacts: NTFS File Attributes", "DEFENCE EVASION", "T1564.004", "https://attack.mitre.org/techniques/T1564/004/", "Windows", SeverityLevel::High, true, ntfs_file_attributes));

        event_rules.emplace_back(
            std::make_tuple("Indicator Removal: Clear Command History", "DEFENCE EVASION", "T1070.003", "https://attack.mitre.org/techniques/T1070/003/", "Windows", SeverityLevel::High, true, clear_command_history));

        event_rules.emplace_back(
            std::make_tuple("Obfuscated Files or Information", "DEFENCE EVASION", "T1027", "https://attack.mitre.org/techniques/T1027/", "Windows", SeverityLevel::High, true, obfuscated_files_or_information));

        event_rules.emplace_back(
            std::make_tuple("Process Injection", "DEFENCE EVASION", "T1055", "https://attack.mitre.org/techniques/T1055/", "Windows", SeverityLevel::High, true, process_injection));

        event_rules.emplace_back(
            std::make_tuple("Indicator Removal: File Deletion", "DEFENCE EVASION", "T1070.004", "https://attack.mitre.org/techniques/T1070/004", "Windows", SeverityLevel::High, true, indicator_removal_file_deletion));

        event_rules.emplace_back(
            std::make_tuple("Hide Artifacts: Hidden Window", "DEFENCE EVASION", "T1564.003", "https://attack.mitre.org/techniques/T1564/003", "Windows", SeverityLevel::High, true, hidden_window_hide_artifacts));

        event_rules.emplace_back(
            std::make_tuple("Impair Defenses: Disable or Modify Tools", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001", "Windows", SeverityLevel::High, true, impair_defenses_disable_modify_tools));

        event_rules.emplace_back(
            std::make_tuple("Subvert Trust Controls: Install Root Certificate", "DEFENCE EVASION", "T1553.004", "https://attack.mitre.org/techniques/T1553/004", "Windows", SeverityLevel::High, true, install_root_certificate));

        event_rules.emplace_back(
            std::make_tuple("System Binary Proxy Execution: InstallUtil", "DEFENCE EVASION", "T1218.004", "https://attack.mitre.org/techniques/T1218/004", "Windows", SeverityLevel::High, true, system_binary_proxy_execution_installutil));

        event_rules.emplace_back(
            std::make_tuple("Deobfuscate/Decode Files or Information", "DEFENCE EVASION", "T1140", "https://attack.mitre.org/techniques/T1140", "Windows", SeverityLevel::High, true, decode_files_or_information));

        event_rules.emplace_back(
            std::make_tuple("Modify Registry", "DEFENCE EVASION", "T1112", "https://attack.mitre.org/techniques/T1112/", "Windows", SeverityLevel::High, true, modify_registry_netwire));

        event_rules.emplace_back(
            std::make_tuple("Modify Registry", "DEFENCE EVASION", "T1112", "https://attack.mitre.org/techniques/T1112/", "Windows", SeverityLevel::High, true, modify_registry_ursnif));

        event_rules.emplace_back(
            std::make_tuple("Modify Registry", "DEFENCE EVASION", "T1112", "https://attack.mitre.org/techniques/T1112/", "Windows", SeverityLevel::High, true, modify_registry_terminal_server_client));

        event_rules.emplace_back(
            std::make_tuple("Modify Registry", "DEFENCE EVASION", "T1112", "https://attack.mitre.org/techniques/T1112/", "Windows", SeverityLevel::High, true, modify_registry_blackbyte));

        event_rules.emplace_back(
            std::make_tuple("Modify Registry", "DEFENCE EVASION", "T1112", "https://attack.mitre.org/techniques/T1112/", "Windows", SeverityLevel::High, true, modify_registry_load_service_safemode));

        event_rules.emplace_back(
            std::make_tuple("Modify Registry", "DEFENCE EVASION", "T1112", "https://attack.mitre.org/techniques/T1112/", "Windows", SeverityLevel::High, true, modify_registry_disable_win_registry_tool));

        event_rules.emplace_back(
            std::make_tuple("Modify Registry", "DEFENCE EVASION", "T1112", "https://attack.mitre.org/techniques/T1112/", "Windows", SeverityLevel::High, true, modify_registry_disable_win_security_notifications));

        event_rules.emplace_back(
            std::make_tuple("Modify Registry", "DEFENCE EVASION", "T1112", "https://attack.mitre.org/techniques/T1112/", "Windows", SeverityLevel::High, true, modify_registry_win_group_policy_feature));

        event_rules.emplace_back(
            std::make_tuple("Impair Defenses: Disable or Modify Tools", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001", "Windows", SeverityLevel::High, true, impair_defenses_disable_modify_tools_AMSI_Byspass));

        event_rules.emplace_back(
            std::make_tuple("Impair Defenses: Disable or Modify Tools", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001", "Windows", SeverityLevel::High, true, impair_defenses_disable_modify_tools_office_security));

        event_rules.emplace_back(
            std::make_tuple("Impair Defenses: Disable or Modify System Firewall", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004", "Windows", SeverityLevel::High, true, impair_defenses_disable_defender_firewall));

        event_rules.emplace_back(
            std::make_tuple("Hijack Execution Flow: COR_PROFILER", "DEFENCE EVASION", "T1574.012", "https://attack.mitre.org/techniques/T1574/012", "Windows", SeverityLevel::High, true, user_scope_cor_profile));

        event_rules.emplace_back(
            std::make_tuple("Impair Defenses: Disable or Modify Tools", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001", "Windows", SeverityLevel::High, true, impair_defenses_tamper_win_defender));

        event_rules.emplace_back(
            std::make_tuple("Hide Artifacts: Hidden Files and Directories", "DEFENCE EVASION", "T1564.001", "https://attack.mitre.org/techniques/T1564/001", "Windows", SeverityLevel::High, true, hide_artifacts_through_registry));

        event_rules.emplace_back(
            std::make_tuple("Subvert Trust Controls: Install Root Certificate", "DEFENCE EVASION", "T1553.004", "https://attack.mitre.org/techniques/T1553/004", "Windows", SeverityLevel::High, true, install_root_certificate_win_certutil));

        event_rules.emplace_back(
            std::make_tuple("Indicator Removal: File Deletion", "DEFENCE EVASION", "T1070.004", "https://attack.mitre.org/techniques/T1070/004", "Windows", SeverityLevel::High, true, indicator_removal_del_single_file));

        event_rules.emplace_back(
            std::make_tuple("Scheduled Task/Job: Scheduled Task", "Privilege Escalation", "T1053.005", "https://attack.mitre.org/techniques/T1053/005/", "Windows", SeverityLevel::Medium, true, scheduled_task));

        event_rules.emplace_back(
            std::make_tuple("Create or Modify System Process: Windows Service", "Privilege Escalation", "T1543.003", "https://attack.mitre.org/techniques/T1543/003/", "Windows", SeverityLevel::High, true, create_or_modify_windows_process));

        event_rules.emplace_back(
            std::make_tuple("Event Triggered Execution: Application Shimming", "Privilege Escalation", "T1546.011", "https://attack.mitre.org/techniques/T1546/011/", "Windows", SeverityLevel::High, true, application_shimming));

        event_rules.emplace_back(
            std::make_tuple("Event Triggered Execution: Netsh Helper DLL", "Privilege Escalation", "T1546.007", "https://attack.mitre.org/techniques/T1546/007/", "Windows", SeverityLevel::High, true, netsh_helper_dll));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Autostart Execution: Registry Run Keys / Startup Folder", "Privilege Escalation", "T1547.001", "https://attack.mitre.org/techniques/T1547/001/", "Windows", SeverityLevel::High, true, registry_run_keys));

        event_rules.emplace_back(
            std::make_tuple("Access Token Manipulation: SID-History Injection", "Privilege Escalation", "T1134.005", "https://attack.mitre.org/techniques/T1134/005/", "Windows", SeverityLevel::High, true, sid_history_injection));

        event_rules.emplace_back(
            std::make_tuple("Hijack Execution Flow: DLL Search Order Hijacking ", "Privilege Escalation", "T1574.001", "https://attack.mitre.org/techniques/T1574/001/", "Windows", SeverityLevel::High, true, dll_search_order_hijacking));

        event_rules.emplace_back(
            std::make_tuple("Process Injection: Thread execution Hijacking", "Privilege Escalation", "T1055.003", "https://attack.mitre.org/techniques/T1055/003/", "Windows", SeverityLevel::High, true, thread_execution_hijacking));

        event_rules.emplace_back(
            std::make_tuple("Access Token Manipulation: Parent PID Spoofing", "Privilege Escalation", "T1134.004", "https://attack.mitre.org/techniques/T1134/004/", "Windows", SeverityLevel::High, true, pid_parent_spoofing));

        event_rules.emplace_back(
            std::make_tuple("System Binary Proxy Execution: CMSTP", "Privilege Escalation", "T1218.003", "https://attack.mitre.org/techniques/T1218/003/", "Windows", SeverityLevel::High, true, cmstp));

        event_rules.emplace_back(
            std::make_tuple("Event Triggered Execution: Accessibility Features", "Privilege Escalation", "T1546.008", "https://attack.mitre.org/techniques/T1546/008/", "Windows", SeverityLevel::High, true, event_triggered_execution_accessibility_features));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Autostart Execution: Security Support Provider", "Privilege Escalation", "T1547.005", "https://attack.mitre.org/techniques/T1547/005/", "Windows", SeverityLevel::High, true, security_support_provider));

        event_rules.emplace_back(
            std::make_tuple("Domain Policy Modification: Group Policy Modification", "Privilege Escalation", "T1484.001", "https://attack.mitre.org/techniques/T1484/001/", "Windows", SeverityLevel::High, true, group_policy_modification));

        event_rules.emplace_back(
            std::make_tuple("Event Triggered Execution: Image File Execution Options Injection", "Privilege Escalation", "T1546.012", "https://attack.mitre.org/techniques/T1546/012", "Windows", SeverityLevel::High, true, image_file_execution_options_injection));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Autostart Execution: Winlogon Helper DLL", "Privilege Escalation", "T1547.004", "https://attack.mitre.org/techniques/T1547/004/", "Windows", SeverityLevel::High, true, winlogon_helper_dll));

        event_rules.emplace_back(
            std::make_tuple("Event Triggered Execution: Component Object Model Hijacking", "Privilege Escalation", "T1546.015", "https://attack.mitre.org/techniques/T1546/015", "Windows", SeverityLevel::High, true, com_hijacking_inprocserver32));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Autostart Execution: Winlogon Helper DLL", "Privilege Escalation", "T1547.004", "https://attack.mitre.org/techniques/T1547/004/", "Windows", SeverityLevel::High, true, winlogon_notify_key_logon));

        event_rules.emplace_back(
            std::make_tuple("Screen Capture", "Collection", "T1113", "https://attack.mitre.org/techniques/T1113", "Windows", SeverityLevel::High, true, screen_capture));

        event_rules.emplace_back(
            std::make_tuple("Data Staged: Local Data Staging", "Collection", "T1074.001", "https://attack.mitre.org/techniques/T1074/001/", "Windows", SeverityLevel::High, true, data_staged));

        event_rules.emplace_back(
            std::make_tuple("Automated Collection", "Collection", "T1119", "https://attack.mitre.org/techniques/T1119", "Windows", SeverityLevel::High, true, automated_collection));

        event_rules.emplace_back(
            std::make_tuple("Clipboard Data", "Collection", "T1115", "https://attack.mitre.org/techniques/T1115", "Windows", SeverityLevel::High, true, clipboard_data));

        event_rules.emplace_back(
            std::make_tuple("Archive Collected Data", "Collection", "T1560", "https://attack.mitre.org/techniques/T1560", "Windows", SeverityLevel::High, true, archive_collected_data));

        event_rules.emplace_back(
            std::make_tuple("Video Capture", "Collection", "T1125", "https://attack.mitre.org/techniques/T1125", "Windows", SeverityLevel::High, true, video_capture));

        event_rules.emplace_back(
            std::make_tuple("Data from Network Shared Drive", "Collection", "T1039", "https://attack.mitre.org/techniques/T1039", "Windows", SeverityLevel::High, true, network_shared_drive_data));

        event_rules.emplace_back(
            std::make_tuple("Audio Capture", "Collection", "T1123", "https://attack.mitre.org/techniques/T1123", "Windows", SeverityLevel::High, true, audio_capture));

        event_rules.emplace_back(
            std::make_tuple("Input Capture: GUI Input Capture", "Collection", "T1056.002", "https://attack.mitre.org/techniques/T1056/002", "Windows", SeverityLevel::High, true, gui_input_capture));

        event_rules.emplace_back(
            std::make_tuple("Group Policy Discovery", "Discovery", "T1615", "https://attack.mitre.org/techniques/T1615", "Windows", SeverityLevel::High, true, group_policy_discovery));

        event_rules.emplace_back(
            std::make_tuple("Browser Information Discovery", "Discovery", "T1217", "https://attack.mitre.org/techniques/T1217", "Windows", SeverityLevel::High, true, browser_information_discovery));

        event_rules.emplace_back(
            std::make_tuple("Account Discovery: Domain Account", "Discovery", "T1087.002", "https://attack.mitre.org/techniques/T1087/002", "Windows", SeverityLevel::High, true, account_discovery_domain_account));

        event_rules.emplace_back(
            std::make_tuple("System Information Discovery", "Discovery", "T1082", "https://attack.mitre.org/techniques/T1082", "Windows", SeverityLevel::High, true, system_information_discovery));

        event_rules.emplace_back(
            std::make_tuple("Domain Trust Discovery", "Discovery", "T1482", "https://attack.mitre.org/techniques/T1482", "Windows", SeverityLevel::High, true, domain_trust_discovery));

        event_rules.emplace_back(
            std::make_tuple("File and Directory Discovery", "Discovery", "T1083", "https://attack.mitre.org/techniques/T1083", "Windows", SeverityLevel::High, true, file_and_directory_discovery));

        event_rules.emplace_back(
            std::make_tuple("Software Discovery: Security Software Discovery", "Discovery", "T1518.001", "https://attack.mitre.org/techniques/T1518/001", "Windows", SeverityLevel::High, true, win_security_software_discovery));

        event_rules.emplace_back(
            std::make_tuple("System Location Discovery: System Language Discovery", "Discovery", "T1614.001", "https://attack.mitre.org/techniques/T1614/001", "Windows", SeverityLevel::High, true, discover_system_language_chcp));

        event_rules.emplace_back(
            std::make_tuple("Data Encoding: Standard Encoding", "Command and control", "T1132.001", "https://attack.mitre.org/techniques/T1132/001/", "Windows", SeverityLevel::High, true, data_encoding_standard_encoding));

        event_rules.emplace_back(
            std::make_tuple("Application Layer Protocol: DNS", "Command and control", "T1071.004", "https://attack.mitre.org/techniques/T1071/004/", "Windows", SeverityLevel::High, true, dns_large_query_volume));

        event_rules.emplace_back(
            std::make_tuple("Remote Access Software", "Command and control", "T1219", "https://attack.mitre.org/techniques/T1219", "Windows", SeverityLevel::High, true, remote_access_software));

        event_rules.emplace_back(
            std::make_tuple("Protocol Tunneling", "Command and control", "T1572", "https://attack.mitre.org/techniques/T1572/", "Windows", SeverityLevel::High, true, code_executed_via_excel));

        event_rules.emplace_back(
            std::make_tuple("Ingress Tool Transfer", "Command and control", "T1105", "https://attack.mitre.org/techniques/T1105/", "Windows", SeverityLevel::High, true, win_ingress_tool_transfer));

        event_rules.emplace_back(
            std::make_tuple("Non Standard Port", "Command and control", "T1571", "https://attack.mitre.org/techniques/T1571/", "Windows", SeverityLevel::High, true, non_standard_port));

        event_rules.emplace_back(
            std::make_tuple("Ingress Tool Transfer", "Command and control", "T1105", "https://attack.mitre.org/techniques/T1105/", "Windows", SeverityLevel::High, true, win_ingress_tool_transfer_certutil));

        event_rules.emplace_back(
            std::make_tuple("Ingress Tool Transfer", "Command and control", "T1105", "https://attack.mitre.org/techniques/T1105/", "Windows", SeverityLevel::High, true, win_ingress_tool_transfer_curl_download));

        event_rules.emplace_back(
            std::make_tuple("Proxy: Internal Proxy", "Command and control", "T1090.001", "https://attack.mitre.org/techniques/T1090/001", "Windows", SeverityLevel::High, true, internal_proxy_portproxy_regkey));

        event_rules.emplace_back(
            std::make_tuple("Ingress Tool Transfer", "Command and control", "T1105", "https://attack.mitre.org/techniques/T1105/", "Windows", SeverityLevel::High, true, win_ingress_tool_transfer_certutil));

        event_rules.emplace_back(
            std::make_tuple("Ingress Tool Transfer", "Command and control", "T1105", "https://attack.mitre.org/techniques/T1105/", "Windows", SeverityLevel::High, true, win_ingress_tool_transfer_curl_download));

        event_rules.emplace_back(
            std::make_tuple("Proxy: Internal Proxy", "Command and control", "T1090.001", "https://attack.mitre.org/techniques/T1090/001", "Windows", SeverityLevel::High, true, internal_proxy_portproxy_regkey));

        event_rules.emplace_back(
            std::make_tuple("Brute Force: Password Guessing", "Credential Access", "T1110.001", "https://attack.mitre.org/techniques/T1110/001/", "Windows", SeverityLevel::High, true, password_guessing));

        event_rules.emplace_back(
            std::make_tuple("Steal Web Session Cookie", "Credential Access", "T1539", "https://attack.mitre.org/techniques/T1539/", "Windows", SeverityLevel::High, true, steal_web_session_cookie));

        event_rules.emplace_back(
            std::make_tuple("Registry dump of SAM, creds, and secrets", "Credential Access", "T1003.002", "https://attack.mitre.org/techniques/T1003/002", "Windows", SeverityLevel::High, true, registry_dump_of_sam_creds_secrets));

        event_rules.emplace_back(
            std::make_tuple("Packet Capture Windows Command Prompt", "Credential Access", "T1040", "https://attack.mitre.org/techniques/T1040/", "Windows", SeverityLevel::High, true, packet_capture_windows_command_prompt));

        event_rules.emplace_back(
            std::make_tuple("Unsecured Credentials: Credentials in Registry", "Credential Access", "1552.002", "https://attack.mitre.org/techniques/T1552/002/", "Windows", SeverityLevel::High, true, enumeration_for_credentials_in_registry));

        event_rules.emplace_back(
            std::make_tuple("Modify Authentication Process: Password Filter DLL", "Credential Access", "1556.002", "https://attack.mitre.org/techniques/T1556/002/", "Windows", SeverityLevel::High, true, install_and_register_paassword_filter_dll));

        event_rules.emplace_back(
            std::make_tuple("Unsecured Credentials: Group Policy Preferences", "Credential Access", "T1552.006", "https://attack.mitre.org/techniques/T1552/006/", "Windows", SeverityLevel::High, true, unsecured_credentials_gpp_passwords));

        event_rules.emplace_back(
            std::make_tuple("OS Credential Dumping: LSASS Memory", "Credential Access", "T1003.001", "https://attack.mitre.org/techniques/T1003/001/", "Windows", SeverityLevel::High, true, lsass_memory_using_comsvcs_dll));

        event_rules.emplace_back(
            std::make_tuple("Brute Force: Password Spraying", "Credential Access", "T1110.003", "https://attack.mitre.org/techniques/T1110/003/", "Windows", SeverityLevel::High, true, password_spraying_kurbute));

        event_rules.emplace_back(
            std::make_tuple("Input Capture: Credential API Hooking", "Credential Access", "T1056.004", "https://attack.mitre.org/techniques/T1056/004/", "", SeverityLevel::High, true, input_capture_credential_api_hooking));

        event_rules.emplace_back(
            std::make_tuple("OS Credential Dumping: DCSync", "Credential Access", "T1003.006", "https://attack.mitre.org/techniques/T1003/006/", "Windows", SeverityLevel::High, true, os_credential_dumping_dcsync));

        event_rules.emplace_back(
            std::make_tuple("Credentials from Password Stores: Windows Credential Manager", "Credential Access", "T1555.004", "https://attack.mitre.org/techniques/T1555/004/", "Windows", SeverityLevel::High, true, password_stores_windows_credentail_manager));

        event_rules.emplace_back(
            std::make_tuple("OS Credential Dumping", "Credential Access", "T1003", "https://attack.mitre.org/techniques/T1003/", "Windows", SeverityLevel::High, true, os_credential_dumping));

        event_rules.emplace_back(
            std::make_tuple("Forced Authentication", "Credential Access", "T1187", "https://attack.mitre.org/techniques/T1187/", "Windows", SeverityLevel::High, true, forced_authentication));

        event_rules.emplace_back(
            std::make_tuple("Steal or Forge Kerberos Tickets: Kerberoasting", "Credential Access", "T1558.003", "https://attack.mitre.org/techniques/T1558/003", "Windows", SeverityLevel::High, true, kerberoasting_steal_or_forge_Kerberos_tickets));

        event_rules.emplace_back(
            std::make_tuple("OS Credential Dumping: Security Account Manager", "Credential Access", "T1003", "https://attack.mitre.org/techniques/T1003", "Windows", SeverityLevel::High, true, os_credential_dumping_esentutl));

        event_rules.emplace_back(
            std::make_tuple("Event Triggered Execution: PowerShell Profile", "Persistence", "T1546.013", "https://attack.mitre.org/techniques/T1546/013/", "Windows", SeverityLevel::High, true, append_malicious_start_process_cmdlet));

        event_rules.emplace_back(
            std::make_tuple("External Remote Services", "Persistence", "T1133", "https://attack.mitre.org/techniques/T1133/", "Windows", SeverityLevel::High, true, running_chrome_vpn_extensions));

        event_rules.emplace_back(
            std::make_tuple("Active Setup", "Persistence", "T1547.014", "https://attack.mitre.org/techniques/T1547/014/", "Windows", SeverityLevel::High, true, active_setup));

        event_rules.emplace_back(
            std::make_tuple("Time Providers", "Persistence", "T1547.03", "https://attack.mitre.org/techniques/T1547/003/", "Windows", SeverityLevel::High, true, time_providers_new));

        event_rules.emplace_back(
            std::make_tuple("Office Application Startup: Add-ins", "Persistence", "T1137.006", "https://attack.mitre.org/techniques/T1137/006/", "Windows", SeverityLevel::High, true, persistent_code_evecution_via_excel_vba_addin));

        event_rules.emplace_back(
            std::make_tuple("Office Application Startup: Add-ins", "Persistence", "T1137.006", "https://attack.mitre.org/techniques/T1137/006/", "Windows", SeverityLevel::High, true, persistent_code_execution_via_word_addin));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Autostart Execution: Port Monitors", "Persistence", "T1547.010", "https://attack.mitre.org/techniques/T1547/010/", "Windows", SeverityLevel::High, true, port_monitors));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Autostart Execution: Shortcut Modification", "Persistence", "T1547.009", "https://attack.mitre.org/techniques/T1547/009/", "Windows", SeverityLevel::High, true, shortcut_modification));

        event_rules.emplace_back(
            std::make_tuple("Hijack Execution Flow: Path Interception by Search Order Hijacking", "Persistence", "T1574.008", "https://attack.mitre.org/techniques/T1574/008/", "Windows", SeverityLevel::High, true, search_order_hijacking));

        event_rules.emplace_back(
            std::make_tuple("Server Software Component: Web Shell", "Persistence", "T1505.003", "https://attack.mitre.org/techniques/T1505/003/", "Windows", SeverityLevel::High, true, server_software_component_web_shell));

        event_rules.emplace_back(
            std::make_tuple("Event Triggered Execution: Component Object Model Hijacking", "Persistence", "T1546.015", "https://attack.mitre.org/techniques/T1546/015/", "Windows", SeverityLevel::High, true, component_object_model_hijacking));

        event_rules.emplace_back(
            std::make_tuple("Event Triggered Execution: Change Default File Association", "Persistence", "T1546.001", "https://attack.mitre.org/techniques/T1546/001/", "Windows", SeverityLevel::High, true, change_default_file_association));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Initialization Scripts: Logon Script (Windows)", "Persistence", "T1037.001", "https://attack.mitre.org/techniques/T1037/001", "Windows", SeverityLevel::High, true, win_logon_script));

        event_rules.emplace_back(
            std::make_tuple("Event Triggered Execution: Screensaver", "Persistence", "T1546.002", "https://attack.mitre.org/techniques/T1546/002/", "Windows", SeverityLevel::High, true, event_triggered_exevution_screensaver));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Autostart Execution: Registry Run Keys / Startup Folder", "Persistence", "T1547.001", "https://attack.mitre.org/techniques/T1547/001/", "Windows", SeverityLevel::High, true, registry_run_keys_persistence_via_recycle_bin));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Autostart Execution: Security Support Provider", "Persistence", "T1547.005", "https://attack.mitre.org/techniques/T1547/005/", "Windows", SeverityLevel::High, true, security_support_provider_ssp));

        event_rules.emplace_back(
            std::make_tuple("Abuse Elevation Control Mechanism: Bypass User Account Control", "Persistence", "T1548.002", "https://attack.mitre.org/techniques/T1548/002/", "Windows", SeverityLevel::High, true, bypass_uac_sdclt_delegate_execute));

        event_rules.emplace_back(
            std::make_tuple("Abuse Elevation Control Mechanism: Bypass User Account Control", "Persistence", "T1548.002", "https://attack.mitre.org/techniques/T1548/002/", "Windows", SeverityLevel::High, true, bypass_uac_eventviewer));

        event_rules.emplace_back(
            std::make_tuple("Abuse Elevation Control Mechanism: Bypass User Account Control", "Persistence", "T1548.002", "https://attack.mitre.org/techniques/T1548/002/", "Windows", SeverityLevel::High, true, bypass_uac_disable_reg));

        event_rules.emplace_back(
            std::make_tuple("Office Application Startup", "Persistence", "T1137", "https://attack.mitre.org/techniques/T1137", "Windows", SeverityLevel::High, true, office_applicatoin_startup));

        event_rules.emplace_back(
            std::make_tuple("Boot or Logon Autostart Execution: Registry Run Keys / Startup Folder", "Persistence", "T1547.001", "https://attack.mitre.org/techniques/T1547/001/", "Windows", SeverityLevel::High, true, boot_logon_autostart_execution_run_runonce));
        event_rules.emplace_back(
            std::make_tuple("Supply Chain Compromise", "Initial Access", "T1195", "https://attack.mitre.org/techniques/T1195", "Windows", SeverityLevel::High, true, scheduled_task));

        // event_rules.emplace_back(
        // std::make_tuple("Hardware additions", "Initial Access", "T1200", "https://attack.mitre.org/techniques/T1200","Windows", SeverityLevel::High, true, win_hardware_additions));

        event_rules.emplace_back(
            std::make_tuple("Phishing: Spearphishing Attachment", "Initial Access", "T1566.001", "https://attack.mitre.org/techniques/T1566/001", "Windows", SeverityLevel::High, true, spearphishing_attack));

        event_rules.emplace_back(
            std::make_tuple("External Remote Service", "Initial Access", "T1133", "https://attack.mitre.org/techniques/T1113", "Windows", SeverityLevel::High, true, external_remote_services));

        event_rules.emplace_back(
            std::make_tuple("Automated Exfiltration", "Exfiltration", "T1020", "https://attack.mitre.org/techniques/T1020", "Windows", SeverityLevel::High, true, automated_exfiltration));

        event_rules.emplace_back(
            std::make_tuple("Exfiltration Over Alternative Protocol - Exfiltration Over Asymmetric Encrypted Non-C2 Protocol", "Exfiltration", "T1048.002", "https://attack.mitre.org/techniques/T1048/002", "Windows", SeverityLevel::High, true, exfiltration_over_encrypted_protocol));

        event_rules.emplace_back(
            std::make_tuple("Exfiltration Over Web Service: Exfiltration to Cloud Storage", "Exfiltration", "T1567.002", "https://attack.mitre.org/techniques/T1567/002", "Windows", SeverityLevel::High, true, exfiltration_over_web_service));

        event_rules.emplace_back(
            std::make_tuple("Abuse Nslookup with DNS Records", "Execution", "T1059.001", "https://attack.mitre.org/techniques/T1059/001/", "Windows", SeverityLevel::Medium, true, abuse_nslookup));

        event_rules.emplace_back(
            std::make_tuple("Delete Volume Shadow Copies Via WMI With PowerShell", "Execution", "T1490", "https://attack.mitre.org/techniques/T1490/", "Windows", SeverityLevel::High, true, delete_volume_shadow_copies_via_WMI_with_powershell));

        event_rules.emplace_back(
            std::make_tuple("PowerShell Downgrade Attack", "DEFENCE EVASION", "T1059.001", "https://attack.mitre.org/techniques/T1059/001", "Windows", SeverityLevel::Medium, true, powerShell_downgrade_attack));

        event_rules.emplace_back(
            std::make_tuple("Netcat The Powershell Version", "Command_and_control", "T1095", "https://attack.mitre.org/techniques/T1095", "Windows", SeverityLevel::Medium, true, netcat_the_powershell_version));

        event_rules.emplace_back(
            std::make_tuple("Remote PowerShell Session (PS Classic)", "Execution", "T1059.001", "https://attack.mitre.org/techniques/T1059/001/", "Windows", SeverityLevel::High, true, remote_powershell_session));

        event_rules.emplace_back(
            std::make_tuple("Use Get-NetTCPConnection", "Discovery", "T1049", "https://attack.mitre.org/techniques/T1049/", "Windows", SeverityLevel::Low, true, use_get_net_tcp_connection));

        event_rules.emplace_back(
            std::make_tuple("Tamper Windows Defender - PSClassic", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001", "Windows", SeverityLevel::High, true, tamper_windows_defender));

        event_rules.emplace_back(
            std::make_tuple("Suspicious Non PowerShell WSMAN COM Provider", "Execution", "T1059.001", "https://attack.mitre.org/techniques/T059/001", "Windows", SeverityLevel::Medium, true, suspicious_non_powerShell_WSMAN_COM_provider));

        event_rules.emplace_back(
            std::make_tuple("Suspicious Get Local Groups Information", "Discovery", "T1069.001", "https://attack.mitre.org/techniques/T1069/001/", "Windows", SeverityLevel::Low, true, suspicious_get_local_groups_information));

        event_rules.emplace_back(
            std::make_tuple("Access to Browser Login Data", "Credential_access", "T1555.003", "https://attack.mitre.org/techniques/T1555/003/", "Windows", SeverityLevel::Medium, true, access_to_browser_login_data));

        event_rules.emplace_back(
            std::make_tuple("AMSI Bypass Pattern Assembly GetType", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001/", "Windows", SeverityLevel::High, true, AMSI_bypass_pattern_assembly_getType));

        event_rules.emplace_back(
            std::make_tuple("Powershell Install a DLL in System Directory", "Credential_access", "T1556.002", "https://attack.mitre.org/techniques/T1556/002/", "Windows", SeverityLevel::Medium, true, powershell_install_a_DLL_in_system_directory));

        event_rules.emplace_back(
            std::make_tuple("Registry-Free Process Scope COR_PROFILER", "Persistence", "T1136.001", "https://attack.mitre.org/techniques/T1136/001/", "Windows", SeverityLevel::Medium, true, registry_free_process_scope_COR_PROFILER));

        event_rules.emplace_back(
            std::make_tuple("PowerShell Create Local User", "Execution", "T1574.012", "https://attack.mitre.org/techniques/T1574/012/", "Windows", SeverityLevel::Medium, true, powershell_create_local_user));

        event_rules.emplace_back(
            std::make_tuple("Create Volume Shadow Copy with Powershell", "Credential_access", "T1003.003", "https://attack.mitre.org/techniques/T1003/003/", "Windows", SeverityLevel::High, true, create_volume_shadow_copy_with_powershell));

        event_rules.emplace_back(
            std::make_tuple("Powershell Detect Virtualization Environment", "DEFENCE EVASION", "T1497.001", "https://attack.mitre.org/techniques/T1497/001/", "Windows", SeverityLevel::Medium, true, powershell_detect_virtualization_environment));

        event_rules.emplace_back(
            std::make_tuple("DirectorySearcher Powershell Exploitation", "Discovery", "T1018", "https://attack.mitre.org/techniques/T1018/", "Windows", SeverityLevel::Medium, true, directorySearcher_powershell_exploitation));

        event_rules.emplace_back(
            std::make_tuple("Disable-WindowsOptionalFeature Command PowerShell", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001/", "Windows", SeverityLevel::High, true, disable_WindowsOptionalFeature_command_powershell));

        event_rules.emplace_back(
            std::make_tuple("Dump Credentials from Windows Credential Manager With PowerShell", "Credential_access", "T1555", "https://attack.mitre.org/techniques/T1555/", "Windows", SeverityLevel::Medium, true, dump_credentials_from_windows_credential_manager_with_powershell));

        event_rules.emplace_back(
            std::make_tuple("Enable Windows Remote Management", "Lateral_movement", "T1021.006", "https://attack.mitre.org/techniques/T1021/006/", "Windows", SeverityLevel::Medium, true, enable_windows_remote_management));

        event_rules.emplace_back(
            std::make_tuple("Certificate Exported Via PowerShell", "Credential_access", "T1552.004", "https://attack.mitre.org/techniques/T1552/004/", "Windows", SeverityLevel::Medium, true, certificate_exported_via_powershell));

        event_rules.emplace_back(
            std::make_tuple("Service Registry Permissions Weakness Check", "Persistence", "T1574.011", "https://attack.mitre.org/techniques/T1574/011/", "Windows", SeverityLevel::Medium, true, service_registry_permissions_weakness_check));

        event_rules.emplace_back(
            std::make_tuple("Active Directory Computers Enumeration with Get-AdComputer", "Discovery", "T1018", "https://attack.mitre.org/techniques/T1018/", "Windows", SeverityLevel::Low, true, active_directory_computers_enumeration_with_get_AdComputer));

        event_rules.emplace_back(
            std::make_tuple("Active Directory Computers Enumeration with Get-AdGroup", "Discovery", "T1069.002", "https://attack.mitre.org/techniques/T1069/002/", "Windows", SeverityLevel::Low, true, active_directory_computers_enumeration_with_get_AdGroup));

        event_rules.emplace_back(
            std::make_tuple("Suspicious Get-ADReplAccount", "Credential_access", "T1003.006", "https://attack.mitre.org/techniques/T1003/006/", "Windows", SeverityLevel::Medium, true, suspicious_get_ADReplAccount));

        event_rules.emplace_back(
            std::make_tuple("PowerShell ICMP Exfiltration", "Exfiltration", "T1048.003", "https://attack.mitre.org/techniques/T1048/003/", "Windows", SeverityLevel::Medium, true, powershell_ICMP_exfiltration));

        event_rules.emplace_back(
            std::make_tuple("Execute Invoke-command on Remote Host", "Lateral_movement", "T1021.006", "https://attack.mitre.org/techniques/T1021/006/", "Windows", SeverityLevel::Medium, true, execute_invoke_command_on_remote_host));

        event_rules.emplace_back(
            std::make_tuple("Powershell DNSExfiltration", "Exfiltration", "T1048", "https://attack.mitre.org/techniques/T1048/", "Windows", SeverityLevel::High, true, powershell_DNSExfiltration));

        event_rules.emplace_back(
            std::make_tuple("Powershell Keylogging", "Collection", "T1056.001", "https://attack.mitre.org/techniques/T1056/001/", "Windows", SeverityLevel::Medium, true, powershell_keylogging));

        event_rules.emplace_back(
            std::make_tuple("Powershell LocalAccount Manipulation", "Persistence", "T1098", "https://attack.mitre.org/techniques/T1098/", "Windows", SeverityLevel::Medium, true, powershell_localAccount_manipulation));

        event_rules.emplace_back(
            std::make_tuple("Powershell MsXml COM Object", "Execution", "T1059.001", "https://attack.mitre.org/techniques/T1059/001/", "Windows", SeverityLevel::Medium, true, powershell_MsXml_COM_object));

        event_rules.emplace_back(
            std::make_tuple("NTFS Alternate Data Stream", "DEFENCE EVASION", "T1059.001", "https://attack.mitre.org/techniques/T1059/001/", "Windows", SeverityLevel::High, true, NTFS_alternate_data_stream));

        event_rules.emplace_back(
            std::make_tuple("PowerShell Remote Session Creation", "execution", "T1059.001", "https://attack.mitre.org/techniques/T1059/001/", "Windows", SeverityLevel::Medium, true, powershell_remote_session_creation));

        event_rules.emplace_back(
            std::make_tuple("Use Remove-Item to Delete File", "execution", "T1070.004", "https://attack.mitre.org/techniques/T1070/004/", "Windows", SeverityLevel::Low, true, use_remove_item_to_delete_file));

        event_rules.emplace_back(
            std::make_tuple("Request A Single Ticket via PowerShell", "Credential_access", "T1558.003", "https://attack.mitre.org/techniques/T1558/003/", "Windows", SeverityLevel::High, true, request_a_single_ticket_via_powershell));

        event_rules.emplace_back(
            std::make_tuple("Suspicious Invoke-Item From Mount-DiskImage", "DEFENCE EVASION", "T1553.005", "https://attack.mitre.org/techniques/T1553/005/", "Windows", SeverityLevel::High, true, suspicious_invoke_item_from_mount_diskImage));

        event_rules.emplace_back(
            std::make_tuple("Security Software Discovery by Powershell", "Discovery", "T1518.001", "https://attack.mitre.org/techniques/T1518/001/", "Windows", SeverityLevel::Low, true, security_software_discovery_by_powershell));

        event_rules.emplace_back(
            std::make_tuple("Powershell Exfiltration Over SMTP", "Exfiltration", "T1048.003", "https://attack.mitre.org/techniques/T1048/003/", "Windows", SeverityLevel::Medium, true, powershell_exfiltration_over_SMTP));

        event_rules.emplace_back(
            std::make_tuple("Detected Windows Software Discovery", "Discovery", "T1518", "https://attack.mitre.org/techniques/T1518/", "Windows", SeverityLevel::Medium, true, windows_software_discovery_powershell));

        event_rules.emplace_back(
            std::make_tuple("Powershell Store File In Alternate Data Stream", "DEFENCE EVASION", "T1564.004", "https://attack.mitre.org/techniques/T1564/004/", "Windows", SeverityLevel::Medium, true, powershell_store_file_in_alternate_data_stream));

        event_rules.emplace_back(
            std::make_tuple("Potential PowerShell Obfuscation Using Character Join", "DEFENCE EVASION", "T1059.001", "https://attack.mitre.org/techniques/T1059/001/", "Windows", SeverityLevel::Medium, true, potential_powershell_obfuscation_using_character_join));

        event_rules.emplace_back(
            std::make_tuple("Suspicious Eventlog Clear", "DEFENCE EVASION", "T1070.001", "https://attack.mitre.org/techniques/T1070/001/", "Windows", SeverityLevel::Medium, true, suspicious_eventlog_clear));

        event_rules.emplace_back(
            std::make_tuple("Powershell Execute Batch Script", "Execution", "T1059.003", "https://attack.mitre.org/techniques/T1059/003/", "Windows", SeverityLevel::Medium, true, powershell_execute_batch_script));

        event_rules.emplace_back(
            std::make_tuple("Password Policy Discovery With Get-AdDefaultDomainPasswordPolicy", "Discovery", "T1201", "https://attack.mitre.org/techniques/T1201/", "Windows", SeverityLevel::Low, true, password_policy_discovery_with_Get_AdDefaultDomainPasswordPolicy));

        event_rules.emplace_back(
            std::make_tuple("Suspicious PowerShell Get Current User", "Discovery", "T1033", "https://attack.mitre.org/techniques/T1033/", "Windows", SeverityLevel::Low, true, suspicious_powershell_get_current_user));

        event_rules.emplace_back(
            std::make_tuple("Suspicious Process Discovery With Get-Process", "Discovery", "T1057", "https://attack.mitre.org/techniques/T1057/", "Windows", SeverityLevel::Low, true, suspicious_process_discovery_with_get_process));

        event_rules.emplace_back(
            std::make_tuple("PowerShell Get-Process LSASS in ScriptBlock", "Credential_access", "T1003.001", "https://attack.mitre.org/techniques/T1003/001/", "Windows", SeverityLevel::High, true, powershell_get_process_LSASS_in_scriptblock));

        event_rules.emplace_back(
            std::make_tuple("Suspicious GetTypeFromCLSID ShellExecute", "Persistence", "T1546.015", "https://attack.mitre.org/techniques/T1546/015/", "Windows", SeverityLevel::Medium, true, suspicious_GetTypeFromCLSID_shellexecute));

        event_rules.emplace_back(
            std::make_tuple("Suspicious Hyper-V Cmdlets", "DEFENCE EVASION", "T1564.006", "https://attack.mitre.org/techniques/T1564/006/", "Windows", SeverityLevel::Medium, true, suspicious_hyper_v_cmdlets));

        event_rules.emplace_back(
            std::make_tuple("Change User Agents with WebRequest", "Command_and_control", "T1071.001", "https://attack.mitre.org/techniques/T1071/001/", "Windows", SeverityLevel::Medium, true, change_user_agents_with_webRequest));

        event_rules.emplace_back(
            std::make_tuple("Suspicious IO.FileStream", "DEFENCE EVASION", "T1070.003", "https://attack.mitre.org/techniques/T1070/003/", "Windows", SeverityLevel::Medium, true, suspicious_io_fileStream));

        event_rules.emplace_back(
            std::make_tuple("Powershell Local Email Collection", "Collection", "T1114.001", "https://attack.mitre.org/techniques/T1114/001/", "Windows", SeverityLevel::Medium, true, powershell_local_email_collection));

        event_rules.emplace_back(
            std::make_tuple("PowerShell Deleted Mounted Share", "DEFENCE EVASION", "T1070.005", "https://attack.mitre.org/techniques/T1070/005/", "Windows", SeverityLevel::Medium, true, powershell_deleted_mounted_share));

        event_rules.emplace_back(
            std::make_tuple("Suspicious Connection to Remote Account", "Credential_access", "T1110.001", "https://attack.mitre.org/techniques/T1110/001/", "Windows", SeverityLevel::Low, true, suspicious_connection_to_remote_account));

        event_rules.emplace_back(
            std::make_tuple("Suspicious New-PSDrive to Admin Share", "Lateral_movement", "T1021.002", "https://attack.mitre.org/techniques/T1021/002/", "Windows", SeverityLevel::Medium, true, suspicious_new_PSDrive_to_admin_share));

        event_rules.emplace_back(
            std::make_tuple("Recon Information for Export with PowerShell", "Collection", "T1119", "https://attack.mitre.org/techniques/T1119/", "Windows", SeverityLevel::Medium, true, recon_information_for_export_with_powershell));

        event_rules.emplace_back(
            std::make_tuple("Remove Account From Domain Admin Group", "Impact", "T1531", "https://attack.mitre.org/techniques/T1531/", "Windows", SeverityLevel::Medium, true, remove_account_from_domain_admin_group));
        //
        event_rules.emplace_back(
            std::make_tuple("Suspicious SSL Connection", "Command_and_control", "T1573", "https://attack.mitre.org/techniques/T1573/", "Windows", SeverityLevel::Low, true, suspicious_SSL_connection));

        event_rules.emplace_back(
            std::make_tuple("Suspicious Unblock-File", "DEFENCE EVASION", "T1553.005", "https://attack.mitre.org/techniques/T1553/005/", "Windows", SeverityLevel::Medium, true, suspicious_unblock_file));

        event_rules.emplace_back(
            std::make_tuple("Replace Desktop Wallpaper by Powershell", "Impact", "T1491.001", "https://attack.mitre.org/techniques/T1491/001/", "Windows", SeverityLevel::Low, true, replace_desktop_wallpaper_by_powershell));

        event_rules.emplace_back(
            std::make_tuple("Powershell Suspicious Win32_PnPEntity", "Discovery", "T1120", "https://attack.mitre.org/techniques/T1120/", "Windows", SeverityLevel::Low, true, powershell_suspicious_win32_PnPEntity));

        event_rules.emplace_back(
            std::make_tuple("Suspicious PowerShell WindowStyle Option", "DEFENCE EVASION", "T1564.003", "https://attack.mitre.org/techniques/T1564/003/", "Windows", SeverityLevel::Medium, true, suspicious_powershell_windowStyle_option));

        // event_rules.emplace_back(
        //     std::make_tuple("Tamper Windows Defender Remove-MpPreference - ScriptBlockLogging", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001/","Windows", SeverityLevel::High, true, tamper_windows_defender_remove_MpPreference_ScriptBlockLogging));
        //
        event_rules.emplace_back(
            std::make_tuple("Tamper Windows Defender - ScriptBlockLogging", "DEFENCE EVASION", "T1562.001", "https://attack.mitre.org/techniques/T1562/001/", "Windows", SeverityLevel::High, true, tamper_windows_defender_ScriptBlockLogging));

        event_rules.emplace_back(
            std::make_tuple("Powershell Timestomp", "DEFENCE EVASION", "T1070.006", "https://attack.mitre.org/techniques/T1070/006/", "Windows", SeverityLevel::Medium, true, powershell_timestomp));

        event_rules.emplace_back(
            std::make_tuple("Potential Persistence Via PowerShell User Profile Using Add-Content", "Persistence", "T1546.013", "https://attack.mitre.org/techniques/T1546/013/", "Windows", SeverityLevel::Medium, true, potential_persistence_via_powershell_user_profile_using_add_content));

        event_rules.emplace_back(
            std::make_tuple("PowerShell WMI Win32_Product Install MSI", "DEFENCE EVASION", "T1218.007", "https://attack.mitre.org/techniques/T1218/007/", "Windows", SeverityLevel::Medium, true, powershell_WMI_Win32_product_install_MSI));

        event_rules.emplace_back(
            std::make_tuple("Windows Firewall Profile Disabled", "DEFENCE EVASION", "T1562.004", "https://attack.mitre.org/techniques/T1562/004/", "Windows", SeverityLevel::Medium, true, windows_firewall_profile_disabled));

        // event_rules.emplace_back(
        //     std::make_tuple("Winlogon Helper DLL", "Persistence", "T1547.004", "https://attack.mitre.org/techniques/T1547/004/", SeverityLevel::Medium, true, winlogon_helper_DLL));

        event_rules.emplace_back(
            std::make_tuple("Powershell WMI Persistence", "Privilege_escalation", "T1546.003", "https://attack.mitre.org/techniques/T1546/003/", "Windows", SeverityLevel::Medium, true, powershell_WMI_persistence));

        event_rules.emplace_back(
            std::make_tuple("Powershell XML Execute Command", "Execution", "T1059.001", "https://attack.mitre.org/techniques/T1059/001/", "Windows", SeverityLevel::Medium, true, powershell_XML_execute_command));


       


        event_rules.emplace_back(
            std::make_tuple("RULE_BUILDER", "Rule Builder", "NA", "NA", "NA", SeverityLevel::High, true, rule_builder_rule));
    }

    ~ProcessEventProcessor() override
    {
        event_rules.clear();
        // win_event_rules.clear();
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

    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const ProcessEvent &, Event &)>>> process_event_rules() override
    {
        return this->event_rules;
    }
    // std::vector<std::tuple<std::string, std::string, std::string, std::string, SeverityLevel, bool, std::function<bool(const WinProcessEvent &, Event &)>>> win_process_event_rules() override{
    //         return this->win_event_rules;
    // }
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