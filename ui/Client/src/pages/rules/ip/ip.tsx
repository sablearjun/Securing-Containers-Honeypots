import {useState, useEffect, useRef} from 'react';
import { ToastContainer, toast } from 'react-toastify';
import { IP_FILE_LIST, IP_FILE_UPLOAD } from '../../../apis/urls';
import axios from 'axios';
import { getData } from '../../../apis/requests';
import { formatBytes, timeConverter } from '../../../helpers';
import swal from 'sweetalert';

const Ip = () => {
  const [file, setFile] = useState<File |null>(null);
  const [IPFiles,setIPFiles] = useState<any>();
  const [isMenuOpen, setIsMenuOpen] = useState(false)
  const [activeRule, setActiveRule] = useState(0);
  const tableRefs = useRef<any>([]);

  const toggleOpen = (Id : any) => {
    console.log("id: ",Id);
    setIsMenuOpen(!isMenuOpen);setActiveRule(Id)
  }
  const handleFileChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    if (event.target.files && event.target.files.length > 0) {
      setFile(event.target.files[0]);
    }
  };

  const handleClickOutside = (event:any) => {
    if (tableRefs.current && !tableRefs.current.contains(event.target)) {
      setIsMenuOpen(false);
    }
  };

  const handleFileUpload = async()=>{
    //console.log("File: ");
    if(!file)
    {
     
      toast.error('Please select a file.')
      return false
    }
    
    
    else{
      // console.log("File: ",file);
      const formData = new FormData();
      formData.append('file', file);
      if(file.type!='text/csv'){
        toast.error("Only CSV files allowed");
        return;
      }
      try{
        axios.post(IP_FILE_UPLOAD + '?username=' + localStorage.getItem('Username'), formData, {
          headers: {
            'Content-Type': 'multipart/form-data', // Important for handling form data
            'Authorization': "Token " + localStorage.getItem("Token"),
          },
        })
        .then((response) => {
          console.log('Response:', response.data);
          // Handle the response data as needed
          if(response.data.status == "Success"){
            toast.success(response.data.message);
          }else{
            toast.error(response.data.message);
          }
        })
        .catch((error) => {
          console.error('Error:', error);
          // Handle errors here
        });
      }catch(error){
        console.error('An error occured: ', error);
      }
    }
    // else if (!file){
    //   toast.error('Please select a file.')
    //   return false
    // }
  }

  const deleteFile = (id : number, file: any) => {
    setIsMenuOpen(false)
    swal({
      text: "Are you sure want to delete this rule ?",
      icon: "warning",
      buttons: ['Cancel', 'Yes, delete'],
      dangerMode: true,
    })
    .then((willDelete) => {
      if (willDelete) {
        // deleteMe({ id:ruleID }, RULE_BUILDER_RULE_DELETE)
        // getData(RULE_BUILDER_RULE_LIST, setRuleData);

        // Feature not implemented.
        swal("Work in progress!", {
          icon: "success",
        }).then((willDel) => window.location.reload());
      }
    });
  }
  useEffect(()=>{
    getData(IP_FILE_LIST, setIPFiles);
    document.addEventListener("click", handleClickOutside, false);
    return () => {
      document.removeEventListener("click", handleClickOutside, false);
    };
  },[])
    return (
      <>
        <ToastContainer />
        <div className="card py-3">
          <div className="card-header bg-white border-0">
            <div className="hstack">
              <div>
                <h4 className="fw-mid my-0">IP</h4>
              </div>
              <div className="ms-auto">
                <button
                  type="button"
                  className="btn btn-primary-light align-middle text-primary"
                  data-bs-toggle="modal"
                  data-bs-target="#ManageIpModal"
                >
                  <i className="fa fa-plus me-2" aria-hidden="true"></i>Add IP
                  File
                </button>
              </div>
            </div>
          </div>
          { IPFiles?.data?.length ===0 ? <>
            <div className="card-body px-6">
            <p className='text-center text-secondary fw-bold'>No records found.</p>
            </div>
          </>:<>
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
                      Filename
                    </th>
                    <th className="fw-light" scope="col">
                      Uploaded At
                    </th>
                    <th className="fw-light" scope="col">
                      Size
                    </th>
                    <th className="fw-light" scope="col">
                      Uploaded By
                    </th>
                    <th></th>
                  </tr>
                </thead>
                <tbody className="border-top-0">
                  {IPFiles?.data?.map((file:any, id: any)=>{
                    return (
                      <tr key={id}>
                        <td>{file.name}</td>
                        <td>{timeConverter(parseInt(file.uploadedAt)/1000)}</td>
                        <td>{formatBytes(file.size)}</td>
                        <td>{file.uploadedBy}</td>
                        <td>
                          {/* <div className="dropstart" onClick={() => toggleOpen(id)}>
                            <a type="button" id="dropdownMenuButton" data-bs-toggle="dropdown" aria-haspopup="true" aria-expanded="false" 
                              >
                              <i className="fa fa-ellipsis-v"></i>
                            </a>
                            <div className={`dropdown-menu${isMenuOpen && activeRule == id? " show" : ""}`} aria-labelledby="dropdownMenuButton">
                              <a className="dropdown-item" type="button"
                                // data-bs-toggle="modal"
                                // data-bs-target="#ViewRulesModal"
                                onClick={() => {}}
                              >
                                View
                              </a>
                              <a className="dropdown-item" type="button" onClick={() => deleteFile(id, file)}>
                                Delete
                              </a>
                            </div>
                          </div> */}
                        </td>
                      </tr>
                    )
                  })
                  }
                </tbody>
              </table>
            </div>
          </div>
          </>}
        </div>
        <div
          className="modal fade"
          id="ManageIpModal"
          tabIndex={-1}
          aria-labelledby="ManageIPModalLabel"
          aria-hidden="true"
        >
          <div className="modal-dialog">
            <div className="modal-content">
              <div className="modal-header">
                <h5 className="modal-title" id="ManageIPModalLabel">
                  Upload CSV File of malicious IPs &nbsp;
                  <i
                    title="File uploaded should be of type 'csv' only and contain only one column - 'ip'. The values in each row should be valid IPv4/IPv6 addresses so that they will be saved in database. No blank rows should be present."
                    className="p-2 fa fa-info ms-2 bg-primary-light"
                  ></i>
                </h5>
                <button
                  type="button"
                  className="btn-close"
                  data-bs-dismiss="modal"
                  aria-label="Close"
                ></button>
              </div>
              <div className="modal-body">
                <form>
                  <div className="form-group row align-items-center mb-5">
                    <label className="col-3">Upload File</label>
                    <div className="col-9">
                      <input
                        type="file"
                        className="form-control-file"
                        id="formControlFile"
                        onChange={handleFileChange}
                      />
                    </div>
                  </div>
                </form>
              </div>
              <div className="modal-footer">
                <button
                  type="button"
                  className="btn btn-secondary"
                  data-bs-dismiss="modal"
                >
                  Close
                </button>
                <button
                  type="button"
                  // disabled={true}
                  className="btn btn-primary"
                  data-bs-dismiss="modal"
                  onClick={()=>{handleFileUpload()
                    getData(IP_FILE_LIST, setIPFiles);
                  }}
                >
                  Upload
                </button>
              </div>
            </div>
          </div>
        </div>
      </>
    );
  };

  export default Ip;
