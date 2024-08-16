/* eslint-disable jsx-a11y/anchor-is-valid */
import React from "react";

const ExportAlert = () => {
  return (
    <div className="btn-group">
      <button
        type="button"
        className="btn btn-sm btn-outline-secondary text-dark"
      >
        <i className="fi fi-rr-paper-plane me-2 align-middle"></i>Export Alerts
      </button>
      <button
        type="button"
        className="btn btn-sm btn-outline-secondary dropdown-toggle dropdown-toggle-split"
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
  );
};

export default ExportAlert;
