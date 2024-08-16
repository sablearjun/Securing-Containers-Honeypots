import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { User } from 'orm/entities/DB1/users/User';
import { CustomError } from 'utils/response/custom-error/CustomError';
import { tokenDecoder } from 'utils/createJwtToken';


export const user_details = async (req: Request, res: Response, next: NextFunction) => {
    const authHeader = req.get('Authorization');
    const jwtPayload = tokenDecoder(authHeader);
    const id = jwtPayload.id;
  try{
      const result = await dbSourceConnection()[0].getRepository(User).find({where : {id: id}});
      // console.log("Result:", result);
      res.customSuccess(200, "User Details", result);
  } catch (err) {
    const customError = new CustomError(400,'Raw','Can not retrieve user details', null, err);
    return next(customError);
  }
};
