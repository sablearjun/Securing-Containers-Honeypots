import { Entity, Column, PrimaryGeneratedColumn, ManyToOne, JoinColumn } from 'typeorm';
import { Tactic } from './Tactic';

@Entity('technique')
export class Technique {
  @PrimaryGeneratedColumn()
  id: number;

  @ManyToOne(() => Tactic, { onDelete: 'CASCADE' })
  @JoinColumn({ name: 'tactic' })
  tactic: Tactic;

  @Column({
    length: 200,
    nullable: false,
  })
  technique_name: string;

  @Column({
    length: 200,
    nullable: false,
  })
  created_by: string;

  toString(): string {
    return this.technique_name;
  }
}
