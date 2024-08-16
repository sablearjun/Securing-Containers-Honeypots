import Editor from '@monaco-editor/react'
import { useState, useEffect, useRef, createRef } from 'react'
import { Link, useNavigate } from "react-router-dom"
import swal from 'sweetalert'
import { deleteMe, getData } from '../../../apis/requests'
import { DELETE_PYTHON_RULE_URL, LIST_PYTHON_RULE_URL } from '../../../apis/urls'
import { severity } from '../../../data/rules-data'
import { timeConverter } from '../../../helpers'

interface PythonRuleData {
  id: Number,
  name: string,
  severity: Number,
  os_name: string,
  technique: string,
  rule_written_by: string,
  comment: string,
  link: string,
  rule: string,
  created: string,
  last_modified: string
}

export const ViewPythonRule = () => {

    const navigate = useNavigate()
    const [ruleList, setRuleList] = useState<any>()
    const [modalData, setModalData] = useState<PythonRuleData | null>(null);
    const [isMenuOpen, setIsMenuOpen] = useState(false)
    const [activeRule, setActiveRule] = useState(0)
    const tableRefs = useRef<any>([]);

    useEffect(() => {
      // console.log(modalData)
    }, [modalData])

    // to check onclick outside tbody below is the same as componentDidMount and componentDidUnmount
    useEffect(() => {
      // fetch rule list data
      getData(LIST_PYTHON_RULE_URL,setRuleList)
      document.addEventListener("click", handleClickOutside, false);
      return () => {
        document.removeEventListener("click", handleClickOutside, false);
      };
    }, []);

    const handleClickOutside = (event:any) => {
      if (tableRefs.current && !tableRefs.current.contains(event.target)) { 
        setIsMenuOpen(false);
      }
    };

    const deletePythonRule = (ruleID : number) => {
      setIsMenuOpen(false)
      swal({
        text: "Are you sure want to delete this python rule ?",
        icon: "warning",
        buttons: ['Cancel', 'Yes, delete'],
        dangerMode: true,
      })
      .then((willDelete) => {
        if (willDelete) {
          deleteMe({ id:ruleID }, DELETE_PYTHON_RULE_URL);
          swal("Your python rule has been deleted!", {
            icon: "success",
          });
          getData(LIST_PYTHON_RULE_URL,setRuleList);
        }
      });
    }

    function isJsonString(str:string) {
      try {
          JSON.parse(str);
      } catch (e) {
          return false;
      }
      return true;
  }

    const toggleOpen = (ruleId : any) => {
      console.log(ruleId)
      setIsMenuOpen(!isMenuOpen); setActiveRule(ruleId)
    }

    const timeStampToDate = (timestamp:any) => new Date(Number(timestamp)).toLocaleString()

    const valueLabelFormat = (value:any) => {
      let resultValue = severity.find((severityVal) => severityVal.value === (value - 1)*25)
      return resultValue?.label
    }

    return (
        <>
      <div className="card py-3">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Python Rules</h4>
            </div>
            <div className="ms-auto">
              <Link to="/rules/add-python-rule">
              <button
                type="button"
                className="btn btn-primary-light align-middle text-primary"
              >
                <i className="fa fa-plus me-2" aria-hidden="true"></i>Add Python
                Rule
              </button>
              </Link>
            </div>
          </div>
        </div>
        <div className="card-body px-6">
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
                    Rule Name
                  </th>
                  <th className="fw-light" scope="col">
                    OS Type
                  </th>
                  <th className="fw-light" scope="col">
                    Technique
                  </th>
                  <th className="fw-light" scope="col">
                    
                  </th>
                </tr>
              </thead>
              <tbody className="border-top-0" ref={tableRefs}>
                {
                  ruleList?.data?.map((rule:any, index:number) => {
                    return <tr key={index}>
                      <td>{rule.name}</td>
                      <td>
                        {rule.os_name == 'Windows' ? <i className="fa fa-windows"></i> : <i className="fa fa-linux"></i>} {rule.os_name}
                      </td>
                      <td>{rule.technique}</td>
                      <td>
                        <div className="dropstart" onClick={() => toggleOpen(rule.id)}>
                          <a type="button" id="dropdownMenuButton" data-bs-toggle="dropdown" aria-haspopup="true" aria-expanded="false" 
                            >
                            <i className="fa fa-ellipsis-v"></i>
                          </a>
                          <div className={`dropdown-menu${isMenuOpen && activeRule == rule.id ? " show" : ""}`} aria-labelledby="dropdownMenuButton">
                            <a className="dropdown-item" type="button"
                              data-bs-toggle="modal"
                              data-bs-target="#ViewPythonModal"
                              onClick={() => setModalData(rule)}
                            >
                              View
                            </a>
                            <a className="dropdown-item" type="button" onClick={() => navigate('/rules/add-python-rule', { state: rule})}>
                              Edit
                            </a>
                            <a className="dropdown-item" type="button" onClick={() => deletePythonRule(rule.id)}>
                              Delete
                            </a>
                          </div>
                        </div>
                      </td>
                    </tr>
                  })
                }
              </tbody>
            </table>
          </div>
        </div>
      </div>
      <div
        className="modal fade"
        id="ViewPythonModal"
        tabIndex={-1}
        aria-labelledby="ViewPythonModalLabel"
        aria-hidden="true"
      >
        <div className="modal-dialog python-rule-modal">
          <div className="modal-content">
            <div className="modal-header">
              <h5 className="modal-title" id="ViewPythonModal">
                {modalData?.name}
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
                    <p className="fw-bold mb-0">OS Type</p>
                    <p>{modalData?.os_name}</p>
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Technique</p>
                    <p>{modalData?.technique}</p>
                  </div>
                </div>
                <div className="row align-items-center">
                  <div className="col">
                    <p className="fw-bold mb-0">Severity</p>
                    <p>{valueLabelFormat(modalData?.severity)}</p>
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Link</p>
                    <p>{modalData?.link}</p>
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Written By</p>
                    <p>{modalData?.rule_written_by}</p>
                  </div>
                </div>
                <div className="row align-items-center">
                  <div className="col">
                    <p className="fw-bold mb-0">Created On</p>
                    {
                      modalData?.created!==undefined?
                      <p>{timeConverter(parseInt(modalData?.created))}</p>:<></>
                    }
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Last Modified</p>
                    {
                      modalData?.last_modified!==undefined?
                      <p>{timeConverter(parseInt(modalData?.last_modified))}</p>:<></>
                    }
                  </div>
                  <div className="col">
                    <p className="fw-bold mb-0">Comment</p>
                    <p>{modalData?.comment}</p>
                  </div>
                </div>
                <div className="row align-items-end">
                  <div className="col">
                    <p className='fw-bold'>Rule</p>
                    {
                      modalData?.rule!==undefined&&isJsonString(modalData?.rule)?
                      <Editor
                       options={{readOnly: true}}
                        height="40vh"
                        width="100%"
                        language="python"
                        theme="vs-light"
                        value={JSON.parse(modalData?.rule)}
                      />:<Editor
                      options={{readOnly: true}}
                      height="40vh"
                      width="100%"
                      language="python"
                      theme="vs-light"
                      value={modalData?.rule}
                    />
                    }
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
    )
}

export default ViewPythonRule