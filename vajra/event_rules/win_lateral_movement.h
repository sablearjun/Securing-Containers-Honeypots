#ifndef FLEET_WIN_LATERAL_MOVEMENT_RULES_H
#define FLEET_WIN_LATERAL_MOVEMENT_RULES_H

#include "../common/EdrDataTypes.h"

bool enable_windows_remote_management(const ProcessEvent &process_event, Event &rule_event);
bool execute_invoke_command_on_remote_host(const ProcessEvent &process_event, Event &rule_event);
bool suspicious_new_PSDrive_to_admin_share(const ProcessEvent &process_event, Event &rule_event);

#endif 
