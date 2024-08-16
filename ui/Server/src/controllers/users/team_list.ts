import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { Team } from 'orm/entities/DB1/users/team/Team';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const team_list = async (req: Request, res: Response, next: NextFunction) => {
  try{
      const result = await dbSourceConnection()[0].getRepository(Team).find({ order: { id: 'ASC'}});
      res.customSuccess(200, "List of teams", result);
  } catch (err) {
    const customError = new CustomError(400,'Raw','Can not retrieve team list', null, err);
    return next(customError);
  }
};
