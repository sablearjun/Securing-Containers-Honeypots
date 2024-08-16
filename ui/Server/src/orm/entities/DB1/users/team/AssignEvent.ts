import { Entity, PrimaryGeneratedColumn, Column } from 'typeorm';

@Entity('assign_event')
export class AssignEvent {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({ type: 'bigint'})
  user_id: number;

  @Column({ type: 'bigint', array: true, default: '{}' }) // 'integer' or 'text', use an appropriate type
  event_id: number[];
}
