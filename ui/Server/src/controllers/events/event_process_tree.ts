import { Request, Response } from "express";
import { dbSourceConnection } from "index";

// interface BpfProcessEntry {
//     id: number;
//     host_identifier: string;
//     hostname: string;
//     unixtime: number | string;
//     tid: number;
//     pid: number;
//     parent: number;
//     uid: number;
//     gid: number;
//     cid: number;
//     exit_code: string;
//     probe_error: number;
//     syscall: string;
//     path: string;
//     cwd: string;
//     cmdline: string;
//     duration: number;
//     json_cmdline: string;
//     ntime: string;
//     time: number | string;
//     eid: number;
//     md5: string;
//     sha1: string;
//     sha256: string;
//     is_container_process: number;
//     container_age: number;
//     container_name: string;
//     process_tree: string;
//     uptime: number | string;
//     is_processed: boolean;
//     is_processed_py: boolean;
//     is_processed_rule_builder: boolean;
// }

// interface BpfFileEntry {
//     id: number;
//     host_identifier: string;
//     hostname: string;
//     unixtime: number;
//     tid: number;
//     pid: number;
//     parent: number;
//     uid: number;
//     gid: number;
//     cid: number;
//     exit_code: string;
//     probe_error: number;
//     syscall: string;
//     path: string;
//     file_path: string;
//     flags: string;
//     ntime: string;
//     duration: number;
//     time: number;
//     process_tree: string;
//     uptime: number;
//     is_processed: boolean;
//     is_processed_py: boolean;
//     is_processed_rule_builder: boolean;
//   }

export const event_process_tree = async (req: Request, res: Response) => {

  const parameters = req.body;

  console.log("parameters", parameters);

  let error_json;

  let record_id: number;
  let os_name: string;
  let host_identifier: string, alert_type: string;
  let process_tree: any = {
    root_pid: '',
    graph: []
  };

  if (parameters.hasOwnProperty("os_name")) {
    os_name = parameters["os_name"];
  }
  if (parameters.hasOwnProperty("host_identifier")) {
    host_identifier = parameters["host_identifier"];
  }
  if (parameters.hasOwnProperty("id")) {
    record_id = parameters["id"];
  }
  if (parameters.hasOwnProperty("alert_type")) {
    alert_type = parameters["alert_type"];
  }
  
  let entityManager : any;

  try {
    entityManager = dbSourceConnection()[1];
  } catch(error) {
    console.log(error)
  }

  if(os_name == 'Linux') {
    try {
      const soc_event: boolean = parameters["socket_event"];
      const file_event: boolean = parameters["file_event"];
      let process_list: any = [];
  
      // below code is to make backward process tree
      if (alert_type === "File Alert") {
        const _tmp_process_list: any = await getBpfFileTree(entityManager, record_id);
        process_list.push(..._tmp_process_list);
        console.log(process_list.length);
      } else if (alert_type === "Socket Alert") {
        const _tmp_process_list: any = await getBpfSocketTree(entityManager, record_id);
        process_list.push(..._tmp_process_list);
        console.log(process_list.length);
      } else {
        const _tmp_process_list: any = await eventBpfBacktrackProcessTree(entityManager, record_id);
        // console.log("list", _tmp_process_list);
        process_list.push(..._tmp_process_list);
        console.log("length of process_list", process_list.length);
      }
  
      let pid: number;
      let result: any;
      if (alert_type === "Socket Alert") {
        const querySocketPid = "SELECT pid FROM bpf_socket_events WHERE id = $1";
        result = await entityManager.query(querySocketPid, [record_id]);
        pid = result[0].pid;
      } else if (alert_type === "File Alert") {
        const queryFilePid = "SELECT pid FROM bpf_file_events WHERE id = $1";
        result = await entityManager.query(queryFilePid, [record_id]);
        pid = result[0].pid;
      } else {
        const queryProcessPid = "SELECT pid FROM bpf_process_events WHERE id = $1";
        result = await entityManager.query(queryProcessPid, [record_id]);
        pid = result[0].pid;
      }
  
      const queryProcessEventId = "SELECT id FROM bpf_process_events WHERE pid = $1 AND host_identifier = $2 ORDER BY id ASC LIMIT 1";
      result = await entityManager.query(queryProcessEventId, [pid, host_identifier]);
      
      // console.log("rndm", result);
  
      if (result.length > 0) {
  
        const queryProcessDataFromId = "SELECT * FROM bpf_process_events WHERE id = $1";
        result = await entityManager.query(queryProcessDataFromId, [record_id]);
      }
  
      if(result.length == 0) {
        console.error(`No process found with record ID bpf ${record_id}`);
      }
      else if(result.length != 1) {
        console.error(`Multiple processes found with record ID ${record_id}`);
      }
      else {
        let loop = true;
        const pid = result[0].pid;
    
        const host_identifier = result[0].host_identifier;
    
        let timestamp = result[0].time;
        let uptime = result[0].uptime;
        const current_time = Math.floor(Date.now() / 1000);
  
        const s_current_time = current_time.toString();
    
        const queue: number[] = [pid];
  
        while(queue.length > 0) {
          const queryProcessDataExecve = "SELECT * FROM bpf_process_events WHERE cmdline <> '' AND parent = $1 AND host_identifier = $2 AND unixtime BETWEEN $3 AND $4 ORDER BY time ASC LIMIT 10";
          result = await entityManager.query(queryProcessDataExecve, [queue[0], host_identifier, timestamp, s_current_time]);
  
          if (result.length == 0)
          {
            const queryProcessDataNonExecve = "SELECT * FROM bpf_process_events WHERE cmdline = '' AND parent = $1 AND host_identifier = $2 AND unixtime BETWEEN $3 AND $4 ORDER BY time ASC LIMIT 10";
            result = await entityManager.query(queryProcessDataNonExecve, [queue[0], host_identifier, timestamp, s_current_time]);
          }
  
          if(result.length > 0) {
            for (let i = 0; i < result.size(); ++i) {
              const newTimestamp = result.at(i)["time"] as number;
              const newUptime = result.at(i)["uptime"] as number;
          
              const timestampDelta = timestamp - newTimestamp;
              const uptimeDelta = uptime - newUptime;
          
              if (Math.abs(timestampDelta - uptimeDelta) > 3) {
                loop = false;
                continue;
              } else {
                timestamp = newTimestamp;
                uptime = newUptime;
              }
          
              const path = result.at(i)["path"] as string;
              const childPid = result.at(i)["pid"] as number;
              queue.push(result.at(i)["pid"] as number);
              timestamp = result.at(i)["time"] as number;
          
              const entry: any = {
                pid: childPid,
                tid: result.at(i)["tid"] as number,
                uid: result.at(i)["uid"] as number,
                gid: result.at(i)["gid"] as number,
                cid: result.at(i)["cid"] as number,
                cmdline: result.at(i)["cmdline"] as string,
                exit_code: result.at(i)["exit_code"] as string,
                probe_error: result.at(i)["probe_error"] as number,
                syscall: result.at(i)["syscall"] as string,
                path: path,
                cwd: result.at(i)["cwd"] as string,
                duration: result.at(i)["duration"] as number,
                json_cmdline: result.at(i)["json_cmdline"] as string,
                ntime: result.at(i)["ntime"] as string,
                time: timestamp,
                eid: result.at(i)["eid"] as number,
                md5: result.at(i)["md5"] as string,
                sha1: result.at(i)["sha1"] as string,
                sha256: result.at(i)["sha256"] as string,
                is_container_process: result.at(i)["is_container_process"] as number,
                container_age: result.at(i)["container_age"] as number,
                container_name: result.at(i)["container_name"] as string,
                uptime: result.at(i)["uptime"] as number,
                ppid: result.at(i)["parent"] as number,
                root_pid: result.at(i)["pid"] as number,
                event_type: "Process Event",
                parent: queue[0]
              };
          
              process_list.push(entry);
          
              if (soc_event) {
                const resultSocket = await entityManager.query("select * from bpf_socket_events where pid = $1 and host_identifier = $2 order by id asc limit 1", [childPid, host_identifier]);
                if (resultSocket.length > 0) {
                  const entrySocket: any = {
                    path: resultSocket.at(0)["path"] as string,
                    host_identifier: resultSocket.at(0)["host_identifier"] as string,
                    hostname: resultSocket.at(0)["hostname"] as string,
                    ppid: resultSocket.at(0)["parent"] as number,
                    uid: resultSocket.at(0)["uid"] as number,
                    gid: resultSocket.at(0)["gid"] as number,
                    cid: resultSocket.at(0)["cid"] as number,
                    exit_code: resultSocket.at(0)["exit_code"] as string,
                    syscall: resultSocket.at(0)["syscall"] as string,
                    protocol: resultSocket.at(0)["protocol"] as number,
                    local_address: resultSocket.at(0)["local_address"] as string,
                    remote_address: resultSocket.at(0)["remote_address"] as string,
                    local_port: resultSocket.at(0)["local_port"] as number,
                    remote_port: resultSocket.at(0)["remote_port"] as number,
                    duration: resultSocket.at(0)["duration"] as number,
                    event_type: "Socket Event",
                    timestamp: resultSocket.at(0)["time"] as number,
                    uptime: resultSocket.at(0)["uptime"] as number
                  };
                  process_list.push(entrySocket);
                }
              }
          
              if (file_event) {
                const resultFile = await entityManager.query("select * from bpf_file_events where pid = $1 and host_identifier = $2 order by id asc limit 1", [childPid, host_identifier]);
                if (resultFile.length > 0) {
                  const entryFile: any = {
                    pid: resultFile.at(0)["pid"] as number,
                    path: resultFile.at(0)["path"] as string,
                    host_identifier: resultFile.at(0)["host_identifier"] as string,
                    hostname: resultFile.at(0)["hostname"] as string,
                    ppid: resultFile.at(0)["parent"] as number,
                    uid: resultFile.at(0)["uid"] as number,
                    gid: resultFile.at(0)["gid"] as number,
                    cid: resultFile.at(0)["cid"] as number,
                    exit_code: resultFile.at(0)["exit_code"] as string,
                    syscall: resultFile.at(0)["syscall"] as string,
                    file_path: resultFile.at(0)["file_path"] as string,
                    flags: resultFile.at(0)["flags"] as string,
                    duration: resultFile.at(0)["duration"] as number,
                    event_type: "File Event",
                    timestamp: resultFile.at(0)["time"] as number,
                    uptime: resultFile.at(0)["uptime"] as number
                  };
                  process_list.push(entryFile);
                }
              }
              queue.shift();
            }
          } else {
            loop = false;
            queue.shift();
          }
        }
      }
  
      let smallestRootPid: number;
      if (process_list.length > 1) {
        let minRootPid = process_list[0].root_pid;
        for (let i = 1; i < process_list.length; i++) {
            if (process_list[i].root_pid < minRootPid) {
                minRootPid = process_list[i].root_pid;
            }
        }
  
        // Store the smallest value in smallestRootPid
        smallestRootPid = minRootPid;
  
        let graph: any[] = [], j: any[] = [];
  
        for (const entry of process_list) {
          const obj: any = {
            pid: entry.pid,
            parent: entry.ppid,
            tid: entry.tid,
            uid: entry.uid,
            gid: entry.gid,
            cid: entry.cid,
            exit_code: entry.exit_code,
            probe_error: entry.probe_error,
            syscall: entry.syscall,
            path: entry.path,
            cwd: entry.cwd,
            cmdline: entry.cmdline,
            duration: entry.duration,
            json_cmdline: entry.json_cmdline,
            ntime: entry.ntime,
            time: entry.time,
            eid: entry.eid,
            md5: entry.md5,
            sha1: entry.sha1,
            sha256: entry.sha256,
            is_container_process: entry.is_container_process,
            container_age: entry.container_age,
            container_name: entry.container_name,
            fd: entry.fd,
            family: entry.family,
            protocol: entry.protocol,
            local_address: entry.local_address,
            remote_address: entry.remote_address,
            local_port: entry.local_port,
            remote_port: entry.remote_port,
            uptime: entry.uptime,
            file_path: entry.file_path,
            flags: entry.flags,
            event_type: entry.event_type,
            // parent: entry.parent,
            root_pid: smallestRootPid,
          };
          graph.push(obj);
        }
        
        j["root_pid"] = smallestRootPid;
        j["graph"] = graph;
    
        process_tree = j;    

      } else {
        error_json = {
          status: 'Fail',
          message: 'No process tree found',
        }
      }
    } catch(error) {
      console.error(error);
      return res.status(500).json({ message: 'Internal Server Error' });
    }
  }
  else if(os_name == "Windows") {
    const soc_event: boolean = parameters["socket_event"];
    const file_event: boolean = parameters["file_event"];
    const process_list: any = [];

    if (alert_type === "File Alert") {
      const _tmp_process_list: any = await getBpfFileTree(entityManager, record_id);
      process_list.push(..._tmp_process_list);
      console.log(process_list.length);
    } else if (alert_type === "Socket Alert") {
      const _tmp_process_list: any = await getBpfSocketTree(entityManager, record_id);
      process_list.push(..._tmp_process_list);
      console.log(process_list.length);
    } else {
      const _tmp_process_list: any = await eventBpfBacktrackProcessTree(entityManager, record_id);
      process_list.push(..._tmp_process_list);
      console.log(process_list.length);
    }

    let result: any;

    let process_guid: string, parent_process_guid: string;

    if (alert_type === "Socket Alert") {
      const querySocketPid = "select process_guid from win_socket_events where id = $1";
      result = await entityManager.query(querySocketPid, [record_id]);
      process_guid = result[0].pid;
    } else if (alert_type === "File Alert") {
      const queryFilePid = "select process_guid from win_file_events where id = $1";
      result = await entityManager.query(queryFilePid, [record_id]);
      process_guid = result[0].pid;
    } else {
      const queryProcessPid = "select process_guid from win_process_events where id = $1";
      result = await entityManager.query(queryProcessPid, [record_id]);
      process_guid = result[0].pid;
    }

    const queryProcessEventId = "select id from win_process_events where process_guid = $1 and host_identifier = $2 order by id asc limit 1";
    result = await entityManager.query(queryProcessEventId, [process_guid, host_identifier]);

    if (result.length > 0) {

      const queryProcessDataFromId = "SELECT * FROM win_process_events WHERE id = $1";
      result = await entityManager.query(queryProcessDataFromId, [record_id]);
    }

    if(result.length == 0) {
      console.error(`No process found with record ID bpf ${record_id}`);
    }
    else if(result.length != 1) {
      console.error(`Multiple processes found with record ID ${record_id}`);
    }
    else {
      let loop = true;

      process_guid = result[0].process_guid;
      parent_process_guid = result[0].parent_process_guid;
  
      const host_identifier = result[0].host_identifier;
  
      let timestamp = result[0].time;
      let uptime = result[0].uptime;
      const current_time = Math.floor(Date.now() / 1000);

      const s_current_time = current_time.toString();
  
      const queue: any[] = [process_guid];

      while(queue.length > 0) {
        const queryProcessDataExecve = "SELECT * FROM win_process_events WHERE cmdline <> '' AND parent = $1 AND host_identifier = $2 AND unixtime BETWEEN $3 AND $4 ORDER BY time ASC LIMIT 10";
        result = await entityManager.query(queryProcessDataExecve, [queue[0], host_identifier, timestamp, s_current_time]);

        if (result.length == 0)
        {
          const queryProcessDataNonExecve = "SELECT * FROM win_process_events WHERE cmdline = '' AND parent = $1 AND host_identifier = $2 AND unixtime BETWEEN $3 AND $4 ORDER BY time ASC LIMIT 10";
          result = await entityManager.query(queryProcessDataNonExecve, [queue[0], host_identifier, timestamp, s_current_time]);
        }

        if(result.length > 0) {
          for (let i = 0; i < result.size(); ++i) {
            const newTimestamp = result.at(i)["time"] as number;
            const newUptime = result.at(i)["uptime"] as number;
        
            const timestampDelta = timestamp - newTimestamp;
            const uptimeDelta = uptime - newUptime;
        
            if (Math.abs(timestampDelta - uptimeDelta) > 3) {
              loop = false;
              continue;
            } else {
              timestamp = newTimestamp;
              uptime = newUptime;
            }
        
            const path = result.at(i)["path"] as string;
            const cmdline = result.at(i)["cmdline"] as string;
            const child_process_guid = result.at(i)["process_guid"] as string;
            queue.push(result.at(i)["process_guid"] as string);
            timestamp = result.at(i)["time"] as number;
        
            const entry: any = {
              process_guid: child_process_guid,
              action: result.at(i)["action"] as string,
              eid: result.at(i)["eid"] as string,
              pid: result.at(i)["pid"] as number,
              path: path,
              cmdline: cmdline,
              ppid: result.at(i)["parent_pid"] as number,
              parent_process_guid: result.at(i)["parent_process_guid"] as string,
              parent_path: result.at(i)["parent_path"] as string,
              owner_uid: result.at(i)["owner_uid"] as string,
              time: result.at(i)["time"] as number,
              utc_time: result.at(i)["utc_time"] as string,
              eventid: result.at(i)["eventid"] as number,
              sha256: result.at(i)["sha256"] as string,
              uptime: result.at(i)["uptime"] as number,
              event_type: "Process Event",
              root_pid: result.at(i)["pid"] as number
            };

            entry.parent_process_guid = queue[0];
        
            process_list.push(entry);
        
            if (soc_event) {
              const result_socket = await entityManager.query("select * from win_socket_events where pid = $1 and host_identifier = $2 order by id asc limit 1", [child_process_guid, host_identifier]);
              if (result_socket.length > 0) {
                const entry_soc: any = {
                  pid: result_socket.at(0)["pid"] as number,
                  event_type: result_socket.at(0)["event_type"] as string,
                  host_identifier: result_socket.at(0)["host_identifier"] as string,
                  hostname: result_socket.at(0)["hostname"] as string,
                  parent: result_socket.at(0)["parent_pid"] as number,
                  action: result_socket.at(0)["action"] as number,
                  eid: result_socket.at(0)["eid"] as string,
                  timestamp: result_socket.at(0)["time"] as number,
                  utc_time: result_socket.at(0)["utc_time"] as string,
                  process_guid: result_socket.at(0)["process_guid"] as string,
                  process_name: result_socket.at(0)["process_name"] as string,
                  family: result_socket.at(0)["family"] as number,
                  protocol: result_socket.at(0)["protocol"] as number,
                  local_address: result_socket.at(0)["local_address"] as string,
                  remote_address: result_socket.at(0)["remote_address"] as string,
                  local_port: result_socket.at(0)["local_port"] as number,
                  remote_port: result_socket.at(0)["remote_port"] as number,
                  eventid: result_socket.at(0)["eventid"] as number,
                  // event_type: "Socket Event",    //TODO uncomment
                  unixtime: result_socket.at(0)["unixTime"] as number,
                  root_pid: result.at(0)["pid"] as number,
                  uptime: result_socket.at(0)["uptime"] as number,
                };
                process_list.push(entry_soc);
              }
                
            }
        
            if (file_event) {
              const result_file = await entityManager.query("select * from win_file_events where pid = $1 and host_identifier = $2 order by id asc limit 1", [child_process_guid, host_identifier]);
              if (result_file.length > 0) {
                const entry_file: any = {
                  pid: result_file.at(0)["pid"] as number,
                  host_identifier: result_file.at(0)["host_identifier"] as string,
                  hostname: result_file.at(0)["hostname"] as string,
                  parent: result_file.at(0)["parent_pid"] as number,
                  action: result_file.at(0)["action"] as string,
                  eid: result_file.at(0)["eid"] as string,
                  target_path: result_file.at(0)["target_path"] as string,
                  md5: result_file.at(0)["md5"] as string,
                  sha256: result_file.at(0)["sha256"] as string,
                  hashed: result_file.at(0)["hashed"] as number,
                  uid: result_file.at(0)["uid"] as string,
                  utc_time: result_file.at(0)["utc_time"] as string,
                  unixtime: result_file.at(0)["unixTime"] as number,
                  timestamp: result_file.at(0)["time"] as number,
                  uptime: result_file.at(0)["uptime"] as number,
                  process_guid: result_file.at(0)["process_guid"] as string,
                  process_name: result_file.at(0)["process_name"] as string,
                  pe_file: result_file.at(0)["pe_file"] as string,
                  root_pid: result.at(0)["pid"] as number,
                  eventid: result_file.at(0)["eventid"] as number,
                  event_type: "File Event"
                };
                
                process_list.push(entry_file);

              }
            }
            queue.shift();
          }
        } else {
          loop = false;
          queue.shift();
        }
      }
    }

    let smallestRootPid: number;

    console.log("process_list", process_list);

    if (process_list.length > 1) {
      let minRootPid = process_list[0].root_pid;
      for (let i = 1; i < process_list.length; i++) {
          if (process_list[i].root_pid < minRootPid) {
              minRootPid = process_list[i].root_pid;
          }
      }

      // Store the smallest value in smallestRootPid
      smallestRootPid = minRootPid;

      let graph: any[] = [], j: any[] = [];

      for (const entry of process_list) {
        const obj: any = {};
    
    
        obj["pid"] = entry.pid;
        obj["parent_pid"] = entry.ppid;
        obj["action"] = entry.action;
        // obj["eid"] = entry.eid;
        obj["process_guid"] = entry.process_guid;
        obj["path"] = entry.path;
        obj["cmdline"] = entry.cmdline;
        obj["parent_process_guid"] = entry.parent_process_guid;
        obj["parent_path"] = entry.parent_path;
        obj["owner_uid"] = entry.owner_uid;
        // obj["eventid"] = entry.eventid;
        obj["sha256"] = entry.sha256;
        obj["family"] = entry.family;
        obj["protocol"] = entry.protocol;
        obj["local_address"] = entry.local_address;
        obj["remote_address"] = entry.remote_address;
        obj["local_port"] = entry.local_port;
        obj["remote_port"] = entry.remote_port;
        obj["target_path"] = entry.target_path;
        obj["hashed"] = entry.hashed;
        obj["pe_file"] = entry.pe_file;
        // obj["parent_pid"] = entry.parent;
        obj["event_type"] = entry.event_type;
        obj["root_pid"] = smallestRootPid;
    
        graph.push(obj);
      }
    
      j["root_pid"] = smallestRootPid;
      j["graph"] = graph;
  
      process_tree = j;
    } else {
      error_json = {
        status: 'Fail',
        message: 'No process tree found',
      }
    }
    
  } 
  else {
    error_json = {
      status: 'Fail',
      message: 'Missing required parameters',
    };
  }

  if(process_tree.root_pid != '') {
    const responseObj = {
      root_pid: process_tree.root_pid,
      graph: process_tree.graph
    };
    return res.status(200).json(responseObj);
  } else {
    return res.status(400).json(error_json);
  }

}

async function eventBpfBacktrackProcessTree(
  entityManager: any,
  record_id: number
): Promise<any[]> { // Using 'any' type for process_list
  const process_list: any[] = [];

  try {
    // Raw SQL queries (replace with your actual queries)
    const queryGetBpfProcessDataFromId = `
      SELECT *
      FROM bpf_process_events
      WHERE id = $1
    `;

    const queryGetBpfProcessDataExecve = `
      SELECT *
      FROM bpf_process_events
      WHERE cmdline <> '' AND pid = $1 AND host_identifier = $2
      ORDER BY time DESC
      LIMIT 1
    `;

    const queryGetBpfProcessDataNonExecve = `
      SELECT *
      FROM bpf_process_events
      WHERE cmdline = '' AND pid = $1 AND host_identifier = $2
      ORDER BY time DESC
      LIMIT 1
    `;

    const queryGetDockerEvents = `
      SELECT *
      FROM docker_events
      WHERE status = 'start' AND time BETWEEN $1 AND $2
        AND pid = $3 AND host_identifier = $4
    `;

    // Get process data from bpf_process_events
    let result = await entityManager.query(
      queryGetBpfProcessDataFromId,
      [record_id]
    );

    // Process entry from resultGetBpfProcessDataFromId
    const entry = {
      pid: result[0].pid,
      ppid: result[0].parent,
      tid: result[0].tid,
      uid: result[0].uid,
      gid: result[0].gid,
      cid: result[0].cid,
      cmdline: result[0].cmdline,
      exit_code: result[0].exit_code,
      probe_error: result[0].probe_error,
      syscall: result[0].syscall,
      path: result[0].path,
      cwd: result[0].cwd,
      duration: result[0].duration,
      json_cmdline: result[0].json_cmdline,
      ntime: result[0].ntime,
      time: result[0].time,
      eid: result[0].eid,
      md5: result[0].md5,
      sha1: result[0].sha1,
      sha256: result[0].sha256,
      is_container_process: result[0].is_container_process,
      container_age: result[0].container_age,
      container_name: result[0].container_name,
      uptime: result[0].uptime,
      root_pid: result[0].pid,
      event_type: "Process Event",
    };
    
    process_list.push(entry);    

    if(result.length == 0) {
      console.error(`No process found with record ID bpf ${record_id}`);
      return [];
    }
    else if (result.length !== 1) {
      console.error(`Multiple processes found with record ID ${record_id}`);
      return process_list;
    } 
    else {
      let loop = true;
      let ppid = result[0]["parent"] as number;
      let pid = result[0]["pid"] as number;

      let path: string, cmdline: string, host_identifier: string, container_name: string, container_id: string;
      let container_pid: number;
      path = result[0]["path"] as string;
      cmdline = result[0]["cmdline"] as string;
      host_identifier = result[0]["host_identifier"] as string;

      let timestamp = result[0]["time"] as number;
      let uptime = result[0]["uptime"] as number;

      // Checking for container process
      const container_result = await entityManager.query(
        queryGetDockerEvents,
        [timestamp - 3600, timestamp + 3600, ppid, host_identifier]
      );

      if (container_result.length > 0) {
        container_pid = container_result[0]["pid"] as number;
        container_name = container_result[0]["container_name"] as string;
        container_id = container_result[0]["container_id"] as string;

        if (pid == container_pid || ppid == container_pid) {
          const queryUpdateContainerProcess = `update bpf_process_events set is_container_process = '1' where id = $1`;
          await entityManager.query(queryUpdateContainerProcess, [record_id]);
        }
      }

      while (loop) {
        result = await entityManager.query(queryGetBpfProcessDataExecve, [ppid, host_identifier]);

        let entry: any = {};

        if (result.length == 0) {
          result = await entityManager.query(queryGetBpfProcessDataNonExecve, [ppid, host_identifier]);
        }

        if (result.length > 0) {
          const new_timestamp = result[0]["time"] as number;
          const new_uptime = result[0]["uptime"] as number;

          const timestamp_delta = timestamp - new_timestamp;
          const uptime_delta = uptime - new_uptime;

          if (Math.abs(timestamp_delta - uptime_delta) > 3) {
            loop = false;
            continue;
          } else {
            timestamp = new_timestamp;
            uptime = new_uptime;
          }
          
          path = result[0]["path"] as string;
          cmdline = result[0]["cmdline"] as string;
          const tmp_ppid = result[0]["parent"] as number;

          timestamp = result[0]["time"] as number;

          entry.pid = ppid;
          entry.ppid = tmp_ppid;

          entry.tid = result[0]["tid"] as bigint; // Use appropriate type for bigint
          entry.uid = result[0]["uid"] as bigint; // Use appropriate type for bigint
          entry.gid = result[0]["gid"] as bigint; // Use appropriate type for bigint
          entry.cid = result[0]["cid"] as bigint; // Use appropriate type for bigint
          entry.exit_code = result[0]["exit_code"] as string;
          entry.probe_error = result[0]["probe_error"] as bigint; // Use appropriate type for bigint
          entry.syscall = result[0]["syscall"] as string;
          entry.path = path;
          entry.cwd = result[0]["cwd"] as string;
          entry.cmdline = cmdline;
          entry.duration = result[0]["duration"] as bigint; // Use appropriate type for bigint
          entry.json_cmdline = result[0]["json_cmdline"] as string;
          entry.ntime = result[0]["ntime"] as string;
          entry.time = timestamp;
          entry.eid = result[0]["eid"] as bigint; // Use appropriate type for bigint
          entry.md5 = result[0]["md5"] as string;
          entry.sha1 = result[0]["sha1"] as string;
          entry.sha256 = result[0]["sha256"] as string;
          entry.is_container_process = result[0]["is_container_process"] as bigint; // Use appropriate type for bigint
          entry.container_age = result[0]["container_age"] as bigint; // Use appropriate type for bigint
          entry.container_name = result[0]["container_name"] as string;
          entry.event_type = "Process Event";

          ppid = tmp_ppid;

          process_list.push(entry);

        } else {
          loop = false;
        }
      }

      const _tmp_process_list = await snapshotProcessTree(entityManager, pid, timestamp, uptime, host_identifier);
      console.log("this is the snapshot data", _tmp_process_list);
      process_list.push(..._tmp_process_list);

    }

      return process_list;
    } catch (error) {
      console.error(error);
      throw error;
    }
  }

async function snapshotProcessTree(entityManager: any, ppid: number, timestamp: number, uptime: number, host_identifier: string) : Promise<any[]> {
  try {
    const process_list: any[] = [];
    const root: number[] = [];

    const query_before = "SELECT * FROM snapshot_result WHERE unixTime < $1 AND query_name = $2 AND host_identifier = $3 ORDER BY id DESC LIMIT 1";
    const result_before = await entityManager.query(query_before, [timestamp, "processes", host_identifier]);

    const query_after = "SELECT * FROM snapshot_result WHERE unixTime > $1 AND query_name = $2 AND host_identifier = $3 ORDER BY id ASC LIMIT 1";
    const result_after = await entityManager.query(query_after, [timestamp, "processes", host_identifier]);

    const result = result_before.length > 0 ? result_before : result_after;

    if (result.length > 0) {
      const processes = result[0]["result"];

      const entry = getParentFromSnapshot(processes, ppid);
      let _tmp_pid = entry.ppid;
      let r_id: number | undefined;

      if (entry.pid) {
        process_list.push(entry);
      }

      while (_tmp_pid !== 0) {
        const entry = getParentFromSnapshot(processes, _tmp_pid);
        if (entry.pid !== 0) {
            r_id = entry.pid;
        }

        if (r_id !== 0) {
            root.push(r_id);
        }
        entry.root_pid = root[root.length - 1];
        process_list.push(entry);
        _tmp_pid = entry.ppid;
      }
    }

    return process_list;

  } catch (error) {
      console.error(error);
      return [];
  }
}

async function getBpfSocketTree(entityManager: any, record_id: number): Promise<any[]> {
  try {
      const process_list: any[] = [];

      const querySocketData = "SELECT * FROM bpf_socket_events WHERE id = $1";
      const resultSocket = await entityManager.query(querySocketData, [record_id]);

      if (resultSocket.length === 0) {
          console.error(`No socket event found with record ID ${record_id}`);
          return process_list;
      }

      const socketEvent = resultSocket[0];
      const pid = socketEvent.pid;

      const path = socketEvent.path;
      const host_identifier = socketEvent.host_identifier;
      const timestamp = socketEvent.time;
      const uptime = socketEvent.uptime;

      const queryProcessData = "SELECT * FROM bpf_process_events WHERE pid = $1 "
          + "AND host_identifier = $2 AND time <= $3 AND path = $4 ORDER BY id DESC LIMIT 1";
      const resultProcess = await entityManager.query(queryProcessData, [pid, host_identifier, timestamp, path]);

      if (resultProcess.length === 0) {
          const snapshotprocess_list = await snapshotProcessTree(entityManager, pid, timestamp, uptime, host_identifier);
          process_list.push(...snapshotprocess_list);
      } else {
          const processEvent = resultProcess[0];
          const processRecordId = processEvent.id;
          const backtrackprocess_list = await eventBpfBacktrackProcessTree(entityManager, processRecordId);
          process_list.push(...backtrackprocess_list);
      }

      return process_list;
  } catch (error) {
      console.error(error);
      // Handle the error as needed
      return [];
  }
}

async function getBpfFileTree(entityManager: any, record_id: number): Promise<any> {
  try {
    const process_list: any = [];
    let result: any;

    {
      const entry_file: any = {};
      result = await entityManager.query("select * from bpf_file_events where id = $1", [record_id]);

      console.log("debug", result[0].pid);

      entry_file.pid = result[0].pid;
      entry_file.path = result[0].path;
      entry_file.host_identifier = result[0].host_identifier;
      entry_file.hostname = result[0].hostname;
      entry_file.ppid = result[0].parent;
      entry_file.uid = result[0].uid;
      entry_file.gid = result[0].gid;
      entry_file.cid = result[0].cid;
      entry_file.exit_code = result[0].exit_code;
      entry_file.syscall = result[0].syscall;
      entry_file.file_path = result[0].file_path;
      entry_file.flags = result[0].flags;
      entry_file.duration = result[0].duration;
      entry_file.root_pid = result[0].pid;
      entry_file.timestamp = result[0].time;
      entry_file.uptime = result[0].uptime;
      entry_file.event_type = "File Event";        
      process_list.push(entry_file);
    }

    if (result.length == 0) {
        console.error(`No process found with record ID ${record_id}`);
    } else if (result.length != 1) {
        console.error(`Multiple processes found with record ID ${record_id}`);
    } else {
        const pid = result[0].pid;
        const path = result[0].path;
        const host_identifier = result[0].host_identifier;
        const timestamp = result[0].time;
        const uptime = result[0].uptime;

        result = await entityManager.query(
          "select * from bpf_process_events where pid = $1 and host_identifier = $2 and time <= $3 and path = $4 order by id desc limit 1",
          [pid, host_identifier, timestamp, path]
        );

        if (result.length > 0) {
          const process_record_id = result[0].id;
          const _tmp_process_list = await eventBpfBacktrackProcessTree(entityManager, process_record_id);
          process_list.push(..._tmp_process_list);
        } else {
          /*
            * It seems that the socket event is coming from a process that was started *before* osquery was started.
            * We have to pick data from the snapshot.
            *
            * Since we have PID, instead of PPID, we have to remove the first entry in the process list.
            */
          const _tmp_process_list = await snapshotProcessTree(entityManager, pid, timestamp, uptime, host_identifier);
          process_list.push(..._tmp_process_list);
        }
      }
    return process_list;
  } catch (error: unknown) {
      if (error instanceof Error) {
        console.error(error.message);
      } else {
        console.error("Unknown error: could not get max node severity count");
      }
      return [];
  }
}  

function getParentFromSnapshot(processes: any[], pid: number): any {
  try {
    let ppid = 0;

    let entry: any = {};

    if (Array.isArray(processes)) {
      for (const process of processes) {
        if(
            process.hasOwnProperty("pid") &&
            process.hasOwnProperty("parent") &&
            process.hasOwnProperty("cmdline") &&
            process.hasOwnProperty("path")
          ) {
          if (Number(process["pid"]) === pid) {
            ppid = Number(process["parent"]);

            entry.pid = pid;
            entry.ppid = ppid;
            entry.path = process["path"];
            entry.cmdline = process["cmdline"];
            entry.cwd = process["cwd"];
            entry.root_pid = pid;
            entry.event_type = "Process Event";
          }
        }
      }
    }
    return entry;
  } catch (error) {
    console.error(error);
    // Handle the error as needed
    return [];
  }
}


