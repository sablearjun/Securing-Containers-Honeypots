/* eslint-disable jsx-a11y/anchor-is-valid */
import React, { useReducer, useRef } from "react";
import TextField from "@mui/material/TextField";
import Button from "react-bootstrap/Button";
import Collapse from "react-bootstrap/Collapse";
import { useState, useEffect } from "react";
import { NODES_LIST_URL, VUL_PROGRAMS_LIST } from "../../../apis/urls";
import { useLazyFetch } from "../../../hooks";
import { Event, Node, toggle_state } from "../../../types";
import { Link } from "react-router-dom";
import loaderForTable from "../../../assets/images/loader-table.svg";
import ReactTooltip from "react-tooltip";
import { fileName, timeConverter } from "../../../helpers";
import CsvDownloadButton from "react-json-to-csv";
import { Error } from "../../../components/Error";
import { toast, ToastContainer } from "react-toastify";
import Tab from "react-bootstrap/Tab";
import Tabs from "react-bootstrap/Tabs";
import { PIEchart } from "../../../components/graph";
import { INITIAL_STATE, reducerFunc } from "../../../apis/requests/reducer";
import { getDataReducer } from "../../../apis/requests";
import Box from "@mui/material/Box";
import Typography from "@mui/material/Typography";
import Modal from "@mui/material/Modal";

const NodeTable = () => {
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
  const [softwareTab, setSoftwareTab] = useState<boolean>(false);
  const [appCount, setAppCount] = useReducer(reducerFunc, INITIAL_STATE);
  const [openVul, setOpenVul] = useState<boolean>(false);
  const [rowData, setRowData] = useState<any>({});
  const [ descSeverityOrder, setDescSeverityOrder] = useState<any>({order : true})
  

  const style = {
    position: "absolute" as "absolute",
    top: "50%",
    left: "50%",
    transform: "translate(-50%, -50%)",
    width: "80vw",
    height: "85vh",
    bgcolor: "background.paper",
    border: "2px solid #000",
    boxShadow: 24,
    p: 4,
    // zIndex: 100,
  };

  useEffect(() => {
    if (isactive == null) {
      url = NODES_LIST_URL;
    } else if (isactive) {
      url = NODES_LIST_URL + "?is_active=true";
    } else {
      url = NODES_LIST_URL + "?is_active=false";
    }
    if (SearchHostName !== null) {
      if (url.endsWith("/")) {
        url = NODES_LIST_URL + "?hostname=" + SearchHostName;
      } else {
        url = url + "&hostname=" + SearchHostName;
      }
    }
    if (SearchStartTime !== null) {
      if (typeof SearchStartTime == "object") {
        if (url.endsWith("/")) {
          url =
            NODES_LIST_URL + "?start_time=" + String(SearchStartTime.unixTime);
        } else {
          url = url + "&start_time=" + String(SearchStartTime.unixTime);
        }
      }
    }
    if (SearchEndTime !== null) {
      if (typeof SearchEndTime == "object") {
        if (url.endsWith("/")) {
          url = NODES_LIST_URL + "?end_time=" + String(SearchEndTime.unixTime);
        } else {
          url = url + "&end_time=" + String(SearchEndTime.unixTime);
        }
      }
    }
    if (ispageNo > 1) {
      url = NODES_LIST_URL;
      if (isrow !== 0) {
        url = url + "?pageno=" + ispageNo + "&rows=" + isrow;
      } else {
        url = url + "?pageno=" + ispageNo;
      }
      // is_active is for ONLINE
      if (isactive === true && !url.includes("is_active")) {
        url = url + "&is_active=true";
      }
      if (isactive === false && !url.includes("is_active")) {
        url = url + "&is_active=false";
      }
    } else if (isrow !== 0) {
      if (url.endsWith("/")) {
        url = NODES_LIST_URL + "?rows=" + isrow;
      } else {
        url = url + "&rows=" + isrow;
      }
      if (isactive && !url.includes("is_active")) {
        url = url + "&is_active=true";
      }
      if (isactive && !url.includes("is_active")) {
        url = url + "&is_active=false";
      }
    }
    run({ URL: url });
  }, [
    isactive,
    ispast,
    istoggle,
    isrow,
    ispageNo,
    SearchHostName,
    SearchEndTime,
    SearchStartTime,
  ]);

  useEffect(() => {
    if (appCount.Data === undefined) {
      getDataReducer(VUL_PROGRAMS_LIST, setAppCount);
    }
  }, []);

  const { loading, error, data, run } = useLazyFetch<{
    nodes: Node[];
    no_of_records: number;
  }>();

  const stringToArray = (rowData: any, column: string) => {
    if (rowData !== undefined && typeof rowData[column] === "string") {
      const tmp = rowData[column];
      rowData[column] = JSON.parse(tmp);
    }
    return rowData;
  };

  const handleRowClick = (e: any, rowData: any) => {
    setOpenVul(true);
    let updatedData = stringToArray(rowData, "cve_match");
    setRowData(updatedData);
  };

  const handleClose = () => {
    setOpenVul(false);
    setRowData({});
  };

  const tableResults = (data: any) => {

    return (
      <table className="table table-hover">
        <thead
          className="bg-white"
          style={{ position: "sticky", top: 0, zIndex: 950 }}
        >
          <tr className="fw-light text-muted">
            <th className="fw-light" scope="col">
              Name
            </th>
            <th className="fw-light" scope="col">
              Version
            </th>
            <th className="fw-light" scope="col">
              Publisher
            </th>
            <th className="fw-light" scope="col" onClick={()=>{setDescSeverityOrder({...descSeverityOrder, order: !descSeverityOrder.order})}} >
              Severity
              <i className={descSeverityOrder.order ? `ms-2 fa fa-arrow-down`: `ms-2 fa fa-arrow-up`}></i>
            </th>
            <th className="fw-light" scope="col">
              Affected Node Count
            </th>
            <th className="fw-light" scope="col">
              View
            </th>
          </tr>
        </thead>
        <tbody className="border-top-0">
          { (descSeverityOrder.order ? data: data?.slice().reverse())?.map((program: any, index: any) => {
            return (
              <tr key={index}>
                <td>{program.name}</td>
                <td>{program.version}</td>
                <td>{program.publisher}</td>
                <td>{program.severity}</td>
                <td>{program.nodes_count}</td>
                <td>
                  <button
                    type="button"
                    className="btn btn-primary-light btn-sm"
                    onClick={(e) => {
                      if(descSeverityOrder.order){
                        handleRowClick(e, data?.[index]);
                      }else{
                        handleRowClick(e, data?.reverse()?.[index]);
                      }
                    }}
                  >
                    <i className="fa fa-eye"></i>
                  </button>
                </td>
              </tr>
            );
          })}
        </tbody>
      </table>
    );
  };

  const softwareVulNodes = (name: string, data: any) => {
    
    // if(name === "Summary"){
    //   data = {
    //   //   High: String(Object.keys(data?.HIGH)?.length) + "/"  + String(machine?.total),
    //   // Medium: String(Object.keys(data?.MEDIUM)?.length) + "/"  + String(machine?.total),
    //   // Low:String(Object.keys(data?.LOW)?.length) + "/"  + String(machine?.total),
      
    //   }
    // }
    return (
      <>
        <div className="col d-flex justify-content-center">
          <h5>{name}</h5>
        </div>
        <div
          className="col overflow-auto styleScroll"
          style={{
            padding: "2rem",
            border: "1px solid",
            borderRadius: "1rem",
            height: "250px",
          }}
        >
          <table className="table table-hover p-5">
            <thead
              className="bg-white"
              style={{ position: "sticky", top: 0, zIndex: 950 }}
            >
              <tr className="fw-light text-muted">
                <th className="fw-light" scope="col">
                  {(name === "Summary")? 'Severity Type' : 'HostName'}
                </th>
                <th className="fw-light" scope="col">
                {(name === "Summary")? 'Host Count' : 'Count'}
                </th>
              </tr>
            </thead>

            <tbody className="border-top-0">
              {data &&
                Object.keys(data)?.map((it: string, index: any) => {
                  return (
                    <tr key={index}>
                      <td>{it}</td>
                      <td>{data?.[it]} {(name==="Summary")? `/ ${machine?.total}`:''}</td>
                    </tr>
                  );
                })}
            </tbody>
          </table>
        </div>
      </>
    );
  };

  function getResults() {
    if (loading) {
      return (
        <div className="loading-table-gif">
          <img src={loaderForTable} />
        </div>
      );
    } else if (error) {
      if (error.message === "Unauthorized") {
        <div className="">Unauthorized</div>;
        return;
      }
      return <p>{error.message}</p>;
    } else if (data) {
      const dataNodes = data.nodes;

      let finalData = dataNodes.sort((a, b) => {
        if (a.status === "Online" && b.status === "Offline") {
          return -1;
        } else if (a.status === "Offline" && b.status === "Online") {
          return 1;
        } else {
          return 0;
        }
      });
      if (
        (filter_enroll_time_btn !== false ||
          SearchHostName !== null ||
          SearchStartTime !== null ||
          SearchEndTime !== null) &&
        data.nodes.length <= 0
      ) {
        setFilter_enroll_time_btn(false);
        setSearchHostName(null);
        setSearchStartTime(null);
        setSearchEndTime(null);
        toast.error("No nodes with such filter available", {
          toastId: "no_nodes",
        });
        return;
      }

      if (data.nodes.length <= 0) return <Error message="No nodes available" />;
      if (finalData.length <= 0) return <Error message="No nodes available" />;

      if (isactive == null && SearchHostName == null) {
        let online: number = 0;
        let offline: number = 0;
        data.nodes.filter((row) => {
          if (row.status == "Online") {
            online = online + 1;
          } else {
            offline = offline + 1;
          }
        });
        machine.online = online;
        machine.offline = offline;
        machine.total = data.nodes.length;
      }

      return (
        <table className="table table-hover">
          <thead
            className="bg-white"
            style={{ position: "sticky", top: 0, zIndex: 950 }}
          >
            <tr className="fw-light text-muted">
              <th className="fw-light" scope="col">
                OS
              </th>
              <th scope="row">
                <div
                  style={{
                    height: "4vh",
                    display: "flex",
                    alignItems: "center",
                  }}
                >
                  {/* <input
                    className="form-check-input flex-shrink-0"
                    type="checkbox"
                    value=""
                  /> */}
                </div>
              </th>
              <th className="fw-light" scope="col">
                {HostName !== null ? (
                  <TextField
                    id="standard-basic"
                    label={
                      <>
                        Host Name
                        <i
                          className="fa fa-search chat-arrow-grow align-middle"
                          style={{ marginLeft: "1rem", color: "black" }}
                        ></i>
                      </>
                    }
                    variant="standard"
                    value={String(HostName)}
                    InputProps={{ disableUnderline: true }}
                    onChange={(e) => {
                      setHostName((e.target as HTMLInputElement).value);
                    }}
                    onKeyUp={(e) => {
                      if (e.key === "Enter") {
                        setSearchHostName(HostName);
                      }
                    }}
                  />
                ) : (
                  <TextField
                    id="standard-basic"
                    label={
                      <>
                        Host Name
                        <i
                          className="fa fa-search chat-arrow-grow align-middle"
                          style={{ marginLeft: "1rem", color: "black" }}
                        ></i>
                      </>
                    }
                    variant="standard"
                    InputProps={{ disableUnderline: true }}
                    onChange={(e) => {
                      setHostName((e.target as HTMLInputElement).value);
                    }}
                    onKeyUp={(e) => {
                      if (e.key === "Enter") {
                        setSearchHostName(HostName);
                      }
                    }}
                  />
                )}
              </th>
              <th className="fw-light" scope="col">
                {filter_enroll_time_btn === false ? (
                  <Button
                    onClick={() =>
                      setFilter_enroll_time_btn(!filter_enroll_time_btn)
                    }
                    // aria-controls="example1-collapse-text"
                    aria-expanded={filter_enroll_time_btn}
                    style={{
                      border: "none",
                      backgroundColor: "transparent",
                      color: "grey",
                      padding: 0,
                      outline: "none",
                      height: "3vh",
                    }}
                    className="btn_y"
                  >
                    {filter_enroll_time_btn ? (
                      ""
                    ) : (
                      <>
                        Enrolled on
                        <i
                          className="fa fa-search chat-arrow-grow align-middle"
                          style={{ marginLeft: "1rem", color: "black" }}
                        ></i>
                      </>
                    )}
                  </Button>
                ) : (
                  <></>
                )}
                <Collapse in={filter_enroll_time_btn}>
                  <div
                    id="example1-collapse-text"
                    style={{ marginTop: "0.5rem" }}
                  >
                    <div className="d-flex align-items-center">
                      {SearchStartTime !== null ? (
                        <TextField
                          id="datetime-local"
                          label="Start Date"
                          type="datetime-local"
                          defaultValue={SearchStartTime.Time}
                          sx={{ width: 250 }}
                          InputLabelProps={{
                            shrink: true,
                          }}
                          onKeyPress={(e) => {
                            if (e.key === "Enter") {
                              let time =
                                Date.parse(
                                  (e.target as HTMLInputElement).value
                                ) / 1000;
                              setSearchStartTime({
                                unixTime: time,
                                Time: (e.target as HTMLInputElement).value,
                              });
                            }
                          }}
                        />
                      ) : (
                        <TextField
                          id="datetime-local"
                          label="Start Date"
                          type="datetime-local"
                          sx={{ width: 250 }}
                          InputLabelProps={{
                            shrink: true,
                          }}
                          onKeyPress={(e) => {
                            if (e.key === "Enter") {
                              let time =
                                Date.parse(
                                  (e.target as HTMLInputElement).value
                                ) / 1000;
                              setSearchStartTime({
                                unixTime: time,
                                Time: (e.target as HTMLInputElement).value,
                              });
                            }
                          }}
                        />
                      )}
                      <span
                        className="Box"
                        onClick={() => {
                          setFilter_enroll_time_btn(!filter_enroll_time_btn);
                          setSearchStartTime(null);
                          setSearchEndTime(null);
                        }}
                        style={{
                          marginLeft: "2rem",
                          fontSize: "2rem",
                          cursor: "pointer",
                        }}
                      >
                        <i
                          className="fa fa-times-circle"
                          aria-hidden="true"
                        ></i>
                      </span>
                    </div>
                  </div>
                </Collapse>
              </th>
              <th className="fw-light" scope="col">
                <TextField
                  id="standard-basic"
                  disabled={true}
                  label="Last Seen"
                  variant="standard"
                  InputProps={{ disableUnderline: true }}
                />
              </th>
              <th className="fw-light" scope="col">
                {HostIdentifier !== null ? (
                  <TextField
                    id="standard-basic"
                    label={
                      <>
                        Host Identifier
                        <i
                          className="fa fa-search chat-arrow-grow align-middle"
                          style={{ marginLeft: "1rem", color: "black" }}
                        ></i>
                      </>
                    }
                    variant="standard"
                    value={String(HostIdentifier)}
                    InputProps={{ disableUnderline: true }}
                    onChange={(e) => {
                      setHostIdentifier((e.target as HTMLInputElement).value);
                    }}
                    onKeyUp={(e) => {
                      if (e.key === "Enter") {
                        setSearchHostName(HostIdentifier);
                      }
                    }}
                  />
                ) : (
                  <TextField
                    id="standard-basic"
                    label={
                      <>
                        Host Identifier
                        <i
                          className="fa fa-search chat-arrow-grow align-middle"
                          style={{ marginLeft: "1rem", color: "black" }}
                        ></i>
                      </>
                    }
                    variant="standard"
                    InputProps={{ disableUnderline: true }}
                    onChange={(e) => {
                      setHostIdentifier((e.target as HTMLInputElement).value);
                    }}
                    onKeyUp={(e) => {
                      if (e.key === "Enter") {
                        setSearchHostName(HostIdentifier);
                      }
                    }}
                  />
                )}
              </th>
              <th className="fw-light" scope="col">
                <TextField
                  id="standard-basic"
                  disabled={true}
                  label="Operating System"
                  variant="standard"
                  InputProps={{ disableUnderline: true }}
                />
              </th>
              <th className="fw-light" scope="col">
                <TextField
                  id="standard-basic"
                  disabled={true}
                  label="Os-Query Version"
                  variant="standard"
                  InputProps={{ disableUnderline: true }}
                />
              </th>
              <th className="fw-light" scope="col">
                <TextField
                  id="standard-basic"
                  disabled={true}
                  label="View"
                  style={{ width: "2vw" }}
                  variant="standard"
                  InputProps={{ disableUnderline: true }}
                />
              </th>
            </tr>
          </thead>
          <tbody className="border-top-0">
            {finalData.map((data, it) => {
              return (
                <tr key={it}>
                  <td className="">
                    {data.os_platform === "darwin" ? (
                      <i
                        className="fa fa-apple"
                        data-tip
                        data-for={data.node_key + "OS"}
                        style={{ fontSize: "2rem" }}
                        aria-hidden="true"
                      ></i>
                    ) : data.os_platform === "windows" ? (
                      <i
                        className="fa fa-windows"
                        data-tip
                        data-for={data.node_key + "OS"}
                        style={{ fontSize: "2rem" }}
                        aria-hidden="true"
                      ></i>
                    ) : (
                      <i
                        data-tip
                        data-for={data.node_key + "OS"}
                        className="fa fa-linux"
                        style={{ fontSize: "2rem" }}
                        aria-hidden="true"
                      ></i>
                    )}
                    <ReactTooltip
                      id={data.node_key + "OS"}
                      place="right"
                      effect="float"
                      type="dark"
                    >
                      <p>
                        {data.os_name +
                          " " +
                          data.os_arch +
                          " " +
                          data.os_major +
                          "." +
                          data.os_minor}
                      </p>
                    </ReactTooltip>
                  </td>
                  <td>
                    {/* <input type="checkbox" onChange={(e) => putAlertsInBucket(e,data)}/> */}
                  </td>
                  <td>
                    {data.status == "Online" ? (
                      <i
                        className="fa fa-circle text-success"
                        style={{ marginRight: "0.5rem" }}
                        aria-hidden="true"
                      ></i>
                    ) : (
                      <i
                        className="fa fa-circle text-danger"
                        style={{ marginRight: "0.5rem" }}
                        aria-hidden="true"
                      ></i>
                    )}
                    <a style={{ cursor: "default" }}>{data.hostname}</a>
                  </td>
                  <td>{timeConverter(parseInt(data.enrolled_on))}</td>
                  <td>
                    {data.status === "Online"
                      ? "Online"
                      : timeConverter(data.last_seen)}
                  </td>
                  <td>{data.host_identifier}</td>
                  <td>
                    {data.os_name +
                      " " +
                      data.os_arch +
                      " " +
                      data.os_major +
                      "." +
                      data.os_minor}
                  </td>
                  <td>{data.osquery_version}</td>
                  <td>
                    <Link to="/nodeSheetView" state={data}>
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
      );
    }

    return null;
  }

  const handleFilterChange = (event: any) => {
    setisrow(event.target.value);
    setispageNo(1);
  };

  return (
    <>
      <ToastContainer />
      <Modal
        open={openVul}
        onClose={handleClose}
        aria-labelledby="modal-modal-title"
        aria-describedby="modal-modal-description"
      >
        <Box sx={style} className="overflow-auto">
          <div className="d-flex flex-md-row flex-column">
            <div className="col-12 col-md-6">
              <Typography id="modal-modal-title" variant="h5" component="h2">
                <span className="text-primary">Program: </span>
                {rowData?.name}
              </Typography>

              <Typography id="modal-modal-description" sx={{ mt: 2 }}>
                <span className="fw-bold">Publisher: </span>
                {rowData?.publisher}
                <br />
                <span className="fw-bold">Version: </span>
                {rowData?.version}
              </Typography>
              <Typography id="modal-modal-content" sx={{ mt: 2 }}>
                <h5>Details: </h5>
                <p>
                  <span className="fw-bold">Severity: </span>
                  {rowData?.severity}
                </p>
                <p>
                  <span className="fw-bold">Affected Nodes Count: </span>
                  {rowData?.nodes_count}
                </p>
                {rowData?.nodes?.length > 0 ? (
                  <>
                    <ul>
                      {rowData?.nodes?.map((node: any) => {
                        return (
                          <li>
                            <span className="text-primary">
                              {node.hostname}
                            </span>{" "}
                            - [{node.host_identifier}]
                          </li>
                        );
                      })}
                    </ul>
                  </>
                ) : (
                  <></>
                )}
              </Typography>
            </div>
          </div>
          <div>
            <div>
              <table className="table table-hover">
                <thead className="bg-white" style={{ top: 0, zIndex: 950 }}>
                  <tr className="fw-light text-muted">
                    <th className="fw-light" scope="col">
                      CVE ID
                    </th>
                    <th className="fw-light" scope="col">
                      Published
                    </th>
                    <th className="fw-light" scope="col">
                      Last Modified
                    </th>
                    <th className="fw-light" scope="col">
                      Base Score
                    </th>
                    <th className="fw-light" scope="col">
                      Base Severity
                    </th>
                    <th className="fw-light" scope="col">
                      Reference
                    </th>
                  </tr>
                </thead>
                <tbody className="border-top-0">
                  {rowData?.cve_match?.map((row: any, index: any) => {
                    return (
                      <tr key={index}>
                        <td>{row.cve_id}</td>
                        <td>{row.published}</td>
                        <td>{row.lastModified}</td>
                        <td>{row.baseScore}</td>
                        <td>{row.baseSeverity}</td>
                        <td>
                          {JSON.parse(row.reference)?.map((it: any) => {
                            return (
                              <>
                                <a href={it.url} target="_blank">
                                  {it.source} :- {it.url}
                                </a>
                                <br />
                              </>
                            );
                          })}
                        </td>
                      </tr>
                    );
                  })}
                </tbody>
              </table>
            </div>
          </div>
        </Box>
      </Modal>
      <div className="card py-3">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Nodes Status</h4>
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
                    <li className="nav-item" role="tab">
                      <button
                        className="nav-link active"
                        id="all_nodes"
                        data-bs-toggle="pill"
                        data-bs-target="#nodes"
                        type="button"
                        role="tab"
                        aria-controls="nodes"
                        aria-selected="true"
                        onClick={() => {
                          if (isactive === false || isactive === true) {
                            setIsactive(null);
                          }
                          setSearchHostName(null);
                          setSearchEndTime(null);
                          setSearchStartTime(null);
                          setispageNo(1);
                          setSoftwareTab(false);
                        }}
                      >
                        All Nodes
                      </button>
                    </li>
                    <li className="nav-item" role="tab">
                      <button
                        className="nav-link"
                        id="offline_nodes"
                        data-bs-toggle="pill"
                        data-bs-target="#nodes"
                        type="button"
                        role="tab"
                        aria-controls="nodes"
                        aria-selected="false"
                        onClick={() => {
                          if (isactive || isactive == null) {
                            setIsactive(false);
                          }
                          if (ispast !== 0) {
                            setispast(0);
                          }
                          setispageNo(1);
                          setSoftwareTab(false);
                        }}
                      >
                        Offline Nodes
                      </button>
                    </li>
                    <li className="nav-item" role="tab">
                      <button
                        className="nav-link"
                        id="online_nodes"
                        data-bs-toggle="pill"
                        data-bs-target="#nodes"
                        type="button"
                        role="tab"
                        aria-controls="nodes"
                        aria-selected="false"
                        onClick={() => {
                          if (!isactive) {
                            setIsactive(true);
                          }
                          if (ispast !== 0) {
                            setispast(0);
                          }
                          setispageNo(1);
                          setSoftwareTab(false);
                        }}
                      >
                        Online Nodes
                      </button>
                    </li>
                    <li className="nav-item" role="tab">
                      <button
                        className="nav-link"
                        id="critical_nodes"
                        data-bs-toggle="pill"
                        data-bs-target="#nodes"
                        type="button"
                        role="tab"
                        aria-controls="nodes"
                        aria-selected="false"
                        disabled
                      >
                        Critical Nodes
                      </button>
                    </li>
                    <li className="nav-item" role="tab">
                      <button
                        className="nav-link"
                        id="software_tab"
                        data-bs-toggle="pill"
                        data-bs-target="#software_content"
                        type="button"
                        role="tab"
                        aria-controls="software_content"
                        aria-selected="false"
                        onClick={() => setSoftwareTab(true)}
                        // disabled={true}
                      >
                        Software Vulnerabilities
                      </button>
                    </li>
                  </ul>
                </div>
              </div>
              <div>
                {softwareTab ? (
                  <></>
                ) : (
                  <>
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
                          filename={fileName("Nodes")}
                          data={data?.nodes}
                        >
                          <i className="fa fa-download me-2 align-middle"></i>
                          Export Nodes
                        </CsvDownloadButton>
                      ) : (
                        <></>
                      )}

                      <Link
                        to="/download-agent"
                        className="btn btn-sm btn-outline-secondary text-dark"
                      >
                        Download Agent
                      </Link>
                    </div>
                  </>
                )}
              </div>
            </div>
            <div
              className="w-100 overflow-auto styleScroll"
              style={{ height: "calc(100vh - 25rem)" }}
              id="#scrollbar-1"
            >
              <div
                aria-labelledby="nodes"
                className={
                  softwareTab ? `d-none` : `tab-pane h-100 active fade show`
                }
                id="nodes"
                role="tabpanel"
              >
                {getResults()}
              </div>
              <div
                aria-labelledby="software"
                className={!softwareTab ? `d-none` : `tab-pane h-100 fade`}
                id="software_content"
                role="tabpanel"
              >
                <div className="card py-3 vh-75">
                  <div className="card-header bg-white border-0 mb-3">
                    <div className="hstack">
                      <div>
                        <h6 className="fw-mid my-0 text-primary">
                          Software Vulnerabilities
                        </h6>
                      </div>
                    </div>
                  </div>

                  <Tabs
                    defaultActiveKey="windows"
                    id="uncontrolled-tab-example"
                    className="mb-3 text-primary"
                  >
                    <Tab
                      eventKey="windows"
                      title={
                        <span className="fs-7">
                          <i className="fa fa-windows"></i> &nbsp; Windows
                        </span>
                      }
                    >
                      <div className="p-4">
                        <div className="d-flex flex-column">
                          <div className="d-flex my-2 flex-column flex-md-row align-items-start justify-content-between">
                            <div className="col-12 col-md-2 m-1">
                              <PIEchart
                                name={"Vulnerable Applications"}
                                renderState={{
                                  Data: appCount?.Data?.piechart,
                                  Error: appCount.Error,
                                  Loading: appCount.Loading,
                                }}
                                dataKey={"count"}
                              />
                            </div>
                            <div className="col-12 col-md-3 m-1">
                              {softwareVulNodes(
                                "High",
                                appCount?.Data?.nodeCount?.HIGH
                              )}
                            </div>
                            <div className="col-12 col-md-3 m-1">
                              {softwareVulNodes(
                                "Medium",
                                appCount?.Data?.nodeCount?.MEDIUM
                              )}
                            </div>
                            <div className="col-12 col-md-3 m-1">
                              {/* {softwareVulNodes(
                                "Low",
                                appCount?.Data?.nodeCount?.LOW
                              )} */}
                              {softwareVulNodes("Summary", appCount?.Data?.nodeSummary)}
                            </div>
                          </div>
                          <div className="col-12 my-2">
                            <Tabs
                              defaultActiveKey="all_cves"
                              id="uncontrolled-tab-example_windows"
                              className="mb-3 text-primary"
                            >
                              <Tab
                                eventKey="all_cves"
                                title={<span className="fs-7">All</span>}
                              >
                                {tableResults(appCount?.Data?.programs)}
                              </Tab>
                              <Tab
                                eventKey="low_cves"
                                title={<span className="fs-7">Low</span>}
                              >
                                {tableResults(
                                  appCount?.Data?.programs?.filter(
                                    (it: any) => it.severity === "LOW"
                                  )
                                )}
                              </Tab>
                              <Tab
                                eventKey="medium_cves"
                                title={<span className="fs-7">Medium</span>}
                              >
                                {tableResults(
                                  appCount?.Data?.programs?.filter(
                                    (it: any) => it.severity === "MEDIUM"
                                  )
                                )}
                              </Tab>
                              <Tab
                                eventKey="high_cves"
                                title={<span className="fs-7">High</span>}
                              >
                                {tableResults(
                                  appCount?.Data?.programs?.filter(
                                    (it: any) => it.severity === "HIGH"
                                  )
                                )}
                              </Tab>
                            </Tabs>
                          </div>
                        </div>
                      </div>
                    </Tab>
                    <Tab
                      eventKey="linux"
                      title={
                        <span className="fs-7">
                          <i className="fa fa-linux"></i> &nbsp; Linux
                        </span>
                      }
                    >
                      <div className="p-7">
                        <div className="d-flex align-items-start flex-column">
                          <div className="d-flex align-items-start col-12 flex-column flex-lg-row">
                            Linux
                          </div>
                          <div className="col-12 col-md-4 mt-2">
                            {/* <PIEchart name={"Applications"} renderState={{Data: appCount?.Data?.piechart, Error: appCount.Error, Loading: appCount.Loading}} dataKey={"count"}/> */}
                          </div>
                        </div>
                      </div>
                    </Tab>
                  </Tabs>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </>
  );
};

export default NodeTable;
