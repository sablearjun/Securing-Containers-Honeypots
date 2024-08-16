import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';
// TO Fix
export const schedule_query_send = async (req: Request, res: Response) => {
    const bodyJSON = req.body;
    let query : string | undefined = '';
    let hosts : Array<String> | undefined = [];

    if (bodyJSON.hasOwnProperty('query')) {
      query = String(bodyJSON['query']);
    }
    if (bodyJSON.hasOwnProperty('hosts')) {
      hosts = String(bodyJSON['hosts'].trim()).split(',');
    }

    try {
      const entityManager = dbSourceConnection()[1];
      // console.log("Query: ",query);
      // console.log("Hosts: ",hosts);
      if (query=="" || hosts.length ==0 ) {
        res
          .status(400)
          .json({ message: "Query scheduling failed due to bad request" });
      }
      const hostQueryMap: Record<string, number> = {};

      await Promise.all(
        hosts.map(async (host: string) => {
          try {
            const queryText =
              "INSERT INTO query_queue (host_identifier, query, sent) VALUES ($1, $2, $3) RETURNING id";
            const values = [host, query, false];

            const result = await entityManager.query(queryText, values);

            if (result.length > 0) {
              result.forEach((row: any, index: number) => {
                hostQueryMap[`${host}:${index}`] = row.id;
              });
            }
            else {
              console.log("No rows");
            }
          } catch (error) {
            console.error("Error in inserting schedule query", error);
            res.status(500).json({ message: "Internal Server Error" });
          }
        })
      )
      // console.log("hostQueryMap: ",hostQueryMap);
      res.customSuccess(200, 'Query scheduled', hostQueryMap);
    } catch (error) {
      console.error("Error:", error);

      res
        .status(500)
        .json({ message: "Query scheduling failed due to database error" });
    }
  };