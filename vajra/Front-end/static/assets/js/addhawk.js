// *********************************************************************
                // Global Variables
// *********************************************************************
var isCancelled = false; // To keep note of cancelled query
let gridOptions;
var isInitialData = true;
// *********************************************************************
// *********************************************************************


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




function queryCancel() {
    isCancelled = true;
}

function error() {
    setTimeout(() => {
        document.getElementById("error").innerHTML = "Please Select Host Host Identifiers or Fill in a query"
    }, 20);
}

function sendData() {
    // this scheduled the query in backend
    // takes in well formatted json of hostidentifiers and query
    // return the token no given by backend
    document.getElementById("query_main").style.visibility = "hidden";
    const form = document.getElementById("query_form");
    const xhr = new XMLHttpRequest();
    const FD = new FormData(form);
    // const csrftoken = document.querySelector('[name=csrfmiddlewaretoken]').value;
    xhr.open('POST', "/addhawK/", false);
    // xhr.setRequestHeader('X-CSRFToken', csrftoken); // csrf token improves security
    xhr.send(FD);
    // xhr.onreadystatechange = processRequest;
    var response = xhr.responseText;
    console.log(response);
    // after query is submitted display cancel query button
    document.getElementById("error").innerHTML = "Query Submitted!";
    document.getElementById("cancel_query").style.display = "block";
    return response
}

function getStatus(config_dict) {
    // get status of query from backend
    // takes in config_id_list
    // isCancelled = false;
    const xhr = new XMLHttpRequest();
    const csrftoken = document.querySelector('[name=csrfmiddlewaretoken]').value;
    xhr.open('POST', "/addhawkStatus/", false);
    xhr.setRequestHeader('X-CSRFToken', csrftoken);
    xhr.send(config_dict);
    try {
        var response = JSON.parse(xhr.responseText);
    }
    catch {
        // maybe possible first attempt returns null due to OSquery delay
        var response = { "status": "wait" };
    }
    console.log(response);

    if (response.status == "ready") {
        // when query is ready remove cancel query button
        document.getElementById("error").innerHTML = "Query Results Ready, fetching Result";
        document.getElementById("cancel_query").style.display = "none";
        // call to fill in table
        getResult(config_dict)
        return;
    }
    if (isCancelled) {
        // if is cancel query called stop query
        document.getElementById("error").innerHTML = "Query Cancelled";
        document.getElementById("cancel_query").style.display = "none";
        isCancelled = false;
        return;
    }
    // if status not ready send request again after 1 second
    setTimeout(function () {
        console.log("trying again...")
        getStatus(config_dict);
    }, 1000);
}

function getResult(config_dict) {
    // will get response of query from backend
    // takes config list as input and populate results
    const xhr = new XMLHttpRequest();
    const csrftoken = document.querySelector('[name=csrfmiddlewaretoken]').value;
    xhr.open('POST', "/addhawkResults/", false);
    xhr.setRequestHeader('X-CSRFToken', csrftoken);
    xhr.send(config_dict);
    var response = JSON.parse(xhr.responseText);
    let list = [];
    response = response.query_resp;
    for (let i = 0; i < response.length; i++) {
        // list.push({hostname:response[i].hostname})
        let fist = JSON.parse(response[i].data);
        console.log(fist.length);
        for (let index = 0; index < fist.length; index++) {
            (fist[index]).hostname = response[i].hostname;
            list.push((fist[index]));
        }
        // list=list.concat(JSON.parse(response[i].data))
    }
    // list = JSON.parse(response[0].data);
    document.getElementById("query_main").style.visibility = "visible";
    drawTable(list);
    // $("#qinput").on("keyup", function () {
    //     var value = $(this).val().toLowerCase();
    //     $("#qtable tr").filter(function () {
    //         $(this).toggle($(this).text().toLowerCase().indexOf(value) > -1)
    //     });
    // });
}

function querySubmit() {
    // isCancelled = false;
    hostfun();
    var q = document.getElementById("query").value;
    if (q && host.length != 0) {
        var scheduleResponse = sendData();
        var config_dict = new FormData();
        config_dict.append("config_ids", scheduleResponse);
        var isReady;
        getStatus(config_dict);
    }
    else {
        error();
    }
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
    columns = [{ field: "hostname", filter: true, resizable: true }];

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
// function downloadCSV(data) {
//     var items = data;
//     const replacer = (key, value) => value === null ? '' : value; // specify how you want to handle null values here
//     const header = Object.keys(items[0]);
//     let csv = items.map(row => header.map(fieldName => JSON.stringify(row[fieldName], replacer)).join(','));
//     csv.unshift(header.join(','));
//     csv = csv.join('\r\n');

//     // Current time
//     var today = new Date();
//     var date = today.getDate() + '-' + (today.getMonth() + 1) + '-' + today.getFullYear();
//     var time = parseInt(parseInt(today.getHours())%12) + "-" + today.getMinutes() + "-" + today.getSeconds();
//     var dateTime = date + '    ' + time;

//     //Download the file as CSV
//     var downloadLink = document.createElement("a");
//     var blob = new Blob(["\ufeff", csv]);
//     var url = URL.createObjectURL(blob);
//     downloadLink.href = url;
//     downloadLink.download = dateTime+".csv";  //Name the file here
//     document.body.appendChild(downloadLink);
//     downloadLink.click();
//     document.body.removeChild(downloadLink);
// }
