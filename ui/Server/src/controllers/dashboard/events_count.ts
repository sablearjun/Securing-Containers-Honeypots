import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const events_count = async (req: Request, res: Response, next: NextFunction) => {
  let host_identifier: string | undefined = "";
  const eventsRepository = dbSourceConnection()[1];
  const queryJSON = req.query;

  if (queryJSON.hasOwnProperty("host_identifier")) {
    host_identifier = String(queryJSON["host_identifier"]);
  }


  try {

    const eventsResult = await eventsRepository.query("select is_open, host_identifier from alerts");


    let openEvents: number = 0;
    let closedEvents: number = 0;
    let sameHostOpenEvents: number = 0;
    let sameHostClosedEvents: number = 0;

    for(let i = 0; i< eventsResult.length; i++){
        if(eventsResult[i].is_open == true){
            if(eventsResult[i].host_identifier == host_identifier){
                sameHostOpenEvents++;
            }
            else
                openEvents++;
        }else{
            if(eventsResult[i].host_identifier == host_identifier){
                sameHostClosedEvents++;
            }
            closedEvents++;
        }
    }

    const events = [
        {"count":host_identifier? sameHostOpenEvents: openEvents, "name": "Open"},
        {"count": host_identifier? sameHostClosedEvents: closedEvents, "name": "Close"},
    ]
    const total = host_identifier? sameHostOpenEvents + sameHostClosedEvents : openEvents + closedEvents;
    if(total>0){
      res.status(200).json(events);
    }else{
      res.status(200).json({ status : "Fail", message : "No data available"})
    }
  } catch (err) {
    const customError = new CustomError(400, 'Raw', `Can't retrieve count of nodes.`, null, err);
    return next(customError);
  }
};
