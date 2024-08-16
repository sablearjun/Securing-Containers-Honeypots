import { MigrationInterface, QueryRunner, DataSource } from 'typeorm';

import { Role } from '../entities/DB1/users/types';
import { User } from '../entities/DB1/users/User';
import config from 'orm/config/ormconfig';

export class SeedUsers1590519635401 implements MigrationInterface {
  public async up(queryRunner: QueryRunner): Promise<any> {
    let user = new User();
    const db = await new DataSource(config);
    const userRepository = db.getRepository(User);

    user.username = 'gaurishewale';
    user.name = 'Gauri Shewale';
    user.email = 'gaurishewale@gmail.com';
    user.password = 'admin';
    user.hashPassword();
    user.role = 'ADMINISTRATOR' as Role;
    await userRepository.save(user);

    user = new User();
    user.username = 'kni9ht';
    user.name = 'Yogesh Jadhav';
    user.email = 'yogeshjadhav@gmail.com';
    user.password = 'kni9ht';
    user.hashPassword();
    user.role = 'STANDARD' as Role;
    await userRepository.save(user);

    user = new User();
    user.username = 'sablearjun';
    user.name = 'Arjun Sable';
    user.email = 'arjun.sable@gmail.com';
    user.password = 'sablearjun';
    user.hashPassword();
    await userRepository.save(user);

    user = new User();
    user.username = 'Azeez7';
    user.name = 'Azeez Dandawala';
    user.email = 'azeezd@gmail.com';
    user.password = 'azeez7';
    user.hashPassword();
    await userRepository.save(user);

    user = new User();
    user.username = 'mkhanawal';
    user.name = 'Manjesh K Hanawal';
    user.email = 'mkhanawal@gmail.com';
    user.password = 'mkhanawal';
    user.hashPassword();
    await userRepository.save(user);
  }

  public async down(queryRunner: QueryRunner): Promise<any> {
    console.log('Not implemented');
  }
}
