import { Request, Response, NextFunction } from 'express';

import { dbSourceConnection } from 'index';

import { CustomError } from 'utils/response/custom-error/CustomError';

function transformToTitleCase(str: string): string {
  const words = str.split("_");
  const capitalizedWords = words.map((word, index) => {
      return word.charAt(0).toUpperCase() + word.slice(1).toLowerCase();
  });
  return capitalizedWords.join(" ");
}

export const tables_list = async (req: Request, res: Response) => {
  try {
    const entityManager = dbSourceConnection()[1];

    const tables = [
      "bpf_process_events",
      "bpf_socket_events",
      "bpf_file_events",
      "win_process_events",
      "win_socket_events",
      "win_file_events"
    ];
    const finalList = [];

    for (const table of tables) {
      // const queryColumns = `SELECT column_name, col_description($1, ordinal_position::int) from information_schema.columns where table_name=$1;`;
      //  THis query runs in psql shell but not via code.
      const queryColumns = `SELECT a.attname AS column_name, d.description AS column_description FROM pg_class c JOIN pg_attribute a ON c.oid = a.attrelid LEFT JOIN pg_description d ON a.attnum = d.objsubid AND c.oid = d.objoid WHERE c.relname = $1;`
      const result = await entityManager.query(queryColumns, [table]);
      const numColumns = result.length;
      const parentsList = [];
      let colName;
      for (let colNum = 0; colNum < numColumns; colNum++) {
        colName = result[colNum].column_name;
        if(colName ==='tableoid' || colName ==='cmax' || colName ==='cmin'||colName==='xmin'||colName==='xmax' ||colName==='ctid') continue;
        const obj = {
          column_name: colName,
          column_label:
            result[colNum].column_description ||
            transformToTitleCase(colName),
        };
        parentsList.push(obj);
      }

      const columnsList = {
        table_name: table,
        table_label: transformToTitleCase(table),
        columns: parentsList,
      };

      finalList.push(columnsList);
    }

    if (finalList.length <= 0) {
      res.status(200).json({
        status: "Fail",
        message: "No data available",
      });
    } else {
      res.status(200).json({
        data: finalList,
      });
    }
  } catch (error) {
    console.error("Database error:", error);
    res.status(500).json({
      status: "Fail",
      message: "Query to get list of tables failed due to database error.",
    });
  }
};