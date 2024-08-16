import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const schedule_query_status = async (req: Request, res: Response) => {
    const bodyJSON = req.body;
    let configIds: Array<String> | undefined = [];

    if(bodyJSON.hasOwnProperty("config_id_list")){
      configIds = String(bodyJSON['config_id_list']).split(',');
    }
    try {

      const entityManager = dbSourceConnection()[1]

      if(configIds.length ==0)
        res.status(400).json({ message: "Config_id_list is required. Bad Request"});

      const queryStatusMap: Record<string, { "sent" : string , "send_time": string}> = {};

      await Promise.all(
        configIds.map(async (id: string) => {
          try {
            const queryText =
              "SELECT sent, send_time FROM query_queue WHERE id = $1";
            const result = await entityManager.query(queryText, [id]);

            if (result.length > 0) {
              result.forEach((row: any) => {
                const isSent = row.sent;
                const sentTime = row.send_time || 0;
                queryStatusMap[id] = { "sent": isSent,"send_time": sentTime};
              });
            }
          } catch (error) {
            console.error("Error in retrieving query status", error);
            res.status(500).json({ message: "Internal Server Error" });
          }
        })
      );
      // console.log("\n\n Status: ", queryStatusMap);
      res.status(200).json(queryStatusMap);
    } catch (error) {
      console.error("Error in retrieving query status", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
};
