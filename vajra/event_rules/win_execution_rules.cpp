#include "win_execution_rules.h"
#include <sstream>

//EXECUTION



//T1053.005 - Scheduled Task/Job: Scheduled Task
//"select path, parent_path, cmdline from win_process_events where (cmdline like '%SCHTASKS%' and (cmdline like '%Create%' or cmdline like '%create%') and (cmdline like '%system32%' or cmdline like '%WinSxS%' or cmdline like '%program files%'));
//ii) schedule task : select * from win_process_events where action=""PROC_CREATE"" and path like ""%at.exe%"" and cmdline like '%at.exe"";"

bool scheduled_task_job(const ProcessEvent &win_process_event, Event &rule_event)
{
	if(win_process_event.entry.cmdline.find("SCHTASKS") != std::string::npos && (win_process_event.entry.cmdline.find("Create") != std::string::npos || win_process_event.entry.cmdline.find("create") != std::string::npos) && (win_process_event.entry.cmdline.find("system32") != std::string::npos || win_process_event.entry.cmdline.find("WinSxS") != std::string::npos || win_process_event.entry.cmdline.find("program files") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Win Task Scheduler may be possibly abused to run malicious tasks";
        	rule_event.metadata = ss.str();
        	return true;
	}
	
	if(win_process_event.entry.action == "PROC_CREATE" && win_process_event.entry.path.find("at.exe") != std::string::npos && win_process_event.entry.path.find("at.exe") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Win Task Scheduler may be possibly abused to run malicious tasks";
        	rule_event.metadata = ss.str();
        	return true;
	}
	return false;
}

//T1106: Native API
/*
i) select path, parent_path, cmdline from win_process_events where action="PROC_CREATE" and cmdline like '%cvtres%' and cmdline like '%NOLOGO%'  order by time desc limit 15;
ii) select path, parent_path, cmdline from win_process_events where parent_path like "%Microsoft.NET\Framework64%csc.exe" and cmdline like "%cvtres.exe /NOLOGO /READONLY%";
iv) select path, parent_path, cmdline from win_process_events where parent_path like "%services.exe" and cmdline like "%.\pipe%";
*/

bool native_api(const ProcessEvent &win_process_event, Event &rule_event)
{
	if(win_process_event.entry.action == "PROC_CREATE" && win_process_event.entry.cmdline.find("cvtres") != std::string::npos && win_process_event.entry.cmdline.find("NOLOGO") != std::string::npos)
	{
		std::stringstream ss;
		ss << "...";
        	rule_event.metadata = ss.str();
        	return true;
	}
	
	if((win_process_event.entry.parent_path.find("Microsoft.NET\\Framework64")!=std::string::npos && win_process_event.entry.parent_path.find("csc.exe")!=std::string::npos) && win_process_event.entry.cmdline.find("cvtres.exe /NOLOGO /READONLY")!=std::string::npos)
	{
		std::stringstream ss;
		ss << "...";
        	rule_event.metadata = ss.str();
        	return true;
	}
	return false;
}

//T1059: Command and Scripting Interpreter
// select * from win_process_events where action="PROC_CREATE" and path like "%powershell.exe%" and parent_path like "%powershell.exe%" and cmdline like "%IEX%Net.WebClient%";

bool command_and_scripting_interpreter(const ProcessEvent &win_process_event, Event &rule_event)
{
	if(win_process_event.entry.action == "PROC_CREATE" && win_process_event.entry.path.find("powershell.exe") != std::string::npos && win_process_event.entry.parent_path.find("powershell.exe") != std::string::npos && win_process_event.entry.cmdline.find("IEX") != std::string::npos && win_process_event.entry.cmdline.find("Net.WebClient") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Powershell Command may be abused";
        	rule_event.metadata = ss.str();
        	return true;
	}
	
	return false;
}

//T1569.002: System Services: Service Execution
// select * from win_process_events where cmdline like '%sc.exe create%' and cmdline like '%binPath%' and cmdline like '%ARTService%';
bool service_execution(const ProcessEvent &win_process_event, Event &rule_event)
{
	if(win_process_event.entry.cmdline.find("sc.exe create")!=std::string::npos && win_process_event.entry.cmdline.find("binPath")!=std::string::npos && win_process_event.entry.cmdline.find("ARTService")!=std::string::npos )
	{
		rule_event.metadata = "New service created";
		return true;
	}
	return false;
}

//T1059.007 - Command and Scripting Interpreter: JavaScript
// select * from win_process_events where path like '%wscript%' and cmdline like '%wscript%' and cmdline like '%.js%';

bool command_scripting_interpreter_javascript(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.path.find("wscript")!=std::string::npos && process_event.entry.cmdline.find("wscript")!=std::string::npos && process_event.entry.cmdline.find(".js")!=std::string::npos)
	{
		std::stringstream ss;
		ss << "Javascript executed for malicious purpose";
        rule_event.metadata = ss.str();
        return true;
	}
	return false;
}

// T1059.001 - Command and Scripting Interpreter: PowerShell
// select * from win_process_events where (cmdline like '%mshta.exe%' and cmdline like '%javascript%') or (cmdline like '%SharpHound.ps1%' and cmdline '%write-host%');

bool command_scripting_interpreter_powershell(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if((cmdline.find("mshta.exe") != std::string::npos && cmdline.find("javascript") != std::string::npos) 
	|| (cmdline.find("SharpHound.ps1") != std::string::npos && cmdline.find("write-host") != std::string::npos )){

		std::stringstream ss;
		ss << "Powershell commands and scripts used for execution";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}



// T1059.003 - Command and Scripting Interpreter: Windows Command Shell
// select * from win_process_events where (path like '%cmd.exe%' or path like '%powershell.exe%') and (cmdline like '%do start%' and cmdline like '%powershell%'); 

bool command_scripting_interpreter_win_command_shell(const ProcessEvent &process_event, Event &rule_event)
{ 
	std::string path = process_event.entry.path;
	std::string cmdline = process_event.entry.cmdline;

	if((path.find("cmd.exe") != std::string::npos || path.find("powershell.exe")) && (cmdline.find("do start") != std::string::npos && cmdline.find("powershell.exe") ))
	{
		std::stringstream ss;
		ss << "Windows command shell used for execution ";
		rule_event.metadata = ss.str();
		return true;

	}
	return false;
}

//T1059.005 - Command and Scripting Interpreter: Visual Basic
// select * from win_process_events where path like '%powershell.exe%' and cmdline like '%cscript%' and cmdline like '%powershell.exe%';

bool command_scripting_interpreter_visual_basic(const ProcessEvent &process_event, Event &rule_event)
{ 
	std::string path = process_event.entry.path;
	std::string cmdline = process_event.entry.cmdline;

	if(path.find("powershell.exe") && cmdline.find("cscript") != std::string::npos && cmdline.find("powershell.exe"))
	{

		std::stringstream ss;
		ss << " Visual Basic used for execution";
		rule_event.metadata = ss.str();
		return true;

	}
	return false;
}

//T1204.002 - User Execution: Malicious File
// select * from win_process_events where path like '%cscript.exe%' and cmdline like '%WScript%' and cmdline like '%cscript%';

bool malicious_file_user_execution(const ProcessEvent &process_event, Event &rule_event)
{
	std::string path = process_event.entry.path;
	std::string cmdline = process_event.entry.cmdline;
	if(path.find("cscript.exe") != std::string::npos && cmdline.find("WScript") != std::string::npos && cmdline.find("cscript") != std::string::npos)
	{
		std::stringstream ss;
		ss<< " ";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}



// T1059.001 - Abuse Nslookup with DNS Records
// select * from win_process_events where (path like '%nslookup.exe%' and (cmdline like '%nslookup.exe%' || cmdline like '%-q=txt%') and (path like '%powershell.exe%' and (cmdline like '%powershell.exe%')

bool abuse_nslookup(const ProcessEvent &process_event, Event &rule_event)
{
	std::string path = process_event.entry.path;
	std::string cmdline = process_event.entry.cmdline;

	if ((path.find("nslookup.exe") != std::string::npos && (cmdline.find("nslookup.exe") != std::string::npos || cmdline.find("-q=txt") != std::string::npos)) && (path.find("powershell.exe") != std::string::npos && cmdline.find("powershell.exe") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Abused nslookup with DNS records";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1490 - Delete Volume Shadow Copies Via WMI With PowerShell
// select * from win_process_events where (cmdline like '%powershell.exe%' and (cmdline like '%Get-WmiObject%' and cmdline like '%Win32_Shadowcopy%') and (cmdline like '%Delete()%' or cmdline like '%Remove-WmiObject%'))

bool delete_volume_shadow_copies_via_WMI_with_powershell(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("Get-WmiObject") != std::string::npos && cmdline.find("Win32_Shadowcopy") != std::string::npos) && (cmdline.find("Delete()") != std::string::npos || cmdline.find("Remove-WmiObject") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Remote powershell created and may be used";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1021.006, T1059.001 - Remote PowerShell Session (PS Classic)
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Enable-PSRemoting%' and cmdline like '%whoami%' or (cmdline like '%wsmprovhost.exe%' and cmdline like '%ServerRemoteHost%')

bool remote_powershell_session(const ProcessEvent &process_event, Event &rule_event)
{	
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && ((cmdline.find("Enable-PSRemoting") != std::string::npos && cmdline.find("whoami") != std::string::npos) || (cmdline.find("wsmprovhost.exe") != std::string::npos && cmdline.find("ServerRemoteHost") != std::string::npos)))
	{
		std::stringstream ss;
		ss << "Remote powershell created and may be used";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1021.003 - Suspicious Non PowerShell WSMAN COM Provider
// select * from win_process_events where (cmdline like '%powershell.exe%' and (cmdline like '%WSMan%' (To be checked again)

bool suspicious_non_powerShell_WSMAN_COM_provider(const ProcessEvent &process_event, Event &rule_event)
{	
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("WSMan") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Remote powershell created and may be used";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1136.001 - PowerShell Create Local User
// select * from win_process_events where (cmdline like '%powershell.exe%' and (cmdline like '%WSMan%' (To be checked again)

bool powershell_create_local_user(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-LocalUser") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Local user created";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1059.001 - Powershell MsXml COM Object
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%New-Object%' and cmdline like '%-ComObject%' and cmdline like '%MsXml2%' and cmdline like '%XmlHttp%');

bool powershell_MsXml_COM_object(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-Object") != std::string::npos && cmdline.find("-ComObject") != std::string::npos && cmdline.find("MsXml2") != std::string::npos && cmdline.find("XmlHttp") != std::string::npos)
	{
		std::stringstream ss;
		ss << "PowerShell commands and scripts may be abused for execution";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1059.001 - PowerShell Remote Session Creation
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%New-PSSession%' and cmdline like '%-ComputerName%' and cmdline like '%Set-Content%' and cmdline like '%Get-Content%'); (to be checkedd)

bool powershell_remote_session_creation(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-PSSession") != std::string::npos && cmdline.find("-ComputerName") != std::string::npos && cmdline.find("Set-Content") != std::string::npos && cmdline.find("Get-Content") != std::string::npos)
	{
		std::stringstream ss;
		ss << "PowerShell remote session created";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1070.004 - Use Remove-Item to Delete File
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Remove-Item%' and cmdline like '%-path%');

bool use_remove_item_to_delete_file(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Remove-Item") != std::string::npos && cmdline.find("-path") != std::string::npos)
	{
		std::stringstream ss;
		ss << "File deleted using remove-item from temporary directory";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}


// T1059.003 - Powershell Execute Batch Script
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Start-Process%' and (cmdline like '%.bat%' || cmdline like '%.cmd%'));

bool powershell_execute_batch_script(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Start-Process") != std::string::npos && (cmdline.find(".bat") != std::string::npos || cmdline.find(".cmd") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Batch script executed in powershell";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1546.003 - Powershell XML Execute Command
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%New-Object%' and cmdline like '%System.Xml.XmlDocument%' && cmdline like '%.Load%');

bool powershell_XML_execute_command(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-Object") != std::string::npos && cmdline.find("System.Xml.XmlDocument") != std::string::npos && cmdline.find(".Load") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Powershell XML execute command";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

