import { useEffect} from "react";
import { HISTORIC_DATA_URL} from "../apis/urls";
import { getDefaultHeaders, useLazyFetch } from "../hooks";
import QueryEditor from "../modules/threat-hunting/query-editor";
import UserDetails from "../modules/threat-hunting/user-details";
import { HistoricData } from "../types";

const ThreatHunting = (props:any) => {
  const { loading, error, data, run } = useLazyFetch<HistoricData[]>();
  useEffect(() => {
  }, []);

  return (
    <div className="card">
      <div className="card-header bg-white border-0">
        {
          props.runQuery?(""):(<><UserDetails runQuery={props.runQuery}/><hr className="w-100 my-4" /></>)
        }
        <QueryEditor
          status={props.online}
          hostIdentifier={props.host_identifier}
          runQuery={props.runQuery}
          setNotification={props.setNotification}
          run={({ query, queryType }) => {
            let obj = {
              URL: HISTORIC_DATA_URL,
              init: {
                method: "POST",
                body: query,
                headers: getDefaultHeaders({
                  "Content-Type": "application/json",
                }),
              },
            };
            if (queryType === "historic-data") {
              run(obj);
            }
          }}
        />

        <hr className="w-100 my-4" />

        {loading ? <p>loading...</p> : null}
        {error ? <p>{error.message}</p> : null}
        {/* {
          queryResult.Data?
          <SecondaryTable name="Historical Data" tableState={queryResult}/>:<></>
        } */}
        {/* {finalDataToShow.length > 0 ? (
          <DataTable runQuery={props.runQuery} data={finalDataToShow} />
        ) : null} */}
      </div>
    </div>
  );
};

ThreatHunting.defaultProps = {
  runQuery:false
}
export default ThreatHunting;
