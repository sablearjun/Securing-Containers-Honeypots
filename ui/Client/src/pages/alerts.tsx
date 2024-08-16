import AlertStatus from "../modules/alerts/alert-status";
import AlertTable from "../modules/alerts/alert-tables";
const Alerts = (params:any) => {
  
  return (
    <>
      {/* <div className="card card-body">
        <AlertStatus />
        


      </div> */}

      {/* <div className="card py-3"> */}
        <AlertTable notification={params?.notification} setNotification={params?.setNotification}/>
      {/* </div> */}
    </>
  );
};

export default Alerts;
