import React, { memo, useReducer, useState } from "react";
import CsvDownloadButton from "react-json-to-csv";
import { fileName, timeConverter, formatDuration } from "../../helpers";
import { INITIAL_STATE, INITIAL_STATE_TYPE, reducerFunc } from "../../apis/requests/reducer";
import LoadGraph from "../loadGraph";
import { Error } from "../Error";
import { AutoSizer, Table, Column } from "react-virtualized";
import Modal from "@mui/material/Modal";
import Box from "@mui/material/Box";
import Typography from "@mui/material/Typography";
import SecondaryTable from ".";
import { BARgraph, PIEchart } from "../graph";

interface tableParams {
  tableState: INITIAL_STATE_TYPE;
  name: string;
  columns?: Array<string>;
  convertTime?: Array<string>;
  convertNanoTime?: Array<string>;
  dropdown?: any;
  Height?: number;
  Width?: number;
  ErrorMesg?: string;
}

const style = {
  position: "absolute" as "absolute",
  top: "50%",
  left: "50%",
  transform: "translate(-50%, -50%)",
  width: "80vw",
  height: "85vh",
  bgcolor: "background.paper",
  border: "2px solid #000",
  boxShadow: 24,
  p: 4,
  // zIndex: 100,
};

const ProgramsTable: React.FC<tableParams> = memo(
  ({
    tableState,
    name,
    columns,
    convertTime,
    convertNanoTime,
    dropdown,
    Height,
    Width,
    ErrorMesg,
  }: tableParams) => {
    let size =
      "calc(100vh - " + (Height ? Height.toString() + "rem)" : "30rem)");
    ErrorMesg = ErrorMesg === undefined ? "Data Not Available" : ErrorMesg;
    const [search, setSearch] = useState<string>("");
    const [open, setOpen] = useState<boolean>(false);

    const [rowData, setRowData] = useState<any>({});
    const [pieData, setPieData] = useReducer(reducerFunc,INITIAL_STATE);
    const [barData, setBarData] = useReducer(reducerFunc,INITIAL_STATE)

    const stringToArray = (rowData:any, column : string)=>{
      if(rowData !== undefined && typeof(rowData[column]) === "string"){
        const tmp = rowData[column];
        rowData[column] = JSON.parse(tmp);
      }
      return rowData;
    }
    const graphDataMaker = (rowData:any)=>{
      const severityCount :any = {};
      const impactCount : any = {};

      for(let i=0; i<=10; i++){
        impactCount[i] = 0;
      }

      rowData?.cve_match?.forEach((cpe:any) => {
        const severity = cpe.baseSeverity;
        severityCount[severity] = (severityCount[severity] || 0) + 1;
        const impactScore : number = Number(cpe.impactScore);
        impactCount[impactScore] = impactCount[impactScore] +1;
      });


      const data :any = Object.keys(severityCount).map(severity => ({
      "name": severity,
      "value": severityCount[severity]
      }));
      // console.log(data);

      setPieData({ type: "Fetched", payload: data });
      setBarData({ type: "Fetched", payload: impactCount});
    }


    const handleRowClick = (e: any, rowData: any) => {
      setOpen(true);
      let updatedData = stringToArray(rowData, 'cve_match');
      setRowData(updatedData);
      graphDataMaker(updatedData);
    };

    const handleClose = () => {
      setOpen(false);
      setRowData({});
    }

    return (
      <>
        <Modal
          open={open}
          onClose={handleClose}
          aria-labelledby="modal-modal-title"
          aria-describedby="modal-modal-description"
        >
          <Box sx={style} className="overflow-auto">
            <div className="d-flex flex-md-row flex-column">
            <div className="col-12 col-md-4">
              <Typography id="modal-modal-title" variant="h5" component="h2">
                <span className="text-primary">Program:</span> {rowData?.name}
              </Typography>
              <Typography id="modal-modal-description" sx={{ mt: 2 }}>
              <span className="fw-bold">Publisher:</span> {rowData?.publisher}<br/>
              <span className="fw-bold">Version:</span> {rowData?.version}
              </Typography>
              <div className="col-12 col-md-9 text-break">
              <Typography id="modal-modal-content" sx={{ mt: 2 }}>
                <p className="fw-bold">Installation Details</p>
                <p><span className="fw-bold">Path:</span> {rowData?.install_location}</p>
                <p><span className="fw-bold">Source:</span> {rowData?.install_source}</p>
                <p><span className="fw-bold">Severity:</span> {rowData?.severity}</p>
              </Typography>
              </div>
            </div>
            <div className="col-12 col-md-8 d-flex justify-content-end align-items-top p-2">
              {rowData?.cve_match?.length >=0  ?
              <div className="col-12 d-flex flex-column flex-md-row justify-content-between align-items-center p-6">
                  {/* <div className="d-flex justify-content-center align-items-center bg-info mx-4 p-6 my-md-3">
                    <h4>{rowData.cpe?.length} Vulnerabilities</h4>
                  </div> */}
                  <div className="col-12 col-md-7">
                  <BARgraph name={"Impact Count"} renderState={barData} label={"Impact Distribution"} color={["#82CA9D","#CD3636","#ABC9FF"]}/>
                  </div>
                  <div className="col-12 col-md-4">
                    <PIEchart name={"Vulnerabilities"} renderState={pieData} dataKey={"value"}/>
                  </div>
              </div> :
              <div className="col-5 d-flex justify-content-center align-items-center bg-info p-6 mx-3">
                  <h4>No data</h4>
              </div>
              }
            </div>
            </div>
            <div>
              <div>
              {/* <SecondaryTable 
              tableState={{
                Data: rowData?.cve_match,
                Loading: false,
                Error: "",
              }}
              name={"Vulnerabilities"}
              columns={[
                "cve_id",
                "published",
                "lastModified",
                "baseScore",
                "baseSeverity",
                "impactScore",
                "reference"
              ]}
            /> */}
              <table className="table table-hover">
                <thead
                    className="bg-white"
                    style={{ position: 'sticky', top: 0, zIndex: 950 }}
                  >
                  <tr className="fw-light text-muted">
                    <th className="fw-light" scope="col">CVE ID</th>
                    <th className="fw-light" scope="col">Published</th>
                    <th className="fw-light" scope="col">Last Modified</th>
                    <th className="fw-light" scope="col">Base Score</th>
                    <th className="fw-light" scope="col">Base Severity</th>
                    <th className="fw-light" scope="col">Reference</th>
                  </tr>
                  </thead>
                  <tbody className="border-top-0">
                  {rowData?.cve_match?.map((row:any, index:any)=>{
                    return <tr key={index}>
                      <td>{row.cve_id}</td>
                      <td>{row.published}</td>
                      <td>{row.lastModified}</td>
                      <td>{row.baseScore}</td>
                      <td>{row.baseSeverity}</td>
                      <td>{JSON.parse(row.reference)?.map((it:any)=>{
                        return(<><a href={it.url} target="_blank">{it.source} :- {it.url}</a><br/></>)
                      })}</td>
                    </tr>
                  })}
                  </tbody>
                </table>
            </div>
            </div>
          </Box>
        </Modal>
        <div className="d-flex row align-items-start align-content-start px-4 py-5 h-100">
          <div className="d-flex justify-content-between align-items-center mb-6 gap-3 w-100">
            <div className="d-flex justify-content-between align-items-center gap-3">
              <i className="fi fi-rr-time-forward text-success"></i>
              <h6 className="me-1 my-0">
                {!tableState?.Loading ? (
                  tableState?.Data?.length
                    ?.toString()
                    ?.replace(/\B(?=(\d{3})+(?!\d))/g, ",")
                ) : (
                  <></>
                )}{" "}
                {name}
              </h6>
              <p className="fs-7 my-0 text-muted fw-mid"></p>
              <p className="fs-7 my-0 text-muted fst-italic"></p>
            </div>
            <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
              <button className="btn btn-link fw-semi text-muted btn-sm">
                {Date()
                  .toLocaleString()
                  .substring(0, Date().toLocaleString().length - 21) + "(IST)"}
              </button>
              <div className="d-flex justify-content-start align-items-center">
                {tableState?.Data?.length > 0 ? (
                  <CsvDownloadButton
                    className="btn btn-sm btn-outline-secondary text-dark"
                    filename={fileName(name, dropdown?.props?.state)}
                    data={tableState?.Data}
                  >
                    <i className="fa fa-download me-2 align-middle"></i>
                    {"Export " + name}
                  </CsvDownloadButton>
                ) : (
                  <></>
                )}
              </div>
              {dropdown ? (
                <>
                  <div className="vr h-8px my-auto"></div>
                  <div className="d-flex justify-content-start align-items-center">
                    {dropdown}
                  </div>
                  <div className="vr h-8px my-auto"></div>
                </>
              ) : (
                <></>
              )}
              <div>
                <form
                  className="position-relative"
                  autoComplete="off"
                  onSubmit={(e: any) => {
                    e.preventDefault();
                  }}
                >
                  <span className="position-absolute top-50 align-middle start-00 ms-5 translate-middle ">
                    <i className="fi fi-rr-search fs-7 ms-2 align-middle text-muted"></i>
                  </span>
                  <input
                    type="text"
                    className="form-control bg-light ps-7 border-0"
                    name="search"
                    id="search_applicant"
                    placeholder={"Search " + name}
                    onChange={(e: any) => {
                      setSearch(e.target.value);
                    }}
                  />
                </form>
              </div>
            </div>
          </div>
          <div className="row">
            <div
              className="w-100 overflow-auto styleScroll"
              style={{ height: size }}
              id="#scrollbar-1"
            >
              {!tableState?.Loading ? (
                tableState?.Data !== undefined &&
                tableState?.Data?.length > 0 ? (
                  <AutoSizer>
                    {({ width, height }) => (
                      <Table
                        width={!Width ? width : width + height * 10}
                        height={height - 20} // Set the desired height for your table
                        headerHeight={30}
                        rowHeight={70}
                        rowCount={tableState?.Data?.length}
                        rowGetter={({ index }) => tableState?.Data[index]}
                        rowClassName="table-row" // Add a CSS class for custom row styling if needed
                        headerClassName="table-header" // Add a CSS class for custom header styling if needed
                        onRowClick={({ event, index, rowData }) =>{
                          handleRowClick(event, tableState?.Data[index])
                        }
                        }
                      >
                        {!columns
                          ? Object.keys(tableState?.Data?.[0]).map(
                              (key: string, _: number) => (
                                <Column
                                  key={key}
                                  dataKey={key}
                                  label={
                                    key.replace("_", " ")[0].toUpperCase() +
                                    key.replace("_", " ").slice(1)
                                  }
                                  width={300} // Set the desired width for each column
                                  flexGrow={1} // Allow columns to grow and fill the available width
                                  cellRenderer={({ rowData }) => {
                                    const data = rowData[key];

                                    if (
                                      key.includes("time") ||
                                      key.includes("Time") ||
                                      key.includes("duration") ||
                                      key.includes("created")
                                    ) {
                                      if (convertTime?.includes(key)) {
                                        return timeConverter(data);
                                      } else if (
                                        convertNanoTime?.includes(key)
                                      ) {
                                        return formatDuration(
                                          Math.floor(parseInt(data) / 1e9)
                                        );
                                      } else {
                                        return formatDuration(parseInt(data));
                                      }
                                    } else {
                                      return search?.length > 0 &&
                                        String(data)
                                          .toLocaleLowerCase()
                                          .includes(search) ? (
                                        <p
                                          style={{ backgroundColor: "yellow" }}
                                        >
                                          {data}
                                        </p>
                                      ) : (
                                        <> (data)</>
                                      );
                                    }
                                  }}
                                />
                              )
                            )
                          : columns.map((key: string, _: number) => (
                              <Column
                                key={key}
                                dataKey={key}
                                label={
                                  key.replace("_", " ")[0].toUpperCase() +
                                  key.replace("_", " ").slice(1)
                                }
                                width={200} // Set the desired width for each column
                                flexGrow={1} // Allow columns to grow and fill the available width
                                cellRenderer={({ rowData }) => {
                                  const data = rowData[key];
                                  if (
                                    key.includes("time") ||
                                    key.includes("Time") ||
                                    key.includes("duration")
                                  ) {
                                    if (convertTime?.includes(key)) {
                                      return timeConverter(data);
                                    } else if (key === "ntime") {
                                      return formatDuration(
                                        Math.floor(parseInt(data) / 1e9)
                                      );
                                    } else {
                                      return formatDuration(parseInt(data));
                                    }
                                  } else {
                                    return search?.length > 0 &&
                                      String(data)
                                        .toLocaleLowerCase()
                                        .includes(search) ? (
                                      <p style={{ backgroundColor: "yellow" }}>
                                        {data}
                                      </p>
                                    ) : (
                                      data
                                    );
                                  }
                                }}
                              />
                            ))}
                      </Table>
                    )}
                  </AutoSizer>
                ) : (
                  <Error message={ErrorMesg} />
                )
              ) : (
                <LoadGraph />
              )}
            </div>
          </div>
        </div>
      </>
    );
  }
);
export default ProgramsTable;
