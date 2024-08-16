import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const add_member = async (req: Request, res: Response, next: NextFunction) => {
  const body = req.body;
  // console.log("Body: ", body);
  let team_id: string | undefined = '';
  let member_ids: string | undefined = '';
  let append: boolean | undefined = true;
  let membersList : string | undefined = '';

  if (body.hasOwnProperty("team_id")) {
    team_id = body.team_id;
  } else {
    return res.status(400).json({ message: "Team Id should be present" });
  }

  if (body.hasOwnProperty("member_ids")) {
    member_ids = body.member_ids;
  } else {
    return res.status(400).json({ message: "Team member ids should be present" });
  }

  if (body.hasOwnProperty("append")) {
    append = Boolean(body.append);
  }

  let result;
  const query1 = `UPDATE team set member_ids = $1 where id = $2;`;
  const query2 = `SELECT member_ids from team where id = $1;`;
  try {
    const entityManager = dbSourceConnection()[0];
    if (!append) {
      result = await entityManager.query(query1, [member_ids, team_id]);
    }
    else {
      result = await entityManager.query(query2, [team_id]);

      if (result[0].member_ids == "") {
        result = await entityManager.query(query1, [member_ids, team_id]);
      } else {
        const existing_members = new Set(result[0].member_ids.split(','));

        const new_members = new Set(member_ids);
        new_members.forEach((member) => existing_members.add(member));
        // console.log("New Members:", existing_members);
        const new_member_list = Array.from(existing_members).join(',');
        result = await entityManager.query(query1, [new_member_list, team_id]);
      }
    }

    if (result)
      res.status(200).json({ status : 'Success', message: 'Group member updation successful' });
    else
      res.status(500).json({ status: 'Fail', message: 'Error in adding members' });

  } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
