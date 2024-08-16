import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

export const blog_delete = async (req: Request, res: Response) => {
    const id = req.body.id;
    try {
      if (!id) return res.status(400).json({ message: "Bad request" });

      const entityManager = dbSourceConnection()[0];
      const result = await entityManager.query(
        `DELETE FROM blogs where id = $1`,
        [BigInt(id)]
      );

      if(result[1])
        return res.json({ status: "Success", message : `Deletion of blog with id ${id} successful`});
      else
        return res.json({ status: "Fail", message : `Blog with id ${id} not found. Deletion not successful`});
    } catch (error) {
      console.error("Error in deleting the blog", error);
      return res.status(500).json({ message: "Internal Server Error" });
    }
  };