import React, { FC, FormEvent } from "react";
import { Component } from "react";
import { Link } from "react-router-dom"

interface RegisterFormProps {
  username: string;
  setUsername: (username: string) => void;
  password: string;
  setPassword: (password: string) => void;
  email:string;
  setEmail:(email:string)=>void;
  firstname:string;
  setFirstname:(firstname:string)=>void;
  lastname:string;
  setLastname:(lastname:string)=>void;
  onSubmit: (e: FormEvent<HTMLFormElement>) => void;
  validator: any
}
const RegisterForm: FC<RegisterFormProps> = (props: RegisterFormProps) => {
//   state = { username: "" ,
// email:'',
// password:'',
// first_name:'',
// last_name:''

// };
  const { 
    username, setPassword, password, setUsername,
    email,setEmail,firstname,setFirstname,lastname,setLastname, 
    onSubmit, validator } = props;

  return (
    <form onSubmit={onSubmit}>
      <div className="form-floating mb-4">
        <input
          type="text"
          className={`form-control ${validator.username ? 'border-danger' : ''}`}
          id="floatingInput"
          placeholder="username"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
        />
        <label htmlFor="floatingInput">Username</label>
      </div>
      <div className="form-floating mb-3">
        <input
          type="password"
          className={`form-control ${validator.password ? 'border-danger' : ''}`}
          id="floatingPassword"
          placeholder="Password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
        />
        <label htmlFor="floatingEmail">Password</label>
      </div>
      <div className="form-floating mb-3">
        <input
          type="email"
          className={`form-control ${validator.email ? 'border-danger' : ''}`}
          id="floatingEmail"
          placeholder="email"
          value={email}
          onChange={(e) => setEmail(e.target.value)}
        />
        <label htmlFor="floatingEmail">Email</label>
      </div>
      <div className="form-floating mb-3">
        <input
          type="firstname"
          className={`form-control ${validator.firstname ? 'border-danger' : ''}`}
          id="floatingFirstName"
          placeholder="FirstName"
          value={firstname}
          onChange={(e) => setFirstname(e.target.value)}
        />
        <label htmlFor="floatingFirstName">FirstName</label>
      </div>
      <div className="form-floating mb-3">
        <input
          type="lastname"
          className={`form-control ${validator.lastname ? 'border-danger' : ''}`}
          id="floatingLastName"
          placeholder="LastName"
          value={lastname}
          onChange={(e) => setLastname(e.target.value)}
        />
        <label htmlFor="floatingLastName">LastName</label>
      </div>
      <hr />
      <button type="submit" className="btn btn-primary btn-lg w-100" >
        Register
      </button>
      
    </form>
  );
};

export default RegisterForm;
