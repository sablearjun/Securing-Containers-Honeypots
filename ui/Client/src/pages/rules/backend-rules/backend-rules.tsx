import Editor from '@monaco-editor/react'
import { useState, useEffect, useRef, createRef } from 'react'
import { Link, useNavigate } from "react-router-dom"
import swal from 'sweetalert'
import { getData } from '../../../apis/requests'
import { LIST_BACKEND_RULE_URL } from '../../../apis/urls'
import { severity } from '../../../data/rules-data'
import { timeConverter } from '../../../helpers'
import { ToastContainer, toast } from 'react-toastify'

interface BackendRuleData {
    id: String,
    tactic: String,
    technique: String,
    technique_id: String,
    link: String,
    severity: String,
    os_name: String,
    query: String
}

export const ViewBackendRules = () => {

    const navigate = useNavigate()
    const [ruleList, setRuleList] = useState<any>()
    const [modalData, setModalData] = useState<BackendRuleData | null>(null);
    const [isMenuOpen, setIsMenuOpen] = useState(false)
    const [activeRule, setActiveRule] = useState(0)
    const tableRefs = useRef<any>([]);
    const [result, setResult] = useState<any>();
    const [searchQuery, setSearchQuery] = useState<any>('');
    const [filteredResults, setFilteredResults] = useState<any>([]);

    // useEffect(() => {
    //   console.log(modalData)
    // }, [modalData])

    // to check onclick outside tbody below is the same as componentDidMount and componentDidUnmount
    useEffect(() => {
      // fetch rule list data
      getData(LIST_BACKEND_RULE_URL,setRuleList)
      setResult(ruleList?.data);
      // document.addEventListener("click", handleClickOutside, false);
      // return () => {
      //   document.removeEventListener("click", handleClickOutside, false);
      // };
    }, []);

    // const handleClickOutside = (event:any) => {
    //   if (tableRefs.current && !tableRefs.current.contains(event.target)) { 
    //     setIsMenuOpen(false);
    //   }
    // };

    const searchRules = (search: string) => {
      setSearchQuery(search);
      if (searchQuery !== '') {
        const filteredData = ruleList?.data?.filter((item:any) => {
            return Object.values(item).join('').toLowerCase().includes(searchQuery?.toLowerCase())
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
          setFilteredResults(ruleList?.data);
      }
    }
    return (
        <>
        <ToastContainer/>
      <div className="card py-3">
        <div className="card-header bg-white border-0">
          <div className="d-flex flex-column flex-md-row justify-content-center">
            <div>
              <h4 className="fw-mid my-0">Backend Rules</h4>
            </div>
            <div className='ms-auto mt-auto'>
              {searchQuery?.length>0? <><h5 className='fw-normal'>{filteredResults?.length} Rows</h5></>: <><h5 className='fw-normal'>{ruleList?.data?.length} Rows</h5></>}
            </div>
            <div className='ms-auto'>
              <div className="col-md-6 w-100 row-filter-search">
                <div className="input-group w-50" >
                  <input
                    type="search"
                    placeholder="Search"
                    className="form-control"
                    onChange={(e)=>{
                      searchRules(e.target.value)
                    }}
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
                    Id
                  </th>
                  <th className="fw-light" scope="col">
                    Tactic
                  </th>
                  <th className="fw-light" scope="col">
                    Technique
                  </th>
                  <th className="fw-light" scope="col">
                    Technique Id
                  </th>
                  <th className="fw-light" scope="col">
                    Severity
                  </th>
                  <th className="fw-light" scope="col">
                    OS Name
                  </th>
                </tr>
              </thead>
              <tbody className="border-top-0" ref={tableRefs}>
                {searchQuery?.length > 0 ?
                <>
                  { filteredResults!==undefined && filteredResults?.map((rule:any, index:number) => {
                    return <tr key={index}>
                      <td>{rule?.id}</td>
                      <td>{rule?.tactic?.toUpperCase()}</td>
                      <td>{rule?.technique}</td>
                      <td><a href={rule?.link == null? '/': rule?.link} target="_blank" rel="noopener noreferrer">{rule?.technique_id}</a></td>
                      <td>{rule?.severity}</td>
                      <td>
                        {rule?.os_name == 'Windows' ? <i className="fa fa-windows"></i> : <i className="fa fa-linux"></i>} {rule?.os_name}
                      </td>
                    </tr>
                  })
                }
                </>:<>
                { ruleList!==undefined && ruleList?.data?.map((rule:any, index:number) => {
                    return <tr key={index}>
                      <td>{rule?.id}</td>
                      <td>{rule?.tactic?.toUpperCase()}</td>
                      <td>{rule?.technique}</td>
                      <td><a href={rule.link == null? '/': rule?.link} target="_blank" rel="noopener noreferrer">{rule?.technique_id}</a></td>
                      <td>{rule?.severity}</td>
                      <td>
                        {rule?.os_name == 'Windows' ? <i className="fa fa-windows"></i> : <i className="fa fa-linux"></i>} {rule?.os_name}
                      </td>
                    </tr>
                  })
                }</>}
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </>
    )
}

export default ViewBackendRules