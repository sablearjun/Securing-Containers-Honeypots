import { Request, Response, response } from "express";
import { DataSource } from 'typeorm';
import { dbSourceConnection } from "index";

export const event_list = async (req: Request, res: Response) => {
    let events_list_json: any;
    let response_json: { events: EventResult[] } = { events: [] };
    let host_identifier: string;
    let is_open: string;
    let start_time: string;
    let end_time: string;
    let pageno: string;
    let no_of_rows_per_page: string;
    let ss: any = {}; // Depending on the usage of 'ss', you may want to replace 'any' with a specific type.
    let row_to_skip: number;
    let r1: number = 1;
    let r2: number = 100;
    let total_records: number;

    try {
        const entityManager = dbSourceConnection()[1];
        const db1Manager = dbSourceConnection()[0];

        try {
                events_list_json = req.query;

            if (events_list_json.hasOwnProperty("host_identifier")) {
                host_identifier = events_list_json["host_identifier"] as string;
            }
            if (events_list_json.hasOwnProperty("is_open")) {
                is_open = events_list_json["is_open"] as string;
            }
            if (events_list_json.hasOwnProperty("start_time")) {
                start_time = events_list_json["start_time"] as string;
            }
            if (events_list_json.hasOwnProperty("end_time")) {
                end_time = events_list_json["end_time"] as string;
            }
            if (events_list_json.hasOwnProperty("pageno")) {
                pageno = events_list_json["pageno"] as string;
                r1 = parseInt(pageno, 10);
                // r1 = r1--;
            }
            if (events_list_json.hasOwnProperty("rows")) {
                no_of_rows_per_page = events_list_json["rows"] as string;
                r2 = parseInt(no_of_rows_per_page, 10);
            }

                row_to_skip = (r1 - 1) * r2;
            } catch (error) {
                // Handle any exceptions that might occur during the processing
                console.error(error);
            }

        // Prepare and execute the first query to get the count
        const countQuery = await generateEventsCountQuery(host_identifier, is_open, start_time, end_time);
        const countResult = await entityManager.query(countQuery);
        total_records = Number(countResult[0].count);

        // Prepare and execute the second query to get the events
        const eventQuery = await generateEventQuery(host_identifier, is_open, start_time, end_time, row_to_skip, r2);
        const eventsResult: EventResult[] = await entityManager.query(eventQuery);

        response_json["no_of_records"] = total_records;

        for (const c of eventsResult) {
            // Build an event object for each iteration and push it into the 'events' array

            response_json.events.push({
                id: c.id,
                host_identifier: c.host_identifier,
                hostname: c.hostname,
                unixtime: c.unixtime,
                event_code: c.event_code,
                metadata: c.metadata,
                is_alert: c.is_alert,
                is_open: c.is_open,
                context_data: c.context_data,
                severity: c.severity,
                technique: c.technique,
                technique_id: c.technique_id,
                link: c.link,
                alert_type: c.alert_type,
                created: Number(c.created),
                last_modified: Number(c.last_modified),
                action: c.action,
                os_name:c.os_name,
                rule_written_by: c.rule_written_by,
                event_id: Number(c.event_id),
                pid: c.pid,
            });
            // console.log("response", response_json);
        }

        if (eventsResult.length <= 0) {
            return res.status(200).json({
              status: 'Fail',
              message: 'No data available',
            });
        } else {
            return res.status(200).json(response_json);
        }
    } catch (error) {
        console.error('Error in retrieving event list information:', error);
        return res.status(500).json({ message: 'Internal Server Error' });
    }
}

interface EventResult {
  id: string;
  host_identifier: string;
  hostname: string;
  unixtime: string;
  event_code: string;
  metadata: string;
  is_alert: string;
  is_open: string;
  context_data: string;
  severity: string;
  technique: string;
  technique_id: string;
  link: string;
  alert_type: string;
  created: number;
  last_modified: number;
  action: string;
  os_name: string;
  rule_written_by: string;
  event_id: number;
  pid: string;
}

enum JoinType {
  LEFT_JOIN = 'LEFT JOIN',
  FULL_JOIN = 'FULL JOIN',
}

async function generateEventQuery(
  hostIdentifier: string,
  isOpen: string,
  startTime: string,
  endTime: string,
  rowToSkip: number,
  r2: number,
  joinType: JoinType = JoinType.FULL_JOIN
): Promise<string> {
  if (!hostIdentifier && !isOpen && !startTime && !endTime) {
      joinType = JoinType.LEFT_JOIN;
    }
  let query = `select eve.*, case when (pr.created IS NULL) then '0' else created end as created, case when (pr.last_modified IS NULL) then '0' else last_modified end as last_modified, case when (pr.rule_written_by IS NULL) then 'NULL' else rule_written_by end as rule_written_by from alerts as eve ${joinType} python_rules as pr ON eve.event_code = pr.name`;

  const conditions: string[] = [];

  if (hostIdentifier) {
    conditions.push(`(host_identifier like '%${hostIdentifier}%' or hostname like '%${hostIdentifier}%')`);
  }

  if (isOpen) {
    conditions.push(`is_open = '${isOpen}'`);
  }

  if (startTime) {
    conditions.push(`unixtime >= '${startTime}'`);
  }

  if (endTime) {
    conditions.push(`unixtime <= '${endTime}'`);
  }

  if (conditions.length > 0) {
    query += ` where ${conditions.join(' and ')}`;
  }

  query += ` order by eve.unixtime desc offset ${rowToSkip} rows fetch next ${r2} rows only`;

  return query;
}

function isEmptyString(value: string | undefined | null): boolean {
  return value === undefined || value === null || value.trim() === '';
}

async function generateEventsCountQuery(
  hostIdentifier: string | undefined | null,
  isOpen: string | undefined | null,
  startTime: string | undefined | null,
  endTime: string | undefined | null
): Promise<string> {
  const conditions: string[] = [];

  if (!isEmptyString(hostIdentifier)) {
    conditions.push(`(host_identifier LIKE '%${hostIdentifier}%' OR hostname LIKE '%${hostIdentifier}%')`);
  }

  if (!isEmptyString(isOpen)) {
    conditions.push(`is_open = '${isOpen}'`);
  }

  if (!isEmptyString(startTime)) {
    conditions.push(`unixtime >= '${startTime}'`);
  }

  if (!isEmptyString(endTime)) {
    conditions.push(`unixtime <= '${endTime}'`);
  }

  let query = 'SELECT COUNT(id) from alerts';

  if (conditions.length > 0) {
    query += ` WHERE ${conditions.join(' AND ')}`;
  }

  query += ';';

  return query;
}
