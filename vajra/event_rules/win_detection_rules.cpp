#include "win_detection_rules.h"
#include <sstream>

bool get_download_tmp(const ProcessEvent &win_process_event, Event &rule_event)
{
 //   if ((process_event.entry.path.find("C:\\Windows\\System32")) && (process_event.entry.cmdline.find("C:\\Windows\\System32\\smartscreen")))
   if(win_process_event.entry.cmdline == "consent.exe 16140 422 000002B4D4A31570")
    {

        std::stringstream ss;

        ss << "[" << win_process_event.entry.path << " (pid: " << win_process_event.entry.pid << ")] downloading file in /tmp/";

        rule_event.metadata = ss.str();

        return true;
        
    }
    return false;
}
