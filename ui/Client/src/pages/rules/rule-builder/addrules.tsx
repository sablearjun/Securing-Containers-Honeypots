import { useEffect, useState } from 'react';
import { Link, useLocation, useNavigate } from 'react-router-dom';
import ReactSelect from 'react-select';
import ReactMultiSelectDropdown from '../../../components/react-multiselect';
import { Platforms, Tactics } from '../../../data/rules-data';
import { v4 as uuidv4 } from 'uuid';
import { conditionTableData } from '../../../apis/requests';
import { queryClause } from '../../../data/rules-data';
import { ToastContainer, toast } from 'react-toastify';
import { sendRuleBuilderData } from '../../../apis/requests';
import { severity as SeverityMaster } from '../../../data/rules-data';
import { RULE_BUILDER_RULE_ADD, RULE_BUILDER_RULE_UPDATE } from '../../../apis/urls';
import FormControl from '@mui/material/FormControl';
//import { PythonRule } from '../../../../types';
import InputLabel from '@mui/material/InputLabel';
import MenuItem from '@mui/material/MenuItem';
import { Title } from 'chart.js';

const AddRule = () => {

  const queryObj = {
    tableName: '',
    columnName: '',
    conditionType: '',
    value: '',
    dataType: "string",
    id: uuidv4()
  }

  const [ruleFormData, setRuleFormData] = useState<any>({
    name: '',
    description: '',
    alerters: '',
    severity: '',
    platform: '',
    type: 'Default',
    tactics: '',
    technique_id: '',
    link: '',
    rule_builder: {
      operator: 'AND' || 'OR',
      queries: []
    }
  })
  const location = useLocation();
  let navigate = useNavigate();
  const [mode,setMode] = useState<'add'| 'edit'>('add')
  const [isDisabled, setIsDisabled] = useState(true);
  const [queryRawData, setQueryRawData] = useState<any>([]);
  const [columnData, setColumnData] = useState<any>({});
  const [rerender, setRerender] = useState(false);
  const [showMITREInputs, setShowMITREInputs] = useState(false);
  const { name, description, alerters, conditions, severity, platform, type, tactics, technique_id, link,
    rule_builder : { operator, queries }
  } = ruleFormData;
  const [ruleId, setRuleId] = useState<any>(''); // For editing rule

  const formInputFormat = {
    name: '',
    description: '',
    alerters: '',
    severity: '',
    platform: '',
    type: 'Default',
    tactics: '',
    technique_id: '',
    link: '',
    rule_builder: {
      operator: 'AND' || 'OR',
      queries: [queryObj]
    }
  }

  // handle values of form element
  const handleChange = (name:string, index?:number, queryParam?:string) => (event : any) => {
    switch (name) {
      case 'platform':
      case 'tactics':
        setRuleFormData({...ruleFormData, [name]: event})
        break;
      case 'operator':
        setRuleFormData((prevState:any) => {
          prevState.rule_builder[name] = event.target.value
          return({...prevState})
        })
        break;
      case 'queries':
        if(queryParam === 'tableName') renderColumns(event.target.value, index!);
        setRuleFormData((prevState:any) => {
          prevState.rule_builder.queries[index!][queryParam!] = event.target.value
          return({...prevState})
        })
        break;
      case 'severity':
          setRuleFormData((prevState:any) => {
          prevState.rule_builder[name] = event.target.value
          return({...prevState})
        })
        break;

        
      default:
        setRuleFormData({...ruleFormData, [name]: event.target.value})
    }
  }

  // const onIOSEmojiPanelOpen= (name: string )=>(event :any) =>
  // {
  //   onIOSEmojiPanelOpen(name) {
  //     name.preventDefault();
  //   }
    
  // }

  const handleMultiSelectChange = (selectedValue:any) => setRuleFormData({...ruleFormData, alerters: selectedValue})

  const addRule = () => {
    let updateQueries = queries;
    updateQueries.push(queryObj)
    if(updateQueries.length < 5){
      setRuleFormData((prevState:any) => {
        prevState.rule_builder.queries = updateQueries;
        return({
          ...prevState
        })
      })
    }else{
      toast.error("You can add maximum 4 query rules")
    }

    updateQueries.length > 1 ? setIsDisabled(false) : setIsDisabled(true);
  }

  const deleteRule = (index:number) => {
    if(queries.length > 1){
      let newQueries = queries.filter((query:any) => query.id !== index)
      setRuleFormData((prevState:any) => {
        prevState.rule_builder.queries = newQueries;
        return({
          ...prevState
        })
      }) 
    }
  }

  useEffect(() => {
    // if(queries.length < 1) addRule() 
    addRule()
    conditionTableData()
      .then(res => setQueryRawData(res.data))
      .catch(err => console.log(err))
    if(location.state !==  null){
      setMode('edit')
      updateDataForEditMode(location.state)
    }else{
      setMode('add')
    }
  }, [])

  useEffect(() => {
    type === 'MITRE' ? setShowMITREInputs(true) : setShowMITREInputs(false)
  }, [type])

  const updateDataForEditMode = (propsData : any) => {
    let propsTactics = propsData.tactics ? JSON.parse(propsData.tactics) : [];
    let propsRule_builder = JSON.parse(propsData.rule_builder);
    let propsAlerters = propsData.alerters ? JSON.parse(propsData.alerters) : [];
    let platforms = JSON.parse(propsData.platform);

    let queriesFromDB : any = [];
    propsRule_builder.queries?.map((queElement:any, indexPoint:number) => {
      const queryObj = {
        tableName: queElement.tableName,
        columnName: queElement.columnName,
        conditionType: queElement.conditionType,
        value: queElement.value,
        dataType: queElement.dataType,
        id: queElement.id
      }
      queriesFromDB.push(queryObj)
      renderColumns(queElement.tableName, indexPoint);
    });

    setIsDisabled(false)
    setRuleId(propsData.id)
    setRuleFormData({...ruleFormData,
      name: propsData.name,
      description: propsData.description,
      alerters: propsAlerters,
      severity: (propsData.severity - 1) * 25,
      platform: platforms,
      type: propsData.type,
      tactics: propsTactics,
      technique_id: propsData.technique_id,

    
      link: propsData.link,
      rule_builder: {
        operator: propsRule_builder.operator,
        queries: queriesFromDB
      }
    })
  }

  const clearFormData = () => {
    setRuleFormData(formInputFormat)
  }

  const saveDetails = (e:any) => {
    e.preventDefault(); 
    let requestBody = ruleFormData;
    if(mode === 'edit'){
      requestBody.id = ruleId;
    }
    requestBody.severity = Math.floor(requestBody.severity/25+1);
    requestBody.rule_written_by = localStorage.getItem('Username');
    if(handleValidation()) {
      let url = (mode === 'add') ? RULE_BUILDER_RULE_ADD : RULE_BUILDER_RULE_UPDATE;
      // console.log(requestBody);
      sendRuleBuilderData(requestBody, url)
        .then((res :any) => {
          if(res.status === "Success") {
            toast.success(res.message);
            setRuleFormData(formInputFormat);  
            setTimeout(() => {
              setRuleId('')
              navigate("/rules/view-rules")
            }, 1000);
          } else {
            toast.error(res.message);
          }
        })
        .catch((err:any)=> toast.error(err))

    }

  }

  const renderColumns = (eventValue:string, index:number) =>  {
    let tableName = eventValue;
    // console.log("rawdata in render:", queryRawData); // This data does not arrive that is why column options are empty.
    let columnsArr : any = columnData;
    if(tableName !== ''){
      const tempColData = queryRawData?.filter((queryObj:any) => queryObj?.table_name === tableName);
      // console.log("TempCol :", tempColData);
      columnsArr[index] = tempColData;
      setColumnData(columnsArr)
      setRerender(!rerender);
    }
    // console.log("table: ", tableName, index, "Columns: ", columnsArr,);
  }

  const handleValidation =  () => {
    if(name === '') {
      toast.error('Rule name is mandatory.');
      return false;
    }
    const regex=/^[a-zA-Z0-9_]+$/;
    //   [a-zA-Z\s&\d]
     if(!regex.test(name))
     {
      toast.error('Enter valid Rule name.');
      return false;
     }
     if(name.length > 10 || name.length < 3 )
     {
      toast.error('Rule name should be 3-10 characters');
      return false;

     }
    //  if(!Title.match('[a-zA-Z\d]'))
    //  {
    //   toast.error('Enter valid Rule Title.');
    //   return false;
    //  }
    //console.log(platform)
    if(platform.length === 0){
      toast.error('Platform field is mandatory.');
      return false;
    }
    if(severity.length === 0){
      toast.error('Severity field is mandatory.');
      return false;
    }
    
    if(queries.length > 0) {
      const queriesValid = queries.every((item : any) => { 
        let t = item.tableName && item.columnName && item.conditionType && item.value && item.dataType;
        return t;
      });
      if(!queriesValid) {
        toast.error('Please add query conditions related details.');
        return false;
      }
    } else {
      toast.error('Conditions fields are mandatory.');
      return false;
    }
    return true;
  }

  return (
    <>
      <ToastContainer />
      <div className="card py-3">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Add Rule</h4>
            </div>
            <div className="ms-auto">
              <Link to="/rules/view-rules">
                <button
                  type="button"
                  className="btn btn-primary-light align-middle text-primary"
                >
                  View Rules
                </button>
              </Link>
            </div>
          </div>
        </div>
        <div className="card-body px-6">
          <form>
            <div className="form-group row mb-6 required">
              <label className="col-sm-2 col-form-label">Rule Name</label>
              <div className="col-sm-10">
                <input type="text" className="form-control" v-model="text"   required onChange={handleChange("name")} value={name} />
                {/* //onKeyDown={onIOSEmojiPanelOpen($event)} */}
                
                <div className="invalid-feedback">Rule Name is required</div>
              </div>
            </div>
            <div className="form-group row mb-6">
              <label className="col-sm-2 col-form-label">Description</label>
              <div className="col-sm-10">
                <textarea className="form-control" rows={3} onChange={handleChange("description")} value={description} />
              </div>
            </div>
            <div className="form-group row mb-6">
              <label className="col-sm-2 col-form-label">Alerters</label>
              <div className="col-sm-10">
                <ReactMultiSelectDropdown value={alerters} handleMultiSelectChange={handleMultiSelectChange} />
              </div>
            </div>
            <div className="form-group row mb-6 required">
              <label className="col-sm-2 col-form-label">Conditions</label>
              <div className="col-sm-10">
                <div id="query-builder" className="query-builder form-inline">
                  <div
                    id="query-builder_group_0"
                    className="rules-group-container"
                  >
                    <div className="rules-group-header">
                      <div className="btn-group group-conditions" onChange={handleChange('operator')}>
                        <label className={`btn ${operator == 'AND' ? 'btn-primary': 'btn-primary-light'} ${isDisabled ? 'disabled': ''}`}>
                          <input
                            type="radio"
                            name="condition"
                            value="AND"
                            checked={operator === 'AND'}
                            disabled={isDisabled}
                          />
                          AND
                        </label>
                        <label className={`btn btn-primary ${operator == 'OR' ? 'btn-primary': 'btn-primary-light'} ${isDisabled ? 'disabled': ''}`}>
                          <input
                            type="radio"
                            name="condition"
                            value="OR"
                            checked={operator === 'OR'}
                            disabled={isDisabled}
                          />
                          OR
                        </label>
                      </div>
                      <div className="btn-group group-actions pull-right">
                        <button
                          type="button"
                          className="btn btn-success"
                          data-add="rule"
                          onClick={addRule}
                        >
                          Add rule
                        </button>
                      </div>
                    </div>
                    <div className="rules-group-body">
                      <div className="rules-list">
                        {/* add class "has-error" for invalid field UI */}
                        {
                          queries?.map((query:any, index:number) => {
                            return (
                            <div
                              id="query-builder_rule_0"
                              className="rule-container"
                              key={index}
                              >
                              <div className="rule-header">
                                <div className="btn-group rule-actions">
                                  <button
                                    type="button"
                                    className="btn btn-danger"
                                    data-delete="rule"
                                    onClick={() => deleteRule(query.id)}
                                  >
                                    Delete
                                  </button>
                                </div>
                              </div>
                              <div className="rule-container-grid">
                                <div className="rule-filter-container">
                                  <div
                                    className="dropdown dropup"
                                    style={{ width: '170px' }}
                                  >
                                    <select
                                      className="form-select"
                                      name="tableName"
                                      tabIndex={-1}
                                      onChange={handleChange('queries', index, 'tableName')}
                                      value={queries[index].tableName}
                                    >
                                      <option value="" disabled selected>Table name</option>
                                      {queryRawData?.map((table:any,index:number) => <option key={index+table.table_name} value={table.table_name}>{table.table_label}</option>)}
                                    </select>
                                  </div>
                                </div>
                                <div className="rule-operator-container">
                                  <div
                                    className="dropdown dropup"
                                    style={{ width: '170px' }}
                                  >
                                    <select
                                      className="form-select"
                                      name="columnName"
                                      tabIndex={-1}
                                      onChange={handleChange('queries', index, 'columnName')}
                                      value={queries[index].columnName}
                                    >
                                      <option value="" disabled selected>Column name</option>
                                      {
                                        columnData[index] ?
                                        columnData[index][0]?.columns.map((column:any,index:number) => <option key={index+column.column_name} value={column.column_name}>{column.column_label}</option>)
                                        : ''
                                      }
                                    </select>
                                  </div>
                                </div>
                                <div className="rule-operator-container">
                                  <div
                                    className="dropdown dropup"
                                    style={{ width: '170px' }}
                                  >
                                    <select
                                      className="form-select"
                                      name="conditionType"
                                      tabIndex={-1}
                                      onChange={handleChange('queries', index, 'conditionType')}
                                      value={queries[index].conditionType}
                                    >
                                      <option value="" disabled selected>Clause</option>
                                      {
                                        queryClause?.map((clause, i) =>
                                        <option value={clause.value} key={i}>{clause.label}</option>
                                        )
                                      }
                                    </select>
                                  </div>
                                </div>
                              </div>
                              <div className="rule-value-container">
                                <input
                                  className="form-control"
                                  type="value"
                                  name="query-builder_rule_0_value_0"
                                  placeholder='value'
                                  onChange={handleChange('queries', index, 'value')}
                                  value={queries[index].value}
                                ></input>
                              </div>
                          </div>
                            )
                          })
                        }
                      </div>
                    </div>
                  </div>
                </div>
                <div className="invalid-feedback">
                  Please add rule conditions
                </div>
              </div>
            </div>
            { <div className="form-group row mb-6 required">
              <label className="col-sm-2 col-form-label">Severity</label>
              <div className="col-sm-10">
                <select
                  className="form-select"
                  
                  style={{
                    width: '10%',
                  }}
                  required onChange={(e:any)=>{
                    setRuleFormData((prev: any)=>({...prev,severity: (e.target.value) }))
                  }}
                  value={severity}
                >
                {
                  SeverityMaster?.map((val) => <option value={val.value}>{val.label}</option>)
                }
                </select>
              </div>
            </div> }

            {
            }


            <div className="form-group row mb-6 required">
              <label className="col-sm-2 col-form-label">Platform</label>
              <div className="col-sm-10">
                <ReactSelect
                  className="multiple-select"
                  classNamePrefix="multiple-select-element"
                  options={Platforms}
                  isMulti
                  blurInputOnSelect={false}
                  closeMenuOnSelect={false}
                  hideSelectedOptions={false}
                  placeholder="Nothing Selected"
                  isSearchable={true}
                  required onChange={handleChange("platform")}
                  value={platform}
                />
                <div className="invalid-feedback">Platform is required</div>
              </div>
            </div>
            <div className="form-group row mb-6">
              <label className="col-sm-2 col-form-label">Type</label>
              <div className="col-sm-10">
                <div className="form-check form-check-inline">
                  <input
                    className="form-check-input"
                    type="radio"
                    value="Default"
                    id="default"
                    name="inlineRadioOptions"
                    checked={type === 'Default'}
                    onChange={handleChange('type')} 
                  />
                  <label className="form-check-label" htmlFor="default">
                    Default
                  </label>
                </div>
                <div className="form-check form-check-inline">
                  <input
                    className="form-check-input"
                    type="radio"
                    value="MITRE"
                    id="mitre"
                    name="inlineRadioOptions"
                    checked={type === 'MITRE'} 
                    onChange={handleChange('type')}
                  />
                  <label className="form-check-label" htmlFor="mitre">
                    MITRE
                  </label>
                </div>
              </div>
            </div>
            {
              showMITREInputs && 
              <>
                <div className="form-group row mb-6">
                  <label className="col-sm-2 col-form-label">Tactics</label>
                  <div className="col-sm-10">
                    <ReactSelect
                      className="multiple-select"
                      classNamePrefix="multiple-select-element"
                      options={Tactics}
                      isMulti
                      blurInputOnSelect={false}
                      closeMenuOnSelect={false}
                      hideSelectedOptions={false}
                      placeholder="Nothing Selected"
                      isSearchable={true}
                      onChange={handleChange("tactics")}
                      value={tactics}
                    />
                  </div>
                </div>
                <div className="form-group row mb-6">
                  <label className="col-sm-2 col-form-label">Technique Id</label>
                  <div className="col-sm-10">
                    <input type="text" className="form-control" onChange={handleChange("technique_id")} value={technique_id} />
                  </div>
                </div>
              </>
            }
            <div className="form-group row mb-6">
              <label className="col-sm-2 col-form-label">Link</label>
              <div className="col-sm-10">
                <input type="text" className="form-control" onChange={handleChange("link")} value={link} />
              </div>
            </div>
            <div className="form-footer">
              <p>
                <span style={{ color: '#e32' }}>*</span> Mandatory Fields
              </p>
              <div className="mt-2 form-action w-25 mx-auto d-flex justify-content-evenly">
                <button className="btn btn-primary" onClick={(e) => saveDetails(e)}>
                  { mode === "edit" ? "Update Rule" : "Add Rule"}
                 
                  
                </button>
                <button className="btn btn-light" onClick={clearFormData}>
                  Clear
                </button>
              </div>
            </div>
          </form>
        </div>
      </div>
    </>
  );
};

export default AddRule;


