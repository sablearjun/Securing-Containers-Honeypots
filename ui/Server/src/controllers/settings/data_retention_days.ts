import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const data_retention_days = async (req: Request, res: Response) => {
  try {
    const entityManager = dbSourceConnection()[1]

    const result = await entityManager.query(
      "SELECT days FROM data_retention_settings;"
    );
    res.json(result);
  } catch (error) {
    console.error("Error in retrieving query response", error);
    res.status(500).json({ message: "Internal Server Error" });
  }
};
