import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const rule_update = async (req: Request, res: Response) => {
    const {
      id,
      name,
      rule,
      severity,
      technique,
      technique_id,
      link,
      comment,
      os_name,
      rule_written_by,
    } = req.body;
    try {
      const entityManager = dbSourceConnection()[1];
  
      const last_modified = Math.floor(Date.now() / 1000);
      const result = await entityManager.query(
        `update python_rules set name = $2, rule = $3, severity = $4, technique = $5, technique_id = $6, link = $7, comment = $8, os_name = $9, last_modified = $10, rule_written_by = $11 where id = $1`,
        [
          BigInt(id),
          name,
          rule,
          BigInt(severity),
          technique,
          technique_id,
          link,
          comment,
          os_name,
          BigInt(last_modified),
          rule_written_by,
        ]
      );
      if (result[1]) res.json({ status: "Success", message: `Updation of python rule with id ${id} successful` });
      else res.json({ status: "Fail", message: `Python rule with id ${id} not found. Updation not successful` });
    } catch (error) {
      console.error("Error in updating python rule", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };
  