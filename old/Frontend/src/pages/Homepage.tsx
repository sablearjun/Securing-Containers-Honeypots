import React, { FormEvent, useEffect, useLayoutEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import LoginForm from "../modules/Authentication/Login";
//import { TOKEN } from "../apis/base-urls";
import { LOGIN_GET_TOKEN_URL } from "../apis/urls";
import { ReactDOM } from "react";
import { HashRouter as Router, Route, Routes, Navigate} from "react-router-dom";
import { Link } from "react-router-dom";
import { doSignIn } from "../apis/requests";
import Loader from '../components/loader';
import Logo from '../assets/images/vajralogo.svg'

const HomePage = () => {
  let navigate = useNavigate();

  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const [validator, setValidator] = useState({
    username : false,
    password: false
  })
  const [isloading, setIsLoading] = useState(false);

  useEffect(() => {
    // console.log(isloading)
    if(isloading){
      setTimeout(()=> {
        callSigninAPI()
      }, 3000)
    }
  }, [isloading])
  
  useLayoutEffect(() => {
    try {
      const authenticatedUser = sessionStorage.getItem("authenticatedUser");
      if (authenticatedUser) {
        const { isAuthenticated } = JSON.parse(authenticatedUser);

        if (isAuthenticated) {
          window.location.href = "/node";
        }
      }
    } catch (e) {}
  }, [navigate]);

  const callSigninAPI = () => {
    // setIsLoading(true)
    const trimmedUserName = username.trim().toLowerCase();
    const trimmedPassword = password.trim().toLowerCase();
    let req = {
      "username": trimmedUserName,
      "password": trimmedPassword
    }
    doSignIn(req)
      .then( res=> {
        // console.log("Login response : ", res);
        setIsLoading(false);
        if(res.non_field_errors){
          setError(res.non_field_errors[0])
        }else{
          let x = res.token;
          localStorage.setItem('Token',x)
          localStorage.setItem('Renew_access_token',res.renew_access_token)
          localStorage.setItem('Username',res.user_data['username'])
          // console.log(res.token)
          // console.log(res.user_data['username'])
          // console.log(res.type)
          if(x)
          {
            // redirect
            sessionStorage.setItem(
              "authenticatedUser",
              JSON.stringify({ isAuthenticated: true, username: username.trim() })
            );
            navigate("/node");
          }
          else{
            // console.log('Login: Try again')
          }
        }
      })
      .catch(err => {
        // console.log(err)
        setIsLoading(false);
        setError(err)
      })
  }

  const onSubmit = (e: FormEvent<HTMLFormElement>) => {
    e.preventDefault();
    if(username === '' || password === ''){
      setIsLoading(false)
      if(username === '' && password === ''){
        setValidator({...validator, username:true, password:true})
        setError('Username and Password are required')
      }else if(username === ''){
        setValidator({...validator, username: true, password:false})
        setError('Username is required')
      }else if(password === ''){
        setValidator({...validator, username:false, password : true})
        setError('Password is required')
      }
      return
    }else{
      setValidator({...validator, username:false, password:false})
    }
    setIsLoading(true)
  }

  return (
      (isloading) ? 
        <Loader />
      :
      <div className="vh-100 bg-login">
        <div className="logo-home">
          <img src={Logo} />
        </div>
        <div className="d-flex flex-wrap justify-content-center align-items-center gap-8 h-100">
          <div className="d-flex flex-column justify-content-center h-100 mx-7">
            <h1 className="fw-bold text-primary outfit fs-1">
              Stay one step ahead <br /> of #CyberAttacks.
            </h1>
            <div className="d-flex justify-content-start gap-7 my-7">
              <div className="d-flex flex-column justify-content-between align-items-start">
                <i className="fi fi-rr-shield-interrogation fs-3 text-muted"></i>
                <p className="fw-mid text-muted fs-7 lh-sm mt-1">
                  Investigate <br /> the Attacks
                </p>
              </div>
              <div className="d-flex flex-column justify-content-between align-items-start">
                <i className="fi fi-rr-vector fs-3 text-muted"></i>
                <p className="fw-mid text-muted fs-7 lh-sm mt-1">
                  Powerful & <br /> Rich toolset
                </p>
              </div>
              <div className="d-flex flex-column justify-content-between align-items-start">
                <i className="fi fi-rr-time-quarter-to  fs-3 text-muted"></i>
                <p className="fw-mid text-muted fs-7 lh-sm mt-1">
                  Quickly <br /> Respond
                </p>
              </div>
              <div className="d-flex flex-column justify-content-between align-items-start">
                <i className="fi fi-rr-lock  fs-3 text-muted"></i>
                <p className="fw-mid text-muted fs-7 lh-sm mt-1">
                  Secured <br /> End-points
                </p>
              </div>
            </div>
          </div>
          <div className="d-flex flex-column justify-content-center align-items-center">
            <div className="card card-body p-7 rounded-3">
              <h5 className="fw-light text-primary outfit fs-4 mb-6">
                Welcome to Vajra
              </h5>
              <LoginForm
                username={username}
                setUsername={setUsername}
                password={password}
                setPassword={setPassword}
                onSubmit={onSubmit}
                validator={validator}
              />
              <p className="mt-2">
                Don't have an account? <Link to="/register">Register</Link>
              </p>
              {
                error !== '' ? 
                <p className="text-danger"><strong>Error</strong> : {error}</p>
                : ''
              }
              
            </div>
            <h6 className="fw-light text-muted my-5">
              Secure Systems with Vajra EDR
            </h6>
          </div>
        </div>
      </div>
  );
};

export default HomePage;
