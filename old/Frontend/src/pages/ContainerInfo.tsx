import React, { FC, useEffect, useRef, useState } from "react";
import { useLocation } from "react-router-dom";
import { fetchEventGraph, fetchEventMetaData, getData, getGeoIp, getLiveData } from "../apis/requests";
import { bpf_process_events, Event,toggle_state } from "../types";
import ReactTooltip from "react-tooltip";
import Ubuntu from "../assets/images/ubuntu.png"
import TextField from '@mui/material/TextField';
import Button from 'react-bootstrap/Button';
import Collapse from 'react-bootstrap/Collapse';
import { Link } from "react-router-dom";
import { PieChart, Pie, Sector, Cell, ResponsiveContainer, CartesianGrid, XAxis, YAxis } from 'recharts';
import {Legend as Legend1} from 'recharts';
import {Tooltip as Tooltip1} from 'recharts';
import {
  Chart as ChartJS,
  CategoryScale,
  PointElement,
  LinearScale,
  LineElement,
  BarElement,
  ArcElement,
  Title,
  Tooltip,
  Legend,
} from 'chart.js';
import Tree from 'react-d3-tree';
import  loaderForTable from '../assets/images/loader-table.svg'
import dataNotFound from '../assets/images/data_not_found.png'
import { Bar,Line } from 'react-chartjs-2';
import { MakeBarGraph, MakePieChart } from "../helpers";
import { EVENTS_GRAPH_URL, HISTORIC_DATA_URL, NODE_GRAPH_TABLE_URL, NODE_GRAPH_URL } from "../apis/urls";
import Card from 'react-bootstrap/Card';
import { graphFilerts } from "../types";


const ContainerInfo = () => {
  const [nodeCount, setNodeCount] = useState<any>(undefined)
  const [osCount, setOsCount] = useState<any>(undefined)
  const [isrow, setisrow] = useState(100);
  const [bpf_process_events, setBpf_process_events] = useState<any>(undefined)
  const [bpf_socket_events, setBpf_socket_events] = useState<any>(undefined)
  const [processOpenFiles, setProcessOpenFiles] = useState<any>(undefined)
  const [containerNetwork, setContainerNetwork] = useState<any>(undefined)
  const [containerFileChanges, setContainerFileChanges] = useState<any>(undefined)
  const [loading, setLoading] = useState<boolean>(false)
  const [container_stats, setContainer_stats] = useState<any>(undefined)
  const [forwardProcessTree, setForwardProcessTree] = useState<any>(undefined)
  const [graphFilerts, setGraphFilerts] = useState<graphFilerts>({
    file_event:false,
    socket_event:false
  })
  const [includeChildProcess, setIncludeChileProcess] = useState<boolean>(false)

  useEffect(() => {
    if(includeChildProcess)
    {
      getData(NODE_GRAPH_TABLE_URL,setBpf_process_events,'POST',
      {
        pid:parseInt(edata.pid),
        host_identifier:edata.host_identifier,
        socket_event:graphFilerts.socket_event,
        NameOfContainer:edata.container_name,
        file_event:graphFilerts.file_event
      })
    }
    else if(bpf_process_events===undefined)
    {
      getData(HISTORIC_DATA_URL,setBpf_process_events,"POST",{"query": "select * from bpf_process_events where parent = "+edata.pid+" and not cmdline = '' order by id desc"})
    }
    if(bpf_socket_events===undefined)
    {
      getData(HISTORIC_DATA_URL,setBpf_socket_events,"POST",{"query":"select * from bpf_socket_events where parent = "+edata.pid+" order by id desc"})
    }
    if(processOpenFiles===undefined)
    {
      getData(HISTORIC_DATA_URL,setProcessOpenFiles,"POST",{"query":"select * from process_open_files where pid = "+edata.pid+" order by id desc"})
    }
    if(containerNetwork===undefined)
    {
      getLiveData(edata.host_identifier,"select * from docker_container_networks where id = '"+edata.container_id+"'",setContainerNetwork,setLoading)
    }
    if(containerFileChanges===undefined)
    {
      getLiveData(edata.host_identifier,"select * from docker_container_fs_changes where id = '"+edata.container_id+"'",setContainerFileChanges)
    }
    if(container_stats===undefined)
    {
      getLiveData(edata.host_identifier,"select * from docker_container_stats where id = '"+edata.container_id+"'",setContainer_stats)
    }
    getData(NODE_GRAPH_URL,setForwardProcessTree,'POST',
    {
      pid:parseInt(edata.pid),
      host_identifier:edata.host_identifier,
      socket_event:graphFilerts.socket_event,
      NameOfContainer:edata.container_name,
      file_event:graphFilerts.file_event
    })
    // getGeoIp('182.57.16.34')
  }, [includeChildProcess])

  function timeConverter(UNIX_timestamp: number) {
    var a = new Date(UNIX_timestamp * 1000);
    var months = [
      "Jan",
      "Feb",
      "Mar",
      "Apr",
      "May",
      "Jun",
      "Jul",
      "Aug",
      "Sep",
      "Oct",
      "Nov",
      "Dec",
    ];
    var year = a.getFullYear();
    var month = months[a.getMonth()];
    var date = a.getDate();
    var hour = a.getHours();
    var min = a.getMinutes();
    var sec = a.getSeconds();
    var time =
      date + " " + month + " " + year + " " + hour + ":" + min + ":" + sec;
    return time;
  }
  const location = useLocation();
  let edata: bpf_process_events = location.state as bpf_process_events;
  
  function getResults(state:any,headers:Array<string>,sortIt?:boolean) {
    if (state===undefined) return (<div className="loading-table-gif " style={{height:"100%"}}><img src={loaderForTable} /></div>);
    if (!state) return (<div className="loading-table-gif " style={{height:"100%"}}><img src={dataNotFound} /></div>);
    if (state.length <= 0) return (<div className="d-flex justify-content-center align-items-center" style={{height:"100%"}}><img src={dataNotFound} className="img-fluid" alt="Responsive image"/></div>);
    return (
      <table className="table table-hover">
        <thead
          className="bg-white"
          style={{ position: "sticky", top: 0, zIndex: 950 }}
        >
          <tr className="fw-light text-muted">
            {headers.length>1?headers.map((data:string,_:number)=>{
              return <th key={_} className="fw-light" scope="col">
                {data[0].toUpperCase()+data.replaceAll("_"," ").substring(1,data.length)}
              </th>
            }):<th></th>}
          </tr>
        </thead>
        <tbody className="border-top-0">
          {state.map((data:any,_:number) => {
            return (
              <tr key={_}>
                {headers.map((ky:string,_:number)=>{
                  if(ky.includes("time"))
                  {
                    return <td key={_}>{timeConverter(parseInt(data[ky]))}</td>
                  }
                  else
                  {
                    return <td key={_}>{data[ky]}</td>
                  }
                })}
              </tr>
            );
          })}
        </tbody>
      </table>
    );
  }

  return (
    <>
    {
      console.log(bpf_process_events)
    }
      <div className="hstack gap-3 align-items-stretch">
        <div className="card card-body">
          <div className="hstack">
          <div className="row ms-5 w-100">
            <div className="col-4 border-end">
              <div className="row d-flex justify-content-center align-items-center" style={{fontSize:"2.5rem"}}>
                {edata.container_name}
              </div>
            </div>
            <div className="col-4 border-end">
              <div className="row d-flex justify-content-center align-items-center" style={{fontSize:"2.5rem"}}>
                #{(edata.container_id).substring(0,12)}
              </div>
            </div>
            <div className="col-4">
              <div className="row d-flex justify-content-center align-items-center">
                <div style={{width:"15%",height:"15%"}}>
                  <img
                  src={require('../assets/images/ubuntu.png')}
                  alt="mdo"
                  className="ms-3"
                  />
                </div>
              </div>
            </div>
          </div>
          </div>
        </div>
      </div>

      {/** <---- Process Events -----> **/}

      <div className="d-flex justify-content-start align-items-end ps-2 mt-6" style={{fontSize:"1.5rem",fontWeight:"500"}}>
        Process Events
      </div>
      <div className="card py-3 p-5">
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
                        onClick={()=>{
                          setIncludeChileProcess(false)
                        }}
                      >
                        Process List
                      </button>
                    </li>
                    <li className="nav-item ms-5" role="presentation">
                      <button
                        className="nav-link active"
                        id="pills-profile-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-home"
                        type="button"
                        role="tab"
                        aria-controls="pills-home"
                        aria-selected="true"
                        onClick={()=>{
                          setIncludeChileProcess(true)
                        }}
                      >
                        Chilid Process List
                      </button>
                    </li>
                  </ul>
                </div>
              </div>
              <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                <nav aria-label="Standard pagination example">
                  <ul className="pagination pagination-sm mb-0">
                    <li className="page-item">
                      <a className="page-link" href="#" aria-label="Previous">
                        <span aria-hidden="true">« Prev</span>{" "}
                      </a>
                    </li>
                    <li className="page-item">
                      <a className="page-link" href="#" aria-label="Next" >
                        <span aria-hidden="true">Next »</span>{" "}
                      </a>
                    </li>
                  </ul>
                </nav>
                <div className="form-group">
                  <label className="text-muted">No. of rows:&nbsp;</label>
                  <select className="form-select-sm p-1" style={{border: '1px solid silver'}}>
                    <option value="25">25</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                  </select>
                </div>
              </div>
            </div>
            <div
              className="w-100 overflow-auto"
              style={{ height: "calc(100vh - 35rem)" }}
              id="#scrollbar-1"
            >
              {getResults(bpf_process_events,["cmdline","cwd","path","syscall"],true)}
            </div>
          </div>
        </div>
      </div>
      <div className="d-flex justify-content-start align-items-end ps-2 mt-6" style={{fontSize:"1.5rem",fontWeight:"500"}}>
        Process Tree
      </div>
      <div className="card p-5" style={{ height: "calc(100vh - 10rem)" }}>
        <div style={{ width: '100%', height: '90%'}}>
          {
            forwardProcessTree!==undefined&&forwardProcessTree.length >0?
            <Tree 
              enableLegacyTransitions={true}
              rootNodeClassName="node__root"
              branchNodeClassName="node__branch"
              leafNodeClassName="node__leaf"
              translate={{x:100,y:200}}
              nodeSize={{x:400,y:400}}
              data={forwardProcessTree} 
            />
            :<></>
          }
        </div>
      </div>

      {/** <---- File Events -----> **/}

      <div className="d-flex justify-content-start align-items-end ps-2 mt-6" style={{fontSize:"1.5rem",fontWeight:"500"}}>
        File Events
      </div>
      <div className="card py-3 p-5">
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
                      >
                        File Changes List
                      </button>
                    </li>
                  </ul>
                </div>
              </div>
              <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                <nav aria-label="Standard pagination example">
                  <ul className="pagination pagination-sm mb-0">
                    <li className="page-item">
                      <a className="page-link" href="#" aria-label="Previous">
                        <span aria-hidden="true">« Prev</span>{" "}
                      </a>
                    </li>
                    <li className="page-item">
                      <a className="page-link" href="#" aria-label="Next" >
                        <span aria-hidden="true">Next »</span>{" "}
                      </a>
                    </li>
                  </ul>
                </nav>
                <div className="form-group">
                  <label className="text-muted">No. of rows:&nbsp;</label>
                  <select className="form-select-sm p-1" style={{border: '1px solid silver'}}>
                    <option value="25">25</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                  </select>
                </div>
              </div>
            </div>
            <div
              className="w-100 overflow-auto"
              style={{ height: "calc(100vh - 35rem)" }}
              id="#scrollbar-1"
            >
              {getResults(containerFileChanges,["change_type","id","path"])}
            </div>
          </div>
        </div>
      </div>

      {/** <---- Network Events -----> **/}

      <div className="d-flex justify-content-start align-items-end ps-2 mt-6" style={{fontSize:"1.5rem",fontWeight:"500"}}>
        Network Events
      </div>
      <div className="card py-3 p-5">
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
                      >
                        Network Info
                      </button>
                    </li>
                  </ul>
                </div>
              </div>
              <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                <nav aria-label="Standard pagination example">
                  <ul className="pagination pagination-sm mb-0">
                    <li className="page-item">
                      <a className="page-link" href="#" aria-label="Previous">
                        <span aria-hidden="true">« Prev</span>{" "}
                      </a>
                    </li>
                    <li className="page-item">
                      <a className="page-link" href="#" aria-label="Next" >
                        <span aria-hidden="true">Next »</span>{" "}
                      </a>
                    </li>
                  </ul>
                </nav>
                <div className="form-group">
                  <label className="text-muted">No. of rows:&nbsp;</label>
                  <select className="form-select-sm p-1" style={{border: '1px solid silver'}}>
                    <option value="25">25</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                  </select>
                </div>
              </div>
            </div>
            <div
              className="w-100 overflow-auto"
              style={{ height: "calc(100vh - 48rem)" }}
              id="#scrollbar-1"
            >
              {getResults(containerNetwork,["gateway","ip_address","ip_prefix_len","mac_address","name"])}
            </div>
          </div>
        </div>
      </div>
      <div className="card py-3 p-5">
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
                      >
                        Network Events
                      </button>
                    </li>
                  </ul>
                </div>
              </div>
              <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                <nav aria-label="Standard pagination example">
                  <ul className="pagination pagination-sm mb-0">
                    <li className="page-item">
                      <a className="page-link" href="#" aria-label="Previous">
                        <span aria-hidden="true">« Prev</span>{" "}
                      </a>
                    </li>
                    <li className="page-item">
                      <a className="page-link" href="#" aria-label="Next" >
                        <span aria-hidden="true">Next »</span>{" "}
                      </a>
                    </li>
                  </ul>
                </nav>
                <div className="form-group">
                  <label className="text-muted">No. of rows:&nbsp;</label>
                  <select className="form-select-sm p-1" style={{border: '1px solid silver'}}>
                    <option value="25">25</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                  </select>
                </div>
              </div>
            </div>
            <div
              className="w-100 overflow-auto"
              style={{ height: "calc(100vh - 35rem)" }}
              id="#scrollbar-1"
            >
              {getResults(bpf_socket_events,["path","remote_address","remote_port","syscall","unixtime"])}
            </div>
          </div>
        </div>
      </div>
      
      {/** <---- Resource Utilization -----> **/}
      <div className="d-flex justify-content-start align-items-end ps-2 mt-6" style={{fontSize:"1.5rem",fontWeight:"500"}}>
        Resource Utilization
      </div>
      <div className="card p-5">
        <div className="row">
        <div className="col">
          <Card style={{ width: '18rem', height : '13rem' }}>
            <Card.Body>
              <Card.Title className="d-flex justify-content-center">Memory Usage</Card.Title>
              <Card.Text style={{height:"70%"}}>
                <div className="d-flex p-2 align-items-center justify-content-center h-100 h1">{container_stats!==undefined?((container_stats[0]?.memory_usage/container_stats[0]?.memory_limit)*100).toFixed(3)+" %":<></>}</div>
              </Card.Text>
            </Card.Body>
          </Card>
        </div>
        <div className="col">
          <Card style={{ width: '18rem', height : '13rem' }}>
            <Card.Body>
              <Card.Title className="d-flex justify-content-center">CPU Usage</Card.Title>
              <Card.Text style={{height:"70%"}}>
                <div className="d-flex p-2 align-items-center justify-content-center h-100 h1">{container_stats!==undefined?((container_stats[0]?.cpu_total_usage/container_stats[0]?.system_cpu_usage)*100).toFixed(3)+" %":<></>}</div>
              </Card.Text>
            </Card.Body>
          </Card>
        </div>
        <div className="col">
          <Card style={{ width: '18rem', height : '13rem' }}>
            <Card.Body>
              <Card.Title className="d-flex justify-content-center">Online CPU</Card.Title>
              <Card.Text style={{height:"70%"}}>
                <div className="d-flex p-2 align-items-center justify-content-center h-100 h1">{container_stats!==undefined?container_stats[0]?.online_cpus:<></>}</div>
              </Card.Text>
            </Card.Body>
          </Card>
        </div>
        <div className="col">
          <Card style={{ width: '18rem', height : '13rem' }}>
            <Card.Body>
              <Card.Title className="d-flex justify-content-center">Kernel Mode CPU Usage</Card.Title>
              <Card.Text style={{height:"70%"}}>
                <div className="d-flex p-2 align-items-center justify-content-center h-100 h1">{container_stats!==undefined?((container_stats[0]?.cpu_kernelmode_usage/container_stats[0]?.cpu_total_usage)*100).toPrecision(2)+" %":<></>}</div>
              </Card.Text>
            </Card.Body>
          </Card>
        </div>
        <div className="col">
          <Card style={{ width: '18rem', height : '13rem' }}>
            <Card.Body>
              <Card.Title className="d-flex justify-content-center">User Mode CPU Usage</Card.Title>
              <Card.Text style={{height:"70%"}}>
                <div className="d-flex p-2 align-items-center justify-content-center h-100 h1">{container_stats!==undefined?((container_stats[0]?.cpu_usermode_usage/container_stats[0]?.cpu_total_usage)*100).toPrecision(2)+" %":<></>}</div>
              </Card.Text>
            </Card.Body>
          </Card>
        </div>
          {/* <div className="col-6">
            {MakePieChart(osCount,"Processor")}
          </div>
          <div className="col-6">
            {MakeBarGraph(nodeCount,"Memory")}
          </div> */}
        </div>
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
    </>
  );
};

export default ContainerInfo;
