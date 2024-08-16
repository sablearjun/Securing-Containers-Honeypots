import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const table_list = async (req: Request, res: Response, next: NextFunction) => {

    try {
        const entityManager = dbSourceConnection()[1];

        let result;
        result = await entityManager.query(
          "SELECT * FROM config_table_name"
        );

        res.customSuccess(200, "Table list Queries", result);
    } catch (error) {
        console.error("Error in retrieving table list", error);
        res.status(500).json({ message: "Internal Server Error" });
    }
};
