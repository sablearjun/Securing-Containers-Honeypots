import { Request, Response, NextFunction } from 'express';
import validator from 'validator';

import { CustomError } from 'utils/response/custom-error/CustomError';
import { ErrorValidation } from 'utils/response/custom-error/types';

export const validatorLogin = (req: Request, res: Response, next: NextFunction) => {
  let { username, password } = req.body;
  const errorsValidation: ErrorValidation[] = [];

  username = !username ? '' : username;
  password = !password ? '' : password;

  if (validator.isEmpty(username)) {
    errorsValidation.push({ email: 'Email field is required' });
  }

  if (validator.isEmpty(password)) {
    errorsValidation.push({ password: 'Password field is required' });
  }

  if (errorsValidation.length !== 0) {
    const customError = new CustomError(400, 'Validation', 'Login validation error', null, null, errorsValidation);
    return next(customError);
  }
  return next();
};
