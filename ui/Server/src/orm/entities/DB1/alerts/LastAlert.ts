import { Entity, PrimaryGeneratedColumn, Column } from 'typeorm';

@Entity('last_alert')
export class LastAlert {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({ type: 'bigint' })
  alertId: number;

  @Column({ type: 'bigint' })
  unixtime: number;

}
