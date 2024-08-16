#ifndef FLEET_WIN_COLLECTION_RULES_H
#define FLEET_WIN_COLLECTION_RULES_H

#include "../common/EdrDataTypes.h"

bool screen_capture(const ProcessEvent &process_event, Event &rule_event);
bool data_staged(const ProcessEvent &process_event, Event &rule_event);
bool automated_collection(const ProcessEvent &process_event, Event &rule_event);
// bool automated_collection_2(const ProcessEvent &process_event, Event &rule_event);
bool clipboard_data(const ProcessEvent &process_event, Event &rule_event);
bool archive_collected_data(const ProcessEvent &process_event, Event &rule_event);
bool video_capture(const ProcessEvent &process_event, Event &rule_event);
bool network_shared_drive_data(const ProcessEvent &process_event, Event &rule_event);
bool audio_capture(const ProcessEvent &process_event, Event &rule_event);
bool gui_input_capture(const ProcessEvent &process_event, Event &rule_event);
bool powershell_keylogging(const ProcessEvent &process_event, Event &rule_event);
bool powershell_local_email_collection(const ProcessEvent &process_event, Event &rule_event);
bool recon_information_for_export_with_powershell(const ProcessEvent &process_event, Event &rule_event);


#endif //FLEET_WIN_COLLECTION_RULES_H