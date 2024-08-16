import ThreatSelectionNavbar from "../SelectionNavbar";

const ThreatHuntSelection = () => {
  return (
    <>
      <div className="card-header bg-white border-0">
        <div className="hstack">
          <div>
            <h4 className="fw-mid my-0">Select a Threat Hunt</h4>
          </div>
          <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
            <button className="btn btn-link fw-semi text-success btn-sm">
              <i className="fi fi-rr-refresh  me-2 align-middle"></i>Update
              Hunts
            </button>
            <div className="vr h-8px my-auto"></div>
            <div className="d-flex justify-content-start align-items-center">
              <button type="button" className="btn btn-link btn-sm text-dark">
                <i className="fi fi-rr-edit  me-2 align-middle"></i>Add Notes
              </button>
            </div>
            <div className="vr h-8px my-auto"></div>
            <div className="d-flex justify-content-start align-items-center">
              <button type="button" className="btn btn-link btn-sm text-dark">
                <i className="fi fi-rr-settings me-2 align-middle"></i>Settings
              </button>
            </div>
            <div className="vr h-8px my-auto"></div>
            <div className="btn-group">
              <button
                type="button"
                className="btn border-0 btn-sm btn-outline-secondary text-dark"
              >
                <i className="fi fi-rr-paper-plane me-2 align-middle"></i>Export
                Hunts
              </button>
              <button
                type="button"
                className="btn btn-sm btn-light dropdown-toggle dropdown-toggle-split"
                data-bs-toggle="dropdown"
                aria-expanded="false"
              >
                <span className="visually-hidden">Toggle Dropdown</span>
              </button>
              <ul className="dropdown-menu">
                <li>
                  <a className="dropdown-item" href="#">
                    Export as .xls
                  </a>
                </li>
                <li>
                  <a className="dropdown-item" href="#">
                    Export as .csv
                  </a>
                </li>
                <li>
                  <a className="dropdown-item" href="#">
                    Export as .txt
                  </a>
                </li>
                <li>
                  <hr className="dropdown-divider" />
                </li>
                <li>
                  <a className="dropdown-item" href="#">
                    Custom Export Modal
                  </a>
                </li>
              </ul>
            </div>
            <div className="d-flex justify-content-start align-items-center">
              <button
                type="button"
                className="btn btn-primary btn-sm align-middle"
              >
                <i className="fi fi-rr-add me-2 align-middle"></i>Add New Threat
                Hunt
              </button>
            </div>
          </div>
        </div>
      </div>
      <div className="card-body px-5">
        <div className="d-flex flex-column align-items-start">
          {/* <ThreatSelectionNavbar /> */}
        </div>
      </div>
    </>
  );
};

export default ThreatHuntSelection;
