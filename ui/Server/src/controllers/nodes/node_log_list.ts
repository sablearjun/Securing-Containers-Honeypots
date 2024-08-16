import { dbSourceConnection } from "index";
import { Request, Response, NextFunction } from 'express';

export async function node_log_list(req: Request, res: Response) {
    console.log("node_log_list service function called");
  
    let log_list_json: any = {};
    let id: string;
    let log_type: string;
    let host_identifier: string;
    let str_time: string;
    let os_name: string;
    let start_time: number;
    let end_time: number;
    let time: number;
  
    try {
      const entityManager = dbSourceConnection()[1];
      const log_list = req.query;
      log_type = log_list.log_type as string;
      os_name = log_list.os_name as string;
      host_identifier = log_list.host_identifier as string;
      str_time = log_list.time as string;
      time = parseInt(str_time);
  
      start_time = time - 600;
      end_time = time + 600;
  
      // console.log("log type: ",log_type);
      // console.log("os: ",os_name);
      // console.log("host_identifier: ",host_identifier);
      // console.log("time: ",str_time);

      let query = '';
      let log_list_key = '';
  
      if (os_name === 'Linux') {
        switch (log_type) {
          case 'Process':
            query = 'SELECT * FROM bpf_process_events WHERE unixtime BETWEEN $1 AND $2 AND host_identifier = $3 ORDER BY id';
            log_list_key = 'log_list';
            break;
          case 'Socket':
            query = 'SELECT * FROM bpf_socket_events WHERE unixtime BETWEEN $1 AND $2 AND host_identifier = $3 ORDER BY id';
            log_list_key = 'log_list';
            break;
          case 'File':
            query = 'SELECT * FROM bpf_file_events WHERE unixtime BETWEEN $1 AND $2 AND host_identifier = $3 ORDER BY id';
            log_list_key = 'log_list';
            break;
        }
      } else if (os_name === 'Windows') {
        switch (log_type) {
          case 'Process':
            query = 'SELECT * FROM win_process_events WHERE unixtime BETWEEN $1 AND $2 AND host_identifier = $3 ORDER BY id';
            log_list_key = 'log_list';
            break;
          case 'Socket':
            query = 'SELECT * FROM win_socket_events WHERE unixtime BETWEEN $1 AND $2 AND host_identifier = $3 ORDER BY id';
            log_list_key = 'log_list';
            break;
          case 'File':
            query = 'SELECT * FROM win_file_events WHERE unixtime BETWEEN $1 AND $2 AND host_identifier = $3 ORDER BY id';
            log_list_key = 'log_list';
            break;
        }
      }
  
      // console.log("Query: ", query);
      if (query) {
        const result = await entityManager.query(query, [start_time, end_time, host_identifier]);
        for (const row of result) {
          if (!log_list_json[log_list_key]) {
            log_list_json[log_list_key] = [];
          }
          log_list_json[log_list_key].push({
            id: row.id,
            unixTime: row.unixTime,
            hostname: row.hostname,
            tid: row.tid,
            pid: row.pid,
            parent: row.parent,
            uid: row.uid,
            gid: row.gid,
            cid: row.cid,
            exit_code: row.exit_code,
            probe_error: row.probe_error,
            syscall: row.syscall,
            path: row.path,
            cwd: row.cwd,
            cmdline: row.cmdline,
            duration: row.duration,
            json_cmdline: row.json_cmdline,
            ntime: row.ntime,
            time: row.time,
            eid: row.eid,
            md5: row.md5,
            sha1: row.sha1,
            sha256: row.sha256,
            is_container_process: row.is_container_process,
            container_age: row.container_age,
            container_name: row.container_name,
            fd: row.fd,
            family: row.family,
            type: row.type,
            protocol: row.protocol,
            local_address: row.local_address,
            remote_address: row.remote_address,
            local_port: row.local_port,
            remote_port: row.remote_port,
            file_path: row.file_path,
            flags: row.flags,
            action: row.action,
            process_guid: row.process_guid,
            parent_pid: row.parent_pid,
            parent_process_guid: row.parent_process_guid,
            parent_path: row.parent_path,
            owner_uid: row.owner_uid,
            target_path: row.target_path,
            hashed: row.hashed,
            pe_file: row.pe_file,
            process_name: row.process_name,
            eventid: row.eventid,
            uptime: row.uptime,
          });
        }
      }
  
      if (!log_list_json.log_list) {
        res.status(200).json({
          status: 'Fail',
          message: 'No data available',
        });
      } else {
        res.status(200).json(log_list_json.log_list);
      }
    } catch (error) {
      console.error('Database error:', error);
      res.status(500).json({
        status: 'Fail',
        message: 'Query to get list of configurations failed due to a database error.',
      });
    }
  }