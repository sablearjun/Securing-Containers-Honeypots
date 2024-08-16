import { Role } from '../orm/entities/DB1/users/types';

export type JwtPayload = {
  id: number;
  name: string;
  email: string;
  role: Role;
  created_at: Date;
};
