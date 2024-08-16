import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';


export const rule_builder_rule_add = async (req: Request, res: Response) => {
  const bodyJSON = req.body;
  let name : string | undefined = '';
  let description : string | undefined = '';
  let link : string | undefined = '';
  let platform : string | undefined = '';
  let severity : string | undefined = '';
  let tactics : string | undefined = '';
  let technique_id :string | undefined = '';
  let type : string | undefined = '';
  let rule_written_by : string | undefined = '';
  let alerters : string | undefined = '';
  let rule_builder : string | undefined = '';
  const created : number = Math.floor(Date.now() / 1000);
  const last_modified : number = created;

  if(bodyJSON.hasOwnProperty('name')){
    name = String(bodyJSON['name']);
  }

  if(bodyJSON.hasOwnProperty('description')){
    description = String(bodyJSON['description']);
  }

  if(bodyJSON.hasOwnProperty('link')){
    link = String(bodyJSON['link']);
  }

  if(bodyJSON.hasOwnProperty('platform')){
    platform = JSON.stringify(bodyJSON['platform']);
    // console.log("platform", platform);
  }

  if(bodyJSON.hasOwnProperty('severity')){
    severity = String(bodyJSON['severity']);
  }

  if(bodyJSON.hasOwnProperty('tactics')){
    tactics = String(bodyJSON['tactics']);
  }

  if(bodyJSON.hasOwnProperty('technique_id')){
    technique_id = String(bodyJSON['technique_id']);
  }

  if(bodyJSON.hasOwnProperty('type')){
    type = String(bodyJSON['type']);
  }

  if(bodyJSON.hasOwnProperty('rule_written_by')){
    rule_written_by = String(bodyJSON['rule_written_by']);
  }

  if(bodyJSON.hasOwnProperty('alerters')){
    alerters = JSON.stringify(bodyJSON['alerters']);
  }

  if(bodyJSON.hasOwnProperty('rule_builder')){
    rule_builder = JSON.stringify(bodyJSON['rule_builder']);
  }

  try {

    const entityManager = dbSourceConnection()[1];

    const created = Math.floor(Date.now() / 1000);
    const last_modified = created;
    const result = await entityManager.query(
      `insert into rule_builder_rules (name, description, link, platform, severity, tactics, technique_id, type, rule_written_by, alerters, rule_builder, created, last_modified ) values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13);`,
      [
        name,
        description,
        link,
        platform,
        BigInt(severity),
        tactics,
        technique_id,
        type,
        rule_written_by,
        alerters,
        rule_builder,
        BigInt(created),
        BigInt(last_modified),
      ]
    );
    const ruleBuilderAdditionResult = result;
    res.json({ status: "Success", message: `Rule Builder Rule ${name} added successfully`});
  } catch (error) {
    console.error("Error in adding rule to rule builder", error);
    res.status(500).json({ status: "Fail", message: "Error in adding rule to rule builder due to internal server error" });
  }
};

