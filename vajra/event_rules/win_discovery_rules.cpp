#include "win_discovery_rules.h"
#include<sstream>



//DISCOVERY

//T1615: Group Policy Discovery
//select path, parent_path, cmdline  from win_process_events where cmdline like '%gpresult%' or cmdline like '%Get-DomainGPO%' or cmdline like '%GPOAudit%' or cmdline like '%GPORemoteAccessPolicy%' or cmdline like '%Get-GPO%';

bool group_policy_discovery(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("gpresult")!=std::string::npos || cmdline.find("Get-DomainGPO")!=std::string::npos || cmdline.find("GPOAudit")!=std::string::npos || cmdline.find("GPORemoteAccessPolicy")!=std::string::npos || cmdline.find("Get-GPO")!=std::string::npos)
	{
		rule_event.metadata = "Group policies may be discovered";
		return true;
	}
	return false;
}

//T1087.002 -  Account Discovery: Domain Account
// select * from win_process_events where (path like '%powershell%' or path like '%net.exe%') and (cmdline like '%net user%' or cmdline like '%net  group%');

bool account_discovery_domain_account(const ProcessEvent &process_event, Event &rule_event)
{	
	std::string path = process_event.entry.path;
	std::string cmdline = process_event.entry.cmdline;

	if((path.find("powershell.exe") != std::string::npos || path.find("net.exe") != std::string::npos) && (cmdline.find("net user") != std::string::npos || cmdline.find("net  group") != std::string::npos)){

		std::stringstream ss;
		ss << "Attempt to get listing of domain accounts";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1217 - Browser Information Discovery
// select * from win_process_events where (cmdline like '%where%' and cmdline like '%Bookmarks%') or (cmdline like '%where%' and cmdline like '%places.sqlite%');

bool browser_information_discovery(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if((cmdline.find("where") != std::string::npos && cmdline.find("Bookmarks") != std::string::npos) || (cmdline.find("where") != std::string::npos && cmdline.find("places.sqlite") != std::string::npos) )
	{
		std::stringstream ss;
		ss << "Information about browsers enumerated";
		rule_event.metadata = ss.str();
		return true;	
	}
	return false;
}

//T1082 - System Information Discovery
// select * from win_process_events where path like '%reg.exe%' and cmdline like '%reg%' and cmdline like '%query%' and cmdline like '%SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum%';
bool system_information_discovery(const ProcessEvent &process_event, Event &rule_event)
{	
	std::string cmdline = process_event.entry.cmdline;
	std::string path = process_event.entry.path;

	if(path.find("reg.exe") != std::string::npos && cmdline.find("reg") != std::string::npos && cmdline.find("query") != std::string::npos && cmdline.find("SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum") != std::string::npos)
	{
		std::stringstream ss;
		ss<< "Information about the system compromised";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;

}

//T1482 - Domain Trust Discovery
// select * from win_process_events where (cmdline like '%forest%' and cmdline like '%userdomain%') or cmdline like '%Get-ForestTrust%' or cmdline like '%Get-DomainTrust%';

bool domain_trust_discovery(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if((cmdline.find("forest") != std::string::npos && cmdline.find("userdomain") != std::string::npos) || cmdline.find("Get-ForestTrust") != std::string::npos || cmdline.find("Get-DomainTrust") != std::string::npos)
	{
		std::stringstream ss;
		ss<< "Attempt to gather information using domain trust relationships";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1083 - File and Directory Discovery
// select * from win_process_events where cmdline like '%folderarray%' and cmdline like '%Get-ChildItem%';

bool file_and_directory_discovery(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("folderarray") != std::string::npos && cmdline.find("Get-ChildItem") != std::string::npos)
	{
		std::stringstream ss;
		ss<<"";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1518.001 - Software Discovery: Security Software Discovery
// select * from win_process_events where (path like '%findstr%' and cmdline like '%findstr%' and cmdline like '%virus%') or (path like '%cmd.exe%' and cmdline like '%netsh.exe%' and cmdline like '%tasklist%' and cmdline like '%findstr%' and cmdline like '%virus%');
bool win_security_software_discovery(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	std::string path = process_event.entry.path;

	if((path.find("findstr.exe") != std::string::npos && cmdline.find("findstr") != std::string::npos && cmdline.find("virus") != std::string::npos) 
	|| (path.find("cmd.exe") != std::string::npos && cmdline.find("netsh.exe") != std::string::npos && cmdline.find("tasklist.exe") != std::string::npos && cmdline.find("findstr") != std::string::npos && cmdline.find("virus") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Information from security software discovery used for malicious purpose";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
} 

//T1614.001 - System Location Discovery: System Language Discovery, Discover System Language with chcp

bool discover_system_language_chcp(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	std::string path = process_event.entry.path;

	if(path.find("chcp.com") != std::string::npos && cmdline.find("chcp") != std::string::npos){
		std::stringstream ss;
		ss <<"System language identified using chcp";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;

}



// T1049 - Use Get-NetTCPConnection
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%Get-NetTCPConnection%'

bool use_get_net_tcp_connection(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("gpresult") != std::string::npos || cmdline.find("Get-DomainGPO") != std::string::npos || cmdline.find("GPOAudit") != std::string::npos || cmdline.find("GPORemoteAccessPolicy") != std::string::npos || cmdline.find("Get-GPO") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Group policies may be discovered";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1069.001 - Suspicious Get Local Groups Information
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%get-localgroup%' and cmdline like '%Get-LocalGroupMember%'
bool suspicious_get_local_groups_information(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("get-localgroup") != std::string::npos && cmdline.find("Get-LocalGroupMember") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Local groups information may have been discovered";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1018 - DirectorySearcher Powershell Exploitation
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%New-Object%' and cmdline like '%System.DirectoryServices.DirectorySearcher%'
bool directorySearcher_powershell_exploitation(const ProcessEvent &process_event, Event &rule_event)
{	
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("get-localgroup") != std::string::npos && cmdline.find("Get-LocalGroupMember") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Local groups information may have been discovered";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1018 - Active Directory Computers Enumeration with Get-AdComputer
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%Get-AdComputer%' and cmdline like '%-Filter%'
bool active_directory_computers_enumeration_with_get_AdComputer(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-AdComputer") != std::string::npos && cmdline.find("-Filter") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Enumerate Computers within Active Directory.";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1069.002 - Active Directory Computers Enumeration with Get-AdGroup
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%Get-AdComputer%' and cmdline like '%-Filter%'
bool active_directory_computers_enumeration_with_get_AdGroup(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-AdGroup") != std::string::npos && cmdline.find("-Filter") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Enumerate Groups within Active Directory";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1518.001 - Security Software Discovery by Powershell
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%get-process%' and cmdline like '%.Description%' and cmdline like '%-like%' and (cmdline like '%*virus*%' or cmdline like '%*defender*%' or cmdline like '%*cylance*%' or cmdline like '%*mc*%'));
bool security_software_discovery_by_powershell(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("get-process") != std::string::npos && cmdline.find(".Description") != std::string::npos && cmdline.find("-like") != std::string::npos && (cmdline.find("*virus*") != std::string::npos || cmdline.find("*defender*") != std::string::npos || cmdline.find("*cylance*") != std::string::npos || cmdline.find("*mc*") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Listing of security software, configurations, defensive tools, and sensors that are installed on a system might be extracted";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}


// T1518 - Detected Windows Software Discovery - PowerShell
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%Get-ItemProperty%' and cmdline like '%:\\SOFTWARE%' and cmdline like '%select-object%' and cmdline like '%format-table%');
bool windows_software_discovery_powershell(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-ItemProperty") != std::string::npos && cmdline.find(":\\SOFTWARE") != std::string::npos && cmdline.find("select-object") != std::string::npos && cmdline.find("format-table") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Detected windows software";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1201 - Password Policy Discovery With Get-AdDefaultDomainPasswordPolicy
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%get-addefaultdomainpasswordpolicy%');
bool password_policy_discovery_with_Get_AdDefaultDomainPasswordPolicy(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("get-addefaultdomainpasswordpolicy") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Detailed information about the password policy is exposed";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1033 - Suspicious PowerShell Get Current User
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%[System.Environment]::UserName%' and cmdline like '%[System.Security.Principal.WindowsIdentity]::GetCurrent()%');
bool suspicious_powershell_get_current_user(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("[System.Environment]::UserName") != std::string::npos && cmdline.find("[System.Security.Principal.WindowsIdentity]::GetCurrent()") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Primary user has been discovered";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1057 - Suspicious Process Discovery With Get-Process
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Get-Process%');
bool suspicious_process_discovery_with_get_process(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-Process") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Powershell discovered with get-process, information about running processes on the system might be exposed";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1120 - Powershell Suspicious Win32_PnPEntity
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Get-WMIObject%' and cmdline like '%Win32_PnPEntity%');
bool powershell_suspicious_win32_PnPEntity(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-WMIObject") != std::string::npos &&  cmdline.find("Win32_PnPEntity") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Information about attached peripheral devices and components connected to a computer system might be exposed";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}