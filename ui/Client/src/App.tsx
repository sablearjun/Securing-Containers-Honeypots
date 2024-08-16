import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import ThreatHunting from "./pages/threat-hunting";
import Alerts from "./pages/alerts";
import AppContainer from "./components/app-container";
import HomePage from "./pages/homepage";
import LoginPage from "./pages/loginpage";
import HomePageRegister from "./pages/homepageRegister";
import ThreathuntingTactics from "./pages/threathunting-tactics";
import AlertSheetView from "./pages/alertSheetView";
import NodeSheetView from "./pages/nodeSheetView";
import Node from "./pages/node";
import Settings from "./pages/settings";
import Dashboard from "./pages/dashboard";
import Configuration from "./pages/configuration";
import { PythonRuleEngine } from "./modules/threat-hunting/pythonRule";
import ViewPythonRule from "./modules/threat-hunting/pythonRule/view";
import Rules from "./pages/rules";
import Yara from "./pages/rules/yara/yara";
import AddRule from "./pages/rules/rule-builder/addrules";
import ViewRules from "./pages/rules/rule-builder/viewrules";
import Navbar from "./components/navbar";
import Banner from "./components/Banner";
import Email from './pages/email';
import DataPurge from './pages/datapurge';
import { useReducer, useState } from "react";
import { HISTORIC_DATA_URL } from "./apis/urls";
import { PollingReducer } from "./apis/requests";
import { INITIAL_STATE, reducerFunc } from "./apis/requests/reducer";
import Ip from "./pages/rules/ip/ip";
import ViewBackendRules from "./pages/rules/backend-rules/backend-rules";
import DownloadAgent from "./pages/downloadAgent";
import Page404 from "./pages/page404";
import Blog from "./components/blog/Blog";
import HomePageNavbar from "./components/navbar/homepageNavbar";
import BlogList from "./components/blog/BlogList";
import NetworkDevicesTable from "./modules/network-devices/device-tables";
import NetworkDeviceSheetView from "./pages/networkDeviceView";

function App() {
  const [notification, setNotification] = useState<number>(0)
  // localStorage.setItem('Notification','0')

  const Nav=()=>{
    return <Navbar notification={notification} setNotification={setNotification}/>
  }

  return (
    <Router>
      <Routes>
        <Route
          path="/"
          element={
            <div>
              <HomePage />
            </div>
          }
        />
        <Route
          path="/login"
          element={
            <div>
              <LoginPage />
            </div>
          }
        />
        <Route
          path="/dashboard"
          element={
            <AppContainer>
              <Nav />
{/*               <Banner name="Dashboard"/> */}
              <Dashboard />
            </AppContainer>
          }
        />
        <Route
          path="/threathunting-tactics"
          element={
            <AppContainer>
              <Nav />
              <ThreathuntingTactics />
            </AppContainer>
          }
        />

        <Route
          path="/threat-hunting"
          element={
            <AppContainer>
              <Nav />
              <Banner name="threat-hunting" display="none" />
              <ThreatHunting notification={notification} setNotification={setNotification} />
            </AppContainer>
          }
        />

        <Route
          path="/alerts"
          element={
            <AppContainer>
              <Nav />
              <Banner name="Alerts Status"/>
              <Alerts notification={notification} setNotification={setNotification} />
            </AppContainer>
          }
        />

        <Route
          path="/node"
          element={
            <AppContainer>
              <Nav />
              <Banner name="Node Status"/>
              <Node />
            </AppContainer>
          }
        />
        <Route
          path="/networkdevices"
          element={
            <AppContainer>
              <Nav />
              <Banner name="Network devices" display="none"/>
              <NetworkDevicesTable />
            </AppContainer>
          }
        />

        <Route
          path="/alertSheetView"
          element={
            <AppContainer>
              <Nav />
              <AlertSheetView />
            </AppContainer>
          }
        />
        <Route
          path="/nodeSheetView"
          element={
            <AppContainer>
              <Nav />
              <NodeSheetView />
            </AppContainer>
          }
        />
        <Route
          path="/networkDeviceView"
          element={
            <AppContainer>
              <Nav/>
              <NetworkDeviceSheetView/>
            </AppContainer>
          }
        />
        <Route
          path="/configuration"
          element={
            <AppContainer>
              <Nav />
              <Banner name="Configuration"/>
              <Configuration />
            </AppContainer>
          }
        />
        <Route
          path="/settings"
          element={
            <AppContainer>
              <Nav />
              <Settings/>
            </AppContainer>
          }
        />
        <Route
          path="/rules"
          element={
            <AppContainer>
              <Nav />
              <Banner name="rules" display="none" />
              <Rules/>
            </AppContainer>
          }
          >
            <Route 
              path="/rules/add-python-rule"
              element={
                <AppContainer>
                  <Nav />
                  <PythonRuleEngine />
                </AppContainer>
              }
            />
            <Route
              path="/rules/view-python-rule"
              element={
                <AppContainer>
                  <Nav />
                  <ViewPythonRule />
                </AppContainer>
              }
            />
            <Route
              path="/rules/yara"
              element={
                <AppContainer>
                  <Nav />
                  <Yara />
                </AppContainer>
              }
            />
            <Route
            path="/rules/ip"
            element={
              <AppContainer>
                <Nav/>
                <Ip/>
              </AppContainer>
            }/>
            <Route
              path="/rules/view-rules"
              element={
                <AppContainer>
                  <Nav />
                  <ViewRules />
                </AppContainer>
              }
            />
            <Route
              path="/rules/backend-rules"
              element={
                <AppContainer>
                  <Nav />
                  <ViewBackendRules />
                </AppContainer>
              }
            />
            <Route
              path="/rules/add-rules"
              element={
                <AppContainer>
                  <Nav />
                  <AddRule />
                </AppContainer>
              }
            />
          </Route>
        <Route
          path="/email"
          element={
            <AppContainer>
              <Email />
            </AppContainer>
          }
        />
        <Route
          path="/data-purge"
          element={
            <AppContainer>
              <DataPurge />
            </AppContainer>
          }
        />
        <Route
          path="/download-agent"
          element={
            <AppContainer>
              <Nav/>
              <DownloadAgent/>
            </AppContainer>
        }/>
        <Route
          path="/blog"
          element={
            <div>
              <HomePageNavbar/>
              <Blog/>
            </div>
          }
        />
        <Route
          path="/bloglist"
          element={
            <div>
              <HomePageNavbar/>
              <BlogList/>
            </div>
          }/>
        <Route path="*" element={<Page404 />} />
      </Routes>
    </Router>
  );
}

export default App;
