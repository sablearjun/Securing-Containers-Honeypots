import { dbSourceConnection } from 'index';
import { Request, Response, NextFunction } from 'express';


export async function nodes_reenroll(req: Request, res: Response) {
    console.log("reenroll service function called");
  
    let reenroll_allowed: boolean;
    let host_identifier: string;
  
    try {
      const entityManager = dbSourceConnection()[1];
      const nodes_reenroll_json = req.body;
      host_identifier = nodes_reenroll_json.host_identifier;
      reenroll_allowed = nodes_reenroll_json.reenroll_allowed;
  
      const result = await entityManager.query(
        'UPDATE nodes SET reenroll_allowed = $2 WHERE host_identifier = $1',
        [host_identifier, reenroll_allowed]
      );
  
      const status = result.length;

      if (status > 0) {
        res.status(200).json({
          status: 'Success',
          message: 'Node re-enroll allowed status changed.',
        });
      } else {
        res.status(400).json({
          status: 'Fail',
          message: 'Failed to change status.',
        });
      }
    } catch (error) {
      console.error('Database error:', error);
      res.status(500).json({
        status: 'Fail',
        message: 'Query to change the status of re-enroll node failed due to a database error.',
      });
    }
  }