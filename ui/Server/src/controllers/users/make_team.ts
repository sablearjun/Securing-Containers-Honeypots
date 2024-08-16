import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { Team } from 'orm/entities/DB1/users/team/Team';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const make_team = async (req: Request, res: Response, next: NextFunction) => {
  console.log(req.body);
  const body = req.body;
  let name: string | undefined = '';
  let member_ids: string | undefined = '';
  let admins: string | undefined = '';
  let permissions: string | undefined = '';

  if(body.hasOwnProperty("name")){
    name = String(body.name);
  }

  if(body.hasOwnProperty("member_ids")){
    member_ids = body.member_ids.join(',');
  }

  if(body.hasOwnProperty("admins")){
    admins = body.admins.join(',');
  }

  if(body.hasOwnProperty("permissions")){
    permissions = body.permissions.join(',');
  }

  try{
    console.log(name, member_ids);
    if(name==='' || member_ids===''){
      return res.status(400).json({ status: 'Fail', message: "Team Name should be present"});
    }
    const teamRepository = dbSourceConnection()[0].getRepository(Team);
    const team = await teamRepository.findOne({ where: { name } });
    if(team){
      return res.status(400).json({ status: 'Fail', message: "Team Name should be unique."});
    }

    const newTeam = new Team();
    newTeam.name = name;
    newTeam.member_ids = member_ids;
    newTeam.admin_ids = admins;
    newTeam.permissions = permissions;
    await teamRepository.save(newTeam);
    return res.status(200).json({ status: 'Success', message: 'Team creation successfull'});

    } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
