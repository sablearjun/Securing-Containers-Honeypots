#include "command_control_rules.h"
#include <sstream>

// Command and Control: T1571 Non-Standard Port

// https://attack.mitre.org/techniques/T1571/
 
bool non_standard_port_command_control(const ProcessEvent& process_event, Event& rule_event)
{
   for (auto &process : process_event.parents)
   {
       std::unordered_map<int,std::string> mp;
       mp.insert({4050,"APT-C-36 attack using 4050 port for command control"});
       mp.insert({14146,"APT32 attack using 14146 port for command control"});
       mp.insert({808,"APT33 attack using 808 port for command control"});
       mp.insert({880,"APT33 attack using 880 port for command control "});
       mp.insert({8000,"BADCALL attack using 8000 port for command control "});
       mp.insert({1058,"BANKSHOT attack using 1058 port for command control "});
       mp.insert({5190,"DarkVishnya attack using 5190 port for command control "});
       mp.insert({7900,"DarkVishnya attack using 7900 port for command control "});
       mp.insert({4444,"DarkVishnya attack using 4444 port for command control "});
       mp.insert({4445,"DarkVishnya attack using 4445 port for command control "});
       mp.insert({31337,"DarkVishnya attack using 31337 port for command control "});
       mp.insert({20,"Emotet attack using 20 port for command control "});
       mp.insert({22,"Emotet attack using 22 port for command control "});
       mp.insert({7080,"Emotet attack using 7080 port for command control "});
       mp.insert({50000,"Emotet attack using 50000 port for command control "});
       mp.insert({53,"FIN7 attack using 53 port for command control "});
       mp.insert({80,"FIN7 attack using 80 port for command control "});
       mp.insert({8080,"FIN7 attack using 8080 port for command control "});
       mp.insert({9005,"GoldenSpy attack using 9005 port for command control "});
       mp.insert({9006,"GoldenSpy attack using 9006 port for command control "});
       mp.insert({9002,"GoldenSpy attack using 9002 port for command control "});
       mp.insert({33666,"GoldenSpy attack using 33666 port for command control "});
       mp.insert({8090,"GoldenSpy attack using 8090 port for command control "});
       mp.insert({46769,"GravityRAT attack using 46769 port for command control "});
       mp.insert({4443,"MagicHound attack using 4443 port for command control "});
       mp.insert({9999,"Metamorfo attack using 9999 port for command control "});
       mp.insert({1177,"njRAT attack using 1177 port for command control "});
       mp.insert({995,"RedLeaves attack using 995 port for command control "});
       mp.insert({51640,"Rocke attack using 51640 port for command control "});
       mp.insert({44443,"RTM attack using 44443 port for command control "});
       mp.insert({6789,"SabdwormTeam attack using 6789 port for command control "});
       mp.insert({444,"Silence attack using 444 port for command control "});
       mp.insert({1402,"StrongPity attack using 1402 port for command control "});
       mp.insert({4444,"TEMP.Veles attack using 4444 port for command control "});
       mp.insert({8531,"TEMP.Veles attack using 8531 port for command control "});
       mp.insert({50501,"TEMP.Veles attack using 50501 port for command control "});
       mp.insert({447,"TrickBot attack using 447 port for command control "});
       mp.insert({8082,"TrickBot attack using 8082 port for command control "});
       mp.insert({8443,"TypeFrame attack using 8443 port for command control "});
       mp.insert({25,"WellMail attack using 25 port for command control "});
       mp.insert({2083,"WIRTE attack using 2083 port for command control "});
       mp.insert({2087,"WIRTE attack using 2087 port for command control "});
       mp.insert({1985,"ZxShell attack using 1985 port for command control "});
       mp.insert({1986,"ZxShell attack using 1986 port for command control "});
       if (mp.find(process_event.entry.remote_port)!=mp.end())
       {
           std::stringstream ss;
 
           ss << "[" << process_event.entry.remote_port << " (remote port: " << mp[process_event.entry.remote_port] << ")] non standard port used ]";
           rule_event.metadata = ss.str();
 
           return true;
       }
   }
   return false;
}