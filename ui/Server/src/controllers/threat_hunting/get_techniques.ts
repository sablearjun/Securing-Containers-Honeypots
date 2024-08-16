import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

export async function get_techniques(req: Request, res: Response) {
  const queryJSON = req.query;
  let tacticName :string | undefined ='';

  if (queryJSON.hasOwnProperty('tactic')) {
    tacticName = String(queryJSON['tactic']);
  }

  console.log('Tactic:', tacticName);
  if (tacticName==='') {
    return res.status(400).json({ "status": "Fail", "message":"Tactic name should be present" });
  }

  try {
    const entityManager = dbSourceConnection()[0];
    const query = `
      SELECT t.*
      FROM technique t
      JOIN tactic tc ON t.tactic = tc.tactic_id
      WHERE tc.tactic_name = $1;
    `;
    const techniqueList = await entityManager.query(query, [tacticName]);
    return res.status(200).json(techniqueList);
  } catch (error) {
    return res.status(500).json({ error: 'An error occurred while retrieving techniques' });
  }
}

