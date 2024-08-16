import { dbSourceConnection } from 'index';
import { Request, Response, NextFunction } from 'express';

export async function nodes_memory_health(req: Request, res: Response) {
    console.log("memory_health service function called");
  
    let node_health_json: any = {};
    let host_identifier: string;
    let time = 300; // default time interval in seconds
  
    try {
      const entityManager = dbSourceConnection()[1];
      const node_request = req.query;
      host_identifier = String(node_request.host_identifier);
      time = node_request.time ? parseInt(node_request.time as string) : time;

      let start_time: number;
      const end_time = Math.floor(Date.now() / 1000); // current timestamp in seconds
      let temp_end_time: number;
      let count = 0;
      node_health_json["memory_health"] = [];

      do {
        const temp_start_time = end_time - time;
        temp_end_time = temp_start_time + 60; // interval of 60 seconds
        if (host_identifier) {
          // console.log(temp_start_time, temp_end_time);
          const result_memory = await entityManager.query(
            'SELECT * FROM memory_info WHERE host_identifier = $1 AND unixtime >= $2 AND unixtime <= $3 ORDER BY id DESC LIMIT 1',
            [host_identifier, temp_start_time, temp_end_time]
          );
          if (result_memory.length === 0) {
            node_health_json["memory_health"].push({
              memory_total: 0,
              memory_free: 0,
            });
          } else {
            const memoryInfo = result_memory[0];
            node_health_json["memory_health"].push({
              memory_total: memoryInfo.memory_total,
              memory_free: memoryInfo.memory_free,
            });
            count++;
          }
        }
  
        start_time = temp_end_time;
        time -= 20;
      } while (temp_end_time <= end_time);
  
      //  console.log(node_health_json);
      res.status(200).json(node_health_json);
    } catch (error) {
      console.error('Database error:', error);
      res.status(500).json({
        status: 'Fail',
        message: 'Query to node health failed due to a database error.',
      });
    }
  }