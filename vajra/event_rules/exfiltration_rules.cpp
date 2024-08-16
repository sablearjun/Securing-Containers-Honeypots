#include "exfiltration_rules.h"
#include <sstream>

// ----------------------------------------------------- Exfiltration ---------------------------------------------------------

// T1048.002 - Exfiltration Over Asymmetric Encrypted Non-C2 Protocol
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1529/T1529.md#description-from-attck
bool exfiltrate_data_https_using_curl(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.cmdline.find("curl -F") != std::string::npos and process_event.entry.cmdline.find("file.io") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")] T1048.002 - Exfiltration Over Asymmetric Encrypted Non-C2 Protocol - Exfiltrate data HTTPS using curl linux";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}

// T1048.003 - Exfiltration Over Alternative Protocol: Exfiltration Over Unencrypted/Obfuscated Non-C2 Protocol
// https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1048.003/T1048.003.md#t1048003---exfiltration-over-alternative-protocol-exfiltration-over-unencryptedobfuscated-non-c2-protocol
bool exfiltrate_data_alternate_protocol(const ProcessEvent &process_event, Event &rule_event)
{
    if (process_event.entry.path.find("wget") != std::string::npos and process_event.entry.syscall.find("connect") != std::string::npos)
    {
        std::stringstream ss;

        ss << "[" << process_event.entry.file_path << " (pid: " << process_event.entry.pid << ")]T1048.003 - Exfiltration Over Alternative Protocol: Exfiltration Over Unencrypted/Obfuscated Non-C2 Protocol";
        rule_event.metadata = ss.str();

        return true;
    }
    return false;
}