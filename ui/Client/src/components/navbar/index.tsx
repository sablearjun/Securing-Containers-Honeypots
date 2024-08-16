/* eslint-disable jsx-a11y/anchor-is-valid */
import logo from "../../assets/images/vajralogo.svg";
import { Link, useLocation } from "react-router-dom";
import {
  EVENTS_LIST_URL,
  HISTORIC_DATA_URL,
  LOGOUT_DELETE_TOKEN_URL,
} from "../../apis/urls";
import { memo, useEffect, useReducer, useState } from "react";
import Badge from "@mui/material/Badge";
import GppMaybeIcon from "@mui/icons-material/GppMaybe";
import { INITIAL_STATE, reducerFunc } from "../../apis/requests/reducer";
import { PollingReducer } from "../../apis/requests";
import AlarmSound from "../../assets/music/emergency_alarm.mp3";
import useSound from "use-sound";
import Container from "react-bootstrap/Container";
import Nav from "react-bootstrap/Nav";
import Navbar from "react-bootstrap/Navbar";
import { logout } from "../../helpers";

interface NavBarParams {
  notification: number;
  setNotification: Function;
}

const NavbarComponent: React.FC<NavBarParams> = memo((params: NavBarParams) => {
  const location = useLocation();
  const [isProfileClick, setIsProfileClick] = useState(false);
  const [rawData, setRawData] = useReducer(reducerFunc, INITIAL_STATE);
  const [isPlaying, setIsPlaying] = useState(false);
  // const [play, { stop }] = useSound(AlarmSound);

  PollingReducer(HISTORIC_DATA_URL,{state:rawData, setter:setRawData}, 5000, 1, "POST", {"query":"select * from alerts order by id desc limit 100"}, true, params.setNotification,"id")
  // const toggleAudio = () => {
  //   if (isPlaying) {
  //     stop();
  //   } else {
  //     play();
  //   }
  //   setIsPlaying(!isPlaying);
  // };

  // const stopAudio = () =>{
  //   stop();
  //   setIsPlaying(false);
  // }

  // useEffect(()=>{
  //   console.log(params?.notification);
  //   if(params?.notification!=0){
  //     play();
  //     setIsPlaying(true);
  //   }else{
  //     stop();
  //     setIsPlaying(false);
  //   }
  // }, [params])

  /* Event source Implementation alert notification */

  // useEffect(() => {
  //   const eventSource = new EventSource(EVENTS_LIST_URL+'sse/'); // Adjust the URL to match our SSE route
  //   console.log(eventSource);
  //   eventSource.onmessage=(event)=>{
  //       const eventData = JSON.parse(event.data);
  //       console.log(eventData);
  //       // params?.setNotification(eventData.events.length);
  //   }
  //   return () => {
  //     eventSource.close();
  //   };
  // }, []);

  return (
    <Navbar collapseOnSelect expand="lg" className="bg-body-tertiary p-0">
      <Container fluid className="p-0">
        <Navbar.Brand href="/" className="ms-5">
          <img alt="Vajra UI Logo" src={logo} />
        </Navbar.Brand>
        <Navbar.Toggle aria-controls="responsive-navbar-nav" />
        <Navbar.Collapse id="responsive-navbar-nav" className="p-0">
          <Nav className="ms-5">
            <Nav.Link as={Link}
              className={`nav-link px-5 fs-5 ${
                location.pathname === "/node" ? "active text-primary" : ""
              }`}
              aria-current="page"
              to={"/node"}
            >
              <i
                className="fa fa-server me-2 align-middle"
                aria-hidden="true"
              ></i>
              Nodes
            </Nav.Link>
            {/* <Nav.Link as={Link}
              className={`nav-link px-5 fs-5 ${
                location.pathname === "/networkdevices" ? "active text-primary" : ""
              }`}
              aria-current="page"
              to={"/networkdevices"}
            >
              <i
                className="fa fa-wifi me-2 align-middle"
                aria-hidden="true"
              ></i>
              Network Devices
            </Nav.Link> */}
            <Nav.Link as={Link}
              className={`nav-link px-5 fs-5 ${
                location.pathname === "/alerts" ? "active text-primary" : ""
              }`}
              aria-current="page"
              to={"/alerts"}
            >
              <Badge color="secondary" badgeContent={params?.notification}>
                <div className="pe-2">
                  <i
                    className="fa fa-bullhorn me-2 align-middle"
                    aria-hidden="true"
                  ></i>
                  Alerts
                </div>
              </Badge>
            </Nav.Link>
            {/* <button className="btn btn-sm btn-primary" onClick={()=>{toggleAudio()}}>
            {isPlaying ? "Pause" : "Play"}
          </button> */}
            {/* { params?.notification ? <button className="btn btn-sm btn-primary" onClick={()=>{stopAudio()}}>
            <i className="fa fa-bell-slash me-2 align-middle"></i>
          </button>:<></>} */}
            <Nav.Link as={Link}
              className={`nav-link px-5 fs-5 ${
                location.pathname === "/threat-hunting" ? "active text-primary" : ""
              }`}
              aria-current="page"
              to={"/threat-hunting"}
            >
              <i
                className="fa fa-shield me-2 align-middle"
                aria-hidden="true"
              ></i>
              Threat hunting
            </Nav.Link>
            <Nav.Link as={Link}
              className={`nav-link px-5 fs-5 ${
                location.pathname === "/dashboard" ? "active text-primary" : ""
              }`}
              aria-current="page"
              to={"/dashboard"}
            >
              <i
                className="fa fa-area-chart me-2 align-middle"
                aria-hidden="true"
              ></i>
              Dashboard
            </Nav.Link>
            <Nav.Link as={Link}
              className={`nav-link px-5 fs-5 ${
                location.pathname === "/configuration" ? "active text-primary" : ""
              }`}
              aria-current="page"
              to={"/configuration"}
            >
              <i
                className="fa fa-cogs me-2 align-middle"
                aria-hidden="true"
              ></i>
              Configuration
            </Nav.Link>
            <Nav.Link as={Link}
              className={`nav-link px-5 fs-5 ${
                location.pathname === "/rules" ? "active text-primary" : ""
              }`}
              aria-current="page"
              to={"/rules"}
            >
              <i
                className="fa fa-list-alt me-2 align-middle"
                aria-hidden="true"
              ></i>
              Rules
            </Nav.Link>
            {/* <p className="nav-link disabled" aria-disabled>
            <i className="fi fi-rr-chart-connected me-2 align-middle"></i>
            Queries
          </p> */}
            {/* <p className="nav-link disabled" aria-disabled> */}
            {/* <i className="fi fi-rr-settings me-2 align-middle"></i> */}
            {/* <i className="fa fa-cogs me-2 align-middle" aria-hidden="true"></i> */}

            {/* Settings */}
            {/* Node Configuration */}
            {/* </p> */}
          </Nav>

          <Nav className="ms-md-auto me-md-0 ms-5">
            {/* Split Dropdown */}
            <div className="btn-group btn-group-sm">
              <Nav.Link as={Link}className="px-5 fs-5" to={"/settings"} style={{ textDecoration: "none" }}>
                <button
                  type="button"
                  className="btn btn-link btn-outline-secondary btn-sm"
                >
                  <i className="fa fa-wrench me-2 align-middle"></i>
                  Settings
                </button>
              </Nav.Link>
              {/* <button
            type="button"
            className="btn btn-link btn-outline-secondary btn-sm"
          >
            <i className="fi fi-rr-bell-ring me-1 align-middle"></i>Emergency
          </button>
          <button
            type="button"
            className="btn btn-link btn-outline-secondary btn-sm dropdown-toggle dropdown-toggle-split"
            data-bs-toggle="dropdown"
            aria-expanded="false"
          >
            <span className="visually-hidden">Toggle Dropdown</span>
          </button>
          <ul className="dropdown-menu dropdown-menu-dark">
            <li>
              <a className="dropdown-item" href="#">
                Emergency 2
              </a>
            </li>
            <li>
              <a className="dropdown-item" href="#">
                Emergency 3
              </a>
            </li>
            <li>
              <a className="dropdown-item" href="#">
                Emergency 4
              </a>
            </li>
            <li>
              <hr className="dropdown-divider" />
            </li>
            <li>
              <a className="dropdown-item" href="#">
                Call Entire Team
              </a>
            </li>
          </ul> */}
            </div>
            {/* <div className="btn-group btn-group-sm ms-2">
          <Nav.Link to={"/alerts"} >
            <Badge color="primary" badgeContent={params?.notification}>
              <GppMaybeIcon color="error"/>
            </Badge>
          </Nav.Link>
        </div> */}
          </Nav>
          <Nav className="ms-5">
            <div className="dropdown">
              <a
                href="#"
                className="d-flex align-items-center justify-content-start fw-semi btn btn-link text-dark btn-sm px-5 fs-7"
                id="dropdownUser3"
                data-bs-toggle="dropdown"
                aria-expanded="false"
                onClick={() => setIsProfileClick(isProfileClick ? false : true)}
              >
                Hi {localStorage.getItem("Username")}
                <img
                  src="https://github.com/mdo.png"
                  alt="mdo"
                  width="28"
                  height="28"
                  className="ms-3 rounded-circle"
                />
              </a>
              <ul
                className={`dropdown-menu dropdown-menu-dark text-small shadow ${
                  isProfileClick ? "show" : ""
                } `}
                aria-labelledby="dropdownUser3"
              >
                {/* <li>
              <a className="dropdown-item" href="#">
                <i className="fi fi-rr-apps-add me-2"></i>New Threat Hunt
              </a>
            </li>
            <li>
              <a className="dropdown-item" href="#">
                <i className="fi fi-rr-shield-exclamation me-2"></i>New Alert
              </a>
            </li>
            <li>
              <hr className="dropdown-divider" />
            </li>
            <li>
              <a className="dropdown-item" href="#">
                <i className="fi fi-rr-settings me-2"></i>Settings
              </a>
            </li>
            <li>
              <a className="dropdown-item" href="#">
                <i className="fi fi-rr-portrait me-2"></i>My Profile
              </a>
            </li>
            <li>
              <hr className="dropdown-divider" />
            </li> */}
                <li>
                  <button
                    onClick={() => 
                      logout()
                    }
                    className="dropdown-item"
                  >
                    <i className="fa fa-sign-out me-2"></i>Sign out
                  </button>
                </li>
              </ul>
            </div>
          </Nav>
        </Navbar.Collapse>
      </Container>
    </Navbar>
  );
});

export default NavbarComponent;
