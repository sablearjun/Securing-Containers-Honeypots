import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import Home from "./pages/Home";
import Dashboard from "./pages/Alerts";
import Navbar from "./components/navbar";
import ContainerInfo from "./pages/ContainerInfo";
import GeoIP from "./pages/GeoIP";
import Alerts from "./pages/Alerts";
import AlertSheetView from "./pages/alertSheetView";

function App() {
  return (
    <Router>
      <Routes>
        <Route
          path="/"
          element={
            <div className="vh-100 bg-light">
              <div className="bg-primary w-100" style={{ height: "6px" }}>
                <div className="vstack gap-3 px-5 py-3">
                  <Navbar />
                  <Home />
                </div>
              </div>
            </div>
          }
        />
        <Route
          path="/containerInfo"
          element={
            <div className="vh-100 bg-light">
              <div className="bg-primary w-100" style={{ height: "6px" }}>
                <div className="vstack gap-3 px-5 py-3">
                  <Navbar />
                  <ContainerInfo />
                </div>
              </div>
            </div>
          }
        />
        <Route
          path="/alertSheetView"
          element={
            <>
              <Navbar />
              <AlertSheetView />
            </>
          }
        />
        <Route
          path="/alerts"
          element={
            <div className="vh-100 bg-light">
              <div className="bg-primary w-100" style={{ height: "6px" }}>
                <div className="vstack gap-3 px-5 py-3">
                  <Navbar />
                  <Alerts />
                </div>
              </div>
            </div>
          }
        />
        <Route
          path="/geoip"
          element={
            <div className="vh-100 bg-light">
              <div className="bg-primary w-100" style={{ height: "6px" }}>
                <div className="vstack gap-3 px-5 py-3">
                  <Navbar />
                  <GeoIP />
                </div>
              </div>
            </div>
          }
        />
      </Routes>
    </Router>
  );
}

export default App;
