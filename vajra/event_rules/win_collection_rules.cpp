#include "win_collection_rules.h"
#include<sstream>


// T1113 - Screen Capture
// select * from win_process_events where path like '%psr.exe' and cmdline like '%psr.exe%' and cmdline like '%sc%';

bool screen_capture(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.path.find("psr.exe") != std::string::npos && process_event.entry.cmdline.find("psr.exe") != std::string::npos
    && process_event.entry.cmdline.find("start") != std::string::npos && process_event.entry.cmdline.find("sc") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Screen capture used for gathering information";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

//T1074.001 - Data Staged: Local Data Staging

// select * from win_process_events where cmdline like '%Invoke-WebRequest%' and cmdline like '%discovery.bat%';

// bool data_staged(const WinFileEvent &win_file_event, Event &rule_event)
// {

// 	if(win_file_event.entry.target_path.find(".bat") != std::string::npos && win_file_event.entry.target_path.find("AppData") != std::string::npos && (win_file_event.entry.process_name.find("powershell") != std::string::npos || win_file_event.entry.process_name.find("cmd") != std::string::npos)) // || chrome_extension.entry.permissions.find("://*/"))
// 	{
// 		std::stringstream ss;

//         	ss << "[" << win_file_event.entry.target_path << ")] Possibly malicious";
			
//         	rule_event.metadata = ss.str();

//         	return true;	
// 	}
	
// 	return false;
// }

bool data_staged(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.cmdline.find("powershell.exe") != std::string::npos && process_event.entry.cmdline.find("Invoke-WebRequest") != std::string::npos && process_event.entry.cmdline.find("discovery.bat") != std::string::npos ) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Data staged for malicious purpose";
			
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

// T1119 - Automated Collection
// select path, parent_path, cmdline from win_process_events where action="PROC_CREATE" and cmdline like '%findstr%' and cmdline like '%temp%' and cmdline like '%copy%' limit 10;

bool automated_collection(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.cmdline.find("findstr") != std::string::npos && process_event.entry.cmdline.find("temp") != std::string::npos && process_event.entry.cmdline.find("copy") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Internal data collected using automated techniques for malicious purpose";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

//POWERSHELL!!
// bool automated_collection_2(const ProcessEvent &process_event, Event &rule_event)
// {

// 	if(process_event.entry.cmdline.find("Get-Children") != std::string::npos && process_event.entry.cmdline.find("Recurse") != std::string::npos && process_event.entry.cmdline.find("Copy-Item") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
// 	{
// 		std::stringstream ss;

//         	ss << "[" << process_event.entry.file_path << " " << process_event.entry.name << ")] Possibly malicious";
//         	rule_event.metadata = ss.str();

//         	return true;	
// 	}
	
// 	return false;
// }


// T1115 - Clipboard Data
// select * from win_process_events where action='PROC_CREATE' and cmdline like '%clip%' and cmdline like '%.txt%' and cmdline like '%echo%';

bool clipboard_data(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.cmdline.find("clip") != std::string::npos && process_event.entry.cmdline.find(".txt") != std::string::npos && process_event.entry.cmdline.find("echo") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Clipboard data accessed using clip.exe";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}


//T1560 - Archive Collected Data
// select * from win_process_events where cmdline like '%Compress-Archive%' and cmdline like '%Recurse%';


bool archive_collected_data(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.cmdline.find("Compress-Archive") != std::string::npos && process_event.entry.cmdline.find("Recurse") != std::string::npos && process_event.entry.cmdline.find(".zip") != std::string::npos  ) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Collected data is archived";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}


// T1125 - Video Capture
// select * from win_process_events where path like '%reg%' and cmdline like '%reg%' and cmdline like '%add%' and cmdline like '%webcam%';

bool video_capture(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.path.find("reg.exe") != std::string::npos && process_event.entry.cmdline.find("reg") != std::string::npos && process_event.entry.cmdline.find("add") != std::string::npos && process_event.entry.cmdline.find("webcam") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Video capture used for gathering information";

        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}


// T1039 - Data from Network Shared Drive
// select * from win_process_events where cmdline like '%Easter_Bunny%' and cmdline like '%Easter_egg%';
bool network_shared_drive_data(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.cmdline.find("cmd") != std::string::npos && process_event.entry.cmdline.find("Easter_Bunny") != std::string::npos && process_event.entry.cmdline.find("Easter_egg") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Sensitive data collected via shared networks for malicious purpose";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}


//T1123 - Audio capture
// select * from win_process_events where cmdline like '%WindowsAudioDevice-Powershell-Cmdlet%' or (cmdline like '%reg%' and cmdline like '%add%' and cmdline like '%microphone%');

bool audio_capture(const ProcessEvent &process_event, Event &rule_event)
{  
	std::string cmdline = process_event.entry.cmdline;
	if((cmdline.find("WindowsAudioDevice-Powershell-Cmdlet") != std::string::npos) || (cmdline.find("reg") != std::string::npos && cmdline.find("add") != std::string::npos && cmdline.find("microphone") != std::string::npos)){

		std::stringstream ss;
		ss << "Audio capture used for gathering information";
		rule_event.metadata = ss.str();

		return true;
	}
	return false;

}

//T1056.002 - Input Capture: GUI Input Capture
// select * from win_process_events where cmdline like '%UI.PromptForCredential%';

bool gui_input_capture(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("UI.PromptForCredential") != std::string::npos)
	{
		std::stringstream ss;
		ss << "OS GUI mimicked to obtain credentials";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1056.001 - Powershell Keylogging
// select * from win_process_events where path like '%powershell.exe' and cmdline like '%Get-Keystrokes%' and cmdline like '%Get-ProcAddress user32.dll GetAsyncKeyState%' and cmdline like '%Get-ProcAddress user32.dll GetForegroundWindow%';

bool powershell_keylogging(const ProcessEvent &process_event, Event &rule_event)
{

	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-Keystrokes") != std::string::npos && cmdline.find("Get-ProcAddress user32.dll GetAsyncKeyState") != std::string::npos && cmdline.find("Get-ProcAddress user32.dll GetForegroundWindow") != std::string::npos)
	{
		std::stringstream ss;

		ss << "Keystrokes are being captured";
		rule_event.metadata = ss.str();

		return true;
	}

	return false;
}

// T1114.001 - Powershell Local Email Collection
// select * from win_process_events where (path like '%powershell.exe' and (cmdline like '%Get-Inbox.ps1%' or cmdline like '%-comobject outlook.application%'));

bool powershell_local_email_collection(const ProcessEvent &process_event, Event &rule_event)
{

	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("Get-Inbox.ps1") != std::string::npos || cmdline.find("-comobject outlook.application") != std::string::npos))
	{
		std::stringstream ss;

		ss << "Local emails might be exposed";
		rule_event.metadata = ss.str();

		return true;
	}

	return false;
}

// T1119 - Recon Information for Export with PowerShell
// select * from win_process_events where (path like '%powershell.exe' and cmdline like '%Get-Service%' and cmdline like '%Get-ChildItem%' and cmdline like '%Get-Process%');

bool recon_information_for_export_with_powershell(const ProcessEvent &process_event, Event &rule_event)
{

	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-Service") != std::string::npos && cmdline.find("Get-ChildItem") != std::string::npos && cmdline.find("Get-Process") != std::string::npos)
	{
		std::stringstream ss;

		ss << "Automated techniques might be used for collecting internal data";
		rule_event.metadata = ss.str();

		return true;
	}

	return false;
}
