import { Entity, Column, PrimaryGeneratedColumn, ManyToOne, JoinColumn } from 'typeorm';
import { Tactic } from './Tactic';
import { Technique } from './Technique';

@Entity('sub_technique')
export class SubTechnique {
  @PrimaryGeneratedColumn()
  id: number;

  @ManyToOne(() => Tactic, { cascade: true, onDelete: 'CASCADE' })
  @JoinColumn({ name: 'tactic_id' })
  tactic: Tactic;

  @ManyToOne(() => Technique, { cascade: true, onDelete: 'CASCADE' })
  @JoinColumn({ name: 'technique_id' })
  technique_id: Technique;

  @Column({
    length: 200,
  })
  technique_name: string;

  @Column({
    length: 200,
  })
  created_by: string;

  toString(): string {
    return this.technique_name;
  }
}
