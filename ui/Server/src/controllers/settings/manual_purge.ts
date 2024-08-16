import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const manual_purge = async (req: Request, res: Response) => {
  const days = String(req.body.days);
  const seconds = parseInt(days) * 86400;
  const now = Math.floor(Date.now() / 1000);
  const cutoffTime = now - seconds;
  let affectedRows = 0;

  console.log('Days: ', days, 'cutoffTime: ', cutoffTime);
  const tables: string[] = [
    'alerts',
    'bpf_file_events',
    'bpf_process_events',
    'bpf_socket_events',
    'chrome_extensions',
    'cpu_time',
    'file',
    'interface_addresses',
    'logged_in_users',
    'memory_info',
    'process_events',
    'process_open_files',
    'process_open_pipes',
    'process_open_sockets',
    'query_result',
    'snapshot_result',
    'socket_events',
    'suid_bin',
    'user_events',
    'wmi_cli_event_consumers',
    'win_process_events',
    'win_file_events',
    'win_socket_events',
  ];
  try {
    if (days === null || req.body.days === undefined || isNaN(Number(days))|| Number(days) < 60) {
      res.status(400).json({ status: 'Fail', message: 'Bad request' });
    } else {
      const entityManager = dbSourceConnection()[1];

      affectedRows = 0;

      await Promise.all(
        tables.map(async (table: string) => {
          const query = `DELETE FROM ${table} WHERE unixtime < ${cutoffTime}`;
          try {
            const result = await entityManager.query(query);
            affectedRows += result.rowCount;
          } catch (error) {
            console.error('Error in retrieving query response', error);
            res.status(500).json({ message: 'Internal Server Error' });
          }
        }),
      );
      if (affectedRows <= 0) res.status(200).json({ status: 'Fail', message: 'No data available' });
      else res.status(200).json({ status: 'Success', message: 'Data purged successfully' });
    }
  } catch (error) {
    console.error('Error in manual purge', error);
    res.status(500).json({ message: 'Internal Server Error' });
  }
};
