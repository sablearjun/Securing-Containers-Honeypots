/* eslint-disable jsx-a11y/anchor-is-valid */
import logo from "../../assets/images/vajralogo.svg";
import { NavLink, useLocation } from "react-router-dom";
import { LOGOUT_DELETE_TOKEN_URL } from "../../apis/urls";
import { useState } from "react";

const Navbar = () => {
  const location = useLocation();
  let name=localStorage.getItem('Username');
  const [isProfileClick, setIsProfileClick] = useState(false)

  return (
    <div className="hstack gap-5 px-3 align-middle">
      <div className="me-6 mb-2">
        <img alt="Vajra UI Logo" src={logo} />
      </div>
      <div>
        <nav className={`nav fs-5`}>
          <NavLink
            className={`nav-link ${
              location.pathname === "/" ? "active" : ""
            }`}
            aria-current="page"
            to={"/"}
          >
            <i className="fa fa-server me-2 align-middle" aria-hidden="true"></i>
            Container List
          </NavLink>
          {/* <NavLink
            className={`nav-link ${
              location.pathname === "/geoip" ? "active" : ""
            }`}
            aria-current="page"
            to={"/geoip"}
          >
            <i className="fa fa-area-chart me-2 align-middle" aria-hidden="true"></i>
            GeoIP Map
          </NavLink> */}
          <NavLink
            className={`nav-link ${
              location.pathname === "/alerts" ? "active" : ""
            }`}
            aria-current="page"
            to={"/alerts"}
          >
            <i className="fa fa-exclamation-triangle me-2 align-middle" aria-hidden="true"></i>
            Alerts
          </NavLink>
        </nav>
      </div>
      <div>
      </div>
    </div>
  );
};

export default Navbar;
