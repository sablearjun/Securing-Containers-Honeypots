import React,{ useMemo, useRef } from "react"
import { UpdateRule } from "./update"
import axios from "axios";
import no_data from "../../../assets/images/data_not_found.png";
import TextField from '@mui/material/TextField';
import Stack from '@mui/material/Stack';
import { VajraModal } from "../../../components/modal";
import ProgressBar from 'react-bootstrap/ProgressBar'
import Box from '@mui/material/Box';
import Button from '@mui/material/Button';
import Collapse from 'react-bootstrap/Collapse';
import { useState, useEffect } from "react";
import { DELETE_PYTHON_RULE_URL, LIST_PYTHON_RULE_URL } from "../../../apis/urls";
import { updateMe,deleteMe,getData } from "../../../apis/requests";
import { deleteConf, Event,Node , toggle_state} from "../../../types";
import { useLocation } from "react-router-dom";
import { Navigate } from "react-router";
import * as bootstrap from "bootstrap";
import "../../../css/custom.css"
import swal from 'sweetalert'
import  loaderForTable from '../../../assets/images/loader-table.svg'
import { json } from "stream/consumers";
import ReactTooltip from "react-tooltip";
import Typography from '@mui/material/Typography'
import Chip from '@mui/material/Chip';
import Alert from '@mui/material/Alert';
import AddCircleOutlineIcon from '@mui/icons-material/AddCircleOutline';
import Modal from 'react-bootstrap/Modal';
import Button1 from 'react-bootstrap/Button';
import pythonIcon from '../../../assets/images/icons8-python.svg'
import Fade from "@mui/material/Fade";
import { Rule } from "@mui/icons-material";

export const PythonRuleEngine = () => {
  const [ruleList,setRuleList] = useState<any>(undefined)
  const [selectedRule,setSelectedRule] = useState<any>(undefined)
  const [mode,setMode] = useState<'add'| 'edit'>('add')
  const [alertData, setAlertData] = useState<any>(undefined)
  const [alert, setAlert] = useState<boolean>(false)
  const location = useLocation()

  useEffect(() => {
    if(location.state !==  null){
      setMode('edit')
      setSelectedRule(location.state)
    }else{
      setMode('add')
      setSelectedRule({})
    }
  }, [])

  useEffect(() => {
    getData(LIST_PYTHON_RULE_URL,setRuleList)
  },[mode,selectedRule,alert]);

  return <>
    {
      alert?alertData!==undefined?alertData.status==="Success"?
      (<Fade
       in={alert}
       timeout={{ enter: 1000, exit: 1000 }}
       addEndListener={() => {
         setTimeout(() => {
           setAlert(false)
         }, 5000);
       }}
       >
      <Alert onClose={() => {setAlert(false)}}>{alertData.message}</Alert></Fade>
      ):
      (<Fade
        in={alert}
        timeout={{ enter: 1000, exit: 1000 }}
        addEndListener={() => {
          setTimeout(() => {
            setAlert(false)
          }, 30000);
        }}
        >
      <Alert  onClose={() => {setAlert(false)}} severity="error">{alertData.message}</Alert></Fade>):
      (<></>):(<></>)
    }
    <div className="container-fluid">
    <div className="row" style={{height:"90vh"}}>
        {/* <div className="col-3 pe-0">
          <div className="card py-3" style={{borderRadius:"10px 0 0 10px",borderRight:0}}>
            <div className="d-flex flex-column align-items-start gap-3">
              <div
                style={{ height: "calc(100vh - 8rem)",width:"100%",flexDirection:"column" }}
                id="#scrollbar-1"
              >
                <div
                  style={{ height: "calc(100vh - 15rem)",width:"100%",flexDirection:"column",overflowY:"scroll" }}
                  className="acroItem"
                >
                  {
                    ruleList!==undefined&&ruleList.rule_list?ruleList.rule_list.map((rule:any,_:number)=>{
                      return <div className="container d-flex justify-content-center" key={_}>
                        <div className="col container p-1 d-flex align-items-center" style={{ height: "2.8rem",width:"100%"}}>
                          <div className="row" style={{width:"100%"}}>
                            <div className="col-1 d-flex justify-content-center align-items-center">
                                  <img src={pythonIcon} width="25rem" alt="pythonIcon" />
                            </div>
                            <div className="col-10 d-flex justify-content-center align-items-center">
                              <div className="d-flex justify-content-start align-items-center" style={{height:"100%", width:"100%", cursor:"pointer"}}
                                  onClick={()=>{
                                    try {
                                      rule.rule=JSON.parse(rule.rule) 
                                    } catch (error) {
                                      rule.rule=rule.rule
                                    }
                                    setMode('edit')
                                    setSelectedRule(rule)
                                  }}
                                >
                                <span style={{fontSize:"calc(0.5vh + 0.5vw)"}}>
                                  {rule.name}.py
                                </span>
                              </div>
                            </div>
                            <div className="col-sm-1 d-flex justify-content-center align-items-center">
                              <Button onClick={()=>{
                                deleteMe({id:rule.id},DELETE_PYTHON_RULE_URL,setAlertData)
                                setAlert(true)
                              }}>
                                <i className="fa fa-trash-o" style={{fontSize:"1.2vw",color:"red"}} aria-hidden="true"></i>
                              </Button>
                            </div>
                          </div>
                        </div>
                      </div>
                    }):<></>
                  }
                </div>
                <div className="container d-flex justify-content-center">
                  <div className="col container p-0 mt-2 d-flex justify-content-center align-items-center" style={{ height: "3.5rem",width:"100%"}}>
                    <Button variant="outlined" color="secondary" style={{width:"100%",height:"100%",fontSize:"1.2rem"}} onClick={()=>{
                      setMode('add')
                      setSelectedRule({})
                    }}>
                      Add<i className="fa fa-plus" style={{marginLeft:"1rem"}} aria-hidden="true"></i>
                    </Button>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div> */}
        <div className={"col-12 ps-0"}>
          <div className="card" style={{border: '0px'}}>
            <div className="d-flex flex-column align-items-start gap-3">
              <div
                className="w-100 overflow-auto"
                style={{ height: "calc(100vh - 6.5rem)" }}
                id="#scrollbar-1"
              >
                <UpdateRule 
                  mode={mode} 
                  data={selectedRule} 
                  selected={setSelectedRule} 
                  setAlert={setAlert} 
                  setAlertData={setAlertData}
                />
                {/* {
                  selectedRule!==undefined?<UpdateRule mode={mode} data={selectedRule} selected={setSelectedRule} setAlert={setAlert} setAlertData={setAlertData}/>:
                <div className="container p-5 h-100 w-100">
                  <div className="container w-100 h-100 d-flex justify-content-center align-items-center">
                    <Typography variant="h6" component="h6">
                      Select Rule to Edit/View
                    </Typography>
                  </div>
                </div>
                } */}
              </div>
            </div>
          </div>
        </div>
        </div>
      </div>
  </>
}
