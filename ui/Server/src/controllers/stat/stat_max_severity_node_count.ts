import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const stat_max_severity_node_count = async (
    req: Request,
    res: Response
  ) => {
    try {
    const entityManager = dbSourceConnection()[1];
      const params = req.query;
      const level = params.level || "desc";
      const topn = params.topn || "1";
  
      const maxSeverityNodeCountJson: any = {};
  
      for (let i = 0; i <= 4; i++) {
        const query = `SELECT host_identifier, hostname, COUNT(severity) from alerts WHERE severity = '${i}' GROUP BY host_identifier, hostname ORDER BY count ${level} LIMIT ${topn}`;
        const result = await entityManager.query(query);
  
        for (const row of result.rows) {
          const hostIdentifier = row.host_identifier;
          const hostname = row.hostname;
          const count = row.count;
  
          if (!maxSeverityNodeCountJson.node_severity_count) {
            maxSeverityNodeCountJson.node_severity_count = [];
          }
  
          maxSeverityNodeCountJson.node_severity_count.push({
            host_identifier: hostIdentifier,
            hostname: hostname,
            severity: i,
            count: count,
          });
        }
      }
  
      res.status(200).json(maxSeverityNodeCountJson);
    } catch (error) {
      console.error("Error:", error);
      res
        .status(500)
        .json({ message: "Failed to retrieve max node severity count" });
    }
  };