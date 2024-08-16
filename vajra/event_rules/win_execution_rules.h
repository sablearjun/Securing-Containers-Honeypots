#ifndef FLEET_WIN_EXECUTION_RULES_H
#define FLEET_WIN_EXECUTION_RULES_H

#include "../common/EdrDataTypes.h"

bool scheduled_task_job(const ProcessEvent &win_process_event, Event &rule_event);
bool native_api(const ProcessEvent &win_process_event, Event &rule_event);
bool command_and_scripting_interpreter(const ProcessEvent &win_process_event, Event &rule_event);
bool service_execution(const ProcessEvent &win_process_event, Event &rule_event);
bool command_scripting_interpreter_javascript(const ProcessEvent &process_event, Event &rule_event);
bool command_scripting_interpreter_powershell(const ProcessEvent &process_event, Event &rule_event);
bool command_scripting_interpreter_win_command_shell(const ProcessEvent &process_event, Event &rule_event);
bool command_scripting_interpreter_visual_basic(const ProcessEvent &process_event, Event &rule_event);
bool malicious_file_user_execution(const ProcessEvent &process_event, Event &rule_event);
bool abuse_nslookup(const ProcessEvent &process_event, Event &rule_event);
bool delete_volume_shadow_copies_via_WMI_with_powershell(const ProcessEvent &process_event, Event &rule_event);
bool remote_powershell_session(const ProcessEvent &process_event, Event &rule_event);
bool suspicious_non_powerShell_WSMAN_COM_provider(const ProcessEvent &process_event, Event &rule_event);
bool powershell_create_local_user(const ProcessEvent &process_event, Event &rule_event);
bool powershell_MsXml_COM_object(const ProcessEvent &process_event, Event &rule_event);
bool powershell_remote_session_creation(const ProcessEvent &process_event, Event &rule_event);
bool use_remove_item_to_delete_file(const ProcessEvent &process_event, Event &rule_event);
bool powershell_execute_batch_script(const ProcessEvent &process_event, Event &rule_event);
bool powershell_XML_execute_command(const ProcessEvent &process_event, Event &rule_event);
#endif // FLEET_WIN_EXECUTION_RULES_H