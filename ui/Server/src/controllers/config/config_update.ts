import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';
import { CustomError } from 'utils/response/custom-error/CustomError';




export const config_update = async (req: Request, res: Response) => {
  const configBody :string = req.body;
  const entityManager = dbSourceConnection()[1];
  
  let id: string | undefined = '';
  let query: string | undefined = '';
  let query_name: string | undefined = '';
  let os_name: string | undefined = '';
  let os_arch: string | undefined = '';
  let kernel_version: string | undefined = '';
  const isJason = JSON.parse(configBody);
  if (configBody.hasOwnProperty('id')) {
    id = String(configBody['id']);
  }

  if (configBody.hasOwnProperty('query_name')) {
    query_name = String(configBody['query_name']);
  }
  if (configBody.hasOwnProperty('query')) {
    query = JSON.stringify(configBody['query']);
    // console.log("Updated Query:", query);
  }
  if(configBody.hasOwnProperty('os_name')){
    os_name = String(configBody['os_name']);
  }
  if (configBody.hasOwnProperty("os_arch")) {
    os_arch = String(configBody["os_arch"]);

  }
  if (configBody.hasOwnProperty("kernel_version")) {
    kernel_version = String(configBody["kernel_version"]);
  }
  if (JSON.parse(configBody))
  {
    console.log({ status: "Fail", message: 'Unsupported Format' });
  }
// if(!query ||!id ||!query_name)
// {
//   res.status(400).json({status:'Fail',message:'Unsupported format'})
// }
//  if(req.body === '' || req.body === 'object')
//  {
//   return res.status(400).json({status:'Fail',message:'Unsupported format'})
//  }
  // if (JSON.parse(configBody)){
  try {
    //const parsedJSON = JSON.parse(configBody);
   
    

    // if ((typeof(req.body) === 'object') && (req.body!== null)) {
    //   console.log('Request body is a JSON object:', req.body);
    //   //return next();
    // } else {
    //   console.log('Request body is not JSON:', req.body);
    //   //return res.status(400).json({status:'Fail', message:'Invalid JSON data'});
    // }
    

    if (id === '' || query === '' || query_name === '' || os_name === '') 
    {
      
      return res.status(400).json({ status: "Fail", message: 'Unsupported Format' });}
    // if ((typeof(req.body) === 'object') && (req.body!== null) ) ;
    const result = await entityManager.query(`UPDATE config_queries set query = $2, query_name = $3, os_name = $4, os_arch = $5, kernel_version = $6 where id = $1`, [
      BigInt(id),
      query,
      query_name,
      os_name,
      os_arch,
      kernel_version
    ]);
    if (result[1]) return res.json({ status: "Success", message: `Updation of config query with id ${id} successful` });
    else return res.json({ status: "Fail", message: `Config query with id ${id} not found. Updation not successful` });
  } catch (error) {
    console.error("Error in adding to config", error);
    return res.status(400).json({ message: 'Internal Server Error' });
  // res.status(400).json({ message: 'Unsupported format' });
  return console.log('Error in updating config', error);
  }
// }
// else 
// {
//   console.log('invalid request');
// };

};
