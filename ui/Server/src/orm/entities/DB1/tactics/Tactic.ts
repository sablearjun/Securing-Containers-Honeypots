import { Entity, PrimaryGeneratedColumn, Column } from 'typeorm';

@Entity('tactic')
export class Tactic {
  @PrimaryGeneratedColumn()
  tactic_id: number;

  @Column({
    length: 50,
  })
  tactic_name: string;

  @Column({
    length: 200,
  })
  created_by: string;

  toString(): string {
    return this.tactic_name;
  }
}
