import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

export async function nodes_query(req: Request, res: Response) {
  let nodes_query_json: any = {};
  let host_identifier: string;
  let no_of_records = 3;
  let start_time = 0;
  let end_time = 0;
  let osName: string = 'linux';
  let ipQuery: string = `SELECT address FROM interface_addresses WHERE host_identifier = $1 AND broadcast <> '' AND interface <> 'lo' AND (interface LIKE '%wl%' OR interface LIKE 'en%' OR interface LIKE 'eth%') ORDER BY id DESC LIMIT 1`;

  let nodes_query_response_json: any = {
    no_of_records: 3,
    start_time: 0,
    end_time: 0,
    events: [], // Initialize the events array
    severity: [], // Initialize the severity array
    users: [], // Initialize the users array
  };

  try {
    nodes_query_json = req.query;
    host_identifier = nodes_query_json.host_identifier;
    no_of_records = Number(nodes_query_json.no_of_records) || no_of_records;
    start_time = Number(nodes_query_json.start_time) || start_time;
    end_time = Number(nodes_query_json.end_time) || end_time;

    const entityManager = dbSourceConnection()[1];

    let result: any;
     
    if (!host_identifier && start_time === 0 && end_time === 0) {
      result = await entityManager.query('SELECT * from alerts ORDER BY id DESC LIMIT $1', [no_of_records]);
    } else if (host_identifier && start_time === 0 && end_time === 0) {
      result = await entityManager.query('SELECT * from alerts WHERE host_identifier = $1 ORDER BY id DESC LIMIT $2', [
        host_identifier,
        no_of_records,
      ]);
    } else if (host_identifier && start_time !== 0 && end_time === 0) {
      result = await entityManager.query(
        'SELECT * from alerts WHERE host_identifier = $1 AND unixtime >= $2 ORDER BY id DESC LIMIT $3',
        [host_identifier, start_time, no_of_records],
      );
    } else if (host_identifier && start_time === 0 && end_time !== 0) {
      result = await entityManager.query(
        'SELECT * from alerts WHERE host_identifier = $1 AND unixtime <= $2 ORDER BY id DESC LIMIT $3',
        [host_identifier, end_time, no_of_records],
      );
    } else if (host_identifier && start_time !== 0 && end_time !== 0) {
      result = await entityManager.query(
        'SELECT * from alerts WHERE host_identifier = $1 AND unixtime >= $2 AND unixtime <= $3 ORDER BY id DESC LIMIT $4',
        [host_identifier, start_time, end_time, no_of_records],
      );
    } else {
      throw new Error('Invalid query parameters');
    }

    const node = await entityManager.query(`SELECT * from nodes where host_identifier = $1`, [host_identifier]);
    //  If no node found with host identifier return Bad Request 400 response
    if(node.length<=0 ){
      return res.status(400).json({ status: "Fail", message: "Bad Request"});
    }
    osName = node[0].os_platform;

    if (result.length > 0) {
      for (const i of result) {
        nodes_query_response_json.events.push({
          id: i.id,
          host_identifier: i.host_identifier,
          hostname: i.hostname,
          unixtime: i.unixtime,
          event_code: i.event_code,
          metadata: i.metadata,
          is_alert: i.is_alert,
          is_open: i.is_open,
          context_data: i.context_data,
          severity: i.severity,
          alert_type: i.alert_type,
        });
      }
    }

    let result_config: any;

    if (host_identifier) {
      const result_count = await entityManager.query(
        `SELECT severity, COUNT(severity)
          from alerts
            GROUP BY severity`,
      );

      // console.log("rc", result_count);

      if (result_count.length > 0) {
        for (const i of result_count) {
          const severity_level: string = i['severity'].toString();
          const severity_counts: string = i['count'].toString();

          nodes_query_response_json.severity.push({
            severity_count: severity_counts,
            severity_level: severity_level,
          });
        }
      }

      const result_logged_user = await entityManager.query(
        `SELECT DISTINCT logged_user
          FROM logged_in_users
          WHERE type = 'user' AND host_identifier = $1`,
        [host_identifier],
      );

      // console.log("rlu", result_logged_user);

      if (result_logged_user.length > 0) {
        for (const i of result_logged_user) {
          nodes_query_response_json.users.push({
            logged_user: i.logged_user,
          });
        }
      }

      result_config = await entityManager.query(
        `SELECT config_id
          FROM nodes
          WHERE host_identifier = $1`,
        [host_identifier],
      );

      // console.log("rcon", result_config);

      if (result_config.length > 0) {
        for (const i of result_config) {
          nodes_query_response_json.config = i.config_id;
        }
      }

      // HOST IP
      if (osName === 'windows') {
          // FLEET DM QUERY
        ipQuery = `SELECT ia.address, id.mac FROM interface_addresses ia JOIN interface_details id ON id.interface = ia.interface JOIN routes r ON r.interface = ia.address WHERE ia.host_identifier = $1 AND r.destination = '0.0.0.0' AND r.netmask = 0 AND r.type = 'remote' AND ( inet(ia.address) IS NOT NULL AND ( split_part(ia.address, '.', 1) = '10' OR (split_part(ia.address, '.', 1) = '172' AND (CAST(split_part(ia.address, '.', 2) AS INTEGER) & 240) = 16) OR (split_part(ia.address, '.', 1) = '192' AND split_part(ia.address, '.', 2) = '168') ) OR (inet(ia.address) IS NULL AND (lower(ia.address) ~ '^f[cd][0-9a-f][0-9a-f]:[0-9a-f:]+') IS NOT NULL) ) ORDER BY r.metric ASC, inet(ia.address) IS NOT NULL DESC LIMIT 1;`
      }

      let result_ip = await entityManager.query(
       ipQuery,[host_identifier],
      );
        console.log("T:", result_ip);
      if( result_ip.length ===0 && osName === 'windows'){

        // Custom query
        ipQuery = `SELECT i.interface, i.address, i.mask FROM interface_addresses i WHERE i.host_identifier = $1 AND (i.address LIKE '192.168%%' OR i.address LIKE '172.16%%' OR i.address LIKE '172.17%%' OR i.address LIKE '172.18%%' OR i.address LIKE '172.19%%' OR i.address LIKE '172.20%%' OR i.address LIKE '172.21%%' OR i.address LIKE '172.22%%' OR i.address LIKE '172.23%%' OR i.address LIKE '10.%%') AND i.address NOT LIKE '127.%%' AND i.address IS NOT NULL AND i.address NOT LIKE '%::1%' AND i.mask NOT LIKE 'ffff:ffff:ffff:ffff::' AND i.mask <> 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff' ORDER BY i.interface DESC, i.unixtime DESC LIMIT 1;`
        result_ip = await entityManager.query(
          ipQuery,[host_identifier],
         );
         console.log("This was called");
      }
      if (result_ip.length > 0) {
        for (const i of result_ip) {
          nodes_query_response_json.address = i.address;
          console.log(node);
        }
      }
    }

    // console.log(nodes_query_response_json);

    if (result_config.length == 0) {
      return res.status(200).json({
        status: 'Fail',
        message: 'No data available',
      });
    }

    return res.status(200).json(nodes_query_response_json);
  } catch (error) {
    console.error('Database error:', error);
    return res.status(500).json({
      status: 'Fail',
      message: 'Query to the particular node failed due to a database error.',
    });
  }
}
