import { Request, Response } from 'express';

import { dbSourceConnection } from 'index';

export const blog_list = async (req: Request, res: Response) => {
    let id: string | undefined = "";
    const blogJSON = req.query;
    if (blogJSON.hasOwnProperty("id")) {
      id = String(blogJSON["id"]);
    }
    try {
      const entityManager = dbSourceConnection()[0];

      let result;

      if (id == "") {
        result = await entityManager.query(
          "SELECT * FROM blogs order by id"
        );
      } else {
        result = await entityManager.query(
          `SELECT * FROM blogs where id = $1 order by id`,
          [Number(id)]
        );
      }

      res.customSuccess(200, "Blogs", result);
    } catch (error) {
      console.error("Error in retrieving blog list", error);
      res.status(500).json({ message: "Internal Server Error" });
    }
  };
