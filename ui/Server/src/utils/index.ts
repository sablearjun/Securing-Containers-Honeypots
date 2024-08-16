import { User } from "orm/entities/DB1/users/User";
import { Role } from "orm/entities/DB1/users/types";


export const serialize_user_data = (user : User)=>{
    return {
        id: user.id,
        username: user.username,
        email: user.email,
        role: user.role as Role,
    }
}