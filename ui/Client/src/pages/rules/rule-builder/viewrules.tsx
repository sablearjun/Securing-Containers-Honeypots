import { useEffect, useRef, useState } from 'react';
import { Link, useNavigate } from 'react-router-dom';
import { deleteMe, getData } from '../../../apis/requests';
import { RULE_BUILDER_RULE_DELETE, RULE_BUILDER_RULE_LIST } from '../../../apis/urls';
import { timeConverter } from '../../../helpers';
import { severity } from '../../../data/rules-data';
import swal from 'sweetalert';
import { ToastContainer, toast } from 'react-toastify'

const ViewRules = () => {
  const navigate = useNavigate()
  const [modalData, setModalData] = useState<any>(null);
  const [isMenuOpen, setIsMenuOpen] = useState(false)
  const [activeRule, setActiveRule] = useState(0)
  const tableRefs = useRef<any>([]);
  const [ruleData, setRuleData] = useState<any>([])
  const [searchQuery, setSearchQuery] = useState<any>('');
  const [filteredResults, setFilteredResults] = useState<any>([]);
  
  const toggleOpen = (ruleId : any) => {
    setIsMenuOpen(!isMenuOpen); setActiveRule(ruleId)
  }
  const handleClickOutside = (event:any) => {
    if (tableRefs.current && !tableRefs.current.contains(event.target)) { 
      setIsMenuOpen(false);
    }
  };
  // to check onclick outside tbody below is the same as componentDidMount and componentDidUnmount
  useEffect(() => {
    getData(RULE_BUILDER_RULE_LIST, setRuleData);
    document.addEventListener("click", handleClickOutside, false);
    return () => {
      document.removeEventListener("click", handleClickOutside, false);
    };
  }, []);

  const valueLabelFormat = (value:any) => {
    let resultValue = severity.find((severityVal) => severityVal.value === (value - 1)*25)
    return resultValue?.label
  }

  const queriesList = (queryData : any) => {
    let ruleBuilder = JSON.parse(queryData);
    return ruleBuilder?.queries?.map((rule:any, index:number) => {
      return <span>
              Select * from {rule.tableName} where {rule.columnName} {rule.conditionType} {rule.value}
              <span className='fw-bold text-primary mx-2'>
                {index < ruleBuilder?.queries.length - 1 ? ruleBuilder.operator : ''}
              </span>
            </span>
    })
  }

  const deleteRule = (ruleID : number) => {
    setIsMenuOpen(false)
    swal({
      text: "Are you sure want to delete this rule ?",
      icon: "warning",
      buttons: ['Cancel', 'Yes, delete'],
      dangerMode: true,
    })
    .then((willDelete) => {
      if (willDelete) {
        deleteMe({ id:ruleID }, RULE_BUILDER_RULE_DELETE)
        getData(RULE_BUILDER_RULE_LIST, setRuleData);
        swal("Rule has been deleted successfully!", {
          icon: "success",
        }).then((willDel) => window.location.reload());
      }
    });
  }

  const searchRules = (search: string) => {
    setSearchQuery(search);
    if (searchQuery !== '') {
      const filteredData = ruleData?.data?.filter((item:any) => {
          return Object.values(item).join('').toLowerCase().includes(searchQuery.toLowerCase())
      })
      setFilteredResults(filteredData);
      if(filteredData.length === 0)
        {
          toast.info("No rules with such keyword!",{
            toastId:"no_rules"
          });
        }
    }
    else{
        setFilteredResults(ruleData?.data);
    }
  }
  return (
    <>
     <ToastContainer/>
      <div className="card py-3">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Vajra Rules</h4>
            </div>
            <div className="ms-auto">
              <Link to="/rules/add-rules">
                <button
                  type="button"
                  className="btn btn-primary-light align-middle text-primary"
                >
                  <i className="fa fa-plus me-2" aria-hidden="true"></i>Add
                  Rules
                </button>
              </Link>
            </div>
          </div>
        </div>
        {
          ruleData?.data?.length > 0 ?
          <div className="card-body px-6">
          <div className="row mb-5 align-items-center">
            <div className="col-md-6 row-filter-search">
              <div className="input-group w-50" >
                <input
                  type="search"
                  placeholder="Search"
                  className="form-control"
                  onChange={(e)=> searchRules(e.target.value)}
                />
                <i
                  className="fa fa-search text-primary"
                  style={{
                    position: 'absolute',
                    right: '10px',
                    top: '50%',
                    transform: 'translateY(-50%)',
                  }}
                ></i>
              </div>
            </div>
            <div className="col-md-6 d-flex row-select-filter justify-content-end">
              <div className="form-group">
                <label className="text-muted">Results per page:&nbsp;</label>
                <select
                  className="form-select-sm p-1"
                  style={{ border: '1px solid silver' }}
                >
                  <option value="10">10</option>
                  <option value="25">25</option>
                  <option value="50">50</option>
                  <option value="100">100</option>
                </select>
              </div>
            </div>
          </div>
          <div
            className="w-100 overflow-auto"
            style={{ height: 'calc(100vh - 27rem)' }}
            id="#scrollbar-1"
          >
            <table className="table table-hover">
              <thead
                className="bg-white"
                style={{ position: 'sticky', top: 0, zIndex: 950 }}
              >
                <tr className="fw-light text-muted">
                  <th className="fw-light" scope="col">
                    Name
                  </th>
                  <th className="fw-light" scope="col">
                    Severity
                  </th>
                  <th className="fw-light" scope="col">
                    Created On
                  </th>
                  <th className="fw-light" scope="col">
                    Written By
                  </th>
                  <th></th>
                </tr>
              </thead>
              <tbody className="border-top-0">
                {searchQuery.length>1 ? 
                <>
                {filteredResults?.map((rule : any, index : number) => {
                  return (
                    <tr key={index}>
                      <td>{rule.name}</td>
                      <td>{valueLabelFormat(rule?.severity)}</td>
                      <td>
                        {
                          rule?.created !== undefined ?
                          <p>{timeConverter(parseInt(rule?.created))}</p>:<></>
                        }
                      </td>
                      <td>{rule.rule_written_by}</td>
                      <td>
                        <div className="dropstart" onClick={() => toggleOpen(rule.id)}>
                          <a type="button" id="dropdownMenuButton" data-bs-toggle="dropdown" aria-haspopup="true" aria-expanded="false" 
                            >
                            <i className="fa fa-ellipsis-v"></i>
                          </a>
                          <div className={`dropdown-menu${isMenuOpen && activeRule == rule.id ? " show" : ""}`} aria-labelledby="dropdownMenuButton">
                            <a className="dropdown-item" type="button"
                              data-bs-toggle="modal"
                              data-bs-target="#ViewRulesModal"
                              onClick={() => setModalData(rule)}
                            >
                              View
                            </a>
                            <a className="dropdown-item" type="button" onClick={() => navigate('/rules/add-rules', { state: rule})}>
                              Edit
                            </a>
                            <a className="dropdown-item" type="button" onClick={() => deleteRule(rule.id)}>
                              Delete
                            </a>
                          </div>
                        </div>
                      </td>
                    </tr>
                  );
                })}
                </>:<>
                {ruleData?.data?.map((rule : any, index : number) => {
                  return (
                    <tr key={index}>
                      <td>{rule.name}</td>
                      <td>{valueLabelFormat(rule?.severity)}</td>
                      <td>
                        {
                          rule?.created !== undefined ?
                          <p>{timeConverter(parseInt(rule?.created))}</p>:<></>
                        }
                      </td>
                      <td>{rule.rule_written_by}</td>
                      <td>
                        <div className="dropstart" onClick={() => toggleOpen(rule.id)}>
                          <a type="button" id="dropdownMenuButton" data-bs-toggle="dropdown" aria-haspopup="true" aria-expanded="false" 
                            >
                            <i className="fa fa-ellipsis-v"></i>
                          </a>
                          <div className={`dropdown-menu${isMenuOpen && activeRule == rule.id ? " show" : ""}`} aria-labelledby="dropdownMenuButton">
                            <a className="dropdown-item" type="button"
                              data-bs-toggle="modal"
                              data-bs-target="#ViewRulesModal"
                              onClick={() => setModalData(rule)}
                            >
                              View
                            </a>
                            <a className="dropdown-item" type="button" onClick={() => navigate('/rules/add-rules', { state: rule})}>
                              Edit
                            </a>
                            <a className="dropdown-item" type="button" onClick={() => deleteRule(rule.id)}>
                              Delete
                            </a>
                          </div>
                        </div>
                      </td>
                    </tr>
                  );
                })}</>}
              </tbody>
            </table>
          </div>
        </div>
          :
          <div className="card-body px-6">
            <p className='text-center text-secondary fw-bold'>No records found.</p>
          </div>
        }
      </div>
      <div
        className="modal fade"
        id="ViewRulesModal"
        tabIndex={-1}
        aria-labelledby="ViewRulesModalLabel"
        aria-hidden="true"
      >
        <div className="modal-dialog view-rule-modal">
          <div className="modal-content">
            <div className="modal-header">
              <h5 className="modal-title" id="ViewRulesModal">
                View Rule Details
              </h5>
              <button
                type="button"
                className="btn-close"
                data-bs-dismiss="modal"
                aria-label="Close"
              ></button>
            </div>
            <div className="modal-body">
              <div className="container">
                <div className="row align-items-start">
                  <div className="col">
                    <p className="fw-bold mb-0">Rule ID</p>
                    <p>{modalData?.id}</p>
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Name</p>
                    <p>{modalData?.name}</p>
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Severity</p>
                    <p>{valueLabelFormat(modalData?.severity)}</p>
                  </div>
                </div>
                <div className="row align-items-start">
                  <div className="col">
                    <p className="fw-bold mb-0">Technique ID</p>
                    <p>{modalData?.technique_id}</p>
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Platform</p>
                    <p>
                    {
                        modalData?.platform ? (
                            typeof JSON.parse(modalData.platform) === 'string' ? (
                                <span>{JSON.parse(modalData.platform)}</span>
                            ) : (
                                (() => {
                                    const values = JSON.parse(modalData.platform).map((plt: any) => plt.label);
                                    const OS = values.join(', ');
                                    // console.log(OS);
                                    return <span>{OS}</span>;
                                })()
                            )
                        ) : (
                            ''
                        )
                    }
                    </p>
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Tactics</p>
                    <p>{
                        modalData?.tactics ?
                        typeof JSON.parse(modalData?.tactics) === 'string' 
                        ? JSON.parse(modalData?.tactics)
                        : JSON.parse(modalData?.tactics).map((tactic:any, i:number) => {
                            return <span>{(i ? ', ' : '') + tactic.label}</span>
                          })
                        : ''}
                    </p>
                  </div>
                </div>
                <div className="row align-items-center">
                  <div className="col">
                    <p className="fw-bold mb-0">Link</p>
                    <p>{modalData?.link}</p>
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Created On</p>
                    {
                      modalData?.created !== undefined ?
                      <p>{timeConverter(parseInt(modalData?.created))}</p>:<></>
                    }
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Last Modified On</p>
                    {
                      modalData?.last_modified !== undefined ?
                      <p>{timeConverter(parseInt(modalData?.last_modified))}</p>:<></>
                    }
                  </div>
                </div>
                <div className='row'>
                  <div className="col">
                    <p className="fw-bold mb-0">Description</p>
                    <p>{modalData?.description}</p>
                  </div>
                </div>
                <div className="row align-items-center">
                  <div className="col-10 rule-builder-list">
                    <p className="fw-bold mb-0">Rules</p>
                    <code>
                    {
                      modalData?.rule_builder !== undefined ? queriesList(modalData?.rule_builder) : ''
                    }
                    </code>
                  </div>
                </div>
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
            </div>
          </div>
        </div>
      </div>
    </>
  );
};

export default ViewRules;
