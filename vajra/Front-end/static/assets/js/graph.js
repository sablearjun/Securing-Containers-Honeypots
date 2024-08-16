let data = [0,0,0,0,0]
let data1 = [0,0,0,0,0]
// const xhr = new XMLHttpRequest();
// const axios = new axios;
// let time=0;

// setInterval(function()
// {
//     if(time==50)
//     {
//         window.location.reload();
//         time=0;
//     }
//     xhr.open('GET', "/graph/", false);
//     xhr.send();
//     var response = xhr.responseText;
//     data=JSON.parse(response)
//     // chart.data.datasets[0].data=data["mem"];
//     // chart.update();
//     Chart.data.datasets[0].data=data["cpu"];
//     Chart.data.datasets[1].data=data["mem"];
//     Chart.update();
//     severity.data.datasets[0].data = data["severity"];
//     severity.update();
//     document.getElementById("non").innerHTML=data["severity"][0];
//     document.getElementById("information").innerHTML=data["severity"][1];
//     document.getElementById("low").innerHTML=data["severity"][2];
//     document.getElementById("medium").innerHTML=data["severity"][3];
//     document.getElementById("high").innerHTML=data["severity"][4];
//     time++;
// }, 1000);

// var ctx = document.getElementById('my-mem-Chart');
// var chart=new Chart(ctx, {
//     type: 'line',
//     data: {
//         labels: ["", "", "", "", ""],
//         datasets: [{
//             data: data,
//             label: "Memory",
//             borderColor: "#BDEA14",
//             backgroundColor: "#BDEA14",
//             fill: false
//         }
//         ]
//     },
//     options: {
//         responsive: true,
//         plugins: {
//             title: {
//                 display: false,
//                 text: 'Machine Health',
//                 color: '#949494',
//                 fullSize: true,
//                 font: { size: 15 }
//             }
//         },
//     }
// });

// var parent = document.getElementById("top_mem_proc");
// var parent1 = document.getElementById("top_cpu_proc");
// let cpu_proc = ["process 1", "process 2", "process 3", "process 4", "process 5"];
// let mem_proc = ["process 1", "process 2", "process 3", "process 4", "process 5"]

// cpu_proc.map(data => {
//     let para = document.createElement("p");
//     para.textContent = data;
//     parent.appendChild(para);
// });

// mem_proc.map(data => {
//     const para = document.createElement("p");
//     para.innerHTML = data;
//     parent1.appendChild(para);
// });


var mtx = document.getElementById('sivChart');
var severity = new Chart(mtx, {
    type: 'doughnut', //doughnut
    data: {
        labels: ["None", "Information", "Low", "Medium", "High"],
        datasets: [{
            data: data,
            // data:[57,48,68,26,32],
            backgroundColor: [
                "#36CCFC",
                "#5DEE3B",
                "#FCE552",
                "#FEB032",
                "#FD393F",
            ]
        }
        ]
    },
    options: {
        plugins: {
            legend: false,
            tooltip: true,
            title: {
                display: true,
                text: '',
                color: '#949494',
                fullSize: true,
                font: { size: 15 }
            }
        },
        elements: {
            arc: {
                backgroundColor: function (context) {
                    let c = colors[context.dataIndex];
                    if (!c) {
                        return;
                    }
                    if (context.active) {
                        c = helpers.getHoverColor(c);
                    }
                    const mid = helpers.color(c).desaturate(0.2).darken(0.2).rgbString();
                    const start = helpers.color(c).lighten(0.2).rotate(270).rgbString();
                    const end = helpers.color(c).lighten(0.1).rgbString();
                    return createRadialGradient3(context, start, mid, end);
                },
            }
        }
    }
});

var ctx = document.getElementById('my-cpu-Chart');
var Chart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: ["", "", "", "", ""],
        datasets: [
            {
                data: data,
                // data:[321,211,231,23,12],
                label: "CPU",
                borderColor: "#FF0080",
                backgroundColor: "#FF0080",
                fill: false
            },
            {
                data: data1,
                // data:[121,311,131,83,42],
                label: "Memory",
                borderColor: "#BDEA14",
                backgroundColor: "#BDEA14",
                fill: false
            }
        ]
    },
    options: {
        responsive: true,
        plugins: {
            title: {
                display: true,
                text: 'Machine Health',
                color: '#949494',
                fullSize: true,
                font: { size: 15 }
            }
        },
    }
});

setInterval(() => {
    axios.get('/graph/').then(resp => {
    let data=resp.data;
    Chart.data.datasets[0].data=data["cpu"];
    Chart.data.datasets[1].data=data["mem"];
    Chart.update();
    severity.data.datasets[0].data = data["severity"];
    severity.update();
    document.getElementById("non").innerHTML=data["severity"][0];
    document.getElementById("information").innerHTML=data["severity"][1];
    document.getElementById("low").innerHTML=data["severity"][2];
    document.getElementById("medium").innerHTML=data["severity"][3];
    document.getElementById("high").innerHTML=data["severity"][4];
    });
}, 1000);