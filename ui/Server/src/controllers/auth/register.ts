import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

import { User } from 'orm/entities/DB1/users/User';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const register = async (req: Request, res: Response, next: NextFunction) => {
  const { username, name, role, email, password } = req.body;

  const userRepository = dbSourceConnection()[0].getRepository(User);
  try {
    const user = await userRepository.findOne({ where: { username } });

    if (user) {
      const customError = new CustomError(400, 'General', 'User already exists', [
        `Username '${user.username}' already exists`,
      ]);
      return next(customError);
    }

    try {
      const newUser = new User();
      newUser.username = username;
      newUser.name = name;
      newUser.email = email;
      newUser.password = password;
      if(role)
        newUser.role = role;
      newUser.hashPassword();
      await userRepository.save(newUser);

      res.status(200).json({ status:'Success', message: 'User successfully created.'});
    } catch (err) {
      const customError = new CustomError(400, 'Raw', `User '${email}' can't be created`, null, err);
      return next(customError);
    }
  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
