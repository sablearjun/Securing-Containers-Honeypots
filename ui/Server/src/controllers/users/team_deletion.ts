import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { Team } from 'orm/entities/DB1/users/team/Team';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const team_deletion = async (req: Request, res: Response, next: NextFunction) => {
  console.log(req.body);
  const body = req.body;
  let id : number | undefined;

  if(body.hasOwnProperty("id")){
    id = Number(body.id);
  }

  try{
    if(id===undefined){
      return res.status(400).json({ status: 'Fail', message: "Group details should be present"});
    }
    const teamRepository = dbSourceConnection()[0].getRepository(Team);
    const team = await teamRepository.delete(id);
    console.log("Team", team);
    if(!team.affected){
      return res.status(404).json({ status: 'Fail', message: "Group deletion unsuccessful as it does not exist."});
    }
    return res.status(200).json({ status: 'Success', message: 'Group deletion successful.'});

    } catch (err) {
    const customError = new CustomError(400, 'Raw', 'Error', null, err);
    return next(customError);
  }
};
