import React, { memo, useState } from 'react'
import CsvDownloadButton from 'react-json-to-csv'
import { fileName, timeConverter, formatDuration } from '../../helpers'
import {INITIAL_STATE_TYPE} from '../../apis/requests/reducer'
import LoadGraph from '../loadGraph'
import { Error } from '../Error'
import { AutoSizer, Table, Column } from 'react-virtualized';

interface tableParams{
    tableState:INITIAL_STATE_TYPE,
    name:string,
    columns?:Array<string>
    convertTime?:Array<string>
    convertNanoTime?:Array<string>
    dropdown?:any
    Height?:number
    Width?:number
    ErrorMesg?:string
}


const SecondaryTable:React.FC<tableParams> = memo(({tableState,name,columns,convertTime,convertNanoTime,dropdown,Height,Width, ErrorMesg}:tableParams) => {
    let size="calc(100vh - "+(Height?Height.toString()+"rem)":"30rem)")
    ErrorMesg = ErrorMesg === undefined ?'Data Not Available': ErrorMesg;
    const [search, setSearch] = useState<string>("")
  return (
    <>
        <div className="d-flex row align-items-start align-content-start px-4 py-5 h-100">
            <div className="d-flex justify-content-between align-items-center mb-6 gap-3 w-100">
                <div className="d-flex justify-content-between align-items-center gap-3">
                    <i className="fi fi-rr-time-forward text-success"></i>
                    <h6 className="me-1 my-0">{!tableState?.Loading?tableState?.Data?.length?.toString()?.replace(/\B(?=(\d{3})+(?!\d))/g, ","):(<></>)} {name}</h6>
                    <p className="fs-7 my-0 text-muted fw-mid">
                    </p>
                    <p className="fs-7 my-0 text-muted fst-italic">
                    </p>
                </div>
                <div className="d-flex justify-content-end align-items-center gap-3 ms-auto">
                    <button className="btn btn-link fw-semi text-muted btn-sm">
                        {Date().toLocaleString().substring(0,(Date().toLocaleString().length-21))+"(IST)"}
                    </button>
                    <div className="d-flex justify-content-start align-items-center">
                    {
                        tableState?.Data?.length>0?
                        <CsvDownloadButton className="btn btn-sm btn-outline-secondary text-dark" filename={fileName(name,dropdown?.props?.state)} data={tableState?.Data} >
                            <i className="fa fa-download me-2 align-middle"></i>{"Export "+name}
                        </CsvDownloadButton>:<></>
                    }
                    </div>
                    {
                        dropdown?<>
                        <div className="vr h-8px my-auto"></div>
                        <div className="d-flex justify-content-start align-items-center">
                            {dropdown}
                        </div>
                        <div className="vr h-8px my-auto"></div></>:<></>
                    }
                    <div>
                        <form className="position-relative" autoComplete="off" onSubmit={(e:any) =>{ e.preventDefault()}}>
                            <span className="position-absolute top-50 align-middle start-00 ms-5 translate-middle ">
                                <i className="fi fi-rr-search fs-7 ms-2 align-middle text-muted"></i>
                            </span>
                            <input
                            type="text"
                            className="form-control bg-light ps-7 border-0"
                            name="search"
                            id="search_applicant"
                            placeholder={"Search "+name}
                            onChange={(e:any)=>{
                                setSearch(e.target.value)
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
                        {
                            !(tableState?.Loading)?tableState?.Data!==undefined&&tableState?.Data?.length>0?
                            (<AutoSizer>
                                {({ width,height }) => (
                                  <Table
                                  
                                     width={!Width? width: width+height*15}
                                    height={height-10} // Set the desired height for your table
                                    headerHeight={50}
                                    rowHeight={120}
                                    className={'text-center'}
                                    rowCount={tableState?.Data?.length}
                                    rowGetter={({ index }) => tableState?.Data[index]}
                                    rowClassName="table-row" // Add a CSS class for custom row styling if needed
                                    //headerClassName=" text-center ScrollName" //center-header Add a CSS class for custom header styling if needed
                                  >
                                    {!columns
                                      ? Object.keys(tableState?.Data?.[0]).map((key: string, _: number) => (
                                          <Column
                                            key={key}
                                            dataKey={key}
                                            label={key.replace('_', ' ')[0].toUpperCase() + key.replace('_', ' ').slice(1)}
                                            width={500} // Set the desired width for each column
                                            
                                            flexGrow={1} // Allow columns to grow and fill the available width
                                            className='mx-3 overflow-auto styleScroll'
                                            cellRenderer={({ rowData }) => {
                                              const data = rowData[key];
                                              if (key.includes('time') || key.includes('Time')||key.includes('duration')||key.includes('created')) {
                                                if (convertTime?.includes(key)) {
                                                  return timeConverter(data);
                                                }else if(convertNanoTime?.includes(key)){
                                                  return  formatDuration(Math.floor(parseInt(data)/1e9));
                                                }else {
                                                  return formatDuration(parseInt(data));
                                                }
                                              } else {
                                                return search?.length > 0 && String(data).toLocaleLowerCase().includes(search) ? (
                                                  <p style={{ backgroundColor: 'yellow' }}>{data}</p>
                                                ) : <span className='p-1' >{data}</span>;
                                              }
                                            }}
                                          />
                                        ))
                                      : columns.map((key: string, _: number) => (
                                          <Column
                                            key={key}
                                            dataKey={key}
                                            label={key.replace('_', ' ')[0].toUpperCase() + key.replace('_', ' ').slice(1)}
                                            width={500}// Set the desired width for each column
                                            flexGrow={1}
                                            className='mx-3 overflow-auto styleScroll'
                                             // Allow columns to grow and fill the available width
                                            cellRenderer={({ rowData }) => {
                                              const data = rowData[key];
                                              if (key.includes('time') || key.includes('Time') ||key.includes('duration')) {
                                                if (convertTime?.includes(key)) {
                                                  return timeConverter(data);
                                                } else if(key==='ntime'){
                                                  return  formatDuration(Math.floor(parseInt(data)/1e9));
                                                }else {
                                                  return formatDuration(parseInt(data));
                                                }
                                              } else {
                                                return search?.length > 0 && String(data).toLocaleLowerCase().includes(search) ? (
                                                  <p style={{ backgroundColor: 'yellow' }}>{data}</p>
                                                ) : <span className='p-1 ' >{data}</span>;
                                              }
                                            }}
                                          />
                                        ))}
                                  </Table>
                                )}
                              </AutoSizer>):(<Error message={ErrorMesg}/>):(<LoadGraph/>)
                        }
                    </div>
            </div>
        </div>
    </>
  )
})
export default SecondaryTable;