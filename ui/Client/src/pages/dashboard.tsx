import {useCallback,useReducer } from 'react';
import Dropdown from 'react-bootstrap/Dropdown';
import { PieChart, Pie, Sector, Cell, ResponsiveContainer} from 'recharts';
import {Legend as Legend1} from 'recharts';
import LoadGraph from '../components/loadGraph';
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
import { useState, useEffect } from "react";
import { MITRE_ANALYSIS_URL} from "../apis/urls";
import { NODE_COUNT_URL, EVENTS_COUNT_URL, OS_COUNT_URL, SEVERITY_COUNT_URL, ALERT_COUNT_URL, HIGH_RISK_COUNT_URL,NODE_ALERT_COUNT_URL,WEEKLY_ALERTS_COUNT_URL} from "../apis/urls";
import { getDataReducer} from "../apis/requests";
import { reducerFunc,INITIAL_STATE } from '../apis/requests/reducer';
import { BARgraph, PIEchart } from '../components/graph';

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
  const [nodeCount, setNodeCount] = useReducer(reducerFunc,INITIAL_STATE)
  const [eventCount, setEventCount] = useReducer(reducerFunc,INITIAL_STATE)
  const [osCount, setOsCount] = useReducer(reducerFunc,INITIAL_STATE)
  const [highRiskCount, setHighRiskCount] = useReducer(reducerFunc,INITIAL_STATE)
  const [alertCount, setAlertCount] = useReducer(reducerFunc,INITIAL_STATE)
  const [severityCount, setSeverityCount] = useReducer(reducerFunc,INITIAL_STATE)
  const [nodeAlertCount, setNodeAlertCount] = useReducer(reducerFunc,INITIAL_STATE)
  const [weeklyAlertsCount, setWeeklyAlertsCount] = useReducer(reducerFunc,INITIAL_STATE)
  const [mitreAnalysis, setMitreAnalysis] = useReducer(reducerFunc,INITIAL_STATE)
  const [maliciousGraphArgs, setMaliciousGraphArgs] = useState<any>({severity:4,level:"desc",topn:5});
  const [high_risk_count_url, setHigh_risk_count_url] = useState<string>((HIGH_RISK_COUNT_URL+"?topn=5"))

  useEffect(() => {
    if(nodeCount.Data===undefined)
    {
      getDataReducer(NODE_COUNT_URL,setNodeCount);
    }
    if(mitreAnalysis.Data===undefined)
    {
      getDataReducer(MITRE_ANALYSIS_URL,setMitreAnalysis);
    }
    if(eventCount.Data===undefined)
    {
      getDataReducer(EVENTS_COUNT_URL,setEventCount);
    }
    if(osCount.Data===undefined)
    {
      getDataReducer(OS_COUNT_URL,setOsCount);
    }
    if(severityCount.Data===undefined)
    {
      getDataReducer(SEVERITY_COUNT_URL,setSeverityCount);
    }
    if(alertCount.Data===undefined)
    {
      getDataReducer(ALERT_COUNT_URL,setAlertCount);
    }
    if(nodeAlertCount.Data===undefined)
    {
      getDataReducer(NODE_ALERT_COUNT_URL,setNodeAlertCount);
    }
    if(weeklyAlertsCount.Data===undefined)
    {
      getDataReducer(WEEKLY_ALERTS_COUNT_URL,setWeeklyAlertsCount);
    }
    getDataReducer(high_risk_count_url,setHighRiskCount);
  },[high_risk_count_url])

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

  const COLORS1 = ["#00bfdd", "#20c997", "#ffc107", "#fe7f2d", "#cd3636"];

  const [activeIndex, setActiveIndex] = useState(0);
  const onPieEnter = useCallback(
    (_:any, index:any) => {
      setActiveIndex(index);
    },
    [setActiveIndex]
  );

  // const alertGraphkeys = Object.keys(weeklyAlertsCount?.Data);
  // // const fileAlertData = alertGraphkeys.map(date => weeklyAlertsCount?.Data?.[date]["File Alert"]);
  // // const processAlertData = alertGraphkeys.map(date => weeklyAlertsCount?.Data?.[date]["Process Alert"]);
  // // const socketAlertData = alertGraphkeys.map(date => weeklyAlertsCount?.Data?.[date]["Socket Alert"]);
  // console.log("Graph keys: ", alertGraphkeys);
  return (
    <>
      <div className="card py-3">
        <div className="card-body px-6">
        <div className="container-fluid mt-2">
        <div className="row">
          <div className="col-xl-8">
            <div className="row">
              <div className="col-12 col-md-4 mt-2">
                <BARgraph name={"Host Connectivity"} renderState={nodeCount} label={"Nodes"} color={["#82CA9D","#CD3636","#ABC9FF"]}/>
              </div>
              <div className="col-12 col-md-4 mt-2">
                <PIEchart name={"Events Status Graph"} renderState={eventCount} dataKey={"count"}/>
              </div>
              <div className="col-12 col-md-4 mt-2">
                <PIEchart name={"Host by OS"} renderState={osCount} dataKey={"count"}/>
              </div>
            </div>
            <div className="row">
              <div className="col">
                <div className="col d-flex justify-content-center mt-2">
                  <h5>Alerts Time Graph</h5>
                </div>
                <div className="col" style={{padding:'0.5rem 0.5rem 0.5rem 0',border: '1px solid', borderRadius:'1rem',height:"250px"}}>
                {
                  weeklyAlertsCount.Loading?<LoadGraph/>:weeklyAlertsCount.Data!==undefined?
                  (<Line
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
                        labels:Object.keys(weeklyAlertsCount?.Data)
                      ,
                        datasets: [
                          {
                            label: 'File Alert',
                            data: Object.keys(weeklyAlertsCount?.Data).map(date => weeklyAlertsCount?.Data?.[date]["File Alert"])
                            ,
                            borderColor: '#9C9EFE',
                            backgroundColor: '#9C9EFE',
                          },
                          {
                            label: 'Process Alert',
                            data: Object.keys(weeklyAlertsCount?.Data).map(date => weeklyAlertsCount?.Data?.[date]["Process Alert"]),
                            borderColor: '#00D7FF',
                            backgroundColor: '#00D7FF',
                          },
                          {
                            label: 'Socket Alert',
                            data: Object.keys(weeklyAlertsCount?.Data).map(date => weeklyAlertsCount?.Data?.[date]["Socket Alert"]),
                            borderColor: '#7DCE13',
                            backgroundColor: '#7DCE13',
                          }
                        ],
                      }
                    } />):<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
                }
                </div>
              </div>
            </div>
          </div>
          <div className="col-xl-4">
            <div className="col d-flex justify-content-center mt-2">
              <h5>Most Alerts Generating Machines</h5>
            </div>
            <div className="col styleScroll" style={{overflowY:'auto',padding:'0.2rem 0.5rem 2rem 0.5rem',border: '1px solid', borderRadius:'1rem',height:'250px'}}>
            {
                nodeAlertCount.Loading?<LoadGraph/>:nodeAlertCount.Data!==undefined?
                (<table className="table">
                <thead>
                  <tr>
                    <th scope="col">Host Name</th>
                    <th scope="col">Os</th>
                    <th scope="col">Alerts Generated</th>
                  </tr>
                </thead>
                <tbody>
                  {
                    nodeAlertCount.Data?.map((data:any,it:number)=>{
                      return(<tr key={it}>
                        <td scope="row">{data?.hostname}</td>
                        <td>{data?.os_name + " " + data?.os_arch + " " + data?.os_major + "." + data?.os_minor}</td>
                        <td>{data?.count}</td>
                      </tr>)
                    })
                  }
                </tbody>
              </table>):<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
            }
            </div>
            <div className="col d-flex justify-content-center" style={{marginTop:"0.5rem"}}>
              <h5>Severity Graph</h5>
            </div>
            <div className="col" style={{paddingLeft:'0.5rem',border: '1px solid', borderRadius:'1rem',height:'44%'}}>
              {
                  severityCount.Loading?<LoadGraph/>:severityCount.Data!==undefined?
                  (<ResponsiveContainer width="100%" height="100%">
                  <PieChart width={400} height={400}>
                    <Pie
                      activeIndex={activeIndex}
                      activeShape={renderActiveShape}
                      data={severityCount.Data}
                      cx={'2rem'}
                      cy={'2rem'}
                      innerRadius={"60%"}
                      outerRadius={"70%"}
                      fill="#8884d8"
                      dataKey="value"
                      onMouseEnter={onPieEnter}
                    >
                    {severityCount!== undefined && severityCount?.Data?.map((entry:any, index:any) => (
                          <Cell key={`cell-${index}`} fill={COLORS1[index % COLORS1.length]} />
                    ))}
                    </Pie>
                  <Legend1/>
                  </PieChart>
              </ResponsiveContainer>):<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
              }
            </div>
          </div>
        </div>

        <div className="row mt-2">
          <div className="col-xl-5">
            <BARgraph name={"Most Alert Triggered"} renderState={alertCount} label={"Event triggered"} color={"#ABC9FF"}/>
          </div>
          <div className="col-xl-3">
            <BARgraph name={"MITRE ATT&CK® Analysis"} renderState={mitreAnalysis} label={"Event triggered"} color={"#ABC9FF"}/>
          </div>
          <div className="col-xl-4">
            <div className="col d-flex justify-content-center">
              <h5>Malicious Nodes</h5>
            </div>
            <div className="col" style={{padding:'0.5rem 0.5rem 0.5rem 0',border: '1px solid', borderRadius:'1rem',height:'250px'}}>
              {/* <div className="row"> */}
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
                <div className="col-12 d-flex justify-content-center">
                {
                    highRiskCount.Loading?<LoadGraph/>:highRiskCount.Data!==undefined?
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
                        labels:highRiskCount.Data.map((item: any) => item.hostname),
                        datasets: [
                          {
                            label: 'Event triggered',
                            data: highRiskCount.Data.map((item: any) => item.count),
                            // backgroundColor: 'rgba(255, 99, 132, 0.5)',
                            backgroundColor: COLORS1[maliciousGraphArgs.severity],
                          }
                        ],
                      }
                    } />):<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
                  }
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