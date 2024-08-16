#ifndef FLEET_WIN_COMMAND_CONTROL_RULES_H
#define FLEET_WIN_COMMAND_CONTROL_RULES_H

#include "../common/EdrDataTypes.h"

bool data_encoding_standard_encoding(const ProcessEvent &process_event, Event &rule_event);
bool dns_large_query_volume(const ProcessEvent &process_event, Event &rule_event);
bool remote_access_software(const ProcessEvent &process_event, Event &rule_event);
bool code_executed_via_excel(const ProcessEvent &process_event, Event &rule_event);
bool win_ingress_tool_transfer(const ProcessEvent &process_event, Event &rule_event);
bool non_standard_port(const ProcessEvent &process_event, Event &rule_event);
bool win_ingress_tool_transfer_certutil(const ProcessEvent &process_event, Event &rule_event);
bool internal_proxy_portproxy_regkey(const ProcessEvent &process_event, Event &rule_event);
bool win_ingress_tool_transfer_curl_download(const ProcessEvent &process_event, Event &rule_event);
bool netcat_the_powershell_version(const ProcessEvent &process_event, Event &rule_event);
bool change_user_agents_with_webRequest(const ProcessEvent &process_event, Event &rule_event);
bool suspicious_SSL_connection(const ProcessEvent &process_event, Event &rule_event);
#endif