import React, { FC, useCallback, useEffect, useRef, useState } from "react";
import { useLocation } from "react-router-dom";
import { fetchEventGraph, fetchEventMetaData, getData, updateMe } from "../apis/requests";
// import ThreatHunting from "../pages/threat-hunting"
import { Event, graphFilerts } from "../types";
import { timeConverter } from "../helpers";
import ReactTooltip from "react-tooltip";
import Table from 'react-bootstrap/Table';
import { ASSIGN_ALERT_USER_URL, EVENTS_GRAPH_URL} from "../apis/urls";
import Button from '@mui/material/Button';
// import { Tree, TreeNode } from 'react-organizational-chart';
import Tree from 'react-d3-tree';
import RectangleIcon from '@mui/icons-material/Rectangle';
import { Chip, Stack } from "@mui/material";

interface AlertSheetProps {
  event: Event;
}
// const AlertSheetView: FC<AlertSheetProps> = (props: AlertSheetProps) => {
//     const { event } = props;
const AlertSheetView = () => {
  const [graphData, setGraphData] = useState<any>();
  const [assignedUser, setAssignedUser] = useState<any>(undefined);
  const [forwardProcessTree, setForwardProcessTree] = useState<any>(undefined)
  const [color, setColor] = useState<boolean>(false)
  const [graphFilerts, setGraphFilerts] = useState<graphFilerts>({
    file_event:false,
    socket_event:false
  })
  const [errors, setErrors] = useState({
    graphError : ''
  })
  const location = useLocation();
  let edata: Event = location.state as Event;
  // console.log(edata.host_identifier);
  // console.log(edata.severity);
  // console.log(edata.unixtime);
  // console.log(location.state);
  var metadataObj = JSON.parse(edata.metadata);
  let signalTable:any=JSON.parse(edata.metadata).parents;
  let contextData:any={
    process_open_files:{},
    process_open_pipes:{},
    process_open_sockets:{}
  };
  JSON.parse(edata.context_data).map((e:any)=>{
    Object.keys(JSON.parse(e)).map((ky)=>{
      contextData[ky]=JSON.parse(e)[ky]
    })
  })
  // var ParObj = metadataObj.parents;
  // console.log('event data: ', edata);
  // console.log('event metadata: ', metadataObj);

  function resetColorNodes() {
    document.querySelectorAll('.rd3t-node.node__branch').forEach((component:any)=>{
      if(component.children[1].textContent.split(":")[1].trim()==metadataObj.pid && component.children[1].textContent.split(":").length===2)
      {
        component.classList.remove("malicious_node")
      }
      if(component.children[1].textContent.split(":").length===3 &&component.children[1].textContent.split(":")[2].trim()==="File Event")
      {
        component.classList.remove("File_Event")
      }
      if(component.children[1].textContent.split(":").length===3 &&component.children[1].textContent.split(":")[2].trim()==="Socket Event")
      {
        component.classList.remove("Socket_Event")
      }
    })
  }

  function colorNodes() {
    setColor(true)
    document.querySelectorAll('.rd3t-node.node__branch').forEach((component:any)=>{
      if(component.children[1].textContent.split(":")[1].trim()==metadataObj.pid && component.children[1].textContent.split(":").length===2)
      {
        component.classList.toggle("malicious_node")
      }
      if(component.children[1].textContent.split(":").length===3 &&component.children[1].textContent.split(":")[2].trim()==="File Event")
      {
        component.classList.toggle("File_Event")
      }
      if(component.children[1].textContent.split(":").length===3 &&component.children[1].textContent.split(":")[2].trim()==="Socket Event")
      {
        component.classList.toggle("Socket_Event")
      }
    })
  }
  
  useEffect(() => {
    // getGraphData();
    // console.log(metadataObj.parents[0]["pid"])
    // track=document.querySelectorAll('.rd3t-node.node__branch')//To access the div with class slide track
    getData(EVENTS_GRAPH_URL,setForwardProcessTree,'POST',{"pid":metadataObj.pid,"host_identifier":edata.host_identifier,"id":parseInt(edata.id),socket_event:graphFilerts.socket_event,file_event:graphFilerts.file_event})
    updateMe({"event_id":[edata.id]},ASSIGN_ALERT_USER_URL,setAssignedUser)
  }, [graphFilerts])
  
  const getGraphData = () => {
    let eventId = parseInt(edata.id)
    fetchEventGraph(eventId)
    .then((res) => {
      // console.log('Event Graph Success: ', res)
      res.message ? setErrors({...errors, graphError: res.message}) : setGraphData(res.data)
    })
    .catch((err) => {
      // console.log('Event Graph Error: ', err)
      setErrors({...errors, graphError: err})
    })
  }

  interface Critical{
    source:any,
    target:any
  }
  
  // const renderProcessTree = (data:any) =>{
    //   return data.map((node:any,_:number)=>{
      //     return <TreeNode key={_} label={<span className="BTreeNode">{node.pid}</span>}>
      //       {
        //         node.child?renderProcessTree(node.child):<></>
        //       }
        //     </TreeNode>
        //   })
        // }
        
        // const RenderProcessTree = (data:any) =>{
          //     return <TreeNode label={data.pid}>
          //       {
            //         data.child?RenderProcessTree(data.child):<></>
            //       }
            //     </TreeNode>
            // }
            // const useCenteredTree = (defaultTranslate = { x: 0, y: 0 }) => {
            //   const [translate, setTranslate] = useState(defaultTranslate);
            //   const containerRef = useCallback((containerElem) => {
            //     if (containerElem !== null) {
            //       const { width, height } = containerElem.getBoundingClientRect();
            //       setTranslate({ x: width / 2, y: height / 2 });
            //     }
            //   }, []);
            //   return [translate, containerRef];
            // }
            // const [translate, containerRef] = useCenteredTree();
            // const renderChild = (data:any) => {
            //   if(data){
            //     return <>
            //       <ul>
            //         <li>
            //           <a data-tip data-for={data.pid+"Child"} className={`${data.child ? '' : 'bg-danger text-light'}`}>{data.pid}</a>
            //           <i className="gg-arrow-long-right"></i>
            //           <ReactTooltip id={data.pid+"Child"} place="bottom" effect="float" type="dark">
            //             <p>PPID: {data.ppid}</p>
            //             <p>PATH: {data.path}</p>
            //           </ReactTooltip>
            //           { 
            //             data.child ? renderChild(data.child) : ''
            //           }
            //         </li>
            //       </ul>
            //       </>
            //     }
            //   }

  return (
    <>
    {
      // console.log(parseInt(edata.id),' ',metadataObj.pid)
      // console.log(parseInt(edata.id),' ',metadataObj.parents[0]["pid"])
      // console.log(parseInt(edata.id),' ',metadataObj.pid)
    }
      <div className="hstack gap-3 align-items-stretch">
        <div className="w-250px align-content-stretch">
          <div className="card card-body h-100">
            <div className="vstack">
              <div className="w-100">
                <div className="rounded-2 fw-mid bg-danger fs-6 px-4 py-2 text-white">
                  Critical Alert {edata.severity}/5
                </div>
                <div className="d-flex justify-content-start align-items-center mt-2 ms-1">
                  <p className="my-0 form-label text-muted fw-light me-1">
                    Time:
                  </p>
                  <p className="mb-0 btn-link bg-white mt-1 fw-mid text-dark btn-sm">
                    
                    {timeConverter(parseInt(edata.unixtime))}
                  </p>
                </div>
              </div>
            </div>
          </div>
        </div>
        <div className="card card-body">
          <div className="hstack">
            <div className="d-flex flex-column justify-content-start w-100">
              <div className="hstack justify-content-between">
                <h6 className="fs-7 fw-semi mt-0 mb-1">Alerts Summary</h6>
                <p className="my-0 form-label text-orange fst-italic me-1">
                  {/* Optional Update here */}
                </p>
              </div>
              <div className="hstack gap-3 justify-content-between">
                <div className="vstack gap-0">
                  <div className="d-flex justify-content-start align-items-center">
                    <p className="my-0 form-label text-muted fw-light me-1">
                      Hostname:
                    </p>
                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                      {/* Name of the host here */}
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
                      Assigned to:
                    </p>
                    <button className="btn btn-link fw-mid text-dark btn-sm">
                      {assignedUser!==undefined?assignedUser.username:("")}
                    </button>
                  </div>
                  <div className="d-flex justify-content-start align-items-center">
                    <p className="my-0 form-label text-muted fw-light me-3">
                      Alert Status:
                    </p>
                    <span className="fs-7 badge bg-teal text-white">
                      {edata.is_open ? ("Open") : ("Close")}
                    </span>
                  </div>
                </div>
                <div className="vstack gap-0 w-50">
                  <div className="d-flex justify-content-start align-items-center">
                    <p className="my-0 form-label text-muted fw-light me-1">
                      Tags:
                    </p>
                    <Stack direction="row" spacing={1}>
                    {edata.technique!=""?edata.technique.split(",").map((tag:string)=>{
                      return <Chip label={tag} color="primary" size="small" variant="outlined" component="a" href={edata.link} clickable/>
                    }):<></>}
                    </Stack>
                  </div>
                  <div className="hstack gap-6">
                    <div className="d-flex justify-content-start align-items-center">
                      <p className="my-0 form-label text-muted fw-light me-1">
                        IP Address:
                      </p>
                      <button className="btn btn-link fw-mid text-dark btn-sm">
                      {edata.address}
                      </button>
                    </div>
                    {/* <div className="d-flex justify-content-start align-items-center">
                      <p className="my-0 form-label text-muted fw-light me-1">
                      technique_id:
                      </p>
                      <button className="btn btn-link fw-mid text-dark btn-sm">
                      {edata.address}
                      </button>
                    </div> */}
                    <div className="d-flex justify-content-start align-items-center">
                      <p className="my-0 form-label text-muted fw-light me-1">
                        Version:
                      </p>
                      <button className="btn btn-link fw-mid text-dark btn-sm">
                        Linux (version)
                      </button>
                    </div>
                    {/* <div className="d-flex justify-content-start align-items-center">
                      <p className="my-0 form-label text-muted fw-light me-1">
                        link:
                      </p>
                      <button className="btn btn-link fw-mid text-dark btn-sm">
                        Linux (version)
                      </button>
                    </div> */}
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
            <div>
              <h4 className="fw-mid my-0 normalisedLink"><a href={edata.link}>{edata.event_code}</a></h4>
            </div>
            <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
              <button className="btn btn-link fw-semi text-success btn-sm">
                <i className="fi fi-rr-refresh me-2 align-middle"></i>
                Update Alert
              </button>
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
              {/* <div className="vr h-8px my-auto"></div> */}
              {/* <div className="btn-group">
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
              <div className="d-flex justify-content-start align-items-center">
                <button
                  className="btn btn-primary btn-sm align-middle"
                  type="button" data-bs-toggle="modal" data-bs-target="#exampleModal"
                >
                  <i className="fi fi-rr-add me-2 align-middle"></i>
                  Close Alert
                </button>
                  {/*  */}
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
                {/*  */}
              </div>
            </div>
          </div>
        </div>
        <div className="card-body px-5">
          <div className="d-flex flex-column align-items-start">
            <div className="d-flex justify-content-between align-items-center gap-3 w-100">
              <div className="d-flex justify-content-between align-items-center gap-0">
                <ul className="nav nav-tabs justify-content-between mx-2">
                  <li className="nav-item">
                    <a
                      aria-controls="metadata-tab"
                      aria-selected="true"
                      className="nav-link px-2 active"
                      data-bs-target="#metadata-tab"
                      data-bs-toggle="tab"
                      id="tab-1"
                      role="tab"
                      type="button"
                    >
                      Metadata
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
                      // onClick={()=>{
                      //   colorNodes()
                      // }}
                    >
                      Detection Graph
                    </a>
                  </li>
                  <li className="nav-item">
                    <a
                      aria-controls="signal-table"
                      aria-selected="false"
                      className="nav-link px-2"
                      data-bs-target="#signal-table"
                      data-bs-toggle="tab"
                      id="tab-3"
                      role="tab"
                      type="button"
                    >
                      Signal Table
                    </a>
                  </li>
                  {/* <li className="nav-item">
                    <a
                      aria-controls="run-query"
                      aria-selected="false"
                      className="nav-link px-2"
                      data-bs-target="#run-query"
                      data-bs-toggle="tab"
                      id="tab-4"
                      role="tab"
                      type="button"
                    >
                      Run Query
                    </a>
                  </li> */}
                </ul>
              </div>
            </div>
            <div
              className="tab-content card card-body px-3 py-0 w-100 overflow-auto"
              id="myTabContent"
              style={{ height: "calc(100vh - 24rem)" }}
            >
              <div
                aria-labelledby="tab-1"
                className="tab-pane h-100 fade active show"
                id="metadata-tab"
                role="tabpanel"
              >
                <div className="d-flex align-items-start align-content-stretch px-4 py-5 h-100">
                  <div className="hstack align-items-start align-content-stretch gap-6 h-100 w-100">
                    <div className="w-75">
                      <div className="d-flex flex-column justify-content-start w-100">
                        <div className="hstack justify-content-between">
                          <h6 className="fw-semi mt-0 my-2">
                            Metadata for {edata.event_code}
                          </h6>
                        </div>
                        <div className="vstack gap-3 justify-content-between">
                          <div className="vstack gap-2">
                            <div className="card mb-2">
                              <p className="me-auto ps-3 pt-3 mb-0 form-label text-muted">
                                Parents:
                              </p>
                              <ul className="metadata-parents-ul">
                                {
                                  metadataObj.parents.map((parent:any,index:any) => {
                                    return <li key={index}>
                                            <span className="badge badge-vajra badge-pill">{parent.pid}</span>
                                            <span className="path mb-0 btn-link bg-white fw-mid text-dark btn-sm">{parent.path}</span>  
                                          </li>
                                  })
                                }
                              </ul>
                            </div>
                            <div className="card mb-2">
                              <p className="me-auto ps-3 pt-3 mb-0 form-label text-muted">
                                Extra Metadata:
                              </p>
                              <p className="mb-0 btn-link fw-mid text-dark btn-sm">
                                {metadataObj.extra_metadata}
                              </p>
                            </div>
                            <div className="card">
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
                                    {/* E:main/secondary/images/picture.jpg */}
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
                            </div>
                          </div>
                        </div>
                      </div>
                    </div>
                    <div className="ms-auto card card-body">
                      <div className="vstack gap-3 justify-content-between">
                        <div className="vstack gap-2 px-1">
                          <h6 className="fw-semi mt-0 my-2">
                            Asset Information:
                          </h6>
                          <div className="d-flex justify-content-start align-items-center">
                            <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                              OS:
                            </p>
                            <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                            {edata.os_name}
                            </p>
                          </div>
                          <div className="d-flex justify-content-start align-items-center">
                            <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                              IP:
                            </p>
                            <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                            {edata.address}
                            </p>
                          </div>
                          <div className="d-flex justify-content-start align-items-center">
                            <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                              Non Compliance:
                            </p>
                            <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                              
                            </p>
                          </div>
                          <div className="d-flex justify-content-start align-items-center">
                            <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                              Detections:
                            </p>
                            <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                              
                            </p>
                          </div>
                        </div>
                        <div
                          className="accordion accordion-flush mx-n3"
                          id="accordionFlushExample"
                        >
                          <div className="accordion-item ">
                            <h4 className="accordion-header" id="flush-headingOne">
                              <button
                                className="accordion-button collapsed"
                                type="button"
                                data-bs-toggle="collapse" 
                                data-bs-target="#flush-collapseOne" 
                                aria-expanded="false" 
                                aria-controls="flush-collapseOne"
                              >
                                <h6 className="fw-semi mt-0 my-0">
                                  Files & Processes ({Object.keys(contextData.process_open_files).length>0?(contextData.process_open_files.length):("0")})
                                </h6>
                              </button>
                            </h4>
                            <div
                              id="flush-collapseOne" 
                              className="accordion-collapse collapse" 
                              aria-labelledby="flush-headingOne" 
                              data-bs-parent="#accordionFlushExample"
                            >
                              <div className="accordion-body d-flex">
                                <div className="vstack gap-2 acroItem" style={{overflowY:"auto",height:"20vh", width:"25vw"}}>
                                    {
                                      Object.keys(contextData.process_open_files).length>0?
                                        (
                                          contextData.process_open_files.map((e:any, ky:number)=>{
                                            return (
                                              <div key={ky} className="d-flex justify-content-start align-items-center">
                                              {
                                                (((e.action+":")[0]).toUpperCase()+(e.action+":").slice(1))=="Added:"?
                                              (<p className="my-0 form-label text-success fw-dark me-1 w-125px">
                                              {(((e.action+":")[0]).toUpperCase()+(e.action+":").slice(1))}
                                              </p>):(
                                                <p className="my-0 form-label text-danger fw-light me-1 w-125px">
                                                {(((e.action+":")[0]).toUpperCase()+(e.action+":").slice(1))}
                                                </p>
                                              )
                                              }
                                              <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                                {e.path}
                                              </p>
                                            </div>)
                                          }
                                          
                                        )
                                        ):(<div className="d-flex justify-content-start align-items-center">
                                          <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                            No data Available
                                          </p>
                                        </div>)
                                    }
                                  {/* <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div>
                                  <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div> */}
                                </div>
                              </div>
                            </div>
                          </div>
                          {/* <div className="accordion-item ">
                            <h4 className="accordion-header" id="flush-headingTwo">
                              <button
                                className="accordion-button collapsed" 
                                type="button" 
                                data-bs-toggle="collapse" 
                                data-bs-target="#flush-collapseTwo" 
                                aria-expanded="false" 
                                aria-controls="flush-collapseTwo"
                              >
                                <h6 className="fw-semi mt-0 my-0">
                                  Open Sockets ({Object.keys(contextData.process_open_sockets).length>0?(contextData.process_open_sockets.length):("0")})
                                </h6>
                              </button>
                            </h4>
                            <div
                              id="flush-collapseTwo" className="accordion-collapse collapse" 
                              aria-labelledby="flush-headingTwo" 
                              data-bs-parent="#accordionFlushExample"
                            >
                              <div className="accordion-body">
                                <div className="vstack gap-2">
                                    {
                                      Object.keys(contextData.process_open_sockets).length>0?
                                        (
                                          contextData.process_open_sockets.map((e:any, ky:number)=>{
                                            return (<div key={ky}>
                                            <div  className="d-flex justify-content-start align-items-center">
                                              <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                                {e.action}
                                              </p>
                                              <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                                1234567
                                              </p>
                                            </div>
                                            <div className="d-flex justify-content-start align-items-center">
                                              <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                                {e.action}
                                              </p>
                                              <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                                1234567
                                              </p>
                                            </div>
                                            </div>)
                                          }
                                          
                                        )
                                        ):(<div className="d-flex justify-content-start align-items-center">
                                          <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                            No data Available
                                          </p>
                                        </div>)
                                    }
                                  <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div>
                                  <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div>
                                </div>
                              </div>
                            </div>
                          </div> */}
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
              <div
                aria-labelledby="tab-2"
                className="tab-pane h-100 fade"
                id="graph"
                role="tabpanel"
              >
                <div className="d-flex align-items-start align-content-stretch px-4 py-5 h-100">
                  <div className="hstack align-items-start align-content-stretch gap-6 h-100 w-100">
                    <div className="w-100 h-100">
                      <div className="d-flex flex-column justify-content-start w-100">
                        <div className="hstack gap-5">
                          <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              onChange={(e:any)=>{
                                e.target.checked?setGraphFilerts(prev=>({...prev,file_event:true})):setGraphFilerts(prev=>({...prev,file_event:false}))
                                resetColorNodes()
                                setColor(false)
                                }}
                              // value=""
                              // id="flexCheckDefault"
                            />
                            <label
                              className="form-check-label"
                              htmlFor="flexCheckDefault"
                              >
                              {
                                color?<span style={{marginRight:"0.2rem",color:"#80489C"}}><RectangleIcon/></span>:<></>
                              }
                              Files (50)
                            </label>
                          </div>
                          <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              onChange={(e:any)=>{
                              e.target.checked?setGraphFilerts(prev=>({...prev,socket_event:true})):setGraphFilerts(prev=>({...prev,socket_event:false}))
                              resetColorNodes()
                              setColor(false)
                              }}
                              // value=""
                              // id="flexCheckChecked"
                              // checked
                            />
                            <label
                              className="form-check-label"
                              htmlFor="flexCheckChecked"
                            >
                              {
                                color?<span style={{marginRight:"0.2rem",color:"#FF8FB1"}}><RectangleIcon/></span>:<></>
                              }
                              Sockets (72)
                            </label>
                          </div>
                          <div className="form-check">
                            <Button variant="contained" color={color?"secondary":"inherit"} onClick={()=>{
                              // resetColorNodes()
                              colorNodes()
                            }}>Colour Nodes</Button>
                          </div>
                          
                          {/* <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              // value=""
                              // id="flexCheckChecked"
                              // checked
                            />
                            <label
                              className="form-check-label"
                              htmlFor="flexCheckChecked"
                            >
                              Process (2)
                            </label>
                          </div> */}
                          {/* <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              // value=""
                              // id="flexCheckChecked"
                              // checked
                            />
                            <label
                              className="form-check-label"
                              htmlFor="flexCheckChecked"
                            >
                              Registry (34)
                            </label>
                          </div> */}
                          {/* <div className="form-check">
                            <input
                              className="form-check-input"
                              type="checkbox"
                              // value=""
                              // id="flexCheckChecked"
                              // checked
                            />
                            <label
                              className="form-check-label"
                              htmlFor="flexCheckChecked"
                            >
                              DNS Lookup (21)
                            </label>
                          </div> */}
                        </div>
                      </div>
                      {/* <div className="detection-graph"> */}
                      <div style={{ width: '100%', height: '90%'}}>
                        {
                          forwardProcessTree!==undefined&&forwardProcessTree.Graph.length >0?
                          <Tree 
                            enableLegacyTransitions={true}
                            rootNodeClassName="node__root"
                            branchNodeClassName="node__branch"
                            leafNodeClassName="node__leaf"
                            translate={{x:100,y:200}}
                            nodeSize={{x:400,y:400}}
                            data={forwardProcessTree?.Graph} 
                          />
                          :<></>
                        }
                      </div>
                      {/* {
                          graphData != null ?
                          <div className="tree">
                            <ul>
                              <li>
                                <a>{graphData.pid} - {graphData.path}</a>
                                {
                                  graphData.child ? renderChild(graphData.child)
                                  : ''
                                }
                              </li>
                            </ul>
                          </div>
                          : <p className="text-danger mt-5 text-center fw-bold">Error : {errors.graphError}</p>
                        } */}
                        {/* {
                          graphData !== null && graphData!==undefined ?
                          <div className="tree">
                            <ul>
                              <li>
                                <a>{graphData.pid} - {graphData.path}</a>
                                {
                                  graphData.child ? renderChild(graphData.child)
                                  : ''
                                }
                              </li>
                            </ul>
                          </div>
                          : <p className="text-danger mt-5 text-center fw-bold">Error : {errors.graphError}</p>
                        } */}
                        {/* <div className="BTree">
                          <Tree label={<span className="BTreeNode">Root</span>}>
                          {
                            forwardProcessTree!==undefined?forwardProcessTree?.forward_process_tree?.map((node:any,_:number)=>{
                              return <TreeNode key={_} label={<span className="BTreeNode">{node.pid}</span>}>
                                {
                                  node.child?renderProcessTree(node.child):<></>
                                }
                              </TreeNode>
                            }):<></>
                          }
                          </Tree>
                        </div> */}
                        {/* <Tree label="root">
                        {
                          graphData!==null && graphData!==undefined?<TreeNode label={graphData.pid}>
                            {
                              graphData.child?RenderProcessTree(graphData.child):<></>
                            }
                          </TreeNode>
                          :<></>
                        }
                        </Tree> */}
                        {/* <div className="Tree">
                        <Tree label={<span className="TreeNode">A</span>} >
                          <TreeNode label={<span className="TreeNode">a</span>}>
                            <TreeNode label={<span className="TreeNode">a.1</span>}></TreeNode>
                            <TreeNode label={<span className="TreeNode">a.2</span>}></TreeNode>
                          </TreeNode>
                          <TreeNode label="b">
                          <TreeNode label="b.1"></TreeNode>
                          </TreeNode>
                          <TreeNode label="c"></TreeNode>
                        </Tree>
                        </div> */}
                      {/* </div> */}
                    </div>
                    {/* <div className="ms-auto card card-body">
                      <div className="vstack gap-3 justify-content-between">
                        <div className="vstack gap-2 px-1">
                          <h6 className="fw-semi mt-0 my-2">
                            Asset Information:
                          </h6>
                          <div className="d-flex justify-content-start align-items-center">
                            <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                              OS:
                            </p>
                            <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                            {edata.os_name}
                            </p>
                          </div>
                          <div className="d-flex justify-content-start align-items-center">
                            <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                              IP:
                            </p>
                            <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                            {edata.address}
                            </p>
                          </div>
                          <div className="d-flex justify-content-start align-items-center">
                            <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                              Non Compliance:
                            </p>
                            <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                              1234567
                            </p>
                          </div>
                          <div className="d-flex justify-content-start align-items-center">
                            <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                              Detections:
                            </p>
                            <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                              1234567
                            </p>
                          </div>
                        </div>
                        <div
                          className="accordion accordion-flush mx-n3"
                          id="accordionExample"
                        >
                          <div className="accordion-item ">
                            <h4 className="accordion-header" id="headingOne">
                              <button
                                className="accordion-button collapsed"
                                type="button"
                                data-bs-toggle="collapse"
                                data-bs-target="#collapseOne"
                                aria-expanded="false"
                                aria-controls="collapseOne"
                              >
                                <h6 className="fw-semi mt-0 my-0">
                                Files & Processes ({Object.keys(contextData.process_open_files).length>0?(contextData.process_open_files.length):("0")})
                                </h6>
                              </button>
                            </h4>
                            <div
                              id="collapseOne"
                              className="accordion-collapse collapse"
                              aria-labelledby="headingOne"
                              data-bs-parent="#accordionExample"
                            >
                              <div className="accordion-body">
                                <div className="vstack gap-2 acroItem" style={{overflowY:"auto",height:"20vh", width:"25vw"}}>
                                {
                                      Object.keys(contextData.process_open_files).length>0?
                                        (
                                          contextData.process_open_files.map((e:any, ky:number)=>{
                                            return (
                                              <div key={ky} className="d-flex justify-content-start align-items-center">
                                              {
                                                (((e.action+":")[0]).toUpperCase()+(e.action+":").slice(1))=="Added:"?
                                              (<p className="my-0 form-label text-success fw-dark me-1 w-125px">
                                              {(((e.action+":")[0]).toUpperCase()+(e.action+":").slice(1))}
                                              </p>):(
                                                <p className="my-0 form-label text-danger fw-light me-1 w-125px">
                                                {(((e.action+":")[0]).toUpperCase()+(e.action+":").slice(1))}
                                                </p>
                                              )
                                              }
                                              <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                                {e.path}
                                              </p>
                                            </div>)
                                          }
                                          
                                        )
                                        ):(<div className="d-flex justify-content-start align-items-center">
                                          <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                            No data Available
                                          </p>
                                        </div>)
                                    }
                                  <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div>
                                  <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div>
                                  <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div>
                                </div>
                              </div>
                            </div>
                          </div>
                          <div className="accordion-item ">
                            <h4 className="accordion-header" id="headingTwo">
                              <button
                                className="accordion-button collapsed"
                                type="button"
                                data-bs-toggle="collapse"
                                data-bs-target="#collapseTwo"
                                aria-expanded="false"
                                aria-controls="collapseTwo"
                              >
                                <h6 className="fw-semi mt-0 my-0">
                                  User List (6)
                                </h6>
                              </button>
                            </h4>
                            <div
                              id="collapseTwo"
                              className="accordion-collapse collapse"
                              aria-labelledby="headingTwo"
                              data-bs-parent="#accordionExample"
                            >
                              <div className="accordion-body">
                                <div className="vstack gap-2">
                                  <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div>
                                  <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div>
                                  <div className="d-flex justify-content-start align-items-center">
                                    <p className="my-0 form-label text-muted fw-light me-1 w-125px">
                                      Filler Text:
                                    </p>
                                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                                      1234567
                                    </p>
                                  </div>
                                </div>
                              </div>
                            </div>
                          </div>
                        </div>
                      </div>
                    </div> */}
                  </div>
                </div>
              </div>
              <div
                aria-labelledby="tab-3"
                className="tab-pane h-100 fade"
                id="signal-table"
                role="tabpanel"
              >
                <div className="d-flex row align-items-start align-content-start px-4 py-5 h-100">
                  <div className="d-flex justify-content-between align-items-center mb-6 gap-3 w-100">
                    <div className="d-flex justify-content-between align-items-center gap-3">
                      <i className="fi fi-rr-time-forward text-success"></i>
                      <h6 className="me-1 my-0">{signalTable.length.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",")} Signals</h6>
                      <p className="fs-7 my-0 text-muted fw-mid">
                        {/* <i className="fi fi-rr-file-check text-run mx-1 align-middle"></i> */}
                        {/* {signalTable.length.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",")} Rows */}
                      </p>
                      <p className="fs-7 my-0 text-muted fst-italic">
                        {/* Status Message here for the table rendered */}
                      </p>
                    </div>
                    <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                      <button className="btn btn-link fw-semi text-muted btn-sm">
                        {/* Feb 1, 2020 11:59 am (IST) */}
                        {Date().toLocaleString().substring(0,(Date().toLocaleString().length-21))+"(IST)"}
                      </button>
                      <div className="d-flex justify-content-start align-items-center">
                        <p className="my-0 form-label text-muted fw-light me-1">
                          Select:
                        </p>
                        <select className="form-select text-start btn btn-link fw-semi text-muted btn-sm w-100px">
                          {/* <option selected="">All</option> */}
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
                            // value=""
                            id="search_applicant"
                            placeholder="Search Applicants"
                          />
                        </form>
                      </div>
                    </div>
                  </div>
                  <div className="row">
                  <div
                        className="w-100 overflow-auto"
                        style={{ height: "calc(100vh - 30rem)" }}
                        id="#scrollbar-1"
                      >
                      {
                        signalTable.length>0?
                        (<table className="table table-responsive table-hover fs-7">
                          <thead
                            className="bg-white"
                            style={{ position: "sticky", top: 0, zIndex: 2000 }}
                          >
                            <tr className="fw-light text-muted">
                              {Object.keys(signalTable[0]).map((key:string,_:number) => {
                                return (<th key={_} className="fw-light" scope="col">
                                  {(key.replace("_"," "))[0].toUpperCase()+(key.replace("_"," ")).slice(1)}
                                </th>);
                              })}
                            </tr>
                          </thead>
                          <tbody className="border-top-0">
                            {signalTable.map((data:any,_:number) => {
                              return (
                                <tr key={_}>
                                  {
                                    Object.keys(data).map((key:string,ky:number)=>{
                                      return (<td key={ky}>{data[key]}</td>);
                                    })
                                  }
                                </tr>
                              );
                            })}
                          </tbody>
                        </table>):("")
                      }
                      </div>
                  </div>
                </div>
              </div>
              {/* <div
                aria-labelledby="tab-4"
                className="tab-pane h-100 fade"
                id="run-query"
                role="tabpanel"
              >
                <ThreatHunting runQuery={true} host_identifier={edata.host_identifier} online={edata.}/>
              </div> */}
            </div>
            {/* <!-- Modal --> */}
            <div
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
            </div>
            {/* <!-- Modal::end --> */}
          </div>
        </div>
      </div>
    </>
  );
};

export default AlertSheetView;
