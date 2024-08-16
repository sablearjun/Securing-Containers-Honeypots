import { dbSourceConnection } from 'index';
import { Request, Response, NextFunction } from 'express';

export async function nodes_delete(req: Request, res: Response) {
    console.log("delete service function called");
    let nodes_delete_json: any = {};
    let host_identifier: string;
  
    try {
      const entityManager = dbSourceConnection()[1];
      nodes_delete_json = req.body;
      host_identifier = nodes_delete_json.host_identifier;
  
      const result = await entityManager.query(
        'DELETE FROM nodes WHERE host_identifier = $1',
        [host_identifier]
      );
  
      const status = result.length;
  
      if (status > 0) {
        res.status(200).json({
          status: 'Success',
          message: 'Node Deleted.',
        });
      } else {
        res.status(200).json({
          status: 'Fail',
          message: 'Failed to delete node',
        });
      }
    } catch (error) {
      console.error('Database error:', error);
      res.status(500).json({
        status: 'Fail',
        message: 'Query to delete node failed due to a database error.',
      });
    }
  }