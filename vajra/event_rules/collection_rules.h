#ifndef FLEET_COLLECTION_RULES_H
#define FLEET_COLLECTION_RULES_H

#include "../common/EdrDataTypes.h"

bool data_compressed_zip(const ProcessEvent& process_event, Event& rule_event);

bool data_compressed_zip_nix_gzip(const ProcessEvent& process_event, Event& rule_event);

bool data_compressed_zip_nix_tar(const ProcessEvent& process_event, Event& rule_event);

bool x_windows_capture(const ProcessEvent& process_event, Event& rule_event);

bool capture_linux_desktop_using_import_tool(const ProcessEvent& process_event, Event& rule_event);

bool terminal_input_capture_linux_with_pam_d(const ProcessEvent& process_event, Event& rule_event);

bool logging_bash_history_to_syslog(const ProcessEvent& process_event, Event& rule_event);

bool sshd_pam_keylogger(const ProcessEvent& process_event, Event& rule_event);

bool stage_data_from_discovery_sh(const ProcessEvent& process_event, Event& rule_event);

bool add_or_copy_content_to_clipboard_with_xclip(const ProcessEvent& process_event, Event& rule_event);

bool compressing_data_using_gzip_in_python(const ProcessEvent& process_event, Event& rule_event);

bool compressing_data_using_bz2_in_python(const ProcessEvent& process_event, Event& rule_event);

bool compressing_data_using_zipfile_in_python(const ProcessEvent& process_event, Event& rule_event);

#endif // FLEET_COLLECTION_RULES_H