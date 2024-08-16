import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

import { Nodes } from 'orm/entities/DB2/nodes/Nodes';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const node_alert_count = async (req: Request, res: Response, next: NextFunction) => {
  const entityManager = dbSourceConnection()[1];
  try {

    const query = "select * from (select n.*,count(a.*) from nodes n left join alerts a on n.host_identifier=a.host_identifier group by n.id,n.config_id,n.host_identifier) as p  where not count = 0 order by count desc limit 5;"

    const results = await entityManager.query(query);

    res.status(200).json(results);
  } catch (err) {
    const customError = new CustomError(400, 'Raw', `Can't retrieve count of nodes.`, null, err);
    return next(customError);
  }
};
