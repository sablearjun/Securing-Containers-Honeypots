import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const rule_add = async (req: Request, res: Response) => {
  // return res.json(req.body);
  const bodyJSON = req.body;
  let rule : string | undefined = '';
  let name : string | undefined = '';
  let severity : string | undefined = '';
  let technique : string | undefined = '';
  let technique_id :string | undefined = '';
  let link : string | undefined = '';
  let comment : string | undefined = '';
  let os_name : string | undefined = '';
  const created : number = new Date().getSeconds();
  const last_modified : number = created;
  let rule_written_by : string | undefined = '';
  
  if(bodyJSON.hasOwnProperty('rule')){
    rule = String(bodyJSON['rule']);
  }
  if(bodyJSON.hasOwnProperty('name')){
    name = String(bodyJSON['name']);
  }
  if(bodyJSON.hasOwnProperty('severity')){
    severity = String(bodyJSON['severity']);
  }
  if(bodyJSON.hasOwnProperty('technique')){
    technique = String(bodyJSON['technique']);
  }
  if(bodyJSON.hasOwnProperty('technique_id')){
    technique_id = String(bodyJSON['technique_id']);
  }
  if(bodyJSON.hasOwnProperty('link')){
    link = String(bodyJSON['link']);
  }
  if(bodyJSON.hasOwnProperty('comment')){
    comment = String(bodyJSON['comment']);
  }
  if(bodyJSON.hasOwnProperty('os_name')){
    os_name = String(bodyJSON['os_name']);
  }
  if(bodyJSON.hasOwnProperty('rule_written_by')){
    rule_written_by = String(bodyJSON['rule_written_by']);
  }

  // console.log("Name : ", name);
  //  res to empty things
  try {
    const entityManager = dbSourceConnection()[1];

    const created = Math.floor(Date.now() / 1000);
    const last_modified = created;
    const result = await entityManager.query(
      `INSERT into python_rules (rule, name, severity, technique, technique_id, link, comment, os_name, created, last_modified, rule_written_by) values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)`,
      [
        rule,
        name,
        BigInt(severity),
        technique,
        technique_id,
        link,
        comment,
        os_name,
        created,
        BigInt(last_modified),
        rule_written_by,
      ]
    );
    const ruleAdditionResult = result;
    res.json({ status: "Success", message : `Python Rule ${name} added successfully`});
  } catch (error) {
    console.error("Error in adding to Python Rules", error);
    res.status(500).json({ status: "Fail", message: "Error in adding to Python Rules. Internal Server Error" });
  }
};
