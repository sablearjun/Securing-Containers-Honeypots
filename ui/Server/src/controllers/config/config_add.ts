import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const config_add = async (req: Request, res: Response) => {
  const configBody = req.body;
  console.log(configBody);
  
  const entityManager = dbSourceConnection()[1];

  let query: string | undefined = "";
  let os_name:string | undefined = "";
  let os_arch: string | undefined = "";
  let kernel_version: string | undefined = "";
  let query_name: string | undefined = "";
  if (configBody.hasOwnProperty("query_name")) {
    query_name = String(configBody["query_name"]);
  }
  if (configBody.hasOwnProperty("query")) {
    query = JSON.stringify(configBody["query"]);
    console.log(query);
  }
  if (configBody.hasOwnProperty("os_name")) {
    os_name = JSON.stringify(configBody["os_name"]);
  }
  if (configBody.hasOwnProperty("os_arch")) {
    os_arch = String(configBody["os_arch"]);
  }
  if (configBody.hasOwnProperty("kernel_version")) {
    kernel_version = String(configBody["kernel_version"]);
  }

  try {
  //  console.log('query',query ,'os_name',os_name , 'query_name',query_name);
    if (query==='' || query_name==='' || os_name === '') return res.status(400).json({ status: "Fail", message: "Bad request" });

    const result = await entityManager.query(
      `INSERT into config_queries (query, query_name, os_name, os_arch, kernel_version) values ($1, $2, $3, $4, $5)`,
      [query, query_name, os_name, os_arch, kernel_version]
    );

    return res.json({ status: "Success", message : `Addition of config query with query_name ${query_name} successful`});

  } catch (error) {
    console.error("Error in adding to config", error);
    return res.status(500).json({ message: "Internal Server Error" });
  }
};
