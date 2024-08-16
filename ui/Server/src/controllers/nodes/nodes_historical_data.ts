import { dbSourceConnection } from 'index';
import { Request, Response, NextFunction } from 'express';

export async function nodes_historical_data(req: Request, res: Response) {
    console.log("historical_data service function called");
  
    // let query;
    // let host_identifier: string;
    const parents_list: any[] = [];
  
    try {
      const entityManager = dbSourceConnection()[1];
      let { query } = req.body;
      
      if(!query)
      {
        return res.status(400).json({status:"Fail", message:'Missing Body parameters.'}) 
      }
  
      if (query) {
        query = query.replace(/%20/g, ' ');
      }

      const historical_data_result = await entityManager.query(query);
  
      for (const row of historical_data_result) {
        const obj: any = {};
  
        for (const [colName, fieldValue] of Object.entries(row)) {
          if (fieldValue === null) {
            obj[colName] = 'NULL';
          } else {
            obj[colName] = fieldValue;
          }
        }
  
        parents_list.push(obj);
      }

      return res.status(200).json(parents_list);
    } catch (error) {
      console.error('Database error:', error);
      return res.status(500).json({
        status: 'Fail',
        message: 'Query to historical data failed due to a database error.',
      });
    }
  }