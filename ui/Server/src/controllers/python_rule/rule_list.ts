import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const rule_list = async (req: Request, res: Response) => {
    let ruleConfig = req.query;
    let id: string | undefined = "";
    let os_name: string | undefined = "";
  
    if (ruleConfig.hasOwnProperty("id")) {
      id = String(ruleConfig["id"]);
    }
  
    if (ruleConfig.hasOwnProperty("os_name")) {
      os_name = String(ruleConfig["os_name"]);
    }
  
    try {
      const entityManager = dbSourceConnection()[1];
  
      let result;
  
      if (id == "" && os_name != "") {
        result = await entityManager.query(
          `SELECT * FROM python_rules where os_name = $1 order by id`,
          [os_name]
        );
      } else if (id != "") {
        result = await entityManager.query(
          `SELECT * FROM python_rules where id = $1`,
          [BigInt(id)]
        );
      } else {
        result = await entityManager.query("SELECT * FROM python_rules order by id");
      }
      res.customSuccess(200, "Python Rules", result);
    } catch (error) {
      console.error("Error in retrieving rules list", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };