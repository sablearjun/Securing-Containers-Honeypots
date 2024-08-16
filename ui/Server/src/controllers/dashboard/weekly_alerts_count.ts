import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const weekly_alerts_count = async (req: Request, res: Response, next: NextFunction) => {
    const entityManager = dbSourceConnection()[1];
    let host_identifier: string | undefined = "";
    const weekdayNames = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];
    const alertCount = new Map<string, Record<string, number>>();
    const alertsResultObject: { [key: string]: Record<string, number> } = {};

    try {
      if(req.query.hasOwnProperty('host_identifier')){
        host_identifier= String(req.query.host_identifier);
      }

      const currentDate = new Date();
      let dayStartTime = new Date(
        currentDate.getFullYear(),
        currentDate.getMonth(),
        currentDate.getDate(),
        0, 0, 0
      ).getTime()/1000;

      let currentTime = Math.floor(currentDate.getTime()/ 1000);
      let formattedWeekday = `${currentDate.getDate()}/${currentDate.getMonth() + 1}/${currentDate.getFullYear()}`;

      for (let i = 1; i <=7; i++) {
        alertCount.set(formattedWeekday, {});

        let query: string;
        let queryParams: any[];
        if (host_identifier != "") {
          query = `
            SELECT alert_type, COUNT(*) AS count
            from alerts
            WHERE host_identifier = $1 AND unixtime BETWEEN $2 AND $3
            GROUP BY alert_type
          `;
          queryParams = [host_identifier, dayStartTime, currentTime];
        } else {
          query = `
            SELECT alert_type, COUNT(*) AS count
            from alerts
            WHERE unixtime BETWEEN $1 AND $2
            GROUP BY alert_type
          `;
          queryParams = [dayStartTime, currentTime];
        }
        // console.log(formattedWeekday, dayStartTime, currentTime);
        alertCount.get(formattedWeekday)["File Alert"] = 0;
        alertCount.get(formattedWeekday)["Process Alert"] = 0;
        alertCount.get(formattedWeekday)["Socket Alert"] = 0;

        const result  = await entityManager.query(query, queryParams);
        result.forEach((row) => {
          alertCount.get(formattedWeekday)[row.alert_type] = parseInt(row.count);
        });


        for (const [key, value] of alertCount) {
            alertsResultObject[key] = value;
        }

        currentTime = dayStartTime;
        dayStartTime -= 86400; // Move to the previous day
        const currentDate = new Date(currentTime * 1000);
        formattedWeekday = `${currentDate.getDate()}/${currentDate.getMonth() + 1}/${currentDate.getFullYear()}`;
      }


      if (alertCount.size === 0) {
        res.status(200).json({
          status: 'Fail',
          message: 'No data available',
        });
      } else {
        res.status(200).json(alertsResultObject);
      }
    } catch (error) {
      console.error('Database error:', error);
      res.status(500).json({
        status: 'Fail',
        message: 'Query to get an event failed due to a database error.',
      });
    }
};
