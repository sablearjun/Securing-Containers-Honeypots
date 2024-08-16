import { Request, Response, NextFunction } from 'express';
import { tokenDecoder } from 'utils/createJwtToken';
import { dbSourceConnection } from 'index';

export const blog_update = async (req: Request, res: Response) => {
  const blogBody = req.body;
//   const authHeader = req.get('Authorization');
//   const jwtPayload = tokenDecoder(authHeader);
//   const blogCreator = jwtPayload.name;

  const entityManager = dbSourceConnection()[0];
  let blogId : string | undefined = "";
  let blogName: string | undefined = "";
  let blogContent:string | undefined = "";
  let blogImage: string | undefined = "";
  let blogLinks: string | undefined ="";

  if (blogBody.hasOwnProperty("name")) {
    blogName = String(blogBody["name"]);
  }
  if (blogBody.hasOwnProperty("content")) {
    blogContent = String(blogBody["content"]);
  }
  if (blogBody.hasOwnProperty("image")) {
    blogImage = String(blogBody["image"]);
  }
  if (blogBody.hasOwnProperty("blog_links")) {
    blogLinks = String(blogBody["blog_links"]);
  }
  if (blogBody.hasOwnProperty("id")) {
    blogId = String(blogBody["id"]);
  }

  try {
    if (!blogId || !blogName || !blogContent ) return res.status(400).json({ status: "Fail", message: "Bad request" });
    const blogModifiedAt = new Date();

    const result = await entityManager.query(
      `UPDATE blogs set name=$1, content=$2, image=$3, updated_at=$4, links=$5 where id = $6;`,
      [blogName, blogContent, blogImage, blogModifiedAt, blogLinks, blogId ]
    );

    if (result[1]) return res.json({ status: "Success", message: `Updation of blog with name ${blogName} successful` });
    else return res.json({ status: "Fail", message: `Blog with name ${blogName} not found. Updation not successful` });
  } catch (error) {
    console.error("Error in adding the blog", error);
    return res.status(500).json({ message: "Internal Server Error" });
  }
};
