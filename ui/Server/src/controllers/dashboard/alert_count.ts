import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const alert_count = async (req: Request, res: Response, next: NextFunction) => {
    const entityManager = dbSourceConnection()[1];
    let host_identifier: string | undefined = "";
    const queryJSON = req.query;

    if (queryJSON.hasOwnProperty("host_identifier")) {
      host_identifier = String(queryJSON["host_identifier"]);
    }

    let eventsResult;

    try{
        if(host_identifier == ""){
            eventsResult = await entityManager.query("SELECT event_code, COUNT(*) as count from alerts group by event_code order by count desc limit 10;");

        }else{
            eventsResult = await entityManager.query(`SELECT event_code, COUNT(*) as count from alerts where host_identifier = $1 group by event_code order by count desc limit 10;`, [ host_identifier ]);
        }

        const result = eventsResult.reduce((it, event) => {
          const { event_code, count } = event;
          it[event_code] = parseInt(count);
          return it;
        }, {});
        if(Object.keys(result).length > 0)
          res.status(200).json(result);
        else{
          res.status(200).json({ status : "Fail", message : "No data available"})
        }
    } catch (err) {
    const customError = new CustomError(400, 'Raw', `Can't retrieve count of nodes.`, null, err);
    return next(customError);
  }
};
