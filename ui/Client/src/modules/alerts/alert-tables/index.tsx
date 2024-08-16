/* eslint-disable jsx-a11y/anchor-is-valid */
import React, { useRef } from "react";
import { useState, useEffect } from "react";
import { ASSIGN_ALERT_USER_URL, EVENTS_LIST_URL, EVENTS_UPDATE_URL, GET_USER_LIST_URL } from "../../../apis/urls";
import TextField from '@mui/material/TextField';
import Button from 'react-bootstrap/Button';
import Collapse from 'react-bootstrap/Collapse';
import { useLazyFetch } from "../../../hooks";
import { Event} from "../../../types";
import Dropdown from 'react-bootstrap/Dropdown';
import DropdownButton from 'react-bootstrap/DropdownButton';
import { Link } from "react-router-dom";
import * as bootstrap from "bootstrap";
import swal from 'sweetalert'
import  loaderForTable from '../../../assets/images/loader-table.svg';
import Loader from "../../../components/loader";
import { updateEvent,getData, updateMe} from "../../../apis/requests";
import { fileName, logout, timeConverter } from "../../../helpers";
import Alert from '@mui/material/Alert';
import CsvDownloadButton from 'react-json-to-csv'
import Chip from '@mui/material/Chip';
import Button1 from '@mui/material/Button';
import DoNotTouchIcon from '@mui/icons-material/DoNotTouch';
import CancelIcon from '@mui/icons-material/Cancel';
import LoadingButton from '@mui/lab/LoadingButton';
import SettingsIcon from '@mui/icons-material/Settings'
import { Error } from "../../../components/Error";
import { toast, ToastContainer } from "react-toastify";
import { Accordion, AccordionSummary, AccordionDetails, Typography } from '@mui/material';


const AlertTable = (params:any) => {
  
  let url = EVENTS_LIST_URL;
  let flag=false;
  // 1629170374
  const [ispast, setispast] = useState(0);
  const [isopen, setIsopen] = useState(true);
  const [isrow, setisrow] = useState(100);
  const [ispageNo, setispageNo] = useState(1);
  const [selectedAlerts, setSelectedAlerts] = useState<any>([])
  const [selectedGroups, setSelectedGroups] = useState<any>([]);
  const [comment, setComment] = useState('');
  const [showLoading, setShowLoading] = useState(false);
  const closeAlertModalRef = useRef() as React.MutableRefObject<HTMLInputElement>;
  const [contextData, setContextData] = useState<any>(undefined);
  const [HostName, setHostName] = useState<String | null>(null);
  const [alertId, setAlertId] = useState<String | null>(null);
  const [searchAlert, setSearchAlert] = useState<String | null>(null);
  const [SearchHostName, setSearchHostName] = useState<String | null>(null);
  const [filter_enroll_time_btn, setFilter_enroll_time_btn] = useState(false);
  const [SearchStartTime, setSearchStartTime] = useState<any | null>(null);
  const [SearchEndTime, setSearchEndTime] = useState<any | null>(null);
  const [myAlert, setMyAlert] = useState<boolean>(false);
  const [userList, setUserList] = useState<any>(undefined);
  const [selectedUser, setSelectedUser] = useState<string>("Assigned to");
  const [alertS, setAlertS] = useState<boolean>(false)
  const [alertData, setAlertData] = useState<any>(undefined)
  const [alertGroupCount, setAlertGroupCount] = useState(0);
  const UpdateAlert = (eventid:string, action:string) =>{ 
    fetch(EVENTS_UPDATE_URL, {
      method: "POST",
      headers: {
        Authorization: "Token " + localStorage.getItem("Token"),
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        event_id: [parseInt(eventid)],
        action: action,
      }),
    }).then((res)=>{
      setAlertS(!alertS)
    })
  }

  useEffect(() => {
    if(userList===undefined)
    {
      getData(GET_USER_LIST_URL,setUserList)
    }
    if (ispast !== 0) {
      //const currentTimeInMilliseconds = Date.now(); // unix timestamp in milliseconds
      var today = new Date();
      const priorDate = Math.floor(
        new Date().setDate(today.getDate() - ispast) / 1000.0
      );
      var todayt = Math.floor(today.getTime() / 1000.0);

      if (isopen) {
        flag=true;
        url =
          EVENTS_LIST_URL + "?start_time=" + priorDate + "&end_time=" + todayt;
      } else {
        flag=true;
        url =
          EVENTS_LIST_URL +
          "?is_open=false&start_time=" +
          priorDate +
          "&end_time=" +
          todayt;
      }
    } else {
      if (isopen) {
        url = EVENTS_LIST_URL;
      } else {
        flag=true;
        url = EVENTS_LIST_URL + "?is_open=false";
      }
    }
    if (ispageNo > 1) {
      if (isrow !== 0) {
        if(flag)
        {
          url = url + "&pageno=" + ispageNo + "&rows=" + isrow;
        }
        else{
          url = url + "?pageno=" + ispageNo + "&rows=" + isrow;
        }

      } else {
        if(flag)
        {
          url = url + "&pageno=" + ispageNo;
        }
        else{
          url = url + "?pageno=" + ispageNo;
        }

      }
      // console.log(ispageNo);
    } else if(isrow!==0){
      if(flag)
      {
        url = url + "&rows=" + isrow;
      }
      else{
        url = url + "?rows=" + isrow;
      }

    }
    if(SearchHostName!==null)
    {
      if(url.endsWith('/'))
      {
        url = EVENTS_LIST_URL+"?host_identifier="+SearchHostName;
      }
      else
      {
        url = url+"&host_identifier="+SearchHostName;
      }
    }
    if(SearchStartTime!==null)
    {
      if((typeof SearchStartTime)=='object')
      {
        if(url.endsWith('/'))
        {
          url = EVENTS_LIST_URL+"?start_time="+String(SearchStartTime.unixTime);
        }
        else
        {
          url = url+"&start_time="+String(SearchStartTime.unixTime);
        }
      }
    }
    if(SearchEndTime!==null)
    {
      if((typeof SearchEndTime)=='object')
      {

        if(url.endsWith('/'))
        {
          url = EVENTS_LIST_URL+"?end_time="+String(SearchEndTime.unixTime);
        }
        else
        {
          url = url+"&end_time="+String(SearchEndTime.unixTime);
        }
      }
    }
    if(myAlert)
    {
      url = EVENTS_LIST_URL+'myalert';
    }
    run({ URL: url });
  }, [isopen, ispast, isrow,alertS, alertData, userList, ispageNo,SearchHostName,SearchEndTime,SearchStartTime,myAlert,params?.notification]);

  if(params?.notification>0)
  {
    setTimeout(() => {
      params?.setNotification(0)
    }, 8000);
  }

  const { loading, error, data, run } = useLazyFetch<{
    events: Event[];
    no_of_records: number;
  }>();

  const handleAlert = (req:any)=>{
    updateEvent(req)
    .then((res) => {
      setShowLoading(false)
      setSelectedAlerts([])
      // console.log('close alert response : ', res)
      if(res.status == "Success"){
        swal('', res.message,'success').then(updated => updated ? window.location.reload() : '')
      }else{
        alert(res.message)
      }
    })
    .catch((err) => {
      setShowLoading(false)
      // console.log('Error close alert : ', err)
      alert(err)
    })
  }

  const keyFinder= (map: Map<string, any>, maxTimeDifference: number, keyFirstHalf: string, alertTime: number)=>{

    const [alert_name, alert_host_identifier] = keyFirstHalf.split('_');

    for (const key of map.keys()) {
      const components = key.split('_');
      let [event_code, host_identifier, timeIdentifierStr] = components;
      if ((alert_name == event_code) && (alert_host_identifier == host_identifier)){
        let timeIdentifier = parseInt(timeIdentifierStr, 10);
        timeIdentifier = timeIdentifier - (timeIdentifier%60);

        const timeDifference =  timeIdentifier - alertTime;

        if (timeDifference >=0  && timeDifference <= maxTimeDifference) {
          return key;
        }
      }
    }

    const key = [alert_name, alert_host_identifier, alertTime];
    return key.join('_');

  }

  const eventGrouper = (events: any, maxTimeDiffMins: number) => {
    const maxTimeDifference = maxTimeDiffMins * 60;
    const eventGroups: any[][] = [];

    // Create a map to store events grouped by event_code and host_identifier
    const eventMap = new Map<string, any[]>();
    let timeIdentifier ;
    for (const event of events) {
      timeIdentifier = event.unixtime - (event.unixtime % 60); // ---> Time in minutes
      let keyFirstHalf = `${event.event_code}_${event.host_identifier}`;

      const key = keyFinder(eventMap, maxTimeDifference, keyFirstHalf, timeIdentifier);

      if (!eventMap.has(key)) {
        // Create a new group for this key
        eventMap.set(key, [event]);
      } else {
        // Find the group for this key
        const group = eventMap.get(key);

        if (group) {
          // Check if the event can be added to the current group
          let canAdd = true;

          const groupEvent = group[0];
          // Calculate the time difference in milliseconds
          const timeDifference = Math.abs(event.unixtime - groupEvent.unixtime);

          // Check if the time difference is less than or equal to 5 minutes (300,000 milliseconds)
          if (timeDifference > maxTimeDifference) {
            canAdd = false;
            break;
          }


          if (canAdd) {
            group.push(event);
          } else {
            // Start a new group for this key
            eventMap.set(key, [event]);
          }
        }
      }
    }

   const eventArray: any[][] = Array.from(eventMap.values());
    // Convert the map values to an array of event groups
    let prefixCount = 0;
    let index = 0;
    let flag = true;
    for (let group of eventArray) {
      group.sort((a,b) => b.unixtime - a.unixtime); // Sorting as per unixtime.. ID can be used instead
      eventGroups.push(group);
      prefixCount += group.length;
      if(params?.notification > prefixCount && flag){
        setAlertGroupCount(index);
        flag = false;
      }
      index++;
    }

    return eventGroups;
  };

  const handleManageAlert = (sUser: string, sAlerts: any[]) =>{
    let AlertList:any=[];
    let userID:string="";

    if(sAlerts.length <=0){
      toast.error('Alert should be selected first');
      return;
    }
    if(sUser==="Assigned to"){
      toast.error('Alert should be assigned to someone');
      return;
    }
    sAlerts.map((ob:any)=>{
      AlertList.push(parseInt(ob.id))
    })

    userList.data.map((user:any)=>{
      if(user.username===sUser)
      {
        userID=user.id
      }
    })
    const reqObj = {"user_id":userID,"event_id":AlertList};
 
    setAlertS(true)

    updateMe(reqObj, ASSIGN_ALERT_USER_URL,setAlertData)

    setSelectedAlerts([])
    // window.location.reload()
  }
  function getResults() {
    // let obj1 = {
    //   URL: EVENTS_LIST_URL,
    //   init: {
    //     method: "GET",
    //     //body: JSON.stringify({ query }),
    //     headers: getDefaultHeaders({
    //       "Content-Type": "text/plain",
    //     }),
    //   },
    // };
    // let obj2 = {
    //   URL: EVENTS_LIST_URL+'?is_open=false',
    //   init: {
    //     method: "GET",
    //     //body: JSON.stringify({ query }),
    //     headers: getDefaultHeaders({
    //       "Content-Type": "text/plain",
    //     }),
    //   },
    // };

    if (loading) {
      return <div className="loading-table-gif">
                <img src={loaderForTable} />
              </div>;
    } else if (error) {
      if(error.message ==='Unauthorized'){
        <div className="">
          Unauthorized
        </div>;
        // logout();
        return ;
      }
      return <p>{error.message}</p>;
    } else if (data) {
      let finalData = data.events;
      // console.log(finalData);
      if (!finalData) return (<Error message="No alerts available"/>);
      if (finalData.length <= 0) return (<Error message="No alerts available"/>);

      let headingsToShow: Array<keyof Omit<Event, "metadata">> = Object.keys(
        finalData[0]
      ).filter((heading) => heading !== "metadata") as Array<
        keyof Omit<Event, "metadata">
      >;
      //console.log(headingsToShow);

      return (
        <table className="table table-hover styleScroll">
          <thead
            className="bg-white"
            style={{ position: "sticky", top: 0, zIndex: 950 }}
          >
            <tr className="fw-light text-muted">
              <th scope="row">
                <div style={{height:"4vh", display:"flex", alignItems:"center"}}>
                  <input
                    className="form-check-input flex-shrink-0"
                    type="checkbox"
                    value=""
                  />
                </div>
              </th>
              <th className="fw-light col-1" scope="col">
                <TextField id="standard-basic" disabled={true} label="OS" variant="standard" InputProps={{disableUnderline: true}}/>
              </th>
              {/* <th className="fw-light" scope="col">
                {alertId!==null?
                  (<TextField id="standard-basic" label={<>Alert Id<i className="fa fa-search chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>} variant="standard" value={String(alertId)} style={{width:"100%"}} InputProps={{disableUnderline: true}}
                  onChange={
                    (e)=>{
                      setAlertId((e.target as HTMLInputElement).value)
                    }
                  }
                  onKeyUp={
                      (e)=>{
                        if (e.key === 'Enter') {
                          setSearchAlert(alertId)
                      }      
                      }
                    }/>):(<TextField id="standard-basic" label={<>Alert Id<i className="fa fa-search chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>} variant="standard" style={{width:"100%"}} InputProps={{disableUnderline: true}}
                    onChange={
                      (e)=>{
                          setAlertId((e.target as HTMLInputElement).value)
                      }
                    }
                    onKeyUp={
                        (e)=>{
                          if (e.key === 'Enter') {
                            setSearchAlert(alertId)
                        }
                        }
                      }/>)
                  }
              </th> */}
              <th className="fw-light col-4" scope="col">
                <TextField id="standard-basic" disabled={true} label="Alert Name" variant="standard" InputProps={{disableUnderline: true}}/>
              </th>
              <th className="fw-light" scope="col">
                <TextField id="standard-basic" disabled={true} label="Severity" style={{width:"3vw"}} variant="standard" InputProps={{disableUnderline: true}}/>
              </th>
              <th className="fw-light" scope="col">
                {
                  filter_enroll_time_btn===false?
                  <Button
                    onClick={() => setFilter_enroll_time_btn(!filter_enroll_time_btn)}
                    aria-controls="example1-collapse-text"
                    aria-expanded={filter_enroll_time_btn}
                    style={{border:'none',backgroundColor:'transparent',color:'grey',padding:0,outline:'none',height:"3vh"}}
                    className="btn_y"
                  >
                  {
                    filter_enroll_time_btn?(""):(<>Captured on<i className="fa fa-search chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>)
                  }
                  </Button>:<></>
                }
                <Collapse in={filter_enroll_time_btn}>
                  <div id="example1-collapse-text" style={{marginTop:"0.5rem"}}>
                  <div className="d-flex align-items-center">
                  {
                    SearchStartTime!==null?
                    (<TextField
                      id="datetime-local"
                      label="Start Date"
                      type="datetime-local"
                      defaultValue={SearchStartTime.Time}
                      sx={{ width: 250 }}
                      InputLabelProps={{
                        shrink: true,
                      }}
                      onKeyPress={(e)=>{
                        if (e.key === 'Enter') {
                        let time=Date.parse((e.target as HTMLInputElement).value)/1000;
                        setSearchStartTime(
                        {
                          unixTime:time,
                          Time:(e.target as HTMLInputElement).value
                        })
                      }
                      }}
                      // onChange={(e)=>{setSearchStartTime(Date.parse((e.target as HTMLInputElement).value)/1000)}}
                    />):(<TextField
                    id="datetime-local"
                    label="Start Date"
                    type="datetime-local"
                    sx={{ width: 250 }}
                    InputLabelProps={{
                      shrink: true,
                    }}
                    onKeyPress={(e)=>{
                      if (e.key === 'Enter') {
                      let time=Date.parse((e.target as HTMLInputElement).value)/1000;
                      setSearchStartTime(
                      {
                        unixTime:time,
                        Time:(e.target as HTMLInputElement).value
                      })
                    }}}
                    // onChange={(e)=>{setSearchStartTime(Date.parse((e.target as HTMLInputElement).value)/1000)}}
                  />)
                }
                <span className="Box" 
                onClick={()=>{setFilter_enroll_time_btn(!filter_enroll_time_btn);setSearchStartTime(null);setSearchEndTime(null)}} 
                style={{marginLeft:"2rem",fontSize:"2rem",cursor:'pointer'}}><i className="fa fa-times-circle" aria-hidden="true"></i></span>
                </div>
                </div>
              </Collapse>
              </th>
              <th className="fw-light" scope="col">
              {HostName!==null?
                (<TextField id="standard-basic" label={<>Host Name<i className="fa fa-search chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>} variant="standard" style={{width:"100%"}} value={String(HostName)} InputProps={{disableUnderline: true}}
                onChange={
                  (e)=>{
                      setHostName((e.target as HTMLInputElement).value)
                  }
                }
                onKeyUp={
                    (e)=>{
                      if (e.key === 'Enter') {
                        setSearchHostName(HostName)
                    }
                    }
                  }/>):(<TextField id="standard-basic" label={<>Host Name<i className="fa fa-search chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>} variant="standard" style={{width:"100%"}} InputProps={{disableUnderline: true}}
                  onChange={
                    (e)=>{
                        setHostName((e.target as HTMLInputElement).value)
                    }
                  }
                  onKeyUp={
                      (e)=>{
                        if (e.key === 'Enter') {
                          setSearchHostName(HostName)
                      }
                      }
                    }/>)
                }
              </th>

              {/* <th className="fw-light" scope="col">
                Status
              </th> */}
              <th className="fw-light" scope="col">
                <TextField id="standard-basic" disabled={true} label="Status" style={{width:"3vw"}} variant="standard" InputProps={{disableUnderline: true}}/>
              </th>
              <th className="fw-light" scope="col">
                <TextField id="standard-basic" disabled={true} label="View" style={{width:"3vw"}} variant="standard" InputProps={{disableUnderline: true}}/>
              </th>
              <th className="fw-light" scope="col">
                <TextField id="standard-basic" disabled={true} label="Action" style={{width:"4vw"}} variant="standard" InputProps={{disableUnderline: true}}/>
              </th>
            </tr>
          </thead>
          <tbody className="border-top-0">

          {eventGrouper(finalData, 5).map((group, groupIndex) => (
                  <tr key={groupIndex} className="m-0">
                    <td colSpan={12} className="p-0">
                  <Accordion className="shadow-none p-0">
                    <AccordionSummary key={groupIndex} className={ group.filter(((item:any) => item.id === sessionStorage.getItem('lastVisitedAlert'))).length >0? "bg-primary bg-opacity-10 p-0" :"p-0"}>
                      <table className="w-100 table-hover table-borderless styleScroll mb-0">
                        <tbody>
                        <tr>
                        <td className="fw-light col-1" scope="col">
                          <div className="d-flex align-items-center">
                            <input id={group[0].id} type="checkbox" className="mx-3" onChange={(e) => {
                              handleGroupCheckboxChange(e, group)
                            }} checked={selectedGroups.includes(group[0].id)}/>
                            {group[0].os_name === "Windows" ? (
                              <i className="fa fa-windows" data-tip data-for={group[0].os_name} style={{ fontSize: "2rem" }}></i>
                            ) : (
                              <i className="fa fa-linux" data-tip data-for={group[0].os_name} style={{ fontSize: "2rem" }}></i>
                            )}
                          </div>
                        </td>
                        {
                            group[0].link.match(/http+/g)!==null? (groupIndex<=alertGroupCount && params?.notification!=0)?
                            <td className="normalisedLink col-4"><Link to={group[0].link} target="_blank" rel="noopener noreferrer">{group[0].event_code}<Chip variant="outlined" label="new" size="small" color="primary" className="ms-2"/></Link></td>:<td className="normalisedLink col-4"><Link to={group[0].link} target="_blank" rel="noopener noreferrer">{group[0].event_code}</Link></td>
                            :(groupIndex<=alertGroupCount && params?.notification!=0)?
                            <td className="normalisedLink col-4">{group[0].event_code}<Chip variant="outlined" label="new" size="small" color="primary" className="ms-2"/></td>:<td className="normalisedLink col-4">{group[0].event_code}</td>
                          }
                        <td className="col-1">
                            {group[0].severity == "4"? (
                              <span className="fs-7 badge bg-danger  text-white">
                                High
                              </span>
                            ) : group[0].severity == "3"? (
                              <span className="fs-7 badge bg-warning  text-white">
                                Mid
                              </span>
                            ) : group[0].severity == "2"?(
                              <span className="fs-7 badge bg-orange  text-white">
                                Low
                              </span>
                            ):group[0].severity == "1"? (
                              <span className="fs-7 badge bg-teal  text-white">
                                Info
                              </span>
                            ):(
                              <span className="fs-7 badge bg-cyan  text-white">
                                None
                              </span>
                            )}
                          </td>
                          <td className="col-2">{timeConverter(parseInt(group[0].unixtime))}</td>
                          <td className="col-1">{group[0].hostname.length < 15? group[0].hostname: group[0].hostname.slice(0, 15) + "\n" + group[0].hostname.slice(15)}</td>
                          <td className="col-1"> {/*is _open */}</td>
                          <td className="col-2"> 
                          {/* <button
                                type="button"
                                className="btn btn-primary-light ntn-sm"
                              ><i className="fa fa-eye" aria-hidden="true"></i></button> */}
                              <i className="fa fa-angle-down" aria-hidden="true"></i>
                              </td>
                              <td className="col-1">{/*Action */}</td>

                              </tr>
                              </tbody>
                              </table>
                    </AccordionSummary>
                    <AccordionDetails className="p-0 ms-1">
                    <table className="w-100 table table-hover styleScroll">
                      <tbody className="border-top-0">
                    {group.map((data,ky) => {
                      return (
                        <tr key={ky} className={ sessionStorage.getItem('lastVisitedAlert') === data.id? "bg-dark bg-opacity-10" :""}>
                          <td className="fw-light col-1" scope="col">
                            <div className="d-flex align-items-center">
                              <input id={data.id} type="checkbox" onChange={(e) => putAlertsInBucket(e,data)} className="me-3"/>
                              {data.os_name === "Windows" ? (
                                <i className="fa fa-windows" data-tip data-for={data.os_name} style={{ fontSize: "2rem" }}></i>
                              ) : (
                                <i className="fa fa-linux" data-tip data-for={data.os_name} style={{ fontSize: "2rem" }}></i>
                              )}
                            </div>
                            </td>

                          {
                            data.link.match(/http+/g)!==null?ky<params?.notification?
                            <td className="normalisedLink col-4"><Link to={data.link} target="_blank" rel="noopener noreferrer">{data.event_code}<Chip variant="outlined" label="new" size="small" color="primary" className="ms-2"/></Link></td>:<td className="normalisedLink col-4"><Link to={data.link} target="_blank" rel="noopener noreferrer">{data.event_code}</Link></td>
                            :ky<params?.notification?
                            <td className="normalisedLink col-4">{data.event_code}<Chip variant="outlined" label="new" size="small" color="primary" className="ms-2"/></td>:<td className="normalisedLink col-4">{data.event_code}</td>
                          }
                          {/* <td>{data.id}</td> */}
                          <td className="">
                            {data.severity == "4"? (
                              <span className="fs-7 badge bg-danger  text-white">
                                High
                              </span>
                            ) : data.severity == "3"? (
                              <span className="fs-7 badge bg-warning  text-white">
                                Mid
                              </span>
                            ) : data.severity == "2"?(
                              <span className="fs-7 badge bg-orange  text-white">
                                Low
                              </span>
                            ):data.severity == "1"? (
                              <span className="fs-7 badge bg-teal  text-white">
                                Info
                              </span>
                            ):(
                              <span className="fs-7 badge bg-cyan  text-white">
                                None
                              </span>
                            )}
                          </td>
                          <td className="col-2">{timeConverter(parseInt(data.unixtime))}</td>
                          <td className="">{data.hostname}</td>
                          {/* {data.is_open == "t" ? <td>open</td> : <td>closed</td>} */}
                          <td className="">
                          {data.is_open != true
                              ? <Button1 
                                  onClick={() =>
                                    handleAlert({
                                      event_id: [parseInt(data.id)],
                                      remarks: "testing",
                                      is_open: true,
                                    })
                                  }
                                  variant="outlined"
                                  color="success"
                                  endIcon={<i className="fa fa-check-circle-o" aria-hidden="true"></i>}>
                                    Close
                                </Button1>
                              :
                                <Button1 
                                  onClick={() =>
                                    handleAlert({
                                      event_id: [parseInt(data.id)],
                                      remarks: "testing",
                                      is_open: false,
                                    })
                                  }
                                  variant="outlined"
                                  color="error"
                                  endIcon={<i className="fa fa-exclamation-triangle" aria-hidden="true"></i>}>
                                    Open
                                </Button1>
                              }
                            {/* <button
                              type="button"
                              className="btn btn-primary-light ntn-sm"
                              onClick={() =>
                                setIstoggle({
                                  event_id: data.id,
                                  remarks: "testing",
                                  state: data.is_open,
                                })
                              }
                            >
                              {data.is_open == "t"
                              ? <>Close <i className="fa fa-folder-o" aria-hidden="true"></i></>
                              : <>Open <i className="fa fa-folder-open-o" aria-hidden="true"></i></>
                              }

                            </button> */}
                          </td>
                          <td className="">
                            <Link to="/alertSheetView" state={data}>
                              {/* <Link to={{pathname:"/alertSheetView", data}}> */}
                              <button
                                type="button"
                                className="btn btn-primary-light ntn-sm"
                                onClick={() => {
                                  sessionStorage.setItem('lastVisitedAlert', data.id);
                                }}
                              >
                                <i className="fa fa-eye"></i>
                              </button>
                            </Link>
                          </td>
                          <td className="">
                            {
                              data.action==="None"?
                              <>
                              <div style={{display:"flex", gap:"0.5rem"}}>
                                <Button1 disabled={true} variant="contained" onClick={()=>{UpdateAlert(data.id,"Kill")}} size="small" color="error" startIcon={<CancelIcon />}>
                                {/* getLiveData(data.host_identifier,"select * from windows_vajra_agent where pid=",()=>{}) */}
                                  Kill
                                </Button1>
                                <Button1 disabled={true} onClick={()=>{UpdateAlert(data.id,"Block")}} size="small" variant="contained" color="warning" startIcon={<DoNotTouchIcon />}>
                                  Block
                                </Button1>
                              </div>
                              </>:
                              data.action==="Block"?
                                <LoadingButton
                                  disabled={true}
                                  loading
                                  loadingPosition="start"
                                  startIcon={<CancelIcon />}
                                  variant="contained"
                                  // onClick={()=>{UpdateAlert(data.id,"None")}}
                                  loadingIndicator={  
                                    <SettingsIcon className='spin' />
                                  }
                                >
                                  Blocking
                                </LoadingButton>
                              // <Button1 variant="contained" onClick={()=>{UpdateAlert(data.id,"Kill")}} size="small" color="error" startIcon={<CancelIcon />}>
                              //   Kill
                              // </Button1>
                              :
                              <Button1 disabled={true} variant="contained" onClick={()=>{UpdateAlert(data.id,"None")}} size="small" color="error" startIcon={<CancelIcon />}>
                                Process Killed
                              </Button1>
                            }
                          </td>
                        </tr>
                      );
                    })}
                    </tbody>
                    </table>

                    </AccordionDetails>
                  </Accordion>
                  </td>
                  </tr>
                ))}

          </tbody>
        </table>
      );
    }

    return null;
  }

  const handleFilterChange = (event:any) => {
    // console.log(event.target.value)
    setisrow(event.target.value)
  }

  const putAlertsInBucket = (e:any, alert:any) => {
    const isAlertChecked = e.target.checked;
    if(isAlertChecked){
      let alreadyExist = selectedAlerts.find((ele:any) => ele.id == alert.id)
      // console.log(alreadyExist)
      if(alreadyExist == null) setSelectedAlerts([...selectedAlerts, alert])
    }else{
      let alerts = selectedAlerts.filter((ele:any) => ele.id !== alert.id)
      setSelectedAlerts(alerts)
    }
  }

  const handleGroupCheckboxChange =(e:any, group: any)=>{
    const isChecked = e.target.checked;
    if (isChecked) {
      // If the group checkbox is checked, add its ID to selectedGroups
      setSelectedGroups([...selectedGroups, group[0].id]);

      // Add all data IDs from the group to selectedAlerts
      setSelectedAlerts([...selectedAlerts, group]);
    } else {
      // If the group checkbox is unchecked, remove its ID from selectedGroups
      setSelectedGroups(selectedGroups.filter((groupId:any) => groupId !== group[0].id));

      // Remove all data IDs from the group from selectedAlerts
      const groupDataIds = group.map((data:any) => data.id);
      setSelectedAlerts(selectedAlerts.filter((alertId:any) => !groupDataIds.includes(alertId)));
    }
  }

  const handleCloseAlerts = () => {
    // console.log(selectedAlerts)
    // console.log('handle close alerts')
    // check whethere any alert is selected or not
    if(selectedAlerts.length > 0){
      // check if is there any open category alert in list
      const isAnyOpenAlerts = selectedAlerts.some((ele:any) => ele.is_open == false);
      // console.log(isAnyOpenAlerts)
      if(!isAnyOpenAlerts){
        const modalEle = closeAlertModalRef.current
        const bsModal = new bootstrap.Modal(modalEle, {
            backdrop: 'static',
            keyboard: false
        })
        bsModal.toggle()
      }else{
        swal('', 'Please remove close category alerts', 'warning')
      }
    }else{
      swal('', 'Please select atleast 1 alert', 'warning')
    }
  }

  const closeAlerts = (e:any) => {
    e.preventDefault();
    // setShowLoading(true);
    // Find events id for close
    let eventsId = selectedAlerts.map((event:any) => parseInt(event.id))
    console.log(eventsId)
    let req = {
      "event_id": eventsId,
      "remarks": comment,
      "is_open": false
    }
    // console.log('event update-close request: ', req)
    handleAlert(req);
  }

  return (
    showLoading ? 
      <Loader />
      :
      <>
      <ToastContainer/>
      {
        alertS?alertData!==undefined?alertData.status==="Success"?(<Alert onClose={() => {setAlertS(false)}}>{alertData.message}</Alert>):(<Alert  onClose={() => {setAlertS(false)}} severity="error">{alertData.message}</Alert>):(<></>):(<></>)
      }
      {/* <Banner name="Alerts Status"/> */}
      {/* <div className="card card-body">
        <div className="hstack">
          <div className="d-flex flex-column justify-content-start" style={{width:"12vw"}}>
            <h3 className="fw-mid my-0">Alerts Status</h3>
            <div>
              <div className="mt-2">
                <button
                  className="fs-8 px-1 btn btn-link text-muted"
                  onClick={() => setispast(7)}
                >
                  7 Days
                </button>
                <button
                  className="fs-8 px-1 btn btn-link text-muted active"
                  onClick={() => setispast(15)}
                >
                  15 Days
                </button>
                <button
                  className="fs-8 px-1 btn btn-link text-muted"
                  onClick={() => setispast(180)}
                >
                  180 Days
                </button>
              </div>
            </div>
          </div>
        </div>
      </div> */}
      <div className="card py-3">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Alerts Table</h4>
            </div>
            <div className="ms-auto">
              <button
                type="button"
                className="btn btn-primary-light align-middle text-primary"
                data-bs-toggle="modal"
                data-bs-target="#ManageAlertsModal"
              >
                <i className="fa fa-users me-2 align-middle"></i>Manage Alerts
              </button>
              {/* <button className="btn btn-primary-light align-middle text-primary">
              <i className="fi fi-rr-plus me-2 align-middle"></i>Add New Alert
            </button> */}
            </div>
          </div>
        </div>
        <div className="card-body px-6">
          <div className="d-flex flex-column align-items-start gap-3">
            <div className="d-flex flex-column flex-md-row justify-content-between align-items-center gap-3 w-100">
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
                        onClick={() => {
                          if (!isopen) {
                            setIsopen(true);
                          }
                          if (ispast !== 0) {
                            setispast(0);
                          }
                          setMyAlert(false)
                          //getResults(true);
                        }}
                      >
                        Latest Alerts
                      </button>
                    </li>
                    <li className="nav-item" role="presentation">
                      <button
                        className="nav-link"
                        id="pills-contact-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-contact"
                        type="button"
                        role="tab"
                        aria-controls="pills-contact"
                        aria-selected="false"
                        onClick={() => {
                          // is_open='false';
                          if (isopen) {
                            setIsopen(false);
                          }
                          if (ispast !== 0) {
                            setispast(0);
                          }
                          setMyAlert(false)
                          // setIsopen(false);
                          // setispast(0);
                          //getResults(false);
                        }}
                      >
                        Responded Alerts
                      </button>
                    </li>
                    <li className="nav-item" role="presentation">
                      <button
                        className="nav-link"
                        id="pills-disabled-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-disabled"
                        type="button"
                        role="tab"
                        aria-controls="pills-disabled"
                        aria-selected="false"
                        onClick={()=>{
                          setMyAlert(true)
                        }}
                        // disabled
                      >
                        My Alerts
                      </button>
                    </li>
                    <li className="nav-item" role="presentation">
                      <button
                        className="nav-link"
                        id="pills-home-tab"
                        data-bs-toggle="pill"
                        data-bs-target="#pills-disabled"
                        type="button"
                        role="tab"
                        aria-controls="pills-disabled"
                        aria-selected="false"
                        disabled
                      >
                        Team Alerts
                      </button>
                    </li>
                  </ul>
                  {/* <ul className="nav nav-pills">
                    <li className="nav-item">
                      <a
                        className="nav-link px-2 me-2 active"
                        aria-current="page"
                        href="#"
                        onClick={() => {
                          if (!isopen) {
                            setIsopen(true);
                          }
                          if (ispast != 0) {
                            setispast(0);
                          }
                          //getResults(true);
                        }}
                      >
                        Latest Alerts
                      </a>
                    </li>
                    <li className="nav-item">
                      <a
                        className="nav-link px-2 me-2"
                        href="#"
                        onClick={() => {
                          // is_open='false';
                          if (isopen) {
                            setIsopen(false);
                          }
                          if (ispast != 0) {
                            setispast(0);
                          }
                          // setIsopen(false);
                          // setispast(0);
                          //getResults(false);
                        }}
                      >
                        Closed Alerts
                      </a>
                    </li>
                    <li className="nav-item">
                      <a className="nav-link px-2 me-2" href="#">
                        My Alerts
                      </a>
                    </li>
                    <li className="nav-item">
                      <a className="nav-link px-2 me-2" href="#">
                        Team Alerts
                      </a>
                    </li>
                  </ul> */}
                </div>
              </div>
              <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                <nav aria-label="Standard pagination example">
                  <ul className="pagination pagination-sm mb-0">
                    <li className="page-item">
                      <a
                        className="page-link"
                        href="#"
                        aria-label="Previous"
                        onClick={() => {
                          if (ispageNo > 1) {
                            setispageNo(ispageNo - 1);
                          }
                        }}
                      >
                        {" "}
                        <span aria-hidden="true">« Prev</span>{" "}
                      </a>
                    </li>
                    <li className="page-item">
                      <a
                        className="page-link"
                        href="#"
                        aria-label="Next"
                        onClick={() => setispageNo(ispageNo + 1)}
                      >
                        {" "}
                        <span aria-hidden="true">Next »</span>{" "}
                      </a>
                    </li>
                  </ul>
                </nav>
                <div className="form-group">
                  <label className="text-muted">No. of rows: &nbsp;</label>
                  <select className="form-select-sm p-1" onChange={handleFilterChange} value={isrow} style={{border: '1px solid silver'}}>
                    <option value="10">10</option>
                    <option value="25">25</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                  </select>
                </div>
               
                {/* <nav aria-label="Standard pagination example">
                  <ul className="pagination pagination-sm mb-0">
                    No. of rows
                    <li className="page-item">
                      <a
                        className="page-link"
                        href="#"
                        aria-label="Rows2"
                        onClick={() => setisrow(2)}
                      >
                        {" "}
                        <span aria-hidden="true">2</span>{" "}
                      </a>
                    </li>
                    <li className="page-item">
                      <a
                        className="page-link"
                        href="#"
                        aria-label="Rows5"
                        onClick={() => setisrow(5)}
                      >
                        {" "}
                        <span aria-hidden="true">5</span>{" "}
                      </a>
                    </li>
                    <li className="page-item">
                      <a
                        className="page-link"
                        href="#"
                        aria-label="Rows100"
                        onClick={() => setisrow(100)}
                      >
                        {" "}
                        <span aria-hidden="true">100</span>{" "}
                      </a>
                    </li>
                  </ul>
                </nav> */}
                <button
                  className="btn btn-link fw-semi text-muted btn-sm"
                  onClick={() => {
                    setIsopen(true);
                    setispast(0);
                    setisrow(100);
                    setispageNo(1);
                    setSelectedAlerts([])
                    setComment('');
                    setShowLoading(false);
                    setContextData(undefined);
                    setHostName(null);
                    setAlertId(null);
                    setSearchHostName(null);
                    setFilter_enroll_time_btn(false);
                    setSearchStartTime(null);
                    setSearchEndTime(null);
                    sessionStorage.removeItem('lastVisitedAlert');
                    // console.log("refresh")
                    // if (!isopen) {
                    //   setIsopen(true);
                    // }
                    // if (ispast != 0) {
                    //   setispast(0);
                    // }
                    // getResults();
                  }}
                >
                  Refresh Alerts
                </button>
                {/* <div className="d-flex justify-content-start align-items-center">
                  <button
                    type="button"
                    className="btn btn-link fw-semi btn-sm text-dark"
                    data-bs-toggle="modal"
                    data-bs-target="#ManageAlertsModal"
                  >
                    <i className="fi fi-rr-edit me-2 align-middle"></i>Add Notes
                  </button>
                </div> */}
                <div
                  className="modal fade"
                  id="ManageAlertsModal"
                  tabIndex={-1}
                  aria-labelledby="ManageAlertsModalLabel"
                  aria-hidden="true"
                >
                  <div className="modal-dialog">
                    <div className="modal-content">
                      <div className="modal-header">
                        <h5 className="modal-title" id="ManageAlertsModalLabel">
                          Manage Alerts
                        </h5>
                        <button
                          type="button"
                          className="btn-close"
                          data-bs-dismiss="modal"
                          aria-label="Close"
                        ></button>
                      </div>
                      <div className="modal-body">
                        <form>
                          <div className="form-floating mb-4">
                            <DropdownButton id="dropdown-basic-button" title={selectedUser}>
                              {
                                userList ? userList?.data?.map((user:any,_:number)=>{ //userList.data is the array
                                  return <Dropdown.Item key={_} onClick={(e:any)=>{
                                    setSelectedUser(e.target.text)
                                  }}>{user?.username}</Dropdown.Item>
                                }):<></>
                              }
                            </DropdownButton>
                          </div>
                          <div className="form-floating mb-4">
                            <input
                              type="text"
                              className="form-control"
                              id="floatingInput"
                              placeholder="Notes"
                            />
                            <label htmlFor="floatingInput">Add Notes</label>
                          </div>
                        </form>
                      </div>
                      <div className="modal-footer">
                        <button
                          type="button"
                          className="btn btn-secondary"
                          data-bs-dismiss="modal"
                        >
                          Close
                        </button>
                        <button type="button" data-bs-dismiss="modal" onClick={()=>{
                          handleManageAlert(selectedUser, selectedAlerts)
                          console.log("After handle");}} className="btn btn-primary">
                          Save changes
                        </button>
                      </div>
                    </div>
                  </div>
                </div>
                <div className="vr h-8px my-auto"></div>
                <div className="d-flex justify-content-start align-items-center">
                  <button
                    type="button"
                    className="btn btn-link fw-semi btn-sm text-dark"
                    // data-bs-toggle="modal"
                    // data-bs-target="#CloseAlertModal"
                    onClick={handleCloseAlerts}
                  >
                    <i className="fa fa-shield me-2 align-middle"></i>
                    Respond Alerts
                  </button>
                  {/*  */}

                  <div
                    className="modal fade"
                    ref={closeAlertModalRef}
                    id="CloseAlertModal"
                    tabIndex={-1}
                    aria-labelledby="CloseAlertModalLabel"
                    aria-hidden="true"
                  >
                    <div className="modal-dialog">
                      <div className="modal-content">
                        <form onSubmit={closeAlerts}>
                          <div className="modal-header">
                            <h5 className="modal-title" id="CloseAlertModalLabel">
                              Are you sure you want to Close the event
                            </h5>
                            <button
                              type="button"
                              className="btn-close"
                              data-bs-dismiss="modal"
                              aria-label="Close"
                            ></button>
                          </div>
                          <div className="modal-body">
                            <div className="form-floating mb-4 has-validatio">
                              <input
                                type="text"
                                className="form-control"
                                id="floatingInput"
                                placeholder="comment"
                                value={comment}
                                onChange={(e) => setComment(e.target.value)}
                                required
                              />
                              <label htmlFor="floatingInput">Comment</label>
                            </div>
                          </div>
                          <div className="modal-footer">
                            <button
                              type="button"
                              className="btn btn-secondary"
                              data-bs-dismiss="modal"
                            >
                              Close
                            </button>
                            <button type="submit" className="btn btn-primary" data-bs-dismiss="modal">
                              Save changes
                            </button>
                          </div>
                        </form>
                      </div>
                    </div>
                  </div>
                  {/*  */}
                </div>
                {/* <div className="vr h-8px my-auto"></div>
              <div className="d-flex justify-content-start align-items-center">
                <button
                  type="button"
                  className="btn btn-link fw-semi btn-sm text-dark"
                >
                  <i className="fi fi-rr-settings me-2 align-middle"></i>
                  Settings
                </button>
              </div> */}
                <div className="vr h-8px my-auto"></div>
                {
                  data!==null?
                  <CsvDownloadButton className="btn btn-sm btn-outline-secondary text-dark" filename={fileName('Alerts')} data={data?.events} >
                    <i className="fa fa-download me-2 align-middle"></i>Export Alerts
                  </CsvDownloadButton>:<></>
                }
                {/* <ExportAlert /> */}
              </div>
            </div>
            <div
              className="w-100 overflow-auto styleScroll"
              style={{ height: "calc(100vh - 25rem)" }}
              id="#scrollbar-1"
            >
              {getResults()}
            </div>
          </div>
        </div>
      </div>
    </>
  );
};

export default AlertTable;
