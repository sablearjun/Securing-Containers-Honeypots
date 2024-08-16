import { Entity, PrimaryGeneratedColumn, Column } from 'typeorm';

@Entity('alerts')
export class Alerts {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({ type: 'varchar' , length: 40})
  host_identifier: string;

  @Column({ type: 'text' })
  hostname: string;

  @Column({ type: 'bigint' })
  unixtime: number;

  @Column({ type: 'text' })
  event_code: string;

  @Column({ type: 'text' })
  metadata: string;

  @Column({ type: 'boolean' })
  is_alert: boolean;

  @Column({ type: 'boolean' })
  is_open: boolean;

  @Column({ type: 'text', nullable: true })
  remarks: string;

  @Column({ type: 'text' })
  context_data: string;

  @Column({ type: 'text' })
  severity: string;

  @Column({ type: 'text' })
  alert_type: string;

  @Column({ type: 'text' })
  technique: string;

  @Column({ type: 'text' })
  technique_id: string;

  @Column({ type: 'text' })
  link: string;

  @Column({ type: 'bigint' })
  pid: number;

  @Column({ type: 'bigint' })
  event_id: number;

  @Column({ type: 'varchar',
    length: 5,
  })
  action: string;

}
