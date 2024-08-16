#ifndef FLEET_WIN_IMPACT_RULES_H
#define FLEET_WIN_IMPACT_RULES_H

#include "../common/EdrDataTypes.h"

bool service_stop_one(const ProcessEvent &win_process_event, Event &rule_event);
bool service_stop_two(const ProcessEvent &win_process_event, Event &rule_event);
bool service_stop_three(const ProcessEvent &win_process_event, Event &rule_event);
bool internal_defacement_one(const ProcessEvent &win_process_event, Event &rule_event);
bool internal_defacement_two(const ProcessEvent &win_process_event, Event &rule_event);
bool data_encrypted_impact(const ProcessEvent &process_event, Event &rule_event);
bool remove_account_from_domain_admin_group(const ProcessEvent &process_event, Event &rule_event);
bool replace_desktop_wallpaper_by_powershell(const ProcessEvent &process_event, Event &rule_event);
#endif // FLEET_WIN_IMPACT_RULES_H