import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { User } from 'orm/entities/DB1/users/User';
import { CustomError } from 'utils/response/custom-error/CustomError';


export const user_show = async (req: Request, res: Response, next: NextFunction) => {
  const {id} = req.jwtPayload;

  const userRepository = dbSourceConnection()[0];
  try {
    const user = await userRepository.query(`SELECT * from user where id = $1`, [id]);

    if (!user) {
      const customError = new CustomError(404, 'General', `User with id:${id} not found.`, ['User not found.']);
      return next(customError);
    }
    res.customSuccess(200, 'User found', user);
  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
