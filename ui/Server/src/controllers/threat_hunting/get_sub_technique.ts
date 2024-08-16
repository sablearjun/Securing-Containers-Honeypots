import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

// Pending

export async function get_sub_technique(req: Request, res: Response) {
  const bodyJson = req.query;

  let tacticName : string | undefined;
  let techniqueName : string | undefined;

  if(bodyJson.hasOwnProperty('tactic')){
    tacticName = String(bodyJson['tactic']);
  }
  if(bodyJson.hasOwnProperty('technique')){
    techniqueName = String(bodyJson['technique']);
  }

  if (tacticName ===undefined || techniqueName === undefined) {
    return res.status(400).json({ "status": "Fail","message": "Tactic name and technique name should be present."});
  }

  try {
    const entityManager = dbSourceConnection()[0];
    const query = `
      SELECT st.*
      FROM sub_technique st
      JOIN tactic t ON st.tactic_id = t.tactic_id
      JOIN technique tec ON st.technique_id = tec.id
      WHERE t.tactic_name = $1 AND tec.technique_name = $2
    `;
    const values = [tacticName, techniqueName];
    const result = await entityManager.query(query, values);
    return res.status(200).json(result);
  } catch (error) {
    return res.status(500).json({ message: 'An error occurred while retrieving sub-techniques' });
  }
}

