/* eslint-disable jsx-a11y/anchor-is-valid */
import { HistoricData } from "../../../types";
import CsvDownloadButton from 'react-json-to-csv'

interface DataTableProps {
  data?: HistoricData[];
  runQuery:boolean
}
const DataTable = ({ data = [],runQuery }: DataTableProps) => {
  const keys: Array<keyof HistoricData> = (
    data.length > 0 ? Object.keys(data[0]) : []
  ) as Array<keyof HistoricData>;
  let date=Date().toLocaleString().substring(0,(Date().toLocaleString().length-21))+"(IST)";
  return (
    <div className="d-flex flex-column align-items-start gap-3" style={{width:"100%"}}>
      <div className="d-flex justify-content-between align-items-center gap-3 w-100">
        <div className="d-flex justify-content-between align-items-center gap-3">
          <h6 className="me-1 my-0">Threat hunt Result Table</h6>
          <p className="fs-7 my-0 text-muted fw-mid">
            <i className="fi fi-rr-file-check text-run mx-1 align-middle"></i>
            {/* 2,000 Rows */}
            {data.length.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",")} Rows
          </p>
          <p className="fs-7 my-0 text-muted fst-italic">
            {/* Status Message here for the table rendered */}
          </p>
        </div>
        <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
          <button className="btn btn-link fw-semi text-muted btn-sm">
            {/* Feb 1, 2020 11:59 am (IST) */}
            {date}
          </button>
          <div className="d-flex justify-content-start align-items-center">
            <p className="my-0 form-label text-muted fw-light me-1">Hosts:</p>
            <button className="btn btn-link fw-semi text-muted btn-sm">
              All
            </button>
          </div>
          {/* <div className="vr h-8px my-auto"></div> */}
          {/* <div className="d-flex justify-content-start align-items-center">
            <p className="my-0 form-label text-muted fw-light me-1">
              Filter 1:
            </p>
            <button className="btn btn-link fw-semi text-muted btn-sm">
              All Sides
            </button>
          </div> */}
          {/* <div className="vr h-8px my-auto"></div> */}
          {/* <div className="d-flex justify-content-start align-items-center">
            <p className="my-0 form-label text-muted fw-light me-1">
              Filter 2:
            </p>
            <button className="btn btn-link fw-semi text-muted btn-sm">
              More Data
            </button>
          </div> */}
          <div className="vr h-8px my-auto"></div>
          <div className="btn-group">
            {
              data!==null?
              <CsvDownloadButton className="btn btn-sm btn-outline-secondary text-dark" data={data} >
                <i className="fi fi-rr-paper-plane me-2 align-middle"></i>Export Alerts
              </CsvDownloadButton>:<></>
            }
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
                  Action
                </a>
              </li>
              <li>
                <a className="dropdown-item" href="#">
                  Another action
                </a>
              </li>
              <li>
                <a className="dropdown-item" href="#">
                  Something else here
                </a>
              </li>
              <li>
                <hr className="dropdown-divider" />
              </li>
              <li>
                <a className="dropdown-item" href="#">
                  Separated link
                </a>
              </li>
            </ul>
          </div>
        </div>
      </div>
      {
        runQuery?
      (<div
        className="w-100 overflow-auto"
        style={{ height: "calc(100vh - 45rem)" }}
        id="#scrollbar-1"
      >
        <table className="table table-sm table-hover fs-7">
          <thead
            className="bg-white"
            style={{ position: "sticky", top: 0, zIndex: 2000 }}
          >
            <tr className="fw-light text-muted">
              {keys.map((key) => (
                <th key={key} className="fw-light" scope="col">
                  {(key.replace("_"," "))[0].toUpperCase()+(key.replace("_"," ")).slice(1)}
                </th>
              ))}
            </tr>
          </thead>
          <tbody className="border-top-0">
            {data.map((data, _) => {
              return (
                <tr key={_}>
                  {keys.map((key) => {
                    return <td key={key}>{data[key]}</td>;
                  })}
                </tr>
              );
            })}
          </tbody>
        </table>
      </div>):
      (<div
        className="w-100 overflow-auto"
        style={{ height: "calc(100vh - 34.5rem)" }}
        id="#scrollbar-1"
      >
        <table className="table table-sm table-hover fs-7">
          <thead
            className="bg-white"
            style={{ position: "sticky", top: 0, zIndex: 2000 }}
          >
            <tr className="fw-light text-muted">
              {keys.map((key) => (
                <th key={key} className="fw-light" scope="col">
                  {(key.replace("_"," "))[0].toUpperCase()+(key.replace("_"," ")).slice(1)}
                </th>
              ))}
            </tr>
          </thead>
          <tbody className="border-top-0">
            {data.map((data, _) => {
              return (
                <tr key={_}>
                  {keys.map((key) => {
                    return <td key={key}>{data[key]}</td>;
                  })}
                </tr>
              );
            })}
          </tbody>
        </table>
      </div>)
      }
    </div>
  );
};

export default DataTable;
