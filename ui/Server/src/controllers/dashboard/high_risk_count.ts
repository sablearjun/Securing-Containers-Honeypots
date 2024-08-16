import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const high_risk_count = async (req: Request, res: Response, next: NextFunction) => {
  let level: string | undefined = "DESC";
  let topn: number | undefined = 1;
  let severity: string | undefined = "4";

  const queryJSON = req.query;

  if (queryJSON.hasOwnProperty("level")) {
    level = String(queryJSON.level);
  }
  if (queryJSON.hasOwnProperty("topn")) {
    topn = Number(queryJSON.topn);
  }
  if (queryJSON.hasOwnProperty("severity")) {
    severity = String(queryJSON.severity);
  }

  const entityManager = dbSourceConnection()[1];
  try {

    let result;
    if (level === "desc" || level === "DESC") {
      result = await entityManager.query(
        `select host_identifier, hostname, count(severity) from alerts where severity = $1 group by host_identifier, hostname order by count desc limit $2`,
        [severity, topn]
      );
    } else {
      result = await entityManager.query(
        `select host_identifier, hostname, count(severity) from alerts where severity = $1 group by host_identifier, hostname order by count asc limit $2`,
        [severity, topn]
      );
    }

    const highRiskAlertResult = result.map((row) => {
      return { "host_identifier" : row.host_identifier, "hostname" : row.hostname, "count": parseInt(row.count)}
    });

    res.status(200).json(highRiskAlertResult);
  } catch (err) {
    const customError = new CustomError(400, 'Raw', `Can't retrieve count of nodes.`, null, err);
    return next(customError);
  }
};
