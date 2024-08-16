import { DataSourceOptions } from 'typeorm';
import { SnakeNamingStrategy } from 'typeorm-naming-strategies';

const configSeed: DataSourceOptions = {
  type: 'postgres',
  host: process.env.PG_HOST,
  port: Number(process.env.PG_PORT),
  username: process.env.POSTGRES_USER,
  password: process.env.POSTGRES_PASSWORD,
  database: process.env.POSTGRES_DB,
  synchronize: true,
  logging: false,
  entities: ['dist/orm/entities/DB1/**/*.js'],
  migrations: ['dist/orm/seeds/**/*.js'],
  namingStrategy: new SnakeNamingStrategy(),
};

export = configSeed;
