import { useEffect, useReducer, useState } from "react";
import { Link, useLocation } from "react-router-dom";
import {
  NODES_QUERY_URL,
  NODES_CONFIG_URL,
  NODES_MEMORY_HEALTH_URL,
  CONFIG_LIST_URL,
  EVENTS_COUNT_URL,
  ALERT_COUNT_URL,
  WEEKLY_ALERTS_COUNT_URL,
  MITRE_ANALYSIS_URL,
  SEVERITY_COUNT_URL,
  NODE_PROGRAMS_LIST,
  AGENT_INSTALLER_URL,
} from "../apis/urls";
import {
  getData,
  getDataReducer,
  getLiveData,
  getReduceLiveData,
  updateMe,
} from "../apis/requests";
import loaderForTable from "../assets/images/loader-table.svg";
import Checkbox from "@mui/material/Checkbox";
import Button from "react-bootstrap/Button";
import ThreatHunting from "../pages/threat-hunting";
import Modal from "react-bootstrap/Modal";
import Alert from "@mui/material/Alert";
import Fade from "@mui/material/Fade";
import Table from "@mui/material/Table";
import TableBody from "@mui/material/TableBody";
import TableCell from "@mui/material/TableCell";
import TableContainer from "@mui/material/TableContainer";
import TableHead from "@mui/material/TableHead";
import TableRow from "@mui/material/TableRow";
import {
  formatDuration,
  isValidIp,
  isValidPort,
  timeConverter,
} from "../helpers";
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  ArcElement,
} from "chart.js";
import { Line } from "react-chartjs-2";
import useInterval from "../hooks";
import { INITIAL_STATE, reducerFunc } from "../apis/requests/reducer";
import { BARgraph, PIEchart } from "../components/graph";
import LoadGraph from "../components/loadGraph";
import SecondaryTable from "../components/SecondaryTable";
import { formatBytes } from "../helpers";
import { toast, ToastContainer } from "react-toastify";
import { Tab, Tabs } from "react-bootstrap";
import Typography from "@mui/material/Typography";
import ProgramsTable from "../components/SecondaryTable/programsTable";
import { data } from "../data/programs";

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  ArcElement
);

// const AlertSheetView: FC<AlertSheetProps> = (props: AlertSheetProps) => {
//     const { event } = props;
const NodeSheetView = () => {
  // const [graphData, setGraphData] = useState<any>();
  const [alertStats, setAlertStats] = useState<any>(undefined);
  const [containerList, setContainerList] = useReducer(
    reducerFunc,
    INITIAL_STATE
  );
  const [containerInfo, setContainerInfo] = useState<any>(undefined);
  const [containerName, setContainerName] = useState<string | undefined>(
    undefined
  );
  const [containerTabel, setContainerTabel] = useState<string | undefined>(
    undefined
  );
  const [loading, setLoading] = useState<boolean>(false);
  const [btnLoading, setbtnLoading] = useState<boolean>(true);
  const [alert, setAlert] = useState<boolean>(false);
  const [alertData, setAlertData] = useState<any>(undefined);
  const [memoryData, setMemoryData] = useState<any>(undefined);
  const [configBucket, setConfigBucket] = useState<any>([]);
  const [show, setShow] = useState(false);
  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);
  const [config, setConfig] = useState<any>(undefined);
  const [submit, setSubmit] = useState<boolean>(false);
  const [counter, setCounter] = useState<number>(0);
  const [eventCount, setEventCount] = useReducer(reducerFunc, INITIAL_STATE);
  const [alertCount, setAlertCount] = useReducer(reducerFunc, INITIAL_STATE);
  const [weeklyAlertsCount, setWeeklyAlertsCount] = useReducer(
    reducerFunc,
    INITIAL_STATE
  );
  const [mitreAnalysis, setMitreAnalysis] = useReducer(
    reducerFunc,
    INITIAL_STATE
  );
  const [severityCount, setSeverityCount] = useReducer(
    reducerFunc,
    INITIAL_STATE
  );
  const [buttonText, setButtonText] = useState("Isolate");
  const [nodeState, setNodeState] = useReducer(reducerFunc, INITIAL_STATE);
  const [portBlockState, setPortBlockState] = useReducer(
    reducerFunc,
    INITIAL_STATE
  );
  const [ipState, setIpState] = useReducer(reducerFunc, INITIAL_STATE);
  const [isLoading, setLoadingButton] = useState(false);
  const [programs, setPrograms] = useReducer(reducerFunc, INITIAL_STATE);
  const [portControl, setPortControl] = useState({
    protocol: "TCP",
    port: "",
    action: "Allow",
    trafficType: "Inbound",
  });
  const [ipControl, setIpControl] = useState({
    ip: "",
    action: "Block",
  });
  // const [lastSeenValue, setLastSeenValue] = useState("");

  const isolateQuery =
    "SELECT * from port_block where port='1234' and action='isolate'";
  const reenrollQuery =
    "SELECT * from port_block where port='1234' and action='reenroll'";
  const statusQuery =
    "SELECT COUNT(*) as is_isolated from port_block where reason like '%Vajra Isolate%'";

  const portBlockQuery = `SELECT * from port_block where port='${portControl.port}' and action='block'`;
  const portStatusQuery = `SELECT * from port_block where reason='Vajra Port Rule'`;
  const portAllowQuery = `SELECT * from port_block where port='${portControl.port}' and action= 'allow'`;

  const ipStatusQuery = `SELECT * from ip_block where reason='Vajra IP Rule'`;
  const ipBlockQuery = `SELECT * from ip_block where ip='${ipControl.ip}' and action='block'`;
  const ipAllowQuery = `SELECT * from ip_block where ip='${ipControl.ip}' and action='allow'`;

  const handleIsolateClick = async () => {
    setLoadingButton(true);

    await getReduceLiveData(
      edata.host_identifier,
      isolateQuery,
      setNodeState,
      3
    );
    await getReduceLiveData(
      edata.host_identifier,
      statusQuery,
      setNodeState,
      3
    );
  };

  const handleReenrollClick = async () => {
    console.log("Number of time | calling");
    setLoadingButton(true);
    await getReduceLiveData(
      edata.host_identifier,
      reenrollQuery,
      setNodeState,
      3
    );
    await getReduceLiveData(
      edata.host_identifier,
      statusQuery,
      setNodeState,
      3
    );
  };

  const handlePortControl = async () => {
    if (portControl.port.trim() === "") {
      return toast.info("Port should not be empty");
    }

    if (!isValidPort(portControl.port)) {
      console.log("Port: ", portControl.port);
      return toast.error(
        "Port should be in range [0-65536] and of format - Eg. 80 \n 443 \n 3:104"
      );
    }

    if (portControl.action === "Allow") {
      console.log("PortAllowQuery: ", portAllowQuery);
      await getReduceLiveData(
        edata.host_identifier,
        portAllowQuery,
        setPortBlockState,
        1
      );
    } else if (portControl.action === "Block") {
      await getReduceLiveData(
        edata.host_identifier,
        portBlockQuery,
        setPortBlockState,
        3
      );
    }

    toast.success("Ports updation successful", {
      toastId: "Port success",
    });
    // if (portBlockState.Data?.[0]?.status === 'Success') {
    //   return toast.error("Ports updation successful");
    // } else {
    //   return toast.error(
    //     "Ports updation not successful due to some error. Try again"
    //   );
    // }
    getReduceLiveData(
      edata.host_identifier,
      portStatusQuery,
      setPortBlockState,
      3
    );
  };

  const handleIpControl = async () => {
    if (ipControl.ip.trim() === "") {
      return toast.info("IP address should not be empty");
    }

    if (!isValidIp(ipControl.ip)) {
      return toast.error("IP should be valid IPv4 address");
    }

    // toast.info("This feature is in work in progress");
    // return;

    if (ipControl.action === "Allow") {
      console.log("IpAllowQuery: ", portAllowQuery);
      await getReduceLiveData(
        edata.host_identifier,
        ipAllowQuery,
        setIpState,
        1
      );
    } else if (ipControl.action === "Block") {
      await getReduceLiveData(
        edata.host_identifier,
        ipBlockQuery,
        setIpState,
        1
      );
    }

    toast.success("Action on IP addresses successful", {
      toastId: "IP success",
    });
    // if (portBlockState.Data?.[0]?.status === 'Success') {
    //   return toast.error("Ports updation successful");
    // } else {
    //   return toast.error(
    //     "Ports updation not successful due to some error. Try again"
    //   );
    // }

    getReduceLiveData(edata.host_identifier, ipStatusQuery, setIpState, 3);
  };

  function updateBucket(e: any) {
    if (e.target.checked) {
      if (
        configBucket.find((id: number) => {
          return id === parseInt(e.target.value);
        }) === undefined
      ) {
        setConfigBucket((prev: any) => [...prev, parseInt(e.target.value)]);
      }
    } else {
      setConfigBucket((prev: any) => [
        ...prev.filter((id: number) => id !== parseInt(e.target.value)),
      ]);
      // let tmp=configBucket
      // if(tmp.indexOf(parseInt(e.target.value)) > -1){
      //   tmp.splice(tmp.indexOf(e.target.value),1)
      //   setConfigBucket(tmp)
      // }
    }
  }
  const location = useLocation();
  let edata: any = location.state as Node;
  let lastSeenValue = formatDuration(
    Math.floor(Date.now() / 1000) - parseInt(edata?.last_seen)
  );

  useEffect(() => {
    if (edata !== null) {
      if (severityCount.Data === undefined) {
        getDataReducer(
          SEVERITY_COUNT_URL + "?host_identifier=" + edata?.host_identifier,
          setSeverityCount
        );
      }
      if (eventCount.Data === undefined) {
        getDataReducer(
          EVENTS_COUNT_URL + "?host_identifier=" + edata?.host_identifier,
          setEventCount
        );
      }
      if (alertCount.Data === undefined) {
        getDataReducer(
          ALERT_COUNT_URL + "?host_identifier=" + edata?.host_identifier,
          setAlertCount
        );
      }
      if (alertStats === undefined) {
        getData(
          NODES_QUERY_URL +
            "?host_identifier=" +
            edata?.host_identifier +
            "&no_of_records=100",
          setAlertStats
        );
        // console.log("Alert_Stats: ", alertStats);
      }
      if (alertStats !== undefined && alertStats.config) {
        let id: any = [];
        alertStats.config.split(",").map((ids: string) => {
          id.push(parseInt(ids));
        });
        // console.log("Id: ", id);
        setConfigBucket(id);
      }
      if (config === undefined) {
        getData(CONFIG_LIST_URL, setConfig);
      }
      if (weeklyAlertsCount.Data === undefined) {
        getDataReducer(
          WEEKLY_ALERTS_COUNT_URL +
            "?host_identifier=" +
            edata?.host_identifier,
          setWeeklyAlertsCount
        );
      }
      if (mitreAnalysis.Data === undefined) {
        getDataReducer(
          MITRE_ANALYSIS_URL + "?host_identifier=" + edata?.host_identifier,
          setMitreAnalysis
        );
      }
      if (edata?.status === "Online" && containerList.Data === undefined) {
        getReduceLiveData(
          edata.host_identifier,
          "select * from docker_containers",
          setContainerList,
          3
        );
      }
      if (programs?.Data === undefined) {
        // PROGRAMS_LIST_URL
        // getReduceLiveData(
        //   edata.host_identifier,
        //   "select * from programs",
        //   setPrograms,
        //   3
        // );
        getDataReducer(
          NODE_PROGRAMS_LIST +
            "?host_identifier=" +
            edata?.host_identifier +
            "&os_name=" +
            edata?.os_platform,
          setPrograms
        );
      }
      // edata.status === "Online" && portBlockState.Data === undefined
      if (portBlockState.Data === undefined) {
        getReduceLiveData(
          edata?.host_identifier,
          portStatusQuery,
          setPortBlockState,
          3
        );
      }
      if (ipState.Data === undefined) {
        getReduceLiveData(edata?.host_identifier, ipStatusQuery, setIpState, 3);
      }
      if (nodeState.Data === undefined) {
        getReduceLiveData(edata?.host_identifier, statusQuery, setNodeState, 3);
      }
    }
  }, [alertStats]);

  useInterval(async () => {
    if (counter < 5 && edata?.status === "Online") {
      setCounter((prev: any) => prev + 1);
      getData(
        NODES_MEMORY_HEALTH_URL + "?host_identifier=" + edata?.host_identifier,
        setMemoryData,
        "GET",
        {},
        setCounter
      );
    }
  }, 3000);

  return (
    <>
      <ToastContainer />
      {alert ? (
        alertData !== undefined ? (
          alertData.status === "Success" ? (
            <Fade
              in={alert}
              timeout={{ enter: 1000, exit: 1000 }}
              addEndListener={() => {
                setTimeout(() => {
                  setAlert(false);
                }, 5000);
              }}
            >
              <Alert
                onClose={() => {
                  setAlert(false);
                }}
              >
                {alertData.message}
              </Alert>
            </Fade>
          ) : (
            <Fade
              in={alert}
              timeout={{ enter: 1000, exit: 1000 }}
              addEndListener={() => {
                setTimeout(() => {
                  setAlert(false);
                }, 5000);
              }}
            >
              <Alert
                onClose={() => {
                  setAlert(false);
                }}
                severity="error"
              >
                {alertData.message}
              </Alert>
            </Fade>
          )
        ) : (
          <></>
        )
      ) : (
        <></>
      )}
      <Modal show={show} onHide={handleClose}>
        <Modal.Header closeButton>
          <Modal.Title style={{ fontWeight: 400, fontSize: "1.5rem" }}>
            Delete Node{" "}
            <span style={{ color: "#823A74" }}>{edata?.hostname}</span>
          </Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <div className="container">
            <div className="row h5">
              Are you sure you want to delete this node
            </div>
            <div className="row">Once delete you cannot undo</div>
          </div>
        </Modal.Body>
        <Modal.Footer>
          <Button variant="secondary" onClick={handleClose}>
            Close
          </Button>
          <Button variant="primary" onClick={handleClose}>
            Delete
          </Button>
        </Modal.Footer>
      </Modal>

      {edata === null ? (
        <>
          <div className="container">
            <div className="d-flex justify-content-center align-items-center">
              <div className="card p-2 bg-secondary">
                <h3> No Node data to view. Refer the list of nodes first. </h3>
              </div>
            </div>
          </div>
        </>
      ) : (
        <>
          <div className="d-flex flex-column flex-md-row gap-3 align-items-stretch">
            <div className="w-250px align-content-stretch">
              <div className="card card-body h-100">
                <div className="vstack">
                  <div className="w-100">
                    <div className="rounded-2 fw-mid fs-6 text-white">
                      {/* <Button variant="outline-secondary" style={{width:"100%",height:"100%"}} disabled={true} onClick={handleShow} size="lg">Isolate</Button>{' '} */}

                      {/* -- WIP for OS other than Windows-- */}
                      {nodeState?.Loading ? (
                        <Button
                          variant="outline-secondary"
                          style={{
                            width: "100%",
                            height: "100%",
                            backgroundColor: "#8F407F",
                          }}
                          disabled={true}
                          size="lg"
                        >
                          Processing
                        </Button>
                      ) : nodeState?.Data !== undefined &&
                        Number(nodeState?.Data?.[0]?.is_isolated) > 0 &&
                        Number(nodeState?.Data?.[0]?.is_isolated) <= 6 ? (
                        <Button
                          variant="outline-secondary"
                          style={{
                            width: "100%",
                            height: "100%",
                            backgroundColor: "#8F407F",
                          }}
                          onClick={() => {
                            handleReenrollClick();
                          }}
                          size="lg"
                          // disabled={isLoading}
                        >
                          Reenroll
                        </Button>
                      ) : (
                        <Button
                          variant="outline-secondary"
                          style={{
                            width: "100%",
                            height: "100%",
                            backgroundColor: "#8F407F",
                          }}
                          onClick={() => {
                            handleIsolateClick();
                          }}
                          size="lg"
                          disabled={nodeState?.Data === undefined}
                        >
                          Isolate
                        </Button>
                      )}
                    </div>

                    <div className="d-flex justify-content-start align-items-center mt-2 ms-1">
                      <p style={{ color: "grey", fontSize: "0.9rem" }}>
                        <i
                          className="fa fa-hand-o-right me-1"
                          style={{ color: "grey" }}
                          aria-hidden="true"
                        ></i>
                        Isolate machine from network
                      </p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
            {/* <div className="w-250px align-content-stretch">
          <div className="card card-body h-100">
            <div className="vstack">
              <div className="w-100">
                <div className="rounded-2 fw-mid fs-6 text-white">
                  <div className="row">
                    <div className="col-6 d-flex align-items-center" style={{color:"black",fontSize:"1.2rem",fontWeight:400}}>
                      Re-enroll
                    </div>
                    <div className="col-6 d-flex align-items-center">
                      <Switch color="secondary" checked={reenroll} onChange={()=>{
                        setReEnroll(!reenroll);
                        setAlert(true)
                        updateMe({
                          host_identifier:edata.host_identifier,
                          reenroll_allowed:!reenroll
                        },NODES_REENROLL_URL,setAlertData);
                        }}/>
                    </div>
                  </div>
                  <div className="row">
                    <div className="col-6 d-flex align-items-center" style={{color:"black",fontSize:"1.2rem",fontWeight:400}}>
                      Node Valid
                    </div>
                    <div className="col-6 d-flex align-items-center">
                      <Switch color="secondary"/>
                    </div>
                  </div>
                </div>
                <div className="d-flex justify-content-start align-items-center mt-2 ms-1">
                </div>
              </div>
            </div>
          </div>
        </div> */}
            <div className="card card-body">
              <div className="hstack">
                <div className="d-flex flex-column justify-content-start w-100">
                  <div className="hstack justify-content-between">
                    <h6 className="fs-7 fw-semi mt-0 mb-1">Node Summary</h6>
                    {/* <p className="my-0 form-label text-orange fst-italic me-1">
                  Optional Update here
                </p> */}
                  </div>
                  <div className="d-flex flex-column flex-md-row gap-3 justify-content-between">
                    <div className="vstack gap-0">
                      <div className="d-flex justify-content-start align-items-center">
                        <p className="my-0 form-label text-muted fw-light me-1">
                          Hostname:
                        </p>
                        <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                          {edata.hostname}
                        </p>
                      </div>
                      <div className="d-flex justify-content-start align-items-center">
                        <p className="my-0 form-label text-muted fw-light me-1">
                          Host Identifier:
                        </p>
                        <button className="btn btn-link fw-mid text-dark btn-sm">
                          {edata.host_identifier}
                        </button>
                      </div>
                    </div>
                    <div className="vstack gap-0">
                      <div className="d-flex justify-content-start align-items-center">
                        <p className="my-0 form-label text-muted fw-light me-1">
                          Enroll no:
                        </p>
                        <button className="btn btn-link fw-mid text-dark btn-sm">
                          {timeConverter(parseInt(edata.enrolled_on))}
                        </button>
                      </div>
                      <div className="d-flex justify-content-start align-items-center">
                        <p className="my-0 form-label text-muted fw-light me-3">
                          Node Status:
                        </p>
                        {edata.status === "Offline" ? (
                          <span className="fs-7 badge bg-danger text-white">
                            {edata.status}
                            {/* {edata.is_open ? ("Open") : ("Close")} */}
                          </span>
                        ) : (
                          <span className="fs-7 badge bg-teal text-white">
                            {edata.status}
                            {/* {edata.is_open ? ("Open") : ("Close")} */}
                          </span>
                        )}
                      </div>
                    </div>
                    <div className="vstack gap-0">
                      <div className="hstack gap-6">
                        <div className="d-flex justify-content-start align-items-center">
                          <p className="my-0 form-label text-muted fw-light me-1">
                            Last seen:
                          </p>
                          <button className="btn btn-link fw-mid text-dark btn-sm">
                            {edata.status === "Offline"
                              ? timeConverter(parseInt(edata.last_seen))
                              : lastSeenValue}
                          </button>
                        </div>
                      </div>
                      <div className="hstack gap-6">
                        <div className="d-flex justify-content-start align-items-center">
                          <p className="my-0 form-label text-muted fw-light me-1">
                            IP Address:
                          </p>
                          <button className="btn btn-link fw-mid text-dark btn-sm">
                            {/* {edata.address} */}
                            {alertStats?.address}
                          </button>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
          <div className="card py-3">
            <div className="card-header bg-white border-0">
              <div className="hstack">
                <div className="d-flex flex-column flex-md-row">
                  <h4 className="fw-mid my-0">{edata.hostname}</h4>
                  <h4 className="fw-light my-0 mx-2">
                    [{edata.os_name}
                    {edata.os_arch} {edata.os_major}.{edata.os_minor}]{" "}
                    <i
                      className={
                        edata.os_name.includes("indows")
                          ? "fa fa-windows fs-4"
                          : "fa fa-linux fs-4"
                      }
                    ></i>
                  </h4>
                </div>
                <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                  {/* <button className="btn btn-link fw-semi text-success btn-sm">
                <i className="fi fi-rr-refresh me-2 align-middle"></i>
                Update Node
              </button> */}
                  {/* <div className="vr h-8px my-auto"></div>
              <div className="d-flex justify-content-start align-items-center">
                <button className="btn btn-link btn-sm text-dark" type="button">
                  <i className="fi fi-rr-edit me-2 align-middle"></i>
                  Add Notes
                </button>
              </div> */}
                  {/* <div className="vr h-8px my-auto"></div>
              <div className="d-flex justify-content-start align-items-center">
                <button className="btn btn-link btn-sm text-dark" type="button">
                  <i className="fi fi-rr-user-add me-2 align-middle"></i>
                  Assign Analyst
                </button>
              </div> */}
                  {/* <div className="vr h-8px my-auto"></div>
              <div className="btn-group">
                <button
                  className="btn border-0 btn-sm btn-outline-secondary text-dark"
                  type="button"
                >
                  <i className="fi fi-rr-paper-plane me-2 align-middle"></i>
                  Manage Alert
                </button>
                <button
                  aria-expanded="false"
                  className="btn btn-sm btn-light dropdown-toggle dropdown-toggle-split"
                  data-bs-toggle="dropdown"
                  type="button"
                >
                  <span className="visually-hidden">Toggle Dropdown</span>
                </button>
                <ul className="dropdown-menu">
                  <li>
                    <a className="dropdown-item" href="#">
                      Option 1
                    </a>
                  </li>
                  <li>
                    <a className="dropdown-item" href="#">
                      Option 2
                    </a>
                  </li>
                  <li>
                    <a className="dropdown-item" href="#">
                      Option 3
                    </a>
                  </li>
                </ul>
              </div> */}
                  {/* <div className="d-flex justify-content-start align-items-center">
                <button
                  className="btn btn-primary btn-sm align-middle"
                  type="button" data-bs-toggle="modal" data-bs-target="#exampleModal"
                >
                  <i className="fi fi-rr-add me-2 align-middle"></i>
                  Close Alert
                </button>
                <div className="modal fade" id="exampleModal" tabIndex={-1} aria-labelledby="exampleModalLabel" aria-hidden="true">
                  <div className="modal-dialog">
                    <div className="modal-content">
                      <div className="modal-header">
                        <h5 className="modal-title" id="exampleModalLabel">Are you sure you want to Close the event</h5>
                        <button type="button" className="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                      </div>
                      <div className="modal-body">
                      <form >
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
                        <button type="button" className="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                        <button type="button" className="btn btn-primary">Save changes</button>
                      </div>
                    </div>
                  </div>
                </div>
              </div> */}
                </div>
              </div>
            </div>
            <div className="card-body px-5">
              <div className="d-flex flex-column align-items-start">
                {/* Tab list */}
                <div className="d-flex justify-content-between align-items-center gap-3 w-100">
                  <div className="d-flex justify-content-between align-items-center gap-0">
                    <ul className="nav nav-tabs justify-content-between mx-2">
                      <li className="nav-item">
                        <a
                          aria-controls="detail-tab"
                          aria-selected="true"
                          className="nav-link px-2 active"
                          data-bs-target="#detail-tab"
                          data-bs-toggle="tab"
                          id="tab-1"
                          role="tab"
                          type="button"
                        >
                          Details
                        </a>
                      </li>
                      <li className="nav-item">
                        <a
                          aria-controls="graph"
                          aria-selected="false"
                          className="nav-link px-2"
                          data-bs-target="#graph"
                          data-bs-toggle="tab"
                          id="tab-2"
                          role="tab"
                          type="button"
                        >
                          Graph
                        </a>
                      </li>
                      <li className="nav-item">
                        <button
                          aria-controls="container-list"
                          aria-selected="false"
                          className="nav-link px-2"
                          data-bs-target="#container-list"
                          data-bs-toggle="tab"
                          id="tab-3"
                          role="tab"
                          type="button"
                          disabled={edata.os_platform === "windows"}
                        >
                          Container List
                        </button>
                      </li>
                      <li className="nav-item">
                        <button
                          aria-controls="docker"
                          aria-selected="false"
                          className="nav-link px-2"
                          data-bs-target="#docker"
                          data-bs-toggle="tab"
                          id="tab-4"
                          role="tab"
                          type="button"
                          disabled={edata.os_platform === "windows"}
                        >
                          Container Query
                        </button>
                      </li>
                      <li className="nav-item">
                        <button
                          aria-controls="control-ports"
                          aria-selected="false"
                          className="nav-link px-2"
                          data-bs-target="#control-ports"
                          data-bs-toggle="tab"
                          id="tab-7"
                          role="tab"
                          type="button"
                          disabled={edata.status === "Offline"}
                        >
                          Port Control
                        </button>
                      </li>
                      <li className="nav-item">
                        <button
                          aria-controls="control-ip"
                          aria-selected="false"
                          className="nav-link px-2"
                          data-bs-target="#control-ip"
                          data-bs-toggle="tab"
                          id="tab-8"
                          role="tab"
                          type="button"
                          disabled={edata.status === "Offline"}
                        >
                          IP Control
                        </button>
                      </li>
                      <li className="nav-item">
                        <a
                          aria-controls="run-query"
                          aria-selected="false"
                          className="nav-link px-2"
                          data-bs-target="#run-query"
                          data-bs-toggle="tab"
                          id="tab-5"
                          role="tab"
                          type="button"
                        >
                          Run Query
                        </a>
                      </li>
                      <li className="nav-item">
                        <button
                          aria-controls="programs"
                          aria-selected="false"
                          className="nav-link px-2"
                          data-bs-target="#programs"
                          data-bs-toggle="tab"
                          id="tab-6"
                          role="tab"
                          type="button"
                        >
                          Programs
                        </button>
                      </li>
                    </ul>
                  </div>
                </div>

                {/*  Tab Content */}
                <div
                  className="tab-content card card-body px-3 py-0 w-100 overflow-auto styleScroll"
                  id="myTabContent"
                  style={{ height: "calc(100vh - 24rem)" }}
                >
                  {/* Details Panel */}
                  <div
                    aria-labelledby="tab-1"
                    className="tab-pane h-100 fade active show"
                    id="detail-tab"
                    role="tabpanel"
                  >
                    <div className="d-flex align-items-start align-content-stretch px-4 py-5 h-100">
                      <div className="d-flex flex-column flex-md-row align-items-start align-content-stretch gap-6 h-100 w-100">
                        <div className="col-12 col-md-9">
                          <div className="d-flex flex-column justify-content-start w-100">
                            {/* <div className="hstack justify-content-between">
                          <h6 className="fw-semi mt-0 my-2">
                            Metadata for {edata.event_code}
                          </h6>
                        </div> */}
                            <div className="vstack gap-3 justify-content-between">
                              <div className="vstack gap-2">
                                <div className="card">
                                  <p className="me-auto ps-3 pt-3 mb-0 form-label text-muted">
                                    Software
                                  </p>
                                  <div className="row mx-0">
                                    <div className="col-lg-4">
                                      <span className="my-0 form-label text-muted fw-light">
                                        OS name :
                                      </span>
                                      <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                        {edata.os_name} {edata.os_arch}{" "}
                                        {edata.os_major}.{edata.os_minor}
                                      </span>
                                    </div>
                                    <div className="col-lg-4">
                                      <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                        OS Build:
                                      </span>
                                      <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                        {edata.os_build}
                                      </span>
                                    </div>
                                    <div className="col-lg-4">
                                      <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                        Platform:
                                      </span>
                                      <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                        {edata.os_platform}
                                      </span>
                                    </div>
                                    <div className="col-lg-4 mb-1">
                                      <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                        OsQuery Version:
                                      </span>
                                      <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                        {edata.osquery_version}
                                      </span>
                                    </div>
                                  </div>
                                </div>
                                <div className="card">
                                  <p className="me-auto ps-3 pt-3 mb-0 form-label text-muted">
                                    Hardware
                                  </p>
                                  <div className="row mx-0">
                                    <div className="col-lg-4">
                                      <span className="my-0 form-label text-muted fw-light">
                                        Vendor:
                                      </span>
                                      <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                        {edata.hardware_vendor}
                                      </span>
                                    </div>
                                    <div className="col-lg-4">
                                      <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                        Hardware Version:
                                      </span>
                                      <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                        {edata.hardware_version}
                                      </span>
                                    </div>
                                    <div className="col-lg-4">
                                      <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                        Memory:
                                      </span>
                                      <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                        {formatBytes(
                                          edata.hardware_physical_memory
                                        )}
                                      </span>
                                    </div>
                                    <div className="col-lg-4 mb-1">
                                      <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                        Logical core present:
                                      </span>
                                      <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                        {edata.hardware_cpu_logical_core}
                                      </span>
                                    </div>
                                    <div className="col-lg-4 mb-1">
                                      <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                        CPU Architecture:
                                      </span>
                                      <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                        {edata.hardware_cpu_type}
                                      </span>
                                    </div>
                                  </div>
                                </div>
                                {alertStats !== undefined &&
                                alertStats.events.length > 0 ? (
                                  <div className="card">
                                    <p className="me-auto ps-3 pt-3 mb-0 form-label text-muted">
                                      Latest alerts from this machine
                                    </p>
                                    <TableContainer
                                      className="mb-2 styleScroll"
                                      style={{
                                        overflowY: "auto",
                                        height: "35vh",
                                      }}
                                    >
                                      <Table
                                        sx={{ minWidth: 650 }}
                                        size="small"
                                        aria-label="a dense table"
                                      >
                                        <TableHead>
                                          <TableRow>
                                            <TableCell>Alert Name</TableCell>
                                            <TableCell align="right">
                                              Severity
                                            </TableCell>
                                            <TableCell align="right">
                                              Time
                                            </TableCell>
                                            <TableCell align="right">
                                              View
                                            </TableCell>
                                          </TableRow>
                                        </TableHead>
                                        <TableBody>
                                          {alertStats.events.map(
                                            (row: any, it: number) => (
                                              <TableRow
                                                key={it}
                                                sx={{
                                                  "&:last-child td, &:last-child th":
                                                    { border: 0 },
                                                }}
                                              >
                                                <TableCell
                                                  component="th"
                                                  scope="row"
                                                >
                                                  {row.event_code}
                                                </TableCell>
                                                <TableCell align="right">
                                                  {row.severity == "4" ? (
                                                    <span className="fs-7 badge bg-danger  text-white">
                                                      High
                                                    </span>
                                                  ) : row.severity == "3" ? (
                                                    <span className="fs-7 badge bg-warning  text-white">
                                                      Mid
                                                    </span>
                                                  ) : row.severity == "2" ? (
                                                    <span className="fs-7 badge bg-orange  text-white">
                                                      Low
                                                    </span>
                                                  ) : row.severity == "1" ? (
                                                    <span className="fs-7 badge bg-teal  text-white">
                                                      Info
                                                    </span>
                                                  ) : (
                                                    <span className="fs-7 badge bg-cyan  text-white">
                                                      None
                                                    </span>
                                                  )}
                                                </TableCell>
                                                <TableCell align="right">
                                                  {timeConverter(
                                                    parseInt(row.unixtime)
                                                  )}
                                                </TableCell>
                                                <TableCell align="right">
                                                  <Link
                                                    to="/alertSheetView"
                                                    state={row}
                                                  >
                                                    <button
                                                      type="button"
                                                      className="btn btn-primary-light ntn-sm"
                                                    >
                                                      <i className="fa fa-eye"></i>
                                                    </button>
                                                  </Link>
                                                </TableCell>
                                              </TableRow>
                                            )
                                          )}
                                        </TableBody>
                                      </Table>
                                    </TableContainer>
                                  </div>
                                ) : (
                                  ""
                                )}
                                {/* <div className="card">
                              <div className="row mx-0">
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light">
                                    PID:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                    {metadataObj.pid}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                    Path:
                                  </span>
                                  <a
                                    className="mb-0 btn-link fw-mid text-primary btn-sm text-decoration-none"
                                    href=""
                                  >
                                    {metadataObj.path}
                                  </a>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                    Command Line:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                    {metadataObj.command_line}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                    CWD:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                    {metadataObj.cwd}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                    PPID:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                    {metadataObj.ppid}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                    Sys Call:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                    {metadataObj.syscall}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                    Event Time:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      {timeConverter(parseInt(metadataObj.event_time))}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                    User ID:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                  {metadataObj.user_id}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                    Group ID:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                    {metadataObj.group_id}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1 w-150px">
                                    Effective GID:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                  {metadataObj.effective_gid}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1">
                                    Effective UIDF:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                    {metadataObj.effective_uid}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <span className="my-0 form-label text-muted fw-light me-1">
                                    Owner ID:
                                  </span>
                                  <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                    {metadataObj.owner_uid}
                                  </span>
                                </div>
                                <div className="col-lg-4">
                                  <div className="d-flex justify-content-start align-items-center">
                                    <span className="my-0 form-label text-muted fw-light me-1">
                                      Owner GID:
                                    </span>
                                    <span className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      {metadataObj.owner_gid}
                                    </span>
                                  </div>
                                </div>
                              </div>
                            </div> */}
                              </div>
                            </div>
                          </div>
                        </div>
                        <div className="col-12 col-md-3 ms-md-auto card card-body">
                          {/* <div className="vstack gap-3 justify-content-between"> */}
                          <h6 className="fw-semi mt-0 my-2">
                            Node configuration:
                          </h6>
                          <div
                            className="vstack px-1 acroItem"
                            style={{
                              height: "calc(100vh - 40rem)",
                              overflowY: "scroll",
                            }}
                          >
                            {
                              // alertStats!==undefined&&alertStats.config?(alertStats.config).split(',').map((conId:any,_:number)=>{
                              config !== undefined
                                ? config?.data
                                    .filter((item: any) => {
                                      const osNames =
                                        item.os_name?.toLowerCase();
                                      let platform =
                                        edata.os_platform.toLowerCase();
                                      const arch = edata.os_arch;

                                      if (!osNames) {
                                        return true; // Include items with no specific OS requirement
                                      }

                                      if (platform === "windows") {
                                        if (
                                          arch === "64-bit" &&
                                          item.query_name === "processes32"
                                        ) {
                                          return false; // Skip items with query_name 'processes32' on 64-bit Windows
                                        }

                                        if (
                                          arch === "32-bit" &&
                                          [
                                            "win_process_events",
                                            "win_socket_events",
                                            "win_config",
                                            "win_file_events",
                                            "processes",
                                          ].includes(item.query_name)
                                        ) {
                                          return false; // Skip specific items on 32-bit Windows
                                        }
                                      }

                                      if (
                                        platform === "linux" ||
                                        platform === "ubuntu"
                                      ) {
                                        platform = "linux";
                                        if (
                                          edata.kernel_version != null &&
                                          item.kernel_version != null
                                        ) {
                                          const limit = parseFloat(
                                            item.kernel_version.substring(1)
                                          );
                                          const nodeVersionMatch =
                                            edata.kernel_version.match(
                                              /^(\d+\.\d+)/
                                            );

                                          if (nodeVersionMatch) {
                                            const nodeVersion = parseFloat(
                                              nodeVersionMatch[0]
                                            );
                                            const sign = item.kernel_version[0];

                                            switch (sign) {
                                              case ">":
                                                return (
                                                  nodeVersion > limit &&
                                                  osNames.includes(platform)
                                                );
                                              case "<":
                                                return (
                                                  nodeVersion < limit &&
                                                  osNames.includes(platform)
                                                );
                                              case "=":
                                                return (
                                                  nodeVersion === limit &&
                                                  osNames.includes(platform)
                                                );
                                              default:
                                                return false;
                                            }
                                          }
                                        }
                                      }

                                      return osNames.includes(platform);
                                    })
                                    .map((config: any, _: number) => {
                                      return (
                                        <div
                                          key={_}
                                          className="d-flex justify-content-start align-items-center"
                                        >
                                          <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                            {alertStats !== undefined &&
                                            alertStats.config ? (
                                              alertStats.config
                                                .split(",")
                                                .find((id: number) => {
                                                  return id === config.id;
                                                }) !== undefined ? (
                                                <Checkbox
                                                  key={_}
                                                  value={config.id}
                                                  color="secondary"
                                                  onChange={(e) => {
                                                    updateBucket(e);
                                                    setSubmit(true);
                                                  }}
                                                  defaultChecked
                                                />
                                              ) : (
                                                <Checkbox
                                                  onChange={(e) => {
                                                    updateBucket(e);
                                                    setSubmit(true);
                                                  }}
                                                  key={_}
                                                  value={config.id}
                                                  color="secondary"
                                                />
                                              )
                                            ) : (
                                              ""
                                            )}
                                          </p>
                                          {/* <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                              {config!==undefined?config.id:""}
                              </p> */}
                                          <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                            {config !== undefined
                                              ? config.query_name
                                              : ""}
                                          </p>
                                        </div>
                                      );
                                    })
                                : ""
                            }
                          </div>

                          <Button
                            variant="primary"
                            disabled={!submit}
                            className="mt-5"
                            onClick={() => {
                              setAlert(true);
                              updateMe(
                                {
                                  host_identifier: edata.host_identifier,
                                  config_id: configBucket,
                                },
                                NODES_CONFIG_URL,
                                setAlertData
                              );
                            }}
                          >
                            Update
                          </Button>
                        </div>
                      </div>
                    </div>
                  </div>

                  {/* Graph Panel */}
                  <div
                    aria-labelledby="tab-2"
                    className="tab-pane h-100 fade"
                    id="graph"
                    role="tabpanel"
                  >
                    <div className="d-flex align-items-start align-content-stretch px-4 py-5 h-100">
                      <div className="d-flex flex-column flex-md-row align-items-start align-content-stretch gap-6 h-100 w-100">
                        <div className="w-100 styleScroll">
                          <div className="d-flex flex-column justify-content-start bg-success">
                            {/* <div className="hstack gap-5">
                          <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              id="file"
                            />
                            <label
                              className="form-check-label"
                              htmlFor="file"
                            >
                              Files (50)
                            </label>
                          </div>
                          <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              id="socket"
                            />
                            <label
                              className="form-check-label"
                              htmlFor="socket"
                            >
                              Sockets (72)
                            </label>
                          </div>
                          <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              id="process"
                            />
                            <label
                              className="form-check-label"
                              htmlFor="process"
                            >
                              Process (2)
                            </label>
                          </div>
                          <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              id="registry"
                            />
                            <label
                              className="form-check-label"
                              htmlFor="registry"
                            >
                              Registry (34)
                            </label>
                          </div>
                          <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              id="lookup"
                              defaultChecked
                            />
                            <label
                              className="form-check-label"
                              htmlFor="lookup"
                            >
                              DNS Lookup (21)
                            </label>
                          </div>
                        </div> */}
                          </div>
                          <div className="detection-graph">
                            <div className="container vw-100">
                              <div className="row nodeGraphContainer">
                                <div className="col-12 col-sm-6 col-md-4 mt-2">
                                  <PIEchart
                                    name={"Events Status Graph"}
                                    renderState={eventCount}
                                    dataKey={"count"}
                                  />
                                </div>
                                <div className="col-12 col-sm-6 col-md-4 mt-2">
                                  <div className="col d-flex justify-content-center">
                                    <h5>Memory graph</h5>
                                  </div>
                                  <div
                                    className="col"
                                    style={{
                                      border: "1px solid",
                                      padding: "0.5rem 0.5rem 0.5rem 0",
                                      borderRadius: "1rem",
                                      height: "250px",
                                    }}
                                  >
                                    {edata.status === "Online" ? (
                                      memoryData !== undefined ? (
                                        <Line
                                          options={{
                                            animation: {
                                              duration: 2000,
                                            },
                                            scales: {
                                              xAxes: {
                                                display: false,
                                              },
                                            },
                                            responsive: true,
                                            maintainAspectRatio: false,
                                            plugins: {
                                              legend: {
                                                position: "top" as const,
                                              },
                                            },
                                          }}
                                          data={{
                                            labels:
                                              memoryData?.memory_health?.map(
                                                (data: any, _: number) => {
                                                  return _;
                                                }
                                              ),
                                            datasets: [
                                              {
                                                borderCapStyle: "round",
                                                fill: true,
                                                label: "Memory used (%)",
                                                data: memoryData?.memory_health?.map(
                                                  (data: any, _: number) => {
                                                    return 100 -
                                                      (parseInt(
                                                        memoryData
                                                          ?.memory_health[_][
                                                          "memory_free"
                                                        ]
                                                      ) /
                                                        parseInt(
                                                          memoryData
                                                            ?.memory_health[_][
                                                            "memory_total"
                                                          ]
                                                        )) *
                                                        100
                                                      ? 100 -
                                                          (parseInt(
                                                            memoryData
                                                              ?.memory_health[
                                                              _
                                                            ]["memory_free"]
                                                          ) /
                                                            parseInt(
                                                              memoryData
                                                                ?.memory_health[
                                                                _
                                                              ]["memory_total"]
                                                            )) *
                                                            100
                                                      : 0;
                                                  }
                                                ),
                                                borderColor:
                                                  "rgb(255, 99, 132)",
                                                backgroundColor:
                                                  "rgba(255, 99, 132, 0.5)",
                                              },
                                            ],
                                          }}
                                        />
                                      ) : (
                                        <LoadGraph />
                                      )
                                    ) : (
                                      <div
                                        className="col d-flex justify-content-center align-items-center"
                                        style={{
                                          height: "100%",
                                          color: "grey",
                                          fontWeight: 600,
                                          fontSize: "1.5rem",
                                        }}
                                      >
                                        Graph not available
                                      </div>
                                    )}
                                  </div>
                                </div>
                                <div className="col-12 col-sm-6 col-md-4 mt-2">
                                  <BARgraph
                                    name={"Most Alert Triggered"}
                                    renderState={alertCount}
                                    label={"Event triggered"}
                                    color={"#ABC9FF"}
                                  />
                                </div>
                                <div className="col-12 col-sm-6 col-md-4 mt-2">
                                  <BARgraph
                                    name={"MITRE ATT&CK® Analysis"}
                                    renderState={mitreAnalysis}
                                    label={"Event triggered"}
                                    color={"#ABC9FF"}
                                  />
                                </div>
                                <div className="col-12 col-sm-6 col-md-4 mt-2">
                                  <PIEchart
                                    name={"Severity"}
                                    renderState={severityCount}
                                    dataKey={"value"}
                                  />
                                  {/* <div className="col d-flex justify-content-center">
                                  <h5>Severity</h5>
                                </div>
                                <div className="col" style={{border: '1px solid',padding:'0.5rem 0.5rem 0.5rem 0', borderRadius:'1rem',height:"250px"}}>
                                  {
                                    alertStats!==undefined?alertStats.severity?
                                    (<Pie
                                    options={{
                                      responsive: true,
                                      maintainAspectRatio: false,
                                      plugins: {
                                        legend: {
                                          position: 'top' as const,
                                        }
                                      },
                                    }}
                                    data={{
                                        labels: alertStats.severity.map((data:any,_:number)=>{
                                          return seveLable[alertStats.severity[_]["severity_level"]]
                                        }),
                                        datasets: [
                                          {
                                            label: '# of Votes',
                                            data: alertStats.severity.map((data:any,_:number)=>{
                                              return alertStats.severity[_]["severity_count"]
                                            }),
                                            backgroundColor: [
                                              "#0088FE", 
                                              "#82CA9D",
                                              "#E8AA42",
                                              "#FFBB28",
                                              "#CD3636"
                                            ],
                                            borderWidth: 1,
                                          },
                                        ],
                                      }} />):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
                                  }
                                </div> */}
                                </div>
                                <div className="col-12 col-sm-6 col-md-4 mt-2">
                                  <div className="col d-flex justify-content-center">
                                    <h5>Alerts Time Graph</h5>
                                  </div>
                                  <div
                                    className="col"
                                    style={{
                                      padding: "0.5rem 0.5rem 0.5rem 0",
                                      border: "1px solid",
                                      borderRadius: "1rem",
                                      height: "250px",
                                    }}
                                  >
                                    {weeklyAlertsCount.Loading ? (
                                      <LoadGraph />
                                    ) : weeklyAlertsCount.Data !== undefined ? (
                                      <Line
                                        options={{
                                          responsive: true,
                                          maintainAspectRatio: false,
                                          plugins: {
                                            legend: {
                                              position: "top" as const,
                                            },
                                          },
                                        }}
                                        data={{
                                          labels: Object.keys(
                                            weeklyAlertsCount?.Data
                                          ),
                                          datasets: [
                                            {
                                              label: "File Alert",
                                              data: Object.keys(
                                                weeklyAlertsCount?.Data
                                              ).map(
                                                (date) =>
                                                  weeklyAlertsCount?.Data?.[
                                                    date
                                                  ]["File Alert"]
                                              ),
                                              borderColor: "#9C9EFE",
                                              backgroundColor: "#9C9EFE",
                                            },
                                            {
                                              label: "Process Alert",
                                              data: Object.keys(
                                                weeklyAlertsCount?.Data
                                              ).map(
                                                (date) =>
                                                  weeklyAlertsCount?.Data?.[
                                                    date
                                                  ]["Process Alert"]
                                              ),
                                              borderColor: "#00D7FF",
                                              backgroundColor: "#00D7FF",
                                            },
                                            {
                                              label: "Socket Alert",
                                              data: Object.keys(
                                                weeklyAlertsCount?.Data
                                              ).map(
                                                (date) =>
                                                  weeklyAlertsCount?.Data?.[
                                                    date
                                                  ]["Socket Alert"]
                                              ),
                                              borderColor: "#7DCE13",
                                              backgroundColor: "#7DCE13",
                                            },
                                          ],
                                        }}
                                      />
                                    ) : (
                                      <div
                                        className="col d-flex justify-content-center align-items-center"
                                        style={{
                                          height: "100%",
                                          color: "grey",
                                          fontWeight: 600,
                                          fontSize: "1.5rem",
                                        }}
                                      >
                                        Graph not available
                                      </div>
                                    )}
                                  </div>
                                </div>
                              </div>
                            </div>
                          </div>
                        </div>
                      </div>
                    </div>
                  </div>
                  {/* <div
                aria-labelledby="tab-3"
                className="tab-pane h-100 fade"
                id="signal-table"
                role="tabpanel"
              >
                <div className="d-flex align-items-start align-content-stretch px-4 py-5 h-100">
                  <div className="d-flex justify-content-between align-items-center gap-3 w-100">
                    <div className="d-flex justify-content-between align-items-center gap-3">
                      <i className="fi fi-rr-time-forward text-success"></i>
                      <h6 className="me-1 my-0">126 Signals from </h6>
                      <p className="fs-7 my-0 text-muted fw-mid">
                        <i className="fi fi-rr-file-check text-run mx-1 align-middle"></i>
                        2,000 Rows{" "}
                      </p>
                      <p className="fs-7 my-0 text-muted fst-italic">
                        Status Message here for the table rendered
                      </p>
                    </div>
                    <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                      <button className="btn btn-link fw-semi text-muted btn-sm">
                        Feb 1, 2020 11:59 am (IST)
                      </button>
                      <div className="d-flex justify-content-start align-items-center">
                        <p className="my-0 form-label text-muted fw-light me-1">
                          Select:
                        </p>
                        <select className="form-select text-start btn btn-link fw-semi text-muted btn-sm w-100px">
                          <option selected="">All</option>
                          <option value="1">One</option>
                          <option value="2">Two</option>
                          <option value="3">Three</option>
                        </select>
                      </div>
                      <div className="vr h-8px my-auto"></div>
                      <div className="d-flex justify-content-start align-items-center">
                        <p className="my-0 form-label text-muted fw-light me-3">
                          Severity:
                        </p>
                        <span className="fs-7 badge rounded-pill bg-teal text-white">
                          Open
                        </span>
                      </div>
                      <div className="vr h-8px my-auto"></div>
                      <div>
                        <form className="position-relative" autoComplete="off">
                          <span className="position-absolute top-50 align-middle start-00 ms-5 translate-middle ">
                            <i className="fi fi-rr-search fs-7 ms-2 align-middle text-muted"></i>
                          </span>
                          <input
                            type="text"
                            className="form-control bg-light ps-7 border-0"
                            name="search"
                            value=""
                            id="search_applicant"
                            placeholder="Search Applicants"
                          />
                        </form>
                      </div>
                    </div>
                  </div>
                </div>
              </div> */}

                  {/* Container List Panel */}
                  <div
                    aria-labelledby="tab-3"
                    className="tab-pane h-100 fade"
                    id="container-list"
                    role="tabpanel"
                  >
                    <SecondaryTable
                      tableState={containerList}
                      name={"Container"}
                      columns={[
                        "state",
                        "status",
                        "name",
                        "image",
                        "created",
                        "pid",
                        "command",
                        "path",
                        "env_variables",
                        "privileged",
                        "readonly_rootfs",
                        "started_at",
                      ]}
                      Width={20}
                      convertTime={["created"]}
                    />
                  </div>

                  {/* Container Query Panel */}
                  <div
                    aria-labelledby="tab-4"
                    className="tab-pane h-100 fade"
                    id="docker"
                    role="tabpanel"
                  >
                    <div className="px-4 py-5 h-100">
                      <div className="card-body p-0">
                        <div className="d-flex align-items-start gap-3">
                          <div className="d-flex align-items-center ms-auto gap-3 mb-3">
                            <div className="vr h-8px my-auto"></div>
                            <button
                              className="btn btn-run btn-sm fs-7 text-white align-middle w-100px"
                              onClick={() => {
                                setContainerInfo(undefined);
                                setLoading(true);
                                getLiveData(
                                  edata.host_identifier,
                                  "select * from " +
                                    containerTabel +
                                    " where id = '" +
                                    containerName +
                                    "'",
                                  setContainerInfo,
                                  setLoading
                                );
                              }}
                            >
                              Run
                            </button>
                          </div>
                        </div>
                        <div className="d-flex flex-column align-items-start gap-3">
                          <div className="row container m-0 p-0">
                            <div className="col">
                              <select
                                className="form-select"
                                value={containerName}
                                onChange={(e) => {
                                  setContainerName(e.target.value);
                                }}
                              >
                                <option value="0">Select Container</option>
                                {containerList.Loading === false ? (
                                  containerList.Data?.length > 0 ? (
                                    containerList.Data?.map(
                                      (data: any, _: number) => {
                                        return (
                                          <option key={_} value={data["id"]}>
                                            {data["image"]}
                                          </option>
                                        );
                                      }
                                    )
                                  ) : (
                                    <option value="1" disabled>
                                      No Container available
                                    </option>
                                  )
                                ) : (
                                  <option value="1" disabled>
                                    Loading ...
                                  </option>
                                )}
                              </select>
                            </div>
                            <div className="col">
                              <select
                                className="form-select acroIteam"
                                value={containerTabel}
                                onChange={(e) => {
                                  setContainerTabel(e.target.value);
                                }}
                              >
                                <option value="0">Select Table</option>
                                {[
                                  "docker_container_fs_changes",
                                  "docker_container_labels",
                                  "docker_container_networks",
                                  "docker_container_ports",
                                  "docker_container_processes",
                                  "docker_container_stats",
                                  "docker_image_history",
                                  "docker_image_labels",
                                  "docker_image_layers",
                                  "docker_images",
                                  "docker_network_labels",
                                  "docker_networks",
                                  "docker_version",
                                  "docker_volume_labels",
                                  "docker_volumes",
                                ].map((data: any, _: number) => {
                                  return (
                                    <option key={_} value={data}>
                                      {data
                                        .replaceAll("_", " ")[0]
                                        .toUpperCase() +
                                        data.replaceAll("_", " ").slice(1)}
                                    </option>
                                  );
                                })}
                              </select>
                            </div>
                          </div>
                        </div>
                        <hr className="w-100 my-4" />
                        <div className="d-flex flex-column align-items-start gap-3">
                          <div className="d-flex justify-content-between align-items-center gap-3 w-100">
                            {containerInfo !== undefined ? (
                              <div className="d-flex justify-content-between align-items-center gap-3">
                                <h6 className="me-1 my-0">
                                  Containers Information
                                </h6>
                                <p className="fs-7 my-0 text-muted fw-mid">
                                  <i className="fi fi-rr-file-check text-run mx-1 align-middle"></i>
                                  {containerInfo.length
                                    .toString()
                                    .replace(/\B(?=(\d{3})+(?!\d))/g, ",")}{" "}
                                  Rows
                                </p>
                              </div>
                            ) : (
                              ""
                            )}
                            <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                              {containerInfo !== undefined ? (
                                <button className="btn btn-link fw-semi text-muted btn-sm">
                                  {Date()
                                    .toLocaleString()
                                    .substring(
                                      0,
                                      Date().toLocaleString().length - 21
                                    ) + "(IST)"}
                                </button>
                              ) : (
                                ""
                              )}
                            </div>
                          </div>
                          <div
                            className="w-100 overflow-auto"
                            style={{ height: "calc(100vh - 36.5rem)" }}
                            id="#scrollbar-1"
                          >
                            {containerInfo !== undefined ? (
                              containerInfo.length > 0 ? (
                                <table className="table table-responsive table-hover fs-7">
                                  <thead
                                    className="bg-white"
                                    style={{
                                      position: "sticky",
                                      top: 0,
                                      zIndex: 2000,
                                    }}
                                  >
                                    <tr className="fw-light text-muted">
                                      {Object.keys(containerInfo[0]).map(
                                        (key: string, _: number) => {
                                          return (
                                            <th
                                              key={_}
                                              className="fw-light"
                                              scope="col"
                                            >
                                              {key
                                                .replace("_", " ")[0]
                                                .toUpperCase() +
                                                key.replace("_", " ").slice(1)}
                                            </th>
                                          );
                                        }
                                      )}
                                    </tr>
                                  </thead>
                                  <tbody className="border-top-0">
                                    {containerInfo.map(
                                      (data: any, _: number) => {
                                        return (
                                          <tr key={_}>
                                            {Object.keys(data).map(
                                              (key: string, ky: number) => {
                                                return (
                                                  <td key={ky}>{data[key]}</td>
                                                );
                                              }
                                            )}
                                          </tr>
                                        );
                                      }
                                    )}
                                  </tbody>
                                </table>
                              ) : (
                                ""
                              )
                            ) : loading ? (
                              <div className="d-flex justify-content-center align-items-center loading-table-gif">
                                <img src={loaderForTable} />
                              </div>
                            ) : (
                              ""
                            )}
                          </div>
                        </div>
                      </div>
                    </div>
                  </div>

                  {/* Run Query Panel */}
                  <div
                    aria-labelledby="tab-5"
                    className="tab-pane h-100 fade"
                    id="run-query"
                    role="tabpanel"
                  >
                    <div className="mt-2">
                      <ThreatHunting
                        runQuery={true}
                        host_identifier={edata.host_identifier}
                        online={edata.status}
                      />
                    </div>
                  </div>

                  {/* Programs Panel */}
                  <div
                    aria-labelledby="tab-6"
                    className="tab-pane h-100 fade"
                    id="programs"
                    role="tabpanel"
                  >
                    {edata.os_platform === "windows" ? (
                      <>
                        <h6 className="me-1 my-5">Packages</h6>
                        <Tabs
                          defaultActiveKey="programs"
                          id="uncontrolled-tab-example"
                          className="mb-3 text-primary"
                        >
                          <Tab
                            eventKey="programs"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa-brands fa-debian"></i> &nbsp;  */}
                                Programs
                              </span>
                            }
                          >
                            <div className="p-1">
                              <div className="col-12 col-sm-6 col-md-4 mt-2">
                                <PIEchart
                                  name={"Vulnerable Programs"}
                                  renderState={{
                                    Data: programs.Data?.vul_programs_piechart,
                                    Loading: programs.Loading,
                                    Error: programs.Error,
                                  }}
                                  dataKey={"count"}
                                />
                              </div>
                              <ProgramsTable
                                tableState={{
                                  Data: programs.Data?.programs,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                  // Data: data.programs,
                                  // Loading: false,
                                  // Error: "",
                                }}
                                name={"Programs"}
                                columns={[
                                  "name",
                                  "publisher",
                                  "version",
                                  "install_date",
                                  "severity",
                                  "first_reported_at",
                                  "install_location",
                                  "install_source",
                                ]}
                                // Width={40}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="choco"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                Chocolatey
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.choco,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"Chocolatey Packages"}
                                columns={[
                                  "name",
                                  "version",
                                  "revision",
                                  "status",
                                  "unixtime",
                                  "admindir",
                                  "maintainer",
                                ]}
                                convertTime={["unixtime"]}
                                Width={10}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="npm_packages"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                NPM
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.npm_packages,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"NPM Packages"}
                                columns={[
                                  "name",
                                  "version",
                                  "description",
                                  "path",
                                  "directory",
                                  "pid_with_namespace",
                                  "mount_namespace_id",
                                  "homepage",
                                  "author",
                                  "license",
                                ]}
                                Width={20}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="chrome_extensions"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                Chrome Extensions
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.chrome_extensions,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"Chrome Extensions"}
                                columns={[
                                  "name",
                                  "version",
                                  "description",
                                  "install_time",
                                  "browser_type",
                                  "path",
                                  "state",
                                  "from_webstore",
                                  "profile",
                                  "update_url",
                                  "identifier",
                                  "permissions",
                                  "manifest_hash",
                                ]}
                                convertTime={["install_time"]}
                                Width={20}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="ie_extentions"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                IE Extensions
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.ie_extensions,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"IE Extensions"}
                                columns={[
                                  "name",
                                  "registry_path",
                                  "version",
                                  "path",
                                ]}
                                // Width={20}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="firefox_addons"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                FireFox AddOns
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.firefox_addons,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"FireFox Addons"}
                                columns={[
                                  "name",
                                  "version",
                                  "description",
                                  "identifier",
                                  "type",
                                  "active",
                                  "disabled",
                                  "autoupdate",
                                  "source_url",
                                  "location",
                                  "path",
                                ]}
                                Width={20}
                              />
                            </div>
                          </Tab>
                        </Tabs>
                      </>
                    ) : (
                      <>
                        {/* <SecondaryTable tableState={programs} name={"Programs"} columns={["name","publisher","version","install_date", "status", "install_location","install_source"]} Width={10}/> */}
                        <h6 className="me-1 my-5">Packages</h6>
                        <Tabs
                          defaultActiveKey="debian"
                          id="uncontrolled-tab-example"
                          className="mb-3 text-primary"
                        >
                          <Tab
                            eventKey="debian"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa-brands fa-debian"></i> &nbsp;  */}
                                Debian
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.deb,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"Debian Packages"}
                                columns={[
                                  "name",
                                  "version",
                                  "revision",
                                  "status",
                                  "unixtime",
                                  "admindir",
                                  "maintainer",
                                ]}
                                convertTime={["unixtime"]}
                                Width={40}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="rpm"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                RPM
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.rpm,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"RPM Packages"}
                                columns={[
                                  "name",
                                  "version",
                                  "revision",
                                  "status",
                                  "unixtime",
                                  "admindir",
                                  "maintainer",
                                ]}
                                convertTime={["unixtime"]}
                                Width={10}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="npm_packages"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                NPM
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.npm_packages,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"NPM Packages"}
                                columns={[
                                  "name",
                                  "version",
                                  "description",
                                  "install_time",
                                  "unixtime",
                                  "browser_type",
                                  "from_webstore",
                                  "profile",
                                  "update_url",
                                  "path",
                                  "manifest_hash",
                                ]}
                                convertTime={["unixtime"]}
                                Width={20}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="portage_packages"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                Portage
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.portage_packages,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"Portage Packages"}
                                columns={[
                                  "package",
                                  "version",
                                  "slot",
                                  "build_time",
                                  "repository",
                                  "eapi",
                                  "size",
                                  "world",
                                ]}
                                convertTime={["build_time"]}
                                Width={20}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="chrome_extensions"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                Chrome Extensions
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.chrome_extensions,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"Chrome Extensions"}
                                columns={[
                                  "name",
                                  "version",
                                  "description",
                                  "install_time",
                                  "browser_type",
                                  "path",
                                  "state",
                                  "from_webstore",
                                  "profile",
                                  "update_url",
                                  "identifier",
                                  "permissions",
                                  "manifest_hash",
                                ]}
                                convertTime={["install_time"]}
                                Width={10}
                              />
                            </div>
                          </Tab>
                          <Tab
                            eventKey="firefox_addons"
                            title={
                              <span className="fs-7">
                                {/* <i className="fa fa-linux"></i> &nbsp;  */}
                                FireFox AddOns
                              </span>
                            }
                          >
                            <div className="p-1">
                              <SecondaryTable
                                tableState={{
                                  Data: programs.Data?.firefox_addons,
                                  Loading: programs.Loading,
                                  Error: programs.Error,
                                }}
                                name={"FireFox Addons"}
                                columns={[
                                  "name",
                                  "version",
                                  "description",
                                  "identifier",
                                  "type",
                                  "active",
                                  "disabled",
                                  "autoupdate",
                                  "source_url",
                                  "location",
                                  "path",
                                ]}
                                Width={20}
                              />
                            </div>
                          </Tab>
                        </Tabs>
                      </>
                    )}
                  </div>

                  {/* Control Ports */}
                  <div
                    aria-labelledby="tab-7"
                    className="tab-pane h-100 fade"
                    id="control-ports"
                    role="tabpanel"
                  >
                    <div className="d-flex align-items-start align-content-stretch px-4 py-5 h-100">
                      <div className="d-flex flex-column flex-md-row align-items-start align-content-stretch justify-content-center align-items-center gap-6 h-100 w-100">
                        <div className="card cardbody p-4 gap-6">
                          <div className="row">
                            <h6 className="me-1 my-0">Port Control</h6>
                          </div>
                          <div>
                            <label>Protocol:</label>
                            <select
                              className="mx-3 px-3"
                              value={portControl.protocol}
                              onChange={(e) =>
                                setPortControl({
                                  ...portControl,
                                  protocol: e.target.value,
                                })
                              }
                            >
                              <option value="TCP">TCP</option>
                              {/* <option value="UDP">UDP</option> */}
                            </select>
                          </div>
                          <div className="d-flex">
                            <div>
                              <label>Port:</label>
                              <input
                                className="mx-3 px-2 w-100px"
                                type="text"
                                value={portControl.port}
                                placeholder="eg 80 or 0:123"
                                required
                                onChange={(e) => {
                                  const inputValue = e.target.value;
                                  setPortControl({
                                    ...portControl,
                                    port: inputValue,
                                  });
                                }}
                              />
                              <i
                                title="Port Number should be either a single number in range 0-65536 [Eg. 80] or a range can be specified in format startPort:endPort [Eg. 0:1024]"
                                // style={{ background: '#f8f9fa' }}
                                className="p-2 fa fa-info ms-2 bg-primary-light"
                              ></i>
                            </div>
                          </div>
                          <div className="d-flex">
                            <div className="d-flex">
                              <label>Action:</label>
                              <div>
                                <input
                                  className="mx-3"
                                  type="radio"
                                  value="Allow"
                                  checked={portControl.action === "Allow"}
                                  onChange={(e) =>
                                    setPortControl({
                                      ...portControl,
                                      action: e.target.value,
                                    })
                                  }
                                />
                                <label>Allow</label>
                              </div>
                              <div className="d-flex">
                                <input
                                  className="mx-3"
                                  type="radio"
                                  value="Block"
                                  checked={portControl.action === "Block"}
                                  onChange={(e) =>
                                    setPortControl({
                                      ...portControl,
                                      action: e.target.value,
                                    })
                                  }
                                />
                                <label>Block</label>
                              </div>
                            </div>
                          </div>
                          {/* <div className="d-flex">
                        <div className="d-flex">
                          <label>Traffic Type:</label>
                          <div>
                            <input
                              className="mx-3"
                              type="radio"
                              value="Inbound"
                              disabled
                              checked={portControl.trafficType === "Inbound"}
                              onChange={(e) =>
                                setPortControl({
                                  ...portControl,
                                  trafficType: e.target.value,
                                })
                              }
                            />
                            <label>Inbound</label>
                          </div>
                          <div className="d-flex ">
                            <input
                              className="mx-3"
                              type="radio"
                              value="Outbound"
                              disabled
                              checked={portControl.trafficType === "Outbound"}
                              onChange={(e) =>
                                setPortControl({
                                  ...portControl,
                                  trafficType: e.target.value,
                                })
                              }
                            />
                            <label>Outbound</label>
                          </div>
                        </div>
                      </div> */}
                          <div className="d-flex justify-content-center mx-auto">
                            <button
                              className="btn btn-primary"
                              onClick={() => handlePortControl()}
                            >
                              {" "}
                              Save Changes
                            </button>
                          </div>
                        </div>
                        <div className="p-0">
                          <SecondaryTable
                            name="Port Block State"
                            tableState={portBlockState}
                            columns={["port", "action", "message", "reason"]}
                            Height={35}
                            ErrorMesg="No Rules configured."
                          />
                        </div>
                      </div>
                    </div>
                  </div>

                  {/* Ip  */}
                  <div
                    aria-labelledby="tab-8"
                    className="tab-pane h-100 fade"
                    id="control-ip"
                    role="tabpanel"
                  >
                    <div className="d-flex align-items-start align-content-stretch px-4 py-5 h-100">
                      <div className="d-flex flex-column flex-md-row align-items-start align-content-stretch justify-content-center align-items-center gap-6 h-100 w-100">
                        <div className="card cardbody p-4 gap-6">
                          <div className="row">
                            <h6 className="me-1 my-0">IP control</h6>
                          </div>
                          <div className="d-flex">
                            <div>
                              <label>IP Address:</label>
                              <input
                                className="mx-3 px-2 w-100px"
                                type="text"
                                value={ipControl.ip}
                                placeholder="eg 192.168.0.1"
                                required
                                onChange={(e) => {
                                  const inputValue = e.target.value;
                                  setIpControl({
                                    ...ipControl,
                                    ip: inputValue,
                                  });
                                }}
                              />
                              <i
                                title="Ip address should be valid IPv4 address"
                                // style={{ background: '#f8f9fa' }}
                                className="p-2 fa fa-info ms-2 bg-primary-light"
                              ></i>
                            </div>
                          </div>
                          <div className="d-flex">
                            <div className="d-flex">
                              <label>Action:</label>
                              <div>
                                <input
                                  className="mx-3"
                                  type="radio"
                                  value="Allow"
                                  checked={ipControl.action === "Allow"}
                                  onChange={(e) =>
                                    setIpControl({
                                      ...ipControl,
                                      action: e.target.value,
                                    })
                                  }
                                />
                                <label>Allow</label>
                              </div>
                              <div className="d-flex">
                                <input
                                  className="mx-3"
                                  type="radio"
                                  value="Block"
                                  checked={ipControl.action === "Block"}
                                  onChange={(e) =>
                                    setIpControl({
                                      ...ipControl,
                                      action: e.target.value,
                                    })
                                  }
                                />
                                <label>Block</label>
                              </div>
                            </div>
                          </div>
                          <div className="d-flex justify-content-center mx-auto">
                            <button
                              className="btn btn-primary"
                              onClick={() => handleIpControl()}
                            >
                              {" "}
                              Save Changes
                            </button>
                          </div>
                        </div>
                        <div className="p-0">
                          <SecondaryTable
                            name="IP State"
                            tableState={ipState}
                            Height={35}
                            ErrorMesg="No IP rules configured."
                          />
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
                {/* <!-- Modal --> */}
                {/* <div
                  aria-hidden="true"
                  aria-labelledby="exampleModalLabel"
                  className="modal fade"
                  id="exampleModal"
                  tabIndex={-1}
                >
                  <div className="modal-dialog modal-dialog-centered">
                    <div className="modal-content">
                      <div className="modal-header">
                        <h5 className="modal-title" id="exampleModalLabel">
                          Alert Details
                        </h5>
                        <button
                          aria-label="Close"
                          className="btn-close"
                          data-bs-dismiss="modal"
                          type="button"
                        ></button>
                      </div>
                      <div className="modal-body">
                        <div></div>
                      </div>
                      <div className="modal-footer">
                        <button
                          className="btn btn-light"
                          data-bs-dismiss="modal"
                          type="button"
                        >
                          Close
                        </button>
                        <button className="btn btn-primary" type="button">
                          Take Action
                        </button>
                      </div>
                    </div>
                  </div>
                </div> */}
                {/* <!-- Modal::end --> */}
              </div>
            </div>
          </div>
        </>
      )}
    </>
  );
};

export default NodeSheetView;
