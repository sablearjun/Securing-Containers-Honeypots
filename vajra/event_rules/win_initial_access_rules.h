#ifndef FLEET_WIN_INITIAL_ACCESS_RULES_H
#define FLEET_WIN_INITIAL_ACCESS_RULES_H

#include "../common/EdrDataTypes.h"

bool scheduled_tasks(const ProcessEvent &process_event, Event &rule_event);
// bool win_hardware_additions(const ProcessEvent &process_event, Event &rule_event);
bool spearphishing_attack(const ProcessEvent &process_event, Event &rule_event);
bool external_remote_services(const ProcessEvent &process_event, Event &rule_event);

#endif