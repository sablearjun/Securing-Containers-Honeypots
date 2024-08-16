import { Entity, Column, PrimaryGeneratedColumn } from 'typeorm';

@Entity('renew_access_token')
export class RenewAccessToken {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({
    length: 200,
    nullable: false,
  })
  user_name: string;

  @Column({
    length: 300,
    nullable: true,
  })
  renew_access_token: string;

  @Column({
    length: 200,
    nullable: true,
  })
  salt: string;

  @Column({ type: 'timestamp', default: () => 'CURRENT_TIMESTAMP' })
  created_at: Date;

  @Column({ type: 'timestamp', default: () => 'CURRENT_TIMESTAMP' })
  updated_at: Date;
}
