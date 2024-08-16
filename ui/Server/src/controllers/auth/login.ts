import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { randomBytes } from 'crypto';

import { Role } from 'orm/entities/DB1/users/types';
import { User } from 'orm/entities/DB1/users/User';
import { JwtPayload } from 'types/JwtPayload';
import { serialize_user_data } from 'utils';
import { createJwtToken, createRenewAcessJwtToken } from 'utils/createJwtToken';
import { CustomError } from 'utils/response/custom-error/CustomError';
import { RenewAccessToken } from 'orm/entities/DB1/users/RenewAccessToken';

export const login = async (req: Request, res: Response, next: NextFunction) => {
  const { username, password } = req.body;

  const dbConnection = dbSourceConnection()[0];
  const userRepository = dbConnection.getRepository(User);
  const renewAccessTokenRepository = dbConnection.getRepository(RenewAccessToken);

  try {
    const user = await userRepository.findOne({ where: { username } });

    if (!user) {
      return res.status(400).json({status:'Fail',message:'Incorrect username or password'})
      //const customError = new CustomError(404, 'General', 'Not Found', ['Incorrect username or password']);
      //return next(customError);
    }

    if (!user.checkIfPasswordMatch(password)) {
      const customError = new CustomError(404, 'General', 'Not Found', ['Incorrect username or password']);
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
      newRenewAccessToken.user_name = username;
      newRenewAccessToken.renew_access_token = renew_access_digest;
      newRenewAccessToken.salt = salt;
      await renewAccessTokenRepository.save(newRenewAccessToken);
      console.log("New access token saved");

      const result = { "user_data" : serialized_data, "token" : `Bearer ${token}`, "renew_access_token" : renew_access_digest };
      res.customSuccess(200, 'Token successfully created.', result);

    } catch (err) {
      const customError = new CustomError(400, 'Raw', "Token can't be created", null, err);
      return next(customError);
    }
  } catch (err) {
    const customError = new CustomError(500, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
