import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const rule_builder_rule_delete = async (req: Request, res: Response) => {
    let id : string | undefined = '';
    if(req.body.hasOwnProperty('id')){
      id = String(req.body.id);
    }else{
      res.status(400).json({
        status: "Fail",
        message: "Id is required. Bad request"
      })
    }
    try {
    const entityManager = dbSourceConnection()[1];
    const result = await entityManager.query(
        `DELETE FROM rule_builder_rules where id = $1`,
        [id]
      );

      if(result[1])
        res.json({ status: "Success", message : `Deletion of rule builder rule with id ${id} successful`});
      else
        res.json({ status: "Fail", message : `Rule Builder Rule with id ${id} not found. Deletion not successful`});
    } catch (error) {
      console.error("Error in deleting the rule builder rule", error);
      res.status(500).json({ status: "Fail", message: "Internal Server Error" });
    }
  };