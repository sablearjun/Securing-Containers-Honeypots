import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';

export const os_count = async (req: Request, res: Response, next: NextFunction) => {
    try{
        const entityManager = dbSourceConnection()[1];
        const node_result = await entityManager.query(`SELECT count(*), CASE WHEN (os_name like ('%Windows%') or os_name like ('%windows%')) THEN 'Windows' WHEN (os_name like ('%Kali%') or os_name like ('%kali%') or os_name like ('%ubuntu%') or os_name like ('%Ubuntu%') or os_name like ('%Linux%') or os_name like ('%Arch%')) THEN 'Linux' WHEN (os_name like ('%Darvin%') or os_name like ('%darvin%')) THEN 'MacOS' END os FROM nodes group by os`)
        const result = node_result.map((row) => {
          return { "count" : parseInt(row.count), "name" : row.os}
        });
        res.status(200).json(result);
    } catch (err) {
    const customError = new CustomError(400, 'Raw', `Can't retrieve count of nodes.`, null, err);
    return next(customError);
  }
};
