/* eslint-disable jsx-a11y/anchor-is-valid */
import React, { useRef } from "react";
import TextField from "@mui/material/TextField";
import Button from "react-bootstrap/Button";
import Collapse from "react-bootstrap/Collapse";
import { useState, useEffect } from "react";
import { NODES_LIST_URL } from "../../../apis/urls";
import { useLazyFetch } from "../../../hooks";
import { Event, Node, toggle_state } from "../../../types";
import { Link } from "react-router-dom";
import loaderForTable from "../../../assets/images/loader-table.svg";
import ReactTooltip from "react-tooltip";
import { fileName, timeConverter } from "../../../helpers";
import CsvDownloadButton from "react-json-to-csv";
import { Error } from "../../../components/Error";
import { toast, ToastContainer } from "react-toastify";
import { deviceData } from "../../../data/networkdevices";

const NetworkDevicesTable = () => {
  let url = NODES_LIST_URL;

  const [filter_enroll_time_btn, setFilter_enroll_time_btn] = useState(false);
  const [SearchHostName, setSearchHostName] = useState<String | null>(null);
  const [HostName, setHostName] = useState<String | null>(null);
  const [HostIdentifier, setHostIdentifier] = useState<String | null>(null);
  const [SearchStartTime, setSearchStartTime] = useState<any | null>(null);
  const [SearchEndTime, setSearchEndTime] = useState<any | null>(null);
  const [ispast, setispast] = useState<number>(0);
  const [isactive, setIsactive] = useState<boolean | null>(null);
  const [machine, setMachine] = useState({ online: 0, offline: 0, total: 0 });
  const [istoggle, setIstoggle] = useState<toggle_state | null>(null);
  const [isrow, setisrow] = useState(100);
  const [ispageNo, setispageNo] = useState(1);

  //   useEffect(() => {
  //     if (isactive == null) {
  //       url = NODES_LIST_URL;
  //     } else if (isactive) {
  //       url = NODES_LIST_URL + "?is_active=true";
  //     } else {
  //       url = NODES_LIST_URL + "?is_active=false";
  //     }
  //     if (SearchHostName !== null) {
  //       if (url.endsWith("/")) {
  //         url = NODES_LIST_URL + "?hostname=" + SearchHostName;
  //       } else {
  //         url = url + "&hostname=" + SearchHostName;
  //       }
  //     }
  //     if (SearchStartTime !== null) {
  //       if (typeof SearchStartTime == "object") {
  //         if (url.endsWith("/")) {
  //           url =
  //             NODES_LIST_URL + "?start_time=" + String(SearchStartTime.unixTime);
  //         } else {
  //           url = url + "&start_time=" + String(SearchStartTime.unixTime);
  //         }
  //       }
  //     }
  //     if (SearchEndTime !== null) {
  //       if (typeof SearchEndTime == "object") {
  //         if (url.endsWith("/")) {
  //           url = NODES_LIST_URL + "?end_time=" + String(SearchEndTime.unixTime);
  //         } else {
  //           url = url + "&end_time=" + String(SearchEndTime.unixTime);
  //         }
  //       }
  //     }
  //     if(ispageNo > 1){
  //       if(isrow!==0){
  //         url = NODES_LIST_URL + "?pageno=" + ispageNo + "&rows=" + isrow;
  //       }else{
  //         url = NODES_LIST_URL + "?pageno=" + ispageNo;
  //       }
  //     }else if(isrow !== 0) {
  //       if (url.endsWith("/")) {
  //         url = NODES_LIST_URL + "?rows=" + isrow;
  //       } else {
  //         url = url + "&rows=" + isrow;
  //       }
  //     }
  //     // run({ URL: url });
  //   }, [
  //     isactive,
  //     ispast,
  //     istoggle,
  //     isrow,
  //     ispageNo,
  //     SearchHostName,
  //     SearchEndTime,
  //     SearchStartTime,
  //   ]);

  const { loading, error, data, run } = useLazyFetch<{
    nodes: Node[];
    no_of_records: number;
  }>();

  // function getResults() {
  //   if (loading) {
  //     return (
  //       <div className="loading-table-gif">
  //         <img src={loaderForTable} />
  //       </div>
  //     );
  //   } else if (error) {
  //     if (error.message === "Unauthorized") {
  //       <div className="">Unauthorized</div>;
  //       return;
  //     }
  //     return <p>{error.message}</p>;
  //   } else if (data) {
  //     const dataNodes = data.nodes;

  //     if(((filter_enroll_time_btn !== false)|| (SearchHostName!==null)||(SearchStartTime !== null) || (SearchEndTime !== null)) && data.nodes.length<=0){
  //       setFilter_enroll_time_btn(false);
  //       setSearchHostName(null);
  //       setSearchStartTime(null);
  //       setSearchEndTime(null);
  //       toast.error("No nodes with such filter available",{
  //         toastId:'no_nodes'
  //       });
  //       return;
  //     }

  //     if (data.nodes.length <= 0) return <Error message="No nodes available" />;

  //     if (isactive == null && SearchHostName == null) {
  //       let online: number = 0;
  //       let offline: number = 0;
  //       data.nodes.filter((row) => {
  //         if (row.status == "Online") {
  //           online = online + 1;
  //         } else {
  //           offline = offline + 1;
  //         }
  //       });
  //       machine.online = online;
  //       machine.offline = offline;
  //       machine.total = data.nodes.length;
  //     }

  //     return (
  //       <table className="table table-hover">
  //         <thead
  //           className="bg-white"
  //           style={{ position: "sticky", top: 0, zIndex: 950 }}
  //         >
  //           <tr className="fw-light text-muted">
  //             {/* <th className="fw-light" scope="col">
  //               {HostName !== null ? (
  //                 <TextField
  //                   id="standard-basic"
  //                   label={
  //                     <>
  //                       Device Name
  //                       <i
  //                         className="fa fa-search chat-arrow-grow align-middle"
  //                         style={{ marginLeft: "1rem", color: "black" }}
  //                       ></i>
  //                     </>
  //                   }
  //                   variant="standard"
  //                   value={String(HostName)}
  //                   InputProps={{ disableUnderline: true }}
  //                   onChange={(e) => {
  //                     setHostName((e.target as HTMLInputElement).value);
  //                   }}
  //                   onKeyUp={(e) => {
  //                     if (e.key === "Enter") {
  //                       setSearchHostName(HostName);
  //                     }
  //                   }}
  //                 />
  //               ) : (
  //                 <TextField
  //                   id="standard-basic"
  //                   label={
  //                     <>
  //                       Device Name
  //                       <i
  //                         className="fa fa-search chat-arrow-grow align-middle"
  //                         style={{ marginLeft: "1rem", color: "black" }}
  //                       ></i>
  //                     </>
  //                   }
  //                   variant="standard"
  //                   InputProps={{ disableUnderline: true }}
  //                   onChange={(e) => {
  //                     setHostName((e.target as HTMLInputElement).value);
  //                   }}
  //                   onKeyUp={(e) => {
  //                     if (e.key === "Enter") {
  //                       setSearchHostName(HostName);
  //                     }
  //                   }}
  //                 />
  //               )}
  //             </th>
  //             <th className="fw-light" scope="col">
  //               {filter_enroll_time_btn === false ? (
  //                 <Button
  //                   onClick={() =>
  //                     setFilter_enroll_time_btn(!filter_enroll_time_btn)
  //                   }
  //                   // aria-controls="example1-collapse-text"
  //                   aria-expanded={filter_enroll_time_btn}
  //                   style={{
  //                     border: "none",
  //                     backgroundColor: "transparent",
  //                     color: "grey",
  //                     padding: 0,
  //                     outline: "none",
  //                     height: "3vh",
  //                   }}
  //                   className="btn_y"
  //                 >
  //                   {filter_enroll_time_btn ? (
  //                     ""
  //                   ) : (
  //                     <>
  //                       Enrolled on
  //                       <i
  //                         className="fa fa-search chat-arrow-grow align-middle"
  //                         style={{ marginLeft: "1rem", color: "black" }}
  //                       ></i>
  //                     </>
  //                   )}
  //                 </Button>
  //               ) : (
  //                 <></>
  //               )}
  //               <Collapse in={filter_enroll_time_btn}>
  //                 <div
  //                   id="example1-collapse-text"
  //                   style={{ marginTop: "0.5rem" }}
  //                 >
  //                   <div className="d-flex align-items-center">
  //                     {SearchStartTime !== null ? (
  //                       <TextField
  //                         id="datetime-local"
  //                         label="Start Date"
  //                         type="datetime-local"
  //                         defaultValue={SearchStartTime.Time}
  //                         sx={{ width: 250 }}
  //                         InputLabelProps={{
  //                           shrink: true,
  //                         }}
  //                         onKeyPress={(e) => {
  //                           if (e.key === "Enter") {
  //                             let time =
  //                               Date.parse(
  //                                 (e.target as HTMLInputElement).value
  //                               ) / 1000;
  //                             setSearchStartTime({
  //                               unixTime: time,
  //                               Time: (e.target as HTMLInputElement).value,
  //                             });
  //                           }
  //                         }}
  //                       />
  //                     ) : (
  //                       <TextField
  //                         id="datetime-local"
  //                         label="Start Date"
  //                         type="datetime-local"
  //                         sx={{ width: 250 }}
  //                         InputLabelProps={{
  //                           shrink: true,
  //                         }}
  //                         onKeyPress={(e) => {
  //                           if (e.key === "Enter") {
  //                             let time =
  //                               Date.parse(
  //                                 (e.target as HTMLInputElement).value
  //                               ) / 1000;
  //                             setSearchStartTime({
  //                               unixTime: time,
  //                               Time: (e.target as HTMLInputElement).value,
  //                             });
  //                           }
  //                         }}
  //                       />
  //                     )}
  //                     <span
  //                       className="Box"
  //                       onClick={() => {
  //                         setFilter_enroll_time_btn(!filter_enroll_time_btn);
  //                         setSearchStartTime(null);
  //                         setSearchEndTime(null);
  //                       }}
  //                       style={{
  //                         marginLeft: "2rem",
  //                         fontSize: "2rem",
  //                         cursor: "pointer",
  //                       }}
  //                     >
  //                       <i
  //                         className="fa fa-times-circle"
  //                         aria-hidden="true"
  //                       ></i>
  //                     </span>
  //                   </div>
  //                 </div>
  //               </Collapse>
  //             </th>
  //             <th className="fw-light" scope="col">
  //               <TextField
  //                 id="standard-basic"
  //                 disabled={true}
  //                 label="Last Seen"
  //                 variant="standard"
  //                 InputProps={{ disableUnderline: true }}
  //               />
  //             </th>
  //             <th className="fw-light" scope="col">
  //               {HostIdentifier !== null ? (
  //                 <TextField
  //                   id="standard-basic"
  //                   label={
  //                     <>
  //                       MAC Address
  //                       <i
  //                         className="fa fa-search chat-arrow-grow align-middle"
  //                         style={{ marginLeft: "1rem", color: "black" }}
  //                       ></i>
  //                     </>
  //                   }
  //                   variant="standard"
  //                   value={String(HostIdentifier)}
  //                   InputProps={{ disableUnderline: true }}
  //                   onChange={(e) => {
  //                     setHostIdentifier((e.target as HTMLInputElement).value);
  //                   }}
  //                   onKeyUp={(e) => {
  //                     if (e.key === "Enter") {
  //                       setSearchHostName(HostIdentifier);
  //                     }
  //                   }}
  //                 />
  //               ) : (
  //                 <TextField
  //                   id="standard-basic"
  //                   label={
  //                     <>
  //                       MAC Address
  //                       <i
  //                         className="fa fa-search chat-arrow-grow align-middle"
  //                         style={{ marginLeft: "1rem", color: "black" }}
  //                       ></i>
  //                     </>
  //                   }
  //                   variant="standard"
  //                   InputProps={{ disableUnderline: true }}
  //                   onChange={(e) => {
  //                     setHostIdentifier((e.target as HTMLInputElement).value);
  //                   }}
  //                   onKeyUp={(e) => {
  //                     if (e.key === "Enter") {
  //                       setSearchHostName(HostIdentifier);
  //                     }
  //                   }}
  //                 />
  //               )}
  //             </th>

  //             <th className="fw-light" scope="col">
  //               <TextField
  //                 id="standard-basic"
  //                 disabled={true}
  //                 label="View"
  //                 style={{ width: "2vw" }}
  //                 variant="standard"
  //                 InputProps={{ disableUnderline: true }}
  //               />
  //             </th> */}
  //             <th className="fw-light" scope="col">
  //               Device Name
  //             </th>
  //             <th className="fw-light" scope="col">
  //               Enrolled On
  //             </th>
  //             <th className="fw-light" scope="col">
  //               Last Seen
  //             </th>
  //             <th className="fw-light" scope="col">
  //               MAC Address
  //             </th>
  //             <th className="fw-light" scope="col">
  //               View
  //             </th>

  //           </tr>
  //         </thead>
  //         <tbody className="border-top-0">
  //           {
  //             deviceData.map((data:any, it:any) => {
  //           <tr key={it}>
  //               <td>{data.name}</td>
  //               <td>{data.enrolled_on}</td>
  //               <td>{data.last_seen}</td>
  //               <td>{data.mac_address}</td>
  //               <td><Link to="/networkDeviceView" state={data}>
  //                     <button
  //                       type="button"
  //                       className="btn btn-primary-light ntn-sm"
  //                     >
  //                       <i className="fa fa-eye"></i>
  //                     </button>
  //                   </Link></td>
  //           </tr>
  //           })}
  //         </tbody>
  //       </table>
  //     );
  //   }

  //   return null;
  // }

  const handleFilterChange = (event: any) => {
    setisrow(event.target.value);
  };

  return (
    <>
      <ToastContainer />
      <div className="card py-3">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Network Devices</h4>
            </div>

            <div className="ms-auto"></div>
          </div>
        </div>
        <div className="card-body px-6">
          <div className="d-flex flex-column align-items-start gap-3">
            <div className="d-flex flex-column flex-md-row justify-content-between align-items-center gap-3 w-100">
              <div className="d-flex justify-content-between align-items-center gap-3">
                <div>
                  <ul
                    className="nav nav-pills mb-3"
                    id="pills-tab"
                    role="tablist"
                  >
                    <li className="nav-item" role="presentation">
                      <button
                        className="nav-link active"
                        id="pills-profile-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-home"
                        type="button"
                        role="tab"
                        aria-controls="pills-home"
                        aria-selected="true"
                        onClick={() => {
                          setIsactive(null);
                          setSearchHostName(null);
                          setSearchEndTime(null);
                          setSearchStartTime(null);
                        }}
                      >
                        All Devices
                      </button>
                    </li>
                    <li className="nav-item" role="presentation">
                      <button
                        className="nav-link"
                        id="pills-contact-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-contact"
                        type="button"
                        role="tab"
                        aria-controls="pills-contact"
                        aria-selected="false"
                        onClick={() => {
                          if (isactive || isactive == null) {
                            setIsactive(false);
                          }
                          if (ispast !== 0) {
                            setispast(0);
                          }
                        }}
                      >
                        Offline Devices
                      </button>
                    </li>
                    <li className="nav-item" role="presentation">
                      <button
                        className="nav-link"
                        id="pills-contact-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-contact"
                        type="button"
                        role="tab"
                        aria-controls="pills-contact"
                        aria-selected="false"
                        onClick={() => {
                          if (!isactive) {
                            setIsactive(true);
                          }
                          if (ispast !== 0) {
                            setispast(0);
                          }
                        }}
                      >
                        Online Devices
                      </button>
                    </li>
                  </ul>
                </div>
              </div>
              <div>
                <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                  <nav aria-label="Standard pagination example">
                    <ul className="pagination pagination-sm mb-0">
                      <li className="page-item">
                        <a
                          className="page-link"
                          href="#"
                          aria-label="Previous"
                          onClick={() => {
                            if (ispageNo > 1) {
                              setispageNo(ispageNo - 1);
                            }
                          }}
                        >
                          {" "}
                          <span aria-hidden="true">« Prev</span>{" "}
                        </a>
                      </li>
                      <li className="page-item">
                        <a
                          className="page-link"
                          href="#"
                          aria-label="Next"
                          onClick={() => setispageNo(ispageNo + 1)}
                        >
                          {" "}
                          <span aria-hidden="true">Next »</span>{" "}
                        </a>
                      </li>
                    </ul>
                  </nav>
                  <div className="form-group">
                    <label className="text-muted">No. of rows:&nbsp;</label>
                    <select
                      className="form-select-sm p-1"
                      onChange={handleFilterChange}
                      value={isrow}
                      style={{ border: "1px solid silver" }}
                    >
                      <option value="25">25</option>
                      <option value="50">50</option>
                      <option value="100">100</option>
                    </select>
                  </div>
                  {data !== null ? (
                    <CsvDownloadButton
                      className="btn btn-sm btn-outline-secondary text-dark"
                      filename={fileName("Network Devices")}
                      data={data?.nodes}
                    >
                      <i className="fa fa-download me-2 align-middle"></i>Export
                      Network Devices
                    </CsvDownloadButton>
                  ) : (
                    <></>
                  )}
                </div>
              </div>
            </div>
            <div
              className="w-100 overflow-auto styleScroll"
              style={{ height: "calc(100vh - 25rem)" }}
              id="#scrollbar-1"
            >
              <table className="table table-hover">
                <thead
                  className="bg-white"
                  style={{ position: "sticky", top: 0, zIndex: 950 }}
                >
                  <tr className="fw-light text-muted">
                    <th className="fw-light" scope="col">
                      Device Name
                    </th>
                    <th className="fw-light" scope="col">
                      Enrolled On
                    </th>
                    <th className="fw-light" scope="col">
                      Last Seen
                    </th>
                    <th className="fw-light" scope="col">
                      MAC Address
                    </th>
                    <th className="fw-light" scope="col">
                      View
                    </th>
                  </tr>
                </thead>
                <tbody className="border-top-0">
                  {deviceData.map((data: any, it: any) => {
                    return (
                      <tr key={it}>
                        <td>{data.device_name}</td>
                        <td>{data.enrolled_on}</td>
                        <td>{data.last_seen}</td>
                        <td>{data.mac_address}</td>
                        <td>
                          <Link to="/networkDeviceView" state={data}>
                            <button
                              type="button"
                              className="btn btn-primary-light ntn-sm"
                            >
                              <i className="fa fa-eye"></i>
                            </button>
                          </Link>
                        </td>
                      </tr>
                    );
                  })}
                </tbody>
              </table>
            </div>
          </div>
        </div>
      </div>
    </>
  );
};

export default NetworkDevicesTable;
