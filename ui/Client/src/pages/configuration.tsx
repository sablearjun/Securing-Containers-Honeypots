import TextField from '@mui/material/TextField';
import { VajraModal } from "../components/modal";
import Button from '@mui/material/Button';
import { useState, useEffect, ChangeEvent } from "react";
import { NODE_COUNT_URL,CONFIG_LIST_URL, CONFIG_TABLE_URL,CONFIG_UPDATE_URL,CONFIG_ADD_URL,CONFIG_DELETE_URL, GET_USER_URL } from "../apis/urls";
import { getData } from "../apis/requests";
import { deleteConf} from "../types";
import "../css/custom.css"
import Typography from '@mui/material/Typography'
import Chip from '@mui/material/Chip';
import { updateMe,deleteMe } from "../apis/requests";
import Alert from '@mui/material/Alert';
import AddCircleOutlineIcon from '@mui/icons-material/AddCircleOutline';
import Modal from 'react-bootstrap/Modal';
import Button1 from 'react-bootstrap/Button';
import InfoIcon from '@mui/icons-material/Info';
import Tooltip from '@mui/material/Tooltip';
import Checkbox from '@mui/material/Checkbox';
import FormControlLabel from '@mui/material/FormControlLabel';
import FormGroup from '@mui/material/FormGroup';
import { toast, ToastContainer } from 'react-toastify';
import Radio from '@mui/material/Radio';
import RadioGroup from '@mui/material/RadioGroup';
import FormControl from '@mui/material/FormControl';
import Switch from '@mui/material/Switch';

const Configuration = () => {
  const [nodeStatus,setNodeStatus] = useState<any>(undefined)
  const [configList,setConfigList] = useState<any>(undefined)
  const [tableList,setTableList] = useState<any>(undefined)
  const [updateButton,setUpdateButton] = useState<boolean>(false)
  const [selectedConfig,setSelectedConfig] = useState<any>({
    query_name:"",
    os_name: "",
    kernel_version: "",
    os_arch: "",
    query:{
      schedule:{
      }
    }
  })
  const [queryName, setQueryName] = useState<any>("");
  const [user, setUser] = useState<any>(undefined)
  const [configModal,setConfigModal] = useState<boolean>(false)
  const [selectedTable,setSelectedTable] = useState<string>()
  const [submit,setSubmit] = useState<"update"|"add">("update")
  const [alert, setAlert] = useState<boolean>(false)
  const [alertData, setAlertData] = useState<any>(undefined)
  const [add, setAdd] = useState<boolean>(false)
  const [showMakeConfigBtn, setShowMakeConfigBtn] = useState<boolean>(true);
  const [show, setShow] = useState(false);
  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);
  const [deleteConf, setDeleteConf] = useState<deleteConf>({
    config_id:0,
    config_name:"notDefined",
  })

  const handlePlatformChange = (e:ChangeEvent<HTMLInputElement>)=>{
    const { name, checked } = e.target;
    let updatedOsName = selectedConfig.os_name;

    if (checked) {
      // Add the OS name if checked
      if (!updatedOsName.includes(name)) {
        updatedOsName += updatedOsName ? `,${name}` : name;
      }
    } else {
      // Remove the OS name if unchecked
      updatedOsName = updatedOsName
        .split(",")
        .map((os: string) => os.trim())
        .filter((os:any) => os !== name)
        .join(",");

      if(updatedOsName == 'Linux')
        setSelectedConfig({
          ...selectedConfig,
          kernel_version: '',
        });
      else
      setSelectedConfig({
        ...selectedConfig,
        os_arch: '',
      });
    }

    setSelectedConfig({
      ...selectedConfig,
      os_name: updatedOsName.replace(/\"/g, ''),
    });
  }

  const handleOsArchChange = (e:ChangeEvent<HTMLInputElement>)=>{
    const { name, checked } = e.target;
    let updatedArchName = selectedConfig.os_arch || '';
    if (checked) {
      // Add the OS name if checked
      // console.log("Arch Name: ", updatedArchName, "current: ",name);
      if (!updatedArchName.includes(name)) {
        updatedArchName += updatedArchName ? `/${name}` : name;
        // console.log("updated: ",updatedArchName);
      }
    } else {
      // Remove the OS name if unchecked
      updatedArchName = updatedArchName
        .split("/")
        .map((item: string) => item.trim())
        .filter((item:any) => item !== name)
        .join("/");
    }

    setSelectedConfig({
      ...selectedConfig,
      os_arch: updatedArchName,
    });
  }

  const handleBpfChange = (e:ChangeEvent<HTMLInputElement>)=>{
    const checked = e.target.checked;
    
    let kernel_version : string = '';
    if(checked){
      kernel_version = '>4.17';
    }else{
      kernel_version = '<4.18';
    }

    console.log(kernel_version);

    setSelectedConfig({
      ...selectedConfig,
      kernel_version: kernel_version,
    });
  }

  const DeleteModal=()=>{
    return (<Modal show={show} onHide={handleClose}>
    <Modal.Header closeButton>
      <Modal.Title style={{fontWeight:400,fontSize:"1.5rem"}}>Delete Config <span style={{color:"#823A74"}}>{deleteConf.config_name}</span></Modal.Title>
    </Modal.Header>
    <Modal.Body> 
      <div className="container">
        <div className="row h5">Are you sure you want to delete this Config</div> 
        <div className="row">Once delete you cannot undo</div>
      </div>
    </Modal.Body>
    <Modal.Footer>
      <Button1 variant="secondary" onClick={handleClose}>
        Close
      </Button1>
      <Button1 variant="primary" onClick={()=>{
          deleteMe({id:deleteConf.config_id},CONFIG_DELETE_URL,setAlertData)
          setAlert(true)
          setConfigList(undefined)
          setSelectedConfig(undefined)
          getData(CONFIG_LIST_URL,setConfigList)
          handleClose()
          setAdd(false)
      }}>
        Delete
      </Button1>
    </Modal.Footer>
  </Modal>);
  }

  useEffect(() => {
    if(configList===undefined)
    {
      getData(CONFIG_LIST_URL,setConfigList);
    }
    if(nodeStatus===undefined)
    {
      getData(NODE_COUNT_URL,setNodeStatus);
    }
    if(tableList===undefined)
    {
      getData(CONFIG_TABLE_URL,setTableList);
    }
    if(user===undefined)
    {
      getData(GET_USER_URL,setUser);
    }
  }, [configList])
  return (
    <>
    <ToastContainer/>
    {
      alert?alertData!==undefined?alertData.status==="Success"?(<Alert onClose={() => {setAlert(false)}}>{alertData.message}</Alert>):(<Alert  onClose={() => {setAlert(false)}} severity="error">{alertData.message}</Alert>):(""):("")
    }
    <DeleteModal/>
    <VajraModal setter={setConfigModal} state={configModal} table={selectedTable} setting={setSelectedConfig} config={selectedConfig} button={setUpdateButton}/>
      <div className="row configView">

         {/* List of Configurations */}
        <div className="col-12 col-md-3">
          <div className="card py-3 mt-4 mt-md-0">
            <div className="d-flex flex-column align-items-start gap-3">
              <div className="row container w-100 h-100 d-flex justify-content-center align-items-center">
                <Typography variant="h6" component="h6">
                    List of Configurations
                </Typography>
              </div>
              <div
                className="ch-18 w-100 flex-column vh-md-100px overflow-auto"
                id="#scrollbar-1"
              >
                <div
                  className="acroItem configTableList h-md-75"
                >
                {
                  configList!==undefined?configList.data?.map((config:any,_:number)=>{
                    return <div className="container d-flex justify-content-center" key={_}>
                      <div className="col container m-1 w-100 h-3-5 d-flex align-items-center" style={{ border:"solid 1px" }}>
                        <div className="d-flex flex-column flex-md-row w-100 ms-0">
                          <div className="col-8 w-75 d-flex justify-content-center align-items-center">
                            <div className="d-flex flex-wrap justify-content-center align-items-center" style={{height:"100%", width:"100%", cursor:"pointer"}}
                                onClick={()=>{
                                  // if(showMakeConfigBtn){
                                  //   toast.info("Click on Make Config first and then click on config name")
                                  // }
                                  if(config.query_name==='win_config'){
                                    toast.info(`Not permitted to update config for ${config.query_name}`)
                                  }
                                  else{
                                    if(typeof(config.query)=="string")
                                    {
                                      config.query=JSON.parse(config.query)
                                    }

                                    // Make everything to default first
                                    setSelectedConfig({
                                      query_name:"",
                                      os_name: "",
                                      kernel_version: "",
                                      os_arch: "",
                                      query:{
                                        schedule:{
                                        }
                                    }})

                                    config.os_name = config.os_name.replace(/\"/g, '')
                                    setShowMakeConfigBtn(false)
                                    setUpdateButton(false)
                                    setSubmit("update")
                                    setSelectedConfig(config)
                                    setAdd(false);
                                }
                              }}
                              >
                              <h6 className='fw-normal'>
                                {config.query_name}
                              </h6>
                            </div>
                          </div>
                          <div className='col-2'>
                          {(user!==undefined && user?.data?.[0].role == "ADMINISTRATOR") ? <>
                              <Button className='fs-3'
                                onClick={()=>{
                                  setDeleteConf((prev:any)=>({...prev,config_id:config.id,config_name:config.query_name}))
                                  handleShow()
                                }}
                                // onClick={()=>{
                                //   deleteMe({id:config.id},CONFIG_DELETE_URL,setAlertData)
                                //   setAlert(true)
                                //   setConfigList(undefined)
                                //   setSelectedConfig(undefined)
                                //   getData(CONFIG_LIST_URL,setConfigList)
                                //   // window.location.reload();
                                // }}>
                                >
                                  <i className="fa fa-trash-o fs-5 text-danger"  aria-hidden="true"></i>
                                </Button>
                              </>:<></>}
                          </div>
                        </div>
                      </div>
                    </div>
                  }):("")
                }
                </div>
              </div>
            </div>
          </div>
        </div>

         {/* Edit/ Make Config Part */}
        <div className="col-12 col-md-6 vh-md-75">
          <div className="card py-3 mt-4 mt-md-0">
            <div className="d-flex flex-column align-items-start gap-3">
              <div
                className="w-100 overflow-auto"
                style={{ height: "calc(100vh - 15rem)" }}
                id="#scrollbar-1"
              >
                {
                  configList!==undefined&&showMakeConfigBtn==false&&selectedConfig!==undefined?
                  <div className="container p-5 h-100 w-100">
                    <div className="container w-100 h-100">
                      <div className="row">
                        <div className="col-4 d-flex align-items-center">
                          <Typography variant="h6" component="h6">
                            Config Name
                          </Typography>
                        </div>
                        <div className="col-8">
                          { submit ==="update" ? <>
                          <TextField required id="outlined-basic" style={{width:"60%"}} label={selectedConfig.query_name} value={selectedConfig.query_name} inputProps={{ maxLength: 20 }} disabled={true}
                           variant="outlined" />
                          </>:<>
                          <TextField required id="outlined-basic-1" style={{width:"60%"}} label={selectedConfig.query_name===""?"Config Name":selectedConfig.query_name} value={selectedConfig.query_name} inputProps={{ maxLength: 20 }}
                          onChange={(e)=>{
                            setUpdateButton(true)
                            setSelectedConfig((prev:any)=>({...prev, query_name:e.target.value}))
                          }} variant="outlined" />
                          </>}
                        </div>
                      </div>
                      {/*console.log("Selected Config: ", selectedConfig)*/}
                      <div className='row' style={{marginTop:"2rem"}}>
                        <div className='col-4 d-flex align-items start'>
                          <Typography variant="h6" component="h6">
                            Platform
                          </Typography>
                        </div>
                        <div className='col-8'>
                          <div className='col'>
                            <FormGroup row>
                              <FormControlLabel
                                  control={
                                    <Checkbox checked={selectedConfig.os_name.includes('Windows')} onChange={(e) => {
                                      handlePlatformChange(e)
                                    }} name="Windows" />
                                  }
                                  label="Windows"
                              />
                              <FormControlLabel
                                      control={
                                        <Checkbox checked={selectedConfig.os_name.includes('Linux')} onChange={(e) =>{
                                          handlePlatformChange(e)
                                        }} name="Linux" />
                                      }
                                      label="Linux"
                              />
                            </FormGroup>
                          </div>
                        </div>
                      </div>
                      <div className={ selectedConfig.os_name.includes('Windows')? "row": "d-none" } style={{marginTop:"2rem"}}>
                        <div className='col-4 d-flex align-items start'>
                          <Typography variant="h6" component="h6">
                            OS Arch
                          </Typography>
                        </div>
                        <div className='col-8'>
                          <div className='col'>
                            <FormGroup row>
                              <FormControlLabel
                                  // checked={selectedConfig.os_arch.includes('32')}
                                  control={
                                    <Checkbox checked={selectedConfig.os_arch ? selectedConfig.os_arch.includes('32') : false} onChange={(e) => {
                                      handleOsArchChange(e)
                                    }} name="32" />
                                  }
                                  label="32-bit"
                              />
                              <FormControlLabel
                                // checked={selectedConfig.os_arch.includes('64')}
                                control={
                                        <Checkbox checked={selectedConfig.os_arch ? selectedConfig.os_arch.includes('64') : false}
                                        onChange={(e) =>{
                                          handleOsArchChange(e)
                                        }} name="64" />
                                      }
                                  label="64-bit"
                              />
                            </FormGroup>
                          </div>
                        </div>
                      </div>
                      <div className={ selectedConfig.os_name.includes('Linux')? "row": "d-none" } style={{marginTop:"2rem"}}>
                        <div className='col-4 d-flex align-items start'>
                          <Typography variant="h6" component="h6">
                            Bpf Support
                          </Typography>
                        </div>
                        <div className='col-8'>
                          <div className='col'>
                            {/* <FormControl> */}
                            {/* <RadioGroup
                              row
                              aria-label="options"
                              name="options"
                              value={selectedConfig.kernel_version}
                              onChange={(e)=>{
                                setSelectedConfig({
                                  ...selectedConfig,
                                  kernel_version: e.target.value,
                                });
                              }}
                            >
                              <FormControlLabel
                                value=">4.17"
                                control={<Radio />}
                                label=">4.17"
                              />
                              <FormControlLabel
                                value="<4.18"
                                control={<Radio />}
                                label="<4.18"
                              />
                              <FormControlLabel
                                value=""
                                control={<Radio />}
                                label="No specification"
                              />
                              </RadioGroup> */}
                            <Switch
                                checked={selectedConfig.kernel_version === ''? false: selectedConfig.kernel_version === '>4.17'}
                                onChange={(e)=>{
                                  handleBpfChange(e);
                                }}
                                inputProps={{ 'aria-label': 'controlled' }}
                            />
                            {/* </FormControl> */}

                          </div>
                        </div>
                      </div>
                      <div className="row" style={{marginTop:"2rem"}}>
                        <div className="col-4 d-flex align-items-center">
                          <Typography variant="h6" component="h6">
                            Tables Configured
                          </Typography>
                        </div>
                        <div className="col-8">
                          <div>
                            <div>
                              {
                                Object.keys(selectedConfig.query.schedule).map((ky:string,_:number)=>{
                                    return <Chip
                                    key={_}
                                    className="me-2 mt-2"
                                    label={ky}
                                    variant="outlined"
                                    onClick={()=>{}}
                                    onDelete={(e)=>{
                                      setUpdateButton(true)
                                      let prev:any={...selectedConfig}
                                      delete prev.query.schedule[ky]
                                      setSelectedConfig(prev)
                                    }}
                                  />
                                })
                              }
                              <Tooltip title={<Typography fontSize={13}>Click on any option in list of tables to add more tables</Typography>} arrow>
                              <Chip
                              className="me-2 mt-2"
                              // deleteIcon={}
                              label={<InfoIcon />}
                              // variant="outlined"
                              onClick={()=>setAdd(true)}
                              // onDelete={(e)=>{
                              //   setUpdateButton(true)
                              //   let prev:any={...selectedConfig}
                              //   delete prev.query.schedule[ky]
                              //   setSelectedConfig(prev)
                              // }}
                              />
                              </Tooltip>
                            </div>
                          </div>
                        </div>
                      </div>
                      <div className="row d-flex justify-content-center align-items-center" style={{marginTop:"2rem"}}>
                        <div className="d-flex justify-content-center align-items-center">
                        <Button variant="outlined" color="info" style={{width:"10vw",height:"5vh"}} onClick={()=>{
                            setSelectedConfig({
                            query_name:"",
                            os_name: "",
                            kernel_version: "",
                            os_arch: "",
                            query:{
                              schedule:{
                              }
                            }})
                            setSubmit("add");
                          }}> Clear
                          </Button>
                          <Button className="mx-1" variant="outlined" color="secondary" style={{width:"10vw",height:"5vh"}} onClick={()=>{
                            if(submit==="add")
                            {
                              // let config_n = selectedConfig.query_name;
                              // let kernel_version_n = selectedConfig.kernel_version || '';
                              // if(selectedConfig.os_name.includes('Windows')){
                              //   if(selectedConfig.os_arch.includes('64')){
                              //     setSelectedConfig({
                              //       ...selectedConfig,
                              //       query_name: config_n + '_win64',
                              //     });
                              //   }else if(selectedConfig.os_arch.includes('32')){
                              //     setSelectedConfig({
                              //       ...selectedConfig,
                              //       query_name: config_n + '_win32',
                              //     });
                              //   }
                              // }else if(selectedConfig.os_name.includes('Linux')){
                              //   if(selectedConfig.kernel_version === '>4.17'){
                              //     setSelectedConfig({
                              //       ...selectedConfig,
                              //       query_name: config_n + '_A4.17',
                              //     });
                              //   }else if(selectedConfig.kernel_version === '<4.18'){
                              //     setSelectedConfig({
                              //       ...selectedConfig,
                              //       query_name: config_n + '_B4.18',
                              //     });
                              //   }
                              // }
                              // console.log("THis is add data: ",selectedConfig)
                              if(selectedConfig.query_name =='' ||selectedConfig.os_name =='' ){
                                toast.error("Config Name, Config OS, Tables configured should be specified");
                              }else if(configList != undefined && configList?.data?.some((config:any) => config.query_name === selectedConfig.query_name)){
                                toast.error(`Config with name - ${selectedConfig.query_name} already present`);
                              }else{
                              updateMe(selectedConfig,CONFIG_ADD_URL,setAlertData)
                              setAlert(true)
                              // setSubmit("update")
                              setConfigList(
                                setSelectedConfig({
                                query_name:"",
                                os_name: "",
                                kernel_version: "",
                                os_arch: "",
                                query:{
                                  schedule:{
                                  }
                                }}))
                              }
                              getData(CONFIG_LIST_URL,setConfigList)

                            }
                            else{
                              // console.log("THis is update data: ",selectedConfig)
                              if(selectedConfig.query_name =='' ||selectedConfig.os_name =='' ){
                                toast.error("Config Name, Config OS, Tables configured should be specified");
                              }else{
                              updateMe(selectedConfig,CONFIG_UPDATE_URL,setAlertData)
                              setAlert(true)
                              setConfigList(
                                setSelectedConfig({
                                  query_name:"",
                                  os_name: "",
                                  kernel_version: "",
                                  os_arch: "",
                                  query:{
                                    schedule:{
                                    }
                                  }})
                              )
                              }
                              getData(CONFIG_LIST_URL,setConfigList)
                            }
                          }}>
                           { submit==="add" ? 'Make' : 'Update'}
                          </Button>
                        </div>
                      </div>
                    </div>
                  </div>:
                  <div className="container p-5 h-100 w-100">
                    <div className="row container w-100 h-100 d-flex justify-content-center align-items-center">
                      {/* <Typography variant="h6" component="h6">
                        Select Config
                      </Typography> */}
                      <div className="container d-flex justify-content-center">
                  <div className="col container p-0 mt-2 d-flex justify-content-center align-items-center" style={{ height: "3.5rem",width:"100%"}}>
                    <Button variant="outlined" color="secondary" style={{width:"75%",height:"100%",fontSize:"1.2rem"}} onClick={()=>{
                      setUpdateButton(false)
                      setSubmit("add")
                      setAdd(true)
                      setSelectedConfig({
                        query_name:"",
                        os_name: "",
                        query:{
                          schedule:{
                          }
                        }
                      })
                      setShowMakeConfigBtn(false)
                    }}>
                      Make Config<i className="fa fa-plus" style={{marginLeft:"1rem"}} aria-hidden="true"></i>
                    </Button>
                  </div>
                </div>
                    </div>
                  </div>
                }
              </div>
            </div>
          </div>
        </div>

        {/* List of Tables */}
        <div className="col-12 col-md-3">
          <div className="card py-3 mt-4 mt-md-0">
            <div className="d-flex flex-column align-items-start gap-3">
              <div className="row container w-100 h-100 d-flex justify-content-center align-items-center">
                <Typography variant="h6" component="h6">
                  List of Tables
                </Typography>
              </div>

                <div
                className='ch-18 w-100 flex-column vh-md-100px overflow-auto'
                id="#scrollbar-1"
              >
                <div
                  className="acroItem configTableList h-md-75"
                >
                {
                  tableList!==undefined&&selectedConfig!==undefined?tableList.data?.map((tableName:any,_:number)=>{
                    if(!Object.keys(selectedConfig.query.schedule).includes(tableName.table_name))
                    {
                      return <div className="container d-flex justify-content-center" key={_}>
                        <div className="col container m-1 w-100 h-3-5 d-flex align-items-center" style={{ border: "var(--bs-cyan) solid 1px"}}>
                            <div className='row w-100 ms-0'>
                              <div className="col-10 w-100 d-flex justify-content-center align-items-center">
                                <div className="d-flex flex-wrap w-100 h-100 justify-content-center align-items-center" onClick={()=>{
                                    setConfigModal(true)
                                    // setUpdateButton(true)
                                    setSelectedTable(tableName.table_name)
                                  }}>
                                
                                    <h6 className='fw-normal text-cyan p-md-2'>
                                    {tableName.table_name}{" "}
                                    { tableName.os_name?.includes('Windows')? <><i className='fa fa-windows'/></>:<></>}&nbsp;
                                    { tableName.os_name?.includes('Linux')? <><i className='fa fa-linux'/></>:<></>}&nbsp;
                                    { tableName.kernel_version? <Chip label={tableName.kernel_version} key={_} color="primary" size="small" variant="outlined"/> :<></>}
                                    </h6>
                                </div>
                              </div>
                            </div>
                        </div>
                      </div>
                    }
                  }):
                  <div className="container p-5 h-100 w-100">
                    <div className="container w-100 h-100 d-flex justify-content-center align-items-center">
                      <Typography variant="h6" component="h6">
                      Click on Add to display tables List
                      </Typography>
                    </div>
                  </div>
                }
                </div>
              </div>
              
            </div>
          </div>
        </div>
      </div>
    </>
  );
};

export default Configuration;
