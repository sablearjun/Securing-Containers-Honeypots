import { Request, Response, NextFunction } from 'express';
import { tokenDecoder } from 'utils/createJwtToken';
import { dbSourceConnection } from 'index';

export const blog_add = async (req: Request, res: Response) => {
  const blogBody = req.body;
  const authHeader = req.get('Authorization');
  const jwtPayload = tokenDecoder(authHeader);
  const blogCreator = jwtPayload.name;

  const entityManager = dbSourceConnection()[0];

  let blogName: string | undefined = "";
  let blogContent:string | undefined = "";
  let blogImage: string | undefined = "";
  let blogLinks: string | undefined ="";

  if (blogBody.hasOwnProperty("name")) {
    blogName = String(blogBody["name"]);
  }
  if (blogBody.hasOwnProperty("content")) {
    blogContent = String(blogBody["content"]);
    // console.log(query);
  }
  if (blogBody.hasOwnProperty("image")) {
    blogImage = String(blogBody["image"]);
  }
  if (blogBody.hasOwnProperty("links")) {
    blogLinks = String(blogBody["links"]);
  }

  try {
    if (!blogName || !blogContent ) return res.status(400).json({ status: "Fail", message: "Bad request" });
    const blogCreatedAt = new Date();
    const blogUpdatedAt = blogCreatedAt;

    const result = await entityManager.query(
      `INSERT into blogs (name, content, image, creator, created_at, updated_at, links) values ($1, $2, $3, $4, $5, $6, $7)`,
      [blogName, blogContent, blogImage, blogCreator, blogCreatedAt, blogUpdatedAt, blogLinks ]
    );

    return res.json({ status: "Success", message : `Addition of blog with name ${blogName} successful`});

  } catch (error) {
    console.error("Error in adding the blog", error);
    return res.status(500).json({ message: "Internal Server Error" });
  }
};
