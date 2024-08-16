import { DataSource, DataSourceOptions, getConnectionManager } from 'typeorm';

export const dbCreateConnection = async (config1: DataSourceOptions, config2: DataSourceOptions): Promise<DataSource[] | null> => {
  try {

    const appDataSource1 = await new DataSource(config1).initialize();
    console.log(`Database connection 1 success. 'Database: '${config1.database}'`);
    const appDataSource2 = await new DataSource(config2).initialize();
    console.log(`Database connection 2 success. 'Database: '${config2.database}'`);
    return [appDataSource1, appDataSource2];
  } catch (err) {
    console.log(err);
  }
  return null;
};
