import { Entity, Column, PrimaryGeneratedColumn } from 'typeorm';

@Entity('assign_event')
export class AssignEvent {
  @PrimaryGeneratedColumn({ type: 'bigint' })
  id: string;

  @Column({ type: 'bigint', nullable: false })
  user_id: string;

  @Column('bigint', { array: true, nullable: false })
  event_id: string[];

  toString(): string {
    return this.event_id.toString();
  }
}
