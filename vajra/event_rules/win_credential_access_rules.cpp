#include "win_credential_access_rules.h"
#include<sstream>



// // Cached Credential Dump via Cmdkey

// bool cached_credential_dump_cmdkey(const ProcessEvent &process_event, Event &rule_event)
// {
//     std::string cmdline = process_event.entry.cmdline;

// 	if(cmdline.find("cmdkey /list") != std::string::npos)
// 	{
// 		std::stringstream ss;

//         	ss << "[" << process_event.entry.file_path << " " << process_event.entry.name << ")] Possibly malicious";
//         	rule_event.metadata = ss.str();

//         	return true;	
// 	}
	
// 	return false;
// }




// T1110.001 - Brute Force: Password Guessing
//select * from win_process_events where path like '%net.exe%' and cmdline like '%net%' and cmdline like '%/user:%' and cmdline like '%use%';

bool password_guessing(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(process_event.entry.path.find("net.exe") != std::string::npos && cmdline.find("net") != std::string::npos && cmdline.find("/user:") != std::string::npos && cmdline.find("use") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Password guessing done for account access";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}








// T1539 - Steal Web Session Cookie
// select * from win_process_events where path like '%powershell.exe%' and ((cmdline like '%firefox%' and cmdline like '%CookieDBLocation%') or (cmdline like '%chrome%' and cmdline like '%Cookies%'));

bool steal_web_session_cookie(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(process_event.entry.path.find("powershell.exe") != std::string::npos && ((cmdline.find("firefox") != std::string::npos && cmdline.find("CookieDBLocation") != std::string::npos) || (cmdline.find("chrome") != std::string::npos && cmdline.find("Cookies") != std::string::npos)))
	{
		std::stringstream ss;

        	ss << "Web cookies stolen for accessing web applications or internet services";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}








// T1003.002: Registry dump of SAM, creds, and secrets
// select * from win_process_events where cmdline like '%reg%' and cmdline like '%save%' and (cmdline like '%sam%' or cmdline like '%system%' or cmdline like '%security%');


bool registry_dump_of_sam_creds_secrets(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("reg") != std::string::npos && cmdline.find("save") != std::string::npos && (cmdline.find("sam") != std::string::npos || cmdline.find("system") != std::string::npos || cmdline.find("security") != std::string::npos))
	{
		std::stringstream ss;

        	ss << "Credential material extracted from Security Account Manager(SAM)";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}









// T1040: Packet Capture Windows Command Prompt
// select * from win_process_events where (path like '%cmd.exe%' and cmdline like '%pktmon.exe%) OR (path like '%PktMon.exe%' and cmdline like '%pktmon.exe% );

bool packet_capture_windows_command_prompt(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    std::string path = process_event.entry.path;
	if((path.find("cmd.exe") != std::string::npos && cmdline.find("pktmon.exe") != std::string::npos) || (path.find("PktMon.exe") != std::string::npos && cmdline.find("pktmon.exe") != std::string::npos) || (cmdline.find("netsh") != std::string::npos && cmdline.find("trace") != std::string::npos))
	{
		std::stringstream ss;

        	ss << "Network Traffic sniffed for caputring information";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}






// T1040: Windows internal packet capture

// bool windows_internal_packet_capture(const ProcessEvent &process_event, Event &rule_event)
// {
//     std::string cmdline = process_event.entry.cmdline;

// 	if(cmdline.find("netsh") != std::string::npos && cmdline.find("trace") != std::string::npos)
// 	{
// 		std::stringstream ss;

//         	ss << "[" << process_event.entry.file_path << " " << process_event.entry.name << ")] Possibly malicious";
//         	rule_event.metadata = ss.str();

//         	return true;	
// 	}
	
// 	return false;
// }








// T1552.002 - Unsecured Credentials: Credentials in Registry
// select * from win_process_events where cmdline like '%reg%' and cmdline like '%query%' and cmdline '%PuTTY%';


// bool enumeration_for_credentials_in_registry(const ProcessEvent &process_event, Event &rule_event)
// {
//     std::string cmdline = process_event.entry.cmdline;

// 	if(cmdline.find("reg") != std::string::npos && cmdline.find("query") != std::string::npos && cmdline.find("password") != std::string::npos)
// 	{
// 		std::stringstream ss;

//         	ss << "[" << process_event.entry.file_path << " " << process_event.entry.name << ")] Possibly malicious";
//         	rule_event.metadata = ss.str();

//         	return true;	
// 	}
	
// 	return false;
// }


bool enumeration_for_credentials_in_registry(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("reg") != std::string::npos && cmdline.find("query") != std::string::npos && cmdline.find("PuTTY") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Compromised Registry searched for stored credentials";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}







// T1552.002 - Unsecured Credentials: Credentials in Registry
// Enumeration for PuTTY Credentials in Registry

// bool putty_credentials_in_registry(const ProcessEvent &process_event, Event &rule_event)
// {
//     std::string cmdline = process_event.entry.cmdline;

// 	if(cmdline.find("reg") != std::string::npos && cmdline.find("query") != std::string::npos && cmdline.find("putty") != std::string::npos)
// 	{
// 		std::stringstream ss;

//         	ss << "[" << process_event.entry.file_path << " " << process_event.entry.name << ")] Possibly malicious";
//         	rule_event.metadata = ss.str();

//         	return true;	
// 	}
	
// 	return false;
// }




// T1556.002 - Modify Authentication Process: Password Filter DLL
//  select * from win_process_events where cmdline like '%reg%' and cmdline like '%export%' and cmdline '%lsa%';


bool install_and_register_paassword_filter_dll(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("reg") != std::string::npos && cmdline.find("export") != std::string::npos && cmdline.find("lsa") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Malicious password filters registered to DLLs for acquiring user credentials";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}








// T1552.006 - Unsecured Credentials: Group Policy Preferences
//  select * from win_process_events where cmdline like '%findstr%' and (cmdline like '%cpassword%' or cmdline '%sysvol%');

bool unsecured_credentials_gpp_passwords(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("findstr") != std::string::npos && (cmdline.find("cpassword") != std::string::npos || cmdline.find("sysvol") != std::string::npos))
	{
		std::stringstream ss;

        	ss << "Unsecured credentials found in Group Policy Preferences";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}






// T1003.001 - OS Credential Dumping: LSASS Memory
//  select * from win_process_events where cmdline like '%lsass%' and cmdline like '%.dmp%';


bool lsass_memory_using_comsvcs_dll(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("lsass") != std::string::npos && cmdline.find(".dmp") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "lsass.exe used for accessing credential material";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}








// T1110.003 - Brute Force: Password Spraying
//  select * from win_process_events where path like '%powershell.exe%' and cmdline like '%passwordspray%' and cmdline '%kerbrute%';


bool password_spraying_kurbute(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(process_event.entry.path.find("powershell.exe") != std::string::npos && cmdline.find("passwordspray") != std::string::npos && process_event.entry.cmdline.find("kerbrute") != std::string::npos )
	{
		std::stringstream ss;

        	ss << "Password spraying done for account access";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}







// T1056.004 - Input Capture: Credential API Hooking
//  select * from win_process_events where cmdline like '%mavinject%' and cmdline like '%.dll%';

bool input_capture_credential_api_hooking(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("mavinject") != std::string::npos && cmdline.find(".dll") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Windows API used for credential access";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}









// T1003.006 - OS Credential Dumping: DCSync
//  select * from win_process_events where cmdline like '%mimikatz%' and cmdline like '%lsadump%';


bool os_credential_dumping_dcsync(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(cmdline.find("mimikatz") != std::string::npos &&  cmdline.find("lsadump") != std::string::npos && cmdline.find("dcsync") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Attempt to access credentials using Windows Domain Controller's application";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}









// T1555.004 - Credentials from Password Stores: Windows Credential Manager
//  select * from win_process_events where path like '%VaultCmd.exe%' and cmdline like '%vaultcmd%';


bool password_stores_windows_credentail_manager(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

	if(process_event.entry.path.find("VaultCmd.exe") != std::string::npos && cmdline.find("vaultcmd") != std::string::npos)
	{
		std::stringstream ss;

        	ss << "Attempt to access credentials from the Windows Credential Manager";
        	rule_event.metadata = ss.str();

        	return true;	
	}
	
	return false;
}

//T1003 - OS Credential Dumping
// select * from win_process_events where (cmdline like '%rundll32.exe%' and cmdline like '%keymgr%' and cmdline like '%KRShowKeyMgr%') or (cmdline like '%Copy-Item%' and cmdline like '%NPPSPY.dll%');


bool os_credential_dumping(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if((cmdline.find("rundll32.exe") != std::string::npos && cmdline.find("keymgr") != std::string::npos && cmdline.find("KRShowKeyMgr") !=std::string::npos) 
    || (cmdline.find("Copy-Item") != std::string::npos && cmdline.find("NPPSPY.dll") != std::string::npos))

    {
            std::stringstream ss;

            ss << "Attempt to dump credentials for obtaining login credentials";
            rule_event.metadata = ss.str();
            return true;
    }
    return false;
}


//T1187 - Forced Authentication
// select * from win_process_events where (cmdline like '%PetiPotam.exe%' and cmdline like '%Write-Host%') or (cmdline like '%impersonate%' and cmdline like '%restore%');

bool forced_authentication(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if((cmdline.find("PetiPotam.exe") != std::string::npos && cmdline.find("Write-Host") != std::string::npos) || (cmdline.find("impersonate") != std::string::npos && cmdline.find("restore") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Credential material for authentication gathered by force";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1558.003 - Steal or Forge Kerberos Tickets: Kerberoasting
// select * from win_process_events where cmdline like '%Invoke-Rubeus%' and cmdline like '%kerberoast%';

bool kerberoasting_steal_or_forge_Kerberos_tickets(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;
	if(cmdline.find("Invoke-Rubeus") != std::string::npos && cmdline.find("kerberoast") != std::string::npos){
		std::stringstream ss;
		ss<< "Valid Kerberos ticket-granting ticket abused";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//T1003 - OS Credential Dumping: Security Account Manager, esentutl.exe SAM copy

bool os_credential_dumping_esentutl(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if(process_event.entry.path.find("esentutl.exe") != std::string::npos && cmdline.find("esentutl.exe") != std::string::npos && cmdline.find("SAM") != std::string::npos)

    {
            std::stringstream ss;

            ss << "SAM Hive copied using esentutl.exe utility";
            rule_event.metadata = ss.str();
            return true;
    }
    return false;
}



// Credential Access Rules

// T1555.003 - Simulating access to Browser Login Data
// select * from win_process_events where cmdline like '%powershell.exe%' and (cmdline like '%\Opera Software\Opera Stable\Login Data%' or cmdline like '%\Mozilla\Firefox\Profiles%' or cmdline like '%\Microsoft\Edge\User Data\Default%' or cmdline like '%\Google\Chrome\User Data\Default\Login Data%' or cmdline like '%\Google\Chrome\User Data\Default\Login Data For Account%');

bool access_to_browser_login_data(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("\\Opera Software\\Opera Stable\\Login Data") != std::string::npos || cmdline.find("\\Mozilla\\Firefox\\Profiles") != std::string::npos || cmdline.find("\\Microsoft\\Edge\\User Data\\Default") != std::string::npos || cmdline.find("\\Google\\Chrome\\User Data\\Default\\Login Data") != std::string::npos || cmdline.find("\\Google\\Chrome\\User Data\\Default\\Login Data For Account") != std::string::npos))
    {
        std::stringstream ss;

        ss << "Browser login data accessed";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// T1556.002 - Powershell Install a DLL in System Directory
// select * from win_process_events where cmdline like '%powershell.exe%' and (cmdline like '%Copy-Item%' and cmdline like '%-Destination%') and (cmdline like '%\Windows\System32%' or cmdline like '%\Windows\SysWOW64%'));

bool powershell_install_a_DLL_in_system_directory(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("Copy-Item") != std::string::npos && cmdline.find("-Destination") != std::string::npos) && (cmdline.find("\\Windows\\System32") != std::string::npos || cmdline.find("\\Windows\\SysWOW64") != std::string::npos))
    {
        std::stringstream ss;

        ss << "DLL file installed in system directory";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// T1003.003 - Create Volume Shadow Copy with Powershell
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%win32_shadowcopy%' and cmdline like '%).Create(%' and cmdline like '%ClientAccessible%');

bool create_volume_shadow_copy_with_powershell(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("gwmi") != std::string::npos && cmdline.find("win32_shadowcopy") != std::string::npos && cmdline.find(").Create(") != std::string::npos && cmdline.find("ClientAccessible") != std::string::npos)
    {
        std::stringstream ss;

        ss << "Volume shadow copy has been created";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// T1555 - Dump Credentials from Windows Credential Manager With PowerShell
// select * from win_process_events where (cmdline like '%powershell.exe%' and (cmdline like '%GetCredmanCreds%' or cmdline like '%Get-PasswordVaultCredentials%'));

bool dump_credentials_from_windows_credential_manager_with_powershell(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("GetCredmanCreds") != std::string::npos || cmdline.find("Get-CredmanCreds") != std::string::npos || cmdline.find("Get-PasswordVaultCredentials") != std::string::npos))
    {
        std::stringstream ss;

        ss << "Credentials have been extracted with powershell";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// T1552.004 - Certificate Exported Via PowerShell - ScriptBlock
// select * from win_process_events where (cmdline like '%powershell.exe%' and (cmdline like '%Export-PfxCertificate%' or cmdline like '%Export-Certificate%'));

bool certificate_exported_via_powershell(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("Export-PfxCertificate") != std::string::npos || cmdline.find("Export-Certificate") != std::string::npos))
    {
        std::stringstream ss;

        ss << "Certificates have been exported via PowerShell";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// T1003.006 - Suspicious Get-ADReplAccount
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Get-ADReplAccount%' and cmdline like '%-All%' and cmdline like '%-Server%');

bool suspicious_get_ADReplAccount(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-ADReplAccount") != std::string::npos && cmdline.find("-All") != std::string::npos && cmdline.find("-Server") != std::string::npos)
    {
        std::stringstream ss;

        ss << "Domain and credentials have been compromised";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// T1558.003 - Request A Single Ticket via PowerShell
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%New-Object%' and cmdline like '%System.IdentityModel.Tokens.KerberosRequestorSecurityToken%');

bool request_a_single_ticket_via_powershell(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-Object") != std::string::npos && cmdline.find("System.IdentityModel.Tokens.KerberosRequestorSecurityToken") != std::string::npos)
    {
        std::stringstream ss;

        ss << "Native PowerShell Identity modules utilized to query the domain to extract the Service Principal Names for a single computer";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// T1003.001 - PowerShell Get-Process LSASS in ScriptBlock
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%-u%' and cmdline like '%-f%' and cmdline like '%Get-Process lsass%');

bool powershell_get_process_LSASS_in_scriptblock(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("-u") != std::string::npos && cmdline.find("-f") != std::string::npos && cmdline.find("Get-Process lsass") != std::string::npos)
    {
        std::stringstream ss;

        ss << "Credential material stored in the process memory of LSASS might be exposed";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// T1110.001 - Suspicious Connection to Remote Account
// select * from win_process_events where (cmdline like '%powershell.exe%' and (cmdline like '%System.DirectoryServices.Protocols.LdapDirectoryIdentifier%' or cmdline like '%System.DirectoryServices.Protocols%' or cmdline like '%System.Net.NetworkCredential%' or cmdline like '%System.DirectoryServices.Protocols.LdapConnection%'));

bool suspicious_connection_to_remote_account(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("System.DirectoryServices.Protocols.LdapDirectoryIdentifier") != std::string::npos || cmdline.find("System.DirectoryServices.Protocols") != std::string::npos || cmdline.find("System.Net.NetworkCredential") != std::string::npos || cmdline.find("System.DirectoryServices.Protocols.LdapConnection") != std::string::npos))
    {
        std::stringstream ss;

        ss << "Passwords might be guessed using an iterative manner";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}



