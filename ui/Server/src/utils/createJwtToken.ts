import jwt from 'jsonwebtoken';

import { JwtPayload } from '../types/JwtPayload';
import { RenewAccessJwtPayload } from 'types/RenewAccessJwtPayload';

const jwtTokenGenerator = (payload : any): string =>{
  return jwt.sign(payload, process.env.JWT_SECRET!, {
    expiresIn: process.env.JWT_EXPIRATION,
  });
}
export const createJwtToken = (payload: JwtPayload): string => {
  return jwtTokenGenerator(payload);
};

export const createRenewAcessJwtToken = (payload: RenewAccessJwtPayload): string =>{
  return jwtTokenGenerator(payload);
}

export const tokenDecoder = (authHeader: string) => {
  const token = authHeader.split(' ')[1];
  let jwtPayload: { [key: string]: any };
  jwtPayload = jwt.verify(token, process.env.JWT_SECRET as string) as { [key: string]: any };
  return jwtPayload;
}