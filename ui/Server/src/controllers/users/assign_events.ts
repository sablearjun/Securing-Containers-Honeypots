import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

// Pending
export const assign_events = async (req: Request, res: Response, next: NextFunction) => {
    const body= req.body;
    let user_id : bigint ;
    let eventIds : bigint[];
    let userResult : any[];
    let eventIdsList : any[] = [];

    if(body.hasOwnProperty('user_id')){
      user_id = BigInt(String(body['user_id']));
    }

    if(body.hasOwnProperty('event_id')){ // ALERT ID
      eventIds = body['event_id'];
    }

    if(user_id===null || !Array.isArray(eventIds)|| eventIds.length===0)
    {
      return res.status(400).json({ status : "Fail", message: "User ID and event list must be present"});
    }

    try{

      const entityManager = dbSourceConnection()[0];

      const checkQuery = `SELECT event_id from assign_event where user_id = $1`
      let updateQuery;
      userResult = await entityManager.query(checkQuery, [user_id]);

      if(userResult.length > 0){
        eventIdsList = userResult[0].event_id.map((i)=>Number(i));
        // console.log("Result1: ",eventIdsList);
      }
      if(eventIdsList.length>=1){
        for(const i of eventIds){
          if(eventIdsList.indexOf(i)!=-1){
            return res.json({ status: 'Error', message: `Event ${i} already assigned`});
          }
        }
      }

      const newEventsId = [... new Set([...eventIdsList, ...eventIds])];

      // console.log("New events: ",newEventsId);

      if(eventIdsList.length >0){
        updateQuery = `UPDATE assign_event set event_id = $2 where user_id = $1`;
      }else{
        updateQuery = `INSERT into assign_event (user_id, event_id) values ($1, $2)`;
      }

      const result = await entityManager.query(updateQuery, [user_id, newEventsId]);
      // console.log("Result2: ",result);
      return res.status(200).json({ message: 'Event Assigned Successfully', status: 'Success'});
  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
