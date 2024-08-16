import { Request, Response, NextFunction } from 'express';
import { DataSource } from 'typeorm';
import { dbSourceConnection } from 'index';
import { User } from 'orm/entities/DB1/users/User';
import { JwtPayload } from 'types/JwtPayload';
import { createJwtToken, createRenewAcessJwtToken } from 'utils/createJwtToken';
import { CustomError } from 'utils/response/custom-error/CustomError';
import { RenewAccessToken } from 'orm/entities/DB1/users/RenewAccessToken';
import { Role } from 'orm/entities/DB1/users/types';
import { serialize_user_data } from 'utils';
import { randomBytes } from 'crypto';

export const renew_access_token = async (req: Request, res: Response, next: NextFunction) => {
  const bodyJSON = req.body;
  let renew_access_token : string | undefined;

  if(bodyJSON.hasOwnProperty('renew_access_token')){
    renew_access_token = String(bodyJSON['renew_access_token']);
  }

  if(renew_access_token === undefined){
    return res.status(400).json({ message: 'Bad Request'});
  }

  const dbConnection: DataSource = dbSourceConnection()[0];

  try {
    const oldRenewAccessToken = await dbConnection.getRepository(RenewAccessToken).findOne({ where: { renew_access_token } });

    if (!oldRenewAccessToken) {
      const customError = new CustomError(403, 'General', 'Forbidden', ['No details found']);
      return next(customError);
    }

    const user = await dbConnection.getRepository(User).findOne({ where: {username: oldRenewAccessToken.user_name} })

    if (!user) {
      const customError = new CustomError(403, 'General', 'Forbidden', ['User not found']);
      return next(customError);
    }

    const jwtPayload: JwtPayload = {
      id: user.id,
      name: user.name,
      email: user.email,
      role: user.role as Role,
      created_at: user.created_at,
    };

    try {
      const token = createJwtToken(jwtPayload);
      const serialized_data = serialize_user_data(user);
      const salt = randomBytes(32).toString('hex');

      const renewAccessToken = {
        salt: salt
      }

      const renew_access_digest = createRenewAcessJwtToken(renewAccessToken);
      const newRenewAccessToken = new RenewAccessToken();
      newRenewAccessToken.user_name = user.username;
      newRenewAccessToken.renew_access_token = renew_access_digest;
      newRenewAccessToken.salt = salt;
      await dbConnection.getRepository(RenewAccessToken).save(newRenewAccessToken);
      console.log("New access token saved");

      const result = { "user_data" : serialized_data, "token" : `Bearer ${token}`, "renew_access_token" : renew_access_digest };
      return res.customSuccess(200, 'Token successfully created.', result);

    } catch (err) {
      const customError = new CustomError(400, 'Raw', "Token can't be created", null, err);
      return next(customError);
    }
  } catch (err) {
    const customError = new CustomError(500, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
