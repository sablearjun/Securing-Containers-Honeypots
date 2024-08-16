import React, { FC, FormEvent } from "react";
interface LoginFormProps {
  username: string;
  setUsername: (username: string) => void;

  password: string;
  setPassword: (password: string) => void;

  onSubmit: (e: FormEvent<HTMLFormElement>) => void;
}
const LoginForm: FC<LoginFormProps> = (props: LoginFormProps) => {
  const { username, setPassword, password, setUsername, onSubmit } = props;
  return (
    <form onSubmit={onSubmit}>
      <div className="form-floating mb-4">
        <input
          type="text"
          className="form-control"
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
          className="form-control"
          id="floatingPassword"
          placeholder="Password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
        />
        <label htmlFor="floatingPassword">Password</label>
      </div>
      <hr />
      <button type="submit" className="btn btn-primary btn-lg w-100">
        Sign in
      </button>
    </form>
  );
};

export default LoginForm;
