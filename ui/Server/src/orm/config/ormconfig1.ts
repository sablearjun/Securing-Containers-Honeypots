import { DataSourceOptions } from 'typeorm';
import { SnakeNamingStrategy } from 'typeorm-naming-strategies';

const config1: DataSourceOptions = {
  type: 'postgres',
  name: 'default',
  host: process.env.PG_HOST1,
  port: Number(process.env.PG_PORT1),
  username: process.env.POSTGRES_USER1,
  password: process.env.POSTGRES_PASSWORD1,
  database: process.env.POSTGRES_DB1,
  synchronize: false,
  logging: false,
  entities: ['dist/orm/entities/DB2/**/*.js'],
  migrations: ['dist/orm/migrations/**/*.js'],
  subscribers: ['dist/orm/subscriber/**/*.js'],
  namingStrategy: new SnakeNamingStrategy(),
};

export = config1;
