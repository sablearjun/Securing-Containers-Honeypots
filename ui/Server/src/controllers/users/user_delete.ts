import { Request, Response, NextFunction } from 'express';
// import { getRepository } from 'typeorm';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const user_delete = async (req: Request, res: Response, next: NextFunction) => {
  const id = req.body.id;

  const userRepository = dbSourceConnection()[0];
  try {
    const user = await userRepository.query(`SELECT * from users where id = $1;`, [id]);

    if (user.length <= 0) {
      console.log("This should not be printed if defined user");
      const customError = new CustomError(404, 'General', `User with id:${id} doesn't exist.`, [`Not found`]);
      return next(customError);
    }
    const result = await userRepository.query(`DELETE from users where id= $1;`,[id]);

    // res.customSuccess(200, 'User successfully deleted.', { id: user.id, name: user.name, email: user.email });
    res.status(200).json({ status: 'Success', message: 'User successfully deleted.'})
  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
