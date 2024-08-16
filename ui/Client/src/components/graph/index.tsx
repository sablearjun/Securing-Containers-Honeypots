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
import { PieChart, Pie, Cell, ResponsiveContainer} from 'recharts';
import {Legend as Legend1} from 'recharts';
import {Tooltip as Tooltip1} from 'recharts';
import LoadGraph from '../loadGraph';
import { Bar } from 'react-chartjs-2';
import { INITIAL_STATE_TYPE } from '../../apis/requests/reducer';
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


var COLORS = ["#82ca9d","#CD3636"];
const COLORS1 = ["#00bfdd", "#20c997","#ffc107","#fe7f2d","#cd3636"];
const COLORS_PIE=["#007ED6","#8399EB", "#8E6CEF", "#E01E84", "#FF0000", "#FF7300", "#FFAF00", "#FFEC00", "#1BAA2F", "#26D7AE", "#26D7AE", "#7CDDDD", "#5FB7D4", "#97D9FF"]

interface PieParameters{
    name:string,
    renderState:INITIAL_STATE_TYPE|undefined,
    dataKey:string
}

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

export const PIEchart=({name,renderState,dataKey}:PieParameters)=>{
    const pieData = renderState?.Data;
    return <>
        <div className="col d-flex justify-content-center">
            <h5>{name}</h5>
        </div>
        <div className="col" style={{padding:'0.5rem 0.5rem 0.5rem 0',border: '1px solid', borderRadius:'1rem',height:"250px"}}>
            {
                renderState?.Loading?<LoadGraph/>:renderState?.Data!==undefined?
                (
                <ResponsiveContainer width="100%" height="100%">
                <PieChart width={400} height={400}>
                    <Pie
                    data={pieData}
                    cx={"2rem"}
                    cy={"2rem"}
                    labelLine={false}
                    label={renderCustomizedLabel}
                    outerRadius={"90%"}
                    fill="#8884d8"
                    dataKey={dataKey}
                    >
                    {pieData.map((entry:any, index:any) => (
                        <Cell key={`cell-${index}`} fill={COLORS_PIE[index % COLORS_PIE.length]} />
                    ))}
                    </Pie>
                    <Legend1 />
                    <Tooltip1 />
                </PieChart>
                </ResponsiveContainer>):<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'1.5rem'}}>Graph not available</div>
            }
        </div>
    </>
}

interface BarParameters{
    name:string,
    renderState:INITIAL_STATE_TYPE|undefined,
    label:string,
    color:Array<string>|string
}

export const BARgraph=({name,renderState,label,color}:BarParameters)=>{
    return <>
        <div className="col d-flex justify-content-center">
            <h5>{name}</h5>
        </div>
        <div className="col" style={{border: '1px solid',padding:'0.5rem 0.5rem 0.5rem 0', borderRadius:'1rem',height:"250px"}}>
            {
            renderState?.Loading?<LoadGraph/>:renderState?.Data!==undefined?
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
                    tooltip: {
                        callbacks: {
                            title:(context) => Object.keys(renderState?.Data)[context[0].dataIndex],
                            label: (context) => context.parsed.y.toString()
                        }
                    },
                    legend: {
                    position: 'top' as const,
                    }
                },
                }}
                data={{
                labels: Object.keys(renderState?.Data).map((l:string)=>{
                    return l.slice(0,10)+"..."
                }),
                datasets: [
                    {
                        label: label,
                        data: Object.values(renderState?.Data),
                        backgroundColor: color
                    }
                ],
                }}
                />):<div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'1.5rem'}}>Graph not available</div>
            }
        </div>
    </>
}