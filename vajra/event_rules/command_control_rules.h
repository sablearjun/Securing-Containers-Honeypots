#ifndef FLEET_COMMAND_CONTROL_RULES_H
#define FLEET_COMMAND_CONTROL_RULES_H

#include "../common/EdrDataTypes.h"

bool non_standard_port_command_control(const ProcessEvent& process_event, Event& rule_event);

#endif // FLEET_COMMAND_CONTROL_RULES_H