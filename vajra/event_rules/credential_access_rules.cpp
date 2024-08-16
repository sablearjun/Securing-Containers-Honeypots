#include "credential_access_rules.h"
#include <sstream>

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
    if (process_event.entry.file_path.find("find")!= std::string::npos and process_event.entry.cmdline.find("id_rsa")!= std::string::npos )
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
    if (process_event.entry.cmdline.find("cp")!= std::string::npos and process_event.entry.cmdline.find("id_rsa")!= std::string::npos )
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
    if (process_event.entry.cmdline.find("grep")!= std::string::npos and process_event.entry.cmdline.find("password")!= std::string::npos )
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