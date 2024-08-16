import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const stat_nodes_count = async (req: Request, res: Response) => {
    try {
        const entityManager = dbSourceConnection()[1];
      const result = await entityManager.query(
        "SELECT count(host_identifier) from nodes"
      );
      res.json(result.rows);
    } catch (error) {
      console.error("Error in retrieving stats nodes count", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };