import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const rule_delete = async (req: Request, res: Response) => {
    // console.log("Body: ", req.body.id);

    const reqBody = req.body;
    let id : string | undefined = '';
    if(reqBody.hasOwnProperty('id')){
      id = String(reqBody['id']);
    }

    if (!id) res.status(400).json("Need id. Bad request");
    try {
      const entityManager = dbSourceConnection()[1];
      const result = await entityManager.query(
        `DELETE FROM python_rules where id = $1`,
        [BigInt(id)]
      );

      if(result[1])
        res.json({ status: "Success", message : `Deletion of python rule with id ${id} successful`});
      else
        res.json({ status: "Fail", message : `Python Rule with id ${id} not found. Deletion not successful`});

    } catch (error) {
      console.error("Error in deleting the python rule", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };