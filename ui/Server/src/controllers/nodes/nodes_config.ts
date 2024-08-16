import { dbSourceConnection } from 'index';
import { Request, Response, NextFunction } from 'express';

export async function nodes_config(req: Request, res: Response) {
    console.log("config service function called");
    let nodes_configid_json: any = {};
    let host_identifier: string;
    let  arr: string;
    let status: number;
  
    try {
      const entityManager = dbSourceConnection()[1];
      nodes_configid_json = req.body;
      host_identifier = nodes_configid_json.host_identifier;

      arr = nodes_configid_json.config_id?.join(',');
      // console.log("Array: ",arr);
      //console.log(host_identifier,arr);
      if(!host_identifier || !arr)
      {
        return res.status(400).json({status:"Fail", message:'Missing payload'}) 
      }

      const result = await entityManager.query(
        `UPDATE nodes SET config_id = $1 WHERE host_identifier = $2`,
        [arr, host_identifier]
      );

      status = result[1];

      if (status > 0) {
        return res.status(200).json({
          status: 'Success',
          message: 'Node Configured.',
        });
      } else {
        return res.status(400).json({
          status: 'Fail',
          message: 'Failed to configure node.',
        });
      }
    } catch (error) {
      console.error('Database error:', error);
      return res.status(500).json({
        status: 'Fail',
        message: 'Query to configure node failed due to a database error.',
      });
    }
  }