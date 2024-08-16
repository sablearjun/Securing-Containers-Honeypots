import { useEffect, useState } from "react";
import { getData } from "../../../apis/requests";
import { GET_TACTIC_URL } from "../../../apis/urls";
import ThreatView from "../ThreatView";

const ThreatSelectionNavbar = () => {
  
  const [tactic, setTactic] = useState<any>(undefined)

  useEffect(() => {
    getData(GET_TACTIC_URL,setTactic)    
  }, [])

  return (
    <>
      <div className="d-flex justify-content-between align-items-center gap-3 w-100">
        <div className="d-flex justify-content-between align-items-center gap-0">
          <ul className="nav nav-tabs justify-content-between mx-2">
            {
              tactic!==undefined?tactic["tactic_list"].map((tac:any,_:number)=>{
                return <li key={_} className="nav-item">
                  <a className={_===0?("nav-link px-2 active"):("nav-link px-2")}
                    id={"tab-"+_}
                    data-bs-toggle="tab"
                    data-bs-target={'#'+tac.tactic_name}
                    type="button"
                    role="tab"
                    aria-controls={tac.tactic_name}
                    aria-selected="true"
                  >
                    {tac.tactic_name}
                    <span className="badge rounded-pill bg-primary-light text-dark ms-2">
                      26
                    </span>
                  </a>
                </li>
              }):<></>
            }
            {/* <li className="nav-item">
              <a
                className="nav-link px-2"
                id="tab-2"
                data-bs-toggle="tab"
                data-bs-target="#profile"
                type="button"
                role="tab"
                aria-controls="profile"
                aria-selected="false"
              >
                Resource Development
                <span className="badge rounded-pill bg-primary-light text-dark ms-2">
                  10
                </span>
              </a>
            </li>
            <li className="nav-item">
              <a
                className="nav-link px-2"
                id="tab-3"
                data-bs-toggle="tab"
                data-bs-target="#profile"
                type="button"
                role="tab"
                aria-controls="profile"
                aria-selected="false"
              >
                Initial Access (22)
              </a>
            </li>
            <li className="nav-item">
              <a className="nav-link px-2 me-2" href="#">
                Execution (21)
              </a>
            </li>
            <li className="nav-item">
              <a className="nav-link px-2" href="#">
                Defence Evasion (23)
              </a>
            </li>
            <li className="nav-item">
              <a className="nav-link px-2" href="#">
                Discovery (23)
              </a>
            </li>
            <li className="nav-item">
              <a className="nav-link px-2" href="#">
                Lateral Movement (24)
              </a>
            </li>
            <li className="nav-item">
              <a className="nav-link px-2" href="#">
                Command & Control (24)
              </a>
            </li> */}
          </ul>
        </div>
      </div>
      <div
      className="tab-content card card-body px-3 py-0 w-100 overflow-auto"
      id="myTabContent"
      style={{ height: "calc(100vh - 23rem)" }}
    >
      {
        tactic!==undefined?tactic["tactic_list"].map((tac:any,_:number)=>{
          return <div
          key={_}
          className="tab-pane fade show active"
          id={tac.tactic_name}
          role="tabpanel"
          aria-labelledby="tab-0"
        >
          <ThreatView TacticName={tac.tactic_name}/>
        </div>
      }):<></>
    }
    </div>
      {/* <!-- Modal --> */}
                      <div className="modal fade" id="exampleModal" tabIndex={-1} aria-labelledby="exampleModalLabel" aria-hidden="true">
                        <div className="modal-dialog modal-dialog-centered">
                        <div className="modal-content">
                           <div className="modal-header">
                              <h5 className="modal-title" id="exampleModalLabel">Alert Details</h5>
                              <button type="button" className="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                           </div>
                           <div className="modal-body">
                              <div>

                              </div>
                           </div>
                           <div className="modal-footer">
                              <button type="button" className="btn btn-light" data-bs-dismiss="modal">Close</button>
                              <button type="button" className="btn btn-primary">Take Action</button>
                           </div>
                        </div>
                        </div>
                     </div>
                     {/* <!-- Modal::end --> */}
    </>
  );
};

export default ThreatSelectionNavbar;
