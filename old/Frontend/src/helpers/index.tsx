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
import { PieChart, Pie, Sector, Cell, ResponsiveContainer, CartesianGrid, XAxis, YAxis } from 'recharts';
import {Legend as Legend1} from 'recharts';
import {Tooltip as Tooltip1} from 'recharts';

export function timeConverter(UNIX_timestamp: number) {
    //console.log(UNIX_timestamp)
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
    var date = ()=>{
      if(a.getDate()<10){
        return "0"+a.getDate()
      }
      return a.getDate()
    };
    var hour = a.toLocaleString('en-US', {
      hour: 'numeric',
      minute: 'numeric',
      hour12: true
    });
    // var min = a.getMinutes();
    // var sec = a.getSeconds();
    var time =
      date() + " " + month + " " + year + " " + hour;
    return time+" (IST)";
  }

  export const MakeBarGraph=(state:any,name:string,label?:string,colours?:string)=>{

    return (<>
              <div className="col d-flex justify-content-center">
                <h5>{name}</h5>
              </div>
              <div className="col" style={{border: '1px solid',padding:'0.5rem 0.5rem 0.5rem 0', borderRadius:'1rem',height:"300px"}}>
                {state!==undefined?state.data?
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
                  labels:state.labels,
                  datasets: [
                    {
                      label: 'Nodes',
                      data: state.data,
                      backgroundColor: ["#ABC9FF","#82CA9D","#CD3636"]
                    }
                  ],
                  }} 
                  />):<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>:<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
                }
            </div>
          </>)
  }

  export const MakePieChart=(State:any,name:string)=>{
    var COLORS = ["#82ca9d","#CD3636"];
    const COLORS1 = ["#00bfdd", "#20c997","#ffc107","#fe7f2d","#cd3636"];
    const COLORS_PIE=["#007ED6","#8399EB", "#8E6CEF", "#E01E84", "#FF0000", "#FF7300", "#FFAF00", "#FFEC00", "#1BAA2F", "#26D7AE", "#26D7AE", "#7CDDDD", "#5FB7D4", "#97D9FF"]
    return (
      <>
        <div className="col d-flex justify-content-center">
          <h5>{name}</h5>
        </div>
        <div className="col" style={{border: '1px solid', borderRadius:'1rem',height:'300px'}}>
          {State!==undefined?State.detail?
          <div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>
          :(<ResponsiveContainer width="100%" height="100%">
            <PieChart width={400} height={400}>
              <Pie
                data={State}
                cx={"2rem"}
                cy={"2rem"}
                labelLine={false}
                // label={renderCustomizedLabel}
                outerRadius={"90%"}
                fill="#8884d8"
                dataKey="count"
              >
                {State.map((entry:any, index:number) => (
                  <Cell key={`cell-${index}`} fill={COLORS_PIE[index % COLORS_PIE.length]} />
                ))}
              </Pie>
              <Legend1 />
              <Tooltip1 />
            </PieChart>
          </ResponsiveContainer>):(<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>Graph not available</div>)
          }
        </div>
      </>
    )
  }