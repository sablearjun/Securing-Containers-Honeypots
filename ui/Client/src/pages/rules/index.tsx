import { Link, Navigate, Route, Routes, useLocation } from "react-router-dom"
import { PythonRuleEngine } from "../../modules/threat-hunting/pythonRule"
import ViewPythonRule from "../../modules/threat-hunting/pythonRule/view"
import AddRule from "./rule-builder/addrules"
import Ip from "./ip/ip"
import ViewRules from "./rule-builder/viewrules"
import Yara from "./yara/yara"
import ViewBackendRules from "./backend-rules/backend-rules"

const Rules = () => {

    const location = useLocation()

    return (
        <>
        <div className="card py-3">
            <div className="card-header bg-white border-0">
                <div className="hstack">
                    <div>
                        <h4 className="fw-mid my-0">Rules
                        </h4>
                    </div>
                </div>
            </div>
            <div className="card-body px-6">
                <ul className="nav nav-pills mb-3" id="pills-tab" role="tablist">
                    <li className="nav-item" role="presentation">
                        <Link to="/rules/view-python-rule" style={{textDecoration: 'none'}}>
                            <button
                                className={`nav-link ${
                                    location.pathname === "/rules/view-python-rule" || location.pathname === "/rules/add-python-rule"
                                    ? "active" : ""
                                }`}
                                id="pills-profile-tab"
                                data-bs-toggle="pill"
                                type="button"
                                role="tab"
                            >
                                Python Rules
                            </button>
                        </Link>
                    </li>
                    <li className="nav-item" role="presentation">
                        <Link to="/rules/view-rules" style={{textDecoration: 'none'}}>
                            <button
                                className={`nav-link ${
                                    location.pathname === "/rules/view-rules" || location.pathname === "/rules/add-rules"
                                    ? "active" : ""
                                }`}
                                id="pills-profile-tab"
                                data-bs-toggle="pill"
                                type="button"
                                role="tab"
                            >
                            Rule Builder
                        </button>
                        </Link>
                    </li>
                    <li className="nav-item" role="presentation">
                        <Link to="/rules/yara" style={{textDecoration: 'none'}}>
                            <button
                                className={`nav-link ${
                                    location.pathname === "/rules/yara" ? "active" : ""
                                }`}
                                id="pills-profile-tab"
                                data-bs-toggle="pill"
                                type="button"
                                role="tab"
                            >
                                Yara Files
                            </button>
                        </Link>
                    </li>
                    <li className="nav-item" role="presentation">
                        <Link to="/rules/ip" style={{textDecoration: 'none'}}>
                            <button
                                className={`nav-link ${
                                    location.pathname === "/rules/ip" ? "active" : ""
                                }`}
                                id="pills-profile-tab"
                                data-bs-toggle="pill"
                                type="button"
                                role="tab"
                            >
                                IP files
                            </button>
                        </Link>
                    </li>
                    <li className="nav-item" role="presentation">
                        <Link to="/rules/backend-rules" style={{textDecoration: 'none'}}>
                            <button
                                className={`nav-link ${
                                    location.pathname === "/rules/backend-rules" ? "active" : ""
                                }`}
                                id="pills-profile-tab"
                                data-bs-toggle="pill"
                                type="button"
                                role="tab"
                            >
                                Display Backend Rules
                            </button>
                        </Link>
                    </li>
                </ul>
                <section className='h-100'>
                    <Routes>
                        <Route
                            path="/"
                            element={<Navigate to="/rules/view-python-rule" />}
                        />
                        <Route
                            path="/add-python-rule"
                            element={ <PythonRuleEngine /> }
                        />
                        <Route
                            path="/view-python-rule"
                            element={ <ViewPythonRule /> }
                        />
                        <Route
                            path="/yara"
                            element={ <Yara /> }
                        />
                        <Route
                            path="/ip"
                            element={ <Ip /> }
                        />
                        <Route
                            path="/view-rules"
                            element={ <ViewRules /> }
                        />
                        <Route
                            path="/backend-rules"
                            element={ <ViewBackendRules /> }
                        />
                         <Route
                            path="/add-rules"
                            element={ <AddRule /> }
                        />
                    </Routes>
                </section>
            </div>
        </div>
        </>
    )
}

export default Rules