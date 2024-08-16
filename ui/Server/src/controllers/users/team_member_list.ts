import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const team_member_list = async (req: Request, res: Response, next: NextFunction) => {
  const bodyJSON = req.query;
  let id;
  let member;
  const memberList = [];

  if (bodyJSON.hasOwnProperty('team_id')) {
    id = Number(bodyJSON['team_id']);
  }

  if (id === undefined) {
    return res.status(400).json({ message: 'Bad Request' });
  }
  try {
    const entityManager = dbSourceConnection()[0];
    const result = await entityManager.query(`SELECT member_ids from team where id = $1`, [id]);
    // admin ids will be JSON format in result;
    if (result.length > 0) {
      const memberIds = result[0].member_ids.split(',');

      for (let i of memberIds) {
        if (i != '') {
          member = await entityManager.query(`SELECT * from users where id = $1`, [i]);
          memberList.push(member);
        }
      }
      return res.customSuccess(200, 'List of members', memberList);
    } else {
      return res.status(404).json({ message: 'Team not found.' });
    }
  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
