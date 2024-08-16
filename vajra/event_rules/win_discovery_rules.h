#ifndef FLEET_WIN_DISCOVERY_RULES_H
#define FLEET_WIN_DISCOVERY_RULES_H

#include "../common/EdrDataTypes.h"

bool group_policy_discovery(const ProcessEvent &process_event, Event &rule_event);
bool account_discovery_domain_account(const ProcessEvent &process_event, Event &rule_event);
bool browser_information_discovery(const ProcessEvent &process_event, Event &rule_event);
bool system_information_discovery(const ProcessEvent &process_event, Event &rule_event);
bool domain_trust_discovery(const ProcessEvent &process_event, Event &rule_event);
bool file_and_directory_discovery(const ProcessEvent &process_event, Event &rule_event);
bool win_security_software_discovery(const ProcessEvent &process_event, Event &rule_event);
//
bool discover_system_language_chcp(const ProcessEvent &process_event, Event &rule_event);
bool use_get_net_tcp_connection(const ProcessEvent &process_event, Event &rule_event);
bool suspicious_get_local_groups_information(const ProcessEvent &process_event, Event &rule_event);
bool directorySearcher_powershell_exploitation(const ProcessEvent &process_event, Event &rule_event);
bool active_directory_computers_enumeration_with_get_AdComputer(const ProcessEvent &process_event, Event &rule_event);
bool active_directory_computers_enumeration_with_get_AdGroup(const ProcessEvent &process_event, Event &rule_event);
bool security_software_discovery_by_powershell(const ProcessEvent &process_event, Event &rule_event);
bool windows_software_discovery_powershell(const ProcessEvent &process_event, Event &rule_event);
bool password_policy_discovery_with_Get_AdDefaultDomainPasswordPolicy(const ProcessEvent &process_event, Event &rule_event);
bool suspicious_powershell_get_current_user(const ProcessEvent &process_event, Event &rule_event);
bool suspicious_process_discovery_with_get_process(const ProcessEvent &process_event, Event &rule_event);
bool powershell_suspicious_win32_PnPEntity(const ProcessEvent &process_event, Event &rule_event);
#endif
