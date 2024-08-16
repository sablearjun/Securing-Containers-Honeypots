import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const add_admin = async (req: Request, res: Response, next: NextFunction) => {
  const body = req.body;
  let team_id: string | undefined = '';
  let admin_ids: string | undefined = '';
  let append: boolean | undefined = true;

  if (body.hasOwnProperty("team_id")) {
    team_id = body.team_id;
  } else {
    return res.status(400).json({ message: "Team Id should be present" });
  }

  if (body.hasOwnProperty("admin_ids")) {
    admin_ids = body.admin_ids;
    const adminSet = new Set(admin_ids);
    admin_ids = Array.from(adminSet).join(',');
  } else {
    return res.status(400).json({ message: "Team admin ids should be present" });
  }

  if (body.hasOwnProperty("append")) {
    append = body.append;
  }

  let result;
  const query1 = `UPDATE team set admin_ids = $1 where id = $2;`;
  const query2 = `SELECT admin_ids from team where id = $1;`;
  try {
    const entityManager = dbSourceConnection()[0];
    if (!append) {

      result = await entityManager.query(query1, [admin_ids, team_id]);
    }
    else {
      result = await entityManager.query(query2, [team_id]);

      if (result[0].admin_ids == "") {
        result = await entityManager.query(query1, [admin_ids, team_id]);
      } else {
        const existing_admins = new Set(result[0].admin_ids.split(','));

        const new_admins = new Set(admin_ids);

        new_admins.forEach((admin) => existing_admins.add(admin));
        const new_admin_list = Array.from(existing_admins).join(',');

        result = await entityManager.query(query1, [new_admin_list, team_id]);
      }
    }

    if (result)
      res.status(200).json({ message: 'Admin addition successfull' });
    else
      res.status(500).json({ message: 'Error in adding admins' });

  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
