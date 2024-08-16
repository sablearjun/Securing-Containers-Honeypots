import { Entity, PrimaryGeneratedColumn, Column } from 'typeorm';

@Entity('team')
export class Team {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({ type: 'varchar' , length: 200})
  name: string;

  @Column({ type: 'varchar', length: 200 })
  admin_ids: string;

  @Column({ type: 'varchar', length: 200 })
  member_ids: string;

  @Column({ type: 'varchar', length: 200 })
  permissions: string;
}
