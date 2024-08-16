import React, { FormEvent, useLayoutEffect } from "react";
import { useNavigate } from "react-router-dom";
import LoginForm from "../modules/home/Login";
//import { TOKEN } from "../apis/base-urls";
import { LOGIN_GET_TOKEN_URL, REGISTER_GET_TOKEN_URL } from "../apis/urls";
import { useState, useEffect } from "react";
//import { useRegisterUser } from "../hooks";
import { User_Info } from "../types";
import { ReactDOM } from "react";
import { Link } from "react-router-dom";
import {
  HashRouter as Router,
  Route,
  Routes,
  Navigate,
} from "react-router-dom";
import RegisterForm from "../modules/home/Register";
import { doSignUp } from "../apis/requests";
import Loader from "../components/loader";
import swal from 'sweetalert';

const HomePageRegister = () => {
  let navigate = useNavigate();

  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [email,setEmail ]=useState("");
  const [firstname,setFirstname ]=useState("");
  const [lastname,setLastname ]=useState("");
  const [error, setError] = useState("");
  const [isloading, setIsLoading] = useState(false);
  const [validator, setValidator] = useState({
    username : false,
    password: false,
    email: false,
    firstname: false,
    lastname: false
  })

  useEffect(() => {
    console.log(isloading)
    if(isloading){
      setTimeout(()=> {
        callSignupAPI()
      }, 3000)
    }
  }, [isloading])

  useLayoutEffect(() => {
    try {
      const authenticatedUser = sessionStorage.getItem("authenticatedUser");
      if (authenticatedUser) {
        const { isAuthenticated } = JSON.parse(authenticatedUser);

        if (isAuthenticated) {
          window.location.href = "/alerts";
        }
      }
    } catch (e) {}
  }, [navigate]);

  const onHandleChange = (name : any, status : boolean) => {
    setValidator(validator => ({...validator, [name] : status}))
  }

  const callSignupAPI = () => {
    // setIsLoading(true)
    let req = {
      username,
      password,
      email,
      "first_name":firstname,
      "last_name":lastname 
    }

    doSignUp(req)
    .then(res => {
      console.log("Registration response : ", res)
      setIsLoading(false);
      swal('User Registered Successfully !!!', 'Please login to continue...', 'success')
      .then(updated => updated ? navigate('/') : '')
    })
    .catch(err => {
      console.log("Registration error : ", err)
      setIsLoading(false);
      setError(err)
    })
  }

  const onSubmit = (e: FormEvent<HTMLFormElement>) => {
    e.preventDefault();
    if(username == '' || password == '' || email == '' || firstname == '' || lastname == ''){
      setIsLoading(false)
      if(username == '' && password == '' && email == '' && firstname == '' && lastname == ''){
        setValidator({...validator, username:true, password:true, email:true, firstname:true, lastname:true})
      }else{
        (username == '') ? onHandleChange('username', true) : onHandleChange('username', false);
        (password == '') ? onHandleChange('password', true) : onHandleChange('password', false);
        (email == '') ? onHandleChange('email', true) : onHandleChange('email', false);
        (firstname == '') ? onHandleChange('firstname', true) : onHandleChange('firstname', false);
        (lastname == '') ? onHandleChange('lastname', true) : onHandleChange('lastname', false)
      }
      setError('Please fill all the required fields')
      return
    }else{
      setValidator({...validator, username:false, password:false, email:false, firstname:false, lastname:false})
    }
    setIsLoading(true)
  };

  return (
    (isloading) ? 
      <Loader />
    :
    <div className="vh-100 bg-login">
      <div className="d-flex justify-content-center align-items-center gap-8 h-100">
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
            <RegisterForm
              username={username}
              setUsername={setUsername}
              password={password}
              setPassword={setPassword}
              email={email}
              setEmail={setEmail}
              firstname={firstname}
              setFirstname={setFirstname}
              lastname={lastname}
              setLastname={setLastname}
              onSubmit={onSubmit}
              validator={validator}
            />
            <p className="mt-2">
              Already have an account? <Link to="/">Login</Link>
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

export default HomePageRegister;
