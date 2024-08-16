import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const severity_count = async (req: Request, res: Response, next: NextFunction) => {
    const entityManager = dbSourceConnection()[1];
    let host_identifier: string | undefined = "";
    const queryJSON = req.query;

    if (queryJSON.hasOwnProperty("host_identifier")) {
        host_identifier = String(queryJSON["host_identifier"]);
    }

    let eventsResult;

    try {
        if (host_identifier != "") {
            const query = `
                SELECT severity, COUNT(*) from alerts
                WHERE host_identifier = $1
                GROUP BY severity;
                `;
            eventsResult = await entityManager.query(query, [ host_identifier ])
        } else {
            eventsResult = await entityManager.query(`select severity, count(severity) from alerts group by severity;`)
        }

        const result = eventsResult.map((item) =>{
            return { "severity": parseInt(item.severity),  "value" : parseInt(item.count)}
        })

        const labels = ["None", "Information", "Low", "Medium", "High"];

        const finalResult = labels.map((label, index) => {
              const item = result.find((entry:any) => entry.severity === index);
              return {
                name: label,
                value: item ? item.value : 0
              };
            });

        // console.log("Final Result : ", finalResult);
        const total = finalResult.reduce((sum, item) => sum + item.value, 0);
        if(total >0)
            res.status(200).json(finalResult);
        else{
            res.status(200).json({ status : "Fail", message : "No data available"});
        }

    } catch (err) {
        const customError = new CustomError(400, 'Raw', `Can't retrieve count of nodes.`, null, err);
        return next(customError);
    }
};
