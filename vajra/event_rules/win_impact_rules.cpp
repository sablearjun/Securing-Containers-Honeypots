//IMPACT
#include "win_impact_rules.h"
#include <sstream>
//T1489: Service Stop
/*
"i) select path, parent_path, cmdline from win_process_events where (parent_path like '%powershell.exe%' or parent_path like '%cmd.exe%') and path like '%sc.exe%' and cmdline like '%sc.exe% stop %';
ii) select path, parent_path, cmdline from win_process_events where parent_path like '%net.exe%'  and path like '%net1.exe%' and cmdline like '%net1% stop %';
iii) select path, parent_path, cmdline from win_process_events where (parent_path like '%powershell.exe%' or parent_path like '%cmd.exe%') and path like '%taskkill.exe' and cmdline like '%taskkill.exe%/im%';"
*/

bool service_stop_one(const ProcessEvent &win_process_event, Event &rule_event)
{
	std::string parent_path = win_process_event.entry.parent_path;
	std::string path = win_process_event.entry.path;
	std::string cmdline = win_process_event.entry.cmdline;
	if((parent_path.find("powershell.exe")!=std::string::npos || parent_path.find("cmd.exe")!=std::string::npos) && path.find("sc.exe")!=std::string::npos && cmdline.find("sc.exe")!=std::string::npos && cmdline.find("stop")!=std::string::npos)
	{
		rule_event.metadata = "Attempt to stop a service";
		return true;	
	}
    return false;
}

bool service_stop_two(const ProcessEvent &win_process_event, Event &rule_event)
{
	std::string parent_path = win_process_event.entry.parent_path;
	std::string path = win_process_event.entry.path;
	std::string cmdline = win_process_event.entry.cmdline;
	if(parent_path.find("net.exe")!=std::string::npos && path.find("net1.exe")!=std::string::npos && cmdline.find("net1")!=std::string::npos && cmdline.find("stop")!=std::string::npos)
	{
		rule_event.metadata = "Attempt to stop a service";
		return true;	
	}
    return false;
}

bool service_stop_three(const ProcessEvent &win_process_event, Event &rule_event)
{
	std::string parent_path = win_process_event.entry.parent_path;
	std::string path = win_process_event.entry.path;
	std::string cmdline = win_process_event.entry.cmdline;
	if((parent_path.find("powershell.exe")!=std::string::npos || parent_path.find("cmd.exe")!=std::string::npos) && path.find("taskkill.exe")!=std::string::npos && cmdline.find("taskkill.exe")!=std::string::npos && cmdline.find("im")!=std::string::npos)
	{
		rule_event.metadata = "Attempt to stop a service";
		return true;	
	}	
    return false;
}

//T1529: System Shutdown/Reboot
//select * from win_process_events where cmdline path like '%shutdown.exe%' and cmdline like '%shutdown%';

// bool system_shutdown_reboot(const ProcessEvent &win_process_event, Event &rule_event)
// {
// 	if(win_process_event.entry.cmdline.find("shutdown")!=std::string::npos)
// 	{
// 		rule_event.metadata = "Attempt to shut down a system using powershell";
// 		return true;	
// 	}	
// }

//T1491: Defacement: Internal Defacement
/*
i) select * from win_process_events where cmdline like '%namespace Win32{%DllImport%SystemParametersInfo%';
ii) select * from win_process_events where cmdline like '%HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System -Name LegalNoticeCaption%' or cmdline like '%HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System -Name LegalNoticeText -Value $newLegalNoticeText%';
*/

bool internal_defacement_one(const ProcessEvent &win_process_event, Event &rule_event)
{
	if(win_process_event.entry.cmdline.find("namespace Win32")!=std::string::npos && win_process_event.entry.cmdline.find("DllImport")!=std::string::npos && win_process_event.entry.cmdline.find("SystemParametersInfo")!=std::string::npos)
	{
		rule_event.metadata = "Defacement: Internal Defacement";
		return true;
	}
	return false;
}

bool internal_defacement_two(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("HKLM:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System -Name LegalNoticeCaption")!=std::string::npos && process_event.entry.cmdline.find("HKLM:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System -Name LegalNoticeText -Value $newLegalNoticeText")!=std::string::npos)
	{
		rule_event.metadata = "Defacement: Internal Defacement";
		return true;
	}
	return false;
}

//T1486 - Data Encrypted for Impact
// select * from win_process_events where cmdline like '%GnuPG\\bin\\gpg.exe%';

bool data_encrypted_impact(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("GnuPG\\bin\\gpg.exe") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Data encrypted to to interrupt availability of system";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1490 - Inhibit System Recovery, Disable System Restore Through Registry

bool inhibit_system_recovery_registry(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("Policies\\Microsoft\\Windows NT\\SystemRestore") != std::string::npos && cmdline.find("DisableConfig") != std::string::npos && cmdline.find("DisableSR") != std::string::npos){
		std::stringstream ss;
		ss<< "System restore disabled through registry" ;
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1531 - Remove Account From Domain Admin Group
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Get-ADUser%' and cmdline like '%Remove-ADGroupMember%' and cmdline like '%-Identity%' and cmdline like '%-Members%');

bool remove_account_from_domain_admin_group(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-ADUser") != std::string::npos && cmdline.find("Remove-ADGroupMember") != std::string::npos && cmdline.find("-Identity") != std::string::npos && cmdline.find("-Members") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Account removed from domain admin group";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1491.001 - Replace Desktop Wallpaper by Powershell
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Get-ItemProperty%' and cmdline like '%Registry::%' and cmdline like '%HKEY_CURRENT_USER\Control Panel\Desktop\%' and cmdline like '%c%');

bool replace_desktop_wallpaper_by_powershell(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-ItemProperty") != std::string::npos && cmdline.find("Registry::") != std::string::npos && cmdline.find("HKEY_CURRENT_USER\\Control Panel\\Desktop\\") != std::string::npos && cmdline.find("WallPaper") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Account removed from domain admin group";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}
