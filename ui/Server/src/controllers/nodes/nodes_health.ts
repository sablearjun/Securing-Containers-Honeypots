import { dbSourceConnection } from 'index';
import { Request, Response, NextFunction } from 'express';

export async function nodes_health(req: Request, res: Response) {
    console.log("health service function called");

    let node_health_json: any = {};
    let host_identifier: string;
    let time = 300; // default time interval in seconds
    const node_health: string[] = [];
  
    try {
      const entityManager = dbSourceConnection()[1];
      const node_request = req.query;
      host_identifier = String(node_request.host_identifier);
      time = node_request.time ? parseInt(node_request.time as string) : time;
  
      let start_time: number;
      const end_time = Math.floor(Date.now() / 1000); // current timestamp in seconds
      let temp_end_time: number;
      let count = 0;
  
      do {
        const temp_start_time = end_time - time;
        temp_end_time = temp_start_time + 20; // interval of 20 seconds
  
        if (host_identifier) {
          const result_cpu = await entityManager.query(
            'SELECT * FROM cpu_time WHERE host_identifier = $1 AND unixtime >= $2 AND unixtime <= $3',
            [host_identifier, temp_start_time, temp_end_time]
          );
  
          if (result_cpu.length === 0) {
            node_health[count] = '0.0';
          } else {
              const totalIdle = result_cpu.reduce((sum: any, row: { idle: any; }) => sum + row.idle, 0);
              const totalTime = result_cpu.reduce((sum: any, row: { users: any; nice: any; system: any; idle: any; }) => sum + row.users + row.nice + row.system + row.idle, 0);
              const average_CPU_load = ((1 - totalIdle / totalTime) * 100).toFixed(2);
              node_health[count] = average_CPU_load;
          }
          count++;
        }
  
        start_time = temp_end_time;
        time -= 20;
      } while (temp_end_time <= end_time);
  
      node_health_json.cpu_health = { average_CPU_load: node_health };
  
      if (!host_identifier) {
        const result_cpu = await entityManager.query(
          'SELECT severity, COUNT(severity) from alerts GROUP BY severity'
        );
        const severityData = result_cpu.map((row: any) => ({
          severity_count: row.count,
          severity_level: row.severity,
        }));
        node_health_json.severity = severityData;
      }
  
      res.status(200).json(node_health_json);
    } catch (error) {
      console.error('Database error:', error);
      res.status(500).json({
        status: 'Fail',
        message: 'Query to node health failed due to a database error.',
      });
    }
  }