#include "win_lateral_movement.h"
#include <sstream>

// Lateral Movement

// T1021.006 - Enable Windows Remote Management
// select * from win_process_events where (cmdline like '%powershell.exe%' && cmdline like '%Enable-PSRemoting%');

bool enable_windows_remote_management(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Enable-PSRemoting") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Windows Remote Management is enabled";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1021.006 - Execute Invoke-command on Remote Host
// select * from win_process_events where cmdline like '%powershell.exe%' && cmdline like '%Enable-PSRemoting%' and cmdline like '%Invoke-Command %' and cmdline like '%-ComputerName%');

bool execute_invoke_command_on_remote_host(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Enable-PSRemoting") != std::string::npos && cmdline.find("Invoke-Command") != std::string::npos && cmdline.find("-ComputerName") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Invoke command on remote host is executed";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1021.002 - Suspicious New-PSDrive to Admin Share
// select * from win_process_events where cmdline like '%powershell.exe%' && cmdline like '%New-PSDrive%' and cmdline like '%-psprovider%' and cmdline like '%filesystem%' and cmdline like '%-root%');

bool suspicious_new_PSDrive_to_admin_share(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-PSDrive") != std::string::npos && cmdline.find("-psprovider") != std::string::npos && cmdline.find("filesystem") != std::string::npos && cmdline.find("-root") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Actions might be performed on behalf of the logged-on user";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}