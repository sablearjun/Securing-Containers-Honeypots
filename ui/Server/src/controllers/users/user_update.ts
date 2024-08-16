import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { User } from 'orm/entities/DB1/users/User';
import { tokenDecoder } from 'utils/createJwtToken';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const user_update = async (req: Request, res: Response, next: NextFunction) => {
  const authHeader = req.get('Authorization');
  const jwtPayload = tokenDecoder(authHeader);
 // const id = jwtPayload.id;
 // console.log("Id: ", id);
 
  const body = req.body;
  const id = Number(String(body.id));
  const username= String(body.username);
  const name = String(body.name);
  const role = String(body.role);
  const password = String(body.password);
  //const passwordConfirm = String(body.passwordConfirm);
  const email = String(body.email);

  // const userRepository = dbSourceConnection()[0];
  // try {
  //   const result = await userRepository.query(`SELECT * from users where id = $1;`, [id]);
  //   const user = result[0];
  //   // console.log('User', user);
  //   if (user===undefined) {
  //     const customError = new CustomError(404, 'General', `User with id:${id} not found.`, ['User not found.']);
  //     return next(customError);
  //   }
  //   if(username === undefined || name===undefined ||role===undefined){
  //     return res.status(400).json({ status: 'Fail', message: 'Entire updated record expected'})
  //   }

  //   try {
  //     const result = await userRepository.query(`UPDATE users SET username = $1, name = $2, role = $3 where id = $4`, [username, name, role, id]);
  //     res.customSuccess(200, 'User details successfully updated.');
  //   } catch (err) {
  //     const customError = new CustomError(409, 'Raw', `User '${user.email}' details can't be saved.`, null, err);
  //     return next(customError);
  //   }
  // } catch (err) {
  //   const customError = new CustomError(400, 'Raw', 'Error', null, err);
  //   return next(customError);
  // }
  const dbConnection = dbSourceConnection()[0];
  const userRepository = dbConnection.getRepository(User);
  
try{
  const user = await userRepository.findOne({ where: { id } });

  if (!user) {
    return res.status(400).json({status:'Fail',message:'Incorrect username or password'})
  }
  user.username = username;
  user.name = name;
  user.email = email;
  user.password = password;
  if(role)
  user.role = role;
  user.hashPassword();
  await userRepository.save(user);

  res.status(200).json({ status:'Success', message: 'User successfully Updated.'});
}
catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }



};
