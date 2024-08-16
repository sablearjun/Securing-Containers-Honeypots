import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

export const event_update = async (req: Request, res: Response) => {
    const parameters = req.body;
    let event_id: bigint;
    let is_open: boolean | undefined;
    let remarks: string = '';
    let action: string = 'None';

    if (parameters.hasOwnProperty('is_open')) {
      is_open = parameters['is_open'];
    }
    if (parameters.hasOwnProperty('remarks')) {
      remarks = parameters['remarks'];
    }
    if (parameters.hasOwnProperty('action')) {
      action = parameters['action'];
    }

    console.log("ReqBody: ", req.body);
    try {
      const entityManager = dbSourceConnection()[1];
      const eventIds = parameters['event_id'];
      const statusArr: number[] = [];

      if(!eventIds || is_open === undefined) return res.status(400).json({message: "Bad Request"});

      for (const eventId of eventIds) {
        event_id = eventId;
        let result;
  // Ask devesh regarding remarks ... he seems to have deleted the column
        if (action==='None') {
          const updateQuery = `UPDATE alerts SET is_open = $1 WHERE id = $2`;
          result = await entityManager.query(updateQuery, [is_open, BigInt(eventId)]);
        } else {
          const updateQuery = `UPDATE alerts SET action = $1 WHERE id = $2`;
          result = await entityManager.query(updateQuery, [action, BigInt(eventId)]);
        }

        // console.log("Event ID: ",eventId,"Result: ",result, "Previous: ",is_open);
        const status = result.length;
        statusArr.push(status);
      }

      let flag:boolean = true;

      for(const status of statusArr){
        if(status<=0)
        {
          flag = false;
          break;
        }
      }

      if (flag) {
        return res.status(200).json({
          status: 'Success',
          message: 'Event Updated Successfully.',
        });
      } else {
        return res.status(400).json({
          status: 'Fail',
          message: 'Failed to update event',
        });
      }
    } catch (error) {
      console.error('Error in updating event:', error);
      return res.status(500).json({ message: 'Internal Server Error' });
    }
  };