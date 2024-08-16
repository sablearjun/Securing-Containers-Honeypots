import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const mitre_analysis = async (req: Request, res: Response, next: NextFunction) => {
    const entityManager = dbSourceConnection()[1];
    let host_identifier: string | undefined = "";
    const queryJSON = req.query;

    if (queryJSON.hasOwnProperty("host_identifier")) {
        host_identifier = String(queryJSON["host_identifier"]);
    }
    let eventsResult, query;

    try{
        if(host_identifier != ""){
            query = `SELECT technique, COUNT(*) as count from alerts
            WHERE host_identifier = $1
            GROUP BY technique`;
            eventsResult = await entityManager.query(query, [ host_identifier ]);
        }else{

            query = `SELECT technique, COUNT(*) as count from alerts GROUP BY technique`;
            eventsResult = await entityManager.query(query);
        }

        const mitr  = new Map <string, number>();

        let currentValue = 0;
        for(let i=0; i<eventsResult.length; i++){
            const num = Number(eventsResult[i].count);
            const technique_name = eventsResult[i].technique.toUpperCase()
            if(technique_name!= ""){
                const strs = technique_name.split(",")
                // console.log(strs, "\n name \n");
                for (let j=0; j<strs.length; j++){
                    strs[j] = strs[j].trim();
                    if(mitr.has(strs[j])){
                        currentValue = mitr.get(strs[j]);
                        mitr.set(strs[j], currentValue + num);
                    }else{
                        mitr.set(strs[j], num);
                    }
                }
            }
            // else{
            //     if(mitr.has("Unnamed")){
            //         currentValue = mitr.get("Unnamed");
            //         mitr.set("Unnamed", currentValue + num);
            //     }else{
            //         mitr.set("Unnamed", num);
            //     }
            // }

        }
        // console.log(mitr);

        const mitrResultObject: { [key: string]: number } = {};

        for (const [key, value] of mitr) {
            mitrResultObject[key] = value;
        }

        if(Object.keys(mitrResultObject).length > 0)
            res.status(200).json(mitrResultObject);
        else
            res.status(200).json({ status : "Fail", message : "No data available"})
    } catch (err) {
    const customError = new CustomError(400, 'Raw', `Can't retrieve count of nodes.`, null, err);
    return next(customError);
  }
};
