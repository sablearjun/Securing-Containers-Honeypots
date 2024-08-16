import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const user_list = async (req: Request, res: Response, next: NextFunction) => {
  const userRepository = dbSourceConnection()[0];
  try {
    const user = await userRepository.query(`SELECT * from users order by id;`);
    res.status(200).json({data: user});
  } catch (err) {
    const customError = new CustomError(400, 'Raw', `Can't retrieve list of users.`, null, err);
    return next(customError);
  }
};
