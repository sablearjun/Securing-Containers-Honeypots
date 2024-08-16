import 'dotenv/config';
import 'reflect-metadata';
import fs from 'fs';
import path from 'path';

import bodyParser from 'body-parser';
import cors from 'cors';
import express from 'express';
import helmet from 'helmet';
import morgan from 'morgan';

import './utils/response/customSuccess';
import { errorHandler } from './middleware/errorHandler';
import { getLanguage } from './middleware/getLanguage';
import { dbCreateConnection } from './orm/dbCreateConnection';
import routes from './routes';
import config1 from './orm/config/ormconfig';
import config2 from './orm/config/ormconfig1';

import { DataSource } from 'typeorm';

let dataSource: DataSource[];
export const app = express();
app.use(cors());
app.use(helmet());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(getLanguage);

try {
  const accessLogStream = fs.createWriteStream(path.join(__dirname, '../log/access.log'), {
    flags: 'a',
  });
  app.use(morgan('combined', { stream: accessLogStream }));
} catch (err) {
  console.log(err);
}
app.use(morgan('combined'));

app.use('/api/', routes);

app.use(errorHandler);

const port = process.env.PORT || 4000;
app.listen(port, () => {
  console.log(`Server running on port ${port}`);
});

(async () => {
  dataSource = await dbCreateConnection(config1, config2);
})();

export const dbSourceConnection=()=>{
  return dataSource;
}