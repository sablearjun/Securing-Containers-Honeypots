#include <sstream>
#include "win_persistence_rules.h"


// T1546.013 - Event Triggered Execution: PowerShell Profile
// Append malicious start-process cmdlet
// select * from win_process_events where cmdline like '%Add-Content%' and cmdline like '%Value%';

bool append_malicious_start_process_cmdlet(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("Add-Content") != std::string::npos && cmdline.find("Value") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Malicious content triggered by powershell profiles";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}





// T1133 - External Remote Services
// Running Chrome VPN Extensions via the Registry 2 vpn extension
// select * from win_process_events where cmdline like '%New-ItemProperty%' and cmdline like '%HKLM:\\Software\\Wow6432Node\\Google\\Chrome\\Extensions%';


bool running_chrome_vpn_extensions(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("HKLM:\\Software\\Wow6432Node\\Google\\Chrome\\Extensions") != std::string::npos && cmdline.find("New-ItemProperty") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "External remote services used to access within a network";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

// T1547.014 - Active Setup
// select * from win_process_events where cmdline like '%HKLM:\\SOFTWARE\\Microsoft\\Active Setup\\Installed Components%';

bool active_setup(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("HKLM:\\SOFTWARE\\Microsoft\\Active Setup\\Installed Components") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Registry Key added to active setup for gaining persistence";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

// T1547.003 - Time Providers
// Create a new time provider
// select * from win_process_events where cmdline like '%reg%' and cmdline like '%add%' and cmdline like '%HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\W32Time\\TimeProviders%';

bool time_providers_new(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("reg") != std::string::npos && (cmdline.find("add") != std::string::npos) && (cmdline.find("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\W32Time\\TimeProviders") != std::string::npos )) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Time providers may be abused to execute DLLs";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

// T1137.006 - Office Application Startup: Add-ins
// Persistent Code Execution Via Excel VBA Add-in File (XLAM)
// select * from win_process_events where cmdline like '%Microsoft\\Excel\\XLSTART%' and cmdline like '%.xlam%';

bool persistent_code_evecution_via_excel_vba_addin(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("Microsoft\\Excel\\XLSTART") != std::string::npos && cmdline.find(".xlam") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Excel VBA Add-ins used for obtaining persistence";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

// T1137.006 - Office Application Startup: Add-ins
// Persistent Code Execution Via Word Add-in File (WLL)
// select * from win_process_events where cmdline like '%Microsoft\\Word\\Startup%' and cmdline like '%.wll%';

bool persistent_code_execution_via_word_addin(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("Microsoft\\Word\\Startup") != std::string::npos && cmdline.find(".wll") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Word Add-in file used for obtaining persistence";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}



//T1547.010 - Boot or Logon Autostart Execution: Port Monitors
// select * from win_process_events where cmdline like '%reg%' and cmdline like '%add%' and cmdline like '%monitors\\ART%';

 bool port_monitors(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("reg") != std::string::npos && cmdline.find("add") != std::string::npos && cmdline.find("monitors\\ART") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Port Monitors may be used to execute DLLs for obtaining persistence";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}


// T1547.009 - Boot or Logon Autostart Execution: Shortcut Modification
// select * from win_process_events where cmdline like '%WScript.Shell%' and cmdline like '%CreateShorcut%';

bool shortcut_modification(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("WScript.Shell") != std::string::npos && cmdline.find("CreateShortcut") != std::string::npos) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Shortcuts created or modified for executing malicious programs";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

// T1574.008 - Hijack Execution Flow: Path Interception by Search Order Hijacking
// select * from win_process_events where (cmdline like '%csc.exe%' or path like '%csc.exe%') AND (cmdline like '%Microsoft\\WindowsApps\\Get-Variable.exe%');




bool search_order_hijacking(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if((cmdline.find("csc.exe") != std::string::npos || process_event.entry.path.find("csc.exe") != std::string::npos) && (cmdline.find("Microsoft\\WindowsApps\\Get-Variable.exe") != std::string::npos)) // || chrome_extension.entry.permissions.find("://*/"))
	{
		std::stringstream ss;

        	ss << "Malicious payload inserted into the search order used by other programs";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

//T1505.003 - Server Software Component: Web Shell
// select * from win_process_events where path like '%xcopy.exe%' and cmdline like '%xcopy%' and cmdline like '%wwwroot%';

bool server_software_component_web_shell(const ProcessEvent &process_event , Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	
	if(process_event.entry.path.find("xcopy.exe") != std::string::npos && process_event.entry.cmdline.find("xcopy") != std::string::npos && process_event.entry.cmdline.find("wwwroot") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Web shells abused for obtaining persistence access to the system";
		rule_event.metadata = ss.str();
		return true;

	}
	return false;
}

//T1546.015 - Event Triggered Execution: Component Object Model Hijacking
// select * from win_process_events where cmdline like '%New-Item%' and '%SOFTWARE\\Classes\\CLSID%';

bool component_object_model_hijacking(const ProcessEvent &process_event, Event &rule_event)
{

	if(process_event.entry.cmdline.find("New-Item") != std::string::npos && process_event.entry.cmdline.find("SOFTWARE\\Classes\\CLSID") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Malicious content triggered by hijacking component object model";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//	
// select * from win_process_events where cmdline like '%.hta=txtfile%' and cmdline like '%assoc%';

bool change_default_file_association(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find(".hta=txtfile") != std::string::npos && process_event.entry.cmdline.find("assoc") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Persistence established triggering malicious content by file type assosciation";
		rule_event.metadata =  ss.str();
		return true;
	}
	return false;
}

//T1037.001 -  Boot or Logon Initialization Scripts: Logon Script (Windows)
// select * from win_process_events where cmdline like '%UserInitMprLogonScript%';
bool win_logon_script(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("UserInitMprLogonScript") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Windows logon script executed to gain persistence";
		rule_event.metadata =  ss.str();
		return true;
	}
	return false;
}


//T1546.002 - Event Triggered Execution: Screensaver
//select * from win_process_events where path like '%reg.exe%' and cmdline like '%Control Panel\\Desktop%' and cmdline like '%ScreenSaveActive%';

bool event_triggered_exevution_screensaver(const ProcessEvent &process_event, Event &rule_event)
{	
	std::string cmdline = process_event.entry.cmdline;
	if(process_event.entry.path.find("reg.exe") != std::string::npos && cmdline.find("Control Panel\\Desktop") != std::string::npos && cmdline.find("ScreenSaveActive") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Malicious activity triggered while user was inactive";
		rule_event.metadata =  ss.str();
		return true;
	}
	return false;
}

//T1547.001 - Boot or Logon Autostart Execution: Registry Run Keys / Startup Folder, Add persistance via Recycle bin
// select * from win_process_events where cmdline like '%CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}\\shell\\open\\command%';

bool registry_run_keys_persistence_via_recycle_bin(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}\\shell\\open\\command") !=  std::string::npos){
		std::stringstream ss;
		ss << "Persistence obtained via recycle bin";
		rule_event.metadata =  ss.str();
		return true;

	}
	return false;
}

//T1547.005 - Boot or Logon Autostart Execution: Security Support Provider
// select * from win_process_events where cmdline like '%HKLM:\\System\\CurrentControlSet\\Control\\Lsa%' and cmdline like '%Security Packages%';

bool security_support_provider_ssp(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("HKLM:\\System\\CurrentControlSet\\Control\\Lsa") !=  std::string::npos && process_event.entry.cmdline.find("Security Packages") != std::string::npos){
		std::stringstream ss;
		ss << "SSP DLLs are loaded into Local Security Authority";
		rule_event.metadata =  ss.str();
		return true;

	}
	return false;
}

//T1548.002 - Abuse Elevation Control Mechanism: Bypass User Account Control, Bypass UAC using sdclt DelegateExecute
// select * from win_process_events where cmdline like '%shell\\open\\command%' and cmdline like '%DelegateExecute%';

bool bypass_uac_sdclt_delegate_execute(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("\\shell\\open\\command") != std::string::npos && process_event.entry.cmdline.find("DelegateExecute") != std::string::npos )
	{
		std::stringstream ss;
		ss << "User Account Control bypassed using DelegateExecute";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1548.002 - Abuse Elevation Control Mechanism: Bypass User Account Control, Bypass UAC using Event Viewer 
// select * from win_process_events where cmdline like '%mscfile\\shell\\open\\command%' and path like '%mmc.exe%';

bool bypass_uac_eventviewer(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("mscfile\\shell\\open\\command") != std::string::npos && process_event.entry.path.find("mmc.exe") != std::string::npos )
	{
		std::stringstream ss;
		ss << "User Account Control bypassed using Eventviewer";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1548.002 - Abuse Elevation Control Mechanism: Bypass User Account Control, Disable UAC using reg.exe
// select * from win_process_events where cmdline like '%\\CurrentVersion\\Policies\\System%' and cmdline like '%EnableLUA%';

bool bypass_uac_disable_reg(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("\\CurrentVersion\\Policies\\System") != std::string::npos && process_event.entry.cmdline.find("EnableLUA") != std::string::npos )
	{
		std::stringstream ss;
		ss << "User Account Control disabled using reg.exe";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1137 - Office Application Startup
//select * from win_process_events where cmdline like '%Outlook\\Security\\Level%';

bool office_applicatoin_startup(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("Outlook\\Security\\Level") != std::string::npos){
		std::stringstream ss;
		ss << "Microsoft Office applications used to gain persistence at startup";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1547.001 - Boot or Logon Autostart Execution: Registry Run Keys / Startup Folder, Reg Key Run/RunOnce
//select * from win_process_events where cmdline '%SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run%' or cmdline like '%SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx%';

bool boot_logon_autostart_execution_run_runonce(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run") !=  std::string::npos || process_event.entry.cmdline.find("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx") != std::string::npos){
		std::stringstream ss;
		ss << "";
		rule_event.metadata =  ss.str();
		return true;

	}
	return false;
}



// T1574.012 - Registry-Free Process Scope COR_PROFILER
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%$env:COR_ENABLE_PROFILING%' and cmdline like '%$env:COR_PROFILER%' and cmdline like '%$env:COR_PROFILER_PATH%');

bool registry_free_process_scope_COR_PROFILER(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("$env:COR_ENABLE_PROFILING") != std::string::npos && cmdline.find("$env:COR_PROFILER") != std::string::npos && cmdline.find("$env:COR_PROFILER_PATH") != std::string::npos)
	{
		std::stringstream ss;

		ss << "Execution flow of programs that load the .NET CLR has been hijacked";
		rule_event.metadata = ss.str();

		return true;
	}

	return false;
}

// T1574.011 - Service Registry Permissions Weakness Check
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%get-acl%' and cmdline like '%REGISTRY::HKLM\SYSTEM\CurrentControlSet\Services\%');

bool service_registry_permissions_weakness_check(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("powershell.exe") != std::string::npos && cmdline.find("get-acl") != std::string::npos && cmdline.find("REGISTRY::HKLM\\SYSTEM\\CurrentControlSet\\Services") != std::string::npos)
	{
		std::stringstream ss;

		ss << "Registry entries used by services have been hijacked";
		rule_event.metadata = ss.str();

		return true;	
	}
	
	return false;
}

// T1098 - Powershell LocalAccount Manipulation
// select * from win_process_events where (cmdline like '%powershell.exe%' and (cmdline like '%Get-LocalGroupMember%' or cmdline like '%-Group Administrators%' or cmdline like '%Disable-LocalUser%' or cmdline like '%Enable-LocalUser%' or cmdline like '%Get-LocalUser%' or cmdline like '%Set-LocalUser%' or cmdline like '%New-LocalUser%' or cmdline like '%Rename-LocalUser%' or cmdline like '%Remove-LocalUser%'));

bool powershell_localAccount_manipulation(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-LocalGroupMember") != std::string::npos && cmdline.find("-Group Administrators") != std::string::npos || cmdline.find("Disable-LocalUser") != std::string::npos || cmdline.find("Enable-LocalUser") != std::string::npos || cmdline.find("Get-LocalUser") != std::string::npos || cmdline.find("Set-LocalUser") != std::string::npos || cmdline.find("New-LocalUser") != std::string::npos || cmdline.find("Rename-LocalUser") != std::string::npos || cmdline.find("Remove-LocalUser") != std::string::npos)
	{
		std::stringstream ss;

		ss << "Local account has been manipulated";
		rule_event.metadata = ss.str();

		return true;	
	}
	
	return false;
}


// T1546.015 - Suspicious GetTypeFromCLSID ShellExecute
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%::GetTypeFromCLSID(%' and cmdline like '%.ShellExecute(%');

bool suspicious_GetTypeFromCLSID_shellexecute(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("powershell.exe") != std::string::npos && cmdline.find("::GetTypeFromCLSID(") != std::string::npos && cmdline.find(".ShellExecute(") != std::string::npos)
	{
		std::stringstream ss;

		ss << "Detected suspicious Powershell code that execute COM Objects";
		rule_event.metadata = ss.str();

		return true;	
	}
	
	return false;
}

// T1546.013 - Potential Persistence Via PowerShell User Profile Using Add-Content
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Add-Content%' and cmdline like '%Start-Process%');

bool potential_persistence_via_powershell_user_profile_using_add_content(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Add-Content") != std::string::npos && cmdline.find("Start-Process") != std::string::npos)
	{
		std::stringstream ss;

		ss << "Creation or modification of a PowerShell profile might be done";
		rule_event.metadata = ss.str();

		return true;	
	}
	
	return false;
}

// // T1547.004 - Winlogon Helper DLL
// // select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%CurrentVersion\Winlogon%' and (cmdline like '%New-Item%' or cmdline like '%Set-ItemProperty%'));

// bool winlogon_helper_DLL(const ProcessEvent &process_event, Event &rule_event)
// {
// 	std::string cmdline = process_event.entry.cmdline;

// 	if(cmdline.find("powershell.exe") != std::string::npos && cmdline.find("CurrentVersion\\Winlogon") != std::string::npos && (cmdline.find("Set-ItemProperty") != std::string::npos || cmdline.find("New-Item") != std::string::npos))
// 	{
// 		std::stringstream ss;

// 		ss << "Abuse of features of Winlogon to execute DLLs and/or executables when a user logs in";
// 		rule_event.metadata = ss.str();

// 		return true;	
// 	}
	
// 	return false;
// }