import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const rule_builder_rule_update = async (req: Request, res: Response) => {
  const bodyJSON = req.body;
  let id : string | undefined = '';
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

  if(bodyJSON.hasOwnProperty('id')){
    id = String(bodyJSON['id']);
  }

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

  if(!id || ! name || !description || !link ||!platform || !severity || !tactics || !technique_id || !type || !rule_written_by || !alerters || !rule_builder)
  {
    return res.status(400).json({status:"Fail",message:'Missing payload'})
  }

    try {
        const entityManager = dbSourceConnection()[1];

      const result = await entityManager.query(
        `update rule_builder_rules set name = $1, description = $2, link = $3, platform = $4, severity = $5, tactics = $6, technique_id = $7, type = $8, alerters = $9, rule_builder = $10, last_modified = $11 where id = $12;`,
        [
          name,
          description,
          link,
          platform,
          BigInt(severity),
          tactics,
          technique_id,
          type,
          alerters,
          rule_builder,
          BigInt(last_modified),
          BigInt(id),
        ]
      );
      if (result[1]) return res.json({ status:"Success", message: `Updation of rule builder rule with id ${id} successful` });
      else return res.json({ status: "Fail", message: `Rule Builder rule with id ${id} not found. Updation not successful` });
    } catch (error) {
      console.error("Error in updating rule builder", error);
      return res.status(500).json({ status: "Fail", message: "Error in updating rule builder rule due to internal server error" });
    }
  };
