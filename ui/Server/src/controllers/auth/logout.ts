import { Request, Response, NextFunction } from 'express';
import { DataSource, Repository } from 'typeorm';
import { RenewAccessToken } from 'orm/entities/DB1/users/RenewAccessToken';
import { tokenDecoder } from 'utils/createJwtToken';
import { CustomError } from 'utils/response/custom-error/CustomError';
import { dbSourceConnection } from 'index';
import { User } from 'orm/entities/DB1/users/User';

export const logout = async (req: Request, res: Response, next: NextFunction) => {
  const authHeader = req.get('Authorization');
  const jwtPayload = tokenDecoder(authHeader);
  const id = jwtPayload.id;
  try{
      const dbConnection = dbSourceConnection()[0]
      const entityManager = dbConnection.getRepository(RenewAccessToken);

      const user = await dbConnection.getRepository(User).findOne({ where: { id: id}});
      if(user===undefined){
        res.status(404).json({status: 'Error', message: 'User not found for logging out'});
      }else{
        const result = await entityManager.delete({ user_name: user.username });
        res.json({ status:"Success", message: "Logout Successful"});
      }
  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
