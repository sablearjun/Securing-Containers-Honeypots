import { Entity, PrimaryGeneratedColumn, Column, PrimaryColumn } from 'typeorm';

@Entity('nodes')
export class Nodes {
  @PrimaryGeneratedColumn()
  id: number;

  @Column()
  config_id: string;

  @PrimaryColumn()
  @Column({
    length: 40,
  })
  host_identifier: string;

  @Column()
  os_arch: string;

  @Column()
  os_build: string;

  @Column()
  os_major: string;

  @Column()
  os_minor: string;

  @Column()
  os_name: string;

  @Column()
  os_platform: string;

  @Column()
  osquery_version: string;

  @Column({
    length : 40,
    unique : true,
  })
  node_key: string;

  @Column()
  node_invalid: boolean;

  @Column()
  reenroll_allowed: boolean;

  @Column()
  hardware_vendor: string;

  @Column()
  hardware_model: string;

  @Column()
  hardware_version: string;

  @Column()
  enrolled_on: number;

  @Column()
  last_seen: number;

  @Column()
  hardware_cpu_logical_core: string;

  @Column()
  hardware_cpu_type: string;

  @Column()
  hardware_physical_memory: string;
}
