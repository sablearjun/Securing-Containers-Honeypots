import { Request, Response, NextFunction } from 'express';

import { CustomError } from '../utils/response/custom-error/CustomError';

export const errorHandler = (err: any, req: Request, res: Response, next: NextFunction) => {
  if(('type' in err) && (err['type']=== 'entity.parse.failed')){
    return res.status(400).json({ message: 'Invalid content type. Only JSON is supported.'})
  }
  return res.status(err.HttpStatusCode).json(err.JSON);
};
