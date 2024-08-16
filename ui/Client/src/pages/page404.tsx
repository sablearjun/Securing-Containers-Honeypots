import React from "react";
import Logo from "../assets/images/vajralogo.svg";

const Page404 = () => {
  return (
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
              <i className="fa fa-microchip fs-3 text-muted"></i>
              <p className="fw-mid text-muted fs-7 lh-sm mt-1">
                Investigate <br /> the Attacks
              </p>
            </div>
            <div className="d-flex flex-column justify-content-between align-items-start">
              <i className="fa fa-terminal fs-3 text-muted"></i>
              <p className="fw-mid text-muted fs-7 lh-sm mt-1">
                Powerful & <br /> Rich toolset
              </p>
            </div>
            <div className="d-flex flex-column justify-content-between align-items-start">
              <i className="fa fa-rocket  fs-3 text-muted"></i>
              <p className="fw-mid text-muted fs-7 lh-sm mt-1">
                Quickly <br /> Respond
              </p>
            </div>
            <div className="d-flex flex-column justify-content-between align-items-start">
              <i className="fa fa-shield  fs-3 text-muted"></i>
              <p className="fw-mid text-muted fs-7 lh-sm mt-1">
                Secured <br /> End-points
              </p>
            </div>
          </div>
        </div>
        <div className="d-flex flex-column justify-content-center align-items-center">
          <div className="card card-body p-7 rounded-3">
            <h5 className="fw-light outfit fs-4 mb-6">
            The page you seek doesn't exist.
            </h5>
            <h1 className="fs-1 text-primary outfit">404</h1>
            <div>
                <a href='/'>Click here to get to main site</a>
            </div>
          </div>
          <h6 className="fw-light text-muted my-5">
            Secure Systems with Vajra EDR
          </h6>
        </div>
      </div>
    </div>
  );
};

export default Page404;
