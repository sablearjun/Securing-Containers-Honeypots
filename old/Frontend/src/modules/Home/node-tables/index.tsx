/* eslint-disable jsx-a11y/anchor-is-valid */
import React, { useRef } from "react";
import axios from "axios";
import no_data from "../../../assets/images/data_not_found.png";
import TextField from '@mui/material/TextField';
import Stack from '@mui/material/Stack';
import ProgressBar from 'react-bootstrap/ProgressBar'
import Box from '@mui/material/Box';
import Button from 'react-bootstrap/Button';
import Collapse from 'react-bootstrap/Collapse';
//import { TOKEN } from "../../../apis/base-urls";
import { useState, useEffect } from "react";
import { EVENTS_LIST_URL, EVENTS_UPDATE_URL, HISTORIC_DATA_URL, NODES_LIST_URL } from "../../../apis/urls";
import { getDefaultHeaders, useFetchURL, useLazyFetch } from "../../../hooks";
import { Event,Node , toggle_state} from "../../../types";
import ExportNode from "../node-alert";
// import NodeSheetView from "../../../pages/nodeSheetView";
import { Link } from "react-router-dom";
import { Navigate } from "react-router";
import * as bootstrap from "bootstrap";
import swal from 'sweetalert'
import  loaderForTable from '../../../assets/images/loader-table.svg'
import { json } from "stream/consumers";
import ReactTooltip from "react-tooltip";
import { timeConverter } from "../../../helpers";
import { getData } from "../../../apis/requests";

const NodeTable = () => {
  let url = NODES_LIST_URL;
  
  // const [filter_host_btn, setFilter_host_btn] = useState(false);
  const [filter_enroll_time_btn, setFilter_enroll_time_btn] = useState(false);
  const [SearchHostName, setSearchHostName] = useState<String | null>(null);
  const [HostName, setHostName] = useState<String | null>(null);
  const [HostIdentifier, setHostIdentifier] = useState<String | null>(null);
  const [SearchStartTime, setSearchStartTime] = useState<any | null>(null);
  const [SearchEndTime, setSearchEndTime] = useState<any | null>(null);
  const [ispast, setispast] = useState<number>(0);
  const [isactive, setIsactive] = useState<boolean | null>(null);
  // const [ref, setref] = useState(false)
  const [machine, setMachine] = useState({online: 0,offline: 0,total: 0});
  const [istoggle, setIstoggle] = useState<toggle_state | null>(null);
  const [isrow, setisrow] = useState(100);
  const [ispageNo, setispageNo] = useState(1);
  const [selectedAlerts, setSelectedAlerts] = useState<any>([])
  const closeAlertModalRef = useRef() as React.MutableRefObject<HTMLInputElement>;
  const [containerList, setContainerList] = useState<any>(undefined)

  useEffect(() => {
    if(containerList===undefined)
    {
      getData(HISTORIC_DATA_URL,setContainerList,"POST",{"query":"select * from (select distinct(pid) from docker_events where not pid='0') a join (select * from docker_events) b on a.pid=b.pid where action = 'start' order by time desc"})
    }
  }, []);


  const { loading, error, data, run } = useLazyFetch<{
    nodes: Node[];
    no_of_records: number;
  }>();

  function getResults() {
    // if (loading) {
    //   return <div className="loading-table-gif"><img src={loaderForTable} /></div>;
    // } else if (error) {
    //   return <p>{error.message}</p>;
    // } else 
    if (containerList!==undefined) {
      let finalData = [{"enrolled_on":"1671698755","hardware_cpu_logical_core":"8","hardware_cpu_type":"x86_64","hardware_model":"Inspiron 5370","hardware_physical_memory":"8235819008","hardware_vendor":"Dell Inc.","hardware_version":"","host_identifier":"4c4c4544-0050-5810-805a-b9c04f4c3432","hostname":"sablearjun","last_seen":1671708762,"node_invalid":"f","node_key":"qDiZSkZfoEDAmGTXXqqv","os_arch":"x86_64","os_build":"unknown","os_major":"2022","os_minor":"4","os_name":"Kali GNU/Linux","os_platform":"kali","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Online"},{"enrolled_on":"1671538258","hardware_cpu_logical_core":"2","hardware_cpu_type":"x86_64","hardware_model":"VMware Virtual Platform","hardware_physical_memory":"2074198016","hardware_vendor":"VMware, Inc.","hardware_version":"None","host_identifier":"94cd4d56-dd97-d3ac-1d8e-01b06f6cd9ef","hostname":"NCETIS","last_seen":1671538258,"node_invalid":"f","node_key":"Oa5LnIMIQuOiNJj8YL8r","os_arch":"x86_64","os_build":"unknown","os_major":"21","os_minor":"04","os_name":"Ubuntu","os_platform":"ubuntu","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Offline"},{"enrolled_on":"1667569596","hardware_cpu_logical_core":"20","hardware_cpu_type":"x86_64","hardware_model":"Dell G15 5520","hardware_physical_memory":"16441458688","hardware_vendor":"Dell Inc.","hardware_version":"","host_identifier":"4c4c4544-0059-4610-804c-c7c04f315133","hostname":"fedora","last_seen":1667582412,"node_invalid":"f","node_key":"KTx0nFe1IX5tjH22F5gX","os_arch":"x86_64","os_build":"","os_major":"36","os_minor":"0","os_name":"Fedora Linux","os_platform":"rhel","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Offline"},{"enrolled_on":"1667295554","hardware_cpu_logical_core":"16","hardware_cpu_type":"x86_64","hardware_model":"System Product Name","hardware_physical_memory":"16536645632","hardware_vendor":"ASUS","hardware_version":"System Version","host_identifier":"03345eaa-ffa5-ef99-e85d-04421a2d938c","hostname":"IEOR","last_seen":1671708771,"node_invalid":"f","node_key":"opGY4i6bYuUG67LaSXd5","os_arch":"x86_64","os_build":"unknown","os_major":"2022","os_minor":"2","os_name":"Kali GNU/Linux","os_platform":"kali","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Online"},{"enrolled_on":"1663668910","hardware_cpu_logical_core":"12","hardware_cpu_type":"x86_64","hardware_model":"HP Pavilion Gaming Laptop 15-dk0xxx","hardware_physical_memory":"16611119104","hardware_vendor":"HP","hardware_version":"Type1ProductConfigId","host_identifier":"36cd3420-4f54-ea11-8104-e8d8d1fb7083","hostname":"sunil-HP-Pavilion","last_seen":1669976394,"node_invalid":"f","node_key":"ETPVzxlFrXL8b7mtKLHI","os_arch":"x86_64","os_build":"unknown","os_major":"21","os_minor":"10","os_name":"Ubuntu","os_platform":"ubuntu","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Offline"},{"enrolled_on":"1662553724","hardware_cpu_logical_core":"8","hardware_cpu_type":"x86_64","hardware_model":"81NE","hardware_physical_memory":"8076103680","hardware_vendor":"LENOVO","hardware_version":"Lenovo IdeaPad S540-15IWL D","host_identifier":"20190727-5c87-9c38-1fb2-5c879c381fb6","hostname":"sablearjun","last_seen":1667450952,"node_invalid":"f","node_key":"tzxlYvKWrO72tK0LK0e1","os_arch":"x86_64","os_build":"unknown","os_major":"22","os_minor":"04","os_name":"Ubuntu","os_platform":"ubuntu","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Offline"},{"enrolled_on":"1662463524","hardware_cpu_logical_core":"20","hardware_cpu_type":"x86_64","hardware_model":"Dell G15 5520","hardware_physical_memory":"16446586880","hardware_vendor":"Dell Inc.","hardware_version":"","host_identifier":"4c4c4544-0050-3510-805a-b7c04f315133","hostname":"vajra","last_seen":1671707298,"node_invalid":"f","node_key":"kCd5WKE2fMtVXa2zKae6","os_arch":"x86_64","os_build":"unknown","os_major":"22","os_minor":"04","os_name":"Ubuntu","os_platform":"ubuntu","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Offline"},{"enrolled_on":"1662380514","hardware_cpu_logical_core":"16","hardware_cpu_type":"x86_64","hardware_model":"System Product Name","hardware_physical_memory":"16537047040","hardware_vendor":"ASUS","hardware_version":"System Version","host_identifier":"29adb7f9-12fa-cccb-41e0-04421a2d9324","hostname":"sablearjun.iitb.ac.in","last_seen":1671708761,"node_invalid":"f","node_key":"J4ogHFZQxwQ2RQ0DRJKR","os_arch":"x86_64","os_build":"unknown","os_major":"2022","os_minor":"2","os_name":"Kali GNU/Linux","os_platform":"kali","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Online"},{"enrolled_on":"1662379906","hardware_cpu_logical_core":"2","hardware_cpu_type":"x86_64","hardware_model":"VirtualBox","hardware_physical_memory":"3116171264","hardware_vendor":"innotek GmbH","hardware_version":"1.2","host_identifier":"6a9e6ee0-5318-e147-93a4-1ac5f302d98b","hostname":"yelena","last_seen":1666775934,"node_invalid":"f","node_key":"BWDxS22JjzhMaiRrV41m","os_arch":"x86_64","os_build":"unknown","os_major":"20","os_minor":"04","os_name":"Ubuntu","os_platform":"ubuntu","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Offline"},{"enrolled_on":"1661258782","hardware_cpu_logical_core":"20","hardware_cpu_type":"x86_64","hardware_model":"H610M H DDR4","hardware_physical_memory":"33420242944","hardware_vendor":"Gigabyte Technology Co., Ltd.","hardware_version":"Default string","host_identifier":"035e02d8-04d3-054d-df06-e00700080009","hostname":"tcaai","last_seen":1671708763,"node_invalid":"f","node_key":"fa37JncCHryDsbzayy4c","os_arch":"x86_64","os_build":"unknown","os_major":"22","os_minor":"04","os_name":"Ubuntu","os_platform":"ubuntu","osquery_version":"5.0.1.1","reenroll_allowed":"t","status":"Online"}];
      // console.log(finalData);
      if (!finalData) return (<div className="d-flex justify-content-center align-items-center" style={{height:"100%"}}><img src={no_data} className="img-fluid" alt="Responsive image"/></div>);
      if (finalData.length <= 0) return (<div className="d-flex justify-content-center align-items-center" style={{height:"100%"}}><img src={no_data} className="img-fluid" alt="Responsive image"/></div>);
      return (
        <table className="table table-hover">
          <thead
            className="bg-white"
            style={{ position: "sticky", top: 0, zIndex: 950 }}
          >
            <tr className="fw-light text-muted">
              {/* <th className="fw-light" scope="col">
                OS
              </th>
              <th scope="row">
                <input
                  className="form-check-input flex-shrink-0"
                  type="checkbox"
                  value=""
                />
              </th> */}
              <th className="fw-light" scope="col">
                Hostname
              </th>
              <th className="fw-light" scope="col">
                Host Identifier
              </th>
              <th className="fw-light" scope="col">
                Created
              </th>
              <th className="fw-light" scope="col">
                Image Name
              </th>
              <th className="fw-light" scope="col">
                Port Binding
              </th>
              <th className="fw-light" scope="col">
                Cmd Line
              </th>
              <th className="fw-light" scope="col">
                Privileged
              </th>
              <th className="fw-light" scope="col">
                Added Capabilities
              </th>
              <th className="fw-light" scope="col">
                Container Name
              </th>
              <th className="fw-light" scope="col">
                More Info
              </th>
            </tr>
          </thead>
          <tbody className="border-top-0">
            {containerList!==undefined?containerList.map((data:any,_:number) => {
              return (
                <tr key={_}>
                  <td className="">
                    {data.hostname}
                  </td>
                  <td>
                    {data.host_identifier}
                  </td>
                  <td>
                    {timeConverter(parseInt(data.time))}
                  </td>
                  <td>{data.container_image}</td>
                  <td>{data.port_binding}</td>
                  <td>{data.cmdline}</td>
                  <td>{data.privileged}</td>
                  <td>{data.added_capabilities}</td>
                  <td>{data.container_name}</td>
                  <td>
                    <Link to="/containerInfo" state={data}>
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
            }):<></>}
          </tbody>
        </table>
      );
    }
  }

  const handleFilterChange = (event:any) => {
    setisrow(event.target.value)
  }

  const putAlertsInBucket = (e:any, alert:any) => {
    console.log(e.target.checked, alert)
    const isAlertChecked = e.target.checked;
    if(isAlertChecked){
      setSelectedAlerts([...selectedAlerts, alert])
    }else{
      let alerts = selectedAlerts.filter((ele:any) => ele.id !== alert.id)
      setSelectedAlerts(alerts)
    }
  }

  const handleCloseAlerts = () => {
    console.log(selectedAlerts)
    console.log('handle close alerts')
    // check whethere any alert is selected or not
    if(selectedAlerts.length > 0){
      // check if is there any open category alert in list
      const isAnyOpenAlerts = selectedAlerts.some((ele:any) => ele.is_open == 'f');
      console.log(isAnyOpenAlerts)
      if(!isAnyOpenAlerts){
        const modalEle = closeAlertModalRef.current
        const bsModal = new bootstrap.Modal(modalEle, {
            backdrop: 'static',
            keyboard: false
        })
        bsModal.toggle()
      }else{
        swal('', 'Please remove open category alerts', 'warning')
      }
    }else{
      swal('', 'Please select atleast 1 alert', 'warning')
    }
  }
  return (
    <>
    {
      console.log(containerList)
    }
      <div className="card card-body">
        <div className="hstack">
          <div className="d-flex flex-column justify-content-start" style={{width:"16vw"}}>
            <h3 className="fw-mid my-0">Container status</h3>
            <div>
              <div className="mt-2">
                <button
                  className="fs-8 px-1 btn btn-link text-muted"
                  onClick={() => setispast(7)}
                >
                  7 Days
                </button>
                <button
                  className="fs-8 px-1 btn btn-link text-muted active"
                  onClick={() => setispast(15)}
                >
                  15 Days
                </button>
                <button
                  className="fs-8 px-1 btn btn-link text-muted"
                  onClick={() => setispast(180)}
                >
                  180 Days
                </button>
              </div>
            </div>
          </div>
          {/* <div className="row ms-5 w-100">
            <div className="col-1 border-end">
              <div className="row d-flex justify-content-center align-items-center" style={{fontSize:"1.5rem"}}>
                Online
              </div>
              <div className="row d-flex justify-content-center align-items-center text-success" style={{fontSize:"2rem"}}>
              {machine.online}/{machine.total} 
              </div>
            </div>
            <div className="col-1">
              <div className="row d-flex justify-content-center align-items-center" style={{fontSize:"1.5rem"}}>
                Offline
              </div>
              <div className="row d-flex justify-content-center align-items-center text-danger" style={{fontSize:"2rem"}}>
              {machine.offline}/{machine.total}
              </div>
            </div>
          </div> */}
        </div>
      </div>
      <div className="card py-3">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Container List</h4>
            </div>
            <div className="ms-auto">
            </div>
          </div>
        </div>
        <div className="card-body px-6">
          <div className="d-flex flex-column align-items-start gap-3">
            <div className="d-flex justify-content-between align-items-center gap-3 w-100">
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
                        All Containers
                      </button>
                    </li>
                    <li className="nav-item" role="presentation" aria-disabled>
                      <button
                        className="nav-link"
                        id="pills-contact-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-contact"
                        type="button"
                        role="tab"
                        aria-controls="pills-contact"
                        aria-selected="false"
                        disabled={true}
                        onClick={() => {
                          if (isactive || isactive == null) {
                            setIsactive(false);
                          }
                          if (ispast != 0) {
                            setispast(0);
                          }
                        }}
                      >
                        Offline Containers
                      </button>
                    </li>
                    <li className="nav-item" role="presentation" aria-disabled>
                      <button
                        className="nav-link"
                        id="pills-contact-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-contact"
                        type="button"
                        role="tab"
                        aria-controls="pills-contact"
                        aria-selected="false"
                        disabled={true}
                        onClick={() => {
                          if (!isactive) {
                            setIsactive(true);
                          }
                          if (ispast != 0) {
                            setispast(0);
                          }
                        }}
                      >
                        Online Containers
                      </button>
                    </li>
                    {/* <li className="nav-item" role="presentation">
                      <button
                        className="nav-link"
                        id="pills-home-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-contact"
                        type="button"
                        role="tab"
                        aria-controls="pills-contact"
                        aria-selected="false"
                        disabled
                      >
                        Critical Nodes
                      </button>
                    </li> */}
                  </ul>
                </div>
              </div>
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
                  <select className="form-select-sm p-1" onChange={handleFilterChange} value={isrow} style={{border: '1px solid silver'}}>
                    <option value="25">25</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                  </select>
                </div>
                <div
                  className="modal fade"
                  id="ManageAlertsModal"
                  tabIndex={-1}
                  aria-labelledby="ManageAlertsModalLabel"
                  aria-hidden="true"
                >
                  <div className="modal-dialog">
                    <div className="modal-content">
                      <div className="modal-header">
                        <h5 className="modal-title" id="ManageAlertsModalLabel">
                          Manage Alerts
                        </h5>
                        <button
                          type="button"
                          className="btn-close"
                          data-bs-dismiss="modal"
                          aria-label="Close"
                        ></button>
                      </div>
                      <div className="modal-body">
                        <form>
                          <div className="form-floating mb-4">
                            <input
                              type="text"
                              className="form-control"
                              id="floatingInput"
                              placeholder="assignedTo"
                            />
                            <label htmlFor="floatingInput">Assigned to</label>
                          </div>
                          <div className="form-floating mb-4">
                            <input
                              type="text"
                              className="form-control"
                              id="floatingInput"
                              placeholder="Notes"
                            />
                            <label htmlFor="floatingInput">Add Notes</label>
                          </div>
                        </form>
                      </div>
                      <div className="modal-footer">
                        <button
                          type="button"
                          className="btn btn-secondary"
                          data-bs-dismiss="modal"
                        >
                          Close
                        </button>
                        <button type="button" className="btn btn-primary">
                          Save changes
                        </button>
                      </div>
                    </div>
                  </div>
                </div>
                <div className="d-flex justify-content-start align-items-center">
                  <div
                    className="modal fade"
                    ref={closeAlertModalRef}
                    id="CloseAlertModal"
                    tabIndex={-1}
                    aria-labelledby="CloseAlertModalLabel"
                    aria-hidden="true"
                  >
                    <div className="modal-dialog">
                      <div className="modal-content">
                        <div className="modal-header">
                          <h5 className="modal-title" id="CloseAlertModalLabel">
                            Are you sure you want to Close the event
                          </h5>
                          <button
                            type="button"
                            className="btn-close"
                            data-bs-dismiss="modal"
                            aria-label="Close"
                          ></button>
                        </div>
                        <div className="modal-body">
                          <form>
                            <div className="form-floating mb-4">
                              <input
                                type="text"
                                className="form-control"
                                id="floatingInput"
                                placeholder="comment"
                              />
                              <label htmlFor="floatingInput">Comment</label>
                            </div>
                          </form>
                        </div>
                        <div className="modal-footer">
                          <button
                            type="button"
                            className="btn btn-secondary"
                            data-bs-dismiss="modal"
                          >
                            Close
                          </button>
                          <button type="button" className="btn btn-primary">
                            Save changes
                          </button>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
            <div
              className="w-100 overflow-auto"
              style={{ height: "calc(100vh - 27rem)" }}
              id="#scrollbar-1"
            >
              {getResults()}
            </div>
          </div>
        </div>
      </div>
    </>
  );
};

export default NodeTable;
