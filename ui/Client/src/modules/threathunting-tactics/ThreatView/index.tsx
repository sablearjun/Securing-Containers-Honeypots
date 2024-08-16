import React, { useEffect, useState } from 'react';
import { getData} from '../../../apis/requests';
import { GET_TECHNIQUE_URL } from '../../../apis/urls';

const ThreatView = (prop:any) => {
  const [techniques, setTechniques] = useState<any>(undefined)
  const [subTechniques, setSubTechniques] = useState<any>(undefined)

  useEffect(() => {
    getData(GET_TECHNIQUE_URL+"?tactic="+prop.TacticName,setTechniques)
  }, [])
  

  return (
    <>
    {
      console.log(techniques)
    }
    <div className="tab-pane fade show active" id="home" role="tabpanel" aria-labelledby="tab-1">
                        <div className="d-flex align-content-stretch flex-wrap fs-7">
                           <div className="d-flex align-self-stretch me-4 tactic-list">                              
                              <div className="my-3 w-100">
                                <div className="fw-bold fs-7 text-primary px-2 my-2 d-flex align-items-center">
                                 <p className="my-0 me-5 lh-sm">Gather (4)</p>
                                 <hr className="flex-fill" />
                                </div>
                                <div className="list-group list-group-flush lh-md">
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Hardware</h6>
                                      </div>
                                      <span className="bg-danger border-0 status-dot" />
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Software</h6>
                                      </div>
                                      <span className="bg-orange border-0 status-dot" />
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Firmware</h6>
                                      </div>
                                      <span className="bg-warning border-0 status-dot"/>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Configuration in complex long statement</h6>
                                      </div>
                                      <span className="bg-success border-0 status-dot" />
                                    </div>
                                  </a>
                                </div>
                              </div>
                           </div>

                           <div className="d-flex align-self-stretch me-4 tactic-list">                              
                              <div className="my-3 w-100">
                                <div className="fw-bold fs-7 text-primary px-2 my-2 d-flex align-items-center">
                                 <p className="my-0 me-5 lh-sm">Gather Victim Information (4)</p>
                                 <span className="badge rounded-pill bg-secondary text-dark align-middle"></span>
                                 <hr className="flex-fill"/>
                                </div>
                                <div className="list-group list-group-flush">
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Hardware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">18 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Software</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Firmware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                </div>
                              </div>
                           </div>

                           <div className="d-flex align-self-stretch me-4 tactic-list">                              
                              <div className="my-3 w-100">
                                <div className="fw-bold fs-7 text-primary px-2 my-2 d-flex align-items-center">
                                 <p className="my-0 me-5 lh-sm">Gather Victim Host Information Information (4)</p>
                                 <span className="badge rounded-pill bg-secondary text-dark align-middle"></span>
                                 <hr className="flex-fill"/>
                                </div>
                                <div className="list-group list-group-flush">
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Hardware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">18 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Software</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Firmware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                </div>
                              </div>
                           </div>

                           <div className="d-flex align-self-stretch me-4 tactic-list">                              
                              <div className="my-3 w-100">
                                <div className="fw-bold fs-7 text-primary px-2 my-2 d-flex align-items-center">
                                 <p className="my-0 me-5 lh-sm">Gather (4)</p>
                                 <span className="badge rounded-pill bg-secondary text-dark align-middle"></span>
                                 <hr className="flex-fill" />
                                </div>
                                <div className="list-group list-group-flush">
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Hardware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">18 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Software</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Firmware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                </div>
                              </div>
                           </div>

                           <div className="d-flex align-self-stretch me-4 tactic-list">                              
                              <div className="my-3 w-100">
                                <div className="fw-bold fs-7 text-primary px-2 my-2 d-flex align-items-center">
                                 <p className="my-0 me-5 lh-sm">Gather Victim Host Information (4)</p>
                                 <span className="badge rounded-pill bg-secondary text-dark align-middle"></span>
                                 <hr className="flex-fill" />
                                </div>
                                <div className="list-group list-group-flush">
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Hardware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">18 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Software</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Firmware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                </div>
                              </div>
                           </div>

                           <div className="d-flex align-self-stretch me-4 tactic-list">                              
                              <div className="my-3 w-100">
                                <div className="fw-bold fs-7 text-primary px-2 my-2 d-flex align-items-center">
                                 <p className="my-0 me-5 lh-sm">Gather Victim Information (4)</p>
                                 <hr className="flex-fill" />
                                </div>
                                <div className="list-group list-group-flush">
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Hardware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">18 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Software</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Firmware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                </div>
                              </div>
                           </div>


                           <div className="d-flex align-self-stretch me-4 tactic-list">                              
                              <div className="my-3 w-100">
                                <div className="fw-bold fs-7 text-primary px-2 my-2 d-flex align-items-center">
                                 <p className="my-0 me-5 lh-sm">Gather Victim Host (4)</p>
                                 <hr className="flex-fill" />
                                </div>
                                <div className="list-group list-group-flush">
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Hardware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">18 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Software</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Firmware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                </div>
                              </div>
                           </div>

                           <div className="d-flex align-self-stretch me-4 tactic-list">                              
                              <div className="my-3 w-100">
                                <div className="fw-bold fs-7 text-primary px-2 my-2 d-flex align-items-center">
                                 <p className="my-0 me-5 lh-sm">Gather Victim Host Information (4)</p>
                                 <span className="badge rounded-pill bg-secondary text-dark align-middle"></span>
                                 <hr className="flex-fill" />
                                </div>
                                <div className="list-group list-group-flush">
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Hardware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">18 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Software</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Firmware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <button className="btn btn-sm btn-outline-indigo-light text-indigo w-100 mt-1"><i className="fi fi-rr-add me-2 align-middle"></i>Expand Tactics 23 more </button>
                                </div>
                              </div>
                           </div>



                           <div className="d-flex align-self-stretch me-4 tactic-list">                              
                              <div className="my-3 w-100">
                                <div className="fw-bold fs-7 text-primary px-2 my-2 d-flex align-items-center">
                                 <p className="my-0 me-5 lh-sm">Gather Victim Host Information (4)</p>
                                 <span className="badge rounded-pill bg-secondary text-dark align-middle"></span>
                                 <hr className="flex-fill" />
                                </div>
                                <div className="list-group list-group-flush">
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Hardware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">18 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Software</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">Firmware</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">Shubham</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Config</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Configuration</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Configuration</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Configuration</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Configuration</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Configuration</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Configuration</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                  <a href="#" className="list-group-item list-group-item-action border-0 d-flex gap-3 p-list px-2" aria-current="true">
                                    <div className="d-flex gap-1 w-100 justify-content-between">
                                      <div>
                                        <h6 className="mb-0 fs-7 fw-mid">System Configuration</h6>
                                      </div>
                                      <small className="opacity-50 text-nowrap">12 Hosts</small>
                                    </div>
                                  </a>
                                </div>
                              </div>
                           </div>



                        </div>
                       </div>
                       </>
  );
}

export default ThreatView;
