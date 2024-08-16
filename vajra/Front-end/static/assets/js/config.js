// Process Events
let pestate = 0;
let pro_events = document.getElementById('pro_events');
let pro_events_div = document.getElementById('Pro_events');
let pro_events_sec = document.getElementById('Pro_Events');
let interval1 = document.getElementById('interval1');
function btn1() {
    if (pestate == 0) {
        pro_events.checked = true;
        interval1.required = true;
        pro_events_div.style.display = "flex";
        pro_events_sec.style.height = "10%";
        pro_events_sec.style.background = "#31343b";
        pestate = 1;
    }
    else if (pestate == 1) {
        pro_events.checked = false;
        interval1.required = false;
        pro_events_div.style.display = "none";
        pro_events_sec.style.height = "0px";
        pro_events_sec.style.background = "transparent";
        pestate = 0;
    }
}
// Process Events ends

// Process File Events
let pfestate = 0;
let pro_file_events = document.getElementById('pro_file_events');
let pro_file_events_div = document.getElementById('Pro_file_events');
let pro_file_events_sec = document.getElementById('Pro_File_Events');
let interval2 = document.getElementById('interval2');
function btn2() {
    if (pfestate == 0) {
        pro_file_events.checked = true;
        interval2.required = true;
        pro_file_events_div.style.display = "flex";
        pro_file_events_sec.style.height = "10%";
        pro_file_events_sec.style.background = "#31343b";
        pfestate = 1;
    }
    else if (pfestate == 1) {
        pro_file_events.checked = false;
        interval2.required = false;
        pro_file_events_div.style.display = "none";
        pro_file_events_sec.style.height = "0px";
        pro_file_events_sec.style.background = "transparent";
        pfestate = 0;
    }
}
// Process File Events ends

// Process List
let plstate = 0;
let pro_list = document.getElementById('pro_list');
let pro_list_div = document.getElementById('Pro_list');
let pro_list_sec = document.getElementById('Pro_List');
let interval3 = document.getElementById('interval3');
let snap = document.getElementById('snap');
function btn3() {
    if (plstate == 0) {
        pro_list.checked = true;
        interval3.required = true;
        snap.required = true;
        pro_list_div.style.display = "flex";
        pro_list_sec.style.height = "10%";
        pro_list_sec.style.background = "#31343b";
        plstate = 1;
    }
    else if (plstate == 1) {
        pro_list.checked = false;
        interval3.required = false;
        snap.required = false;
        pro_list_div.style.display = "none";
        pro_list_sec.style.height = "0px";
        pro_list_sec.style.background = "transparent";
        plstate = 0;
    }
}
// Process List ends

// Socket Events
let sestate = 0;
let soc_events = document.getElementById('soc_events');
let soc_events_div = document.getElementById('Soc_events');
let soc_events_sec = document.getElementById('Soc_Events');
let interval4 = document.getElementById('interval4');
function btn4() {
    if (sestate == 0) {
        soc_events.checked = true;
        interval4.required = true;
        soc_events_div.style.display = "flex";
        soc_events_sec.style.height = "10%";
        soc_events_sec.style.background = "#31343b";
        sestate = 1;
    }
    else if (sestate == 1) {
        soc_events.checked = false;
        interval4.required = false;
        soc_events_div.style.display = "none";
        soc_events_sec.style.height = "0px";
        soc_events_sec.style.background = "transparent";
        sestate = 0;
    }
}
// Socket Events ends

// User Events
let uestate = 0;
let usr_events = document.getElementById('usr_events');
let usr_events_div = document.getElementById('Usr_events');
let usr_events_sec = document.getElementById('Usr_Events');
let interval5 = document.getElementById('interval5');
function btn5() {
    if (uestate == 0) {
        usr_events.checked = true;
        interval5.required = true;
        usr_events_div.style.display = "flex";
        usr_events_sec.style.height = "10%";
        usr_events_sec.style.background = "#31343b";
        uestate = 1;
    }
    else if (uestate == 1) {
        usr_events.checked = false;
        interval5.required = false;
        usr_events_div.style.display = "none";
        usr_events_sec.style.height = "0px";
        usr_events_sec.style.background = "transparent";
        uestate = 0;
    }
}
// User Events ends

// Process Open Pipes
let popstate = 0;
let process_open_pipes = document.getElementById('process_open_pipes');
let process_open_pipes_div = document.getElementById('Process_open_pipes');
let process_open_pipes_sec = document.getElementById('Process_Open_Pipes');
let interval6 = document.getElementById('interval6');
let snap1 = document.getElementById('snap1');
function btn6() {
    if (popstate == 0) {
        process_open_pipes.checked = true;
        interval6.required = true;
        snap.required = true;
        process_open_pipes_div.style.display = "flex";
        process_open_pipes_sec.style.height = "10%";
        process_open_pipes_sec.style.background = "#31343b";
        popstate = 1;
    }
    else if (popstate == 1) {
        process_open_pipes.checked = false;
        interval6.required = false;
        snap.required = false;
        process_open_pipes_div.style.display = "none";
        process_open_pipes_sec.style.height = "0px";
        process_open_pipes_sec.style.background = "transparent";
        popstate = 0;
    }
}
// Process Open Pipes ends

// Process Open Files
let pofstate = 0;
let process_open_files = document.getElementById('process_open_files');
let process_open_files_div = document.getElementById('Process_open_files');
let process_open_files_sec = document.getElementById('Process_Open_Files');
let interval7 = document.getElementById('interval7');
let snap2 = document.getElementById('snap2');
function btn7() {
    if (pofstate == 0) {
        process_open_files.checked = true;
        interval7.required = true;
        snap2.required = true;
        process_open_files_div.style.display = "flex";
        process_open_files_sec.style.height = "10%";
        process_open_files_sec.style.background = "#31343b";
        pofstate = 1;
    }
    else if (pofstate == 1) {
        process_open_files.checked = false;
        interval7.required = false;
        snap2.required = false;
        process_open_files_div.style.display = "none";
        process_open_files_sec.style.height = "0px";
        process_open_files_sec.style.background = "transparent";
        pofstate = 0;
    }
}
// Process Open Files ends

// Listening Ports
let lpstate = 0;
let listening_ports = document.getElementById('listening_ports');
let listening_ports_div = document.getElementById('Listening_ports');
let listening_ports_sec = document.getElementById('Listening_Ports');
let interval8 = document.getElementById('interval8');
let snap3 = document.getElementById('snap3');
function btn8() {
    if (lpstate == 0) {
        listening_ports.checked = true;
        interval8.required = true;
        snap3.required = true;
        listening_ports_div.style.display = "flex";
        listening_ports_sec.style.height = "10%";
        listening_ports_sec.style.background = "#31343b";
        lpstate = 1;
    }
    else if (lpstate == 1) {
        listening_ports.checked = false;
        interval8.required = false;
        snap3.required = false;
        listening_ports_div.style.display = "none";
        listening_ports_sec.style.height = "0px";
        listening_ports_sec.style.background = "transparent";
        lpstate = 0;
    }
}
// Listening Ports ends

// Routes
let rstate = 0;
let routes = document.getElementById('routes');
let routes_div = document.getElementById('Routes');
let routes_sec = document.getElementById('RouteS');
let interval9 = document.getElementById('interval9');
let snap4 = document.getElementById('snap4');
function btn9() {
    if (rstate == 0) {
        routes.checked = true;
        interval9.required = true;
        snap4.required = true;
        routes_div.style.display = "flex";
        routes_sec.style.height = "10%";
        routes_sec.style.background = "#31343b";
        rstate = 1;
    }
    else if (rstate == 1) {
        routes.checked = false;
        interval9.required = false;
        snap4.required = false;
        routes_div.style.display = "none";
        routes_sec.style.height = "0px";
        routes_sec.style.background = "transparent";
        rstate = 0;
    }
}
// Routes ends

// logged_in_users
let liustate = 0;
let logged_in_users = document.getElementById('logged_in_users');
let logged_in_users_div = document.getElementById('Logged_in_users');
let logged_in_users_sec = document.getElementById('Logged_In_Users');
let interval10 = document.getElementById('interval10');
let snap5 = document.getElementById('snap5');
function btn10() {
    if (liustate == 0) {
        logged_in_users.checked = true;
        interval10.required = true;
        snap5.required = true;
        logged_in_users_div.style.display = "flex";
        logged_in_users_sec.style.height = "10%";
        logged_in_users_sec.style.background = "#31343b";
        liustate = 1;
    }
    else if (liustate == 1) {
        logged_in_users.checked = false;
        interval10.required = false;
        snap5.required = false;
        logged_in_users_div.style.display = "none";
        logged_in_users_sec.style.height = "0px";
        logged_in_users_sec.style.background = "transparent";
        liustate = 0;
    }
}
// logged_in_users ends

// arp_cache
let acstate = 0;
let arp_cache = document.getElementById('arp_cache');
let arp_cache_div = document.getElementById('Arp_cache');
let arp_cache_sec = document.getElementById('Arp_Cache');
let interval11 = document.getElementById('interval11');
let snap6 = document.getElementById('snap6');
function btn11() {
    if (acstate == 0) {
        arp_cache.checked = true;
        interval11.required = true;
        snap6.required = true;
        arp_cache_div.style.display = "flex";
        arp_cache_sec.style.height = "10%";
        arp_cache_sec.style.background = "#31343b";
        acstate = 1;
    }
    else if (acstate == 1) {
        arp_cache.checked = false;
        interval11.required = false;
        snap6.required = false;
        arp_cache_div.style.display = "none";
        arp_cache_sec.style.height = "0px";
        arp_cache_sec.style.background = "transparent";
        acstate = 0;
    }
}
// arp_cache ends

// suid_bin
let sbstate = 0;
let suid_bin = document.getElementById('suid_bin');
let suid_bin_div = document.getElementById('Suid_bin');
let suid_bin_sec = document.getElementById('Suid_Bin');
let interval12 = document.getElementById('interval12');
let snap7 = document.getElementById('snap7');
function btn12() {
    if (sbstate == 0) {
        suid_bin.checked = true;
        interval12.required = true;
        snap7.required = true;
        suid_bin_div.style.display = "flex";
        suid_bin_sec.style.height = "10%";
        suid_bin_sec.style.background = "#31343b";
        sbstate = 1;
    }
    else if (sbstate == 1) {
        suid_bin.checked = false;
        interval12.required = false;
        snap7.required = false;
        suid_bin_div.style.display = "none";
        suid_bin_sec.style.height = "0px";
        suid_bin_sec.style.background = "transparent";
        sbstate = 0;
    }
}
// suid_bin ends

// process_open_sockets
let posstate = 0;
let process_open_sockets = document.getElementById('process_open_sockets');
let process_open_sockets_div = document.getElementById('Process_open_sockets');
let process_open_sockets_sec = document.getElementById('Process_Open_Sockets');
let interval13 = document.getElementById('interval13');
let snap8 = document.getElementById('snap8');
function btn13() {
    if (posstate == 0) {
        process_open_sockets.checked = true;
        interval13.required = true;
        snap8.required = true;
        process_open_sockets_div.style.display = "flex";
        process_open_sockets_sec.style.height = "10%";
        process_open_sockets_sec.style.background = "#31343b";
        posstate = 1;
    }
    else if (posstate == 1) {
        process_open_sockets.checked = false;
        interval13.required = false;
        snap8.required = false;
        process_open_sockets_div.style.display = "none";
        process_open_sockets_sec.style.height = "0px";
        process_open_sockets_sec.style.background = "transparent";
        posstate = 0;
    }
}
// process_open_sockets ends

// users
let ustate = 0;
let users = document.getElementById('users');
let users_div = document.getElementById('Users');
let users_sec = document.getElementById('UserS');
let interval14 = document.getElementById('interval14');
let snap9 = document.getElementById('snap9');
function btn14() {
    if (ustate == 0) {
        users.checked = true;
        interval14.required = true;
        snap9.required = true;
        users_div.style.display = "flex";
        users_sec.style.height = "10%";
        users_sec.style.background = "#31343b";
        ustate = 1;
    }
    else if (ustate == 1) {
        users.checked = false;
        interval14.required = false;
        snap9.required = false;
        users_div.style.display = "none";
        users_sec.style.height = "0px";
        users_sec.style.background = "transparent";
        ustate = 0;
    }
}
// users ends

// groups
let gstate = 0;
let groups = document.getElementById('groups');
let groups_div = document.getElementById('Groups');
let groups_sec = document.getElementById('GroupS');
let interval15 = document.getElementById('interval15');
let snap10 = document.getElementById('snap10');
function btn15() {
    if (gstate == 0) {
        groups.checked = true;
        interval15.required = true;
        snap10.required = true;
        groups_div.style.display = "flex";
        groups_sec.style.height = "10%";
        groups_sec.style.background = "#31343b";
        gstate = 1;
    }
    else if (gstate == 1) {
        groups.checked = false;
        interval15.required = false;
        snap10.required = false;
        groups_div.style.display = "none";
        groups_sec.style.height = "0px";
        groups_sec.style.background = "transparent";
        gstate = 0;
    }
}
// groups ends

// process_envs
let penstate = 0;
let process_envs = document.getElementById('process_envs');
let process_envs_div = document.getElementById('Process_envs');
let process_envs_sec = document.getElementById('Process_Envs');
let interval16 = document.getElementById('interval16');
let snap11 = document.getElementById('snap11');
function btn16() {
    if (penstate == 0) {
        process_envs.checked = true;
        interval16.required = true;
        snap11.required = true;
        process_envs_div.style.display = "flex";
        process_envs_sec.style.height = "10%";
        process_envs_sec.style.background = "#31343b";
        penstate = 1;
    }
    else if (penstate == 1) {
        process_envs.checked = false;
        interval16.required = false;
        snap11.required = false;
        process_envs_div.style.display = "none";
        process_envs_sec.style.height = "0px";
        process_envs_sec.style.background = "transparent";
        penstate = 0;
    }
}

// memory_info
let mistate = 0;
let memory_info = document.getElementById('memory_info');
let memory_info_div = document.getElementById('Memory_info');
let memory_info_sec = document.getElementById('Memory_Info');
let interval17 = document.getElementById('interval17');
function btn17() {
    if (mistate == 0) {
        memory_info.checked = true;
        interval17.required = true;
        memory_info_div.style.display = "flex";
        memory_info_sec.style.height = "10%";
        memory_info_sec.style.background = "#31343b";
        mistate = 1;
    }
    else if (mistate == 1) {
        memory_info.checked = false;
        interval17.required = false;
        memory_info_div.style.display = "none";
        memory_info_sec.style.height = "0px";
        memory_info_sec.style.background = "transparent";
        mistate = 0;
    }
}
// memory_info

// cpu_time
let ctstate = 0;
let cpu_time = document.getElementById('cpu_time');
let cpu_time_div = document.getElementById('Cpu_time');
let cpu_time_sec = document.getElementById('Cpu_Time');
let interval18 = document.getElementById('interval18');
function btn18() {
    if (ctstate == 0) {
        cpu_time.checked = true;
        interval17.required = true;
        cpu_time_div.style.display = "flex";
        cpu_time_sec.style.height = "10%";
        cpu_time_sec.style.background = "#31343b";
        ctstate = 1;
    }
    else if (ctstate == 1) {
        cpu_time.checked = false;
        interval17.required = false;
        cpu_time_div.style.display = "none";
        cpu_time_sec.style.height = "0px";
        cpu_time_sec.style.background = "transparent";
        ctstate = 0;
    }
}
// cpu_time