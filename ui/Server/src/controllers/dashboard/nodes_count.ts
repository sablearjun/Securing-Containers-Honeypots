import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const nodes_count = async (req: Request, res: Response, next: NextFunction) => {
  const nodesRepository = dbSourceConnection()[1];
  try {
    const nodesResult = await nodesRepository.query('SELECT last_seen from nodes');

    let online = 0;
    let offline = 0;
    let diff: number;
    let currentTime = Math.floor(Date.now()/1000);
    const LIMIT: number = 60; // seconds

    for(let i = 0; i< nodesResult.length; i++){
        diff = currentTime - nodesResult[i].last_seen;
        if(diff < LIMIT){
            online++;
        }else{
            offline++;
        }
    }

    const nodes = {
        "Online" : online,
        "Offline": offline,
        "Total": online+offline,
    }

    res.status(200).json(nodes);
  } catch (err) {
    const customError = new CustomError(400, 'Raw', `Can't retrieve count of nodes.`, null, err);
    return next(customError);
  }
};
