import React, { useEffect, useRef, useState } from 'react'
import Editor from '@monaco-editor/react';
import Button from '@mui/material/Button';
import DeleteIcon from '@mui/icons-material/Delete';
import LightModeIcon from '@mui/icons-material/LightMode';
import DarkModeIcon from '@mui/icons-material/DarkMode';
import { updateMe } from '../../../../apis/requests';
import { SUBMIT_PYTHON_RULE_URL, UPDATE_PYTHON_RULE_URL } from '../../../../apis/urls';
import Alert from '@mui/material/Alert';
import Fade from "@mui/material/Fade";
import TextField from '@mui/material/TextField';
import Autocomplete from '@mui/material/Autocomplete';
import Slider from '@mui/material/Slider';
import Typography from '@mui/material/Typography';
import { PythonRule } from '../../../../types';
import Select, { SelectChangeEvent } from '@mui/material/Select';
import MenuItem from '@mui/material/MenuItem';
import InputLabel from '@mui/material/InputLabel';
import FormControl from '@mui/material/FormControl';
import { Link } from 'react-router-dom';
import { pythonSampleRule, Tactic, severity } from '../../../../data/rules-data';

function valueLabelFormat(value: number) {
  return severity.findIndex((severity) => severity.value === value) + 1;
}

interface params{
  mode:'edit'|'add',
  data?:any,
  selected:Function,
  setAlert:Function,
  setAlertData:Function
}
function isJsonString(str:string) {
  try {
      JSON.parse(str);
  } catch (e) {
      return false;
  }
  return true;
}

export const UpdateRule = ({mode,data,selected,setAlert,setAlertData}:params) => {
    const DefaultRule:PythonRule=mode==="edit"?data:{
      name:"",
      rule:pythonSampleRule,
      severity:0,
      technique:"",
      technique_id:"",
      link:"",
      comment:"",
      os_name:"Linux"
    }
    const [pythonRule, setPythonRule] = useState<PythonRule>(DefaultRule)
    const [editorTheme, setEditorTheme] = useState<"vs-light"|"vs-dark">("vs-light")
    
    const editorRef = useRef<any>(null)
    function handleEditorDidMount(editor:any, monaco:any) {
      editorRef.current = editor;
    }
    function setValue() {
      setPythonRule(prev=>({...prev,rule:editorRef?.current.getValue()}))
    }

    useEffect(() => {
      setPythonRule(DefaultRule)
    },
    //eslint-disable-next-line
    [data]);

  return (
    <>
    <div className="card py-3">
      <div className="card-header bg-white border-0">
        <div className="hstack">
          <div>
            <h4 className="fw-mid my-0">Add Python Rule</h4>
          </div>
          <div className="ms-auto">
            <Link to="/rules/view-python-rule">
            <button
              type="button"
              className="btn btn-primary-light align-middle text-primary"
            >
              View Python Rules
            </button>
            </Link>
          </div>
        </div>
      </div>
      <div className="card-body bg-white border-0">
      {/* <h4 className="fw-mid my-0">Add Python Rules</h4> */}
        <div className='row mb-3'>
          <div className='col-4'>
            <TextField required size="small" id="outlined-basic" style={{width:"100%"}} value={pythonRule.name} onChange={(e:any)=>{
              setPythonRule(prev=>({...prev,name:e.target.value}))
            }} label="Name" variant="outlined" />
          </div>
          <div className='col-4'>
            <Autocomplete 
              
              
              size="small"
              value={pythonRule.technique}
              onChange={(e:any)=>{
                setPythonRule(prev=>({...prev,technique:e.target.outerText}))
              }}
              disablePortal
              style={{width:"100%"}}
              id="combo-box-demo"
              options={Tactic}
              sx={{ width: 300 }}
              
              renderInput={(params) => <TextField required {...params} label="Tactic"/>}
            />
          </div>
          <div className='col-4'>
            {/* <Typography id="non-linear-slider" gutterBottom>
              Severity
            </Typography>
            <Slider
              aria-label="Restricted values"
              valueLabelFormat={valueLabelFormat}
              step={null}
              value={(pythonRule.severity-1)*25}
              onChange={(e:any)=>{
                setPythonRule(prev=>({...prev,severity:(e.target.value/25+1)}))
              }}
              valueLabelDisplay="auto"
              marks={severity}
            /> */}
            <FormControl required sx={{minWidth: 120 }} size="small">
              <InputLabel id="demo-select-small">Severity</InputLabel>
              <Select
              required
                labelId="demo-select-small"
                id="demo-select-small"
                value={(pythonRule.severity-1)*25}
                label="Severity"
                onChange={(e:any)=>{
                  setPythonRule(prev=>({...prev,severity:(e.target.value/25+1)}))
                }}
              >
                {
                  severity?.map((val) => <MenuItem value={val.value}>{val.label}</MenuItem>)
                }
              </Select>
            </FormControl>
          </div>
        </div>
        <div className='row mb-3'>
          <div className='col-4'>
            <TextField size="small" id="outlined-basic" style={{width:"100%"}} value={pythonRule.link} onChange={(e:any)=>{
              setPythonRule(prev=>({...prev,link:e.target.value}))
            }} label="Link" variant="outlined" />
          </div>
          <div className='col-4'>
            <TextField size="small" id="outlined-basic" style={{width:"100%"}} value={pythonRule.comment} onChange={(e:any)=>{
              setPythonRule(prev=>({...prev,comment:e.target.value}))
            }} label="Comment" variant="outlined" />
          </div>
          <div className='col-4'>
            <FormControl sx={{minWidth: 120 }} size="small">
              <InputLabel id="demo-select-small">Os Type</InputLabel>
              <Select
                labelId="demo-select-small"
                id="demo-select-small"
                value={pythonRule.os_name}
                label="Os Type"
                onChange={(e:SelectChangeEvent)=>{
                  setPythonRule(prev=>({...prev,os_name:e.target.value}))
                }}
              >
                <MenuItem value={"Windows"}>Windows</MenuItem>
                <MenuItem value={"Linux"}>Linux</MenuItem>
              </Select>
            </FormControl>
          </div>
        </div>
        <div className="form-floating w-100" style={{border:"1px solid  rgba(0, 0, 0, 0.23)", padding:"0.5rem", borderRadius: "5px"}}>
            {
              pythonRule?.rule!==undefined&&isJsonString(pythonRule?.rule)?
              <Editor 
                height="40vh" 
                width="100%"
                language="python" 
                theme={editorTheme} 
                onChange={setValue}
                value={JSON.parse(pythonRule?.rule)}
                onMount={handleEditorDidMount}
              />:<Editor 
              height="40vh" 
              width="100%"
              language="python"
              onChange={setValue}
              theme={editorTheme}
              value={pythonRule?.rule}
              onMount={handleEditorDidMount}
            />
            }
            {/* <Editor height="55vh" language="python" theme={editorTheme} value={pythonRule.rule} onChange={setValue} onMount={handleEditorDidMount}/> */}
        </div>
        <div style={{marginTop:"1rem", marginBottom:"0.5rem", display:"flex", flexDirection:"row", gap:"1rem"}}>
            <Button variant="outlined" color="secondary" startIcon={editorTheme==="vs-light"?<LightModeIcon/>:<DarkModeIcon/>} onClick={()=>{
                editorTheme==="vs-light"?setEditorTheme("vs-dark"):setEditorTheme("vs-light")
            }}>
                Theme
            </Button>
            <Button variant="outlined" color="secondary" startIcon={<DeleteIcon />} onClick={()=>{
                setPythonRule(prev=>({...prev,rule:""}))
            }}>
                Clear
            </Button>
            <button type="button" className="btn btn-primary-light align-middle text-primary" onClick={()=>{
              if(mode==="add")
              {
                if(pythonRule.name==="")
                {
                  setAlertData({"message":"Please Enter the name of the rule","status":"Unsuccessful"})
                  setAlert(true)
                
                }else if(pythonRule.technique ==="")
                {
                  setAlertData({"message":"Please select the technique of the rule","status":"Unsuccessful"})
                  setAlert(true)
                } else if (pythonRule.severity=== 0)
                {
                  setAlertData({"message":"Please select the severity of the rule","status":"Unsuccessful"})
                  setAlert(true)
                }else if (pythonRule.rule == "") {
                  setAlertData({ "message": "Please write the rule", "status": "Unsuccessful" })
                  setAlert(true)

                }
                else{
                  pythonRule["mode"]="add"
                  pythonRule["rule_written_by"]=localStorage.getItem("Username")
                  updateMe(pythonRule,SUBMIT_PYTHON_RULE_URL,setAlertData)
                  selected(undefined)
                }
              setAlert(true)
              }
              else
              {
                if(pythonRule.name==="")
                {
                  setAlertData({"message":"Please Enter the name of the rule","status":"Unsuccessful"})
                  setAlert(true)
                
                }else if(pythonRule.technique ==="")
                {
                  setAlertData({"message":"Please select the technique of the rule","status":"Unsuccessful"})
                  setAlert(true)
                } else if (pythonRule.severity=== 0)
                {
                  setAlertData({"message":"Please select the severity of the rule","status":"Unsuccessful"})
                  setAlert(true)
                }else if (pythonRule.rule == "") {
                  setAlertData({ "message": "Please write the rule", "status": "Unsuccessful" })
                  setAlert(true)

                }

                else{
                  pythonRule["mode"]="edit"
                  pythonRule["rule_written_by"]=localStorage.getItem("Username")
                  updateMe(pythonRule,UPDATE_PYTHON_RULE_URL,setAlertData)
                }
                
                setAlert(true)
              }
            }}>{mode==="edit"?"Update Rule":"Submit Rule"}</button>
        </div>
      </div>
    </div>
    </>
  )
}