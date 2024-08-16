#include "detection_rules.h"
#include <sstream>
#include "../common/ConfigurationParser.h"
#include "../common/logger.h"
#include "../server/RequestRouter.h"
#include "../server/WebServer.h"
#include "../common/hostinfo.h"

// New Rules start from here (Mitre Attack)

// bool ssh_authorized_keys(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.path == "/usr/bin/ssh-keygen")
//     {
//         if ((process_event.entry.cmdline.find("ssh-keygen") != std::string::npos))
//         {
//             std::stringstream ss;

//             ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying systems SSH config file" ;

//             rule_event.metadata = ss.str();

//             return true;
//         }
//     }
//     return false;
// }

bool ssh_authorized_keys(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.file_path.find(".ssh") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying systems SSH config file";

        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// bool kernel_modules_extensions_modprobe(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.path == "/usr/sbin/modprobe")
//     {
//         if ((process_event.entry.cmdline.find("modprobe") != std::string::npos))
//         {
//             std::stringstream ss;

//             ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifing kernel using modprobe" ;

//             rule_event.metadata = ss.str();

//             return true;
//         }
//     }
//     return false;
// }

// https://attack.mitre.org/techniques/T1547/006/

bool kernel_modules_extensions_modprobe(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("modprobe") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifing kernel using modprobe";

        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool kernel_modules_extensions_insmod(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/insmod")
    {
        if ((process_event.entry.cmdline.find("insmod") != std::string::npos))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] inserting into kernel using insmod";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// bool kernel_modules_extensions_insmod(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.cmdline.find("insmod")!= std::string::npos)
//     {

//         std::stringstream ss;

//         ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifing kernel using insmod" ;

//         rule_event.metadata = ss.str();

//         return true;

//     }
//     return false;
// }

bool kernel_modules_extensions_lsmod(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/lsmod")
    {
        if ((process_event.entry.cmdline.find("lsmod") != std::string::npos))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] listing kernel using lsmod";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// bool kernel_modules_extensions_lsmod(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.cmdline.find("lsmod")!= std::string::npos)
//     {

//         std::stringstream ss;

//         ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifing kernel using lsmod" ;

//         rule_event.metadata = ss.str();

//         return true;

//     }
//     return false;
// }

bool kernel_modules_extensions_rmmod(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/rmmod")
    {
        if ((process_event.entry.cmdline.find("rmmod") != std::string::npos))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] removing kernel using rmmod";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// https://attack.mitre.org/techniques/T1547/006/
// bool kernel_modules_extensions_rmmod(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.cmdline.find("rmmod")!= std::string::npos)
//     {

//         std::stringstream ss;

//         ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] removing kernel using rmmod" ;

//         rule_event.metadata = ss.str();

//         return true;

//     }
//     return false;
// }

// https://attack.mitre.org/techniques/T1136/001/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1136.001/T1136.001.md
// Persistence
bool create_local_account_useradd(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/useradd ")
    {
        if ((process_event.entry.cmdline.find("useradd") != std::string::npos))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] creating local account using useradd";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// https://attack.mitre.org/techniques/T1136/001/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1136.001/T1136.001.md
// Persistence
bool create_local_account_adduser(const ProcessEvent &process_event, Event &rule_event)
{
    if ((process_event.entry.cmdline.find("adduser") != std::string::npos))
    {
        if (process_event.entry.path == "/usr/sbin/adduser")
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] creating local account using adduser";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// bool create_local_account(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.cmdline.find("useradd")!= std::string::npos or process_event.entry.cmdline.find("adduser")!= std::string::npos)
//     {

//         std::stringstream ss;

//         ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] creating local account" ;

//         rule_event.metadata = ss.str();

//         return true;

//     }
//     return false;
// }

bool create_domain_account(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("net user /add /domain") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] creating domain account";

        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// done check
// https://attack.mitre.org/techniques/T1546/005/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1546.005/T1546.005.md
bool event_triggered_execution_trap(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("trap") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] event triggered as trap";

        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// https://attack.mitre.org/techniques/T1546/005/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1574.006/T1574.006.md
bool dynamic_linker_hijacking(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("LD_PRELOAD") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] dynamic linker hijacking";

        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// https://attack.mitre.org/techniques/T1053/002/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1053.002/T1053.002.md
bool scheduled_task_at(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/at")
    {
        if ((process_event.entry.cmdline.find("at") != std::string::npos))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] scheduling task using at";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// https://attack.mitre.org/techniques/T1053/002/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1053.002/T1053.002.md
bool scheduled_task_atrm(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/atrm")
    {
        if ((process_event.entry.cmdline.find("atrm") != std::string::npos))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] removing task using atrm";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}
// https://attack.mitre.org/techniques/T1053/003
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1053.003/T1053.003.md
bool scheduled_task_cron(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/crontab")
    {
        if ((process_event.entry.cmdline.find("crontab") != std::string::npos))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] scheduled task creation using crontab";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// https://attack.mitre.org/techniques/T1053/006/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1053.006/T1053.006.md
bool scheduled_task_systemd_timers(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("systemctl") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] performing task scheduling using systemd timers";

        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// Done checked
bool abuse_elevation_control_setuid(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("chmod u+s") != std::string::npos or process_event.entry.cmdline.find("chmod u+x") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying setuid bits for an application";

        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// Done checked
bool abuse_elevation_control_setuid1(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/chmod")
    {
        if (process_event.entry.cmdline.find("chmod u+s") != std::string::npos or process_event.entry.cmdline.find("chmod u+x") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying setuid bits for an application (path)";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// Done checked

bool abuse_elevation_control_sudo_caching(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/visudo")
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/sudo")
            {
                if (process.cmdline.find("sudo visudo") != std::string::npos)
                {
                    std::stringstream ss;

                    ss << "[" << process.path << " (pid: " << process.pid << ")] caching the suoders file to elevate privileges";

                    rule_event.metadata = ss.str();

                    return true;
                }
            }
        }
    }
    return false;
}

// Not done
bool create_modify_system_systemd_service(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/systemctl")
    {
        if (process_event.entry.cmdline.find("systemctl list-units -–type=service –all") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying systemd service";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// check done
bool terminal_downloads_curl(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/curl")
    {
        if ((process_event.entry.cmdline.find("curl") != std::string::npos) and (process_event.entry.cmdline.find("%://%") != std::string::npos) or (process_event.entry.cmdline.find("%.%") != std::string::npos))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] downloading curl using terminal";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool impair_command_history_logging(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("$HISTFILE") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] performing clear history command";

        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool terminal_downloads_python(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/python3" or process_event.entry.path == "/usr/bin/python")
    {
        if ((process_event.entry.cmdline.find("%python%") != std::string::npos) or (process_event.entry.cmdline.find("%://%") != std::string::npos))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] downloading python using terminal";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool bash_history_commandLine(const ProcessEvent &process_event, Event &rule_event)
{
    if ((process_event.entry.cmdline.find("unset HISTFILE") != std::string::npos) or (process_event.entry.cmdline.find("export HISTFILESIZE=0") != std::string::npos) or (process_event.entry.cmdline.find("history -c") != std::string::npos) or (process_event.entry.cmdline.find("rm ~/.bash_history") != std::string::npos))
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] downloading curl using terminal";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// Privleged Command Execution Related Events
bool ping_privileged_commandLine(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/ping")
    {
        if (process_event.entry.cmdline.find("ping") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] ping using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool mount_privileged_commandLine(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/mount")
    {
        if (process_event.entry.cmdline.find("mount") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] mount using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool umount_privileged_commandLine(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/umount")
    {
        if (process_event.entry.cmdline.find("umount") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] umount using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool chgrp_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/chgrp")
    {
        if (process_event.entry.cmdline.find("chgrp") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] chgrp using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}
// check done
bool pam_timestamp_check_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/pam_timestamp_check")
    {
        if (process_event.entry.cmdline.find("pam_timestamp_check") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] pam_timestamp_check using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool unix_chkpwd_check_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/unix_chkpwd")
    {
        if (process_event.entry.cmdline.find("unix_chkpwd") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] unix_chkpwd using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool pwck_check_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/pwck")
    {
        if (process_event.entry.cmdline.find("pwck") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] pwck using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool userhelper_check_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/userhelper")
    {
        if (process_event.entry.cmdline.find("userhelper") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] userhelper using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool Xorg_check_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/Xorg")
    {
        if (process_event.entry.cmdline.find("Xorg") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] Xorg using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool rlogin_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/rlogin")
    {
        if (process_event.entry.cmdline.find("rlogin") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] rlogin using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool sudoedit_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/sudoedit")
    {
        if (process_event.entry.cmdline.find("sudoedit") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] sudoedit using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool rsh_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/rsh")
    {
        if (process_event.entry.cmdline.find("rsh") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] rsh using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool gpasswd_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/gpasswd")
    {
        if (process_event.entry.cmdline.find("gpasswd") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] gpasswd using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool sudo_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/sudo")
    {
        if (process_event.entry.cmdline.find("sudo") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] sudo using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool staprun_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/staprun")
    {
        if (process_event.entry.cmdline.find("staprun") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] staprun using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool rcp_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/rcp")
    {
        if (process_event.entry.cmdline.find("rcp") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] rcp using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool passwd_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/passwd")
    {
        if (process_event.entry.cmdline.find("passwd") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] passwd using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool chsh_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/chsh")
    {
        if (process_event.entry.cmdline.find("chsh") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] chsh using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool chfn_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/chfn")
    {
        if (process_event.entry.cmdline.find("chfn") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] chfn using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool chage_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/chage")
    {
        if (process_event.entry.cmdline.find("chage") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] chage using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool setfacl_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/setfacl")
    {
        if (process_event.entry.cmdline.find("setfacl") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] setfacl using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool chacl_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/chacl")
    {
        if (process_event.entry.cmdline.find("chacl") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] chacl using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool chcon_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/chcon")
    {
        if (process_event.entry.cmdline.find("chcon") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] chcon using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool newgrp_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/newgrp")
    {
        if (process_event.entry.cmdline.find("newgrp") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] newgrp using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool sleep_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/sleep")
    {
        if (process_event.entry.cmdline.find("sleep") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] sleep using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool pgrep_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/pgrep")
    {
        if (process_event.entry.cmdline.find("pgrep") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] pgrep using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool grep_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/grep")
    {
        if (process_event.entry.cmdline.find("grep") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] grep using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool lspci_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/lspci")
    {
        if (process_event.entry.cmdline.find("lspci") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] lspci using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool udevadm_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/udevadm")
    {
        if (process_event.entry.cmdline.find("udevadm") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] udevadm using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool findmnt_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/findmnt")
    {
        if (process_event.entry.cmdline.find("findmnt") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] findmnt using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool netstat_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/netstat")
    {
        if (process_event.entry.cmdline.find("netstat") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] netstat using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool awk_privileged_command(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/awk")
    {
        if (process_event.entry.cmdline.find("awk") != std::string::npos)
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] awk using command line";
            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

// bool sed_privileged_command(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.path == "/usr/bin/sed")
//     {
//         if (process_event.entry.cmdline.find("sed") != std::string::npos)
//         {
//             std::stringstream ss;

//             ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] sed using command line" ;
//             rule_event.metadata = ss.str();

//             return true;
//         }
//     }
//     return false;
// }

// https://attack.mitre.org/techniques/T1497/001/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1497.001/T1497.001.md

bool virtualization_evasion_system_checks_systemd_detect_virt(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/systemd-detect-virt" or process_event.entry.cmdline == "systemd-detect-virt")
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1497.001 - System Checks - Virtual env detect";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1497/001/
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1497.001/T1497.001.md
// sub-technique id - T1497.001
// technique id - T1497
// technique - Virtualization/Sandbox Evasion
// subtechnique - 	System Checks
// discription - Adversaries may employ various means to detect and avoid virtualization and analysis environments. This may include changing behaviors based 
// on the results of checks for the presence of artifacts indicative of a virtual machine environment (VME) or sandbox. If the adversary detects a VME, they may 
// alter their malware to disengage from the victim or conceal the core functions of the implant. They may also search for VME artifacts before dropping secondary 
// or additional payloads. Adversaries may use the information learned from [Virtualization/Sandbox Evasion] during 
// automated discovery to shape follow-on behaviors.
// Adversaries may use several methods to accomplish [Virtualization/Sandbox Evasion] such as checking for security monitoring tools 
// (e.g., Sysinternals, Wireshark, etc.) or other system artifacts associated with analysis or virtualization. Adversaries may also check for legitimate user 
// activity to help determine if it is in an analysis environment. Additional methods include use of sleep timers or loops within malware code to avoid operating within a temporary sandbox.

bool virtualization_evasion_system_checks_dmidecode(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/sbin/dmidecode" or process_event.entry.cmdline == "dmidecode")
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1497.001 - System Checks - dmidecode";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1070/006/
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.006/T1070.006.md

bool set_file_access_timestamp(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("touch -a -t") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1070.006 - Set a file's access timestamp";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1070/006/
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.006/T1070.006.md

bool modify_file_timestamp_using_reference_file(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("touch -acmr") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1070.006 - Modify file timestamps using reference file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1070/006/
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.006/T1070.006.md

bool sudo_and_sudo_caching_sudo_usage(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/sudoers") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1548 - Sudo usage";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1548/003/
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1548.003/T1548.003.md
bool sudo_and_sudo_caching_unlimited_sudo_timeout(const ProcessEvent &process_event, Event &rule_event)
{
    if ((process_event.entry.cmdline.find("/etc/sudoers") != std::string::npos) and (process_event.entry.cmdline.find("sudo sed -i 's/env_reset.*$/env_reset,timestamp_timeout=-1/' /etc/sudoers") != std::string::npos) or (process_event.entry.cmdline.find("env_reset") != std::string::npos) or (process_event.entry.cmdline.find("timestamp_timeout=-1") != std::string::npos))
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1548.003 - Unlimited sudo cache timeout";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1548/003/
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1548.003/T1548.003.md
bool sudo_and_sudo_caching_disable_tty_tickets_sudo_caching(const ProcessEvent &process_event, Event &rule_event)
{
    if ((process_event.entry.cmdline.find("/etc/sudoers") != std::string::npos) and (process_event.entry.cmdline.find("Defaults \"'!'\"tty_tickets") != std::string::npos))
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1548.003 - Disable tty_tickets for sudo caching";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1548/003/
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.003/T1562.003.md
bool impair_cmd_history_logging_disable_history_collection(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("export HISTCONTROL=ignoreboth") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.003 - Disable history collection";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1548/003/
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.003/T1562.003.md
bool impair_cmd_history_logging_mac_hist_control(const ProcessEvent &process_event, Event &rule_event)
{
    if ((process_event.entry.cmdline.find("export HISTCONTROL=ignoreboth") != std::string::npos) and (process_event.entry.cmdline.find("echo export \"HISTCONTROL=ignoreboth\" >> ~/.bash_profile") != std::string::npos))
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.003 - Mac HISTCONTROL";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1070/004
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.004/T1070.004.md
bool file_deletion_single_file(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("rm -f") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1070.004 - Delete a single file - Linux/macOS";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1070/004
// // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.004/T1070.004.md
bool file_deletion_entire_folder(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("rm -rf") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1070.004 - Delete entire folder - Linux/macOS";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1070/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.004/T1070.004.md
bool overwrite_and_delete_file_with_shred(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("shred -u") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1070.004 - Atomic Test #3 - Overwrite and delete a file with shred";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// // https://attack.mitre.org/techniques/T1070/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.004/T1070.004.md
bool delete_filesystem_root(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("rm -rf / --no-preserve-root > /dev/null 2> /dev/null") != std::string::npos)
    {

        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1070.004 -Atomic Test #8 - Delete Filesystem - Linux";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

//--------------------------------------------------------------------------- Need File events------------------------------------------------------
// // // https://attack.mitre.org/techniques/T1070/006/
// // // https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.006/T1070.006.md

// bool set_file_creation_timestamp(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.cmdline.find("touch -m -t")!= std::string::npos)
//     {

//             std::stringstream ss;

//             ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1070.006 - Set a file's modification timestamp" ;
//             rule_event.metadata = ss.str();

//             return true;
//     }
//     return false;
// }

// File Events Rules

// bool bash_history_modification(const ProcessEvent& file_events, Event& rule_event)
// {
//     if (file_events.entry.file_path.find("%.bash_history%") != std::string::npos)
//     {
//         std::stringstream ss;

//         ss << "[" << file_events.entry.path << " (pid: " << file_events.entry.pid << ")] performing history modification" ;

//         rule_event.metadata = ss.str();

//         return true;
//     }
//     return false;
// }
//%%%%%%%%%%%%%%%%% Using Red Canary %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1053.003/T1053.003.md
// T1053.003 - Cron

// Atomic Test #1 - Cron - Replace crontab with referenced file

bool modifying_cron_file_reference(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("crontab -l") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] replace crontab with referenced file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// Atomic Test #2 - Cron - Add script to all cron subfolders

bool modifying_cron_daily_file(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/cron.daily/") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying cron daily file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool modifying_cron_hourly_file(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/cron.hourly/") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying cron hourly file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool modifying_cron_monthly_file(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/cron.monthly/") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying cron monthly file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool modifying_cron_weekly_file(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/cron.weekly/") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying cron weekly file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// Atomic Test #3 - Cron - Add script to /var/spool/cron/crontabs/ folder

bool modifying_cron_var_file(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/var/spool/cron/crontabs/") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying cron var file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1574.006/T1574.006.md
// T1574.006 - Dynamic Linker Hijacking

// Atomic Test #1 - Shared Library Injection via /etc/ld.so.preload

bool shared_library_injection(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/ld.so.preload") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] shared library injection";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// Atomic Test #2 - Shared Library Injection via LD_PRELOAD

bool shared_library_injection_ld_preload(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("LD_PRELOAD=") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] shared library injection via LD Preload";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// T1547.006 - Kernel Modules and Extensions

bool load_kernel_module_insmod(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("sudo insmod") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] load kernel module via insmod";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1556.003/T1556.003.md

// Atomic Test #1 - Malicious PAM rule

bool pam_config_file_modify(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/pam.d/su-l") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] PAM config file to modify";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool rule_add_pam_config(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("auth sufficient pam_succeed_if.so uid >= 0") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] Rule to add to the PAM config";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1037.004/T1037.004.md
// https://attack.mitre.org/techniques/T1037/004
// Atomic Test #2 - rc.common

bool rc_script_common_midify(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/rc.common") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying RC Scripts common";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool rc_script_local_midify(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/rc.local") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying RC Scripts local";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1098.004/T1098.004.md

// Atomic Test #1 - Modify SSH Authorized Keys

bool ssh_authorized_keys_midify(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find(".ssh/authorized_keys") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] modifying ssh authorized keys";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1543.002/T1543.002.md

// Atomic Test #1 - Create Systemd Service

bool create_systemd_service_path(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/systemd/system") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] create systemd service using path";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool create_systemd_service_file(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("art-systemd-service.service") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] create systemd service using path";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// https://attack.mitre.org/techniques/T1053/006
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1053.006/T1053.006.md

// Atomic Test #1 - Create Systemd Service and Timer

bool create_systemd_service__timer_service(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/systemd/system/art-timer.service") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] creates Systemd service and timer using unit file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool create_systemd_service__timer_file(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("/etc/systemd/system/art-timer.timer") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] creates Systemd service and timer using timer file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// Lateral Movement
// Remote Services SSH
// https://redcanary.com/blog/lateral-movement-with-secure-shell/

bool lateral_movement_with_secure_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if ((process_event.entry.cmdline.find("ssh") != std::string::npos) or (process_event.entry.cmdline.find("known_hosts") != std::string::npos) or (process_event.entry.cmdline.find("oBatchMode") != std::string::npos) or (process_event.entry.cmdline.find("oStrictHostKeyChecking") != std::string::npos))
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] lateral movement using secure shell vulnerability";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool lateral_movement_with_ssh_rsa(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("ssh-rsa") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] lateral movment using authorized key and modifying file";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

bool lateral_tool_transfer_files(const ProcessEvent &process_event, Event &rule_event)
{
    if ((process_event.entry.cmdline.find("scp") != std::string::npos) or (process_event.entry.cmdline.find("rsync") != std::string::npos) or (process_event.entry.cmdline.find("curl") != std::string::npos) or (process_event.entry.cmdline.find("sftp") != std::string::npos) or (process_event.entry.cmdline.find("ftp") != std::string::npos))
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] lateral movement using secure shell vulnerability";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}


// https://attack.mitre.org/techniques/T1574/006/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1574.006/T1574.006.md
bool shared_library_injection_ld_so_preload(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("sh -c 'echo")!= std::string::npos) and (process_event.entry.cmdline.find("/etc/ld.so.preload")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1574.006 - Dynamic Linker Hijacking Shared Library Injection via /etc/ld.so.preload" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

//T1562.001 - Disable or Modify Tools
// Atomic Test #1 - Disable syslog
bool disable_syslog(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("systemctl stop rsyslog")!= std::string::npos) or (process_event.entry.cmdline.find("systemctl disable rsyslog")!= std::string::npos) or (process_event.entry.cmdline.find("service rsyslog stop")!= std::string::npos) or (process_event.entry.cmdline.find("chkconfig off rsyslog")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.001 - Disable or Modify Tools - Disable syslog" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

//T1562.001 - Disable or Modify Tools
// Atomic Test #1 - Disable Cb Response
// https://attack.mitre.org/techniques/T1562/001/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.001/T1562.001.md
bool disable_cb_response(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("service cbdaemon stop")!= std::string::npos) or (process_event.entry.cmdline.find("chkconfig off cbdaemon")!= std::string::npos) or (process_event.entry.cmdline.find("systemctl stop cbdaemon")!= std::string::npos) or (process_event.entry.cmdline.find("systemctl disable cbdaemon")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.001 - Disable or Modify Tools - Disable Cb Response" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


//T1562.001 - Disable or Modify Tools
// Atomic Test #1 - Disable SELinux

// https://attack.mitre.org/techniques/T1562/003/
// https://attack.mitre.org/techniques/T1562/003/
bool disable_se_linux(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("setenforce 0")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.001 - Disable or Modify Tools - Disable Cb Response" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

//T1562.001 - Disable or Modify Tools
// Atomic Test #4 - Stop Crowdstrike Falcon on Linux
bool stop_crowdstrike_falcon(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("systemctl stop falcon-sensor.service")!= std::string::npos) or (process_event.entry.cmdline.find("systemctl disable falcon-sensor.service")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.001 - Disable or Modify Tools - Stop Crowdstrike Falcon on Linux" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

//T1562.004 - Disable or Modify System Firewall
// Atomic Test #7 - Stop/Start UFW firewall
// https://attack.mitre.org/techniques/T1562/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.004/T1562.004.md
bool stop_start_ufw_firewall(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("ufw disable")!= std::string::npos) or (process_event.entry.cmdline.find("ufw enable")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.004 - Disable or Modify System Firewall - Stop/Start UFW firewall" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


//T1562.004 - Disable or Modify System Firewall systemctl
// Atomic Test #7 - Stop/Start UFW firewall stsyemctl
// https://attack.mitre.org/techniques/T1562/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.004/T1562.004.md
bool stop_start_ufw_firewall_systemctl(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("systemctl stop ufw")!= std::string::npos) or (process_event.entry.cmdline.find("systemctl start ufw")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.004 - Disable or Modify System Firewall - Stop/Start UFW firewall systemctl" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


//T1562.004 - Disable or Modify System Firewall
// Atomic Test #9 - Turn off UFW logging
// https://attack.mitre.org/techniques/T1562/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.004/T1562.004.md
bool turn_off_ufw_logging(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("ufw logging off")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.004 - Disable or Modify System Firewall - Turn off UFW logging" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

//T1562.004 - Disable or Modify System Firewall
// Atomic Test #10 - Add and delete UFW firewall rules
// https://attack.mitre.org/techniques/T1562/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.004/T1562.004.md
bool add_delete_ufw_rules(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("ufw prepend deny from")!= std::string::npos) or (process_event.entry.cmdline.find("ufw status numbered")!= std::string::npos) or (process_event.entry.cmdline.find("ufw prepend")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.004 - Disable or Modify System Firewall - Add/Delete UFW Rules" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


//T1562.004 - Disable or Modify System Firewall
// Atomic Test #11 - Edit UFW firewall user.rules file
// https://attack.mitre.org/techniques/T1562/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.004/T1562.004.md
bool edit_ufw_user_rules_file(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("echo")!= std::string::npos) and (process_event.entry.cmdline.find("/etc/ufw/user.rules")!= std::string::npos) or (process_event.entry.cmdline.find("user.rules")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.004 - Disable or Modify System Firewall - Edit UFW firewall user.rules file" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


//T1562.004 - Disable or Modify System Firewall
//Atomic Test #12 - Edit UFW firewall ufw.conf file
// https://attack.mitre.org/techniques/T1562/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.004/T1562.004.md
bool edit_ufw_conf_file(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("echo")!= std::string::npos) and (process_event.entry.cmdline.find("/etc/ufw/ufw.conf")!= std::string::npos) or (process_event.entry.cmdline.find("ufw.conf")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.004 - Disable or Modify System Firewall - Edit UFW firewall ufw.conf file" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

//T1562.004 - Disable or Modify System Firewall
//Atomic Test #13 - Edit UFW firewall sysctl.conf file
// https://attack.mitre.org/techniques/T1562/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.004/T1562.004.md
bool edit_ufw_rules_sysctl_conf_file(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("echo")!= std::string::npos) and (process_event.entry.cmdline.find("/etc/ufw/sysctl.conf")!= std::string::npos) or (process_event.entry.cmdline.find("sysctl.conf")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.004 - Disable or Modify System Firewall - Edit UFW firewall sysctl.conf file" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

//T1562.004 - Disable or Modify System Firewall
//Atomic Test #14 - Edit UFW firewall main configuration file
// https://attack.mitre.org/techniques/T1562/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.004/T1562.004.md
bool edit_ufw_firewall_main_config_file(const ProcessEvent& process_event, Event& rule_event)
{
    if ((process_event.entry.cmdline.find("echo")!= std::string::npos) and (process_event.entry.cmdline.find("/etc/default/ufw")!= std::string::npos) or (process_event.entry.cmdline.find("default/ufw")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.004 - Disable or Modify System Firewall - Edit UFW firewall main configuration file" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

//T1562.004 - Disable or Modify System Firewall
// Atomic Test #15 - Tail the UFW firewall log file
// 
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1562.004/T1562.004.md
bool tail_ufw_firewall_log_file(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("tail /var/log/ufw.log")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1562.004 - Disable or Modify System Firewall - Tail the UFW firewall log file" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// Atomic Test #3 - Base64 decoding with Python
//https://attack.mitre.org/techniques/T1140
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1140/T1140.md#atomic-test-3---base64-decoding-with-python
bool base64_decoding_python(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("python")!= std::string::npos and process_event.entry.cmdline.find("b64decode")!= std::string::npos )
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1140 - Deobfuscate/Decode Files or Information" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// Atomic Test #4 - Base64 decoding with Perl
//https://attack.mitre.org/techniques/T1140
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1140/T1140.md#atomic-test-4---base64-decoding-with-perl
bool base64_decoding_perl(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("perl")!= std::string::npos and process_event.entry.cmdline.find("decode_base64")!= std::string::npos )
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1140 - Deobfuscate/Decode Files or Information" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// Atomic Test #5 - Base64 decoding with shell utilities
//https://attack.mitre.org/techniques/T1140
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1140/T1140.md#atomic-test-5---base64-decoding-with-shell-utilities
bool base64_decoding_shell_utilities(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("base64 -d")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1140 - Deobfuscate/Decode Files or Information" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

//Atomic Test #6 - Hex decoding with shell utilities
//https://attack.mitre.org/techniques/T1140
//https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1140/T1140.md#atomic-test-6---hex-decoding-with-shell-utilities
bool hex_decoding_shell_utilities(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("xxd -r -p")!= std::string::npos or process_event.entry.cmdline.find("xxd -p -r")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1140 - Deobfuscate/Decode Files or Information" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1027.004 - Compile After Delivery
// https://attack.mitre.org/techniques/T1027/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1027.004/T1027.004.md#t1027004---compile-after-delivery
bool compile_after_delivery_c_compile(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("gcc")!= std::string::npos or process_event.entry.cmdline.find("clang")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1027.004 - Compile After Delivery C compile" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1027.004 - Compile After Delivery
// https://attack.mitre.org/techniques/T1027/004
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1027.004/T1027.004.md#t1027004---compile-after-delivery
bool compile_after_delivery_go_compile(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("go run")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1027.004 - Compile After Delivery Go compile" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1078.004 - Cloud Accounts
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1078.004/T1078.004.md#t1078004---cloud-accounts
bool creating_gcp_service_account_and_key(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("gcloud auth login")!= std::string::npos or process_event.entry.cmdline.find("gcloud config set project")!= std::string::npos or process_event.entry.cmdline.find("gcloud config set project")!= std::string::npos or process_event.entry.cmdline.find("gcloud iam service-accounts create")!= std::string::npos or process_event.entry.cmdline.find("gcloud iam service-accounts keys create")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1078.004 - Cloud Accounts Creating GCP Service Account and Service Account Key" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1070.002 - Clear Linux or Mac System Logs
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.002/T1070.002.md#t1070002---clear-linux-or-mac-system-logs
bool clear_linux_logs_rm_rf(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("rm -rf /private/var/log/system.log*")!= std::string::npos or process_event.entry.cmdline.find("rm -rf /private/var/audit/*")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1070.002 - Clear Linux or Mac System Logs" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1070.002 - Clear Linux or Mac System Logs
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1070.002/T1070.002.md#t1070002---clear-linux-or-mac-system-logs
bool overwrite_linux_mail_spool_and_logs(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("echo 0> /var/spool/mail/")!= std::string::npos or process_event.entry.cmdline.find("echo 0>")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1070.002 - Clear Linux or Mac System Logs" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1053.001 - At (Linux)
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1053.001/T1053.001.md#t1053001---at-linux
bool at_schedule_a_job(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.path.find("/usr/bin/at")!= std::string::npos or process_event.entry.cmdline.find("at")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1053.001 - At (Linux) Schedule a job" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1033 - System Owner/User Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1033/T1033.md#t1033---system-owneruser-discovery
// Supported Platforms: Linux, macOS
bool system_owner_user_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.path.find("/usr/bin/w")!= std::string::npos or process_event.entry.cmdline==("w") or process_event.entry.path.find("/usr/bin/who")!= std::string::npos or process_event.entry.cmdline==("who") or process_event.entry.path.find("/usr/bin/whoami")!= std::string::npos or process_event.entry.cmdline==("whoami") or process_event.entry.path.find("/usr/bin/users")!= std::string::npos or process_event.entry.cmdline==("users"))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1033 - System Owner/User Discovery" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1087.001 - Local Account
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1087.001/T1087.001.md#t1087001---local-account
// Supported Platforms: Linux
bool enumerate_all_accounts_local(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("cat /etc/passwd")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1087.001 - Local Account Enumerate all accounts (Local)" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1087.001 - Local Account
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1087.001/T1087.001.md#t1087001---local-account
// Supported Platforms: Linux
bool view_sudoers_file(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("cat /etc/sudoers")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1087.001 - Local Account - View sudoers access" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1087.001 - Local Account
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1087.001/T1087.001.md#t1087001---local-account
// Supported Platforms: Linux, MacOS
bool view_accounts_with_uid_0(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("grep 'x:0:' /etc/passwd")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1087.001 - Local Account - View accounts with UID 0" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1087.001 - Local Account
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1087.001/T1087.001.md#t1087001---local-account
// Supported Platforms: Linux, MacOS
bool list_opened_files_by_user(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("username=$(id -u -n) && lsof -u $username")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1087.001 - Local Account - List opened files by user" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1087.001 - Local Account
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1087.001/T1087.001.md#t1087001---local-account
// Supported Platforms: Linux
bool show_if_a_user_account_has_ever_logged_in_remotely(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("lastlog")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1087.001 - Local Account - Show if a user account has ever logged in remotely" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1087.001 - Local Account
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1087.001/T1087.001.md#t1087001---local-account
// Supported Platforms: Linux
bool enumerate_users_and_groups(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("groups")!= std::string::npos or process_event.entry.cmdline.find("id")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1087.001 - Local Account - Enumerate users and groups" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1007 - System Service Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1007/T1007.md#t1007---system-service-discovery
// Supported Platforms: Linux
bool system_service_discovery_systemctl(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("systemctl --type=service")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1007 - System Service Discovery - systemctl" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1040 - Network Sniffing
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1040/T1040.md#t1040---network-sniffing
// Supported Platforms: Linux
bool packet_capture_linux(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("tcpdump")!= std::string::npos or process_event.entry.cmdline.find("tshark")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1040 - Network Sniffing - Packet Capture Linux" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1135 - Network Share Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1135/T1135.md#t1135---network-share-discovery
// Supported Platforms: Linux
bool network_share_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("smbstatus --shares")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] T1135 - Network Share Discovery " ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1082 - System Information Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1082/T1082.md#t1082---system-information-discovery
// Supported Platforms: Linux
bool list_os_information(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("uname -a")!= std::string::npos or process_event.entry.cmdline.find("cat /etc/lsb-release")!= std::string::npos or process_event.entry.cmdline.find("cat /etc/redhat-release")!= std::string::npos or process_event.entry.cmdline.find("cat /etc/issue")!= std::string::npos or process_event.entry.cmdline.find("uptime")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")]  T1082 - System Information Discovery - List OS Information" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1082 - Linux VM Check via Hardware [linux]
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1082/T1082.md#t1082---system-information-discovery
// Supported Platforms: Linux
bool linux_vm_check_via_hardware(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/sys/class/dmi/id/bios_version")!= std::string::npos or process_event.entry.file_path.find("/sys/class/dmi/id/product_name")!= std::string::npos or process_event.entry.file_path.find("/sys/class/dmi/id/chassis_vendor")!= std::string::npos or process_event.entry.file_path.find("/proc/scsi/scsi")!= std::string::npos or process_event.entry.file_path.find("/proc/ide/hd0/model")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]  T1082 - Linux VM Check via Hardware [linux]" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1082 -  Linux VM Check via Kernel Modules
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1082/T1082.md#t1082---system-information-discovery
// Supported Platforms: Linux
bool linux_vm_check_via_kernel_modules(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("lsmod")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]  T1082 - Linux VM Check via Kernel Modules`" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1082 -  Hostname Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1082/T1082.md#t1082---system-information-discovery
// Supported Platforms: Linux
bool hostname_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("hostname")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]  T1082 - Hostname Discovery" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1082 -  Environment variables discovery on macos and linux
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1082/T1082.md#t1082---system-information-discovery
// Supported Platforms: Linux
bool environment_variables_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("env")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]  T1082 - Environment variables discovery on macos and linux" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1217 - Browser Bookmark Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1217/T1217.md#t1217---browser-bookmark-discovery// Supported Platforms: Linux
bool list_mozilla_firefox_bookmark_database_files(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("*.mozilla/firefox/*/places.sqlite")!= std::string::npos or process_event.entry.file_path.find("*.mozilla/firefox/*/places.sqlite")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]   T1217 - Browser Bookmark Discovery - List Mozilla Firefox Bookmark Database Files on Linux" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1016 - System Network Configuration Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1217/T1217.md#t1217---browser-bookmark-discovery// Supported Platforms: Linux
bool system_network_configuration_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("arp -a")!= std::string::npos or process_event.entry.cmdline.find("ifconfig")!= std::string::npos or process_event.entry.cmdline.find("ip addr")!= std::string::npos or process_event.entry.cmdline.find("netstat -ant")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]   T1217 - Browser Bookmark Discovery - List Mozilla Firefox Bookmark Database Files on Linux" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1083 - File and Directory Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1083/T1083.md#t1083---file-and-directory-discovery
bool nix_file_and_directory_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("ls -a")!= std::string::npos or process_event.entry.cmdline.find("ls -la")!= std::string::npos or process_event.entry.cmdline.find("file */* *")!= std::string::npos or process_event.entry.cmdline.find("find")!= std::string::npos or process_event.entry.cmdline.find("ls -R ")!= std::string::npos or process_event.entry.cmdline.find("locate")!= std::string::npos or process_event.entry.cmdline.find("which")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1083 - File and Directory Discovery - Nix File and Directory Discovery" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1049 - System Network Connections Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1049/T1049.md#t1049---system-network-connections-discovery
bool system_network_connections_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("netstat")!= std::string::npos or process_event.entry.cmdline.find("who")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1049 - System Network Connections Discovery - " ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1057 - Process Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1057/T1057.md#t1057---process-discovery
bool process_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("ps")!= std::string::npos or process_event.entry.cmdline.find("ps aux")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1057 - Process Discovery - ps " ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1069.001 - Local Groups
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1069.001/T1069.001.md#t1069001---local-groups
bool permission_groups_discovery_local(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("dscacheutil -q group")!= std::string::npos or process_event.entry.cmdline.find("dscl . -list /Groups")!= std::string::npos or process_event.entry.cmdline.find("groups")!= std::string::npos or process_event.entry.cmdline.find("id")!= std::string::npos or process_event.entry.cmdline.find("getent group")!= std::string::npos or process_event.entry.cmdline.find("/etc/group")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1069.001 - Local Groups - Permission Groups Discovery (Local)" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1201 - Password Policy Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1201/T1201.md#t1201---password-policy-discovery
// bool examine_password_complexity_policy(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.file_path.find("/etc/pam.d/common-password")!= std::string::npos or process_event.entry.file_path.find("/etc/security/pwquality.conf")!= std::string::npos or process_event.entry.file_path.find("/etc/pam.d/system-auth")!= std::string::npos or process_event.entry.file_path.find("/etc/security/pwquality.conf")!= std::string::npos or process_event.entry.file_path.find("/etc/login.defs")!= std::string::npos)
//     {

//             std::stringstream ss;

//             ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1201 - Password Policy Discovery - Examine password expiration policy " ;
//             rule_event.metadata = ss.str();

//             return true;
//     }
//     return false;
// }


// T1518.001 - Security Software Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1518.001/T1518.001.md#t1518001---security-software-discovery
bool security_software_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("ps aux")!= std::string::npos and (process_event.entry.cmdline.find("falcond")!= std::string::npos or process_event.entry.cmdline.find("nessusd")!= std::string::npos or process_event.entry.cmdline.find("cbagentd")!= std::string::npos or process_event.entry.cmdline.find("td-agent")!= std::string::npos or process_event.entry.cmdline.find("packetbeat")!= std::string::npos or process_event.entry.cmdline.find("filebeat")!= std::string::npos or process_event.entry.cmdline.find("auditbeat")!= std::string::npos or process_event.entry.cmdline.find("osqueryd")!= std::string::npos))
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1518.001 - Security Software Discovery - Security Software Discovery - ps (Linux)" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1018 - Remote System Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1018/T1018.md#t1018---remote-system-discovery
bool remote_system_discovery_ipneighbour(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("ip neighbour")!= std::string::npos or process_event.entry.cmdline.find("ip route")!= std::string::npos or process_event.entry.cmdline.find("ip tcp_metrics")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1018 - Remote System Discovery - IP Neighbour" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1046 - Network Service Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1046/T1046.md#t1046---network-service-discovery
bool port_scan(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("nmap")!= std::string::npos or process_event.entry.cmdline.find("telnet")!= std::string::npos or process_event.entry.cmdline.find("nc -nv")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1046 - Network Service Discovery - Port Scan" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1486 - Data Encrypted for Impact
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1486/T1486.md#t1486---data-encrypted-for-impact
bool encrypt_file_using_gpg(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("gpg")!= std::string::npos and process_event.entry.cmdline.find("batch")!= std::string::npos and process_event.entry.cmdline.find("passphrase")!= std::string::npos and process_event.entry.cmdline.find("cipher-algo")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1486 - Data Encrypted for Impact - Encrypt using gpg" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1486 - Data Encrypted for Impact
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1486/T1486.md#t1486---data-encrypted-for-impact
bool encrypt_file_using_7z(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("7z a -p")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1486 - Data Encrypted for Impact - Encrypt using 7z" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1486 - Data Encrypted for Impact
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1486/T1486.md#t1486---data-encrypted-for-impact
bool encrypt_file_using_openssl(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("rsautl -encrypt")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1486 - Data Encrypted for Impact - Encrypt using OpenSSL" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1485 - Data Destruction
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1485/T1485.md#t1485---data-destruction
bool overwrite_file_with_dd(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("dd")!= std::string::npos and process_event.entry.cmdline.find("if")!= std::string::npos and process_event.entry.cmdline.find("of")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1485 - Data Destruction -  Overwrite file with DD" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1529 - System Shutdown/Reboot
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1529/T1529.md#description-from-attck
bool system_shutdown_reboot(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("shutdown -r")!= std::string::npos or process_event.entry.cmdline.find("shutdown -h")!= std::string::npos or process_event.entry.cmdline.find("reboot")!= std::string::npos or process_event.entry.cmdline.find("halt -p")!= std::string::npos or process_event.entry.cmdline.find("halt --reboot")!= std::string::npos or process_event.entry.cmdline.find("poweroff")!= std::string::npos or process_event.entry.cmdline.find("poweroff --reboot")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1485 - Data Destruction -  Overwrite file with DD" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}





// T1200 - Tactic: Initial Access
//
bool hardware_additions(const ProcessEvent &process_event, Event &rule_event)
{
   if (process_event.entry.cmdline.find("/usr/bin/nautilus-autorun-software") != std::string::npos)
   {
       std::stringstream ss;
 
       ss << " (pid: " << process_event.entry.pid << ")] USB Autorunning Scripts";
 
       rule_event.metadata = ss.str();
 
       return true;
   }
   return false;
}
 
// T1190 - Tactic: Initial Access
bool inject_ld_preload(const ProcessEvent &process_event, Event &rule_event)
{
   bool condition_1 = process_event.entry.cmdline.find(".so") != std::string::npos;
   bool condition_2 = process_event.entry.cmdline.find("bindtextdomain.so") == std::string::npos;
   if (condition_1 && condition_2)
   {
       std::stringstream ss;
 
       ss << " (pid: " << process_event.entry.pid << ")] LD_PRELOAD injection";
 
       rule_event.metadata = ss.str();
 
       return true;
   }
   return false;
}
// T1098.004-Account Manipulation: SSH Authorized Keys
// https://attack.mitre.org/techniques/T1098/004/
bool manipulation_ssh_authorized_key(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/ssh/sshd_config")!= std::string::npos or process_event.entry.file_path.find("/.ssh/")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1098.004-Account Manipulation: SSH Authorized Keys " ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1546.004-Event Triggered Execution: Unix Shell Configuration Modification
// https://attack.mitre.org/techniques/T1546/004/
bool shell_config_modify(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/profile")!= std::string::npos or process_event.entry.file_path.find("/etc/profile.d")!= std::string::npos or process_event.entry.file_path.find("/.bash_profile")!= std::string::npos or process_event.entry.file_path.find("/.bash_login")!= std::string::npos or process_event.entry.file_path.find("/.profile")!= std::string::npos or process_event.entry.file_path.find("/.bashrc")!= std::string::npos or process_event.entry.file_path.find("/etc/shells")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1546.004-Event Triggered Execution: Unix Shell Configuration Modification " ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1574.006-Hijack Execution Flow: Dynamic Linker Hijacking
// https://attack.mitre.org/techniques/T1574/006/
bool dynamic_linker_hijacking_file(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/ld.so.preload")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1574.006-Hijack Execution Flow: Dynamic Linker Hijacking" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1556.003-Modify Authentication Process: Pluggable Authentication Modules
// https://attack.mitre.org/techniques/T1556/003/
// bool pluggable_authentication_modules_file(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.file_path.find("/etc/pam.d/")!= std::string::npos)
//     {

//             std::stringstream ss;

//             ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1556.003-Modify Authentication Process: Pluggable Authentication Modules" ;
//             rule_event.metadata = ss.str();

//             return true;
//     }
//     return false;
// }

// T1053.002-Scheduled Task-Job: At
// https://attack.mitre.org/techniques/T1053/002/
bool scheduled_task_at_file(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/at.allow")!= std::string::npos or process_event.entry.file_path.find("var/spool/cron/atjobs/")!= std::string::npos or process_event.entry.file_path.find("/etc/at.deny")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1053.002-Scheduled Task-Job: At" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1053.003 : Scheduled Task/Job: Cron
// https://attack.mitre.org/techniques/T1053/003/
bool scheduled_task_cron_file(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/rsyslog.conf")!= std::string::npos or process_event.entry.file_path.find("/etc/syslog.conf")!= std::string::npos or process_event.entry.file_path.find("/etc/cron.allow")!= std::string::npos or process_event.entry.file_path.find("/etc/cron.deny")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1053.003 : Scheduled Task/Job: Cron" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1053.006 : Scheduled Task/Job: Systemd Timers
// https://attack.mitre.org/techniques/T1053/006/
bool scheduled_task_systemd_timers_file(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/rsyslog.conf")!= std::string::npos or process_event.entry.file_path.find("/etc/syslog.conf")!= std::string::npos or process_event.entry.file_path.find("/etc/cron.allow")!= std::string::npos or process_event.entry.file_path.find("/etc/cron.deny")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1053.003 : Scheduled Task/Job: Cron" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}








// T1030 - Data Transfer Size Limits
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1030/T1030.md#t1030---data-transfer-size-limits
bool data_transfer_size_limits(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path.find("split -b") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]T1030 - Data Transfer Size Limits";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}


// T1567.002 - Exfiltration to Cloud Storage
bool exfil_compressed_archive_to_s3_via_aws_cli(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path.find("aws s3 cp") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]T1567.002 - Exfiltration to Cloud Storage";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// T1567.002 - Exfil Compressed Archive to S3 via Golang
bool exfil_compressed_archive_to_s3_via_aws_golang(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path.find("s3upload") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]T1567.002 - Exfil Compressed Archive to S3 via Golang";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

 
// T1090.003 - Proxy: Multi-hop Proxy
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1090.003/T1090.003.md#atomic-test-3---tor-proxy-usage---debianubuntu
bool multi_hop_proxy(const ProcessEvent& process_event, Event& rule_event)
{
   if (process_event.entry.cmdline.find("start tor")!= std::string::npos and process_event.entry.cmdline.find("install tor")!= std::string::npos )
   {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1090.003 - Proxy: Multi-hop Proxy" ;
           rule_event.metadata = ss.str();
 
           return true;
   }
   return false;
}
 
// T1105 - Ingress Tool Transfer
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1105/T1105.md
bool ingress_tool_transfer(const ProcessEvent& process_event, Event& rule_event)
{
   if (process_event.entry.cmdline.find("rsync -r")!= std::string::npos or process_event.entry.cmdline.find("whois -h")!= std::string::npos )
   {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1105 - Ingress Tool Transfer" ;
           rule_event.metadata = ss.str();
 
           return true;
   }
   return false;
}
 
// T1548.001 - Abuse Elevation Control Mechanism: Setuid and Setgid
// https://attack.mitre.org/techniques/T1548/001/
bool Abuse_elevation_control_mechanism_file(const ProcessEvent& process_event, Event& rule_event)
{
   if (process_event.entry.file_path.find("/usr/bin/chmod")!= std::string::npos or process_event.entry.file_path.find("/usr/bin/chgrp")!= std::string::npos)
   {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1548.001 - Abuse Elevation Control Mechanism: Setuid and Setgid" ;
           rule_event.metadata = ss.str();
 
           return true;
   }
   return false;
}
 
// T1548.003 - Abuse Elevation Control Mechanism: Sudo and Sudo Caching
// https://attack.mitre.org/techniques/T1548/003/
bool abuse_elevation_control_mechanism_sudo_caching_file(const ProcessEvent& process_event, Event& rule_event)
{
   if (process_event.entry.file_path.find("/var/db/sudo")!= std::string::npos or process_event.entry.file_path.find("/etc/sudoers")!= std::string::npos)
   {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1548.001 - Abuse Elevation Control Mechanism: Setuid and Setgid" ;
           rule_event.metadata = ss.str();
 
           return true;
   }
   return false;
}
 
// T1543.002 - Create or Modify System Process: Systemd Service
// https://attack.mitre.org/techniques/T1543/002/
bool modify_system_process_systemd_service_file(const ProcessEvent& process_event, Event& rule_event)
{
   if (process_event.entry.file_path.find("/etc/systemd/system")!= std::string::npos or process_event.entry.file_path.find("/usr/lib/systemd/system")!= std::string::npos or process_event.entry.file_path.find("/usr/lib/systemd/system/")!= std::string::npos)
   {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1543.002 - Create or Modify System Process: Systemd Service" ;
           rule_event.metadata = ss.str();
 
           return true;
   }
   return false;
}
 
// T1055.009 - Process Injection: Proc Memory
// https://attack.mitre.org/techniques/T1055/009/
bool process_injection_proc_memory_file(const ProcessEvent& process_event, Event& rule_event)
{
   if (process_event.entry.file_path.find("/usr/include/proc")!= std::string::npos)
   {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1055.009 - Process Injection: Proc Memory" ;
           rule_event.metadata = ss.str();
 
           return true;
   }
   return false;
}
 
// T1037.004 - Boot or Logon Initialization Scripts: RC Scripts
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1037.004/T1037.004.md
bool boot_logon_initialization_scripts_rc_scripts_file(const ProcessEvent& process_event, Event& rule_event)
{
   if (process_event.entry.file_path.find("/etc/rc.common")!= std::string::npos or process_event.entry.file_path.find("/etc/rc.local")!= std::string::npos)
   {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1037.004 - Boot or Logon Initialization Scripts: RC Scripts" ;
           rule_event.metadata = ss.str();
 
           return true;
   }
   return false;
}

// T1037.004 - Boot or Logon Initialization Scripts: RC Scripts
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1037.004/T1037.004.md
bool chaos_malware_infection(const ProcessEvent& process_event, Event& rule_event)
{
   if (process_event.entry.file_path.find("/boot/System.img.config")!= std::string::npos and process_event.entry.file_path.find("/etc/init.d/linux_kill")!= std::string::npos and process_event.entry.file_path.find("/etc/id.services.conf")!= std::string::npos and process_event.entry.file_path.find("/etc/profile.d/bash_config.sh")!= std::string::npos)
   {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] Chaos Malware Infection" ;
           rule_event.metadata = ss.str();
 
           return true;
   }
   return false;
}

bool log4j_spawn_shell(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.path == "/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto& process : process_event.parents)
        {
            if (process.path.find("/usr/lib/jvm/java-8-openjdk")!= std::string::npos)
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned Log4J[" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool rule_builder_rule(const ProcessEvent& process_event, Event& rule_event)
{        
    bool response = false;
    std::unordered_map<std::string, std::string> prepared_queries;
    ConfigurationParser config_parser("config.json");
    if (!config_parser.parse())
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Cannot read configuration";
        return -1;
    }
    ApiServerConfiguration api = config_parser.GetApiServerConfiguration();

    std::shared_ptr<ConnectionPool> connection_pool(
            new ConnectionPool(api.database.ConnectionString(), api.database_pool.maximum_connections, prepared_queries));

    std::string rule_builder, tactics, alerters, platform;
    std::string name, technique_id, type, link, description, rule_written_by;
    nlohmann::json rule_json;
    int id, severity, created, last_modified;
    auto connection = connection_pool->GetConnection();
    connection->prepare("rule_builder_rule", "select * from rule_builder_rules");
    pqxx::work transaction{*connection};
    pqxx::result result = transaction.exec_prepared("rule_builder_rule");
    transaction.commit();
    connection->unprepare("rule_builder_rule");
    int no_affected_rows = result.affected_rows();
    if (no_affected_rows > 0){
        for (int i=0; i<no_affected_rows; i++)
        {
        for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
        {
                id = c["id"].as<long>();
                name = c["name"].as<std::string>();
                description = c["description"].as<std::string>();
                link = c["link"].as<std::string>();
                platform = c["platform"].as<std::string>();
                severity = c["severity"].as<long>();
                nlohmann::json j = nlohmann::json::parse(c["tactics"].as<std::string>());
                tactics = j[0]["value"];
                technique_id = c["technique_id"].as<std::string>();
                type = c["type"].as<std::string>();
                rule_written_by = c["rule_written_by"].as<std::string>();
                alerters = c["alerters"].as<std::string>();
                rule_builder = c["rule_builder"].as<std::string>();
                created = c["created"].as<long>();
                last_modified = c["last_modified"].as<long>();
                std::string _operator, table, match_case;
                rule_json = nlohmann::json::parse(rule_builder);
                int no_of_conditions = rule_json["queries"].size();
                int no_of_tables = 1;
                for (int n = 1; n < no_of_conditions; n++) {
                    for(int p = 0; p < n; p++){
                        if(rule_json["queries"][n]["tableName"]==rule_json["queries"][p]["tableName"]){
                            break;
                        }
                        no_of_tables++;
                    }
                }
                // else{
                std::string operator_rule = rule_json["operator"];
                std::string table_name = rule_json["queries"][0]["tableName"];
                std::string column_name = rule_json["queries"][0]["columnName"];
                
                std::string conditionType = rule_json["queries"][0]["conditionType"];
                std::string value = rule_json["queries"][0]["value"];
                std::string query;
                
                std::time_t t = std::time(0);
                int created = t;
                int new_time = t - 21600;
                if(no_of_conditions == 1){
                    std::string next_part = ")and "+table_name+".unixtime < "+std::to_string(new_time)+ " and is_processed_rule_builder = 'f'";
                    // change < to >
                    query = "select * from "+table_name+" where(";
                    for (int i = 0; i < no_of_conditions; i++) {
                    std::string table_name = rule_json["queries"][i]["tableName"];
                    // std::string table_name_2 = rule_json["queries"][i+1]["tableName"];
                    std::string column_name = rule_json["queries"][i]["columnName"];
                    // std::string column_name_2 = rule_json["queries"][i+1]["columnName"];
                    std::string conditionType = rule_json["queries"][i]["conditionType"];
                    std::string data_type = rule_json["queries"][i]["dataType"];
                    std::string value = rule_json["queries"][i]["value"];
                    if(data_type == "string" and conditionType == "like"){
                    query+= table_name+"."+column_name+" like '%"+value+ "%' ";
                    }
                    else if(data_type=="string" and conditionType=="equal"){                    
                        query+= table_name+"."+column_name+" = '"+value+ "' ";
                    }
                    else if(data_type=="int" and conditionType=="equal"){                    
                        query+= table_name+"."+column_name+" = "+value;
                    }
                    if(i<no_of_conditions-1){
                    query+=operator_rule+" ";
                    }
                    // + " "+table_name+"."+column_name+" like '%"+value+"%' "+operator_rule+ " "+table_name+"."+column_name+" like '%"+value+"%'";
                    }
                    query+=next_part;
                }

                else{

                    std::string table_name_2 = rule_json["queries"][1]["tableName"];
                    std::string column_name_2 = rule_json["queries"][1]["columnName"];
                    std::string conditionType_2 = rule_json["queries"][1]["conditionType"];
                    std::string value_2 = rule_json["queries"][1]["value"];
                    std::string next_part = ")and "+table_name+".unixtime > "+std::to_string(new_time)+ " and "+table_name+".is_processed_rule_builder = 'f'";
                    // change < to >


                    if(no_of_tables==1){
                        query = "select * from "+table_name+" where (";
                    }
                    else{
                    query = "select * from "+table_name+" join "+table_name_2+" on "+table_name+"."+column_name_2+"="+table_name_2+"."+column_name_2+" where(";
                    }
                    
                    for (int i = 0; i < no_of_conditions; i++) {
                    std::string table_name = rule_json["queries"][i]["tableName"];
                    // std::string table_name_2 = rule_json["queries"][i+1]["tableName"];
                    std::string column_name = rule_json["queries"][i]["columnName"];
                    // std::string column_name_2 = rule_json["queries"][i+1]["columnName"];
                    std::string conditionType = rule_json["queries"][i]["conditionType"];
                    std::string value = rule_json["queries"][i]["value"];
                    std::string data_type = rule_json["queries"][i]["dataType"];
                    

                    // for (int m = 0; m < i; m++) {
                    // if(table_name == rule_json["queries"][m]["tableName"]){
                    //     query+= table_name+"."+column_name+" like '%"+value+ "%' ";
                    // }
                    // }

                    if(data_type == "string" and conditionType == "like"){
                    query+= table_name+"."+column_name+" like '%"+value+ "%' ";
                    }
                    else if(data_type=="string" and conditionType=="equal"){                    
                        query+= table_name+"."+column_name+" = '"+value+ "' ";
                    }
                    else if(data_type=="int" and conditionType=="equal"){                    
                        query+= table_name+"."+column_name+" = "+value;
                    }


                    // query+= table_name+"."+column_name+" like '%"+value+ "%' ";
                    if(i<no_of_conditions-1){
                        query+=operator_rule+" ";
                    }
                    // + " "+table_name+"."+column_name+" like '%"+value+"%' "+operator_rule+ " "+table_name+"."+column_name+" like '%"+value+"%'";
                    }
                    query+=next_part;
                }

                connection->prepare("rule_builder_query", query);
                pqxx::result result_count = transaction.exec_prepared("rule_builder_query");
                int no_detection = result_count.affected_rows();
                connection->unprepare("rule_builder_query");
                if(no_detection > 0){
                    for (int k = 0; k < no_detection; k++){
                    long unixtime = result_count.at(k)["unixTime"].as<long>();
                    long start_time = unixtime - 60;
                    long end_time = unixtime + 60;
                    connection->prepare("insert_events", "insert into events (host_identifier, hostname, context_data, unixtime,event_code,metadata,is_alert,severity,is_open, alert_type, technique, technique_id, link, pid) values ($1, $2, $3, $4, $5, $6, $7, $8, true, 'Process Alert', $9, $10, $11, $12)");

                    // transaction.exec_prepared("insert_events", result_count.at(0)["host_identifier"], result_count.at(0)["hostname"], result_count.at(0)["context_list"], result_count.at(0)["unixtime"], name, "event_metadata.dump()", true, severity, tactics, technique_id, link, result_count.at(0)["event_pid"]);
                    pqxx::result insert_events = transaction.exec_prepared("insert_events", result_count.at(k)["host_identifier"].as<std::string>(), result_count.at(k)["hostname"].as<std::string>(), get_context_data(result_count.at(k)["host_identifier"].as<std::string>(), result_count.at(k)["pid"].as<long>(), start_time, end_time), result_count.at(k)["unixtime"].as<std::string>(), name, get_process_tree(result_count.at(k)["pid"].as<long>(),table_name), true, severity, tactics, technique_id, link, result_count.at(k)["pid"].as<long>());
                    int aff = insert_events.affected_rows();
                    // transaction.commit();
                    connection->unprepare("insert_events");
                    std::time_t t = std::time(0);
                    int created = t;
                    std::string query = "update "+table_name+" set is_processed_rule_builder = 't' where unixTime < "+std::to_string(created);
                    connection->prepare("update_is_processed", query);
                    pqxx::result update_count = transaction.exec_prepared("update_is_processed");
                    connection->unprepare("update_is_processed");
                    int no_affected_rows = update_count.affected_rows();
                }
                response = true;
                }
        // }
        }
    }
    //here
    }
   return response;
}

// bool rule_builder(const ProcessEvent& process_event, Event& rule_event)  
// {
//     bool response = false;
//     std::unordered_map<std::string, std::string> prepared_queries;
//     ConfigurationParser config_parser("config.json");
//     if (!config_parser.parse())
//     {
//         BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Cannot read configuration";
//         return -1;
//     }
//     ApiServerConfiguration api = config_parser.GetApiServerConfiguration();
//     std::shared_ptr<ConnectionPool> connection_pool(
//             new ConnectionPool(api.database.ConnectionString(), api.database_pool.maximum_connections, prepared_queries));
//     std::string rule_builder, tactics, alerters, platform;
//     std::string name, technique_id, type, link, description, rule_written_by;
//     nlohmann::json rule_json;
//     int id, severity, created, last_modified;
//     auto connection = connection_pool->GetConnection();
//     connection->prepare("rule_builder_rule", "select * from rule_builder_rules");
//     pqxx::work transaction{*connection};
//     pqxx::result result = transaction.exec_prepared("rule_builder_rule");
//     transaction.commit();
//     connection->unprepare("rule_builder_rule");
//     int no_affected_rows = result.affected_rows();
//     if (no_affected_rows > 0){
//         for (int i=0; i<no_affected_rows; i++)
//         {
//         for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
//         {
//                 id = c["id"].as<long>();
//                 name = c["name"].as<std::string>();
//                 description = c["description"].as<std::string>();
//                 link = c["link"].as<std::string>();
//                 platform = c["platform"].as<std::string>();
//                 severity = c["severity"].as<long>();
//                 nlohmann::json j = nlohmann::json::parse(c["tactics"].as<std::string>());
//                 tactics = j[0]["value"];
//                 technique_id = c["technique_id"].as<std::string>();
//                 type = c["type"].as<std::string>();
//                 rule_written_by = c["rule_written_by"].as<std::string>();
//                 alerters = c["alerters"].as<std::string>();
//                 rule_builder = c["rule_builder"].as<std::string>();
//                 created = c["created"].as<long>();
//                 last_modified = c["last_modified"].as<long>();
//                 std::string _operator, table, match_case;
//                 rule_json = nlohmann::json::parse(rule_builder);
//                 int no_of_conditions = rule_json["queries"].size();
//                 int no_of_tables = 1;
//                 for (int n = 1; n < no_of_conditions; n++) {
//                     for(int p = 0; p < n; p++){
//                         if(rule_json["queries"][n]["tableName"]==rule_json["queries"][p]["tableName"]){
//                             break;
//                         }
//                         no_of_tables++;
//                     }
//                 }
//                 std::vector <ProcessEntry> rules[no_of_conditions];
//                 nlohmann::json obj;
//                 ProcessEntry entry;
//                 for (int i = 0; i < no_of_conditions; i++) {
//                     std::string table_name = rule_json["queries"][i]["tableName"];
//                     // std::string table_name_2 = rule_json["queries"][i+1]["tableName"];
//                     std::string column_name = rule_json["queries"][i]["columnName"];
//                     // std::string column_name_2 = rule_json["queries"][i+1]["columnName"];
//                     std::string conditionType = rule_json["queries"][i]["conditionType"];
//                     std::string data_type = rule_json["queries"][i]["dataType"];
//                     std::string value = rule_json["queries"][i]["value"];
//                     entry.syscall = 123;
//                     rules[i].push_back(entry);
//                 }
//
//
//                 /////////////////////////////////
//                         if (process_event.entry.path == "/usr/sbin/visudo")
//                         {
//                             for (auto &process : process_event.parents)
//                             {
//                                 if (process.path == "/usr/bin/sudo")
//                                 {
//                                     if (process.cmdline.find("sudo visudo") != std::string::npos)
//                                     {
//                                         std::stringstream ss;
//
//                                         ss << "[" << process.path << " (pid: " << process.pid << ")] caching the suoders file to elevate privileges";
//
//                                         rule_event.metadata = ss.str();
//
//                                         return true;
//                                     }
//                                 }
//                             }
//                         }
//                 //////////////////////////////////
//                 //here
//             }
//         }
//     }
//    return false;
// }

std::string get_context_data(std::string host_identifier, long pid, long start_time, long end_time){
        nlohmann::json context_list = nlohmann::json::array();
        std::unordered_map<std::string, std::string> prepared_queries;
        ConfigurationParser config_parser("config.json");
        if (!config_parser.parse())
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Cannot read configuration";
            return NULL;
        }
        ApiServerConfiguration api = config_parser.GetApiServerConfiguration();

        std::shared_ptr<ConnectionPool> connection_pool(
            new ConnectionPool(api.database.ConnectionString(), api.database_pool.maximum_connections, prepared_queries));

        auto connection = connection_pool->GetConnection();

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
        std::string context_data = context_list.dump();
        return context_data;
}

std::string get_process_tree(long pid, std::string table_name){
        std::unordered_map<std::string, std::string> prepared_queries;
        ConfigurationParser config_parser("config.json");
        if (!config_parser.parse())
        {
            BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Cannot read configuration";
            return NULL;
        }
        ApiServerConfiguration api = config_parser.GetApiServerConfiguration();

        std::shared_ptr<ConnectionPool> connection_pool(
            new ConnectionPool(api.database.ConnectionString(), api.database_pool.maximum_connections, prepared_queries));

        auto connection = connection_pool->GetConnection();
        std::string ptree_query = "select * from "+table_name+ " where pid = "+std::to_string(pid);
        connection->prepare("get_process_tree", ptree_query);
        pqxx::work transaction{*connection};
        // pqxx::work transactioninsert{*connection};
        pqxx::result result = transaction.exec_prepared("get_process_tree");
        long record_id = result.at(0)["id"].as<long>();
        std::string process_tree = result.at(0)["process_tree"].as<std::string>();

        auto process_json = nlohmann::json::parse(process_tree);
        auto parents = JsonToProcessEntryList(process_json);
        
        ProcessEvent process_event;
        process_event.parents = parents;
        process_event.entry.pid = result.at(0)["pid"].as<long>();
        
        process_event.entry.parent = result.at(0)["parent_pid"].as<long>();
        process_event.entry.unixtime = result.at(0)["unixTime"].as<long long>();
        process_event.entry.action = result.at(0)["action"].as<std::string>();
        process_event.entry.process_name = result.at(0)["process_name"].as<std::string>();
        process_event.entry.target_path = result.at(0)["target_path"].as<std::string>();
        process_event.entry.md5 = result.at(0)["md5"].as<std::string>();
        process_event.entry.sha256 = result.at(0)["sha256"].as<std::string>();
        process_event.entry.hashed = result.at(0)["hashed"].as<int>();
        process_event.entry.md5 = result.at(0)["md5"].as<std::string>();
        process_event.entry.uptime = result.at(0)["uptime"].as<long long>();

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
            obj["action"] = parent.action;
            obj["path"] = parent.path;
            obj["parent_pid"] = parent.parent;
            obj["parent_path"] = parent.parent_path;
            obj["eventid"] = parent.eventid;
            obj["sha256"] = parent.sha256;

            parents_list.push_back(obj);
        }

        event_metadata["parents"] = parents_list;
        // event_metadata["extra_metadata"] = _event.metadata;
        event_metadata["pid"] = process_event.entry.pid;
        event_metadata["path"] = process_event.entry.path;
        event_metadata["unixTime"] = process_event.entry.unixtime;
        event_metadata["parent_pid"] = process_event.entry.parent;
        event_metadata["action"] = process_event.entry.action;
        event_metadata["process_name"] = process_event.entry.process_name;
        event_metadata["target_path"] = process_event.entry.target_path;
        event_metadata["md5"] = process_event.entry.effective_group_id;
        event_metadata["sha256"] = process_event.entry.effective_user_id;
        event_metadata["hashed"] = process_event.entry.owner_user_id;


        std::string response = event_metadata.dump();
        return response;
}