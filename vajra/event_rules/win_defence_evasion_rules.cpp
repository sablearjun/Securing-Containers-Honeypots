#include "win_defence_evasion_rules.h"
#include <sstream>

// DEFENSE EVASION

//T1197 - BITS jobs
// select action, path, cmdline, parent_path from win_process_events where path like "%bitsadmin%" and (parent_path like "%cmd%" or parent_path like "%powershell%");

bool BITS_jobs(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("bitsadmin") != std::string::npos && (process_event.entry.parent_path.find("cmd") != std::string::npos || process_event.entry.parent_path.find("powershell") != std::string::npos))
    {
        std::stringstream ss;
        ss << "Background Intelligent Transfer Service Abused";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

//T1078.001 - Valid accounts: Default accounts
//select action, path, cmdline, parent_path from win_process_events where cmdline like '%guest%/add%' and cmdline like '%reg%add%';

bool escalate_guest(const ProcessEvent &process_event, Event &rule_event)
{
    if((process_event.entry.cmdline.find("guest") != std::string::npos && process_event.entry.cmdline.find("add") != std::string::npos) && ((process_event.entry.cmdline.find("reg") != std::string::npos && process_event.entry.cmdline.find("add") != std::string::npos) && (process_event.entry.path.find("cmd") != std::string::npos )))
    {
        std::stringstream ss;
        ss << "Default guest account privilege escalated";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// bool escalate_guest(const ProcessEvent &process_event, Event &rule_event)
// {
//     if((process_event.entry.cmdline.find("guest") != std::string::npos && process_event.entry.cmdline.find("add") != std::string::npos) || (process_event.entry.cmdline.find("reg") != std::string::npos && process_event.entry.cmdline.find("add") != std::string::npos && process_event.entry.path.find("reg.exe")!= std::string::npos ))
//     {
//         std::stringstream ss;
//         ss << "Default guest account privilege escalated";
//         rule_event.metadata = ss.str();

//         return true;
//     }

//     return false;
// }


//T1112 - Modify Registry
//select action, path, cmdline, parent_path from win_process_events where cmdline like "%reg%add%";

bool registry_modification(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("reg") != std::string::npos && process_event.entry.cmdline.find("add") != std::string::npos)
    {
        std::stringstream ss;
        ss << "Registry modification for possibly malicious purpose";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}


// select path, parent_path, cmdline, win_process_events where cmdline like '%ExecutionPolicy Bypass%';
// select path, parent_path, cmdline from win_process_events where cmdline like '%reg%' and cmdline like '%password%';

// bool unsecured_credentials(const ProcessEvent &process_event, Event &rule_event)
// {
//     if(process_event.entry.cmdline.find("ExecutionPolicy Bypass") != std::string::npos)
//     {
//         std::stringstream ss;
//         ss << "Unsecure credential access";
//         rule_event.metadata = ss.str();

//         return true;
//     }

//     if(process_event.entry.cmdline.find("reg") != std::string::npos && process_event.entry.cmdline.find("password"))
//     {
//         std::stringstream ss;
//         ss << "Unsecure credential access";
//         rule_event.metadata = ss.str();

//         return true;
//     }

//     return false;
// }


//T1202 - Indirect command Execution
// select action, path, parent_path from win_process_events where cmdline like "%pcalua%" and cmdline like "%calc.exe%" and cmdline like "%cmd.exe%";

bool indirect_command_execution(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("pcalua.exe") != std::string::npos && process_event.entry.cmdline.find("calc.exe") != std::string::npos && process_event.entry.cmdline.find("cmd.exe") != std::string::npos)
    {
        std::stringstream ss;
        ss << "Possible indirect program or command execution via pcalua.exe";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

//T1550.003 - Use Alternate Authentication Material
// select * from win_process_events where (cmdline like '%mimikatz%' and cmdline like '%kerberos::ptt%') OR (cmdline like '%rubeus%' and cmdline like '%PsExec.exe%'); 

bool alternate_authentication(const ProcessEvent &process_event, Event &rule_event)
{
    if((process_event.entry.cmdline.find("mimikatz") != std::string::npos && process_event.entry.cmdline.find("kerberos::ptt") != std::string::npos)
    || (process_event.entry.cmdline.find("rubeus.exe") != std::string::npos && process_event.entry.cmdline.find("PsExec.exe") != std::string::npos))
    {
        std::stringstream ss;
        ss << "Possible alternate authentication using Kerberos ticket";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

//T1070 - Indicator Removal
// select path, PARENT_path, cmdline from win_process_events where action="PROC_CREATE" and path like '%fsutil%' and cmdline like '%deletejournal%';

bool indicator_removal(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.action == "PROC_CREATE" && process_event.entry.path.find("fsutil") != std::string::npos && process_event.entry.cmdline.find("deletejournal") != std::string::npos)
    {
        std::stringstream ss;
        ss << "Possible indicator removal using fsutil";
        rule_event.metadata = ss.str();

        return true;
    }

    return false;
}

// T1562.006 - Impair Defenses: Indicator Blocking
// select * from win_process_events where cmdline like '%PsExec.exe%' and cmdline like '%logman%' and parent_path like '%powershell.exe%';

// bool disable_powershell_etw(const ProcessEvent &process_event, Event &rule_event)
// {
//     if(process_event.entry.action == "PROC_CREATE" && process_event.entry.parent_path.find("PSEXESVC") != std::string::npos && process_event.entry.path.find("logman") != std::string::npos && (process_event.entry.cmdline.find("stop") != std::string::npos || process_event.entry.cmdline.find("update") != std::string::npos || process_event.entry.cmdline.find("delete") != std::string::npos))
//     {
//         std::stringstream ss;
//         ss << "Microsoft Powershell ETW provider disabled using logman.exe";
//         rule_event.metadata = ss.str();
//         return true;
//     }

//     return false;
// }


bool disable_powershell_etw(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("PsExec.exe") != std::string::npos && process_event.entry.cmdline.find("logman") != std::string::npos && process_event.entry.parent_path.find("powershell.exe"))
    {
        std::stringstream ss;
        ss << "Microsoft Powershell ETW provider disabled using logman.exe";
        rule_event.metadata = ss.str();
        return true;
    }

    return false;
}


//T1134.002 - Create process with token
// select * from win_process_events where parent_path like '%powershell%' and cmdline like '%lsass%' and cmdline like '%cmd%' ;

// bool create_process_with_token(const ProcessEvent &process_event, Event &rule_event)
// {
//     if(process_event.entry.parent_path.find("lsass.exe") != std::string::npos && process_event.entry.path.find("cmd.exe") != std::string::npos)
//     {
//         std::stringstream ss;
//         ss << "Access tokens used to run programs under a different user";
//         rule_event.metadata = ss.str();
//         return true;
//     }
//     return false;
// }


bool create_process_with_token(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.parent_path.find("powershell.exe") != std::string::npos && process_event.entry.cmdline.find("lsass") != std::string::npos && process_event.entry.cmdline.find("Set-ExecutionPolicy") != std::string::npos && process_event.entry.cmdline.find("cmd.exe") != std::string::npos)
    {
        std::stringstream ss;
        ss << "Access tokens used to run programs under a different user";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}


// T1055.012 - Process Hollowing
// select * from win_process_events where parent_path like '%powershell%' and cmdline like '%notepad%' and cmdline like '%powershell%' and cmdline like '%cmd%';

// bool process_hollowing(const ProcessEvent &process_event, Event &rule_event)
// {
//     if(process_event.entry.parent_path.find("WINWORD.EXE") != std::string::npos)
//     {
//         std::stringstream ss;
//         ss << "Possible process hollowing";
//         rule_event.metadata = ss.str();
//         return true;
//     }
//     return false;
// }


bool process_hollowing(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("powershell.exe") != std::string::npos && process_event.entry.cmdline.find("notepad.exe") != std::string::npos && process_event.entry.cmdline.find("powershell.exe") != std::string::npos && process_event.entry.cmdline.find("cmd.exe") != std::string::npos)
    {
        std::stringstream ss;
        ss << "Possible process hollowing";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}


// T1222.001 - File and Directory Permissions Modification: Windows File and Directory Permissions Modification
// select * from win_process_events where path like '%icacls.exe%';  (Can't verify if it's actually malicious)

bool grant_access_to_C(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("icacls.exe") != std::string::npos && process_event.entry.cmdline.find("Everyone") != std::string::npos)
    {
        std::stringstream ss;
        ss << "Granted full access to C drive";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

// T1564.001 - Hide Artifacts: Hidden Files and Directories
// select * from win_process_events where parent_path like '%cmd.exe%' and cmdline like '%attrib.exe%' and cmdline like '%temp%';

// bool hide_artifacts(const ProcessEvent &process_event, Event &rule_event)
// {
//     if(process_event.entry.action == "FILE_CREATE" && process_event.entry.process_name.find("WINWORD.exe") != std::string::npos)
//     {
//         std::stringstream ss;
//         ss << "Binaries hidden inside of files stored in Office document";
//         rule_event.metadata = ss.str();
//         return true;
//     }
//     return false;
// }

// gets detected but also triggers for t1222.001
// bool hide_artifacts(const ProcessEvent &process_event, Event &rule_event)
// {
//     if(process_event.entry.action == "PROC_TERMINATE" && (process_event.entry.path.find("attrib.exe") != std::string::npos)  && (process_event.entry.cmdline.find("cmd.exe") != std::string::npos || process_event.entry.cmdline.find("attrib.exe") != std::string::npos))
//     {
//         std::stringstream ss;
//         ss << "Binaries hidden inside of files stored in Office document";
//         rule_event.metadata = ss.str();
//         return true;
//     }
//     return false;
// }

bool hide_artifacts(const ProcessEvent &process_event, Event &rule_event)
{
    if((process_event.entry.path.find("cmd.exe") != std::string::npos)  && (process_event.entry.cmdline.find("cmd.exe") != std::string::npos && process_event.entry.cmdline.find("attrib.exe") != std::string::npos && process_event.entry.cmdline.find("temp") != std::string::npos))
    {
        std::stringstream ss;
        ss << "Binaries hidden inside of files stored in Office document";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}


//T1036 - Masquerading
// select * from win_process_events where path like '%powershell%' and ((cmdline like '%copy-item%' and cmdline like '%destination%') OR cmdline like '%Expand-Archive%');
bool masquerading (const ProcessEvent &process_event, Event &rule_event)
{

    std::string cmdline = process_event.entry.cmdline;
    if(process_event.entry.path.find("powershell.exe") != std::string::npos && ((cmdline.find("copy-item") != std::string::npos && cmdline.find("destination") != std::string::npos)  || cmdline.find("Expand-Archive") != std::string::npos)) 
    {
        std::stringstream ss;
        ss << "Object location manipulated for evading defense";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
        

}

//T1127 - Trusted Developer Utilities Proxy Execution
// select * from win_process_events where path like '%jsc.exe%' and cmdline like '%jsc.exe%';

bool proxy_execution (const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("jsc.exe") != std::string::npos && process_event.entry.cmdline.find("jsc.exe") != std::string::npos){
        std::stringstream ss;
        ss << "Object location manipulated for evading defense";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;



}

//T1548.002 - Abuse Elevation Control Mechanism: Bypass User Account Control
// select * from win_process_events where cmdline like '%reg.exe%' and cmdline like '%eventvwr.msc%';
bool bypass_user_account_control(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("reg.exe") != std::string::npos && process_event.entry.cmdline.find("eventvwr.msc") != std::string::npos){
        std::stringstream ss;
        ss << "Object location manipulated for evading defense";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;



}

//T1220 - XSL Script Processing
// select * from win_process_events where path like '%WMIC%' and cmdline like '%wmic%' and cmdline like '%FORMAT%';

bool xsl_script_processing(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("WMIC") != std::string::npos && process_event.entry.cmdline.find("wmic") != std::string::npos && process_event.entry.cmdline.find("FORMAT") != std::string::npos){
        std::stringstream ss;
        ss <<"Scripts are embedded inside XSL files";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;



}

//T1218.005 - System Binary Proxy Execution: Mshta
// select * from win_process_events where path like '%mshta.exe%' and cmdline like '%mshta.exe%';

bool mshta(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("mshta.exe") != std::string::npos && process_event.entry.cmdline.find("mshta") != std::string::npos){
        std::stringstream ss;
        ss <<"Proxy Execution of .hta files using mshta.exe";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;



}

//T1218.010 - System Binary Proxy Execution: Regsvr32
// select * from win_process_events where path like '%regsvr32%' and (cmdline like '%Temp%' and cmdline like '%scrobj.dll%');

bool system_binary_proxy_execution_regsvr32(const ProcessEvent &process_event, Event &rule_event)
{
    std::string path = process_event.entry.path;
    std::string parent_path = process_event.entry.parent_path;
    std::string cmdline = process_event.entry.cmdline;

    if((path.find("regsvr32") != std::string::npos && cmdline.find("Temp") !=std::string::npos) || (path.find("regsvr32") != std::string::npos && cmdline.find("scrobj.dll") !=std::string::npos) )
    {
        std::stringstream ss;
        ss <<"regsvr32.exe used for proxy execution of malicious code";
        rule_event.metadata = ss.str();
        return true;

    }
    return false;



}

//T1218.007 - System Binary Proxy Execution: Msiexec
// select * from win_process_events where (path like '%cmd.exe%' or path like '%msiexec%') and cmdline like '%msiexec%';

bool system_binary_proxy_execution_msiexec(const ProcessEvent &process_event, Event &rule_event)
{
    std::string path = process_event.entry.path;
    std::string cmdline = process_event.entry.cmdline;

    if((path.find("cmd.exe") != std::string::npos && cmdline.find("msiexec") != std::string::npos) || (path.find("msiexec") != std::string::npos && cmdline.find("msiexec") != std::string::npos)){
        std::stringstream ss;
        ss <<"msiexec.exe used for proxy execution of malicious payloads";
        rule_event.metadata = ss.str();
        return true;
    
    }
    return false;


}

//T1127.001 - Trusted Developer Utilities Proxy Execution: MSBuild
// select * from win_process_events where parent_path like '%cmd.exe%' and path like '%MSBuild% and cmdline like '%msbuild.exe%';

bool proxy_execution_msbuild(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.parent_path.find("cmd.exe") != std::string::npos && process_event.entry.path.find("MSBuild") != std::string::npos && process_event.entry.cmdline.find("msbuild.exe") != std::string::npos)
        {
            std::stringstream ss;
            ss << "MSBuild used for proxy execution of code through Windows utility";
            rule_event.metadata = ss.str();
            return true;
        }
        return false;
    
    }

//T1218 - System Binary Proxy Execution
// select * from win_process_events where path like '%gpscript%' and cmdline like '%Gpscript%' and (cmdline like '%startup%' or cmdline like '%logon%');

bool system_binary_proxy_execution(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("gpscript") != std::string::npos && process_event.entry.cmdline.find("Gpscript") != std::string::npos && (process_event.entry.cmdline.find("startup") != std::string::npos || process_event.entry.cmdline.find("logon") != std::string::npos) )
    {
            std::stringstream ss;
            ss << "Proxy execution done for malicious content using signed binaries";
            rule_event.metadata = ss.str();
            return true;   
    }
    return false;
}

// T1218.011 - System Binary Proxy Execution: Rundll32
// select * from win_process_events where path like '%rundll32.exe%' and cmdline like '%rundll32.exe%' and cmdline like '%desk.cpl%' and cmdline like '%.scr%';

bool system_binary_proxy_execution_rundll32(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("rundll32.exe") != std::string::npos && process_event.entry.cmdline.find("rundll32.exe") != std::string::npos && process_event.entry.cmdline.find("desk.cpl") != std::string::npos && process_event.entry.cmdline.find(".scr") != std::string::npos)
    {

            std::stringstream ss;
            ss << "rundll32.exe used for proxy execution of malicious payloads";
            rule_event.metadata = ss.str();
            return true;  
    }
    return false;
}

//T1574.002 - Hijack Execution Flow: DLL Side-Loading
// select * from win_process_events where cmdline like '%dotnet%' and cmdline like '%echo%' and cmdline like '%.dll%';

bool dll_side_loading(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    if(cmdline.find("dotnet") != std::string::npos && cmdline.find("echo") != std::string::npos && cmdline.find(".dll") != std::string::npos){

        std::stringstream ss;
        ss << "Malicious payloads executed by side loading DLLs";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1564.004 - Hide Artifacts: NTFS File Attributes
// select * from win_process_events where cmdline like '%echo%' and cmdline like '%adstest.txt%';

bool ntfs_file_attributes(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    if(cmdline.find("adstest.txt") != std::string::npos && cmdline.find("echo") != std::string::npos){

        std::stringstream ss;
        ss << "NTFS file attributes used to hide malicious data";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1070.003 - Indicator Removal: Clear Command History
// select * from win_process_events where (cmdline like '%Remove-Item%' and cmdline like '%(Get-PSReadlineOption).HistorySavePath%') or cmdline like '%Set-PSReadlineOption%';

bool clear_command_history(const ProcessEvent &process_event, Event &rule_event)
{  
    std::string cmdline = process_event.entry.cmdline;

    if((cmdline.find("Remove-Item") != std::string::npos && cmdline.find("(Get-PSReadlineOption).HistorySavePath") != std::string::npos ) || (cmdline.find("Set-PSReadlineOption") != std::string::npos))
    {
        std::stringstream ss;
        ss<< "Command history cleared to conceal malicious activities";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1027 - Obfuscated Files or Information
// select * from win_process_events where cmdline like '%IEX%' and cmdline like '%OriginalCommand%' and cmdline like '%Debug%';

bool obfuscated_files_or_information(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    
    if(cmdline.find("IEX") != std::string::npos && cmdline.find("OriginalCommand") != std::string::npos && cmdline.find("Debug") != std::string::npos)
    {
        std::stringstream ss;
        ss << "Content of files obfuscated";
        rule_event.metadata = ss.str();
        return true;

    }
    return false;
}

//T1055 - Process Injection
// select * from win_process_events where cmdline like '%mimikatz.exe%' and cmdline like '%lsadump%' and cmdline like '%inject%';

bool process_injection(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    if(cmdline.find("mimikatz.exe") != std::string::npos && cmdline.find("lsadump") != std::string::npos && cmdline.find("inject") != std::string::npos)
    {
        std::stringstream ss;
        ss<< "Codes injected in processes to elevate privileges ";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1070.004 - Indicator Removal: File Deletion
// select * from win_process_events where (cmdline like '%Remove-Item%' and cmdline like '%Get-ChildItem%') or (cmdline like '%New-Item%' and cmdline like '%Remove-Item%');

bool indicator_removal_file_deletion(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    if((cmdline.find("Remove-Item") != std::string::npos && cmdline.find("Get-ChildItem") != std::string::npos) || (cmdline.find("New-Item") != std::string::npos && cmdline.find("Remove-Item") != std::string::npos))
    {
        std::stringstream ss;
        ss << "Files deleted after malicious intrusion activity";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1564.003 - Hide Artifacts: Hidden Window
// select * from win_process_events where cmdline like '%Start-Process%' and cmdline like '%WindowStyle%' and cmdline like '%hidden%';

bool hidden_window_hide_artifacts(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    if(cmdline.find("Start-Process") != std::string::npos && cmdline.find("WindowStyle") != std::string::npos && cmdline.find("hidden") != std::string::npos)
    {
        std::stringstream ss;
        ss<< "Hidden windows used to conceal malicious activity";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1562.001 - Impair Defenses: Disable or Modify Tools
// select * from win_process_events where (path like '%fltMC.exe%' or path like '%cmd.exe%') and (cmdline like '%fltmc.exe%' and cmdline like '%SysmonDrv%');

bool impair_defenses_disable_modify_tools(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    std::string path = process_event.entry.path;

    if((path.find("cmd.exe") != std::string::npos || path.find("fltMC.exe") != std::string::npos) && (cmdline.find("fltmc.exe") != std::string::npos && cmdline.find("SysmonDrv") != std::string::npos)){

        std::stringstream ss;
        ss << "Security tools disabled or modified to avoid detection";
        rule_event.metadata = ss.str();
        return true;
        
    }
    return false;
} 

//T1553.004 - Subvert Trust Controls: Install Root Certificate
// select * from win_process_events where cmdline like '%Import-Certificate%' and cmdline like '%Cert:\\LocalMachine\\Root%';

bool install_root_certificate(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    if(cmdline.find("Import-Certificate") !=  std::string::npos && cmdline.find("Cert:\\LocalMachine\\Root") != std::string::npos)
    {
        std::stringstream ss;
        ss<< "Root certificate installed to avoid detection";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1218.004 - System Binary Proxy Execution: InstallUtil
// select * from win_process_events where cmdline like '%Invoke-Build%' and cmdline like '%InvokeInstallUtilAssembly%';

bool system_binary_proxy_execution_installutil(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    if(cmdline.find("Invoke-Build") !=  std::string::npos && cmdline.find("InvokeInstallUtilAssembly") != std::string::npos)
    {
        std::stringstream ss;
        ss<< "InstallUtil used for proxy execution of code";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1140 - Deobfuscate/Decode Files or Information
// select * from win_process_events where cmdline like '%certutil%' and cmdline like '%encode%' and cmdline like '%decode%';

bool decode_files_or_information(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if(cmdline.find("certutil") != std::string::npos && cmdline.find("encode") != std::string::npos && cmdline.find("decode") != std::string::npos)
    {
        std::stringstream ss;
        ss << "";
        rule_event.metadata = ss.str();
        return true;

    }
    return false;
}

//T1112 - Modify Registry: NetWire RAT Registry Key Creation
//select * from win_process_events where cmdline like '%HKCU:\\Software\\Netwire%';

bool modify_registry_netwire(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("HKCU:\\Software\\Netwire") != std::string::npos){
         std::stringstream ss;
        ss << "Registry modified using netwire for malicious purpose";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1112 - Modify Registry: Ursnif Malware Registry Key Creation
// select * from win_process_events where cmdline like '%HKCU\\Software\\AppDataLow\\Software\\Microsoft%' and path like '%reg.exe%';

bool modify_registry_ursnif(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("HKCU\\Software\\AppDataLow\\Software\\Microsoft") != std::string::npos && process_event.entry.path.find("reg.exe") != std::string::npos){
         std::stringstream ss;
        ss << "Registry modified using Ursnif Malware for malicious purpose";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1112 - Modify Registry: Terminal Server Client Connection History Cleared    
// select * from win_process_events where cmdline like '%Microsoft\\Terminal Server Client\\Default%' or cmdline like '%Microsoft\\Terminal Server Client\\Servers%';

bool modify_registry_terminal_server_client(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("Microsoft\\Terminal Server Client\\Default") != std::string::npos || process_event.entry.cmdline.find("Microsoft\\Terminal Server Client\\Servers") != std::string::npos){
         std::stringstream ss;
        ss << "Terminal Server Client Connection History Cleared for malicious purpose";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1112 - Modify Registry: Disable Windows Toast Notifications

// bool modify_registry_netwire(const ProcessEvent &process_event, Event &rule_event)
// {
//     if(process_event.entry.cmdline.find("HKCU:\\Software\\Netwire") != std::string::npos){
//          std::stringstream ss;
//         ss << "Registry modified using netwire for malicious purpose";
//         rule_event.metadata = ss.str();
//         return true;
//     }
// }

//T1112 - Modify Registry: BlackByte Ransomware Registry Changes

bool modify_registry_blackbyte(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    if(cmdline.find("HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System") != std::string::npos && cmdline.find("LocalAccountTokenFilterPolicy") != std::string::npos 
    && cmdline.find("EnableLinkedConnections") != std::string::npos && cmdline.find("LongPathsEnabled") != std::string::npos){
         std::stringstream ss;
        ss << "Registry modified using BlackByte Ransomeware for malicious purpose";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1112 - Modify Registry: Windows Add Registry Value to Load Service in Safe Mode

bool modify_registry_load_service_safemode(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("HKLM\\SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal") != std::string::npos){
         std::stringstream ss;
        ss << "Registry modified to load service in safe mode";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1112 - Modify Registry: Disable Windows Registry Tool

bool modify_registry_disable_win_registry_tool(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;
    if(cmdline.find("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System") != std::string::npos && (cmdline.find("DisableRegistryTools") != std::string::npos || cmdline.find("DisableNotificationCenter") != std::string::npos || cmdline.find("DisableChangePassword") != std::string::npos
    || cmdline.find("DisableTaskmgr") != std::string::npos || cmdline.find("DisableLockWorkstation") != std::string::npos)){
         std::stringstream ss;
        ss << "Windows registry tool disabled";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1112 - Modify Registry: Disable Windows Security Center Notifications

bool modify_registry_disable_win_security_notifications(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.cmdline.find("HKLM\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell") != std::string::npos && process_event.entry.cmdline.find("UseActionCenterExperience") != std::string::npos){
         std::stringstream ss;
        ss << "Windows Security center notifications disabled for malicious purposes";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1112 - Modify Registry: Windows Group Policy Feature

bool modify_registry_win_group_policy_feature(const ProcessEvent &process_event, Event &rule_event)
{   
    std::string cmdline = process_event.entry.cmdline;
    if(cmdline.find("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explore") != std::string::npos && (cmdline.find("HideSCAVolume") != std::string::npos || cmdline.find("HideSCAPower") != std::string::npos
    || cmdline.find("HideSCAHealth") != std::string::npos || cmdline.find("HideSCANetwork") != std::string::npos )){
         std::stringstream ss;
        ss << "Registry modified to load service in safe mode";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1562.001 - Impair Defenses: Disable or Modify Tools, AMSI Bypass - Remove AMSI Provider Reg Key

bool impair_defenses_disable_modify_tools_AMSI_Byspass(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if(cmdline.find("HKLM:\\SOFTWARE\\Microsoft\\AMSI\\Providers\\{2781761E-28E0-4109-99FE-B9D127C57AFE}") != std::string::npos){

        std::stringstream ss;
        ss << "Security tools disabled or modified to avoid detection";
        rule_event.metadata = ss.str();
        return true;
        
    }
    return false;

} 

//T1562.001 - Impair Defenses: Disable or Modify Tools, Disable Microsoft Office Security Features

bool impair_defenses_disable_modify_tools_office_security(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if(cmdline.find("Software\\Microsoft\\Office") != std::string::npos && cmdline.find("VBAWarnings") != std::string::npos && cmdline.find("DisableInternetFilesInPV") != std::string::npos 
    && cmdline.find("DisableUnsafeLocationsInPV") != std::string::npos && cmdline.find("DisableAttachementsInPV") != std::string::npos  ){

        std::stringstream ss;
        ss << "Security tools disabled or modified to avoid detection";
        rule_event.metadata = ss.str();
        return true;
        
    }
    return false;
    
} 

//T1562.004 - Impair Defenses: Disable or Modify System Firewall, Disable Microsoft Defender Firewall

bool impair_defenses_disable_defender_firewall(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if(cmdline.find("CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\PublicProfile") != std::string::npos && cmdline.find("EnableFirewall") != std::string::npos ){

        std::stringstream ss;
        ss << "Microsoft Defender FIrewall disabled to avoid detection";
        rule_event.metadata = ss.str();
        return true;
        
    }
    return false;
    
} 

//T1574.012 - Hijack Execution Flow: COR_PROFILER, User scope COR_PROFILER

bool user_scope_cor_profile(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if(cmdline.find("COR_ENABLE_PROFILING") != std::string::npos && cmdline.find("COR_PROFILER") != std::string::npos ){

        std::stringstream ss;
        ss << "COR_PROFILER environment used for hijack execution flow of programs";
        rule_event.metadata = ss.str();
        return true;
        
    }
    return false;
    
} 

//T1564.001 - Hide Artifacts: Hidden Files and Directories , Hide Files Through Registry

bool hide_artifacts_through_registry(const ProcessEvent &process_event, Event &rule_event)
{
    if(process_event.entry.path.find("CurrentVersion\\Explorer\\Advanced") != std::string::npos && process_event.entry.cmdline.find("ShowSuperHidden") != std::string::npos && process_event.entry.cmdline.find("Hidden") != std::string::npos)  
    {
        std::stringstream ss;
        ss << "Show hidden files switch is disabled in the registry";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

// T1562.001 - Impair Defenses: Disable or Modify Tools, Disable Microsoft Office Security Features, Tamper with Windows Defender

bool impair_defenses_tamper_win_defender(const ProcessEvent &process_event, Event &rule_event)
{
    std::string cmdline = process_event.entry.cmdline;

    if((cmdline.find("SOFTWARE\\Policies\\Microsoft\\Windows Defender")  != std::string::npos && cmdline.find("DisableAntiSpyware") != std::string::npos) || (cmdline.find("DisableRealtimeMonitoring") != std::string::npos
    && cmdline.find("DisableBehaviorMonitoring") != std::string::npos && cmdline.find("DisableScriptScanning") != std::string::npos &&  cmdline.find("DisableBlockAtFirstSeen") != std::string::npos)){
        std::stringstream ss;
        ss << "Windows defender is disabled from starting up at reboot";
        rule_event.metadata = ss.str();
        return true;
    }
    return false;
}

//T1553.004 - Subvert Trust Controls: Install Root Certificate,Install root CA on Windows with certutil

bool install_root_certificate_win_certutil(const ProcessEvent &process_event, Event &rule_event)
{

    std::string cmdline = process_event.entry.cmdline;
    std::string path = process_event.entry.path;

    if(path.find("certutil.exe") != std::string::npos && cmdline.find("certutil.exe") != std::string::npos && cmdline.find("addstore") != std::string::npos)
    {
        std::stringstream ss;
        ss << "Root Certificate created using certutil";
        rule_event.metadata = ss.str();
        return true;

    }
    return false;
}

//T1070.004 - Indicator Removal: File Deletion, Delete a single file

bool indicator_removal_del_single_file(const ProcessEvent &process_event, Event &rule_event)
{

    std::string cmdline = process_event.entry.cmdline;
    std::string path = process_event.entry.path;

    if(path.find("cmd.exe") != std::string::npos && cmdline.find("del") != std::string::npos && cmdline.find("/f") != std::string::npos)
    {
        std::stringstream ss;
        ss << "Single file deleted from the temporary directory";
        rule_event.metadata = ss.str();
        return true;

    }
    return false;
}


// T1059.001 - PowerShell Downgrade Attack
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%-version 2%' and cmdline like '%-Command Write-Host%'

bool powerShell_downgrade_attack(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("Get-WmiObject") != std::string::npos && cmdline.find("Win32_Shadowcopy") != std::string::npos && cmdline.find("Delete()") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Powershell downgrade attack";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1562.001 - Tamper Windows Defender - PSClassic
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%Set-MpPreference %' and cmdline like '%DisableRealtimeMonitoring%' and cmdline like '%DisableScriptScanning%' and cmdline like '%DisableBlockAtFirstSeen%'

bool tamper_windows_defender(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Set-MpPreference") != std::string::npos && cmdline.find("DisableRealtimeMonitoring") != std::string::npos && cmdline.find("DisableScriptScanning") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Windows defender has been tampered, scheduled scanning and other parts may be affected";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1562.001 - AMSI Bypass Pattern Assembly GetType
// select * from win_process_events where cmdline like '%powershell.exe%' and cmdline like '%[Ref].Assembly.GetType%' and cmdline like '%SetValue($null,$true)%' and cmdline like '%NonPublic,Static%'

bool AMSI_bypass_pattern_assembly_getType(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("[Ref].Assembly.GetType") != std::string::npos && cmdline.find("SetValue($null,$true)") != std::string::npos && cmdline.find("NonPublic,Static") != std::string::npos)
	{
		std::stringstream ss;
		ss << "AMSI bypass PowerShell scripts have been compromised";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1497.001 - Powershell Detect Virtualization Environment
// select * from win_process_events where cmdline like ('%powershell.exe%' and cmdline like '%Get-WmiObject%' and cmdline like '%MSAcpi_ThermalZoneTemperature%');

bool powershell_detect_virtualization_environment(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-WmiObject") != std::string::npos && cmdline.find("MSAcpi_ThermalZoneTemperature") != std::string::npos)
	{
		std::stringstream ss;
		ss << "AMSI bypass PowerShell scripts have been compromised";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1562.001 - Disable-WindowsOptionalFeature Command PowerShell
// select * from win_process_events where cmdline like ('%powershell.exe%' and (cmdline like '%Disable-WindowsOptionalFeature%' and cmdline like '%-Online%' and cmdline like '%-FeatureName%') and (cmdline like '%Windows-Defender-Gui%' or cmdline like '%Windows-Defender-Features%' or '%Windows-Defender%' or '%Windows-Defender-ApplicationGuard%'));

bool disable_WindowsOptionalFeature_command_powershell(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("Disable-WindowsOptionalFeature") != std::string::npos && cmdline.find("-Online") != std::string::npos && cmdline.find("-FeatureName") != std::string::npos) && (cmdline.find("Windows-Defender-Gui") != std::string::npos || (cmdline.find("Windows-Defender-Features") != std::string::npos || cmdline.find("Windows-Defender") != std::string::npos || cmdline.find("Windows-Defender-ApplicationGuard") != std::string::npos)))
	{
		std::stringstream ss;
		ss << "AMSI bypass PowerShell scripts have been compromised";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1059.001 - NTFS Alternate Data Stream
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Add-Content' and cmdline like '%-Stream%' and cmdline like '%Get-Content%' and cmdline like '%Invoke-Expression%'); (to be checked)

bool NTFS_alternate_data_stream(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Add-Content") != std::string::npos && cmdline.find("-Stream") != std::string::npos && cmdline.find("Get-Content") != std::string::npos && cmdline.find("Windows-Defender-Gui") != std::string::npos && (cmdline.find("Invoke-Expression") != std::string::npos))
	{
		std::stringstream ss;
		ss << "File created with alternate data stream and might get executed";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1553.005 - Suspicious Invoke-Item From Mount-DiskImage
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%Mount-DiskImage' and cmdline like '%-ImagePath %' and cmdline like '%Get-Volume%' and cmdline like '%DriveLetter%' and cmdline like '%invoke-item%'); (to be checked)

bool suspicious_invoke_item_from_mount_diskImage(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Mount-DiskImage") != std::string::npos && cmdline.find("-ImagePath ") != std::string::npos && cmdline.find("Get-Volume") != std::string::npos && cmdline.find("DriveLetter") != std::string::npos && (cmdline.find("invoke-item") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Files of format (.iso, .vhd) may be abused to deliver malicious payloads";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}


// T1564.004 - Powershell Store File In Alternate Data Stream
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '%-ItemType Directory%' and cmdline like '%Start-Process%' and cmdline like '%-FilePath%' and cmdline like '%"$env:comspec"%' and cmdline like '%-ArgumentList%';

bool powershell_store_file_in_alternate_data_stream(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("-ItemType Directory") != std::string::npos && cmdline.find("Start-Process") != std::string::npos && cmdline.find("FilePath") != std::string::npos && cmdline.find("$env:comspec") != std::string::npos && (cmdline.find("-ArgumentList") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Files stored in alternate data streams";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}


// T1059.001 - Potential PowerShell Obfuscation Using Character Join
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '% -Value (-join(%' and cmdline like '%InvokeReturnAsIs()%' and cmdline like '%-Alias%';

bool potential_powershell_obfuscation_using_character_join(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("-Value (-join(") != std::string::npos && cmdline.find("InvokeReturnAsIs()") != std::string::npos && cmdline.find("-Alias") != std::string::npos)
	{
		std::stringstream ss;
		ss << "PowerShell obfuscation done using character join";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}


// T1059.001 - Suspicious Eventlog Clear
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '% Get-EventLog%' and cmdline like '%Clear-EventLog%';

bool suspicious_eventlog_clear(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Get-EventLog") != std::string::npos && cmdline.find("Clear-EventLog") != std::string::npos)
	{
		std::stringstream ss;
		ss << "PowerShell event logs are cleared";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1564.006 - Suspicious Hyper-V Cmdlets
// select * from win_process_events where (cmdline like '%powershell.exe%' and (cmdline like '% New-VM%' or cmdline like '%Set-VMFirmware%' or cmdline like '%Start-VM%'));

bool suspicious_hyper_v_cmdlets(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("New-VM") != std::string::npos || cmdline.find("Set-VMFirmware") != std::string::npos || cmdline.find("Start-VM") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Virtual machine stopped and deleted"; //To be checked
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1006 - Suspicious IO.FileStream
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '% New-Object%' and cmdline like '%IO.FileStream%' and cmdline like '%-InputObject%');

bool suspicious_io_fileStream(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("New-Object") != std::string::npos && cmdline.find("IO.FileStream") != std::string::npos && cmdline.find("-InputObject") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Direct access to files may have been given";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1070.005 - PowerShell Deleted Mounted Share
// select * from win_process_events where (cmdline like '%powershell.exe%' and (cmdline like '% Remove-SmbShare%' or cmdline like '%Remove-FileShare%'));

bool powershell_deleted_mounted_share(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && (cmdline.find("Remove-SmbShare") != std::string::npos || cmdline.find("Remove-FileShare") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Shared connections might be removed to clean up traces of the operation";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1553.005 - Suspicious Unblock-File
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '% Unblock-File%' and cmdline like '%-Path%');

bool suspicious_unblock_file(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Unblock-File") != std::string::npos && cmdline.find("-Path") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Removed the Zone.Identifier alternate data stream which identifies the file as downloaded from the internet";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1564.003 - Suspicious PowerShell WindowStyle Option
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '% Start-Process%' and cmdline like '%-WindowStyle%' and cmdline like '%hidden%');

bool suspicious_powershell_windowStyle_option(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Start-Process") != std::string::npos && cmdline.find("-WindowStyle") != std::string::npos && cmdline.find("hidden") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Hidden windows might be used to conceal malicious activity";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1562.001 - Tamper Windows Defender - ScriptBlockLogging
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '% Set-MpPreference%' and cmdline like '%1%' and (cmdline like '%DisableRealtimeMonitoring%' or cmdline like '%DisableBehaviorMonitoring%' or cmdline like '%DisableScriptScanning %' or cmdline like '%DisableBlockAtFirstSeen%'));

bool tamper_windows_defender_ScriptBlockLogging(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Set-MpPreference") != std::string::npos && cmdline.find("1") != std::string::npos && (cmdline.find("DisableRealtimeMonitoring") != std::string::npos || cmdline.find("DisableBehaviorMonitoring") != std::string::npos || cmdline.find("DisableScriptScanning") != std::string::npos || cmdline.find("DisableBlockAtFirstSeen") != std::string::npos))
	{
		std::stringstream ss;
		ss << "Windows defender tampered";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

// T1070.006 - Powershell Timestomp
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '% .CreationTime =%' and cmdline like '%.LastWriteTime =%' and cmdline like '%.LastAccessTime =%');

bool powershell_timestomp(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("CreationTime =") != std::string::npos && cmdline.find(".LastWriteTime =") != std::string::npos && cmdline.find(".LastAccessTime =") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Timestomps of files might be modified";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//  T1218.007 - PowerShell WMI Win32_Product Install MSI
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '% .Invoke-CimMethod%' and cmdline like '%-ClassName%' and cmdline like '%Win32_Product%' and cmdline like '%-MethodName%' and cmdline like '%.msi%') ;

bool powershell_WMI_Win32_product_install_MSI(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Invoke-CimMethod") != std::string::npos && cmdline.find("-ClassName") != std::string::npos && cmdline.find("Win32_Product") != std::string::npos && cmdline.find("-MethodName") != std::string::npos && cmdline.find(".msi") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Execution of Local MSI file with embedded JScript";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}

//  T1562.004 - Windows Firewall Profile Disabled
// select * from win_process_events where (cmdline like '%powershell.exe%' and cmdline like '% .Set-NetFirewallProfile%' and cmdline like '%-Enabled %' and cmdline like '%False%');

bool windows_firewall_profile_disabled(const ProcessEvent &process_event, Event &rule_event)
{
	std::string cmdline = process_event.entry.cmdline;

	if (cmdline.find("powershell.exe") != std::string::npos && cmdline.find("Set-NetFirewallProfile") != std::string::npos && cmdline.find("-Enabled ") != std::string::npos && cmdline.find("False") != std::string::npos)
	{
		std::stringstream ss;
		ss << "Windows Firewall Profile Disabled";
		rule_event.metadata = ss.str();
		return true;
	}
	return false;
}