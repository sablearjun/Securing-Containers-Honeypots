import { Request, Response, response } from 'express';
import { DataSource } from 'typeorm';
import { dbSourceConnection } from 'index';

const clients: Response[] = []; // Array to store connected clients

export const event_list_sse = async (req: Request, res: Response) => {
  res.writeHead(200, { 'Content-Type': 'text/event-stream', 'Cache-Control': 'no-cache', Connection: 'keep-alive' });

  // Add the response object to the clients array when a new connection is established
  clients.push(res);
  //  console.log("Client Size:", clients.length);
  req.on('close', () => {
    // Remove the response object from the clients array when the connection is closed
    const index = clients.indexOf(res);
    if (index !== -1) {
      clients.splice(index, 1);
    }
  });

  let lastId: number = 0;
  let lastAlertIdQuery = 'SELECT lastid from lastVisitedAlert;';
  let eventQuery = `SELECT * from alerts where id > $1 order by id desc LIMIT 100;`;
  let countLatestAlerts = `SELECT COUNT(*) from alerts where id > $1;`;
  let updateLastAlertId = `update lastVisitedAlert set lastId = $1;`;
  let count: number | 0;

  let eventsRepository: DataSource;
  let lastAlertRepository: DataSource;

  try {
    lastAlertRepository = dbSourceConnection()[0];
    eventsRepository = dbSourceConnection()[1];
  } catch (err) {
    console.error('Error in db connection', err);
    res.status(500).json({ message: 'Internal Server Error' });
  }
  const sendUpdates = () => {
    setInterval(async () => {
      let response_json: { events: EventResult[] } = { events: [] };
      try {
        if (lastId == 0) lastId = await lastAlertRepository.query(lastAlertIdQuery)[0].lastId;

        count = await eventsRepository.query(countLatestAlerts, [Number(lastId)]);

        if (count > 0) {
          const eventsResult: EventResult[] = await eventsRepository.query(eventQuery, [Number(lastId)]); // past one hour ke latest alerts

          for (const c of eventsResult) {
            // Build an event object for each iteration and push it into the 'events' array
            // const assigned_users = await eventsRepository.query(`SELECT ae.user_id, t1.username, ae.event_id from assign_event ae JOIN users t1 on t1.id = ae.user_id WHERE $1 = ANY(ae.event_id); `, [c.event_id]);

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
              os_name: c.os_name,
              rule_written_by: c.rule_written_by,
              event_id: Number(c.event_id),
              // assigned_users: assigned_users,
              pid: c.pid,
            });
          }
          const result = await lastAlertRepository.query(updateLastAlertId, [Number(eventsResult[0].id)]);
          if (result[1]) lastId = Number(eventsResult[0].id);
        }

      // for (const client of clients) {
        res.write(`data: ${JSON.stringify(response_json)}\n\n`);
        //   }
      } catch (err) {
        console.error('Error while sending SSE update:', err);
      }
    }, 7000); // You can adjust the interval (in milliseconds) based on your needs4
  };
  if(lastAlertRepository && eventsRepository)
    sendUpdates();
};

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
