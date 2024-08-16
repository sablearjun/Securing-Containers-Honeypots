#include "win_command_control_rules.h"
#include<sstream>



// T1132.001 - Data Encoding: Standard Encoding
// select * from win_process_events where cmdline like '%bxor%';

bool data_encoding_standard_encoding(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("bxor") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Malicious encoded data detected";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}




// T1071.004 - Application Layer Protocol: DNS
// DNS Large Query Volume
// select * from win_process_events where action like '%PROC_CREATE%' and cmdline like '%TXT%' and cmdline like '%QueryType%' and cmdline like '%C2Interval%';

bool dns_large_query_volume(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(process_event.entry.action.find("PROC_CREATE")  && cmdline.find("TXT") != std::string::npos && cmdline.find("QueryType") != std::string::npos && cmdline.find("C2Interval") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Communication done for malicious purpose using DNS application layer protocol";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}







// T1219 - Remote Access Software
// All combined
// select pid, path, parent_path, cmdline from win_process_events where path like '%\GoToAssist%' or path like '%ScreenConnect%' or path like '%ammyy.exe%' or path like '%RemotePC.exe%' or path like '%UltraViewer%' limit 10;

bool remote_access_software(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("GoToAssist.exe") != std::string::npos || cmdline.find("screenconnect") != std::string::npos || cmdline.find("ammyy.exe") != std::string::npos || cmdline.find("RemotePC.exe") != std::string::npos || cmdline.find("UltraViewer") != std::string::npos || cmdline.find("anydesk") != std::string::npos || cmdline.find("logmein") != std::string::npos || cmdline.find("teamviewer") != std::string::npos)
	{ 
		std::stringstream ss;

        	ss << "Remote access software downloaded to estalish connection for malicious purpose";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}






// T1572 - Protocol Tunneling
// select * from win_process_events where cmdline like '%-le%' and cmdline like '%Invoke-WebRequest%' and cmdline like '%BasicParsing%';

bool code_executed_via_excel(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("-le") != std::string::npos && cmdline.find("Invoke-WebRequest") != std::string::npos && cmdline.find("BasicParsing") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Protocol tunneling done for malicious purpose";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}


// T1105 - Ingress Tool Transfer
// select * from win_process_events where (cmdline like '%certutil%' and cmdline like'%datePath%') or (cmdline like '%cmd.exe%' and cmdline like '%nimgrab.exe%');

bool win_ingress_tool_transfer(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if((cmdline.find("certutil") != std::string::npos && cmdline.find("datePath") != std::string::npos) || (cmdline.find("cmd") != std::string::npos && cmdline.find("nimgrab.exe") != std::string::npos) )
	{
		std::stringstream ss;
		ss << "Tools or files transferred from external system for malicious purpose";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1571 - Non-Standard Port
// select * from win_process_events where cmdline like '%Test-NetConnection%' and cmdline '%port%';

bool non_standard_port(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("Test-NetConnection") != std::string::npos && cmdline.find("port") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Communication via dissimilar port and protocol";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;

}

//T1105 - Ingress Tool Transfer: certutil download (urlcache)
bool win_ingress_tool_transfer_certutil(const ProcessEvent &process_event, Event &rule_event)
{
	std::string path = process_event.entry.path;

	if(path.find("certutil.exe") != std::string::npos){
		std::stringstream ss;
		ss << "Tools or files transferred from external system using certutil for malicious purpose";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1090.001 - Proxy: Internal Proxy, portproxy reg key
bool internal_proxy_portproxy_regkey(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("CurrentControlSet\\Services\\PortProxy\\v4tov4") != std::string::npos){
		std::stringstream ss;
		ss << "Registry key added to set up proxy at the system";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1105 - Ingress Tool Transfer: Curl Download File
bool win_ingress_tool_transfer_curl_download(const ProcessEvent &process_event, Event &rule_event)
{
	std::string path = process_event.entry.path;

	if(path.find("curl.exe") != std::string::npos && process_event.entry.cmdline.find("Curl") != std::string::npos){
		std::stringstream ss;
		ss << "curl.exe used to download a remote DLL for malicious purpose";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1095 - Netcat The Powershell Version
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%powercat%' and cmdline like '%powercat.ps1%';

bool netcat_the_powershell_version(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("powershell.exe") != std::string::npos && cmdline.find("powercat") != std::string::npos && cmdline.find("powercat.ps1") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Netcat powershell version";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

// T1071.001 - Change User Agents with WebRequest
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%Invoke-WebRequest%' and cmdline like '%-UserAgent%';

bool change_user_agents_with_webRequest(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Invoke-WebRequest") != std::string::npos && cmdline.find("-UserAgent") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "User agents changed with web request";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

// T1573 - Suspicious SSL Connection
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%New-Object%' and cmdline like '%System.Net.Security.SslStream%' and cmdline like '%Net.Security.RemoteCertificateValidationCallback%' and cmdline like '%.AuthenticateAsClient%');

bool suspicious_SSL_connection(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-Object") != std::string::npos && cmdline.find("System.Net.Security.SslStream") != std::string::npos && cmdline.find("Net.Security.RemoteCertificateValidationCallback") != std::string::npos && cmdline.find(".AuthenticateAsClient") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Suspicious SSL connection"; //To be checked
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}