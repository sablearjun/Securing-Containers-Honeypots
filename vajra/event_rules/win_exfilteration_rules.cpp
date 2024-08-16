#include "win_exfilteration_rules.h"
#include <sstream>

//Exfiltration

//T1020 - Automated Exfiltration
//select * from win_process_events where cmdline like '%Invoke-WebRequest% -Method Put % -InFile %';
bool automated_exfiltration(const ProcessEvent &process_event, Event &rule_event)
{
	if(process_event.entry.cmdline.find("Invoke-WebRequest")!=std::string::npos && process_event.entry.cmdline.find("-Method Put")!=std::string::npos && process_event.entry.cmdline.find("-InFile")!=std::string::npos)
	{
		std::stringstream ss;
		ss << "Automated Exfiltration Detected";
        	rule_event.metadata = ss.str();
        	return true;
	}
	return false;

}

//T1048.002 - Exfiltration Over Alternative Protocol - Exfiltration Over Asymmetric Encrypted Non-C2 Protocol
//select * from win_process_events where path like '%curl.exe%' and (cmdline like '%Curl.exe%' or cmdline like '%curl%') and cmdline like '%file.io%';

bool exfiltration_over_encrypted_protocol(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("curl.exe")!=std::string::npos && (process_event.entry.cmdline.find("curl")!=std::string::npos || process_event.entry.cmdline.find("Curl.exe")!=std::string::npos) && process_event.entry.cmdline.find("file.io")!=std::string::npos)
    {
        rule_event.metadata = "Exfiltration over encrypted protocol";
        return true;        
    }
    return false;
}

//T1567.002 - Exfiltration Over Web Service: Exfiltration to Cloud Storage(using rclone tool)
//select * from win_process_events where cmdline like '%rclone%';

bool exfiltration_over_web_service(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("rclone")!=std::string::npos)
    {
        rule_event.metadata = "Exfiltration over web service";
        return true;        
    }
    return false;
}

// T1048.003 - PowerShell ICMP Exfiltration
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%New-Object%' and cmdline like '%System.Net.NetworkInformation.Ping%' and cmdline like '%.Send(%';
bool powershell_ICMP_exfiltration(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-Object") != std::string::npos && cmdline.find("System.Net.NetworkInformation.Ping") != std::string::npos && cmdline.find(".Send(") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Powershell is utilizing ping(ICMP) to exfiltrate notepad.exe to a remote address";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1048 - Powershell DNSExfiltration
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%Invoke-DNSExfiltrator%' and cmdline like '%-i%' and cmdline like '%-d%' and cmdline like '%-p%' and cmdline like '%-doh%'and cmdline like '%-t%';
bool powershell_DNSExfiltration(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Invoke-DNSExfiltrator") != std::string::npos && cmdline.find("-i") != std::string::npos && cmdline.find("-d") != std::string::npos && cmdline.find("-p") != std::string::npos && cmdline.find("-doh") != std::string::npos && cmdline.find("-t") != std::string::npos)
	{
		std::stringstream ss;
		ss << "DNS exfiltratration using powershell";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1048.00 - Powershell Exfiltration Over SMTP
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%Send-MailMessage%';
bool powershell_exfiltration_over_SMTP(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Send-MailMessage") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Powershell might've sent an email with attached file to a remote address";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

