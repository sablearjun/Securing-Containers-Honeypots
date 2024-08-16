import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const stat_node_rule_count = async (req: Request, res: Response) => {
    try {
        const entityManager = dbSourceConnection()[1]
  
      const result = await entityManager.query(
        "SELECT DISTINCT host_identifier from alerts"
      );
  
      const nodesRuleJson: any[] = [];
  
      for (const row of result.rows) {
        const hostIdent = row.host_identifier;
        const countResult = await entityManager.query(
          "SELECT severity, COUNT(severity) from alerts WHERE host_identifier = $1 GROUP BY severity",
          [hostIdent]
        );
  
        const resultJson: any = {
          host_identifier: hostIdent,
          severity: [],
        };
  
        for (const countRow of countResult.rows) {
          const severityLevel = countRow.severity;
          const severityCounts = countRow.count;
  
          resultJson.severity.push({
            severity: severityLevel.toString(),
            severity_count: severityCounts.toString(),
          });
        }
        nodesRuleJson.push(resultJson);
      }
      res.status(200).json(nodesRuleJson);
    } catch (error) {
      console.error("Error in retrieving stats nodes count", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };
  