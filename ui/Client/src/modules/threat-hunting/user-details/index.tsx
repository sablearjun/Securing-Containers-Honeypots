import React, { FC, useEffect, useState,useReducer } from "react";
import Modal1 from '@mui/material/Modal';
import Box from '@mui/material/Box';
import Autocomplete from '@mui/material/Autocomplete';
import TextField from '@mui/material/TextField';
import Button1 from '@mui/material/Button';

/* eslint-disable jsx-a11y/anchor-is-valid */
const top100Films = [
  { name: 'Initial Access', ID: 'TA0001' },
  { name: 'Execution', ID: 'TA0002' },
  { name: 'Persistence', ID: 'TA0003' },
  { name: 'Privilege Escalation', ID: 'TA0004' },
  { name: 'Defense Evasion', ID: 'TA0005' },
  { name: 'Credential Access', ID: 'TA0006' },
  { name: 'Discovery', ID: 'TA0007' },
  { name: 'Lateral Movement', ID: 'TA0008' },
  { name: 'Collection', ID: 'TA0009' },
  { name: 'Command and Control', ID: 'TA0011' },
  { name: 'Exfiltration', ID: 'TA0010' },
  { name: 'Impact', ID: 'TA0040' },
];

const UserDetails = (props:any) => {

  useEffect(() => {

  }, [])
  

  const [open, setOpen] = useState(false);
  const handleOpen = () => setOpen(true);
  const handleClose = () => {
    setOpen(false)
    setTactic(null)
    setTechnique(null)
    setSubTechnique(null)
  };
  const [tactic, setTactic] = useState<any>(null)
  const [technique, setTechnique] = useState<any>(null)
  const [subTechnique, setSubTechnique] = useState<any>(null)

  const Modal=()=>{
    const style = {
      position: 'absolute' as 'absolute',
      top: '50%',
      left: '50%',
      transform: 'translate(-50%, -50%)',
      display:"flex",
      width: "40vw",
      // height:"30vh",
      bgcolor: 'background.paper',
      // border: '2px solid #000',
      boxShadow: 24,
      p: 2,
    };

    return <div>
    <Modal1
      open={open}
      onClose={handleClose}
      aria-labelledby="modal-modal-title"
      aria-describedby="modal-modal-description"
    >
      <Box sx={style}>
        <div className="modal-body">
          <form>
            <div className="row">
              <div className="col-6">
              <div className="form-floating mb-4">
              <Autocomplete
                disablePortal
                value={tactic}
                id="combo-box-demo"
                onChange={(event,value)=>{
                  setTactic(value)
                }}
                options={top100Films}
                getOptionLabel={(option:any) => option.name}
                renderInput={(params) => <TextField {...params} label="Tactic" />}
              />
            </div>
              </div>
              <div className="col-6">
                {
                  tactic!==null?
                  <Autocomplete
                    disablePortal
                    value={technique}
                    id="combo-box-demo"
                    onChange={(event,value)=>{
                      setTechnique(value)
                    }}
                    options={top100Films}
                    getOptionLabel={(option:any) => option.name}
                    renderInput={(params) => <TextField {...params} label="Technique" />}
                  />:
                  <Autocomplete
                    disablePortal
                    disabled
                    id="combo-box-demo"
                    options={top100Films}
                    getOptionLabel={(option:any) => option.name}
                    renderInput={(params) => <TextField {...params} label="Technique" />}
                  />
                }
              </div>
            </div>
            {
              technique!==null?
              <Autocomplete
                disablePortal
                value={subTechnique}
                id="combo-box-demo"
                options={top100Films}
                onChange={(event,value)=>{
                  setSubTechnique(value)
                }}
                getOptionLabel={(option:any) => option.name}
                renderInput={(params) => <TextField {...params} label="Sub-Technique" />}
              />:
              <Autocomplete
                disablePortal
                disabled
                id="combo-box-demo"
                options={top100Films}
                getOptionLabel={(option:any) => option.name}
                renderInput={(params) => <TextField {...params} label="Sub-Technique" />}
              />
            }
          </form>
          <div className="d-flex flex-row-reverse mt-5 gap-5">
            <Button1 color="secondary" variant="outlined" onClick={()=>{
              window.location.reload()
            }}>Save</Button1>
            <Button1 variant="outlined" color="error" onClick={()=>{
              handleClose()
            }}>Cancel</Button1>
          </div>
        </div>
      </Box>
    </Modal1>
  </div>
  }
  return (
    <>
    <Modal/>
    <div className="d-flex align-items-stretch justify-content-between pt-3 gap-7 w-100">
      <div className="d-flex flex-column align-items-start justify-content-between flex-fill">
        <div className="hstack gap-3">
          <h4 className="fw-mid my-0">Threat Hunt</h4>
        </div>
        {/* <p className="text-muted fs-7 mb-0 mt-1 text-wrap px-1">
          Here's an example of adding some textual context for this add new
          threat hunt UI Here's an example of adding some textual context for
          this add new threat hunt UI for this add new threat hunt UI
        </p> */}
      </div>
      <div className="d-flex flex-column align-items-stretch justify-content-between">
        <div className="d-flex justify-content-end align-items-center gap-2">
          {/* <button
            className="btn text-muted btn-outline-secondary text-nowrap"
            onClick={() => {window.location.href='/view-python-rule'}}
          >
            Add Python Rules
          </button> */}
          <button
            className="btn text-muted btn-outline-secondary text-nowrap"
            // onClick={() => {window.location.href='/threathunting-tactics'}}
          >
            New Alert
          </button>
          <button className="btn text-muted btn-outline-secondary text-nowrap">
            Save as New
          </button>
          <div className="vr h-8px my-auto"></div>
          <button className="btn btn-primary w-100px text-nowrap" onClick={handleOpen} disabled>Save</button>
        </div>
        <div>
          <p className="text-muted my-0 fw-mid fst-italic fs-7 roboto text-end">
            {/* Status update about the latest changes made in TH */}
          </p>
        </div>
      </div>
    </div>
    </>
  );
};

UserDetails.defaultProps={
  runQuery:false
}

export default UserDetails;
