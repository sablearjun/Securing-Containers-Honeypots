import { useState, useEffect } from "react";
import { Polling } from "../../apis/requests";
import { NODE_COUNT_URL } from "../../apis/urls";

interface params {
  name: string;
  display?: "none";
}

const Banner = ({ name, display }: params) => {
  const [MachineStatus, setMachineStatus] = useState<any>(undefined);
  // const [semaphore, setSemaphore] = useState<number>(0)
  Polling(NODE_COUNT_URL,setMachineStatus,5000,2)
  // useInterval(()=>{
  //     if(semaphore<2)
  //     {
  //         setSemaphore(prev=>(prev+1))
  //         getData(NODE_COUNT_URL,setMachineStatus,"GET",{},setSemaphore)
  //     }
  // },1000)

  /* SSE implementation */
  // useEffect(() => {
  //   console.log("name: ",name);
  //   if(display!=="none"){
  //   const eventSource = new EventSource(NODE_COUNT_URL + "sse/"); // Adjust the URL to match our SSE route
  //   // console.log(eventSource);
  //   eventSource.onmessage = (event) => {
  //     const eventData = JSON.parse(event.data);
  //     setMachineStatus(eventData);
  //   };
  //   return () => {
  //     eventSource.close();
  //   };
  // }
  // }, []);

  return (
    <div className="card card-body" style={{ display: display }}>
      <div className="hstack">
        <div
          className="d-flex flex-column justify-content-start row col-md-2"
        >
          <h3 className="fw-mid my-0">{name}</h3>
          {/* <div>
            <div className="mt-2">
              <button
                className="fs-8 px-1 btn btn-link text-muted"
                onClick={() => {}}
              >
                7 Days
              </button>
              <button
                className="fs-8 px-1 btn btn-link text-muted active"
                onClick={() => {}}
              >
                15 Days
              </button>
              <button
                className="fs-8 px-1 btn btn-link text-muted"
                onClick={() => {}}
              >
                180 Days
              </button>
            </div>
          </div> */}
        </div>
        <div className="w-100">
          <div className="row mb-3 mb-md-0 col-md-8">
            <div className="col-12 col-md-2 col-lg-2 border-end mb-3 mb-md-0">
              <div
                className="row d-flex justify-content-center align-items-center"
                style={{ fontSize: "1.5rem" }}
              >
                Online
              </div>
              <div
                className="row d-flex justify-content-center align-items-center text-success"
                style={{ fontSize: "2rem" }}
              >
                {MachineStatus !== undefined ? MachineStatus?.["Online"] : ""}/
                {MachineStatus !== undefined ? MachineStatus?.["Total"] : ""}
              </div>
            </div>
            <div className="col-12 col-md-2 col-lg-2">
              <div
                className="row d-flex justify-content-center align-items-center"
                style={{ fontSize: "1.5rem" }}
              >
                Offline
              </div>
              <div
                className="row d-flex justify-content-center align-items-center text-danger"
                style={{ fontSize: "2rem" }}
              >
                {MachineStatus !== undefined ? MachineStatus?.["Offline"] : ""}
                /{MachineStatus !== undefined ? MachineStatus?.["Total"] : ""}
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default Banner;
