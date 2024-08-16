import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const config_delete = async (req: Request, res: Response) => {
    const id = req.body.id;
    try {
      if (!id) res.status(400).json({ message: "Bad request" });

      const entityManager = dbSourceConnection()[1];
      const result = await entityManager.query(
        `DELETE FROM config_queries where id = $1`,
        [BigInt(id)]
      );

      if(result[1])
        res.json({ status: "Success", message : `Deletion of config query with id ${id} successful`});
      else
        res.json({ message : `Config query with id ${id} not found. Deletion not successful`});
    } catch (error) {
      console.error("Error in deleting the config query", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };