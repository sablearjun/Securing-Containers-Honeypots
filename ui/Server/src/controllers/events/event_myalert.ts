import { Request, Response, response } from "express";
import { DataSource } from 'typeorm';
import { dbSourceConnection } from "index";
import { tokenDecoder } from "utils/createJwtToken";

export const event_myalert = async (req: Request, res: Response) => {
    const authHeader = req.get('Authorization');
    const jwtPayload = tokenDecoder(authHeader);
    const id = jwtPayload.id;

    try{
        const db1 = dbSourceConnection()[0];
        const db2 = dbSourceConnection()[1];

        const result = await db1.query(`SELECT event_id from assign_event where user_id  = $1`, [id]);
        if(result.length == 0) return res.status(200).json([]);
            const eventList = result[0].event_id;
        const eventQuery = `SELECT * from alerts where id in (${eventList.filter((i:number)=>i!==null).join(',')});`;
        const eventsResult: EventResult[] = await db2.query(eventQuery);

        let response_json = { events: []};
        for (const c of eventsResult) {

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
        }
        return res.status(200).json(response_json);
    }
    catch (error) {
        console.error('Error in retrieving my alert list information', error);
        return res.status(500).json({ message: 'Error in retrieving my alert list information' });
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