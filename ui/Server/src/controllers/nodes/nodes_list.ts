import { Request, Response, NextFunction, response } from 'express';

import { dbSourceConnection } from 'index';

interface NodeInfo {
  id: number;
  host_identifier: string;
  os_arch: string;
  os_build: string;
  os_major: string;
  os_minor: string;
  os_name: string;
  os_platform: string;
  osquery_version: string;
  node_key: string;
  node_invalid: string;
  reenroll_allowed: string;
  hardware_vendor: string;
  hardware_model: string;
  hardware_version: string;
  hostname: string;
  enrolled_on: string;
  status: string;
  last_seen: string;
  hardware_cpu_logical_core: string;
  hardware_cpu_type: string;
  hardware_physical_memory: string;
}

async function generateNodesListQuery(
  hostname: string | null,
  start_time: number,
  end_time: number,
  is_active: string,
  row_to_skip: number,
  r2: number,
): Promise<{ countQuery: string; listQuery: string }> {
  let countQuery = 'SELECT COUNT(id) FROM nodes';
  let listQuery = 'SELECT * FROM nodes';

  const conditions: string[] = [];

  if (hostname !== undefined) {
    conditions.push(`(host_identifier LIKE '%${hostname}%' OR hostname LIKE '%${hostname}%')`);
  }

  if (start_time !== 0 && end_time !== 0) {
    conditions.push(`enrolled_on >= ${start_time} AND enrolled_on <= ${end_time}`);
  } else if (start_time !== 0) {
    conditions.push(`enrolled_on >= ${start_time}`);
  } else if (end_time !== 0) {
    conditions.push(`enrolled_on <= ${end_time}`);
  }

  if (conditions.length > 0) {
    countQuery += ` WHERE ${conditions.join(' AND ')}`;
    listQuery += ` WHERE ${conditions.join(' AND ')}`;
  }

  if(is_active==='true' || is_active==='false'){
    listQuery += ` ORDER BY id DESC`;
  }else{
    listQuery += ` ORDER BY id DESC OFFSET ${row_to_skip} ROWS FETCH NEXT ${r2} ROWS ONLY`;
  }
  return { countQuery, listQuery };
}

export async function nodes_list(req: Request, res: Response) {
  console.log('list service function called');
  let nodes_list_json: any;
  let host_identifier: string;
  let hostname: string;
  let is_active: string;
  let no_of_rows_per_page: string;
  let pageno: string;
  let str_start_time: string;
  let str_end_time: string;
  let start_time: number = 0;
  let end_time: number = 0;
  let row_to_skip: number,
    r1: number = 1,
    r2: number = 100;

  let response_json: { nodes: NodeInfo[] } = { nodes: [] };

  try {
    const entityManager = dbSourceConnection()[1];

    try {
      nodes_list_json = req.query;
      if (nodes_list_json.hasOwnProperty('hostname')) {
        hostname = nodes_list_json['hostname'];
      }

      if (nodes_list_json.hasOwnProperty('start_time')) {
        str_start_time = nodes_list_json['start_time'];
        start_time = parseInt(str_start_time);
      }

      if (nodes_list_json.hasOwnProperty('end_time')) {
        str_end_time = nodes_list_json['end_time'];
        end_time = parseInt(str_end_time);
      }

      if (nodes_list_json.hasOwnProperty('is_active')) {
        is_active = nodes_list_json['is_active'];
      }

      if (nodes_list_json.hasOwnProperty('pageno')) {
        pageno = nodes_list_json['pageno'];
        r1 = parseInt(pageno);
      }

      if (nodes_list_json.hasOwnProperty('rows')) {
        no_of_rows_per_page = nodes_list_json['rows'];
        r2 = parseInt(no_of_rows_per_page);
      }

      row_to_skip = (r1 - 1) * r2;
    } catch (error) {
      // Handle any exceptions that might occur during the processing
      console.error(error);
    }

    const nodeListQuery = await generateNodesListQuery(hostname, start_time, end_time, is_active, row_to_skip, r2);
    console.log(nodeListQuery.countQuery, nodeListQuery.listQuery);
    const result_count = await entityManager.query(nodeListQuery.countQuery);
    const total_records = parseInt(result_count[0].count, 10);
    const result = await entityManager.query(nodeListQuery.listQuery);

    if(is_active == "true") {
      const onlineNodes:any = [];
      for(const i of result) {
        const lastseen: string = i['last_seen'];
        const t: number = Math.floor(Date.now() / 1000);
        const current_time: number = parseInt(t.toString());
        const dif: number = current_time - parseInt(lastseen, 10);

        if(dif < 60) {
          const active_status = "Online";
          onlineNodes.push({
            id: i.id,
            host_identifier: i.host_identifier,
            os_arch: i.os_arch,
            os_build: i.os_build,
            os_major: i.os_major,
            os_minor: i.os_minor,
            os_name: i.os_name,
            os_platform: i.os_platform,
            osquery_version: i.osquery_version,
            node_key: i.node_key,
            node_invalid: i.node_invalid,
            reenroll_allowed: i.reenroll_allowed,
            hardware_vendor: i.hardware_vendor,
            hardware_model: i.hardware_model,
            hardware_version: i.hardware_version,
            hostname: i.hostname,
            enrolled_on: i.enrolled_on,
            status: active_status,
            last_seen: lastseen,
            hardware_cpu_logical_core: i.hardware_cpu_logical_core,
            hardware_cpu_type: i.hardware_cpu_type,
            hardware_physical_memory: i.hardware_physical_memory,
          });
        }
      }

      for(let i=row_to_skip; i<(row_to_skip+r2) && i<onlineNodes.length;i++){
        response_json.nodes.push(onlineNodes[i]);
      }

    } else if(is_active == "false") {
      const offlineNodes:any = [];
      for(const i of result) {
        const lastseen: string = i['last_seen'];
        const t: number = Math.floor(Date.now() / 1000);
        const current_time: number = parseInt(t.toString());
        const dif: number = current_time - parseInt(lastseen, 10);

        if(dif > 60) {
          const active_status = "Offline";
          offlineNodes.push({
            id: i.id,
            host_identifier: i.host_identifier,
            os_arch: i.os_arch,
            os_build: i.os_build,
            os_major: i.os_major,
            os_minor: i.os_minor,
            os_name: i.os_name,
            os_platform: i.os_platform,
            osquery_version: i.osquery_version,
            node_key: i.node_key,
            node_invalid: i.node_invalid,
            reenroll_allowed: i.reenroll_allowed,
            hardware_vendor: i.hardware_vendor,
            hardware_model: i.hardware_model,
            hardware_version: i.hardware_version,
            hostname: i.hostname,
            enrolled_on: i.enrolled_on,
            status: active_status,
            last_seen: lastseen,
            hardware_cpu_logical_core: i.hardware_cpu_logical_core,
            hardware_cpu_type: i.hardware_cpu_type,
            hardware_physical_memory: i.hardware_physical_memory,
          });
        }
      }

      for(let i=row_to_skip; i<(row_to_skip+r2) && i<offlineNodes.length;i++){
        response_json.nodes.push(offlineNodes[i]);
      }

    } else {
      for(const i of result) {
        const lastseen: string = i['last_seen'];
        const t: number = Math.floor(Date.now() / 1000);
        const current_time: number = parseInt(t.toString());
        const dif: number = current_time - parseInt(lastseen, 10);
        let active_status: string;
        if (dif < 60)
        {
            active_status = "Online";
        }
        else
        {
            active_status = "Offline";
        }
        response_json.nodes.push({
          id: i.id,
          host_identifier: i.host_identifier,
          os_arch: i.os_arch,
          os_build: i.os_build,
          os_major: i.os_major,
          os_minor: i.os_minor,
          os_name: i.os_name,
          os_platform: i.os_platform,
          osquery_version: i.osquery_version,
          node_key: i.node_key,
          node_invalid: i.node_invalid,
          reenroll_allowed: i.reenroll_allowed,
          hardware_vendor: i.hardware_vendor,
          hardware_model: i.hardware_model,
          hardware_version: i.hardware_version,
          hostname: i.hostname,
          enrolled_on: i.enrolled_on,
          status: active_status,
          last_seen: lastseen,
          hardware_cpu_logical_core: i.hardware_cpu_logical_core,
          hardware_cpu_type: i.hardware_cpu_type,
          hardware_physical_memory: i.hardware_physical_memory,
        });
      }
    }
    if (!response_json.hasOwnProperty('nodes') || response_json['nodes'].length === 0) {
      return res.status(200).json({
        status: 'Fail',
        nodes: [],
        message: 'Nodata available',
      });
    } else {
        return res.status(200).json(response_json);
    }
  } catch (error) {
    console.error('Error in retrieving event list information:', error);
    return res.status(500).json({ message: 'Internal Server Error' });
  }
}
