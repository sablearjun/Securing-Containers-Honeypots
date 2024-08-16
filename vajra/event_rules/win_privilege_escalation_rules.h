#ifndef FLEET_WIN_PRIVILEGE_ESCALATION_RULES_H
#define FLEET_WIN_PRIVILEGE_ESCALATION_RULES_H

#include "../common/EdrDataTypes.h"


bool scheduled_task(const ProcessEvent &process_event, Event &rule_event);
bool create_or_modify_windows_process(const ProcessEvent &process_event, Event &rule_event);
bool application_shimming(const ProcessEvent &process_event, Event &rule_event);
bool netsh_helper_dll(const ProcessEvent &process_event, Event &rule_event);
bool registry_run_keys(const ProcessEvent &process_event, Event &rule_event);
bool sid_history_injection(const ProcessEvent &process_event, Event &rule_event);
bool dll_search_order_hijacking(const ProcessEvent &process_event,Event &rule_event);
bool thread_execution_hijacking(const ProcessEvent &process_event,Event &rule_event);
bool pid_parent_spoofing(const ProcessEvent &process_event,Event &rule_event);
bool cmstp(const ProcessEvent &process_event,Event &rule_event);
bool event_triggered_execution_accessibility_features(const ProcessEvent &process_event,Event &rule_event);
bool security_support_provider(const ProcessEvent &process_event, Event &rule_event);
bool group_policy_modification(const ProcessEvent &process_event, Event &rule_event);
bool image_file_execution_options_injection (const ProcessEvent &process_event, Event &rule_event);
bool winlogon_helper_dll(const ProcessEvent &process_event, Event &rule_event);
//
bool com_hijacking_inprocserver32(const ProcessEvent &process_event, Event &rule_event);
bool winlogon_notify_key_logon(const ProcessEvent &process_event , Event &rule_event);
bool powershell_WMI_persistence(const ProcessEvent &process_event, Event &rule_event);
#endif //FLEET_WIN_PRIVILEGE_ESCALATION_RULES_H