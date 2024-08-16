let gridOptions;
let config_dict;
let Token;
let chabi;
// ***************************************************************************
// Template
// ***************************************************************************
const columnDefs = [
    { field: "make" },
    { field: "model" },
    { field: "price" }
];

// specify the data
const rowData = [
    { make: "Toyota", model: "Celica", price: 35000 },
    { make: "Ford", model: "Mondeo", price: 32000 },
    { make: "Porsche", model: "Boxter", price: 72000 }
];

// let the grid know which columns and what data to use
gridOptions = {
    columnDefs: columnDefs,
    rowData: rowData
};

// setup the grid after the page has finished loading
document.addEventListener('DOMContentLoaded', () => {
    const gridDiv = document.querySelector('#mySpace');
    new agGrid.Grid(gridDiv, gridOptions);
});
// ***************************************************************************
// ***************************************************************************

function Submit() {
    let field=document.getElementById("query4hawk");
    if(field.value!=null && field.value.length>0)
    {
        document.querySelector("#querydata").setAttribute("class","collapse show");
        var formData = new FormData( document.getElementById("queryForm") );
        let xhr = new XMLHttpRequest();
        xhr.open('POST', "/addhawk/", false);
        xhr.send(formData);
        var response = JSON.parse(xhr.responseText);
        let log=setInterval(() => {
            status=getStatus(response);
            if(status=="ready")
            {
                getResult();
                clearInterval(log);
            }
        }, 1000);
    }
}

function getStatus(response) {
    var key=String(document.querySelector('input[name=hosts]').value)+":0";
    Token=response['csrftoken'];
    let config = new FormData();
    chabi=response[key];
    config.append("config_ids",response[key]);
    const xhr = new XMLHttpRequest();
    xhr.open('POST', "/addhawKstatus/", false);
    xhr.setRequestHeader('X-CSRFToken', response['csrftoken']);
    xhr.send(config);
    var queryResponse = JSON.parse(xhr.responseText);
    document.getElementById("error").innerHTML=String(queryResponse.status)+" . . . . .";
    console.log(queryResponse);
    if (queryResponse.status == "ready") {
        document.getElementById("error").innerHTML="";
        makeButton(document.getElementById("error"));
        config_dict=config;
        return "ready";
    }
    return "wait"
}

function getResult() {
    const xhr = new XMLHttpRequest();
    xhr.open('POST', "/addhawKresults/", false);
    xhr.setRequestHeader('X-CSRFToken', Token);
    xhr.send(config_dict);
    var queryResponse = JSON.parse(xhr.responseText);
    let list =JSON.parse(queryResponse.data[chabi]);
    drawTable(list);
    document.querySelector('#mySpace').style.display="inherit";
}

function closeIt() {
    document.querySelector("#querydata").setAttribute("class","collapse");
}

function focusIt(id) {
    let a=document.getElementById(id);
    if(a.value.length>0 || a.value==null)
    {
        document.querySelector("span[name=search]").style.display="none";
        document.querySelector("span[name=cross]").style.display="flex";
    }
    else
    {
        document.querySelector("span[name=search]").style.display="flex";
        document.querySelector("span[name=cross]").style.display="none";
    }
}

function erase() {
    document.getElementById("query4hawk").value=null;
    document.querySelector("span[name=search]").style.display="flex";
    document.querySelector("span[name=cross]").style.display="none";
}

function drawTable(list) {
    gridOptions.api.destroy();
    makeGrid(list);
    gridOptions.api.sizeColumnsToFit();
}

function makeGrid(list) {
    var cols = [];
    for (var i = 0; i < list.length; i++) {
        for (var k in list[i]) {
            if (cols.indexOf(k) === -1) {
                // Push all keys to the array
                cols.push(k);
            }
        }
    }

    columns = [];

    cols.map((item) => {
        if (item !== "hostname") {
            columns.push({ field: item, filter: true, resizable: true });
        }
    })

    const columnDefs = columns;

    // specify the data
    const rowData = list;

    // let the grid know which columns and what data to use
    gridOptions = {
        columnDefs: columnDefs,
        rowData: rowData,
    };
    // gridOptions.destroy();
    // setup the grid after the page has finished loading
    const gridDiv = document.querySelector('#mySpace');
    new agGrid.Grid(gridDiv, gridOptions);
}

function downloadCSV() {
    gridOptions.api.exportDataAsCsv({fileName: filename()});
}

function filename() {
    var today = new Date();
    var date = today.getDate() + '-' + (today.getMonth() + 1) + '-' + today.getFullYear();
    var time = parseInt(parseInt(today.getHours()) % 12) + "-" + today.getMinutes() + "-" + today.getSeconds();
    var dateTime = date + '\xa0\xa0\xa0\xa0' + time;
    return dateTime;
}

function makeButton(position) {
    let btn=document.createElement("button");
    btn.setAttribute("class","btn btn-outline-primary");
    btn.style.width="7vw";
    btn.setAttribute("onclick","downloadCSV()");
    let icon=document.createElement("i");
    icon.setAttribute("class","fa fa-download");
    icon.setAttribute("aria-hidden","true");
    let span=document.createElement("span");
    span.innerHTML="Download";
    span.style.marginLeft="10%";
    btn.appendChild(icon);
    btn.appendChild(span);
    position.appendChild(btn);
}

let QueryList=[
    "select * from users;",
    "SELECT pid, name, ROUND((total_size * '10e-7'), 2) AS used FROM processes ORDER BY total_size DESC LIMIT 5;",
    "SELECT pid, uid, name, ROUND(((user_time + system_time) / (cpu_time.itsb - cpu_time.tsb)) * 100, 2) AS percentage FROM processes, (SELECT (SUM(user) + SUM(nice) + SUM(system) + SUM(idle) * 1.0) AS tsb,SUM(COALESCE(idle, 0)) +SUM(COALESCE(iowait, 0)) AS itsb FROM cpu_time) AS cpu_time ORDER BY user_time+system_time DESC LIMIT 5;"
]

function fillQuery(index) {
    console.log(QueryList[index]);
    erase();
    document.getElementById("query4hawk").value=QueryList[index];
    focusIt("query4hawk")
}