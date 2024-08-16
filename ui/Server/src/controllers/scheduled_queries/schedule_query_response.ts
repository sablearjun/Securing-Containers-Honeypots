import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

export const schedule_query_response = async (req: Request, res: Response) => {
  const bodyJSON = req.body;
  let configIds: Array<String> | undefined = [];

  if(bodyJSON.hasOwnProperty("config_id_list")){
    configIds = String(bodyJSON['config_id_list']).split(',');
  }
  try {
    const entityManager = dbSourceConnection()[1];

    if (configIds.length > 0) {
      const queryResultMap: Record<string, string> = {};
      const combinedResult : any =[];
      let row;

      await Promise.all(
        configIds.map(async (id: string) => {
          try {
            const queryText =
              "SELECT host_identifier, result FROM query_result WHERE query_name = $1";
            const result = await entityManager.query(queryText, [id]);

            if (result.length > 0) {

              const result_hostname = await entityManager.query(`SELECT hostname from nodes where host_identifier = $1`,[result[0].host_identifier]);
              const queryResult :any= JSON.parse(result[0].result);
              const nodeIdentity:any = {hostname : result_hostname[0].hostname, host_identifier: result[0].host_identifier};

              for(let i=0; i<queryResult.length; i++){
                row = queryResult[i];
                queryResult[i] = {...nodeIdentity, ...row};
              }

              queryResultMap[result[0].host_identifier] = JSON.stringify(queryResult);
            }
          } catch (error) {
            console.error("Error in retrieving query response", error);
            res.status(500).json({ message: "Internal Server Error" });
          }
        })
      );
      // console.log("\n\n Response: ", queryResultMap);
      if(configIds.length > 1){
        // Initialize an empty array to store the combined values
        let combinedArray: any[] = [];

        // Iterate through the keys of the original object
        for (const key in queryResultMap) {
          if (queryResultMap.hasOwnProperty(key)) {
            // Parse the string as JSON and concatenate the arrays
            combinedArray = combinedArray.concat(JSON.parse(queryResultMap[key]));
          }
        }

        // Add the combined array to the original object with a new key "c"
        queryResultMap['combined'] = JSON.stringify(combinedArray);
      }
      // console.log("combined: ", queryResultMap);
      res.status(200).json(queryResultMap);
    } else {
      res
        .status(400)
        .json({ message: "Bad Request with incomplete request body" });
    }
  } catch (error) {
    console.error("Error in retrieving query response", error);
    res.status(500).json({ message: "Internal Server Error" });
  }
};