import {useState, useEffect, useRef} from 'react';
import { ToastContainer, toast } from 'react-toastify';
import { YARA_FILE_LIST, YARA_FILE_UPLOAD } from '../../../apis/urls';
import axios from 'axios';
import { getData } from '../../../apis/requests';
import { formatBytes, timeConverter } from '../../../helpers';
import swal from 'sweetalert';

const Yara = () => {
  const [file, setFile] = useState<File |null>(null);
  const [yaraFiles,setYaraFiles] = useState<any>();
  const [isMenuOpen, setIsMenuOpen] = useState(false)
  const [activeRule, setActiveRule] = useState(0);
  const [platform, setPlatform] = useState<string>('');
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

  const handlePlatformChange = (e:any)=>{
    // console.log("Event: ",e.target.checked, e.target.value);
    const {value, checked } = e.target;
    let updatedPlatform = platform;

    if (checked) {
      // Add the OS name if checked
      if (!updatedPlatform.includes(value)) {
        updatedPlatform += updatedPlatform ? `,${value}` : value;
      }
    } else {
      // Remove the OS name if unchecked
      updatedPlatform = updatedPlatform
        .split(",")
        .map((os: string) => os.trim())
        .filter((os:any) => os !== value)
        .join(",");
    }

    setPlatform(updatedPlatform.replace(/\"/g, ''));
  }

  const handleFileUpload = async()=>{
    if(file){
      // console.log("File: ",file, "Platform:",platform);
      const formData = new FormData();
      formData.append('file', file);
      if(!file.name.endsWith('.yar')){
        toast.error("Only Yara files allowed");
        return;
      }

      if(platform ===''){
        toast.error('Platform should be specified');
        return;
      }
      try{
        axios.post(YARA_FILE_UPLOAD + '?username=' + localStorage.getItem('Username')+ '&platform='+platform, formData, {
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
    getData(YARA_FILE_LIST, setYaraFiles);
    document.addEventListener("click", handleClickOutside, false);
    return () => {
      document.removeEventListener("click", handleClickOutside, false);
    };
  },[])

  return (
    <>
    <ToastContainer/>
      <div className="card py-3">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Yara</h4>
            </div>
            <div className="ms-auto">
              <button
                type="button"
                className="btn btn-primary-light align-middle text-primary"
                data-bs-toggle="modal"
                data-bs-target="#ManageYaraModal"
              >
                <i className="fa fa-plus me-2" aria-hidden="true"></i>Add Yara
                File
              </button>
            </div>
          </div>
        </div>
        { yaraFiles?.data?.length === 0 ? <>
          <div className="card-body px-6">
            <p className='text-center text-secondary fw-bold'>No records found.</p>
            </div>
        </> : <>
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
                    Platform
                  </th>
                  <th className="fw-light" scope="col">
                      Size
                    </th>
                    <th className="fw-light" scope="col">
                      Uploaded At
                    </th>
                    <th className="fw-light" scope="col">
                      Uploaded By
                    </th>
                    <th></th>
                </tr>
              </thead>
              <tbody className="border-top-0">

                { yaraFiles?.data?.map((file: any, id: any) =>{
                  return (
                    <tr key={id}>
                    <td>{file.name}</td>
                    <td>
                      <span>
                        {file.platform.includes('indows')? <>
                        <i
                          className="fa fa-windows"
                          style={{ fontSize: '1rem', marginRight: '10px' }}
                          aria-hidden="true"
                        ></i>
                        Windows
                        </>: <></>}
                      </span>
                      <span style={{ marginLeft: '20px' }}>
                      {file.platform.includes('inux')? <>
                        <i
                          className="fa fa-linux"
                          style={{ fontSize: '1rem', marginRight: '10px' }}
                          aria-hidden="true"
                        ></i>
                        Linux</>:<></>}
                      </span>
                      <span style={{ marginLeft: '20px' }}>
                      {file.platform.includes('darwin')? <>
                        <i
                          className="fa fa-apple"
                          style={{ fontSize: '1rem', marginRight: '10px' }}
                          aria-hidden="true"
                        ></i>
                        Darwin</>:<></>}
                      </span>
                    </td>
                    <td>{formatBytes(file.size)}</td>
                    <td>{timeConverter(parseInt(file.uploadedAt)/1000)}</td>
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
                })}
              </tbody>
            </table>
          </div>
        </div>
        </>}
      </div>
      <div
        className="modal fade"
        id="ManageYaraModal"
        tabIndex={-1}
        aria-labelledby="ManageYaraModalLabel"
        aria-hidden="true"
      >
        <div className="modal-dialog">
          <div className="modal-content">
            <div className="modal-header">
              <h5 className="modal-title" id="ManageYaraModalLabel">
                Manage Yara File &nbsp;
                <i
                    title="File uploaded should be of type 'yara' only."
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
                <div className="form-group row align-items-center">
                  <label className="col-3">Platforms</label>
                  <div className="col-9">
                    <div className="form-check form-check-inline">
                      <input
                        className="form-check-input"
                        type="checkbox"
                        id="windows"
                        value="windows"
                        onChange={(e)=>handlePlatformChange(e)}
                      />
                      <label className="form-check-label" htmlFor="windows">
                        Windows
                      </label>
                    </div>
                    <div className="form-check form-check-inline">
                      <input
                        className="form-check-input"
                        type="checkbox"
                        id="linux"
                        value="linux"
                        onChange={(e)=>handlePlatformChange(e)}
                      />
                      <label className="form-check-label" htmlFor="linux">
                        Linux
                      </label>
                    </div>
                    <div className="form-check form-check-inline">
                      <input
                        className="form-check-input"
                        type="checkbox"
                        id="darwin"
                        value="darwin"
                        onChange={(e)=>handlePlatformChange(e)}
                      />
                      <label className="form-check-label" htmlFor="darwin">
                        Darwin
                      </label>
                    </div>
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
                onClick={()=>{
                  handleFileUpload()
                  getData(YARA_FILE_LIST, setYaraFiles);
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

export default Yara;
