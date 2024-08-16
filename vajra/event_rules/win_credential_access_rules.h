#ifndef FLEET_WIN_CREDENTIAL_ACCESS_RULES_H
#define FLEET_WIN_CREDENTIAL_ACCESS_RULES_H

#include "../common/EdrDataTypes.h"



bool password_guessing(const ProcessEvent &process_event, Event &rule_event);
bool steal_web_session_cookie(const ProcessEvent &process_event, Event &rule_event);
bool registry_dump_of_sam_creds_secrets(const ProcessEvent &process_event, Event &rule_event);
bool packet_capture_windows_command_prompt(const ProcessEvent &process_event, Event &rule_event);
bool enumeration_for_credentials_in_registry(const ProcessEvent &process_event, Event &rule_event);
bool install_and_register_paassword_filter_dll(const ProcessEvent &process_event, Event &rule_event);
bool unsecured_credentials_gpp_passwords(const ProcessEvent &process_event, Event &rule_event);
bool lsass_memory_using_comsvcs_dll(const ProcessEvent &process_event, Event &rule_event);
bool password_spraying_kurbute(const ProcessEvent &process_event, Event &rule_event);
bool input_capture_credential_api_hooking(const ProcessEvent &process_event, Event &rule_event);
bool os_credential_dumping_dcsync(const ProcessEvent &process_event, Event &rule_event);
bool password_stores_windows_credentail_manager(const ProcessEvent &process_event, Event &rule_event);
bool os_credential_dumping(const ProcessEvent &process_event, Event &rule_event);
bool forced_authentication(const ProcessEvent &process_event, Event &rule_event);
bool kerberoasting_steal_or_forge_Kerberos_tickets(const ProcessEvent &process_event, Event &rule_event);
//
bool os_credential_dumping_esentutl(const ProcessEvent &process_event, Event &rule_event);
bool access_to_browser_login_data(const ProcessEvent &process_event, Event &rule_event);
bool powershell_install_a_DLL_in_system_directory(const ProcessEvent &process_event, Event &rule_event);
bool create_volume_shadow_copy_with_powershell(const ProcessEvent &process_event, Event &rule_event);
bool dump_credentials_from_windows_credential_manager_with_powershell(const ProcessEvent &process_event, Event &rule_event);
bool certificate_exported_via_powershell(const ProcessEvent &process_event, Event &rule_event);
bool suspicious_get_ADReplAccount(const ProcessEvent &process_event, Event &rule_event);
bool request_a_single_ticket_via_powershell(const ProcessEvent &process_event, Event &rule_event);
bool powershell_get_process_LSASS_in_scriptblock(const ProcessEvent &process_event, Event &rule_event);
bool suspicious_connection_to_remote_account(const ProcessEvent &process_event, Event &rule_event);

#endif 