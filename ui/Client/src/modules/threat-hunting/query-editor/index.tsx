import { useEffect, useReducer, useRef, useState } from "react";
import { HISTORIC_DATA_URL, NODES_LIST_URL} from "../../../apis/urls";
import { INITIAL_STATE, INITIAL_STATE_TYPE, reducerFunc } from "../../../apis/requests/reducer";
import { getDataReducer, getReduceLiveData } from "../../../apis/requests";
import SecondaryTable from "../../../components/SecondaryTable";
import _ from 'lodash';
import { useInterval } from "usehooks-ts";
import CircleIcon from '@mui/icons-material/Circle';
import Editor from '@monaco-editor/react';
import {toast, ToastContainer} from 'react-toastify';
import Autocomplete from "@mui/material/Autocomplete";
import NodeSelectDropdown from "../../../components/react-multiselect/nodeSelectDropdown";
import FormControl from "@mui/material/FormControl";
import InputLabel from "@mui/material/InputLabel";
import Select, { SelectChangeEvent } from "@mui/material/Select";
import MenuItem from "@mui/material/MenuItem";

const HISTORIC_DATA_LIMIT : number = 50000;
const LIVE_DATA_ROW_LIMIT : number = 2000;

interface QueryEditorProps {
  run: (params: {
    queryType: "historic-data" | "realtime-api";
    query: string;
  }) => void;
  runQuery:boolean
  hostIdentifier:string
  status:string
  setNotification:Function
}

export const INITIAL_STATE_RAW_DATA:INITIAL_STATE_TYPE={
  Loading:true,
  Data:undefined,
  Error:undefined,
}

const QueryEditor = ({ run,runQuery,hostIdentifier,status,setNotification }: QueryEditorProps) => {
  const [selectedNodeIdentifier, setSelectedNodeIdentifier] = useState<any>({nodes:"no-option"});
  const [query, setQuery] = useState("SELECT * FROM bpf_process_events limit 100");
  const [queryType, setQueryType] = useState<"historic-data" | "realtime-api">("historic-data");
  const [queryResult, setQueryResult]=useReducer(reducerFunc,INITIAL_STATE)
  const [rawData, setRawData]=useReducer(reducerFunc,INITIAL_STATE_RAW_DATA)
  const [node, setNode] = useReducer(reducerFunc,INITIAL_STATE)
  const [first, setFirst] = useState<boolean>(false)
  const [semaphore, setSemaphore] = useState<number>(0)
  const [searchNode, setSearchNode] = useState<string>('combined');
  const [filteredResults, setFilteredResults]=useReducer(reducerFunc,INITIAL_STATE)
  const [editorTheme, setEditorTheme] = useState<"vs-light"|"vs-dark">("vs-light")
  const editorRef = useRef<any>(null)
  const [logType, setLogType] = useState<"bpf_file_events"|"bpf_process_events"|"bpf_socket_events"|"win_process_events"|"win_file_events"|"win_socket_events">("bpf_process_events");
  const [isButtonDisabled, setButtonDisabled]=useState<boolean>(false);
  const [selectedNodeName, setSelectedNodeName] = useState<string>("");
  let buttonTimeoutInterval: number = 15000;

  // useInterval(()=>{
  //   if(semaphore<2)
  //   {
  //     getDataReducer(HISTORIC_DATA_URL,setRawData,"POST",{"query":`select * from ${logType} order by id desc limit 100`},setSemaphore, false)
  //     setSemaphore(prev=>prev+1)
  //   }
  // },5000)

  useEffect(()=>{
    // if(semaphore<2)
    {
      getDataReducer(HISTORIC_DATA_URL,setRawData,"POST",{"query":`select * from ${logType} order by id desc limit 100`})
      // setSemaphore(prev=>prev+1)
    }
  },[])
  // const differentialRows = _.differenceWith(currentRows, pastRows, _.isEqual);
  function handleEditorDidMount(editor:any, monaco:any) {
    editorRef.current = editor;
  }

  const Dropdown=(params:any)=>{
    return( 
        <>
          <p className="my-0 form-label text-muted fw-light me-1" style={{width:"3vw"}}>
              Log Type:
          </p>
          <select className="form-select text-start btn btn-link fw-semi text-muted btn-sm" style={{width:"9vw"}} value={params.state} onChange={(e:any)=>{
            params.setter(e.target.value as string);
            setRawData({type:"Fetched","payload":undefined})
            setRawData({type:"Loading","payload":undefined})
          }}>
          {/* <option selected="">All</option> */}
              <option value="bpf_process_events">Linux - Process</option>
              <option value="bpf_file_events">Linux - File</option>
              <option value="bpf_socket_events">Linux - Socket</option>
              <option value="win_process_events">Windows - Process</option>
              <option value="win_file_events">Windows - File</option>
              <option value="win_socket_events">Windows - Socket</option>
          </select>
        </>
      )
  }

  const getFirstOptionText = () => {
    if (node.Loading) return "fetching nodes...";
    else if (node.Error) return node.Error;
    else return "Select Node";
  };

  const updateLimitInQuery = (query: string, rowLimit: number): string => {
    const limitRegex = /LIMIT\s+(\d+)/i; // Regular expression to match LIMIT clause

    // Check if the LIMIT clause is already present in the query
    const limitMatch = query.match(limitRegex);
    // console.log(limitMatch);
    const limitString = 'LIMIT '+String(rowLimit);

    if (limitMatch) {
      const currentLimit = parseInt(limitMatch[1], 10); // Extract the current limit
      if (currentLimit < rowLimit) {
        // If current limit is less than rowlimit, keep it
        return query;
      } else {
        // If current limit is greater than or equal to rowlimit, change it to rowlimit
        return query.replace(limitRegex, limitString);
      }
    } else {
      // If LIMIT is not present, add a LIMIT of rowlimit to the query
      return query + ' '+ limitString;
    }
  }

  const isSelectQuery =(query: string): boolean => {
    // Regular expression to match SELECT queries
    const selectRegex = /^SELECT\b/i;
    // Use the regular expression to check if the query starts with SELECT
    return selectRegex.test(query);
  }

  const onRun = () => {
    if(query === ''){
      toast.error('Please add your query...');
      return;
    }
    if(!isSelectQuery(query)){
      toast.error('Only Select queries permitted');
      return;
    }
    setFirst(true)
    setButtonDisabled(true);

    if (queryType === "historic-data") {
      const updatedQuery = updateLimitInQuery(query, HISTORIC_DATA_LIMIT); // Max 50000 rows can be fetched at once
      getDataReducer(HISTORIC_DATA_URL,setQueryResult,"POST",{"query":updatedQuery},()=>{},true)
    }
    if(queryType === "realtime-api"){
      if(selectedNodeIdentifier.nodes === '' || selectedNodeIdentifier.nodes ==='no-option'){
        toast.error('Please select a node to run a live query');
        setButtonDisabled(false);
        return;
      }

      if(query === ''){
        setButtonDisabled(false);
        toast.error('Please add your query...');
        return;
      }else{
        const updatedQuery = updateLimitInQuery(query, LIVE_DATA_ROW_LIMIT); // Max 2000 rows can be fetched at once
        getReduceLiveData(selectedNodeIdentifier.nodes,updatedQuery,setQueryResult,3)
      }
    }
    buttonTimeoutInterval = (queryType==='realtime-api')? 15000: 2000;

    setTimeout(()=>{
      setButtonDisabled(false);
    },buttonTimeoutInterval)
  };

  const registerCompletionProvider = (tableData: { name: string; columns: { name: string }[] }[]) => {
    const monaco = (window as any).monaco;
    
    // Register the completion provider for SQL language
    monaco.languages.registerCompletionItemProvider('sql', {
      provideCompletionItems: (model: any, position: any) => {
        const wordUntilPosition = model.getWordUntilPosition(position);
        const range = {
          startLineNumber: position.lineNumber,
          endLineNumber: position.lineNumber,
          startColumn: wordUntilPosition.startColumn,
          endColumn: wordUntilPosition.endColumn,
        };

        const textUntilPosition = model.getValueInRange(range);

        if (textUntilPosition.length > 0) {
          // Check if the current text is a table name
          const tableName = textUntilPosition.toUpperCase();
          const table = tableData.find((t) => t.name === tableName);

          if (table) {
            // If the text is a table name, suggest column names
            const columnItems = table.columns.map((column) => ({
              label: column.name,
              kind: monaco.languages.CompletionItemKind.Field,
              insertText: column.name,
            }));

            return {
              suggestions: columnItems,
            };
          }
        }
        return {
          suggestions: [],
        };
      },
    });
  };

  const searchResults = (search: string) => {
    if(search===undefined){
      return queryResult?.Data;
    }
    const nodesList = selectedNodeIdentifier?.nodes?.split(',');
    const searchIndex = nodesList.indexOf(search.trim());

    if(nodesList.length == 1){
      return queryResult?.Data;
    }
    if(search === 'combined'){
      if(queryResult?.Data?.['combined'] !== undefined)
      return JSON.parse(queryResult?.Data?.['combined']);
    }
    if (searchIndex !== -1) {

      return JSON.parse(queryResult?.Data?.[search]);
    }

    return queryResult?.Data; // or handle the case when the search word is not found
  }

  const handleSearchChange = (event: SelectChangeEvent) => {
    // console.log("Event : ", event.target.value);
    setSearchNode(event.target.value);
    if(event.target.value === 'combined'){
      setSelectedNodeName('Combined');
    }else{
      setSelectedNodeName(node?.Data?.nodes?.find((i:any) => i.host_identifier === event.target.value).hostname);
    }
  };

  useEffect(() => {
    getDataReducer(NODES_LIST_URL+"?is_active=true",setNode)
    const monaco = editorRef?.current;
  }, [])

  return (
    <div>
      <ToastContainer/>
      <div className="d-flex align-items-start gap-3">
        <p className={queryType==="realtime-api"?"liveButton":"liveButtonDisables"} style={{fontWeight:500,display:'flex', alignItems:'center', gap:'0.2rem'}}>{queryType==="realtime-api"?<CircleIcon style={{fontSize:"1rem"}}/>:<></>} LIVE</p>
        <div className="form-check form-switch">
          {// runQuery is check for running in TH page else in NodeSheetView Run QueryPage
            runQuery?
            (<>
            <input
              className="form-check-input"
              type="checkbox"
              role="switch"
              id="flexSwitchCheckChecked"
              checked={queryType === "realtime-api"}
              onChange={(e) => {
                if (e.target.checked) {
                  if(status==="Online")
                  {
                    setQueryType("realtime-api");
                    setSelectedNodeIdentifier({...selectedNodeIdentifier, nodes: hostIdentifier});
                  }
                } else {

                  setQueryType("historic-data");
                }
              }}
            />
            </>
            ):(
              <input
              className="form-check-input"
              type="checkbox"
              role="switch"
              id="flexSwitchCheckChecked"
              checked={queryType === "realtime-api"}
              onChange={(e) => {
                if (e.target.checked) {
                  setQueryType("realtime-api");
                } else {
                  setQueryType("historic-data");
                }
              }}
            />
            )
          }
          <label
            className="form-check-label text-muted fst-italic fw-normal fs-7"
            htmlFor="flexSwitchCheckChecked"
          >
            {
              queryType==="realtime-api"?("Fetching Data from Osquery realtime"):("Fetching Data from Server database")
            }
          </label>
        </div>
        <div className="d-flex align-items-center ms-auto gap-3">
          <button
            className="btn btn-link text-muted btn-sm"
            onClick={() => setQuery("")}
          >
            Clear
          </button>
          <div className="vr h-8px my-auto"></div>
          <button
          disabled={isButtonDisabled || rawData.Loading || queryResult.Loading || (runQuery && (selectedNodeIdentifier.nodes === ""||selectedNodeIdentifier.nodes==="no-option"))}
            onClick={()=>{
              onRun()
            }}
            className="btn btn-run btn-sm fs-7 text-white align-middle w-100px"
          >
            Run
          </button>
        </div>
      </div>
      <div className="d-flex flex-column align-items-start gap-3">
        {
          runQuery?(""):
        (
          <>
        {node.Data === undefined ? <><span className="d-flex">No Node Data </span></> : <>
          {queryType === "historic-data" ? <>
          <NodeSelectDropdown data={node?.Data?.nodes} nodeIdentifier={selectedNodeIdentifier} setNodeIdentifier={setSelectedNodeIdentifier} isDisabled={true}/>
          </>:<>
          <NodeSelectDropdown data={node?.Data?.nodes} nodeIdentifier={selectedNodeIdentifier} setNodeIdentifier={setSelectedNodeIdentifier} isDisabled={false}/>
          </>}

        </>}
        </>
        )
        }
        <div className="form-floating w-100">
          <div className="form-floating w-100" style={{border:"1px solid  rgba(0, 0, 0, 0.23)", padding:"0.5rem", borderRadius: "5px"}}>
            <Editor
              height="10vh" 
              width="100%"
              language="sql" 
              theme={editorTheme}
              onChange={(value) => {value!==undefined?setQuery(value):setQuery("")}}
              value={query}
              onMount={handleEditorDidMount}
            />
          </div>
        </div>
        {
          first?
          <div className="col w-100">
            <div className="col w-100">
            <div>
              { selectedNodeIdentifier?.nodes?.split(',').length>1?
            <FormControl sx={{ m: 1, minWidth: 500 }}>
              <InputLabel id="demo-simple-select-autowidth-label" className="p-1 bg-white">Filter via Nodes</InputLabel>
              <Select
                labelId="demo-simple-select-autowidth-label"
                id="demo-simple-select-autowidth"
                value={searchNode}
                onChange={handleSearchChange}
                autoWidth
                // label="node"
              >
                <MenuItem value="combined">
                  <em>Combined</em>
                </MenuItem>
                {selectedNodeIdentifier?.nodes?.split(',')?.map((hostId:any) =>{
                  return <MenuItem value={hostId}>{node?.Data?.nodes?.find((i:any) => i.host_identifier === hostId).hostname}</MenuItem>
                })}
              </Select>
            </FormControl>:<></>
            }
          </div>
            </div>
            <div className="col w-100">
              <SecondaryTable name={selectedNodeIdentifier?.nodes?.split(',').length>1? `${selectedNodeName} Results` : 'Query Results'} tableState={{Data: searchResults(searchNode), Loading: queryResult.Loading, Error: queryResult.Error}} Height={35} Width={1000} />
            </div>
          </div>
          :
          !runQuery?
          <div className="row w-100">
          <div className="col w-100">
            <SecondaryTable name="Raw Data"  tableState={rawData} Height={35} Width={1000}  dropdown={<Dropdown style={{width:"20vw"}} setter={setLogType} state={logType}/>} convertTime={['unixtime', 'time']} convertNanoTime={['ntime']}/>
          </div>
          </div>
          :<></>
        }
      </div>
    </div>
  );
};

export default QueryEditor;