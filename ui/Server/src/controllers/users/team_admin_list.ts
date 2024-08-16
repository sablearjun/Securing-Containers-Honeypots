import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const team_admin_list = async (req: Request, res: Response, next: NextFunction) => {
  const bodyJSON = req.query;
  let id: number | undefined;
  let admin;
  const adminList = [];

  if (bodyJSON.hasOwnProperty('team_id')) {
    id = Number(bodyJSON['team_id']);
  }

  if (id===undefined) {
    return res.status(400).json({ message: 'Bad Request' });
  }
  try {
    const entityManager = dbSourceConnection()[0];
    const result = await entityManager.query(`SELECT admin_ids from team where id = $1`, [id]);
    // admin ids will be JSON format  in result;
    if (result.length > 0) {
      const adminIds = result[0].admin_ids.split(',');

      for (let i of adminIds) {
        if (i != '') {
          admin = await entityManager.query(`SELECT * from users where id = $1`, [i]);
          adminList.push(admin);
        }
      }
      return res.customSuccess(200, 'List of admins', adminList);
    } else {
      return res.status(404).json({ message: 'Team not found.' });
    }
  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
