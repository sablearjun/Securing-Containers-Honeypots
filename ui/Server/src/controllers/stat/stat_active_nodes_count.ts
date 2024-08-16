import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const stat_active_nodes_count = async (
  _req: Request,
  _res: Response
) => {
  // SAME AS NODE COUNT  in nodes
  // ToDo: Remove this
};

