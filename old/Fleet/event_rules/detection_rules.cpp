#include "detection_rules.h"
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

        if ((process_event.entry.command_line.find("-O /tmp/") != std::string::npos) || (process_event.entry.current_working_directory.rfind("/tmp/", 0) == 0) || (process_event.entry.current_working_directory.rfind("\"/tmp/", 0) == 0) || (process_event.entry.current_working_directory == "/tmp") || (process_event.entry.current_working_directory == "\"/tmp\""))
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
            if ((process.path == "/usr/bin/ash" && process.syscall == "exec") && (process.command_line.find("ash") || process.command_line.find("sudo ash")))
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
            if ((process.path == "/usr/bin/awk" && process.syscall == "exec") && (process.command_line.find("awk")))
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
            if ((process.path == "/usr/bin/man" && process.syscall == "exec") && process.command_line.find("/ruby"))
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
            if (process.command_line.find("cpulimit -l 100 -f /bin/sh") && process.syscall == ("exec") && process.path == ("/usr/bin/cpulimit"))
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
            if (process.path == "/usr/bin/dmesg" && process.syscall == "exec" && process.command_line.find("dmesg"))
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

            if ((process.command_line.find("dpkg") && (process.path == "/usr/bin/dpkg" || process.path == "/usr/bin/dpkg-query") && process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/ionice") && (process.command_line == "sudo ionice /bin/sh" || process.command_line == "ionice /bin/sh") && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/journalctl") && (process.command_line == "journalctl" || process.command_line == "sudo journalctl") && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/ksh") && (process.command_line == "ksh" || process.command_line == "sudo ksh") && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/less") && (process.command_line.find("less") || process.command_line.find("sudo less")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/sbin/logsave" || process.path == "/usr/bin/logsave") && (process.command_line.find("logsave") || process.command_line.find("sudo logsave")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/ltrace") && (process.command_line.find("ltrace") || process.command_line.find("sudo ltrace")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/lua") && (process.command_line.find("lua") || process.command_line.find("sudo lua")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/man") && (process.command_line.find("man") || process.command_line.find("sudo man")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/mawk") && (process.command_line.find("mawk") || process.command_line.find("sudo mawk")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/mysql") && (process.command_line.find("mysql") || process.command_line.find("sudo mysql")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/nano") && (process.command_line.find("nano") || process.command_line.find("sudo nano")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/nice") && (process.command_line.find("nice") || process.command_line.find("sudo nice")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/nmap") && (process.command_line.find("nmap") || process.command_line.find("sudo nmap")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/nohup") && (process.command_line.find("nohup") || process.command_line.find("sudo nohup")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/locale" || process.path == "/usr/bin/nroff") && (process.command_line.find("nroff") || process.command_line.find("sudo nroff")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/nsenter") && (process.command_line.find("nsenter") || process.command_line.find("sudo nsenter")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/perl") && (process.command_line.find("perl") || process.command_line.find("sudo perl")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/netkit-ftp" || process.path == "/usr/bin/ftp") && (process.command_line.find("ftp") || process.command_line.find("sudo ftp")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/git") && (process.command_line.find("git") || process.command_line.find("sudo git")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/gawk") && (process.command_line.find("gawk") || process.command_line.find("sudo gawk")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/x86_64-linux-gnu-gcc-9" || process.path == "/usr/bin/gcc") && (process.command_line.find("gcc") || process.command_line.find("sudo gcc")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/gdb") && (process.command_line.find("gdb") || process.command_line.find("sudo gdb")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/tar") && (process.command_line.find("tar") || process.command_line.find("sudo tar")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/taskset") && (process.command_line.find("taskset") || process.command_line.find("sudo taskset")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/time") && (process.command_line.find("time") || process.command_line.find("sudo time")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/timeout") && (process.command_line.find("timeout") || process.command_line.find("sudo timeout")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/tmux") && (process.command_line.find("tmux") || process.command_line.find("sudo tmux")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/watch") && (process.command_line.find("watch") || process.command_line.find("sudo watch")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/vim.basic" || process.path == "/usr/bin/vi") && (process.command_line.find("vi") || process.command_line.find("sudo vi")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/vim.basic" || process.path == "/usr/bin/vim") && (process.command_line.find("vim") || process.command_line.find("sudo vim")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/vim.basic" || process.path == "/usr/bin/view") && (process.command_line.find("view") || process.command_line.find("sudo view")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/xargs") && (process.command_line.find("xargs") || process.command_line.find("sudo xargs")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/zip") && (process.command_line.find("zip") || process.command_line.find("sudo zip")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/strace") && (process.command_line.find("strace") || process.command_line.find("sudo strace")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/stdbuf") && (process.command_line.find("stdbuf") || process.command_line.find("sudo stdbuf")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/sbin/start-stop-daemon") && (process.command_line.find("start-stop-daemon") || process.command_line.find("sudo start-stop-daemon")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/ssh") && (process.command_line.find("ssh") || process.command_line.find("sudo ssh")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/split") && (process.command_line.find("split") || process.command_line.find("sudo split")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/sqlite3") && (process.command_line.find("sqlite3") || process.command_line.find("sudo sqlite3")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/setarch") && (process.command_line.find("setarch") || process.command_line.find("sudo setarch")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/sed") && (process.command_line.find("sed") || process.command_line.find("sudo sed")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/slsh") && (process.command_line.find("slsh") || process.command_line.find("sudo slsh")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/socat") && (process.command_line.find("socat") || process.command_line.find("sudo socat")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/script") && (process.command_line.find("script") || process.command_line.find("sudo script")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/screen") && (process.command_line.find("screen") || process.command_line.find("sudo screen")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/pry") && (process.command_line.find("pry") || process.command_line.find("sudo pry")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/perl") && (process.command_line.find("run-mailcap") || process.command_line.find("sudo run-mailcap")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/puppet") && (process.command_line.find("puppet") || process.command_line.find("sudo puppet")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/rake") && (process.command_line.find("rake") || process.command_line.find("sudo rake")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/rpm") && (process.command_line.find("rpm") || process.command_line.find("sudo rpm")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/rlwrap") && (process.command_line.find("rlwrap") || process.command_line.find("sudo rlwrap")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/ruby") && (process.command_line.find("ruby") || process.command_line.find("sudo ruby")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/rsync") && (process.command_line.find("rsync") || process.command_line.find("sudo rsync")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/pic") && (process.command_line.find("pic") || process.command_line.find("sudo pic")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/scp") && (process.command_line.find("scp") || process.command_line.find("sudo scp")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/php") && (process.command_line.find("php") || process.command_line.find("sudo php")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/unshare") && (process.command_line.find("unshare") || process.command_line.find("sudo unshare")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/at") && (process.command_line.find("at") || process.command_line.find("sudo at")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/bash" || process.path == "/usr/bin/sudo") && (process.command_line.find("bash") || process.command_line.find("sudo bash")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/sbin/capsh") && (process.command_line.find("capsh") || process.command_line.find("sudo capsh")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/games/cowsay") && (process.command_line.find("cowsay") || process.command_line.find("sudo cowsay")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/crash") && (process.command_line.find("crash") || process.command_line.find("sudo crash")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/csh") && (process.command_line.find("csh") || process.command_line.find("sudo csh")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/dash") && (process.command_line.find("dash") || process.command_line.find("sudo dash")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/ed") && (process.command_line.find("ed") || process.command_line.find("sudo ed")) && (process.syscall == "exec"))
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
            if ((process.path == "/snap/emacs/966/usr/bin/emacs") && (process.command_line.find("emacs") || process.command_line.find("sudo emacs")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/vim.tiny" || process.path == "/usr/bin/ex") && (process.command_line.find("ex") || process.command_line.find("sudo ex")) && (process.syscall == "exec"))
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
            if ((process.path == "/snap/expect/3/usr/bin/expect" || process.path == "/usr/bin/expect") && (process.command_line.find("expect") || process.command_line.find("sudo expect")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/facter") && (process.command_line.find("facter") || process.command_line.find("sudo facter")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/lib/ghc/bin/ghc" || process.path == "/usr/bin/ghc") && (process.command_line.find("ghc") || process.command_line.find("sudo ghc")) && (process.syscall == "exec"))
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
            if ((process.command_line == "/bin/sh/usr/bin/ghci" || process.path == "/usr/bin/ghci") && (process.command_line.find("ghci") || process.command_line.find("sudo ghci")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/sbin/hping3" || process.path == "/usr/bin/hping") && (process.command_line.find("hping") || process.command_line.find("sudo hping")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/gtester") && (process.command_line.find("gtester") || process.command_line.find("sudo gtester")) && (process.syscall == "exec"))
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
            if ((process.path == "/snap/gimp/322/usr/bin/python2.7" || process.path == "/usr/bin/gimp") && (process.command_line.find("gimp") || process.command_line.find("sudo gimp")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/ruby" || process.path == "/usr/bin/irb" || process.path == "/usr/bin/sudo") && (process.command_line.find("irb") || process.command_line.find("sudo irb")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/jjs") && (process.command_line.find("jjs") || process.command_line.find("sudo jjs")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/mail.mailutils" || process.path == "/usr/bin/mail") && (process.command_line.find("mail") || process.command_line.find("sudo mail")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/make") && (process.command_line.find("make") || process.command_line.find("sudo make")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/more") && (process.command_line.find("more") || process.command_line.find("sudo more")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/nawk") && (process.command_line.find("nawk") || process.command_line.find("sudo nawk")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/node") && (process.command_line.find("node") || process.command_line.find("sudo node")) && (process.syscall == "exec"))
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
//             if ((process.path == "/usr/bin/nano" || process.path == "/usr/bin/pico") && (process.command_line.find("pico") || process.command_line.find("sudo pico")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/zypper") && (process.command_line.find("zypper") || process.command_line.find("sudo zypper")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/zsh") && (process.command_line.find("zsh") || process.command_line.find("sudo zsh")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/valgrind.bin" || process.path == "/usr/bin/valgrind") && (process.command_line.find("valgrind") || process.command_line.find("sudo valgrind")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/top") && (process.command_line.find("top") || process.command_line.find("sudo top")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/tclsh") && (process.command_line.find("tclsh") || process.command_line.find("sudo tclsh")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/basename" || process.path == "/usr/sbin/service") && (process.command_line.find("basename") || process.command_line.find("sudo service") || process.command_line.find("/usr/sbin/service") || process.command_line.find("service")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/vim.tiny" || process.path == "/usr/bin/rview") && (process.command_line.find("rview") || process.command_line.find("sudo rview")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/run-parts") && (process.command_line.find("run-parts") || process.command_line.find("sudo run-parts")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/python") && (process.command_line.find("python") || process.command_line.find("sudo python")) && (process.syscall == "exec"))
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
            if ((process.path == "/usr/bin/postgresql" || process.path == "/usr/bin/psql") && (process.command_line.find("psql") || process.command_line.find("sudo psql")) && (process.syscall == "exec"))
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

// New Rules start from here (Mitre Attack)

// bool ssh_authorized_keys(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.path == "/usr/bin/ssh-keygen")
//     {
//         if ((process_event.entry.command_line.find("ssh-keygen") != std::string::npos))
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
//         if ((process_event.entry.command_line.find("modprobe") != std::string::npos))
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
    if (process_event.entry.command_line.find("modprobe") != std::string::npos)
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
        if ((process_event.entry.command_line.find("insmod") != std::string::npos))
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
//     if (process_event.entry.command_line.find("insmod")!= std::string::npos)
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
        if ((process_event.entry.command_line.find("lsmod") != std::string::npos))
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
//     if (process_event.entry.command_line.find("lsmod")!= std::string::npos)
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
        if ((process_event.entry.command_line.find("rmmod") != std::string::npos))
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
//     if (process_event.entry.command_line.find("rmmod")!= std::string::npos)
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
        if ((process_event.entry.command_line.find("useradd") != std::string::npos))
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
    if ((process_event.entry.command_line.find("adduser") != std::string::npos))
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
//     if (process_event.entry.command_line.find("useradd")!= std::string::npos or process_event.entry.command_line.find("adduser")!= std::string::npos)
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
    if (process_event.entry.command_line.find("net user /add /domain") != std::string::npos)
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
    if (process_event.entry.command_line.find("trap") != std::string::npos)
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
// bool _linker_hijadynamiccking(const ProcessEvent &process_event, Event &rule_event)
// {
//     if (process_event.entry.command_line.find("LD_PRELOAD") != std::string::npos)
//     {

//         std::stringstream ss;

//         ss << "[" << process_event.entry.path << " (pid: " << process_event.entry.pid << ")] dynamic linker hijacking";

//         rule_event.metadata = ss.str();

//         return true;
//     }
//     return false;
// }

// https://attack.mitre.org/techniques/T1053/002/
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1053.002/T1053.002.md
bool scheduled_task_at(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path == "/usr/bin/at")
    {
        if ((process_event.entry.command_line.find("at") != std::string::npos))
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
        if ((process_event.entry.command_line.find("atrm") != std::string::npos))
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
        if ((process_event.entry.command_line.find("crontab") != std::string::npos))
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
    if (process_event.entry.command_line.find("systemctl") != std::string::npos)
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
    if (process_event.entry.command_line.find("chmod u+s") != std::string::npos or process_event.entry.command_line.find("chmod u+x") != std::string::npos)
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
        if (process_event.entry.command_line.find("chmod u+s") != std::string::npos or process_event.entry.command_line.find("chmod u+x") != std::string::npos)
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
                if (process.command_line.find("sudo visudo") != std::string::npos)
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
        if (process_event.entry.command_line.find("systemctl list-units -–type=service –all") != std::string::npos)
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
        if ((process_event.entry.command_line.find("curl") != std::string::npos) and (process_event.entry.command_line.find("%://%") != std::string::npos) or (process_event.entry.command_line.find("%.%") != std::string::npos))
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
    if (process_event.entry.command_line.find("$HISTFILE") != std::string::npos)
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
        if ((process_event.entry.command_line.find("%python%") != std::string::npos) or (process_event.entry.command_line.find("%://%") != std::string::npos))
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
    if ((process_event.entry.command_line.find("unset HISTFILE") != std::string::npos) or (process_event.entry.command_line.find("export HISTFILESIZE=0") != std::string::npos) or (process_event.entry.command_line.find("history -c") != std::string::npos) or (process_event.entry.command_line.find("rm ~/.bash_history") != std::string::npos))
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
        if (process_event.entry.command_line.find("ping") != std::string::npos)
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
        if (process_event.entry.command_line.find("mount") != std::string::npos)
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
        if (process_event.entry.command_line.find("umount") != std::string::npos)
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
        if (process_event.entry.command_line.find("chgrp") != std::string::npos)
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
        if (process_event.entry.command_line.find("pam_timestamp_check") != std::string::npos)
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
        if (process_event.entry.command_line.find("unix_chkpwd") != std::string::npos)
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
        if (process_event.entry.command_line.find("pwck") != std::string::npos)
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
        if (process_event.entry.command_line.find("userhelper") != std::string::npos)
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
        if (process_event.entry.command_line.find("Xorg") != std::string::npos)
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
        if (process_event.entry.command_line.find("rlogin") != std::string::npos)
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
        if (process_event.entry.command_line.find("sudoedit") != std::string::npos)
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
        if (process_event.entry.command_line.find("rsh") != std::string::npos)
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
        if (process_event.entry.command_line.find("gpasswd") != std::string::npos)
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
        if (process_event.entry.command_line.find("sudo") != std::string::npos)
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
        if (process_event.entry.command_line.find("staprun") != std::string::npos)
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
        if (process_event.entry.command_line.find("rcp") != std::string::npos)
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
        if (process_event.entry.command_line.find("passwd") != std::string::npos)
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
        if (process_event.entry.command_line.find("chsh") != std::string::npos)
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
        if (process_event.entry.command_line.find("chfn") != std::string::npos)
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
        if (process_event.entry.command_line.find("chage") != std::string::npos)
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
        if (process_event.entry.command_line.find("setfacl") != std::string::npos)
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
        if (process_event.entry.command_line.find("chacl") != std::string::npos)
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
        if (process_event.entry.command_line.find("chcon") != std::string::npos)
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
        if (process_event.entry.command_line.find("newgrp") != std::string::npos)
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
        if (process_event.entry.command_line.find("sleep") != std::string::npos)
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
        if (process_event.entry.command_line.find("pgrep") != std::string::npos)
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
        if (process_event.entry.command_line.find("grep") != std::string::npos)
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
        if (process_event.entry.command_line.find("lspci") != std::string::npos)
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
        if (process_event.entry.command_line.find("udevadm") != std::string::npos)
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
        if (process_event.entry.command_line.find("findmnt") != std::string::npos)
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
        if (process_event.entry.command_line.find("netstat") != std::string::npos)
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
        if (process_event.entry.command_line.find("awk") != std::string::npos)
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
//         if (process_event.entry.command_line.find("sed") != std::string::npos)
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
    if (process_event.entry.path == "/usr/bin/systemd-detect-virt" or process_event.entry.command_line == "systemd-detect-virt")
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
    if (process_event.entry.path == "/usr/sbin/dmidecode" or process_event.entry.command_line == "dmidecode")
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
    if (process_event.entry.command_line.find("touch -a -t") != std::string::npos)
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
    if (process_event.entry.command_line.find("touch -acmr") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/sudoers") != std::string::npos)
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
    if ((process_event.entry.command_line.find("/etc/sudoers") != std::string::npos) and (process_event.entry.command_line.find("sudo sed -i 's/env_reset.*$/env_reset,timestamp_timeout=-1/' /etc/sudoers") != std::string::npos) or (process_event.entry.command_line.find("env_reset") != std::string::npos) or (process_event.entry.command_line.find("timestamp_timeout=-1") != std::string::npos))
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
    if ((process_event.entry.command_line.find("/etc/sudoers") != std::string::npos) and (process_event.entry.command_line.find("Defaults \"'!'\"tty_tickets") != std::string::npos))
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
    if (process_event.entry.command_line.find("export HISTCONTROL=ignoreboth") != std::string::npos)
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
    if ((process_event.entry.command_line.find("export HISTCONTROL=ignoreboth") != std::string::npos) and (process_event.entry.command_line.find("echo export \"HISTCONTROL=ignoreboth\" >> ~/.bash_profile") != std::string::npos))
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
    if (process_event.entry.command_line.find("rm -f") != std::string::npos)
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
    if (process_event.entry.command_line.find("rm -rf") != std::string::npos)
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
    if (process_event.entry.command_line.find("shred -u") != std::string::npos)
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
    if (process_event.entry.command_line.find("rm -rf / --no-preserve-root > /dev/null 2> /dev/null") != std::string::npos)
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
//     if (process_event.entry.command_line.find("touch -m -t")!= std::string::npos)
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
    if (process_event.entry.command_line.find("crontab -l") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/cron.daily/") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/cron.hourly/") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/cron.monthly/") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/cron.weekly/") != std::string::npos)
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
    if (process_event.entry.command_line.find("/var/spool/cron/crontabs/") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/ld.so.preload") != std::string::npos)
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
    if (process_event.entry.command_line.find("LD_PRELOAD=") != std::string::npos)
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
    if (process_event.entry.command_line.find("sudo insmod") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/pam.d/su-l") != std::string::npos)
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
    if (process_event.entry.command_line.find("auth sufficient pam_succeed_if.so uid >= 0") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/rc.common") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/rc.local") != std::string::npos)
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
    if (process_event.entry.command_line.find(".ssh/authorized_keys") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/systemd/system") != std::string::npos)
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
    if (process_event.entry.command_line.find("art-systemd-service.service") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/systemd/system/art-timer.service") != std::string::npos)
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
    if (process_event.entry.command_line.find("/etc/systemd/system/art-timer.timer") != std::string::npos)
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
    if ((process_event.entry.command_line.find("ssh") != std::string::npos) or (process_event.entry.command_line.find("known_hosts") != std::string::npos) or (process_event.entry.command_line.find("oBatchMode") != std::string::npos) or (process_event.entry.command_line.find("oStrictHostKeyChecking") != std::string::npos))
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
    if (process_event.entry.command_line.find("ssh-rsa") != std::string::npos)
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
    if ((process_event.entry.command_line.find("scp") != std::string::npos) or (process_event.entry.command_line.find("rsync") != std::string::npos) or (process_event.entry.command_line.find("curl") != std::string::npos) or (process_event.entry.command_line.find("sftp") != std::string::npos) or (process_event.entry.command_line.find("ftp") != std::string::npos))
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
    if ((process_event.entry.command_line.find("sh -c 'echo")!= std::string::npos) and (process_event.entry.command_line.find("/etc/ld.so.preload")!= std::string::npos))
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
    if ((process_event.entry.command_line.find("systemctl stop rsyslog")!= std::string::npos) or (process_event.entry.command_line.find("systemctl disable rsyslog")!= std::string::npos) or (process_event.entry.command_line.find("service rsyslog stop")!= std::string::npos) or (process_event.entry.command_line.find("chkconfig off rsyslog")!= std::string::npos))
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
    if ((process_event.entry.command_line.find("service cbdaemon stop")!= std::string::npos) or (process_event.entry.command_line.find("chkconfig off cbdaemon")!= std::string::npos) or (process_event.entry.command_line.find("systemctl stop cbdaemon")!= std::string::npos) or (process_event.entry.command_line.find("systemctl disable cbdaemon")!= std::string::npos))
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
    if (process_event.entry.command_line.find("setenforce 0")!= std::string::npos)
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
    if ((process_event.entry.command_line.find("systemctl stop falcon-sensor.service")!= std::string::npos) or (process_event.entry.command_line.find("systemctl disable falcon-sensor.service")!= std::string::npos))
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
    if ((process_event.entry.command_line.find("ufw disable")!= std::string::npos) or (process_event.entry.command_line.find("ufw enable")!= std::string::npos))
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
    if ((process_event.entry.command_line.find("systemctl stop ufw")!= std::string::npos) or (process_event.entry.command_line.find("systemctl start ufw")!= std::string::npos))
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
    if (process_event.entry.command_line.find("ufw logging off")!= std::string::npos)
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
    if ((process_event.entry.command_line.find("ufw prepend deny from")!= std::string::npos) or (process_event.entry.command_line.find("ufw status numbered")!= std::string::npos) or (process_event.entry.command_line.find("ufw prepend")!= std::string::npos))
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
    if ((process_event.entry.command_line.find("echo")!= std::string::npos) and (process_event.entry.command_line.find("/etc/ufw/user.rules")!= std::string::npos) or (process_event.entry.command_line.find("user.rules")!= std::string::npos))
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
    if ((process_event.entry.command_line.find("echo")!= std::string::npos) and (process_event.entry.command_line.find("/etc/ufw/ufw.conf")!= std::string::npos) or (process_event.entry.command_line.find("ufw.conf")!= std::string::npos))
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
    if ((process_event.entry.command_line.find("echo")!= std::string::npos) and (process_event.entry.command_line.find("/etc/ufw/sysctl.conf")!= std::string::npos) or (process_event.entry.command_line.find("sysctl.conf")!= std::string::npos))
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
    if ((process_event.entry.command_line.find("echo")!= std::string::npos) and (process_event.entry.command_line.find("/etc/default/ufw")!= std::string::npos) or (process_event.entry.command_line.find("default/ufw")!= std::string::npos))
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
    if (process_event.entry.command_line.find("tail /var/log/ufw.log")!= std::string::npos)
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
    if (process_event.entry.command_line.find("python")!= std::string::npos and process_event.entry.command_line.find("b64decode")!= std::string::npos )
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
    if (process_event.entry.command_line.find("perl")!= std::string::npos and process_event.entry.command_line.find("decode_base64")!= std::string::npos )
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
    if (process_event.entry.command_line.find("base64 -d")!= std::string::npos)
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
    if (process_event.entry.command_line.find("xxd -r -p")!= std::string::npos or process_event.entry.command_line.find("xxd -p -r")!= std::string::npos)
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
    if (process_event.entry.command_line.find("gcc")!= std::string::npos or process_event.entry.command_line.find("clang")!= std::string::npos)
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
    if (process_event.entry.command_line.find("go run")!= std::string::npos)
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
    if (process_event.entry.command_line.find("gcloud auth login")!= std::string::npos or process_event.entry.command_line.find("gcloud config set project")!= std::string::npos or process_event.entry.command_line.find("gcloud config set project")!= std::string::npos or process_event.entry.command_line.find("gcloud iam service-accounts create")!= std::string::npos or process_event.entry.command_line.find("gcloud iam service-accounts keys create")!= std::string::npos)
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
    if (process_event.entry.command_line.find("rm -rf /private/var/log/system.log*")!= std::string::npos or process_event.entry.command_line.find("rm -rf /private/var/audit/*")!= std::string::npos)
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
    if (process_event.entry.command_line.find("echo 0> /var/spool/mail/")!= std::string::npos or process_event.entry.command_line.find("echo 0>")!= std::string::npos)
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
    if (process_event.entry.path.find("/usr/bin/at")!= std::string::npos or process_event.entry.command_line.find("at")!= std::string::npos)
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
    if (process_event.entry.path.find("/usr/bin/w")!= std::string::npos or process_event.entry.command_line==("w") or process_event.entry.path.find("/usr/bin/who")!= std::string::npos or process_event.entry.command_line==("who") or process_event.entry.path.find("/usr/bin/whoami")!= std::string::npos or process_event.entry.command_line==("whoami") or process_event.entry.path.find("/usr/bin/users")!= std::string::npos or process_event.entry.command_line==("users"))
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
    if (process_event.entry.command_line.find("cat /etc/passwd")!= std::string::npos)
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
    if (process_event.entry.command_line.find("cat /etc/sudoers")!= std::string::npos)
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
    if (process_event.entry.command_line.find("grep 'x:0:' /etc/passwd")!= std::string::npos)
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
    if (process_event.entry.command_line.find("username=$(id -u -n) && lsof -u $username")!= std::string::npos)
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
    if (process_event.entry.command_line.find("lastlog")!= std::string::npos)
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
    if (process_event.entry.command_line.find("groups")!= std::string::npos or process_event.entry.command_line.find("id")!= std::string::npos)
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
    if (process_event.entry.command_line.find("systemctl --type=service")!= std::string::npos)
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
    if (process_event.entry.command_line.find("tcpdump")!= std::string::npos or process_event.entry.command_line.find("tshark")!= std::string::npos)
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
    if (process_event.entry.command_line.find("smbstatus --shares")!= std::string::npos)
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
    if (process_event.entry.command_line.find("uname -a")!= std::string::npos or process_event.entry.command_line.find("cat /etc/lsb-release")!= std::string::npos or process_event.entry.command_line.find("cat /etc/redhat-release")!= std::string::npos or process_event.entry.command_line.find("cat /etc/issue")!= std::string::npos or process_event.entry.command_line.find("uptime")!= std::string::npos)
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
    if (process_event.entry.command_line.find("lsmod")!= std::string::npos)
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
    if (process_event.entry.command_line.find("hostname")!= std::string::npos)
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
    if (process_event.entry.command_line.find("env")!= std::string::npos)
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
    if (process_event.entry.command_line.find("*.mozilla/firefox/*/places.sqlite")!= std::string::npos or process_event.entry.file_path.find("*.mozilla/firefox/*/places.sqlite")!= std::string::npos)
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
    if (process_event.entry.command_line.find("arp -a")!= std::string::npos or process_event.entry.command_line.find("ifconfig")!= std::string::npos or process_event.entry.command_line.find("ip addr")!= std::string::npos or process_event.entry.command_line.find("netstat -ant")!= std::string::npos)
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
    if (process_event.entry.command_line.find("ls -a")!= std::string::npos or process_event.entry.command_line.find("ls -la")!= std::string::npos or process_event.entry.command_line.find("file */* *")!= std::string::npos or process_event.entry.command_line.find("find")!= std::string::npos or process_event.entry.command_line.find("ls -R ")!= std::string::npos or process_event.entry.command_line.find("locate")!= std::string::npos or process_event.entry.command_line.find("which")!= std::string::npos)
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
    if (process_event.entry.command_line.find("netstat")!= std::string::npos or process_event.entry.command_line.find("who")!= std::string::npos)
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
    if (process_event.entry.command_line.find("ps")!= std::string::npos or process_event.entry.command_line.find("ps aux")!= std::string::npos)
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
    if (process_event.entry.command_line.find("dscacheutil -q group")!= std::string::npos or process_event.entry.command_line.find("dscl . -list /Groups")!= std::string::npos or process_event.entry.command_line.find("groups")!= std::string::npos or process_event.entry.command_line.find("id")!= std::string::npos or process_event.entry.command_line.find("getent group")!= std::string::npos or process_event.entry.command_line.find("/etc/group")!= std::string::npos)
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
bool examine_password_complexity_policy(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/pam.d/common-password")!= std::string::npos or process_event.entry.file_path.find("/etc/security/pwquality.conf")!= std::string::npos or process_event.entry.file_path.find("/etc/pam.d/system-auth")!= std::string::npos or process_event.entry.file_path.find("/etc/security/pwquality.conf")!= std::string::npos or process_event.entry.file_path.find("/etc/login.defs")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1201 - Password Policy Discovery - Examine password expiration policy " ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1518.001 - Security Software Discovery
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1518.001/T1518.001.md#t1518001---security-software-discovery
bool security_software_discovery(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("ps aux")!= std::string::npos and (process_event.entry.command_line.find("falcond")!= std::string::npos or process_event.entry.command_line.find("nessusd")!= std::string::npos or process_event.entry.command_line.find("cbagentd")!= std::string::npos or process_event.entry.command_line.find("td-agent")!= std::string::npos or process_event.entry.command_line.find("packetbeat")!= std::string::npos or process_event.entry.command_line.find("filebeat")!= std::string::npos or process_event.entry.command_line.find("auditbeat")!= std::string::npos or process_event.entry.command_line.find("osqueryd")!= std::string::npos))
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
    if (process_event.entry.command_line.find("ip neighbour")!= std::string::npos or process_event.entry.command_line.find("ip route")!= std::string::npos or process_event.entry.command_line.find("ip tcp_metrics")!= std::string::npos)
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
    if (process_event.entry.command_line.find("nmap")!= std::string::npos or process_event.entry.command_line.find("telnet")!= std::string::npos or process_event.entry.command_line.find("nc -nv")!= std::string::npos)
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
    if (process_event.entry.command_line.find("gpg")!= std::string::npos and process_event.entry.command_line.find("batch")!= std::string::npos and process_event.entry.command_line.find("passphrase")!= std::string::npos and process_event.entry.command_line.find("cipher-algo")!= std::string::npos)
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
    if (process_event.entry.command_line.find("7z a -p")!= std::string::npos)
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
    if (process_event.entry.command_line.find("rsautl -encrypt")!= std::string::npos)
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
    if (process_event.entry.command_line.find("dd")!= std::string::npos and process_event.entry.command_line.find("if")!= std::string::npos and process_event.entry.command_line.find("of")!= std::string::npos)
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
    if (process_event.entry.command_line.find("shutdown -r")!= std::string::npos or process_event.entry.command_line.find("shutdown -h")!= std::string::npos or process_event.entry.command_line.find("reboot")!= std::string::npos or process_event.entry.command_line.find("halt -p")!= std::string::npos or process_event.entry.command_line.find("halt --reboot")!= std::string::npos or process_event.entry.command_line.find("poweroff")!= std::string::npos or process_event.entry.command_line.find("poweroff --reboot")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1485 - Data Destruction -  Overwrite file with DD" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// ----------------------------------------------------- Exfiltration ---------------------------------------------------------

// T1048.002 - Exfiltration Over Asymmetric Encrypted Non-C2 Protocol
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1529/T1529.md#description-from-attck
bool exfiltrate_data_https_using_curl(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("curl -F")!= std::string::npos and process_event.entry.command_line.find("file.io")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1048.002 - Exfiltration Over Asymmetric Encrypted Non-C2 Protocol - Exfiltrate data HTTPS using curl linux" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// ----------------------------------------------------- Collection ---------------------------------------------------------

// T1560.001 - Archive via Utility
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1560.001/T1560.001.md#t1560001---archive-via-utility
bool data_compressed_zip(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("zip")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1560.001 - Archive via Utility - Data Compressed - nix - zip" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1560.001 - Archive via Utility
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1560.001/T1560.001.md#t1560001---archive-via-utility
bool data_compressed_zip_nix_gzip(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("gzip -k")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1560.001 - Archive via Utility - Data Compressed - nix - gzip Single File" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1560.001 - Archive via Utility
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1560.001/T1560.001.md#t1560001---archive-via-utility
bool data_compressed_zip_nix_tar(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("tar -cvzf")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1560.001 - Archive via Utility - Data Compressed - nix - tar Folder or File" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1113 - Screen Capture
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1113/T1113.md#t1113---screen-capture
bool x_windows_capture(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("xwd")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1113 - Screen Capture - X Windows Capture" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1113 - Screen Capture
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1113/T1113.md#t1113---screen-capture
bool capture_linux_desktop_using_import_tool(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("import -window root")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1113 - Screen Capture - Capture Linux Desktop using Import Tool" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1056.001 - Keylogging
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1056.001/T1056.001.md#t1056001---keylogging
bool terminal_input_capture_linux_with_pam_d(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/pam.d/password-auth")!= std::string::npos or process_event.entry.file_path.find("/etc/pam.d/system-auth")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1056.001 - Keylogging - Living off the land Terminal Input Capture on Linux with pam.d" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1056.001 - Keylogging - Atomic Test #3 - Logging bash history to syslog
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1056.001/T1056.001.md#t1056001---keylogging
bool logging_bash_history_to_syslog(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/var/log/syslog")!= std::string::npos or process_event.entry.file_path.find(".bash_history")!= std::string::npos or process_event.entry.file_path.find(".bashrc")!= std::string::npos or process_event.entry.file_path.find(".bash_aliases")!= std::string::npos or process_event.entry.file_path.find("/etc/skel/.bashrc")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1056.001 - Keylogging - Logging bash history to syslog" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1056.001 - Keylogging - Atomic Test #5 - SSHD PAM keylogger
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1056.001/T1056.001.md#t1056001---keylogging
bool sshd_pam_keylogger(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/var/log/audit/audit.log")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1056.001 - Keylogging - SSHD PAM keylogger & Auditd keylogger" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1074.001 - Local Data Staging
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1056.001/T1056.001.md#t1056001---keylogging
bool stage_data_from_discovery_sh(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("curl")!= std::string::npos and process_event.entry.command_line.find(".sh")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1074.001 - Local Data Staging" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1115 - Clipboard Data
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1115/T1115.md#t1115---clipboard-data
bool add_or_copy_content_to_clipboard_with_xclip(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("xclip -o")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1115 - Clipboard Data - Add or copy content to clipboard with xClip [linux]" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1560.002 - Archive via Library
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1115/T1115.md#t1115---clipboard-data
bool compressing_data_using_gzip_in_python(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("python")!= std::string::npos and process_event.entry.command_line.find("GzipFile")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1560.002 - Archive via Library - Compressing data using GZip in Python (Linux)" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1560.002 - Archive via Library bz2
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1115/T1115.md#t1115---clipboard-data
bool compressing_data_using_bz2_in_python(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("python")!= std::string::npos and process_event.entry.command_line.find("bz2")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1560.002 - Archive via Library - Compressing data using bz2 in Python (Linux)" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1560.002 - Archive via Library zipfile
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1115/T1115.md#t1115---clipboard-data
bool compressing_data_using_zipfile_in_python(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("python")!= std::string::npos and process_event.entry.command_line.find("zipfile")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1560.002 - Archive via Library - Compressing data using zipfile in Python (Linux)" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// Command and Control: T1571 Non-Standard Port

// https://attack.mitre.org/techniques/T1571/
 
bool non_standard_port_command_control(const ProcessEvent& process_event, Event& rule_event)
{
   for (auto &process : process_event.parents)
   {
       std::unordered_map<int,std::string> mp;
       mp.insert({4050,"APT-C-36 attack using 4050 port for command control"});
       mp.insert({14146,"APT32 attack using 14146 port for command control"});
       mp.insert({808,"APT33 attack using 808 port for command control"});
       mp.insert({880,"APT33 attack using 880 port for command control "});
       mp.insert({8000,"BADCALL attack using 8000 port for command control "});
       mp.insert({1058,"BANKSHOT attack using 1058 port for command control "});
       mp.insert({5190,"DarkVishnya attack using 5190 port for command control "});
       mp.insert({7900,"DarkVishnya attack using 7900 port for command control "});
       mp.insert({4444,"DarkVishnya attack using 4444 port for command control "});
       mp.insert({4445,"DarkVishnya attack using 4445 port for command control "});
       mp.insert({31337,"DarkVishnya attack using 31337 port for command control "});
       mp.insert({20,"Emotet attack using 20 port for command control "});
       mp.insert({22,"Emotet attack using 22 port for command control "});
       mp.insert({7080,"Emotet attack using 7080 port for command control "});
       mp.insert({50000,"Emotet attack using 50000 port for command control "});
       mp.insert({53,"FIN7 attack using 53 port for command control "});
       mp.insert({80,"FIN7 attack using 80 port for command control "});
       mp.insert({8080,"FIN7 attack using 8080 port for command control "});
       mp.insert({9005,"GoldenSpy attack using 9005 port for command control "});
       mp.insert({9006,"GoldenSpy attack using 9006 port for command control "});
       mp.insert({9002,"GoldenSpy attack using 9002 port for command control "});
       mp.insert({33666,"GoldenSpy attack using 33666 port for command control "});
       mp.insert({8090,"GoldenSpy attack using 8090 port for command control "});
       mp.insert({46769,"GravityRAT attack using 46769 port for command control "});
       mp.insert({4443,"MagicHound attack using 4443 port for command control "});
       mp.insert({9999,"Metamorfo attack using 9999 port for command control "});
       mp.insert({1177,"njRAT attack using 1177 port for command control "});
       mp.insert({995,"RedLeaves attack using 995 port for command control "});
       mp.insert({51640,"Rocke attack using 51640 port for command control "});
       mp.insert({44443,"RTM attack using 44443 port for command control "});
       mp.insert({6789,"SabdwormTeam attack using 6789 port for command control "});
       mp.insert({444,"Silence attack using 444 port for command control "});
       mp.insert({1402,"StrongPity attack using 1402 port for command control "});
       mp.insert({4444,"TEMP.Veles attack using 4444 port for command control "});
       mp.insert({8531,"TEMP.Veles attack using 8531 port for command control "});
       mp.insert({50501,"TEMP.Veles attack using 50501 port for command control "});
       mp.insert({447,"TrickBot attack using 447 port for command control "});
       mp.insert({8082,"TrickBot attack using 8082 port for command control "});
       mp.insert({8443,"TypeFrame attack using 8443 port for command control "});
       mp.insert({25,"WellMail attack using 25 port for command control "});
       mp.insert({2083,"WIRTE attack using 2083 port for command control "});
       mp.insert({2087,"WIRTE attack using 2087 port for command control "});
       mp.insert({1985,"ZxShell attack using 1985 port for command control "});
       mp.insert({1986,"ZxShell attack using 1986 port for command control "});
       if (mp.find(process_event.entry.remote_port)!=mp.end())
       {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.remote_port << " (remote port: " << mp[process_event.entry.remote_port] << ")] non standard port used ]";
           rule_event.metadata = ss.str();
 
           return true;
       }
   }
   return false;
}

// T1200 - Tactic: Initial Access
//
bool hardware_additions(const ProcessEvent &process_event, Event &rule_event)
{
   if (process_event.entry.command_line.find("/usr/bin/nautilus-autorun-software") != std::string::npos)
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
   bool condition_1 = process_event.entry.command_line.find(".so") != std::string::npos;
   bool condition_2 = process_event.entry.command_line.find("bindtextdomain.so") == std::string::npos;
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
// bool dynamic_linker_hijacking(const ProcessEvent& process_event, Event& rule_event)
// {
//     if (process_event.entry.file_path.find("/etc/ld.so.preload")!= std::string::npos)
//     {

//             std::stringstream ss;

//             ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1574.006-Hijack Execution Flow: Dynamic Linker Hijacking" ;
//             rule_event.metadata = ss.str();

//             return true;
//     }
//     return false;
// }

// T1556.003-Modify Authentication Process: Pluggable Authentication Modules
// https://attack.mitre.org/techniques/T1556/003/
bool pluggable_authentication_modules_file(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/pam.d/")!= std::string::npos)
    {

            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1556.003-Modify Authentication Process: Pluggable Authentication Modules" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

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



// ----------------------------------------------------- Credential Access ---------------------------------------------------------

// T1556.003 - Modify Authentication Process: Pluggable Authentication Modules
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1556.003/T1556.003.md#t1556003---modify-authentication-process-pluggable-authentication-modules
bool malicious_pam_rules(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/pam.d/su-l")!= std::string::npos and process_event.entry.syscall.find("O_WRONLY")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1556.003 -  Input Capture" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1056.001 - Input Capture: Keylogging
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1056.001/T1056.001.md#t1056001---input-capture-keylogging
bool input_capture_keylogging(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/pam.d/password-auth")!= std::string::npos or process_event.entry.file_path.find("/etc/pam.d/password-auth")!= std::string::npos and process_event.entry.syscall.find("O_WRONLY")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1556.003 - Modify Authentication Process: Pluggable Authentication Modules" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1056.001 - Input Capture: Keylogging
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1056.001/T1056.001.md#t1056001---input-capture-keylogging
bool sshd_pam_keylogger_keylogging(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/pam.d/sshd")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1556.003 - SSHD PAM keylogger" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}



// T1555.003 - Credentials from Password Stores: Credentials from Web Browsers
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1555.003/T1555.003.md#t1555003---credentials-from-password-stores-credentials-from-web-browsers
bool dump_credentials_from_web_browsers(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("LaZagne")!= std::string::npos or process_event.entry.file_path.find("mimipenguin")!= std::string::npos )
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1555.003 - Credentials from Password Stores: Credentials from Web Browsers" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1552.004 - Unsecured Credentials: Private Keys
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1552.004/T1552.004.md#t1552004---unsecured-credentials-private-keys  
bool discover_private_ssh_keys(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("find")!= std::string::npos and process_event.entry.command_line.find("id_rsa")!= std::string::npos )
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]  T1552.004 - Unsecured Credentials: Dicover Private Keys" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}



// T1552.004 - Unsecured Credentials: Private Keys
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1552.004/T1552.004.md#t1552004---unsecured-credentials-private-keys  
bool copy_private_ssh_keys_with_cp(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("cp")!= std::string::npos and process_event.entry.command_line.find("id_rsa")!= std::string::npos )
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]  T1552.004 - Unsecured Credentials: Copy Private Keys" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1552.001 - Unsecured Credentials: Credentials In Files
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1552.001/T1552.001.md#t1552001---unsecured-credentials-credentials-in-files
bool extract_passwords_with_grep(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.command_line.find("grep")!= std::string::npos and process_event.entry.command_line.find("password")!= std::string::npos )
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1552.001 - Unsecured Credentials: Credentials In Files" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

// T1003.008 - OS Credential Dumping: /etc/passwd and /etc/shadow
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1003.008/T1003.008.md#t1003008---os-credential-dumping-etcpasswd-and-etcshadow
bool access_etc_shadow_passwd(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.file_path.find("/etc/shadow")!= std::string::npos or process_event.entry.file_path.find("/etc/passwd")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1003.008 - OS Credential Dumping: /etc/passwd and /etc/shadow" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}

