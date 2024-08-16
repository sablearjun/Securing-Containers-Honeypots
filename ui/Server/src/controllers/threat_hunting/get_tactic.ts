import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const get_tactic = async (req: Request, res: Response, next: NextFunction) => {
  try{
        const entityManager = dbSourceConnection()[0];
        const result = await entityManager.query("SELECT * from tactic ORDER BY tactic_id;");
        res.customSuccess(200, "Tactic List", result);
  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
}