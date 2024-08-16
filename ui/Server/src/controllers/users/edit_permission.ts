import { Request, Response, NextFunction } from 'express';
import { DataSource } from 'typeorm';

import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

// Pending
export const edit_permission = async (req: Request, res: Response, next: NextFunction) => {
  const body = req.body;
  let team_id: string | undefined = '';
  let permissions: string | undefined = '';
  let append: boolean | undefined = true;

  if (body.hasOwnProperty("team_id")) {
    team_id = body.team_id;
  } else {
    return res.status(400).json({ message: "Team Id should be present" });
  }

  if (body.hasOwnProperty("permissions")) {
    permissions = body.permissions;
    const permissionsSet = new Set(permissions);
    permissions = Array.from(permissionsSet).join(',');
  } else {
    return res.status(400).json({ message: "Team member ids should be present" });
  }

  if (body.hasOwnProperty("append")) {
    append = body.append;
  }

  let result;
  const query1 = `UPDATE team set permissions = $1 where id = $2;`;
  const query2 = `SELECT permissions from team where id = $1;`;
  try {
    const entityManager:DataSource = dbSourceConnection()[0];
    if (!append) {
      result = await entityManager.query(query1, [permissions, team_id]);
    }
    else {
      result = await entityManager.query(query2, [team_id]);

      if (result[0].member_ids == "") {
        result = await entityManager.query(query1, [permissions, team_id]);
      } else {
        const existing_permissions = new Set(result[0].permissions.split(','));

        const new_permissions = new Set(permissions);
        new_permissions.forEach((member) => existing_permissions.add(member));
        const new_permission_list = Array.from(existing_permissions).join(',');
        result = await entityManager.query(query1, [new_permission_list, team_id]);
      }
    }

    if (result)
      res.status(200).json({ message: 'Permission update successfull' });
    else
      res.status(500).json({ message: 'Error in adding members' });

  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
