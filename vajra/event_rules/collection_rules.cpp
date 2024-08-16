#include "collection_rules.h"
#include <sstream>


// ----------------------------------------------------- Collection ---------------------------------------------------------


// T1560.001 - Archive via Utility
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1560.001/T1560.001.md#t1560001---archive-via-utility
bool data_compressed_zip(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("zip")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
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
    if (process_event.entry.cmdline.find("gzip -k")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
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
    if (process_event.entry.cmdline.find("tar -cvzf")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
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
    if (process_event.entry.cmdline.find("xwd")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
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
    if (process_event.entry.cmdline.find("import -window root")!= std::string::npos and process_event.entry.syscall.find("exec")!= std::string::npos)
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
    if (process_event.entry.cmdline.find("curl")!= std::string::npos and process_event.entry.cmdline.find(".sh")!= std::string::npos)
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
    if (process_event.entry.cmdline.find("xclip -o")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1115 - Clipboard Data - Add or copy content to clipboard with xClip [linux]" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1560.002 - Archive via Library
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1560.002/T1560.002.md
bool compressing_data_using_gzip_in_python(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("python")!= std::string::npos and process_event.entry.cmdline.find("GzipFile")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1560.002 - Archive via Library - Compressing data using GZip in Python (Linux)" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1560.002 - Archive via Library bz2
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1560.002/T1560.002.md
bool compressing_data_using_bz2_in_python(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("python")!= std::string::npos and process_event.entry.cmdline.find("bz2")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1560.002 - Archive via Library - Compressing data using bz2 in Python (Linux)" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}


// T1560.002 - Archive via Library zipfile
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1560.002/T1560.002.md
bool compressing_data_using_zipfile_in_python(const ProcessEvent& process_event, Event& rule_event)
{
    if (process_event.entry.cmdline.find("python")!= std::string::npos and process_event.entry.cmdline.find("zipfile")!= std::string::npos)
    {
            std::stringstream ss;

            ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1560.002 - Archive via Library - Compressing data using zipfile in Python (Linux)" ;
            rule_event.metadata = ss.str();

            return true;
    }
    return false;
}