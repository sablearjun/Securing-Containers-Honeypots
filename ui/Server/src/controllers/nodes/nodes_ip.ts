import { dbSourceConnection } from 'index';
import { Request, Response, NextFunction } from 'express';

export async function nodes_ip(req: Request, res: Response) {
  console.log('ip service function called');
  const nodes_result: any = {};
  let host_identifier;
  let osName: string = 'linux';
  let ipQuery: string = `SELECT address FROM interface_addresses WHERE host_identifier = $1 AND broadcast <> '' AND interface <> 'lo' AND (interface LIKE '%wl%' OR interface LIKE 'en%' OR interface LIKE 'eth%') ORDER BY id DESC LIMIT 1`;

  try {
    const requestQuery = req.query;
    const entityManager = dbSourceConnection()[1];

    if (requestQuery.hasOwnProperty('host_identifier')) {
      host_identifier = String(requestQuery['host_identifier']);
    }

    if (requestQuery.hasOwnProperty('os_name')) {
      osName = String(osName);
    }

    if (host_identifier === undefined) {
      res.status(400).json({ status: 'Fail', message: 'Bad Request' });
    } else {
      if (osName === 'windows') {
        // ipQuery = `SELECT i.interface, i.address, i.mask FROM interface_addresses i WHERE i.host_identifier = $1 AND (i.address LIKE '192.168%%' OR i.address LIKE '172.16%%' OR i.address LIKE '172.17%%' OR i.address LIKE '172.18%%' OR i.address LIKE '172.19%%' OR i.address LIKE '172.20%%' OR i.address LIKE '172.21%%' OR i.address LIKE '172.22%%' OR i.address LIKE '172.23%%' OR i.address LIKE '10.%%') AND i.address NOT LIKE '127.%%' AND i.address IS NOT NULL AND i.address NOT LIKE '%::1%' AND i.mask NOT LIKE 'ffff:ffff:ffff:ffff::' AND i.mask <> 'ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff' ORDER BY i.interface DESC, i.unixtime DESC LIMIT 1;`;
        ipQuery = `SELECT
        ia.address,
        id.mac
    FROM
        interface_addresses ia
        JOIN interface_details id ON id.interface = ia.interface
      -- On Unix ia.interface is the name of the interface,
      -- whereas on Windows ia.interface is the IP of the interface.
        JOIN routes r ON r.interface = ia.address
    WHERE
      -- Destination 0.0.0.0/0 is the default route on route tables.
        r.destination = '0.0.0.0' AND r.netmask = 0
      -- Type of route is "gateway" for Unix, "remote" for Windows.
        AND r.type = 'remote'
      -- We are only interested on private IPs (some devices have their Public IP as Primary IP too).
        AND (
        -- Private IPv4 addresses.
        ia.address IS NOT NULL AND (
          (ia.address ~ '^10\.' OR (ia.address ~ '^172\.' AND (split_part(ia.address, '.', 2)::integer & 240) = 16) OR ia.address ~ '^192\.168\.')
          OR (ia.address ~* '^f[cd][0-9a-f][0-9a-f]:[0-9a-f:]+'))
        -- Private IPv6 addresses start with 'fc' or 'fd'.
        OR ia.address IS NULL AND regex_match(lower(ia.address), '^f[cd][0-9a-f][0-9a-f]:[0-9a-f:]+', 0) IS NOT NULL)
      )
    ORDER BY
        r.metric ASC,
      -- Prefer IPv4 addresses over IPv6 addresses if their route have the same metric.
      inet_aton(ia.address) IS NOT NULL DESC
    LIMIT 1;
    `
      }

      const result = await entityManager.query(ipQuery, [host_identifier]);
      nodes_result.addresses = [];
      for (const row of result) {
        nodes_result.addresses.push(row.address);
      }

      if (nodes_result.addresses.length ==0) {
        res.status(200).json({
          status: 'Fail',
          message: 'No data available',
        });
      } else {
        res.status(200).json(nodes_result);
      }
    }
  } catch (error) {
    console.error('Database error:', error);
    res.status(500).json({
      status: 'Fail',
      message: 'Query to get node IP failed due to a database error.',
    });
  }
}
