import React, { FC } from "react";

import { Event } from "../../../types";

interface ViewModalProps {
  event: Event;
}
const ViewModal: FC<ViewModalProps> = (props: ViewModalProps) => {
  const { event } = props;
 
  // console.log(event.id);
  // console.log("next");
  return (
    <>
    
      <button
        type="button"
        className="btn btn-primary-light ntn-sm"
        data-bs-toggle="modal"
        data-bs-target={"#exampleModal"+event.id}
        
      >
        View
      </button>
    
      <div
        className="modal fade"
        id={"exampleModal"+event.id}
        tabIndex={-1}
        aria-labelledby="exampleModalLabel"
        aria-hidden="true"
      >
        <div className="modal-dialog modal-dialog-centered">
          <div className="modal-content">
            <div className="modal-header">
              <h5 className="modal-title" id="exampleModalLabel">
                Alert Details
              </h5>
              <button
                type="button"
                className="btn-close"
                data-bs-dismiss="modal"
                aria-label="Close"
              ></button>
            </div>

            <div className="modal-body">
              {/* {console.log("inside")}
            {console.log({event})} */}
              <pre>{JSON.stringify({event}, null, 2)}</pre>
             
              {/* {Object.entries(event).map(([key,val])=>
              <pre key={key}>{key}:{val}
              {console.log("inside")}
            {console.log({val})}
              </pre>)} */}
            </div>
            <div className="modal-footer">
              <button
                type="button"
                className="btn btn-light"
                data-bs-dismiss="modal"
              >
                Close
              </button>
              <button type="button" className="btn btn-primary">
                Take Action
              </button>
            </div>
          </div>
        </div>
      </div>
    </>
  );
};

export default ViewModal;
