import React, { PureComponent,useRef,useCallback,forwardRef,Component } from 'react';
import axios from "axios";
import ReactDOM from 'react-dom'
import Dropdown from 'react-bootstrap/Dropdown';
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
import { Bar,Line } from 'react-chartjs-2';
//import { TOKEN } from "../../../apis/base-urls";
import { useState, useEffect } from "react";
import { EVENTS_LIST_URL, EVENTS_UPDATE_URL, NODES_LIST_URL } from "../apis/urls";
import { NODE_COUNT_URL, EVENTS_COUNT_URL, OS_COUNT_URL, SEVERITY_COUNT_URL, ALERT_COUNT_URL, HIGH_RISK_COUNT_URL,NODE_ALERT_COUNT_URL,WEEKLY_ALERTS_COUNT_URL} from "../apis/urls";
import { getDefaultHeaders, useFetchURL, useLazyFetch } from "../hooks";
import { Event,Node , toggle_state} from "../types";
// import NodeSheetView from "../../../pages/nodeSheetView";
import { Link } from "react-router-dom";
import { Navigate } from "react-router";
import { Rapid_fetcher } from "../apis/requests";
import { timeConverter } from '../helpers';

import * as bootstrap from "bootstrap";
import swal from 'sweetalert'
import  loaderForTable from '../../../assets/images/loader-table.svg'
import { json } from "stream/consumers";
import ReactTooltip from "react-tooltip";
import Loader from '../components/loader';
ChartJS.register(
  CategoryScale,
  LinearScale,
  BarElement,
  Title,
  Tooltip,
  Legend,
  PointElement,
  LineElement,
  ArcElement
);
const Dashboard = () => {

  const [nodeCount, setNodeCount] = useState<any>(undefined);
  const [eventCount, setEventCount] = useState<any>(undefined);
  const [osCount, setOsCount] = useState<any>(undefined);
  const [highRiskCount, setHighRiskCount] = useState<any>(undefined);
  const [severityCount, setSeverityCount] = useState<any>(undefined);
  const [alertCount, setAlertCount] = useState<any>(undefined);
  const [nodeAlertCount, setNodeAlertCount] = useState<any>(undefined);
  const [weeklyAlertsCount, setWeeklyAlertsCount] = useState<any>(undefined);
  const [maliciousGraphArgs, setMaliciousGraphArgs] = useState<any>({severity:4,level:"desc",topn:5});
  const [high_risk_count_url, setHigh_risk_count_url] = useState<string>((HIGH_RISK_COUNT_URL+"?topn=5"))

  useEffect(() => {
    if(nodeCount===undefined)
    {
      getDashboard(NODE_COUNT_URL,setNodeCount);
    }
    if(eventCount===undefined)
    {
      getDashboard(EVENTS_COUNT_URL,setEventCount);
    }
    if(osCount===undefined)
    {
      getDashboard(OS_COUNT_URL,setOsCount);
    }
    if(severityCount===undefined)
    {
      getDashboard(SEVERITY_COUNT_URL,setSeverityCount);
    }
    if(alertCount===undefined)
    {
      getDashboard(ALERT_COUNT_URL,setAlertCount);
    }
    if(nodeAlertCount===undefined)
    {
      getDashboard(NODE_ALERT_COUNT_URL,setNodeAlertCount);
    }
    if(weeklyAlertsCount===undefined)
    {
      getDashboard(WEEKLY_ALERTS_COUNT_URL,setWeeklyAlertsCount);
    }
    getDashboard(high_risk_count_url,setHighRiskCount);
  },[high_risk_count_url])

  const getDashboard = (url:string,setter:Function) => {
    Rapid_fetcher(url)
      .then((res) => {
        res.message ? console.log(res.message) : setter(res)
      })
      .catch((err) => {
        setter(undefined)
        // console.log('Event Graph Error: ', err)
      })
  }

  function setArgs(ky:string,val:string) {
    setMaliciousGraphArgs(()=>{
      let Tmp=maliciousGraphArgs;
      Tmp[ky]=val;
      return Tmp;
    })
    let url:string=HIGH_RISK_COUNT_URL;
    (Object.keys(maliciousGraphArgs)).map((a)=>{
      if(url.endsWith("/"))
      {
        url=url+"?"+a+"="+maliciousGraphArgs[a]
      }
      else{
        url=url+"&"+a+"="+maliciousGraphArgs[a]
      }
    })
    setHigh_risk_count_url(url)
  }

  const renderActiveShape = (props: any) => {
    const RADIAN = Math.PI / 180;
    const {
      cx,
      cy,
      midAngle,
      innerRadius,
      outerRadius,
      startAngle,
      endAngle,
      fill,
      payload,
      percent,
      value
    } = props;
    const sin = Math.sin(-RADIAN * midAngle);
    const cos = Math.cos(-RADIAN * midAngle);
    const sx = cx + (outerRadius + 10) * cos;
    const sy = cy + (outerRadius + 10) * sin;
    const mx = cx + (outerRadius + 30) * cos;
    const my = cy + (outerRadius + 30) * sin;
    const ex = mx + (cos >= 0 ? 1 : -1) * 22;
    const ey = my;
    const textAnchor = cos >= 0 ? "start" : "end";
    
    return (
      <g>
        <text x={cx} y={cy} dy={8} textAnchor="middle" fill={fill} style={{fontSize:"1.5rem"}}>
          {payload.name}
        </text>
        <Sector
          cx={cx}
          cy={cy}
          innerRadius={innerRadius}
          outerRadius={outerRadius}
          startAngle={startAngle}
          endAngle={endAngle}
          fill={fill}
        />
        <Sector
          cx={cx}
          cy={cy}
          startAngle={startAngle}
          endAngle={endAngle}
          innerRadius={outerRadius + 6}
          outerRadius={outerRadius + 10}
          fill={fill}
        />
        <path
          d={`M${sx},${sy}L${mx},${my}L${ex},${ey}`}
          stroke={fill}
          fill="none"
        />
        <circle cx={ex} cy={ey} r={2} fill={fill} stroke="none" />
        {/* <text
          x={ex + (cos >= 0 ? 1 : -1) * 12}
          y={ey}
          textAnchor={textAnchor}
          fill="#333"
        >{`PV ${value}`}</text> */}
        <text
          x={ex + (cos >= 0 ? 1 : -1) * 12}
          y={ey}
          // dy={18}
          textAnchor={textAnchor}
          fill="#999"
        >
          {`(${(percent * 100).toFixed(2)}%)`}
        </text>
      </g>
    );
  };

  const RADIAN = Math.PI / 180;
  const renderCustomizedLabel = ({
    cx,
    cy,
    midAngle,
    innerRadius,
    outerRadius,
    percent,
    index
  }: any) => {
    const radius = innerRadius + (outerRadius - innerRadius) * 0.5;
    const x = cx + radius * Math.cos(-midAngle * RADIAN);
    const y = cy + radius * Math.sin(-midAngle * RADIAN);

    return (
      <text
        x={x}
        y={y}
        fill="white"
        textAnchor={x > cx ? "start" : "end"}
        dominantBaseline="central"
      >
        {`${(percent * 100).toFixed(0)}%`}
      </text>
    );
  };

  const data1 = [
    { name: 'None', value: 200 },
    { name: 'Information', value: 300 },
    { name: 'Low', value: 500 },
    { name: 'Medium', value: 100 },
    { name: 'High', value: 50 }
  ];

  const data2 = [
    { name: "Open", value: 400 },
    { name: "Close", value: 300 },
  ];

  // const data3 = [
  //   {
  //     name: 'Sunday',
  //     "File events": 8822,
  //     "Socket events":2322,
  //     "process events":4342
  //   },
  //   {
  //     name: 'Monday',
  //     "File events": 9197,
  //     "Socket events":2522,
  //     "process events":4352
  //   },
  //   {
  //     name: 'Tuesday',
  //     "File events": 9297,
  //     "Socket events":2131,
  //     "process events":4234
  //   },
  //   {
  //     name: 'Wednesday',
  //     "File events": 5289,
  //     "Socket events":2368,
  //     "process events":4389
  //   },
  //   {
  //     name: 'Thursday',
  //     "File events": 5897,
  //     "Socket events":2081,
  //     "process events":9131
  //   },
  //   {
  //     name: 'Friday',
  //     "File events": 6812,
  //     "Socket events":1918,
  //     "process events":9187
  //   },
  //   {
  //     name: 'Saturday',
  //     "File events": 1973,
  //     "Socket events":6891,
  //     "process events":9715
  //   },
  // ];
  const data3=[
    {
        name: 'Friday',
        "File Alert": 0,
        "Process Alert": 17,
        "Socket Alert": 0
    },
    {
        name: 'Thrusday',
        "File Alert": 0,
        "Process Alert": 5,
        "Socket Alert": 0
    },
    {
        name: 'Wednesday',
        "File Alert": 0,
        "Process Alert": 5,
        "Socket Alert": 0
    },
    {
        name: 'Tuesday',
        "File Alert": 0,
        "Process Alert": 8,
        "Socket Alert": 0
    },
    {
        name: 'Monday',
        "File Alert": 0,
        "Process Alert": 0,
        "Socket Alert": 0
    },
    {
        name: 'Sunday',
        "File Alert": 0,
        "Process Alert": 0,
        "Socket Alert": 0
    },
    {
        name: 'Saturday',
        "File Alert": 0,
        "Process Alert": 0,
        "Socket Alert": 0
    },
];

  const data4 = [
    { name: "Privilege Escalation", value: 5 },
    { name: "Persistence", value: 10 },
    { name: "Credential Access", value: 23 },
    { name: "Lateral Movement", value: 12 },
    { name: "Defense Evasion", value: 8 }
  ];

  const data5 = [
    { name: "Linux", value: 20 },
    { name: "Windows", value: 5 },
    { name: "Mac-Os", value: 2 }
  ];

  var COLORS = ["#82ca9d","#CD3636"];
  const COLORS1 = ["#00bfdd", "#20c997","#ffc107","#fe7f2d","#cd3636"];
  const COLORS_PIE=["#007ED6","#8399EB", "#8E6CEF", "#E01E84", "#FF0000", "#FF7300", "#FFAF00", "#FFEC00", "#1BAA2F", "#26D7AE", "#26D7AE", "#7CDDDD", "#5FB7D4", "#97D9FF"]

  const [activeIndex, setActiveIndex] = useState(0);
  const onPieEnter = useCallback(
    (_:any, index:any) => {
      setActiveIndex(index);
    },
    [setActiveIndex]
  );
  return (
    <>
      <div className="card card-body">
        <div className="hstack">
          <div className="d-flex flex-column justify-content-start" style={{width:"12vw"}}>
            <h3 className="fw-mid my-0">Dashboard</h3>
            <div>
              <div className="mt-2">
                <button
                  className="fs-8 px-1 btn btn-link text-muted"
                  // onClick={() => setispast(7)}
                >
                  7 Days
                </button>
                <button
                  className="fs-8 px-1 btn btn-link text-muted active"
                  // onClick={() => setispast(15)}
                >
                  15 Days
                </button>
                <button
                  className="fs-8 px-1 btn btn-link text-muted"
                  // onClick={() => setispast(180)}
                >
                  180 Days
                </button>
              </div>
            </div>
          </div>
          {nodeCount!==undefined?nodeCount.detail?(""):
          <>
          <div className="row ms-5 w-100">
            <div className="col-1 border-end">
              <div className="row d-flex justify-content-center align-items-center" style={{fontSize:"1.5rem"}}>
                Online
              </div>
              <div className="row d-flex justify-content-center align-items-center text-success" style={{fontSize:"2rem"}}>
                {nodeCount.data[1]}/{nodeCount.data[0]}
              </div>
            </div>
            <div className="col-1">
              <div className="row d-flex justify-content-center align-items-center" style={{fontSize:"1.5rem"}}>
                Offline
              </div>
              <div className="row d-flex justify-content-center align-items-center text-danger" style={{fontSize:"2rem"}}>
                {nodeCount.data[2]}/{nodeCount.data[0]}
              </div>
            </div>
          </div>
        </>:("")
      }
      </div>
      </div>
      {/* <div className="card card-body">
        <div className="hstack">
          <div className="d-flex flex-column justify-content-start">
            <h3 className="fw-mid my-0">Dashboard</h3>
            <div>
              <div className="mt-2">
                <button
                  className="fs-8 px-1 btn btn-link text-muted"
                  // onClick={() => setispast(7)}
                >
                  7 Days
                </button>
                <button
                  className="fs-8 px-1 btn btn-link text-muted active"
                  // onClick={() => setispast(15)}
                >
                  15 Days
                </button>
                <button
                  className="fs-8 px-1 btn btn-link text-muted"
                  // onClick={() => setispast(180)}
                >
                  180 Days
                </button>
              </div>
            </div>
          </div>
          {nodeCount!==undefined?nodeCount.detail?("")
          :(<>
          <div className="d-flex px-8 flex-column">
            <h3 className="fw-mid my-0 text-success">{nodeCount.data[1]}/{nodeCount.data[0]}</h3>
            <div>
              <div className="mt-2">
                <button
                  className="fs-10 px-1 btn btn-link text-muted"
                >
                  Online
                </button>
              </div>
            </div>
          </div>
          <div className="d-flex px-6 flex-column">
            <h3 className="fw-mid my-0 text-danger">{nodeCount.data[2]}/{nodeCount.data[0]}</h3>
            <div>
              <div className="mt-2">
                <button
                  className="fs-10 px-1 btn btn-link text-muted"
                >
                  Offline
                </button>
              </div>
            </div>
          </div></>):("")
          } 
        </div>
      </div> */}
      <div className="card py-3">
        <div className="card-body px-6">
        <div className="container-fluid mt-2">
        <div className="row">
          <div className="col-xl-8">
            <div className="row">
              <div className="col-4">
                <div className="col d-flex justify-content-center">
                  <h5>Host Connectivity</h5>
                </div>
                <div className="col" style={{border: '1px solid',padding:'0.5rem 0.5rem 0.5rem 0', borderRadius:'1rem',height:"300px"}}>
                  {nodeCount!==undefined?nodeCount.data?
                  (<Bar 
                   options={{
                    responsive: true,
                    maintainAspectRatio: false,
                    scales: {
                      y: {
                        ticks: {
                            precision: 0
                        }
                      }
                    },
                    plugins: {
                      legend: {
                        position: 'top' as const,
                      }
                    },
                  }}
                   data={{
                    labels:nodeCount.labels,
                    datasets: [
                      {
                        label: 'Nodes',
                        data: nodeCount.data,
                        backgroundColor: ["#ABC9FF","#82CA9D","#CD3636"]
                      }
                    ],
                    }} 
                   />):<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>:<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
                  }
                </div>
              </div>
              <div className="col-4">
                <div className="col d-flex justify-content-center">
                  <h5>Events Status Graph</h5>
                </div>
                <div className="col" style={{padding:'0.5rem 0.5rem 0.5rem 0',border: '1px solid', borderRadius:'1rem',height:"300px"}}>
                  {
                  eventCount!==undefined?eventCount.detail?
                  (<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
                  :(
                  <ResponsiveContainer width="100%" height="100%">
                    <PieChart width={400} height={400}>
                      <Pie
                        data={eventCount}
                        cx={"2rem"}
                        cy={"2rem"}
                        labelLine={false}
                        // label={renderCustomizedLabel}
                        outerRadius={"90%"}
                        fill="#8884d8"
                        dataKey="value"
                      >
                        {data2.map((entry, index) => (
                          <Cell key={`cell-${index}`} fill={COLORS[index % COLORS.length]} />
                        ))}
                      </Pie>
                      <Legend1 />
                      <Tooltip1 />
                    </PieChart>
                  </ResponsiveContainer>):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
                  }
                </div>
              </div>
              <div className="col-4">
                <div className="col">
                  <div className="col d-flex justify-content-center">
                    <h5>Host by OS</h5>
                  </div>
                  <div className="col" style={{border: '1px solid', borderRadius:'1rem',height:'300px'}}>
                    {osCount!==undefined?osCount.detail?
                    <div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
                    :(<ResponsiveContainer width="100%" height="100%">
                      <PieChart width={400} height={400}>
                        <Pie
                          data={osCount}
                          cx={"2rem"}
                          cy={"2rem"}
                          labelLine={false}
                          // label={renderCustomizedLabel}
                          outerRadius={"90%"}
                          fill="#8884d8"
                          dataKey="count"
                        >
                          {osCount.map((entry:any, index:number) => (
                            <Cell key={`cell-${index}`} fill={COLORS_PIE[index % COLORS_PIE.length]} />
                          ))}
                        </Pie>
                        <Legend1 />
                        <Tooltip1 />
                      </PieChart>
                    </ResponsiveContainer>):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
                    }
                  </div>
                </div>
              </div>
            </div>
            <div className="row">
              <div className="col">
                <div className="col d-flex justify-content-center" style={{marginTop:'0.5rem'}}>
                  <h5>Alerts Time Graph</h5>
                </div>
                <div className="col" style={{padding:'0.5rem 0.5rem 0.5rem 0',border: '1px solid', borderRadius:'1rem',height:"300px"}}>
                  {
                    weeklyAlertsCount!==undefined?weeklyAlertsCount.detail?
                    <div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
                  :(<Line 
                    options={{
                      responsive: true,
                      maintainAspectRatio: false,
                      plugins: {
                        legend: {
                          position: 'top' as const,
                        },
                      },
                    }} 
                    data={
                      {
                        labels:weeklyAlertsCount.map((data:any,_:number)=>{
                          return weeklyAlertsCount[_]["name"]
                        }),
                        datasets: [
                          {
                            label: 'File Alert',
                            data: weeklyAlertsCount.map((data:any,_:number)=>{
                              return weeklyAlertsCount[_]["File Alert"]
                            }),
                            borderColor: '#9C9EFE',
                            backgroundColor: '#9C9EFE',
                          },
                          {
                            label: 'Process Alert',
                            data: weeklyAlertsCount.map((data:any,_:number)=>{
                              return weeklyAlertsCount[_]["Process Alert"]
                            }),
                            borderColor: '#00D7FF',
                            backgroundColor: '#00D7FF',
                          },
                          {
                            label: 'Socket Alert',
                            data: weeklyAlertsCount.map((data:any,_:number)=>{
                              return weeklyAlertsCount[_]["Socket Alert"]
                            }),
                            borderColor: '#7DCE13',
                            backgroundColor: '#7DCE13',
                          }
                        ],
                      }
                    } />):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
                  }
                </div>
              </div>
            </div>
          </div>
          <div className="col-xl-4">
            <div className="col d-flex justify-content-center">
              <h5>Most Alerts Generating Machines</h5>
            </div>
            <div className="col" style={{padding:'2rem 0.5rem 2rem 0.5rem',border: '1px solid', borderRadius:'1rem',height:'45%'}}>
              {
                nodeAlertCount!==undefined?nodeAlertCount.detail?
                <div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
              :(<table className="table">
                <thead>
                  <tr>
                    <th scope="col">Host Name</th>
                    {/* <th scope="col">Enrolled on</th> */}
                    <th scope="col">Os</th>
                    <th scope="col">Alerts Generated</th>
                  </tr>
                </thead>
                <tbody>
                  {
                    nodeAlertCount.map((data:any,it:number)=>{
                      return(<tr key={it}>
                        <td scope="row">{data.hostname}</td>
                        {/* <td>{timeConverter(parseInt(data.enrolled_on))}</td> */}
                        <td>{data.os_name + " " + data.os_arch + " " + data.os_major + "." + data.os_minor}</td>
                        <td>{data.count}</td>
                      </tr>)
                    })
                  }
                </tbody>
              </table>):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
              }
            </div>
            <div className="col d-flex justify-content-center" style={{marginTop:"0.5rem"}}>
              <h5>Severity Graph</h5>
            </div>
            <div className="col" style={{paddingLeft:'0.5rem',border: '1px solid', borderRadius:'1rem',height:'45%'}}>
              { severityCount!==undefined?severityCount.detail?
              <div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
              :(<ResponsiveContainer width="100%" height="100%">
                  <PieChart width={400} height={400}>
                    <Pie
                      activeIndex={activeIndex}
                      activeShape={renderActiveShape}
                      data={severityCount}
                      cx={'2rem'}
                      cy={'2rem'}
                      innerRadius={"60%"}
                      outerRadius={"70%"}
                      fill="#8884d8"
                      dataKey="value"
                      onMouseEnter={onPieEnter}
                    >
                    {severityCount.map((entry:any, index:number) => (
                          <Cell key={`cell-${index}`} fill={COLORS1[index % COLORS1.length]} />
                    ))}
                    </Pie>
                  <Legend1/>
                  </PieChart>
              </ResponsiveContainer>):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
              }
            </div>
          </div>
        </div>

        <div className="row">
          <div className="col-xl-5">
            <div className="col d-flex justify-content-center" style={{marginTop:'0.5rem'}}>
              <h5>Most Alert Triggered</h5>
            </div>
            <div className="col" style={{padding:'0.5rem 0.5rem 0.5rem 0',border: '1px solid', borderRadius:'1rem',height:'300px'}}>
              {
                alertCount!==undefined?alertCount.labels?
                (<Bar options={
                {
                  responsive: true,
                  maintainAspectRatio: false,
                  plugins: {
                    legend: {
                      position: 'top' as const,
                    }
                  },
                }
              }
              data={{
                labels:alertCount.labels,
                datasets: [
                  {
                    label: 'Event triggered',
                    data: alertCount.data,
                    backgroundColor: '#ABC9FF',
                  }
                ],
              }} />):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
            }
            </div>
          </div>
          <div className="col-xl-3">
            <div className="col d-flex justify-content-center" style={{marginTop:'0.5rem'}}>
              <h5>MITRE ATT&CK® Analysis</h5>
            </div>
            <div className="col" style={{border: '1px solid', borderRadius:'1rem',height:'300px'}}>
              <ResponsiveContainer width="100%" height="100%">
                <PieChart width={400} height={400}>
                  <Pie
                    data={data4}
                    cx={"2rem"}
                    cy={"2rem"}
                    labelLine={false}
                    // label={renderCustomizedLabel}
                    outerRadius={"90%"}
                    fill="#8884d8"
                    dataKey="value"
                  >
                    {data4.map((entry, index) => (
                      <Cell key={`cell-${index}`} fill={COLORS1[index % COLORS1.length]} />
                    ))}
                  </Pie>
                  <Legend1 />
                  <Tooltip1 />
                </PieChart>
              </ResponsiveContainer>
            </div>
          </div>
          <div className="col-xl-4">
            <div className="col d-flex justify-content-center" style={{marginTop:'0.5rem'}}>
              <h5>Malicious Nodes</h5>
            </div>
            <div className="col" style={{padding:'0.5rem 0.5rem 0.5rem 0',border: '1px solid', borderRadius:'1rem',height:'300px'}}>
              <div className="row">
                <div className="col-12 d-flex justify-content-around">
                <Dropdown>
                  <Dropdown.Toggle variant="secondary" id="dropdown-basic">
                    Severity
                  </Dropdown.Toggle>

                  <Dropdown.Menu>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("severity","0")}}>None</Dropdown.Item>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("severity","1")}}>Information</Dropdown.Item>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("severity","2")}}>Low</Dropdown.Item>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("severity","3")}}>Medium</Dropdown.Item>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("severity","4")}}>High</Dropdown.Item>
                  </Dropdown.Menu>
                </Dropdown>
                <Dropdown>
                  <Dropdown.Toggle variant="secondary" id="dropdown-basic">
                    Order
                  </Dropdown.Toggle>

                  <Dropdown.Menu>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("level","asc")}}>Ascending</Dropdown.Item>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("level","desc")}}>Descending</Dropdown.Item>
                  </Dropdown.Menu>
                </Dropdown>
                <Dropdown>
                  <Dropdown.Toggle variant="secondary" id="dropdown-basic">
                    Count
                  </Dropdown.Toggle>

                  <Dropdown.Menu>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("topn","5")}}>5</Dropdown.Item>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("topn","10")}}>10</Dropdown.Item>
                    <Dropdown.Item onMouseDown={(e)=>{setArgs("topn","20")}}>20</Dropdown.Item>
                  </Dropdown.Menu>
                </Dropdown>
                </div>
                <div className="col-12 d-flex justify-content-center align-items-center" style={{height:'25vh'}}>
                {highRiskCount!==undefined?highRiskCount.labels?
                (<Bar 
                  options={{
                responsive: true,
                maintainAspectRatio: false,
                plugins: {
                  legend: {
                    position: 'top' as const,
                  }
                },
              }}
                data={
                  {
                    labels:highRiskCount.labels,
                    datasets: [
                      {
                        label: 'Event triggered',
                        data: highRiskCount.data,
                        // backgroundColor: 'rgba(255, 99, 132, 0.5)',
                        backgroundColor: COLORS1[maliciousGraphArgs.severity],
                      }
                    ],
                  }
                } />):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
              }
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
        </div>
      </div>
    </>
  );
};

export default Dashboard;