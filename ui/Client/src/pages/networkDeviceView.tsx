import { useLocation } from "react-router-dom";
import SecondaryTable from "../components/SecondaryTable";
import { networkData } from "../data/networkdevices";

interface NetworkDevice {
  device_name: string;
  enrolled_on: string;
  last_seen: string;
  mac_address: string;
}
const NetworkDeviceSheetView = () => {
  const location = useLocation();
  let edata: any = location.state as NetworkDevice;
  return (
    <>
      <div>
        <div className="card card-body">
          <div className="hstack">
            <div className="d-flex flex-column justify-content-start w-100">
              <div className="hstack justify-content-between">
                <h6 className="fs-7 fw-semi mt-0 mb-1">
                  Network Device Summary
                </h6>
                {/* <p className="my-0 form-label text-orange fst-italic me-1">
                  Optional Update here
                </p> */}
              </div>
              <div className="d-flex flex-column flex-md-row gap-3 justify-content-between">
                <div className="vstack gap-0">
                  <div className="d-flex justify-content-start align-items-center">
                    <p className="my-0 form-label text-muted fw-light me-1">
                      Device Name:
                    </p>
                    <p className="mb-0 btn-link bg-white fw-mid text-dark btn-sm">
                      {edata.device_name}
                    </p>
                  </div>
                  <div className="d-flex justify-content-start align-items-center">
                    <p className="my-0 form-label text-muted fw-light me-1">
                      MAC Address:
                    </p>
                    <button className="btn btn-link fw-mid text-dark btn-sm">
                      {edata.mac_address}
                    </button>
                  </div>
                </div>
                <div className="vstack gap-0">
                  <div className="d-flex justify-content-start align-items-center">
                    <p className="my-0 form-label text-muted fw-light me-1">
                      Enrolled on:
                    </p>
                    <button className="btn btn-link fw-mid text-dark btn-sm">
                      {/* {timeConverter(parseInt(edata.enrolled_on))} */}
                      {edata.enrolled_on}
                    </button>
                  </div>
                </div>
                <div className="vstack gap-0">
                  <div className="hstack gap-6">
                    <div className="d-flex justify-content-start align-items-center">
                      <p className="my-0 form-label text-muted fw-light me-1">
                        Last seen:
                      </p>
                      <button className="btn btn-link fw-mid text-dark btn-sm">
                        {/* {edata.status === "Offline"
                              ? timeConverter(parseInt(edata.last_seen))
                              : lastSeenValue} */}
                        {edata.last_seen}
                      </button>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>

        {/* Table */}
        <div className="card card-body my-3">
          <div className="d-flex flex-column flex-md-row">
            <h4 className="fw-mid my-0">{edata.device_name}</h4>
          </div>
          <SecondaryTable
            tableState={{
              Data: JSON.parse(networkData.network_devices),
              Loading: false,
              Error: "",
            }}
            name="Router Logs"
          />
        </div>
      </div>
    </>
  );
};

export default NetworkDeviceSheetView;
