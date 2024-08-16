import React, { useState } from 'react';
import Button from 'react-bootstrap/Button';
import Modal from 'react-bootstrap/Modal';
import InputAdornment from '@mui/material/InputAdornment';
import TextField from '@mui/material/TextField';
import MenuItem from '@mui/material/MenuItem';
import Select, { SelectChangeEvent } from '@mui/material/Select';

interface config{
  interval:number,
  query:string,
  snapshot:boolean
}

export const VajraModal = (props:any) => {
    const handleClose = () => props.setter(false);
    const [configSettings, setConfigSettings] = useState<config>({
      interval:0,
      query:"",
      snapshot:false
    })

    return (
        <>
          <Modal show={props.state} onHide={handleClose}>
            <Modal.Header closeButton>
              <Modal.Title>{props.table}</Modal.Title>
            </Modal.Header>
            <Modal.Body>
              <div className="row">
                <div className="col-2 d-flex justify-content-start align-items-center">
                  Interval
                </div>
                <div className="col-10">
                <TextField
                  label="Interval in seconds"
                  id="outlined-start-adornment"
                  sx={{ m: 1, width: '25ch' }}
                  type="number"
                  onChange={(e)=>{
                    setConfigSettings((prev)=>({...prev,query:"select * from "+props.table}))
                    setConfigSettings((prev:any)=>({...prev,interval:e.target.value}))
                  }}
                  InputProps={{
                    startAdornment: <InputAdornment position="start">Sec</InputAdornment>,
                  }}
                />
                </div>
              </div>
              <div className="row mt-2">
                <div className="col-2 d-flex justify-content-start align-items-center">
                  Snapshot
                </div>
                <div className="col-10 ps-5">
                <Select
                  labelId="demo-simple-select-label"
                  id="demo-simple-select"
                  value={configSettings.snapshot===true?'1':'0'}
                  label="Age"
                  onChange={(event: SelectChangeEvent)=>{
                      setConfigSettings((prev)=>({...prev,query:"select * from "+props.table}))
                      setConfigSettings((prev)=>({...prev,snapshot:parseInt(event.target.value)===0?false:true}))
                  }}
                >
                  <MenuItem value={1}>True</MenuItem>
                  <MenuItem value={0}>False</MenuItem>
                </Select>
                </div>
              </div>
            </Modal.Body>
            <Modal.Footer>
              <Button variant="secondary" onClick={()=>{
                props.button(false)
                handleClose()
              }}>
                Close
              </Button>
              <Button variant="primary" onClick={()=>{
                let tm={...props.config}
                tm.query.schedule[props.table]=configSettings
                props.setting(tm)
                handleClose()
              }}>
                Add
              </Button>
            </Modal.Footer>
          </Modal>
        </>
      );
}
