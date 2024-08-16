#ifndef WIN_PERSISTENCE_RULES_H
#define WIN_PERSISTENCE_RULES_H

#include "../common/EdrDataTypes.h"

bool append_malicious_start_process_cmdlet(const ProcessEvent &process_event, Event &rule_event);
bool running_chrome_vpn_extensions(const ProcessEvent &process_event, Event &rule_event);
bool active_setup(const ProcessEvent &process_event, Event &rule_event);
bool time_providers_new(const ProcessEvent &process_event, Event &rule_event);
bool persistent_code_evecution_via_excel_vba_addin(const ProcessEvent &process_event, Event &rule_event);
bool persistent_code_execution_via_word_addin(const ProcessEvent &process_event, Event &rule_event);
bool port_monitors(const ProcessEvent &process_event, Event &rule_event);
bool shortcut_modification(const ProcessEvent &process_event, Event &rule_event);
bool search_order_hijacking(const ProcessEvent &process_event, Event &rule_event);
bool server_software_component_web_shell(const ProcessEvent &process_event , Event &rule_event);
bool component_object_model_hijacking(const ProcessEvent &process_event, Event &rule_event);
bool change_default_file_association(const ProcessEvent &process_event, Event &rule_event);
//
bool win_logon_script(const ProcessEvent &process_event, Event &rule_event);
bool event_triggered_exevution_screensaver(const ProcessEvent &process_event, Event &rule_event);
bool registry_run_keys_persistence_via_recycle_bin(const ProcessEvent &process_event, Event &rule_event);
bool security_support_provider_ssp(const ProcessEvent &process_event, Event &rule_event);
bool bypass_uac_sdclt_delegate_execute(const ProcessEvent &process_event, Event &rule_event);
bool bypass_uac_eventviewer(const ProcessEvent &process_event, Event &rule_event);
bool bypass_uac_disable_reg(const ProcessEvent &process_event, Event &rule_event);
bool office_applicatoin_startup(const ProcessEvent &process_event, Event &rule_event);
bool boot_logon_autostart_execution_run_runonce(const ProcessEvent &process_event, Event &rule_event);
bool registry_free_process_scope_COR_PROFILER(const ProcessEvent &process_event, Event &rule_event);
bool service_registry_permissions_weakness_check(const ProcessEvent &process_event, Event &rule_event);
bool powershell_localAccount_manipulation(const ProcessEvent &process_event, Event &rule_event);
bool suspicious_GetTypeFromCLSID_shellexecute(const ProcessEvent &process_event, Event &rule_event);
bool potential_persistence_via_powershell_user_profile_using_add_content(const ProcessEvent &process_event, Event &rule_event);
// bool winlogon_helper_DLL(const ProcessEvent &process_event, Event &rule_event);

#endif
