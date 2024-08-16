import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const rule_builder_rule_list = async (req: Request, res: Response) => {
    let id :string | undefined = "";
    let os_name : string | undefined = "";
    const bodyJSON = req.query;

    if(bodyJSON.hasOwnProperty('id')){
      id = String(req.query.id);
    }
    if(bodyJSON.hasOwnProperty('os_name')){
      os_name = String(req.query.os_name);
    }

    try {
        const entityManager = dbSourceConnection()[1];

      let result;

      if (id == "" && os_name != "") {
        result = await entityManager.query(
          `SELECT * FROM rule_builder_rules where os_name = $1 order by id`,
          [os_name]
        );
      } else if (id != "") {
        result = await entityManager.query(
          `SELECT * FROM rule_builder_rules where id = $1`,
          [id]
        );
      } else {
        result = await entityManager.query(
          "SELECT * FROM rule_builder_rules order by id"
        );
      }
      res.customSuccess(200, "Rule Builder Rules Queries", result);

    } catch (error) {
      console.error("Error in retrieving rules list", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };
