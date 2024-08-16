#ifndef FLEET_EXFILTRATION_RULES_H
#define FLEET_EXFILTRATION_RULES_H

#include "../common/EdrDataTypes.h"

bool exfiltrate_data_https_using_curl(const ProcessEvent& process_event, Event& rule_event);
bool exfiltrate_data_alternate_protocol(const ProcessEvent& process_event, Event& rule_event);

#endif // FLEET_EXFILTRATION_RULES_H