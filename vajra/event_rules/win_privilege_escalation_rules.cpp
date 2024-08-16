
#include "win_privilege_escalation_rules.h"
#include <sstream>


//T1053.005 - Scheduled Task/Job: Scheduled Task
// select * from win_process_events where path like '%schtasks%' and cmdline like '%schtasks%';

// bool scheduled_task(const ProcessEvent &process_event, Event &rule_event)
// {
//     std::string cmdline = process_event.entry.cmdline;

// 	if(std::transform(cmdline.begin(), cmdline.end(), cmdline.begin(), ::tolower).find("schtasks") != std::string::npos && (std::transform(cmdline.begin(), cmdline.end(), cmdline.begin(), ::tolower).find("create") != std::string::npos) && (std::transform(cmdline.begin(), cmdline.end(), cmdline.begin(), ::tolower).find("system32") != std::string::npos || std::transform(cmdline.begin(), cmdline.end(), cmdline.begin(), ::tolower).find("winsxs") != std::string::npos)) // || chrome_extension.entry.permissions.find("://*/"))
// 	{
// 		std::stringstream ss;

//         	ss << "[" << process_event.entry.file_path << " " << process_event.entry.name << ")] Possibly malicious";
//         	rule_event.metadata = ss.str();

//         	return true;	
// 	}
	
// 	return false;
// }

bool scheduled_task(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(process_event.entry.path.find("schtasks")!= std::string::npos && cmdline.find("schtasks") != std::string::npos && cmdline.find("create") != std::string::npos && cmdline.find("system") != std::string::npos && cmdline.find("calc.exe") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Win Task Scheduler may be possibly abused to run malicious tasks";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

//T1543.003 - Create or Modify System Process: Windows Service 
//select * from win_process_events where cmdline like'%sc%' and cmdline like '%config%' and cmdline like '%binPath%';

bool create_or_modify_windows_process(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.cmdline.find("sc") != std::string::npos && process_event.entry.cmdline.find("config") != std::string::npos && process_event.entry.cmdline.find("binPath") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Win Services Scheduler may be modified for malicious purposes";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}


//T1546.011 - Event Triggered Execution: Application Shimming
// select * from win_process_events where parent_path like '%cmd.exe%' and path like '%sdbinst%' and cmdline like '%sdbinst%';

bool application_shimming(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.cmdline.find("sdbinst") != std::string::npos && process_event.entry.path.find("sdbinst.exe") != std::string::npos && process_event.entry.parent_path.find("cmd.exe") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss <<"Privileges escalated by using shims";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}


// T1546.007 - Event Triggered Execution: Netsh Helper DLL
//select * from win_process_events where path like '%netsh.exe%' and cmdline like '%add helper%';

bool netsh_helper_dll(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.path.find("netsh.exe") != std::string::npos && process_event.entry.cmdline.find("add") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Malicious content triggered usign netsh";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}



// T1547.001 - Boot or Logon Autostart Execution: Registry Run Keys / Startup Folder
// select * from win_process_events where (path like '%reg.exe%' and cmdline like '%HKCU%' and cmdline like '%REG%' and cmdline like '%ADD%') or (path like '%cmd.exe%' and cmdline like '%HKCU%' and cmdline like '%REG%' and cmdline like '%ADD%');


// bool registry_run_keys(const ProcessEvent &process_event, Event &rule_event)
// {

// 	std::string cmdline = process_event.entry.cmdline;


// 	if(std::transform(cmdline.begin(), cmdline.end(), cmdline.begin(), ::tolower).find("hkcu") != std::string::npos && std::transform(cmdline.begin(), cmdline.end(), cmdline.begin(), ::tolower).find("run") != std::string::npos && std::transform(cmdline.begin(), cmdline.end(), cmdline.begin(), ::tolower).find("reg") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
// 	{
// 		std::stringstream ss;

//         	ss << "[" << process_event.entry.file_path << " " << process_event.entry.name << ")] Possibly malicious";
//         	rule_event.metadata = ss.str();

//         	return true;	
// 	}
	
// 	return false;
// }

bool registry_run_keys(const ProcessEvent &process_event, Event &rule_event)
{

	std::string cmdline = process_event.entry.cmdline;


	if((cmdline.find("HKCU") != std::string::npos && cmdline.find("REG") != std::string::npos && cmdline.find("ADD") != std::string::npos && process_event.entry.path.find("reg.exe") != std::string::npos)
	 ||  cmdline.find("HKCU") != std::string::npos && cmdline.find("REG") != std::string::npos && cmdline.find("cmd.exe") != std::string::npos && cmdline.find("ADD") != std::string::npos && process_event.entry.path.find("cmd.exe") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Malicious program is possibly added to the startup folder";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

// T1134.005 - Access Token Manipulation: SID-History Injection
// select * from win_process_events where cmdline like '%mimikatz%' and cmdline like '%sid::add%' and cmdline like '%sid::patch%';

// bool sid_history_injection(const ProcessEvent &process_event, Event &rule_event)
// {
// 	std::string cmdline = process_event.entry.cmdline;

// 	if(std::transform(cmdline.begin(), cmdline.end(), cmdline.begin(), ::tolower).find("mimikatz") != std::string::npos && std::transform(cmdline.begin(), cmdline.end(), cmdline.begin(), ::tolower).find("sid::add") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
// 	{
// 		std::stringstream ss;

//         	ss << "[" << process_event.entry.file_path << " " << process_event.entry.name << ")] Possibly malicious";
//         	rule_event.metadata = ss.str();

//         	return true;	
// 	}
	
// 	return false;l
// }

bool sid_history_injection(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("mimikatz") != std::string::npos && cmdline.find("sid::add") != std::string::npos && cmdline.find("sid::patch")) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Privileges Escalated through SID-History Injection";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}




//T1574.001 - Hijack Execution Flow: DLL Search Order Hijacking 
// select * from win_process_events where cmdline like '%amsi.dll%' and cmdline like '%copy%' and cmdline like '%APPDATA%'; 

bool dll_search_order_hijacking(const ProcessEvent &process_event,Event &rule_event)
{

	std::string cmdline  = process_event.entry.cmdline;

	if(cmdline.find("amsi.dll")!= std::string::npos && cmdline.find("copy") != std::string::npos && cmdline.find("APPDATA") != std::string::npos){
		std::stringstream ss;

        	ss << "DLL Search Order Hijacking";
        	rule_event.metadata = ss.str();

        	return true;
	}
	return false;
}

//T1055.003 - Process Injection: Thread execution Hijacking
// select * from win_process_events where parent_path like '%powershell.exe%' and cmdline like '%notepad%' and cmdline like '%InjectContext%';

bool thread_execution_hijacking(const ProcessEvent &process_event,Event &rule_event){
	std::string cmdline = process_event.entry.cmdline;
	if(process_event.entry.parent_path.find("powershell.exe") != std::string::npos && cmdline.find("notepad") != std::string::npos && cmdline.find("InjectContext") != std::string::npos){
		std::stringstream ss;

        	ss << "Thread execution Hijacking";
        	rule_event.metadata = ss.str();

        	return true;
	}
	return false;
} 

// T1134.004 - Access Token Manipulation: Parent PID Spoofing
// select * from win_process_events where cmdline like '%iexplore.exe%' and cmdline like '%calc.dll%';

bool pid_parent_spoofing(const ProcessEvent &process_event,Event &rule_event){
	std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("iexplore.exe") != std::string::npos && cmdline.find("calc.dll") != std::string::npos){
		std::stringstream ss;

        	ss << "Parent PID Spoofing";
        	rule_event.metadata = ss.str();

        	return true;
	}
	return false;
} 


// T1218.003 - System Binary Proxy Execution: CMSTP
// select * from win_process_events where (path like '%cmd.exe%' and cmdline like '%cmstp.exe%') or (path like '%cmstp.exe%' and cmdline like '%cmstp%');

bool cmstp(const ProcessEvent &process_event,Event &rule_event){
	std::string cmdline = process_event.entry.cmdline;
	std::string path = process_event.entry.path;
	if((path.find("cmd.exe") != std::string::npos && cmdline.find("cmstp.exe") != std::string::npos) || (path.find("cmstp.exe") != std::string::npos && cmdline.find("cmstp.exe") != std::string::npos)){
		std::stringstream ss;

        	ss << "Execution of malicious code using cmstp.exe";
        	rule_event.metadata = ss.str();

        	return true;
	}
	return false;
}

// T1546.008 - Event Triggered Execution: Accessibility Features
// select * from win_process_events where (cmdline like '%icacls.exe%' and cmdline like '%icacls%' and cmdline like '%osk.exe%') or (path like '%takeown%' and cmdline like '%sethc.exe%');

bool event_triggered_execution_accessibility_features(const ProcessEvent &process_event,Event &rule_event)
{
	std::string path = process_event.entry.path;
	std::string cmdline = process_event.entry.cmdline;

	if((path.find("icacls.exe") != std::string::npos && cmdline.find("icacls") != std::string::npos && cmdline.find("osk.exe") != std::string::npos) 
	   || (path.find("takeown.exe") != std::string::npos && cmdline.find("sethc.exe") != std::string::npos))
	   {
			 std::stringstream ss;
			 ss<< "Malicious content triggered by accessibility features for escalating privileges";
			 rule_event.metadata = ss.str();
			 return true;
	   }
	   return false;
}

//T1547.005 - Boot or Logon Autostart Execution: Security Support Provider
// select * from win_process_events where path like '%powershell.exe%' and cmdline like '%System\\CurrentControlSet\\Control\\Lsa%'

bool security_support_provider(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.path.find("powershell") != std::string::npos && process_event.entry.cmdline.find("System\\CurrentControlSet\\Control\\Lsa") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Security support providers absued to execute DLLs";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1484.001 - Domain Policy Modification: Group Policy Modification
// select * from win_process_events where (cmdline like '%reg%' and cmdline like '%add%') and (cmdline like '%GroupPolicyRefreshTime%' or cmdline like '%GroupPolicyRefreshTimeOffset%' or cmdline like '%GroupPolicyRefreshTimeDC%');

bool group_policy_modification(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if((cmdline.find("reg") != std::string::npos && cmdline.find("add") != std::string::npos) && cmdline.find("GroupPolicyRefreshTime") !=std::string::npos || cmdline.find("GroupPolicyRefreshTimeOffset") != std::string::npos 
	|| cmdline.find("GroupPolicyRefreshTimeDC") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Group Polict objects modified for escalating privileges on domain";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;

}

//T1546.012 - Event Triggered Execution: Image File Execution Options Injection
// select * from win_process_events where cmdline '%SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options%';

bool image_file_execution_options_injection (const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options") != std::string::npos){
		std::stringstream ss;
		ss<< "Priveleges elevated by using IFEO debugger";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1547.004 - Boot or Logon Autostart Execution: Winlogon Helper DLL
// select * from win_process_events where cmdline like '%Set-ItemProperty%' and cmdline like '%Microsoft\\Windows NT\\CurrentVersion\\Winlogon%' and (cmdline like '%Shell%' or cmdline like '%Userinit%');

bool winlogon_helper_dll(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("Set-ItemProperty") != std::string::npos && cmdline.find("Microsoft\\Windows NT\\CurrentVersion\\Winlogon") != std::string::npos && (cmdline.find("Shell") != std::string::npos || cmdline.find("Userinit") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Winlogon features absued to execute DLls ";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;

}

//T1546.015 - Event Triggered Execution: Component Object Model Hijacking, COM Hijacking - InprocServer32
// select * from win_process_events where cmdline like '%HKCU:\\SOFTWARE\\Classes\\CLSID%' and cmdline like '%InprocServer32%';

bool com_hijacking_inprocserver32(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("HKCU:\\SOFTWARE\\Classes\\CLSID") != std::string::npos && process_event.entry.cmdline.find("InprocServer32") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Registry values under InprocServer32 created";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

//T1547.004 - Boot or Logon Autostart Execution: Winlogon Helper DLL, Winlogon Notify Key Logon Persistence
// select * from win_process_events where cmdline like '%Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Notify%' and cmdline like '%.dll%';

bool winlogon_notify_key_logon(const ProcessEvent &process_event , Event &rule_event)
{

	std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Notify") != std::string::npos && cmdline.find(".dll") != std::string::npos){
		std::stringstream ss;
		ss<< "Winlogon Notify key set to execute a notification package DLL at logon ";
		rule_event.metadata =  ss.str();
		return true;
	}
	return false;
}


// T1546.003 - Powershell WMI Persistence
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%New-CimInstance%' and cmdline like '%-Namespace root/subscription%' and cmdline like '%-ClassName __EventFilter%' and cmdline like '%-Property%' and cmdline like '%-ClassName CommandLineEventConsumer%');

bool powershell_WMI_persistence(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-CimInstance") != std::string::npos && cmdline.find("-Namespace root/subscription") != std::string::npos && cmdline.find("-ClassName __EventFilter") != std::string::npos && cmdline.find("-Property") != std::string::npos && cmdline.find("-ClassName CommandLineEventConsumer") != std::string::npos)
	{
		std::stringstream ss;

		ss << "Execution of malicious content triggered by a Windows Management Instrumentation (WMI) event subscription";
		rule_event.metadata = ss.str();

		return true;	
	}
	
	return false;
}