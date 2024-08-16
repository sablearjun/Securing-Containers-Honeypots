#ifndef FLEET_CREDENTIAL_ACCESS_RULES_H
#define FLEET_CREDENTIAL_ACCESS_RULES_H

#include "../common/EdrDataTypes.h"

bool malicious_pam_rules(const ProcessEvent& process_event, Event& rule_event);

bool input_capture_keylogging(const ProcessEvent& process_event, Event& rule_event);

bool sshd_pam_keylogger_keylogging(const ProcessEvent& process_event, Event& rule_event);

bool dump_credentials_from_web_browsers(const ProcessEvent& process_event, Event& rule_event);

bool discover_private_ssh_keys(const ProcessEvent& process_event, Event& rule_event);

bool copy_private_ssh_keys_with_cp(const ProcessEvent& process_event, Event& rule_event);

bool extract_passwords_with_grep(const ProcessEvent& process_event, Event& rule_event);

bool access_etc_shadow_passwd(const ProcessEvent& process_event, Event& rule_event);

#endif // FLEET_COMMAND_AND_CONTROL_RULES_H