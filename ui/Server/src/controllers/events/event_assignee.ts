import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

export const event_assignee = async (req: Request, res: Response) => {
    let id: string | undefined = "";
    const configJSON = req.query;
    if (configJSON.hasOwnProperty("id")) {
      id = String(configJSON["id"]);
    }
    console.log("Id: ", id);
    try {
      const entityManager = dbSourceConnection()[0];

      const assigned_users = await entityManager.query(`SELECT u.username from assign_event ae JOIN users u on u.id = ae.user_id WHERE $1 = ANY(ae.event_id);`, [id]);
      console.log("Assigned_Users: ", assigned_users);
      res.json(assigned_users);
    } catch (error) {
      console.error("Error in retrieving config list", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };
