#ifndef FLEET_WIN_EXFILTERATION_RULES_H
#define FLEET_WIN_EXFILTERATION_RULES_H

#include "../common/EdrDataTypes.h"

bool automated_exfiltration(const ProcessEvent &win_process_event, Event &rule_event);
bool exfiltration_over_encrypted_protocol(const ProcessEvent &win_process_event, Event &rule_event);
bool exfiltration_over_web_service(const ProcessEvent &win_process_event, Event &rule_event);
bool powershell_ICMP_exfiltration(const ProcessEvent &process_event, Event &rule_event);
bool powershell_DNSExfiltration(const ProcessEvent &process_event, Event &rule_event);
bool powershell_exfiltration_over_SMTP(const ProcessEvent &process_event, Event &rule_event);
#endif // FLEET_WIN_EXFILTERATION_RULES_H