import {useState} from 'react';
import { deleteMe, updateMe } from '../apis/requests';
import { DATA_RETENTION_DAYS_UPDATE, MANUAL_DATA_PURGE } from '../apis/urls';
import swal from 'sweetalert';

const DataPurge = () => {
  const [adays, setADays] = useState<any>('');
  const [mdays, setMDays] = useState<any>('');

  const autoPurge = () => {
    if(Number(mdays) >=30 ){
      swal({
        text: "Are you sure you want to set auto purge ?",
        icon: "warning",
        buttons: ['Cancel', 'Yes, Auto Purge'],
        dangerMode: true,
      })
      .then((willDelete) => {
        if (willDelete) {
          updateMe({ days: Number(adays) }, DATA_RETENTION_DAYS_UPDATE);
          swal("Your Settings have been saved!", {
            icon: "success",
          });
        }
      });
    }else{
      swal({
        text: "Retention days should be proper number of days (>30)",
        icon: "warning"
        // dangerMode: true,
      });
    }
  }

  const manualPurge = () => {
    if(Number(mdays) >=30 ){
      swal({
        text: "Are you sure you want to do manual purge ?",
        icon: "warning",
        buttons: ['Cancel', 'Yes, Purge Data'],
        dangerMode: true,
      })
      .then((willDelete) => {
        if (willDelete) {
          deleteMe({ days: Number(mdays) }, MANUAL_DATA_PURGE);
          swal("Your Data has been purged!", {
            icon: "success",
          });
        }
      });
    }else{
      swal({
        text: "Retention days should be proper number of days (>30)",
        icon: "warning"
        // dangerMode: true,
      });
    }
  }

  return (
    <>
      <div className="card py-3 border-0">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Data Purge</h4>
            </div>
          </div>
        </div>
        <div className="card-body px-6">
          <p className="mb-4">
            <span style={{ color: '#cc1313cc' }}>Note : </span>
            The Vajra Endpoint Response server stores data for alerts,
            scheduled query results, status logs, and deleted hosts.
          </p>
          <div className="card py-3">
            <div className="card-header bg-white border-0">
              <div className="hstack">
                <div>
                  <h5 className="fw-mid my-0">Data retention settings :</h5>
                </div>
              </div>
            </div>
            <div className="card-body px-6">
              <form id="data-retension">
                <div className="form-group row mb-6 required">
                  <label className="col-3 col-form-label">
                    Data retention interval :
                    <i
                      title="Automatically purge data daily and retain data for only last <X> days"
                      style={{ background: '#f8f9fa' }}
                      className="p-2 fa fa-info ms-2"
                    ></i>
                  </label>
                  <div className="col-6">
                    <input type="number" className="form-control" onChange={(e)=> setADays(Number(e.target.value))} required />
                    <div className="invalid-feedback">
                      Error On Purge Data Field
                    </div>
                    <p className="mt-1">days</p>
                  </div>
                </div>
                <div className="row">
                  <div className="col-3"></div>
                  <div className="col-6">
                    <button type="button" className="btn btn-primary" onClick={() => autoPurge()}>
                      <i className="fa fa-refresh pe-2"></i>
                      Update
                    </button>
                  </div>
                </div>
              </form>
            </div>
          </div>
          <div className="card py-3 mt-7">
            <div className="card-header bg-white border-0">
              <div className="hstack">
                <div>
                  <h5 className="fw-mid my-0">Manual data purge : </h5>
                </div>
              </div>
            </div>
            <div className="card-body px-6">
              <form id="data-retension">
                <div className="form-group row mb-6 required">
                  <label className="col-3 col-form-label">
                    Retain last x days data :
                    <i
                      title="Manually purge data now and retain data for only last <X> days"
                      style={{ background: '#f8f9fa' }}
                      className="p-2 fa fa-info ms-2"
                    ></i>
                  </label>
                  <div className="col-6">
                    <input type="number" className="form-control" onChange={(e)=> setMDays(Number(e.target.value))} required />
                    <div className="invalid-feedback">
                      Retention days should not be empty
                    </div>
                    <p className="mt-1">days</p>
                  </div>
                </div>
                <div className="row">
                  <div className="col-3"></div>
                  <div className="col-6">
                    <button type="button" className="btn btn-primary" onClick={()=> manualPurge()}>
                      <i className="fa fa-refresh pe-2"></i>
                      Purge
                    </button>
                  </div>
                </div>
              </form>
            </div>
          </div>
        </div>
      </div>
    </>
  );
};

export default DataPurge;
