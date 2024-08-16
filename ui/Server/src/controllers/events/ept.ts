import { Request, Response } from 'express';
import { dbSourceConnection } from 'index';

const DELTA_LIMIT = 5;

const makeTree =(data: any, PID: number, os_name : string)=>{
    // console.log("PID: ",PID,"Data: ", data);
    const dataSet:any = data
    let parent = "parent";

    if(os_name == 'Windows'){
      parent = "ppid";
    }
    const tree:any = [];
    for (const data of dataSet) {
        // console.log("Data : ",data.pid, data.marked,Object.keys(data).includes('cmdline'),'\n');
        if (!Object.keys(data).includes('marked')) {
            if (data[parent] === PID) {
                data['marked'] = true;
                data['name'] = data['path'];
                data['attributes'] = { 'pid': data['pid'] };
                if (data['event_type']) {
                    data['attributes']['Event'] = data['event_type'];
                }
                const tmp: any = data;
                if (!Object.keys(tmp).includes('children')) {
                    tmp['children'] = makeTree(dataSet, tmp['pid'], os_name);
                }
                tree.push(tmp);
            }
        }
    }
    return tree;
}

const getParent = (dataSet : any, pid: number, os_name:string)=>{
   let parent = "parent";
   if(os_name == 'Windows'){
    parent = 'ppid';
   }
   for(const data of dataSet){
    if(data['pid'] == pid)
      return data[parent];
   }
}

function get_parent_from_snapshot(processes: any, pid: number): any {
  try {
    let ppid: number = 0;
    let entry: any = {};
    console.log("Get Parent Snapshot");
    if (Array.isArray(processes)) {
      for (const process of processes) {
        if (pid !==undefined &&
          process.pid !== undefined &&
          process.parent !== undefined &&
          process.cmdline !== undefined &&
          process.path !== undefined
        ) {
          if (process.pid === pid.toString()) {
            ppid = Number(process.parent);

            entry.pid = pid;
            entry.ppid = ppid;
            entry.path = process.path;
            entry.cmdline = process.cmdline;
            entry.cwd = process.cwd;
            entry.root_pid = pid;
            entry.event_type = 'Process Event';
          }
        }
      }
    }

    return entry;
  } catch (e) {
    console.error(e.message);
  }

  // Handle unknown errors here
  console.error('Unknown error');
  return [];
}

function get_win_parent_from_snapshot(processes: any, pid: number): any {
  try {
    let ppid: number = 0;
    let entry: any = {};

    if (Array.isArray(processes)) {
      for (const process of processes) {
        if (
          pid!=undefined &&
          process.pid !== undefined &&
          process.parent !== undefined &&
          process.cmdline !== undefined &&
          process.path !== undefined
        ) {
          if (process.pid === pid.toString()) {
            ppid = Number(process.parent);
            // console.log("Pid: ",pid,"\n", process);
            entry.pid = pid;
            entry.ppid = ppid;
            entry.path = process.path;
            entry.command_line = process.cmdline;
            entry.current_working_directory = process.cwd;
            entry.time = Number(process.start_time);
            entry.uptime = Number(process.elapsed_time);
            entry.root_pid = pid;
            entry.event_type = 'Process Event';
          }
        }
      }
    }

    return entry;
  } catch (e) {
    console.error(e.message);
  }

  // Handle unknown errors here
  console.error('Unknown error');
  return [];
}

async function snapshot_process_tree(
  connection: any,
  ppid: number,
  timestamp: number,
  uptime: number,
  host_identifier: string,
): Promise<any[]> {
  try {
    let process_list: any = [];
    let root: Number[] = [];

    let result: any;
    console.log("\n Making Tree from Snapshot ...");
    const getSnapshotDataBefore = `select * from snapshot_result where unixtime < $1 and query_name = $2 and host_identifier = $3 order by id desc limit 1`;
    const getSnapshotDataAfter = `select * from snapshot_result where unixtime > $1 and query_name = $2 and host_identifier = $3 order by id asc limit 1`;

    result = await connection.query(
      getSnapshotDataBefore,
      [timestamp, 'processes', host_identifier],
    );
    // console.log("Snapshot Result length: ",result.length);
    if (result.length == 0) {
      result = await connection.query(
        getSnapshotDataAfter,
        [timestamp, 'processes', host_identifier],
      );
    } else {
      /*
       * We need to ensure that previous snapshot record is from current sessions. Which means, that unixTime of
       * snapshot record must be greater than timestamp - uptime (i.e. timestamp of system startup).
       */
      let snapshot_unitxTime = Number(result[0].unixtime);

      if (snapshot_unitxTime < (timestamp - uptime)) {
        result = await connection.query(getSnapshotDataAfter,
          [timestamp, 'processes', host_identifier],
        );
      }
    }

    if (result.length != 0) {
      let raw_process_list = result[0].result;
     /*  Eg. Raw Process List = '[{"cmdline":"/sbin/init splash",...,"cwd":"/"}, {"cmdline":"/sbin/init splash",...,"cwd":"/"}]' */
      let processes = JSON.parse(raw_process_list);
      let entry = get_parent_from_snapshot(processes, ppid);

      let _tmp_pid = entry.ppid;
      let r_id: number;

      if (entry.pid) process_list.push(entry);

      while (_tmp_pid !== undefined && _tmp_pid!=0) {
        entry = get_parent_from_snapshot(processes, _tmp_pid);
        if (entry.pid != undefined && entry.pid!=0) {
          r_id = entry.pid;
        }

        if (r_id != undefined && r_id!=0) {
          root.push(r_id);
        }
        entry.root_pid = root[root.length - 1];
        process_list.push(entry);
        _tmp_pid = entry.ppid;
      }
    }
    // console.log("Result length: ",result.length);
    // console.log("Snap Tree: ",process_list);
    return process_list;
  } catch (error) {
    console.error(error);
    return [];
  }
}

async function win_snapshot_process_tree(
  connection: any,
  ppid: number,
  timestamp: number,
  uptime: number,
  host_identifier: string,
): Promise<any[]> {
  try {
    let process_list: any = [];
    let root: Number[] = [];

    let result: any;
    const getSnapshotDataBefore = `select * from snapshot_result where unixtime < $1 and query_name = $2 and host_identifier = $3 order by id desc limit 1`;
    const getSnapshotDataAfter = `select * from snapshot_result where unixtime > $1 and query_name = $2 and host_identifier = $3 order by id asc limit 1`;


    result = await connection.query(
    getSnapshotDataBefore,
      [timestamp, 'processes', host_identifier],
    );
    if (result.length == 0) {
      result = await connection.query(
        getSnapshotDataAfter
       , [timestamp, 'processes', host_identifier],
      );
    } else {
      /*
       * We need to ensure that previous snapshot record is from current sessions. Which means, that unixTime of
       * snapshot record must be greater than timestamp - uptime (i.e. timestamp of system startup).
       */
      let snapshot_unitxTime = Number(result[0].unixtime);
      if (snapshot_unitxTime < (timestamp - uptime)) {
        result = await connection.query(
          getSnapshotDataAfter,
          [timestamp, 'processes', host_identifier],
        );
      }
    }

    if (result.length != 0) {
      let raw_process_list = result[0].result;
      let processes = JSON.parse(raw_process_list);
      let entry = get_win_parent_from_snapshot(processes, ppid);
      let _tmp_pid = entry.ppid;
      let r_id: number;

      if (entry.pid) process_list.push(entry);

      while (_tmp_pid !== undefined) {
        entry = get_win_parent_from_snapshot(processes, _tmp_pid);
        if (entry.pid != undefined && entry.parent != undefined && entry.pid!=0) {
          r_id = entry.pid;
        }

        if (r_id != undefined && r_id!=0) {
          root.push(r_id);
        }
        entry.root_pid = root[root.length - 1];
        if(entry.pid!= undefined)
          process_list.push(entry);
        _tmp_pid = entry.ppid;
      }
    }
    root = [];
    return process_list;
  } catch (error) {
    console.error(error);
    return [];
  }
}

const event_bpf_backtrack_process_tree = async (connection: any, record_id: number): Promise<any[]> => {
  try {
    console.log("\nBPF Backtrack Process Tree in making...");
    const queryGetBpfProcessDataFromId = `select * from bpf_process_events where id = $1`;
    const getBpfProcessDataExecve = `select * from bpf_process_events where cmdline <> '' and pid = $1 and host_identifier = $2 order by time desc limit 1`;
    const getBpfProcessDataNonExecve = `select * from bpf_process_events where cmdline = '' and pid = $1 and host_identifier = $2 order by time desc limit 1`;
    const getDockerEvents = `select * from docker_events where status = 'start' and time between $1 and $2 and pid = $3 and host_identifier = $4`;

    /*
     * Since events can arrive out of order, and at some delay (in case osquery cannot reach fleet server for some reason, we cannot rely upon record ID in database.
     * If we want to get latest record of event, we should order by time column instead, which records timestamp at which the event occurred. Since it comes a
     * monotonic clock, it will never be out of order.
     */
    let process_list: any = [];
    let entry: any = {};
    let result: any;
    {
      result = await connection.query(queryGetBpfProcessDataFromId, [record_id]);

      entry.pid = Number(result[0].pid);
      entry.ppid = Number(result[0].parent);
      entry.tid = Number(result[0].tid);
      entry.uid = Number(result[0].uid);
      entry.gid = Number(result[0].gid);
      entry.cid = Number(result[0].cid);
      entry.cmdline = result[0].cmdline;
      entry.exit_code = result[0].exit_code;
      entry.probe_error = Number(result[0].probe_error);
      entry.syscall = result[0].syscall;
      entry.path = result[0].path;
      entry.cwd = result[0].cwd;
      entry.duration = Number(result[0].duration);
      entry.json_cmdline = result[0].json_cmdline;
      entry.ntime = result[0].ntime;
      entry.time = Number(result[0].time);
      entry.eid = Number(result[0].eid);
      entry.md5 = result[0].md5;
      entry.sha1 = result[0].sha1;
      entry.sha256 = result[0].sha256;
      entry.is_container_process = Number(result[0].is_container_process);
      entry.container_age = Number(result[0].container_age);
      entry.container_name = result[0].container_name;
      entry.uptime = Number(result[0].uptime);
      entry.root_pid = Number(result[0].pid);
      entry.event_type = 'Process Event';
      process_list.push(entry);
    }

    if (result.length == 0) {
      console.error('No process found with record ID bpf ', record_id);
    } else if (result.length != 1) {
      console.error('Multiple processes found with record ID ', record_id);
    } else {
      let loop: boolean = true;
      let ppid: number = Number(result[0].parent);
      let pid: number = Number(result[0].pid);

      let path: string;
      let cmdline: string;
      let host_identifier: string;
      let container_name: string;
      let container_id: string;
      let container_pid: number;

      path = result[0].path;
      cmdline = result[0].cmdline;
      host_identifier = result[0].host_identifier;

      let timestamp = Number(result[0].time);
      let uptime = Number(result[0].uptime);

      // Checking for container process
      let container_result = await connection.query(
         getDockerEvents,
        [timestamp - 3600, timestamp + 3600, ppid, host_identifier],
      );

      if (container_result.length != 0) {
        container_pid = Number(container_result[0].pid);
        container_name = container_result[0].container_name;
        container_id = container_result[0].container_id;

        if (pid == container_pid || ppid == container_pid) {
          let updateResult = await connection.query(
            `update bpf_process_events set is_container_process = 1 where id = $1`,
            [record_id],
          );
        }
      }
      /*
       * First step: we prefer entries from process_events.
       */
      while (loop) {
        result = await connection.query(
          getBpfProcessDataExecve,
          [ppid, host_identifier],
        );

        // console.log("Pid: ", result[0].pid,"Parent: ", result[0].parent );

        let entry: any = {};
        if (result.length == 0) {
          result = await connection.query(
            getBpfProcessDataNonExecve,
            [ppid, host_identifier],
          );
        }

        if (result.length != 0) {
          /*
           * The uptime value of uptime of system when process was created. This is not a monotonically increasing clock.
           * The timestamp value is from a monotonic clock, when process was created.
           *
           * if process and its apparent parent process are from same session (i.e. system did not reboot between two entries),
           * difference is timestamp should be same as difference in uptime.
           *
           */
          let new_timestamp = Number(result[0].time);
          let new_uptime = Number(result[0].uptime);

          let timestamp_delta = timestamp - new_timestamp;
          let uptime_delta = uptime - new_uptime;

          /*
           * timestamp and uptime are measured in sequence; and due to rounding, delta may not be exactly same.
           * A time gap of DELTA_LIMIT seconds is chosen here
           */

          if (Math.abs(timestamp_delta - uptime_delta) > DELTA_LIMIT) {
            // system rebooted somewhere between these two
            loop = false;
            // we need to discard the result, as it is from different session
            continue;
          } else {
            timestamp = new_timestamp;
            uptime = new_uptime;
          }

          path = result[0].path;
          cmdline = result[0].cmdline;
          let tmp_ppid = Number(result[0].parent);

          timestamp = Number(result[0].time);

          entry.pid = ppid;
          entry.ppid = tmp_ppid;

          entry.tid = Number(result[0].tid);
          entry.uid = Number(result[0].uid);
          entry.gid = Number(result[0].gid);
          entry.cid = Number(result[0].cid);
          entry.exit_code = result[0].exit_code;
          entry.probe_error = Number(result[0].probe_error);
          entry.syscall = result[0].syscall;
          entry.path = path;
          entry.cwd = result[0].cwd;
          entry.cmdline = cmdline;
          entry.duration = Number(result[0].duration);
          entry.json_cmdline = result[0].json_cmdline;
          entry.ntime = result[0].ntime;
          entry.time = timestamp;
          entry.eid = Number(result[0].eid);
          entry.md5 = result[0].md5;
          entry.sha1 = result[0].sha1;
          entry.sha256 = result[0].sha256;
          entry.is_container_process = Number(result[0].is_container_process);
          entry.container_age = Number(result[0].container_age);
          entry.container_name = result[0].container_name;
          entry.event_type = 'Process Event';

          ppid = tmp_ppid;

          process_list.push(entry);
        } else loop = false;
      }
      // console.log("ppid : ",ppid,"TimeStamp: ",timestamp,"Uptime: ",uptime,"Host_identifier: ",host_identifier)
      // console.log("Process Tree: ",process_list)
      /*
       * Since we did not find more entries in process_events, we check in snapshot_result table.
       *
       * We first pick the previous snapshot result, because if there is no process event, the process must be
       * alive before it can spawn any other process.
       *
       * If the previous entry does not exist, we check the immediate future entry with a hope that it will have
       * process entry (for example, event from processes started before osquery generated an event after osquery,
       * but before first snapshot result arrived.
       */
      // console.log("PPID: ",ppid);
      if(ppid !=undefined){
        const _tmp_process_list = await snapshot_process_tree(connection, ppid, timestamp, uptime, host_identifier);
        process_list.push(..._tmp_process_list);
      }
    }
    return process_list;
  } catch (error) {
    console.error(error);
    return [];
  }
};

const event_win_backtrack_process_tree = async (connection: any, record_id: number): Promise<any[]> => {
  try {
    const queryGetWinProcessDataFromId = `select * from win_process_events where id = $1`;
    const getWinProcessDataExecve = `select * from win_process_events where cmdline <> '' and process_guid = $1 and host_identifier = $2 order by time desc limit 1`;
    const getWinProcessDataNonExecve = `select * from win_process_events where cmdline = '' and process_guid = $1 and host_identifier = $2 order by time desc limit 1`
    const getDockerEvents = `select * from docker_events where status = 'start' and time between $1 and $2 and pid = $3 and host_identifier = $4`;
    /*
     * Since events can arrive out of order, and at some delay (in case osquery cannot reach fleet server for some reason, we cannot rely upon record ID in database.
     * If we want to get latest record of event, we should order by time column instead, which records timestamp at which the event occurred. Since it comes a
     * monotonic clock, it will never be out of order.
     */
    let process_list: any = [];
    let entry: any = {};
    let result: any;
    {
      result = await connection.query(queryGetWinProcessDataFromId, [record_id]);

      entry.process_guid = result[0].process_guid;
      entry.action = result[0].action;
      entry.eid = result[0].eid;
      entry.pid = Number(result[0].pid);
      entry.path = entry.path = result[0].path;
      entry.cmdline = entry.cmdline = result[0].cmdline;
      entry.ppid = Number(result[0].parent_pid);
      entry.parent_process_guid = result[0].parent_process_guid;
      entry.parent_path = result[0].parent_path;
      entry.owner_uid = result[0].owner_uid;
      entry.unixtime = result[0].unixtime;
      entry.time = Number(result[0].time);
      entry.utc_time = result[0].utc_time;
      entry.eventid = Number(result[0].eventid);
      entry.sha256 = result[0].sha256;
      entry.uptime = Number(result[0].uptime);
      entry.root_pid = Number(result[0].pid);
      entry.event_type = 'Process Event';
      process_list.push(entry);
    }

    if (result.length == 0) {
      console.error('No process found with record ID bpf ', record_id);
    } else if (result.length != 1) {
      console.error('Multiple processes found with record ID ', record_id);
    } else {
      let loop: boolean = true;
      let ppid: number = Number(result[0].parent_pid);
      let pid: number = Number(result[0].pid);

      let path: string;
      let cmdline: string;
      let host_identifier: string;
      let container_name: string;
      let container_id: string;
      let container_pid: number;
      let process_guid: string;
      let parent_process_guid: string;
      let tmp_process_guid: string;
      let tmp_parent_process_guid: string;

      path = result[0].path;
      cmdline = result[0].cmdline;
      host_identifier = result[0].host_identifier;
      process_guid = result[0].process_guid;
      parent_process_guid = result[0].parent_process_guid;

      let timestamp = Number(result[0].time);
      let uptime = Number(result[0].uptime);

      // Checking for container process
      let container_result = await connection.query(
        getDockerEvents,
        [timestamp - 3600, timestamp + 3600, ppid, host_identifier],
      );

      if (container_result.length != 0) {
        container_pid = Number(container_result[0].pid);
        container_name = container_result[0].container_name;
        container_id = container_result[0].container_id;

        if (pid == container_pid || ppid == container_pid) {
          let updateResult = await connection.query(
            `update bpf_process_events set is_container_process = 1 where id = $1`,
            [record_id],
          );
        }
      }
      /*
       * First step: we prefer entries from process_events.
       */

      while (loop) {
        /* TO FIX: It seems parent_process GUID is '' so the loop is not terminating?? */
        console.log('BACKTRACK : parent_process_guid :', parent_process_guid);
        result = await connection.query(
          getWinProcessDataExecve,
          [parent_process_guid, host_identifier],
        );

        let entry: any = {};
        if (result.length == 0) {
          result = await connection.query(
            getWinProcessDataNonExecve,
            [parent_process_guid, host_identifier],
          );
        }

        if (result.length != 0) {
          /*
           * The uptime value of uptime of system when process was created. This is not a monotonically increasing clock.
           * The timestamp value is from a monotonic clock, when process was created.
           *
           * if process and its apparent parent process are from same session (i.e. system did not reboot between two entries),
           * difference is timestamp should be same as difference in uptime.
           *
           */
          let new_timestamp = Number(result[0].time);
          let new_uptime = Number(result[0].uptime);

          let timestamp_delta = timestamp - new_timestamp;
          let uptime_delta = uptime - new_uptime;

          /*
           * timestamp and uptime are measured in sequence; and due to rounding, delta may not be exactly same.
           * A time gap of DELTA_LIMIT seconds is chosen here
           */

          // console.log("time: ",timestamp, "New timestamp",new_timestamp, "uptime: ",uptime,"New uptime:",new_uptime,"time Diff: ",timestamp_delta,"Uptime delta: ",uptime_delta);

          if (Math.abs(timestamp_delta - uptime_delta) > DELTA_LIMIT) {
            // system rebooted somewhere between these two
            loop = false;
            // we need to discard the result, as it is from different session
            continue;
          } else {
            timestamp = new_timestamp;
            uptime = new_uptime;
          }

          path = result[0].path;
          cmdline = result[0].cmdline;
          let tmp_ppid = Number(result[0].parent_pid);
          tmp_process_guid = result[0].process_guid;
          tmp_parent_process_guid = result[0].parent_process_guid;
          timestamp = Number(result[0].time);

          entry.pid = ppid;
          entry.ppid = tmp_ppid;
          entry.process_guid = parent_process_guid;
          entry.parent_process_guid = tmp_parent_process_guid || '';

          entry.action = result[0].action;
          entry.eid = result[0].eid;
          entry.process_guid = result[0].process_guid;
          // entry.parent_process_guid = result[0].parent_process_guid as string;
          // entry.parent_path  = result[0].parent_path as string;
          entry.owner_uid = result[0].owner_uid;
          entry.time = Number(result[0].time);
          entry.unixtime = Number(result[0].unixtime);
          entry.utc_time = result[0].utc_time;
          entry.uptime = Number(result[0].uptime);
          entry.path = path;
          entry.eventid = Number(result[0].eventid);
          entry.command_line = cmdline;
          entry.sha256 = result[0].sha256;
          entry.root_pid = Number(result[0].pid);
          entry.event_type = 'Process Event';
          ppid = tmp_ppid;
          parent_process_guid = tmp_parent_process_guid;
          process_list.push(entry);
        } else loop = false;
      }

      /*
       * Since we did not find more entries in process_events, we check in snapshot_result table.
       *
       * We first pick the previous snapshot result, because if there is no process event, the process must be
       * alive before it can spawn any other process.
       *
       * If the previous entry does not exist, we check the immediate future entry with a hope that it will have
       * process entry (for example, event from processes started before osquery generated an event after osquery,
       * but before first snapshot result arrived.
       */

      if(ppid!==undefined){
        const _tmp_process_list = await win_snapshot_process_tree(connection, ppid, timestamp, uptime, host_identifier);
        process_list.push(..._tmp_process_list);
      }
    }
    return process_list;
  } catch (error) {
    console.error(error);
    return [];
  }
};

const get_bpf_file_tree = async (connection: any, record_id: number): Promise<any[]> => {
  try {
    const getFileDataFromIdQuery = `select * from bpf_file_events where id = $1`;
    const getProcessData = `select * from bpf_process_events where pid = $1 and host_identifier = $2 and time <= $3 and path = $4 order by id desc limit 1`;

    let process_list: any = [];
    let result: any;
    console.log("\nBPF File Tree in making...");
    {
      let entry_file: any = {};
      result = await connection.query(getFileDataFromIdQuery, [record_id]);

      entry_file.pid = Number(result[0].pid);
      entry_file.path = result[0].path;
      entry_file.host_identifier = result[0].host_identifier;
      entry_file.hostname = result[0].hostname;
      // entry_file.parent = result[0].parent as number;
      entry_file.ppid = Number(result[0].parent);
      entry_file.uid = Number(result[0].uid);
      entry_file.gid = Number(result[0].gid);
      entry_file.cid = Number(result[0].cid);
      entry_file.exit_code = result[0].exit_code;
      entry_file.syscall = result[0].syscall;
      entry_file.file_path = result[0].file_path;
      entry_file.flags = result[0].flags;
      entry_file.duration = Number(result[0].duration);
      entry_file.root_pid = Number(result[0].pid);
      entry_file.timestamp = Number(result[0].time);
      entry_file.uptime = Number(result[0].uptime);
      entry_file.event_type = 'File Event';
      process_list.push(entry_file);
    }

    if (result.length == 0) {
      console.error('No process found with record ID ', record_id);
    } else if (result.length > 1) {
      console.error('Multiple processes found with record ID ', record_id);
    } else {
      let pid = Number(result[0].pid);
      let path: string;
      let host_identifier: string;

      path = result[0].path;
      host_identifier = result[0].host_identifier;

      let timestamp = Number(result[0].time);
      let uptime = Number(result[0].uptime);

      result = await connection.query(getProcessData, [pid, host_identifier, timestamp, path]);

      if (result.length != 0) {
        let process_record_id = Number(result[0].id);
        const _tmp_process_list = await event_bpf_backtrack_process_tree(connection, process_record_id);
        process_list.push(..._tmp_process_list);
      } else {
        /*
        * It seems that socket event is coming from a process that was started *before* osquery was started.
        * We have to pick data from snapshot.
        *
        * Since we have PID, instead of PPID, we have to remove first entry in process list.
        */
        const _tmp_process_list = await snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);
        process_list.push(..._tmp_process_list);
      }
    }
    return process_list;
  } catch (error) {
    console.error(error);
    return [];
  }
};
const get_bpf_socket_tree = async (connection: any, record_id: number): Promise<any[]> => {
  try {
    const getSocketDataFromIdQuery = `select * from bpf_socket_events where id = $1`;
    const getProcessData = `select * from bpf_process_events where pid = $1 and host_identifier = $2 and time <= $3 and path = $4 order by id desc limit 1`;

    let process_list: any = [];
    let result: any;

    {
      let entry: any = {};
      result = await connection.query(getSocketDataFromIdQuery, [record_id]);

      entry.pid = Number(result[0].pid);

      entry.path = result[0].path;
      entry.host_identifier = result[0].host_identifier;
      entry.hostname = result[0].hostname;
      // entry.parent = result[0].parent as number;
      entry.ppid = Number(result[0].parent);
      entry.uid = Number(result[0].uid);
      entry.gid = Number(result[0].gid);
      entry.cid = Number(result[0].cid);
      entry.exit_code = result[0].exit_code;
      entry.syscall = result[0].syscall;
      entry.protocol = Number(result[0].protocol);
      entry.local_address = result[0].local_address;
      entry.remote_address = result[0].remote_address;
      entry.local_port = Number(result[0].local_port);
      entry.remote_port = Number(result[0].remote_port);
      entry.duration = Number(result[0].duration);
      entry.event_type = 'Socket Event';
      entry.timestamp = Number(result[0].time);
      entry.uptime = Number(result[0].uptime);
      entry.root_pid = Number(result[0].pid);
      process_list.push(entry);
    }

    if (result.length == 0) {
      console.error('No process found with record ID ', record_id);
    } else if (result.length > 1) {
      console.error('Multiple processes found with record ID ', record_id);
    } else {
      let pid = Number(result[0].pid);

      let path: string;
      let host_identifier: string;

      path = result[0].path;
      host_identifier = result[0].host_identifier;

      let timestamp = Number(result[0].time);
      let uptime = Number(result[0].uptime);

      result = await connection.query(getProcessData, [pid, host_identifier, timestamp, path]);

      if (result.length != 0) {
        let process_record_id = Number(result[0].id);
        const _tmp_process_list = await event_bpf_backtrack_process_tree(connection, process_record_id);
        process_list.push(..._tmp_process_list);
      } else {
        const _tmp_process_list = await snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);
        process_list.push(..._tmp_process_list);
      }
    }
    return process_list;
  } catch (error) {
    console.error(error);
    return [];
  }
};

const get_win_file_tree = async (connection: any, record_id: number): Promise<any[]> => {
  try {
    const getFileDataFromIdQuery = `select * from win_file_events where id = $1`;
    const getProcessData = `select * from win_process_events where process_guid = $1 and host_identifier = $2 and time <= $3 and path = $4 order by id desc limit 1`;

    let process_list: any = [];
    let result: any;

    {
      let entry_file: any = {};
      result = await connection.query(getFileDataFromIdQuery, [record_id]);

      entry_file.pid = Number(result[0].pid);
      entry_file.host_identifier = result[0].host_identifier;
      entry_file.hostname = result[0].hostname;
      entry_file.parent = Number(result[0].parent_pid);
      entry_file.action = result[0].action;
      entry_file.eid = result[0].eid;
      entry_file.target_path = result[0].target_path;
      entry_file.md5 = result[0].md5;
      entry_file.sha256 = result[0].sha256;
      entry_file.hashed = Number(result[0].hashed);
      entry_file.uid = result[0].uid;
      entry_file.utc_time = result[0].utc_time;
      entry_file.unixtime = Number(result[0].unixtime);
      entry_file.timestamp = Number(result[0].time);
      entry_file.uptime = Number(result[0].uptime);
      entry_file.process_guid = result[0].process_guid;
      entry_file.process_name = result[0].process_name;
      entry_file.pe_file = result[0].pe_file;
      entry_file.eventid = Number(result[0].eventid);
      entry_file.root_pid = Number(result[0].pid);
      entry_file.event_type = 'File Event';

      process_list.push(entry_file);
    }

    if (result.length == 0) {
      console.error('No process found with record ID ', record_id);
    } else if (result.length > 1) {
      console.error('Multiple processes found with record ID ', record_id);
    } else {
      let pid = Number(result[0].pid);
      let path: string;
      let host_identifier: string;
      let process_guid: string;

      path = result[0].path;
      host_identifier = result[0].host_identifier;
      process_guid = result[0].process_guid;

      let timestamp = Number(result[0].time);
      let uptime = Number(result[0].uptime);

      result = await connection.query(getProcessData, [process_guid, host_identifier, timestamp, path]);

      if (result.length != 0) {
        let process_record_id = Number(result[0].id);
        const _tmp_process_list = await event_win_backtrack_process_tree(connection, process_record_id);
        process_list.push(..._tmp_process_list);
      } else {
        /*
         * It seems that file event is coming from a process that was started *before* osquery was started.
         * We have to pick data from snapshot.
         *
         * Since we have PID, instead of PPID, we have to remove first entry in process list.
         */
        const _tmp_process_list = await win_snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);
        process_list.push(..._tmp_process_list);
      }
    }
    return process_list;
  } catch (error) {
    console.error(error);
    return [];
  }
};

const get_win_socket_tree = async (connection: any, record_id: number): Promise<any[]> => {
  try {
    const getSocketDataFromIdQuery = `select * from win_socket_events where id = $1`;
    const getProcessData = `select * from win_process_events where process_guid = $1 and host_identifier = $2 and time <= $3 and path = $4 order by id desc limit 1`;

    let process_list: any = [];
    let result: any;

    {
      let entry_soc: any = {};
      result = await connection.query(getSocketDataFromIdQuery, [record_id]);

      entry_soc.pid = Number(result[0].pid);
      entry_soc.host_identifier = result[0].host_identifier;
      entry_soc.hostname = result[0].hostname;
      entry_soc.parent = Number(result[0].parent_pid);
      entry_soc.action = result[0].action;
      entry_soc.eid = result[0].eid;
      entry_soc.timestamp = Number(result[0].time);
      entry_soc.utc_time = result[0].utc_time;
      entry_soc.process_guid = result[0].process_guid;
      entry_soc.process_name = result[0].process_name;
      entry_soc.family = result[0].family;
      entry_soc.protocol = Number(result[0].protocol);
      entry_soc.local_address = result[0].local_address;
      entry_soc.remote_address = result[0].remote_address;
      entry_soc.local_port = Number(result[0].local_port);
      entry_soc.remote_port = Number(result[0].remote_port);
      entry_soc.eventid = Number(result[0].eventid);
      entry_soc.unixtime = Number(result[0].unixtime);
      entry_soc.uptime = Number(result[0].uptime);
      entry_soc.event_type = 'Socket Event';
      entry_soc.root_pid = Number(result[0].pid);
      process_list.push(entry_soc);

      process_list.push(entry_soc);
    }

    if (result.length == 0) {
      console.error('No process found with record ID ', record_id);
    } else if (result.length > 1) {
      console.error('Multiple processes found with record ID ', record_id);
    } else {
      let pid = Number(result[0].pid);
      let path: string;
      let host_identifier: string;
      let process_guid: string;

      path = result[0].path;
      host_identifier = result[0].host_identifier;
      process_guid = result[0].process_guid;

      let timestamp = Number(result[0].time);
      let uptime = Number(result[0].uptime);

      result = await connection.query(getProcessData, [process_guid, host_identifier, timestamp, path]);

      if (result.length != 0) {
        let process_record_id = Number(result[0].id);
        const _tmp_process_list = await event_win_backtrack_process_tree(connection, process_record_id);
        process_list.push(..._tmp_process_list);
      } else {
        /*
         * It seems that socket event is coming from a process that was started *before* osquery was started.
         * We have to pick data from snapshot.
         *
         * Since we have PID, instead of PPID, we have to remove first entry in process list.
         */
        const _tmp_process_list = await win_snapshot_process_tree(connection, pid, timestamp, uptime, host_identifier);
        process_list.push(..._tmp_process_list);
      }
    }
    return process_list;
  } catch (error) {
    console.error(error);
    return [];
  }
};

export const ept = async (req: Request, res: Response) => {
  const parameters = req.body;

  console.log('parameters', parameters);

  let event_id: number;
  let pid: number;
  let record_id: number;
  let remarks: string = '';
  let os_name: string = '';
  let process_guid: string = '';
  let parent_process_guid: string = '';
  // string ss

  let body: string;
  let host_identifier: string;
  let alert_type: string;
  let soc_event: boolean;
  let file_event: boolean;
  let status: number;
  let _tmp_process_list: any[];

  // JSON
  let forward_tree: any = {};
  let process_tree: any = {};

  if (parameters.hasOwnProperty('os_name')) {
    os_name = parameters['os_name'];
  }
  if (parameters.hasOwnProperty('host_identifier')) {
    host_identifier = parameters['host_identifier'];
  }
  if (parameters.hasOwnProperty('id')) {
    record_id = parameters['id'];
  }
  if (parameters.hasOwnProperty('alert_type')) {
    alert_type = parameters['alert_type'];
  }

  try {
    const entityManager = dbSourceConnection()[1];
    // let event_id = await entityManager.query(`select event_id from alerts where id = $1`, [record_id]);
    event_id = record_id;

    soc_event = parameters['socket_event'];
    file_event = parameters['file_event'];

    /* Differentiate as per OS */

    if (os_name == 'Linux') {
      let process_list: any[] = [];
      console.log("\nMaking Backward Process Tree..\n");

      switch(alert_type){
        case 'File Alert':
          _tmp_process_list = await get_bpf_file_tree(entityManager, record_id);
          process_list.push(..._tmp_process_list);
          break;

        case 'Socket Alert':
          _tmp_process_list = await get_bpf_socket_tree(entityManager, record_id);
          process_list.push(..._tmp_process_list);
          break;

        case 'Process Alert':
          _tmp_process_list = await event_bpf_backtrack_process_tree(entityManager, event_id);
          process_list.push(..._tmp_process_list);
          break;

        default:
          console.error("No Alert Type detected");
      }

      console.log('BPF Process Alert Process Tree: ', process_list.length);

      console.log("\nMaking Forward Process Tree..\n");
      // MAKE FORWARD PROCESS TREE
      {
        let result: any;
        let entry: any = {};
        let entry_soc: any = {};
        let entry_file: any = {};
        entry_soc.event_type = 'Socket Event';
        entry_file.event_type = 'File Event';
        entry.event_type = 'Process Event';

        const getBpfFilePid = `SELECT pid from bpf_file_events where id = $1`;
        const getBpfSocketPid = `SELECT pid from bpf_socket_events where id = $1`;
        const getBpfProcessPid = `SELECT pid from bpf_process_events where id = $1`;

        switch(alert_type){
          case 'File Alert':
            result = await entityManager.query(getBpfFilePid, [record_id]);
            pid = Number(result[0].pid);
            break;

          case 'Socket Alert':
            result = await entityManager.query(getBpfSocketPid, [record_id]);
            pid = Number(result[0].pid);
            break;

          case 'Process Alert':
            result = await entityManager.query(getBpfProcessPid, [record_id]);
            pid = Number(result[0].pid);
            break;

          default:
            console.error("No Alert Type detected");
        }

        const queryProcessEventId =
          'SELECT id FROM bpf_process_events WHERE pid = $1 AND host_identifier = $2 ORDER BY id ASC LIMIT 1';
        result = await entityManager.query(queryProcessEventId, [pid, host_identifier]);

        let parent_queue: number[] = [];
        if (result.length != 0) {
          result = await entityManager.query(`select * from bpf_process_events where id = $1`, [record_id]);
        }
        if (result.length == 0) {
          console.error('No process found with record ID bpf ', record_id);
        } else if (result.length != 1) {
          console.error('Multiple processes found with record ID ', record_id);
        } else {
          // We have only one process entry now
          let ppid: number = Number(result[0].parent);
          let pid: number = Number(result[0].pid);

          let path: string;
          let cmdline: string;
          let host_identifier: string;

          path = result[0].path;
          cmdline = result[0].cmdline;
          host_identifier = result[0].host_identifier;

          let timestamp: number = Number(result[0].time);
          let uptime: number = Number(result[0].uptime);

          const current_time = Math.floor(Date.now() / 1000);
          const s_current_time = current_time.toString();

          parent_queue.push(pid);
           /*
            * Since events can arrive out of order, and at some delay (in case osquery cannot reach fleet server for some reason, we cannot rely upon record ID in database.
            * If we want to get latest record of event, we should order by time column instead, which records timestamp at which the event occurred. Since it comes a
            * monotonic clock, it will never be out of order.
            */
          const getBpfProcessDataExecve = `select * from bpf_process_events where cmdline <> '' and parent = $1 and host_identifier = $2 and unixtime between $3 and $4 order by time asc limit 10`;
          const getBpfProcessDataNonExecve = `select * from bpf_process_events where cmdline = '' and parent = $1 and host_identifier = $2 and unixtime between $3 and $4 order by time asc limit 10`;
          
          let forward_list :any = []
          while (parent_queue.length != 0) {
            result = await entityManager.query(getBpfProcessDataExecve,
              [parent_queue[0], host_identifier, timestamp, s_current_time],
            );

            if (result.length == 0) {
              result = await entityManager.query(
                getBpfProcessDataNonExecve,
                [parent_queue[0], host_identifier, timestamp, s_current_time],
              );
            }
            if (result.length != 0) {
              /*
               * The uptime value of uptime of system when process was created. This is not a monotonically increasing clock.
               * The timestamp value is from a monotonic clock, when process was created.
               *
               * if process and its apparent parent process are from same session (i.e. system did not reboot between two entries),
               * difference is timestamp should be same as difference in uptime.
               *
               */
              let child_pid: number;
              for (let i = 0; i < result.length; i++) {
                let new_timestamp: number = Number(result[0].time);
                let new_uptime: number = Number(result[0].uptime);
                let timestamp_delta: number = timestamp - new_timestamp;
                let uptime_delta: number = uptime - new_uptime;

                /*
                 * timestamp and uptime are measured in sequence; and due to rounding, delta may not be exactly same.
                 * A time gap of DELTA_LIMIT seconds is chosen here
                 */

                // console.log("Entry: ",result[i].pid,"Diff: ",Math.abs(timestamp_delta - uptime_delta), "Result Length: ", result.length);
                if (Math.abs(timestamp_delta - uptime_delta) > DELTA_LIMIT) {
                  //  system rebooted somewhere between these two
                  // we need to discard the result, as it is from different session
                  continue;
                } else {
                  timestamp = new_timestamp;
                  uptime = new_uptime;
                }

                path = result[i].path;
                cmdline = result[i].cmdline;
                child_pid = Number(result[i].pid);
                timestamp = Number(result[i].time);
                parent_queue.push(Number(result[i].pid));
                // console.log("Queue: ", parent_queue);

                // entry.ppid = pid;

                entry.pid = child_pid;
                entry.tid = Number(result[i].tid);
                entry.uid = Number(result[i].uid);
                entry.gid = Number(result[i].gid);
                entry.cid = Number(result[i].cid);
                entry.cmdline = result[i].cmdline;
                entry.exit_code = result[i].exit_code;
                entry.probe_error = Number(result[i].probe_error);
                entry.syscall = result[i].syscall;
                entry.path = path;
                entry.cwd = result[i].cwd;
                entry.duration = Number(result[i].duration);
                entry.json_cmdline = result[i].json_cmdline;
                entry.ntime = result[i].ntime;
                entry.time = timestamp;
                entry.eid = Number(result[i].eid);
                entry.md5 = result[i].md5;
                entry.sha1 = result[i].sha1;
                entry.sha256 = result[i].sha256;
                entry.is_container_process = Number(result[i].is_container_process);
                entry.container_age = Number(result[i].container_age);
                entry.container_name = result[i].container_name;
                entry.uptime = Number(result[i].uptime);
                entry.ppid = parent_queue[0];
                entry.root_pid = Number(result[i].pid);
                entry.event_type = 'Process Event';
                entry.parent = parent_queue[0];
                forward_list.push(entry);
                entry = {};
              }

              if (soc_event) {
                let result_socket = await entityManager.query(
                  `select * from bpf_socket_events where pid = $1 and host_identifier = $2 order by id asc limit 1`,
                  [child_pid, host_identifier],
                );

                if (result_socket.length != 0) {
                  entry_soc.pid = Number(result_socket[0].pid);
                  entry_soc.path = result_socket[0].path;
                  entry_soc.host_identifier = result_socket[0].host_identifier;
                  entry_soc.hostname = result_socket[0].hostname;
                  entry_soc.ppid = Number(result_socket[0].parent);
                  entry_soc.uid = Number(result_socket[0].uid);
                  entry_soc.gid = Number(result_socket[0].gid);
                  entry_soc.cid = Number(result_socket[0].cid);
                  entry_soc.exit_code = result_socket[0].exit_code;
                  entry_soc.syscall = result_socket[0].syscall;
                  entry_soc.protocol = Number(result_socket[0].protocol);
                  entry_soc.local_address = result_socket[0].local_address;
                  entry_soc.remote_address = result_socket[0].remote_address;
                  entry_soc.local_port = Number(result_socket[0].local_port);
                  entry_soc.remote_port = Number(result_socket[0].remote_port);
                  entry_soc.duration = Number(result_socket[0].duration);
                  entry_soc.event_type = 'Socket Event';
                  entry_soc.timestamp = Number(result_socket[0].time);
                  entry_soc.uptime = Number(result_socket[0].uptime);
                  forward_list.push(entry_soc);
                  entry_soc = {};
                }
              }

              if (file_event) {
                let result_file = await entityManager.query(
                  `select * from bpf_file_events where pid = $1 and host_identifier = $2 order by id asc limit 1`,
                  [child_pid, host_identifier],
                );

                if (result_file.length != 0) {
                  entry_file.pid = Number(result_file[0].pid);
                  entry_file.path = result_file[0].path;
                  entry_file.host_identifier = result_file[0].host_identifier;
                  entry_file.hostname = result_file[0].hostname;
                  entry_file.ppid = Number(result_file[0].parent);
                  entry_file.uid = Number(result_file[0].uid);
                  entry_file.gid = Number(result_file[0].gid);
                  entry_file.cid = Number(result_file[0].cid);
                  entry_file.exit_code = result_file[0].exit_code;
                  entry_file.syscall = result_file[0].syscall;
                  entry_file.file_path = result_file[0].file_path;
                  entry_file.flags = result_file[0].flags;
                  entry_file.duration = Number(result_file[0].duration);
                  entry_file.event_type = 'File Event';
                  entry_file.timestamp = Number(result_file[0].time);
                  entry_file.uptime = Number(result_file[0].uptime);
                  forward_list.push(entry_file);
                  entry_file = {};
                }
              }
              parent_queue.shift();
            }
           else {
              parent_queue.shift();
            }
          }

          forward_list = forward_list.reverse();
          const backward_list = process_list;
          process_list = forward_list;
          process_list.push(...backward_list);
        }


        let smallest_root_pid: number;
        if (process_list.length >= 1) {
          let min_root_pid: number = Number(process_list[0].pid);
          for (let i = 1; i < process_list.length; i++) {
            if (process_list[i].pid < min_root_pid) {
              min_root_pid = process_list[i].pid;
            }
          }

          // Store the smallest value in entry.pid
          smallest_root_pid = min_root_pid;
        } else {
          smallest_root_pid = 0;
          return res.status(404).json({ status: 'Fail', message: 'No Process Tree' });
        }

        const j: any = {};
        const graph: any = [];

        for (const entry of process_list) {
          const obj: any = {};
          obj['pid'] = entry.pid;
          obj['parent'] = entry.ppid;
          obj['tid'] = entry.tid;
          obj['uid'] = entry.uid;
          obj['gid'] = entry.gid;
          obj['cid'] = entry.cid;
          obj['exit_code'] = entry.exit_code;
          obj['probe_error'] = entry.probe_error;
          obj['syscall'] = entry.syscall;
          obj['path'] = entry.path;
          obj['cwd'] = entry.cwd;
          obj['cmdline'] = entry.cmdline;
          obj['duration'] = entry.duration;
          obj['json_cmdline'] = entry.json_cmdline;
          obj['ntime'] = entry.ntime;
          obj['time'] = entry.time;
          obj['eid'] = entry.eid;
          obj['md5'] = entry.md5;
          obj['sha1'] = entry.sha1;
          obj['sha256'] = entry.sha256;
          obj['is_container_process'] = entry.is_container_process;
          obj['container_age'] = entry.container_age;
          obj['container_name'] = entry.container_name;
          obj['fd'] = entry.fd;
          obj['family'] = entry.family;
          obj['protocol'] = entry.protocol;
          obj['local_address'] = entry.local_address;
          obj['remote_address'] = entry.remote_address;
          obj['local_port'] = entry.local_port;
          obj['remote_port'] = entry.remote_port;
          obj['uptime'] = entry.uptime;
          obj['file_path'] = entry.file_path;
          obj['flags'] = entry.flags;
          obj['event_type'] = entry.event_type;
          // obj["parent"] = entry.parent;
          obj['root_pid'] = smallest_root_pid;

          graph.push(obj);
        }

        j['root_pid'] = smallest_root_pid;
        j['graph'] = graph;

        process_tree = j;
      }

      console.log("\nTree Making done ... Now making it presentable for frontend\n");
      console.log("\nEvents Graph Tree Length: ", process_list.length);
      if (process_list.length >= 1) {
        const data = process_tree;
        const signal_table = JSON.parse(JSON.stringify(process_tree['graph']));
        const graph_f = makeTree(data['graph'], getParent(data['graph'], data['root_pid'], os_name), os_name );
        let finalResult = { "Graph" : graph_f, "signalTable" : signal_table}
        return res.status(200).json(finalResult);
      }
      return res.status(404).json({ status: 'Fail', message: 'No Forward Process Tree' });

    } else if (os_name == 'Windows') {

      let process_list: any[] = [];

      switch(alert_type){
        case 'File Alert':
          _tmp_process_list = await get_win_file_tree(entityManager, record_id);
          process_list.push(..._tmp_process_list);
          break;

        case 'Socket Alert':
          _tmp_process_list = await get_win_socket_tree(entityManager, record_id);
          process_list.push(..._tmp_process_list);
          break;

        case 'Process Alert':
          _tmp_process_list = await event_win_backtrack_process_tree(entityManager, event_id);
          process_list.push(..._tmp_process_list);
          break;

        default:
          console.error("No Alert Type detected");
      }
      console.log('Win Process Alert Process Tree: ', process_list.length);
      // console.log(process_list);
      console.log('\n...Making Event Forward Process Tree:');

      // MAKE FORWARD PROCESS TREE
      {
        let result: any;
        let entry: any = {};
        let entry_soc: any = {};
        let entry_file: any = {};
        entry_soc.event_type = 'Socket Event';
        entry_file.event_type = 'File Event';
        entry.event_type = 'Process Event';
        let smallest_root_pid: number;
        // let forward_list :any = [];

        const getWinFilePid = `SELECT process_guid from win_file_events where id = $1`;
        const getWinSocketPid = `SELECT process_guid from win_socket_events where id = $1`;
        const getWinProcessPid = `SELECT process_guid from win_process_events where id = $1`;

        switch(alert_type){
          case 'File Alert':
            result = await entityManager.query(getWinFilePid, [event_id]);
            process_guid = result[0].process_guid;
            break;

          case 'Socket Alert':
            result = await entityManager.query(getWinSocketPid, [event_id]);
            process_guid = result[0].process_guid;
            break;

          case 'Process Alert':
            result = await entityManager.query(getWinProcessPid, [event_id]);
            process_guid = result[0].process_guid;
            break;

          default:
            console.error("No Alert Type detected");
        }

        const queryProcessEventId =
          'SELECT id FROM win_process_events WHERE process_guid = $1 AND host_identifier = $2 ORDER BY id ASC LIMIT 1';
        result = await entityManager.query(queryProcessEventId, [process_guid, host_identifier]);

        let parent_queue: string[] = [];
        if (result.length != 0) {
          result = await entityManager.query(`select * from win_process_events where id = $1`, [record_id]);
        }
        if (result.length == 0) {
          console.error('No process found with record ID wpf ', event_id);
        } else if (result.length != 1) {
          console.error('Multiple processes found with record ID ', event_id);
        } else {
          // We have only one process entry now
          let ppid: number = Number(result[0].parent_pid);
          let pid: number = Number(result[0].pid);

          let path: string;
          let cmdline: string;
          let host_identifier: string;

          let parent_process_guid = result[0].parent_process_guid;
          let process_guid = result[0].process_guid;
          path = result[0].path;
          cmdline = result[0].cmdline;
          host_identifier = result[0].host_identifier;

          let timestamp: number = Number(result[0].time);
          let uptime: number = Number(result[0].uptime);

          const current_time = Math.floor(Date.now() / 1000);
          const s_current_time = current_time.toString();

          parent_queue.push(process_guid);

          const getWinProcessDataExecve = `select * from win_process_events where cmdline <> '' and parent_process_guid = $1 and host_identifier = $2 and unixtime between $3 and $4 order by time asc limit 10`;
          const getWinProcessDataNonExecve = `select * from win_process_events where cmdline = '' and parent_process_guid = $1 and host_identifier = $2 and unixtime between $3 and $4 order by time asc limit 10`;

          let forward_list: any = [];

          while (parent_queue.length != 0) {
            console.log('Parent_process_guid: ', parent_process_guid);
            result = await entityManager.query(
              getWinProcessDataExecve,
              [parent_queue[0], host_identifier, timestamp, s_current_time],
            );

            if (result.length == 0) {
              result = await entityManager.query(
                getWinProcessDataNonExecve,
                [parent_queue[0], host_identifier, timestamp, s_current_time],
              );
            }

            if (result.length != 0) {
              /*
               * The uptime value of uptime of system when process was created. This is not a monotonically increasing clock.
               * The timestamp value is from a monotonic clock, when process was created.
               *
               * if process and its apparent parent process are from same session (i.e. system did not reboot between two entries),
               * difference is timestamp should be same as difference in uptime.
               *
               */
              let child_process_guid: string;
              for (let i = 0; i < result.length; i++) {
                let new_timestamp: number = Number(result[0].time);
                let new_uptime: number = Number(result[0].uptime);
                let timestamp_delta: number = timestamp - new_timestamp;
                let uptime_delta: number = uptime - new_uptime;

                /*
                 * timestamp and uptime are measured in sequence; and due to rounding, delta may not be exactly same.
                 * A time gap of DELTA_LIMIT seconds is chosen here
                 */

                if (Math.abs(timestamp_delta - uptime_delta) > DELTA_LIMIT) {
                  //  system rebooted somewhere between these two

                  // we need to discard the result, as it is from different session
                  continue;
                } else {
                  timestamp = new_timestamp;
                  uptime = new_uptime;
                }

                path = result[i].path;
                cmdline = result[i].cmdline;
                child_process_guid = result[i].process_guid;
                parent_queue.push(result[i].process_guid);
                timestamp = Number(result[i].time);

                // entry.ppid = pid;

                entry.process_guid = child_process_guid;
                entry.action = result[i].action;
                entry.eid = result[i].eid;
                entry.pid = Number(result[i].pid);
                entry.cmdline = result[i].cmdline;
                entry.ppid = Number(result[i].ppid);
                entry.parent_path = result[i].parent_path;
                entry.owner_uid = result[i].owner_uid;
                entry.time = Number(result[i].time);
                entry.utc_time = result[i].utc_time;
                entry.eventid = Number(result[i].eventid);
                entry.sha256 = result[i].sha256;
                entry.uptime = Number(result[i].uptime);
                entry.event_type = 'Process Event';
                entry.root_pid = Number(result[i].pid);
                entry.parent_process_guid = parent_queue[0];
                forward_list.push(entry);
                entry = {};
              }

              if (soc_event) {
                let result_socket = await entityManager.query(
                  `select * from win_socket_events where process_guid = $1 and host_identifier = $2 order by id asc limit 1`,
                  [child_process_guid, host_identifier],
                );
                if (result_socket.length != 0) {
                  entry_soc.pid = Number(result_socket[0].pid);

                  entry_soc.event_type = result_socket[0].event_type;
                  entry_soc.host_identifier = result_socket[0].host_identifier;
                  entry_soc.hostname = result_socket[0].hostname;
                  entry_soc.parent = Number(result_socket[0].parent_pid);
                  entry_soc.action = Number(result_socket[0].action);
                  entry_soc.eid = result_socket[0].eid;
                  entry_soc.time = Number(result_socket[0].time);
                  entry_soc.utc_time = result_socket[0].utc_time;
                  entry_soc.process_guid = result_socket[0].process_guid;
                  entry_soc.process_name = result_socket[0].process_name;
                  entry_soc.family = Number(result_socket[0].family);
                  entry_soc.protocol = Number(result_socket[0].protocol);
                  entry_soc.local_address = result_socket[0].local_address;
                  entry_soc.remote_address = result_socket[0].remote_address;
                  entry_soc.local_port = Number(result_socket[0].local_port);
                  entry_soc.remote_port = Number(result_socket[0].remote_port);
                  entry_soc.eventid = Number(result_socket[0].eventid);
                  entry_soc.event_type = 'Socket Event';
                  entry_soc.unixtime = Number(result_socket[0].unixtime);
                  entry_soc.root_pid = Number(result[0].pid);
                  entry_soc.uptime = Number(result_socket[0].uptime);
                  forward_list.push(entry_soc);
                  entry_soc = {};
                }
              }

              if (file_event) {
                let result_file = await entityManager.query(
                  `select * from win_file_events where process_guid = $1 and host_identifier = $2 order by id asc limit 1`,
                  [child_process_guid, host_identifier],
                );

                if (result_file.length != 0) {
                  entry_file.pid = Number(result_file[0].pid);
                  entry_file.host_identifier = result_file[0].host_identifier;
                  entry_file.hostname = result_file[0].hostname;
                  entry_file.parent = Number(result_file[0].parent_pid);
                  entry_file.action = result_file[0].action;
                  entry_file.eid = result_file[0].eid;
                  entry_file.target_path = result_file[0].target_path;
                  entry_file.md5 = result_file[0].md5;
                  entry_file.sha256 = result_file[0].sha256;
                  entry_file.hashed = Number(result_file[0].hashed);
                  entry_file.uid = result_file[0].uid;
                  entry_file.utc_time = result_file[0].utc_time;
                  entry_file.unixtime = Number(result_file[0].unixtime);
                  entry_file.time = Number(result_file[0].time);
                  entry_file.uptime = Number(result_file[0].uptime);
                  entry_file.process_guid = result_file[0].process_guid;
                  entry_file.process_name = result_file[0].process_name;
                  entry_file.pe_file = result_file[0].pe_file;
                  entry_file.root_pid = Number(result[0].pid);
                  entry_file.eventid = Number(result_file[0].eventid);
                  entry_file.event_type = 'File Event';

                  forward_list.push(entry_file);
                  entry_file = {};
                }
              }
              parent_queue.shift();
            } else {
              parent_queue.shift();
            }
          }

          console.log("Length of process List: ", process_list.length);
          if (process_list.length >= 1) {
            let min_root_pid: number = Number(process_list[process_list.length-1].pid);
            smallest_root_pid = min_root_pid;
            console.log("Root Pid: ", smallest_root_pid);
          } else {
            smallest_root_pid = 0;
            return res.status(404).json({ status: 'Fail', message: 'No Win Process Tree' });
          }
  
          forward_list = forward_list.reverse();
          const backward_list = process_list;
          process_list = forward_list;
          process_list.push(...backward_list);
        }

        const j: any = {};
        const graph: any = [];

        for (const entry of process_list) {
          let obj = JSON.parse(JSON.stringify(entry));
          obj['cmdline'] = entry.command_line;
          obj['root_pid'] = smallest_root_pid;
          graph.push(obj);
        }

        j['root_pid'] = smallest_root_pid;
        j['graph'] = graph;

        process_tree = j;
      }

      if (process_list.length >= 1) {
        // const responseObj = {
        //   root_pid: process_tree.root_pid,
        //   graph: process_tree.graph,
        // };
        const data = process_tree;
        const signal_table = JSON.parse(JSON.stringify(process_tree['graph']));
        const graph_f = makeTree(data['graph'], getParent(data['graph'], data['root_pid'], os_name), os_name );
        let finalResult = { "Graph" : graph_f, "signalTable" : signal_table}
        return res.status(200).json(finalResult);
      }
      return res.status(404).json({ status: 'Fail', message: 'No Forward Process Tree' });
    } else {
      const error_json = {
        status: 'Fail',
        message: 'Missing required parameters',
      };

      res.status(400).json(error_json);
    }
  } catch (err) {
    console.error('Error:', err);
    return res.status(500).json({ status: 'Fail', message: err });
  }
  return res.status(500).json({ status: 'Fail', message: 'Internal Server Error' });
};
