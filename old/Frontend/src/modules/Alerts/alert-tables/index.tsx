/* eslint-disable jsx-a11y/anchor-is-valid */
import React, { useRef } from "react";
import axios from "axios";
//import { TOKEN } from "../../../apis/base-urls";
import { useState, useEffect } from "react";
import { ASSIGN_ALERT_USER_URL, EVENTS_LIST_URL, EVENTS_UPDATE_URL, GET_USER_LIST_URL } from "../../../apis/urls";
import TextField from '@mui/material/TextField';
import Button from 'react-bootstrap/Button';
import Collapse from 'react-bootstrap/Collapse';
import no_data from "../../../assets/images/data_not_found.png";
import { getDefaultHeaders, useFetchURL, useLazyFetch } from "../../../hooks";
import { Event, toggle_state } from "../../../types";
import ExportAlert from "../export-alert";
import ViewModal from "../view-modal";
import AlertSheetView from "../../../pages/alertSheetView";
import InputLabel from '@mui/material/InputLabel';
import MenuItem from '@mui/material/MenuItem';
import FormControl from '@mui/material/FormControl';
import Select, { SelectChangeEvent } from '@mui/material/Select';
import Dropdown from 'react-bootstrap/Dropdown';
import DropdownButton from 'react-bootstrap/DropdownButton';
import { Link } from "react-router-dom";
import { Navigate } from "react-router";
import * as bootstrap from "bootstrap";
import swal from 'sweetalert'
import  loaderForTable from '../../../assets/images/loader-table.svg';
import Loader from "../../../components/loader";
import { updateEvent,getData, updateMe } from "../../../apis/requests";
import { hostname } from "os";
import { timeConverter } from "../../../helpers";
import Alert from '@mui/material/Alert';

const AlertTable = () => {
  
  let url = EVENTS_LIST_URL;
  let flag=false;
  // 1629170374
  const [ispast, setispast] = useState(0);
  const [isopen, setIsopen] = useState(true);
  const [istoggle, setIstoggle] = useState<toggle_state | null>(null);
  const [isrow, setisrow] = useState(100);
  const [ispageNo, setispageNo] = useState(1);
  const [selectedAlerts, setSelectedAlerts] = useState<any>([])
  const [comment, setComment] = useState('');
  const [showLoading, setShowLoading] = useState(false);
  const closeAlertModalRef = useRef() as React.MutableRefObject<HTMLInputElement>;
  const [contextData, setContextData] = useState<any>(undefined);
  const [HostName, setHostName] = useState<String | null>(null);
  const [HostIdentifier, setHostIdentifier] = useState<String | null>(null);
  const [SearchHostName, setSearchHostName] = useState<String | null>(null);
  const [filter_enroll_time_btn, setFilter_enroll_time_btn] = useState(false);
  const [SearchStartTime, setSearchStartTime] = useState<any | null>(null);
  const [SearchEndTime, setSearchEndTime] = useState<any | null>(null);
  const [myAlert, setMyAlert] = useState<boolean>(false);
  const [userList, setUserList] = useState<any>(undefined);
  const [selectedUser, setSelectedUser] = useState<string>("Assigned to");
  const [alertS, setAlertS] = useState<boolean>(false)
  const [alertData, setAlertData] = useState<any>(undefined)

  useEffect(() => {
    if(userList===undefined)
    {
      getData(GET_USER_LIST_URL,setUserList)
    }
    if (istoggle) {
      // console.log("toggled");
      // console.log(istoggle.event_id);
      // console.log(istoggle.remarks);
      // console.log(istoggle.state);
      let event_id = istoggle.event_id;
      let remarks = istoggle.remarks;
      let is_open: boolean = istoggle.state == "t" ? false : true;

      const fetchToggleUpdate = fetch(EVENTS_UPDATE_URL, {
        method: "POST",
        headers: {
          Authorization: "Token " + localStorage.getItem("Token"),
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          event_id: [parseInt(event_id)],
          remarks: remarks,
          is_open: is_open,
        }),
      });
      // console.log(fetchToggleUpdate);
    }
    if (ispast != 0) {
      //const currentTimeInMilliseconds = Date.now(); // unix timestamp in milliseconds
      var today = new Date();
      const priorDate = Math.floor(
        new Date().setDate(today.getDate() - ispast) / 1000.0
      );
      var todayt = Math.floor(today.getTime() / 1000.0);

      //var eime=new Date().getTime();

      // console.log("priorDate");
      // console.log(priorDate);
      // console.log("today");
      // console.log(todayt);
      // console.log(seime)
      // console.log(currentTimeInMilliseconds);
      // console.log("CurrentTime");
      // console.log(currentTimeInMilliseconds);

      if (isopen) {
        // console.log("Open");
        flag=true;
        //url=EVENTS_LIST_URL+'?start_time=1629294579&end_time=1638900412';
        // console.log(EVENTS_LIST_URL + "?start_time=" + priorDate + "&end_time=" + todayt);
        url =
          EVENTS_LIST_URL + "?start_time=" + priorDate + "&end_time=" + todayt;
        // axios
        // .get(EVENTS_LIST_URL,{headers:{'Authorization':"Token "+TOKEN}})
        // .then(res=>{console.log(res)})
        // .catch(err=>{
        //   console.log(err)
        // })
      } else {
        // console.log("Close");
        flag=true;
        url =
          EVENTS_LIST_URL +
          "?is_open=false&start_time=" +
          priorDate +
          "&end_time=" +
          todayt;
        // console.log(url);
      }
    } else {
      if (isopen) {
        // console.log("Open");
        url = EVENTS_LIST_URL;
        // axios
        // .get(EVENTS_LIST_URL,{headers:{'Authorization':"Token "+TOKEN}})
        // .then(res=>{console.log(res)})
        // .catch(err=>{
        //   console.log(err)
        // })
      } else {
        flag=true;
        // console.log("Close");
        url = EVENTS_LIST_URL + "?is_open=false";
        // console.log(url);
      }
    }
    if (ispageNo > 1) {
      if (isrow != 0) {
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
    } else if(isrow!=0){
      if(flag)
      {
        url = url + "&rows=" + isrow;
      }
      else{
        url = url + "?rows=" + isrow;
      }
      
    }
    if(SearchHostName!=null)
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
    if(SearchStartTime!=null)
    {
      if((typeof SearchStartTime)=='object')
      {
        // console.log("Start Time",SearchStartTime.Time);
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
    if(SearchEndTime!=null)
    {
      if((typeof SearchEndTime)=='object')
      {
        // console.log("End Time",SearchEndTime);
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
      if(url.endsWith('/'))
      {
        url = EVENTS_LIST_URL+"?myalerts=true";  
      }
      else
      {
        url = url+"&myalerts=true";
      }
    }
    run({ URL: url });
  }, [isopen, ispast, istoggle, isrow, ispageNo,SearchHostName,SearchEndTime,SearchStartTime,myAlert]);

  // const { loading, error, data } = useFetchURL<{
  //   events: Event[];
  //   no_of_records: number;
  // }>({
  //   URL: EVENTS_LIST_URL,
  // });

  const { loading, error, data, run } = useLazyFetch<{
    events: Event[];
    no_of_records: number;
  }>();

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
      return <p>{error.message}</p>;
    } else if (data) {
      let finalData = data.events;
      // console.log(finalData);
      if (!finalData) return (<div className="d-flex justify-content-center align-items-center" style={{height:"100%"}}><img src={no_data} className="img-fluid" alt="Responsive image"/></div>);
      if (finalData.length <= 0) return (<div className="d-flex justify-content-center align-items-center" style={{height:"100%"}}><img src={no_data} className="img-fluid" alt="Responsive image"/></div>);

      let headingsToShow: Array<keyof Omit<Event, "metadata">> = Object.keys(
        finalData[0]
      ).filter((heading) => heading !== "metadata") as Array<
        keyof Omit<Event, "metadata">
      >;
      //console.log(headingsToShow);

      return (
        <table className="table table-hover">
          <thead
            className="bg-white"
            style={{ position: "sticky", top: 0, zIndex: 950 }}
          >
            <tr className="fw-light text-muted">
              {/* {headingsToShow.map((heading) => (
                <th key={heading} className="fw-light" scope="col">
                  {heading}
                </th>
              ))} */}
              <th scope="row">
                <input
                  className="form-check-input flex-shrink-0"
                  type="checkbox"
                  value=""
                />
              </th>
              <th className="fw-light" scope="col">
                Alert Name
              </th>
              <th className="fw-light" scope="col">
                {/* <i className="fi fi-rr-chat-arrow-grow me-2 align-middle"></i> */}
                Severity
                <button
                  type="button"
                  className="btn btn-link fw-semi btn-sm text-dark"
                >
                  {/* <i className="fi fi-rr-arrow-down chat-arrow-grow align-middle"></i> */}
                </button>
              </th>
              <th className="fw-light" scope="col">
                <Button
                  onClick={() => setFilter_enroll_time_btn(!filter_enroll_time_btn)}
                  aria-controls="example1-collapse-text"
                  aria-expanded={filter_enroll_time_btn}
                  style={{border:'none',backgroundColor:'transparent',color:'grey',padding:0,outline:'none'}}
                  className="btn_y"
                >
                {
                  filter_enroll_time_btn?(""):(<>Captured on<i className="fi fi-rr-arrow-down chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>)
                }
                </Button>
                <Collapse in={filter_enroll_time_btn}>
                  <div id="example1-collapse-text" style={{marginTop:"0.5rem"}}>
                  <div className="d-flex align-items-center">
                  {
                    SearchStartTime!=null?
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
                {HostIdentifier!==null?
              (<TextField id="standard-basic" label={<>Host Identifier<i className="fi fi-rr-arrow-down chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>} variant="standard" value={String(HostIdentifier)} InputProps={{disableUnderline: true}}
              onChange={
                (e)=>{
                    setHostIdentifier((e.target as HTMLInputElement).value)
                }
              }
              onKeyUp={
                  (e)=>{
                    if (e.key === 'Enter') {
                      setSearchHostName(HostIdentifier)
                  }      
                  }
                }/>):(<TextField id="standard-basic" label={<>Host Identifier<i className="fi fi-rr-arrow-down chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>} variant="standard" InputProps={{disableUnderline: true}}
                onChange={
                  (e)=>{
                      setHostIdentifier((e.target as HTMLInputElement).value)
                  }
                }
                onKeyUp={
                    (e)=>{
                      if (e.key === 'Enter') {
                        setSearchHostName(HostIdentifier)
                    }      
                    }
                  }/>)
              }
              </th>
              <th className="fw-light" scope="col">
              {HostName!=null?
              (<TextField id="standard-basic" label={<>Host Name<i className="fi fi-rr-arrow-down chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>} variant="standard" value={String(HostName)} InputProps={{disableUnderline: true}}
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
                }/>):(<TextField id="standard-basic" label={<>Host Name<i className="fi fi-rr-arrow-down chat-arrow-grow align-middle" style={{marginLeft:"1rem",color:"black"}}></i></>} variant="standard" InputProps={{disableUnderline: true}}
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
                Actions
              </th>
              <th className="fw-light" scope="col">
                View
              </th>
            </tr>
          </thead>
            {finalData.map((data,ky) => {
              return (
                <tbody key={ky} className="border-top-0">
                <tr>
                  <td key={data.id}>
                    <input id={data.id} type="checkbox" onChange={(e) => putAlertsInBucket(e,data)}/>
                  </td>
                  <td className="normalisedLink"><a href={data.link}>{data.event_code}</a></td>
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
                        Mid
                      </span>
                    ):(
                      <span className="fs-7 badge bg-cyan  text-white">
                        Mid
                      </span>
                    )}
                  </td>
                  <td>{timeConverter(parseInt(data.unixtime))}</td>
                  <td>{data.host_identifier}</td>
                  <td>{data.hostname}</td>
                  {/* {data.is_open == "t" ? <td>open</td> : <td>closed</td>} */}
                  <td>
                    <button
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
                      
                    </button>
                  </td>
                  <td>
                    <Link to="/alertSheetView" state={data}>
                      {/* <Link to={{pathname:"/alertSheetView", data}}> */}
                      <button
                        type="button"
                        className="btn btn-primary-light ntn-sm"
                      >
                         <i className="fa fa-eye"></i>
                      </button>
                    </Link>
                  </td>
                </tr>
              </tbody>
              );
            })}
            {/* {finalData.map((data, _) => {
              
              
              return (
                
                <tr key={_}>
                  {headingsToShow.map((heading) => {
                    {console.log(data[heading])}
                    return <td key={heading}>{data[heading]}</td>;
                  })}
                  <td>
                    <ViewModal event={data} />
                  </td>
                </tr>
                
              );
            })} */}
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
    // console.log(e.target.checked, alert)
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

  const handleCloseAlerts = () => {
    // console.log(selectedAlerts)
    // console.log('handle close alerts')
    // check whethere any alert is selected or not
    if(selectedAlerts.length > 0){
      // check if is there any open category alert in list
      const isAnyOpenAlerts = selectedAlerts.some((ele:any) => ele.is_open == 'f');
      // console.log(isAnyOpenAlerts)
      if(!isAnyOpenAlerts){
        const modalEle = closeAlertModalRef.current
        const bsModal = new bootstrap.Modal(modalEle, {
            backdrop: 'static',
            keyboard: false
        })
        bsModal.toggle()
      }else{
        swal('', 'Please remove open category alerts', 'warning')
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
    // console.log(eventsId)
    let req = {
      "event_id": eventsId,
      "remarks": comment,
      "is_open": false
    }
    // console.log('event update-close request: ', req)
    updateEvent(req)
    .then((res) => {
      setShowLoading(false)
      setSelectedAlerts([])
      // console.log('close alert response : ', res)
      if(res.info?.status == "Success"){
        swal('', res.info.message,'success').then(updated => updated ? window.location.reload() : '')
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

  return (
    showLoading ? 
      <Loader />
      :
      <>
      {
        alertS?alertData!==undefined?alertData.status==="Success"?(<Alert onClose={() => {setAlertS(false)}}>{alertData.message}</Alert>):(<Alert  onClose={() => {setAlertS(false)}} severity="error">{alertData.message}</Alert>):(<></>):(<></>)
      }
      <div className="card card-body">
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
      </div>
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
                <i className="fi fi-rr-apps me-2 align-middle"></i>Manage Alerts
              </button>
              {/* <button className="btn btn-primary-light align-middle text-primary">
              <i className="fi fi-rr-plus me-2 align-middle"></i>Add New Alert
            </button> */}
            </div>
          </div>
        </div>
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
                        onClick={() => {
                          if (!isopen) {
                            setIsopen(true);
                          }
                          if (ispast != 0) {
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
                        disabled
                        onClick={() => {
                          // is_open='false';
                          if (isopen) {
                            setIsopen(false);
                          }
                          if (ispast != 0) {
                            setispast(0);
                          }
                          setMyAlert(false)
                          // setIsopen(false);
                          // setispast(0);
                          //getResults(false);
                        }}
                      >
                        Closed Alerts
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
                        disabled
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
                  <label className="text-muted">No. of rows:&nbsp;</label>
                  <select className="form-select-sm p-1" onChange={handleFilterChange} value={isrow} style={{border: '1px solid silver'}}>
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
                    setIstoggle(null);
                    setisrow(100);
                    setispageNo(1);
                    setSelectedAlerts([])
                    setComment('');
                    setShowLoading(false);
                    setContextData(undefined);
                    setHostName(null);
                    setHostIdentifier(null);
                    setSearchHostName(null);
                    setFilter_enroll_time_btn(false);
                    setSearchStartTime(null);
                    setSearchEndTime(null);
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
                                userList!==undefined?userList.map((user:any,_:number)=>{
                                  return <Dropdown.Item key={_} onClick={(e:any)=>{
                                    setSelectedUser(e.target.text as string)
                                  }}>{user.username}</Dropdown.Item>
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
                          let AlertList:any=[]
                          let userID:string=""
                          selectedAlerts.map((ob:any)=>{
                            AlertList.push(parseInt(ob.id))
                          })
                          userList.map((user:any)=>{
                            if(user.username===selectedUser)
                            {
                              userID=user.id
                            }
                          })
                          if(selectedUser!=="Assigned to")
                          {
                            setAlertS(true)
                            updateMe({"user_id":userID,"event_id":AlertList},ASSIGN_ALERT_USER_URL,setAlertData)
                          }
                        }} className="btn btn-primary">
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
                    <i className="fi fi-rr-time-delete me-2 align-middle"></i>
                    Close Alerts
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
                <ExportAlert />
              </div>
            </div>
            <div
              className="w-100 overflow-auto"
              style={{ height: "calc(100vh - 27rem)" }}
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
