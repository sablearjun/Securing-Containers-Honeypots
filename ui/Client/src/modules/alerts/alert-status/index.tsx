import React from "react";

const AlertStatus = () => {
  return (
    <div className="hstack">
      <div className="d-flex flex-column justify-content-start">
        <h3 className="fw-mid my-0">Alerts Status</h3>
        <div>
          <div className="mt-2">
            <button className="fs-8 px-1 btn btn-link text-muted">
              7 Days
            </button>
            <button className="fs-8 px-1 btn btn-link text-muted active">
              15 Days
            </button>
            <button className="fs-8 px-1 btn btn-link text-muted">
              30 Days
            </button>
          </div>
        </div>
      </div>
    </div>
  );
};

export default AlertStatus;
