import { Entity, Column, PrimaryGeneratedColumn, ManyToOne } from 'typeorm';
import { Tactic } from './Tactic';
import { Technique } from './Technique';
import { SubTechnique } from './SubTechnique';

@Entity('threat')
export class Threat {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({
    length: 200,
    nullable: false,
  })
  threat_name: string;

  @Column({
    length: 500,
    nullable: true,
  })
  description: string;

  @ManyToOne(() => Tactic, { onDelete: 'CASCADE' })
  tactic: Tactic;

  @ManyToOne(() => Technique, { onDelete: 'CASCADE' })
  technique: Technique;

  @ManyToOne(() => SubTechnique, { onDelete: 'CASCADE', nullable: true })
  sub_technique: SubTechnique;

  @Column({
    length: 200,
    nullable: false,
  })
  severity: string;

  @Column({
    length: 500,
    nullable: true,
  })
  query: string;

  @Column({ type: 'timestamp', default: () => 'CURRENT_TIMESTAMP' })
  created_at: Date;

  toString(): string {
    return this.threat_name;
  }
}
