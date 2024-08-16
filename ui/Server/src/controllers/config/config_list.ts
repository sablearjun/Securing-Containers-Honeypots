import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const config_list = async (req: Request, res: Response) => {
    let id: string | undefined = "";
    const configJSON = req.query;
    if (configJSON.hasOwnProperty("id")) {
      id = String(configJSON["id"]);
    }
    try {
      const entityManager = dbSourceConnection()[1];

      let result;

      if (id == "") {
        result = await entityManager.query(
          "SELECT * FROM config_queries order by id"
        );
      } else {
        result = await entityManager.query(
          `SELECT * FROM config_queries where id = $1 order by id`,
          [Number(id)]
        );
      }

      res.customSuccess(200, "Config Queries", result);
    } catch (error) {
      console.error("Error in retrieving config list", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };
