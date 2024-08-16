#include "gtfobins_rules.h"
#include <sstream>

bool wget_download_tmp(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/wget")
    {
        /*
         * THere are two ways to download in /tmp using wget
         * 1. by specifying -O /tmp/XXXX in command line
         * 2. by running wget somewhere inside /tmp, and no -O parameter is specified
         */

        if ((process_event.entry.cmdline.find("-O /tmp/") != std::string::npos) || (process_event.entry.cwd.rfind("/tmp/", 0) == 0) || (process_event.entry.cwd.rfind("\"/tmp/", 0) == 0) || (process_event.entry.cwd == "/tmp") || (process_event.entry.cwd == "\"/tmp\""))
        {
            std::stringstream ss;

            ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] downloading file in /tmp/";

            rule_event.metadata = ss.str();

            return true;
        }
    }
    return false;
}

bool apt_get_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/zsh" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/apt-get")
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool apt_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/apt")
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ash_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/sudo" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/ash" && process.syscall == "exec") && (process.cmdline.find("ash") || process.cmdline.find("sudo ash")))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool awk_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/awk" && process.syscall == "exec") && (process.cmdline.find("awk")))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool bundler_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/man" && process.syscall == "exec") && process.cmdline.find("/ruby"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool busctl_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/busctl" && process.syscall == "exec")
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool byebug_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/byebug")
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool cpan_spawn_shell(const ProcessEvent &process_event, Event &rule_event) // not working
{
    if ((process_event.entry.path == "/usr/bin/dash") || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/cpan")
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool cpulimit_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.cmdline.find("cpulimit -l 100 -f /bin/sh") && process.syscall == ("exec") && process.path == ("/usr/bin/cpulimit"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool dmesg_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/dmesg" && process.syscall == "exec" && process.cmdline.find("dmesg"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}
// Cannot capture
bool docker_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/snap/docker/471/bin" || process.path == "/usr/bin/docker")
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

// Without running !/bin/sh it captures
bool dpkg_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {

            if ((process.cmdline.find("dpkg") && (process.path == "/usr/bin/dpkg" || process.path == "/usr/bin/dpkg-query") && process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}
// Not entering the loop
bool env_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/env")
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool find_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/find")
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool flock_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" or process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if (process.path == "/usr/bin/flock")
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ionice_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/ionice") && (process.cmdline == "sudo ionice /bin/sh" || process.cmdline == "ionice /bin/sh") && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool journalctl_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/journalctl") && (process.cmdline == "journalctl" || process.cmdline == "sudo journalctl") && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ksh_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/ksh") && (process.cmdline == "ksh" || process.cmdline == "sudo ksh") && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool less_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/less") && (process.cmdline.find("less") || process.cmdline.find("sudo less")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool logsave_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/sbin/logsave" || process.path == "/usr/bin/logsave") && (process.cmdline.find("logsave") || process.cmdline.find("sudo logsave")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ltrace_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/ltrace") && (process.cmdline.find("ltrace") || process.cmdline.find("sudo ltrace")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool lua_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/lua") && (process.cmdline.find("lua") || process.cmdline.find("sudo lua")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool man_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/man") && (process.cmdline.find("man") || process.cmdline.find("sudo man")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool mawk_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/mawk") && (process.cmdline.find("mawk") || process.cmdline.find("sudo mawk")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool mysql_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/mysql") && (process.cmdline.find("mysql") || process.cmdline.find("sudo mysql")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool nano_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/nano") && (process.cmdline.find("nano") || process.cmdline.find("sudo nano")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool nice_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/nice") && (process.cmdline.find("nice") || process.cmdline.find("sudo nice")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool nmap_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/nmap") && (process.cmdline.find("nmap") || process.cmdline.find("sudo nmap")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool nohup_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/nohup") && (process.cmdline.find("nohup") || process.cmdline.find("sudo nohup")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool nroff_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/locale" || process.path == "/usr/bin/nroff") && (process.cmdline.find("nroff") || process.cmdline.find("sudo nroff")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool nsenter_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/nsenter") && (process.cmdline.find("nsenter") || process.cmdline.find("sudo nsenter")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool perl_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/perl") && (process.cmdline.find("perl") || process.cmdline.find("sudo perl")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

// --------------------------------------------------------------------------------------------------
// anupam part
bool ftp_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/netkit-ftp" || process.path == "/usr/bin/ftp") && (process.cmdline.find("ftp") || process.cmdline.find("sudo ftp")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool git_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/git") && (process.cmdline.find("git") || process.cmdline.find("sudo git")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool gawk_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/gawk") && (process.cmdline.find("gawk") || process.cmdline.find("sudo gawk")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool gcc_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/x86_64-linux-gnu-gcc-9" || process.path == "/usr/bin/gcc") && (process.cmdline.find("gcc") || process.cmdline.find("sudo gcc")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool gdb_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/gdb") && (process.cmdline.find("gdb") || process.cmdline.find("sudo gdb")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool tar_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/tar") && (process.cmdline.find("tar") || process.cmdline.find("sudo tar")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool taskset_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/taskset") && (process.cmdline.find("taskset") || process.cmdline.find("sudo taskset")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool time_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/time") && (process.cmdline.find("time") || process.cmdline.find("sudo time")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool timeout_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/timeout") && (process.cmdline.find("timeout") || process.cmdline.find("sudo timeout")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool tmux_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/tmux") && (process.cmdline.find("tmux") || process.cmdline.find("sudo tmux")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool watch_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/watch") && (process.cmdline.find("watch") || process.cmdline.find("sudo watch")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool vi_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/vim.basic" || process.path == "/usr/bin/vi") && (process.cmdline.find("vi") || process.cmdline.find("sudo vi")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool vim_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/vim.basic" || process.path == "/usr/bin/vim") && (process.cmdline.find("vim") || process.cmdline.find("sudo vim")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool view_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/vim.basic" || process.path == "/usr/bin/view") && (process.cmdline.find("view") || process.cmdline.find("sudo view")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool xargs_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/xargs") && (process.cmdline.find("xargs") || process.cmdline.find("sudo xargs")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool zip_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/zip") && (process.cmdline.find("zip") || process.cmdline.find("sudo zip")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool strace_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/strace") && (process.cmdline.find("strace") || process.cmdline.find("sudo strace")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool stdbuf_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/stdbuf") && (process.cmdline.find("stdbuf") || process.cmdline.find("sudo stdbuf")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool startstopdaemon_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/sbin/start-stop-daemon") && (process.cmdline.find("start-stop-daemon") || process.cmdline.find("sudo start-stop-daemon")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ssh_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/ssh") && (process.cmdline.find("ssh") || process.cmdline.find("sudo ssh")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool split_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/split") && (process.cmdline.find("split") || process.cmdline.find("sudo split")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool sqlite3_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/sqlite3") && (process.cmdline.find("sqlite3") || process.cmdline.find("sudo sqlite3")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool setarch_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/setarch") && (process.cmdline.find("setarch") || process.cmdline.find("sudo setarch")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool sed_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/sed") && (process.cmdline.find("sed") || process.cmdline.find("sudo sed")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool slsh_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/slsh") && (process.cmdline.find("slsh") || process.cmdline.find("sudo slsh")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}
bool socat_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/socat") && (process.cmdline.find("socat") || process.cmdline.find("sudo socat")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool script_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/script") && (process.cmdline.find("script") || process.cmdline.find("sudo script")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}
bool screen_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/screen") && (process.cmdline.find("screen") || process.cmdline.find("sudo screen")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool pry_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/pry") && (process.cmdline.find("pry") || process.cmdline.find("sudo pry")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool runmailcap_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/perl") && (process.cmdline.find("run-mailcap") || process.cmdline.find("sudo run-mailcap")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool puppet_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/puppet") && (process.cmdline.find("puppet") || process.cmdline.find("sudo puppet")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool rake_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/rake") && (process.cmdline.find("rake") || process.cmdline.find("sudo rake")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool rpm_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/rpm") && (process.cmdline.find("rpm") || process.cmdline.find("sudo rpm")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool rlwrap_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/rlwrap") && (process.cmdline.find("rlwrap") || process.cmdline.find("sudo rlwrap")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ruby_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/ruby") && (process.cmdline.find("ruby") || process.cmdline.find("sudo ruby")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool rsync_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/rsync") && (process.cmdline.find("rsync") || process.cmdline.find("sudo rsync")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool pic_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/pic") && (process.cmdline.find("pic") || process.cmdline.find("sudo pic")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}
bool scp_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/scp") && (process.cmdline.find("scp") || process.cmdline.find("sudo scp")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool php_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/php") && (process.cmdline.find("php") || process.cmdline.find("sudo php")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool unshare_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/unshare") && (process.cmdline.find("unshare") || process.cmdline.find("sudo unshare")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

// Arjun

bool at_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/at") && (process.cmdline.find("at") || process.cmdline.find("sudo at")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool bash_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2) || process_event.entry.path == "/usr/bin/sudo")
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/bash" || process.path == "/usr/bin/sudo") && (process.cmdline.find("bash") || process.cmdline.find("sudo bash")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool capsh_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/sbin/capsh") && (process.cmdline.find("capsh") || process.cmdline.find("sudo capsh")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool cowsay_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/games/cowsay") && (process.cmdline.find("cowsay") || process.cmdline.find("sudo cowsay")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool crash_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/crash") && (process.cmdline.find("crash") || process.cmdline.find("sudo crash")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool csh_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/csh") && (process.cmdline.find("csh") || process.cmdline.find("sudo csh")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool dash_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/dash") && (process.cmdline.find("dash") || process.cmdline.find("sudo dash")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ed_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/ed") && (process.cmdline.find("ed") || process.cmdline.find("sudo ed")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool emacs_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/snap/emacs/966/usr/bin/emacs") && (process.cmdline.find("emacs") || process.cmdline.find("sudo emacs")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ex_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/vim.tiny" || process.path == "/usr/bin/ex") && (process.cmdline.find("ex") || process.cmdline.find("sudo ex")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool expect_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/snap/expect/3/usr/bin/expect" || process.path == "/usr/bin/expect") && (process.cmdline.find("expect") || process.cmdline.find("sudo expect")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool facter_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/facter") && (process.cmdline.find("facter") || process.cmdline.find("sudo facter")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ghc_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/lib/ghc/bin/ghc" || process.path == "/usr/bin/ghc") && (process.cmdline.find("ghc") || process.cmdline.find("sudo ghc")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool ghci_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.cmdline == "/bin/sh/usr/bin/ghci" || process.path == "/usr/bin/ghci") && (process.cmdline.find("ghci") || process.cmdline.find("sudo ghci")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool hping_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/sbin/hping3" || process.path == "/usr/bin/hping") && (process.cmdline.find("hping") || process.cmdline.find("sudo hping")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool gtester_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/gtester") && (process.cmdline.find("gtester") || process.cmdline.find("sudo gtester")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool gimp_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/snap/gimp/322/usr/bin/python2.7" || process.path == "/usr/bin/gimp") && (process.cmdline.find("gimp") || process.cmdline.find("sudo gimp")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool irb_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/ruby" || process.path == "/usr/bin/irb" || process.path == "/usr/bin/sudo") && (process.cmdline.find("irb") || process.cmdline.find("sudo irb")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool jjs_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/jjs") && (process.cmdline.find("jjs") || process.cmdline.find("sudo jjs")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool mail_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/mail.mailutils" || process.path == "/usr/bin/mail") && (process.cmdline.find("mail") || process.cmdline.find("sudo mail")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool make_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/make") && (process.cmdline.find("make") || process.cmdline.find("sudo make")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool more_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/more") && (process.cmdline.find("more") || process.cmdline.find("sudo more")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool nawk_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/nawk") && (process.cmdline.find("nawk") || process.cmdline.find("sudo nawk")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool node_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/node") && (process.cmdline.find("node") || process.cmdline.find("sudo node")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

// bool pico_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
// {
//     if (process_event.entry.path == "/usr/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
//     {
//         for (auto &process : process_event.parents)
//         {
//             if ((process.path == "/usr/bin/nano" || process.path == "/usr/bin/pico") && (process.cmdline.find("pico") || process.cmdline.find("sudo pico")) && (process.syscall == "exec"))
//             {
//                 std::stringstream ss;

//                 ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
//                    << " (pid: " << process_event.entry.pid << ")]";

//                 rule_event.metadata = ss.str();

//                 return true;
//             }
//         }
//     }
//     return false;
// }

bool zypper_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path.find("zypper-x"))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/zypper") && (process.cmdline.find("zypper") || process.cmdline.find("sudo zypper")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool zsh_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/zsh") && (process.cmdline.find("zsh") || process.cmdline.find("sudo zsh")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool valgrind_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path.find("/usr/libexec/valgrind/memcheck"))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/valgrind.bin" || process.path == "/usr/bin/valgrind") && (process.cmdline.find("valgrind") || process.cmdline.find("sudo valgrind")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool top_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/bash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/top") && (process.cmdline.find("top") || process.cmdline.find("sudo top")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool tclsh_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/tclsh") && (process.cmdline.find("tclsh") || process.cmdline.find("sudo tclsh")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool service_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/basename" || process.path == "/usr/sbin/service") && (process.cmdline.find("basename") || process.cmdline.find("sudo service") || process.cmdline.find("/usr/sbin/service") || process.cmdline.find("service")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool rview_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/vim.tiny" || process.path == "/usr/bin/rview") && (process.cmdline.find("rview") || process.cmdline.find("sudo rview")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool run_parts_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/run-parts") && (process.cmdline.find("run-parts") || process.cmdline.find("sudo run-parts")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool python_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/python") && (process.cmdline.find("python") || process.cmdline.find("sudo python")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}

bool psql_spawn_shell(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/bin/dash" || (process_event.entry.path.rfind("sh", process_event.entry.path.size() - 2) == process_event.entry.path.size() - 2))
    {
        for (auto &process : process_event.parents)
        {
            if ((process.path == "/usr/bin/postgresql" || process.path == "/usr/bin/psql") && (process.cmdline.find("psql") || process.cmdline.find("sudo psql")) && (process.syscall == "exec"))
            {
                std::stringstream ss;

                ss << "[" << process.path << " (pid: " << process.pid << ")] spawned [" << process_event.entry.path
                   << " (pid: " << process_event.entry.pid << ")]";

                rule_event.metadata = ss.str();

                return true;
            }
        }
    }
    return false;
}