#include<sstream>
#include "win_initial_access_rules.h"



//T1195: Supply Chain Compromise
//select * from win_process_events where (cmdline like '%ExplorerSync%' and cmdline like '%temp%') or (path like '%schtasks.exe%' and cmdline like '%ExplorerSync%');

bool scheduled_tasks(const ProcessEvent &process_event, Event &rule_event)
{
	if((process_event.entry.cmdline.find("ExplorerSync") != std::string::npos && process_event.entry.cmdline.find("temp") != std::string::npos) 
    || (process_event.entry.path.find("schtasks.exe") != std::string::npos && process_event.entry.cmdline.find("ExplorerSync") != std::string::npos))
	{
		std::stringstream ss;

        	ss << "Malicious Process Scheduled";
        	rule_event.metadata = ss.str();

        	return true;
	}
	return false;

}

// //T1200: Hardware additions
// // select * from file where (path like 'D:%' or path like 'E:%' or path like 'F:%' or path like 'G:%' or path like 'H:%') and filename like '%AUTORUN%';
// //need to set query for file table in appropriate drive
// bool win_hardware_additions(const ProcessEvent &process_event, Event &rule_event)
// {
// 	if(process_event.entry.filename.find("AUTORUN") != std::string::npos)
// 	{
// 		std::stringstream ss;
// 		ss << "Autorun file detected in USB";
//         	rule_event.metadata = ss.str();
//         	return true;
// 	}
// 	return false;
// }


//T1566: Phishing: Spearphishing Attachment
//select * from win_process_events where cmdline like '%url%' and cmdline like '%OutFile%' and cmdline like '%Invoke-WebRequest%';

bool spearphishing_attack(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("url") != std::string::npos && cmdline.find("OutFile") != std::string::npos && cmdline.find("Invoke-WebRequest") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Spearphising emails with malicious attachment detected";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

//T1133 - External Remote Services, Running Chrome VPN Extensions via the Registry 2 vpn extension


bool external_remote_services(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("Wow6432Node\\Google\\Chrome\\Extensions") != std::string::npos && cmdline.find("fcfhplploccackoneaefokcmbjfbkenj") != std::string::npos && cmdline.find("fdcgdnkidjaadafnichfpabhfomcebme") != std::string::npos  ){
		std::stringstream ss;

        	ss << "Chrome VPN Extensions used for gaining access";
        	rule_event.metadata = ss.str();

        	return true;

	}
	return false;
}
