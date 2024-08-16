import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';


/*  To Do  */
export const data_retention = async (req: Request, res: Response) => {
    try {
      res.json({ message: "This is an REST API" });
    } catch (error) {
      console.error("Error in manual purge", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };