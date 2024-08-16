
import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

export const event_query = async (req: Request, res: Response) => {
    const eventQueryJson = req.query;
    let event_id: any;
    let host_identifier: any;
    let query_list_json: any;
  
    if (eventQueryJson.hasOwnProperty('event_id')) {
      event_id = eventQueryJson['event_id']
    }
    if (eventQueryJson.hasOwnProperty('host_identifier')) {
      host_identifier = eventQueryJson['host_identifier']
    }
    console.log("Data: ",event_id, host_identifier);
  
    try {
      const entityManager = dbSourceConnection()[1];
  
      const result_ip = await entityManager.query(
        `select alerts.*, interface_addresses.address from alerts inner join interface_addresses on alerts.host_identifier = interface_addresses.host_identifier where alerts.id = $1 and alerts.host_identifier = $2 and interface_addresses.broadcast <> '' and interface_addresses.interface <> 'lo' and (interface_addresses.interface like '%wlx%' or interface_addresses.interface like 'en%' or interface_addresses.interface like 'eth%') order by interface_addresses.id desc limit 1;`,
        [event_id, host_identifier]
      );
  
      const result = await entityManager.query(
        `select alerts.*, nodes.os_name, nodes.os_platform, nodes.osquery_version from alerts inner join nodes on alerts.host_identifier = nodes.host_identifier where alerts.id = $1 and alerts.host_identifier = $2 order by alerts.id desc;`,
        [event_id, host_identifier]
      );

      // console.log("ri", result_ip);
      // console.log("r", result);
  
      const os_names: string[] = [];
      const os_platforms: string[] = [];
      const osquery_versions: string[] = [];
  
      for (const c of result) {
        const os_name = c['os_name'];
        const os_platform = c['os_platform'];
        const osquery_version = c['osquery_version'];

        os_names.push(os_name);
        os_platforms.push(os_platform);
        osquery_versions.push(osquery_version);
      }
  
      let address = '';
      let os_index = 0;
  
      for (const c of result_ip) {
        if (result.rows.length > 0) {
          address = result[0]['address'];
        }
  
        query_list_json = {
          id: c.id,
          host_identifier: c.host_identifier,
          hostname: c.hostname,
          unixtime: c.unixtime,
          event_code: c.event_code,
          metadata: c.metadata,
          is_alert: c.is_alert,
          is_open: c.is_open,
          context_data: c.context_data,
          severity: c.severity,
          address: address,
          os_name: os_names[os_index],
          os_platform: os_platforms[os_index],
          osquery_version: osquery_versions[os_index],
          technique: c.technique,
          technique_id: c.technique_id,
          pid: c.pid,
          link: c.link,
          action: c.action,
          event_id: c.event_id,
          alert_type: c.alert_type,
        };
        
  
        if (os_index < result_ip.length) {
          os_index = os_index + 1;
        }
      }
  
      if (result.length <= 0) {
        return res.status(200).json({
          status: 'Fail',
          message: 'Nodata available',
        });
      } else {
        return res.status(200).json(query_list_json);
      }
    } catch (error) {
      console.error('Error in retrieving event list information:', error);
      return res.status(500).json({ message: 'Internal Server Error' });
    }
  };