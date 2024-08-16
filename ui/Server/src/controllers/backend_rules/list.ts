import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

export const list = async (req: Request, res: Response) => {
    try {
      const entityManager = dbSourceConnection()[1];

      let result;
        result = await entityManager.query(
          "SELECT * FROM rules where technique <>'RULE_BUILDER' order by id"
        );

      res.customSuccess(200, "Backend Rules", result);
    } catch (error) {
      console.error("Error in retrieving config list", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };
