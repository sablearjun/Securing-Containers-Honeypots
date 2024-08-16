import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const data_retention_days_update = async (req: Request, res: Response) => {
    const days = Number(req.body.days);

    if(!days) res.status(400).json({ message: "Parameters missing"});

    try {
    const entityManager = dbSourceConnection()[1]

    const result = await entityManager.query(
        `UPDATE data_retention_settings set days = $1`,[days]
    );
    if(result[1])
        res.json({status: "Success", message: "Updation successful"});
    else
        res.json({ status: "Fail", message: "Updation not successful" });
    } catch (error) {
    console.error("Error in retrieving query response", error);
    res.status(500).json({ message: "Internal Server Error" });
    }
};
