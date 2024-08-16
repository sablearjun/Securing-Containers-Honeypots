import {
  GET_USER_URL,
  LOGIN_GET_TOKEN_URL,
  RENEW_TOKEN_URL,
  REGISTER_GET_TOKEN_URL,
  EVENTS_GRAPH_URL,
  EVENTS_QUERY_URL,
  EVENTS_UPDATE_URL,
  CONDITION_RULE_DATA,
  RULE_BUILDER_RULE_ADD,
  RULE_BUILDER_RULE_UPDATE,
} from "../urls";
import {
  SCHEDULE_QUERY_URL,
  QUERY_STATUS_URL,
  QUERY_RESPONSE_URL,
} from "../urls";

import axios from "axios";
import useInterval from "../../hooks";
import { useReducer, useState } from "react";
import { INITIAL_STATE, INITIAL_STATE_TYPE, reducerFunc } from "./reducer";
import _ from "lodash";

export const requests = {};

// Login API Method
export const doSignIn = (signinData: any) => {
  // console.log(signinData)
  return fetch(LOGIN_GET_TOKEN_URL, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(signinData),
  })
    .then((res) => {
      // console.log(res);
      return res.json();
    })
    .catch((err) => console.log(err));
};

export const validateSession = () => {
  // console.log(signinData)
  return fetch(GET_USER_URL, {
    method: "GET",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-Type": "application/json",
    },
  })
    .then((res) => {
      // console.log(res.status);
      return res.status === 401 ? false : true;
    })
    .then((res) => {
      return res;
    })
    .catch((err) => console.log(err));
};

export const renewToken = (signinData: any) => {
  return fetch(RENEW_TOKEN_URL, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(signinData),
  })
    .then((res) => {
      return res.json();
    })
    .catch((err) => console.log(err));
};

// Register API Method
export const doSignUp = (signupData: any) => {
  // console.log(signupData)
  return fetch(REGISTER_GET_TOKEN_URL, {
    method: "POST",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-Type": "application/json",
    },
    body: JSON.stringify(signupData),
  })
    .then((res) => {
      return res.json();
    })
    .catch((err) => {
      console.log(err);
    });
};

// Event Graph API Method
export const fetchEventGraph = (eventId: number) => {
  // console.log(`Event ID : ${eventId}`)
  return fetch(`${EVENTS_GRAPH_URL}?id=${eventId}`, {
    method: "GET",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-Type": "application/json",
    },
  })
    .then((res) => {
      // console.log(res);
      return res.json();
    })
    .catch((err) => console.log(err));
};

export const Rapid_fetcher = (url: string, method?: string, args?: any) => {
  if (method === "POST") {
    // let formData = new FormData();
    // Object.keys(args).forEach(key => {
    //   formData.append(String(key), String(args[key]));
    // })
    // let formData = new FormData();
    // formData.append("data", JSON.stringify(args));
    return axios
      .post(url, JSON.stringify(args), {
        headers: {
          Authorization: "Token " + localStorage.getItem("Token"),
          "Content-type": "application/json",
        },
      })
      .then((res) => {
        // console.log(res.data)
        // console.log(`Success of query response`, res.data);
        // let finalData : any =  Object.keys(res.data).map((key) => res.data[key])
        // return JSON.parse(res.data)
        return res.data;
      })
      .catch((err) => err);
  } else {
    // console.log("Ref URL: ",url);
    return fetch(`${url}`, {
      method: "GET",
      headers: {
        Authorization: "Token " + localStorage.getItem("Token"),
        "Content-Type": "application/json",
      },
    })
      .then((res) => {
        return res.json();
      })
      .catch((err) => err);
  }
};

export const getData = (
  url: string,
  setter: Function,
  method?: string,
  args?: any,
  setCount?: Function
) => {
  if (method === "POST") {
    Rapid_fetcher(url, method, args)
      .then((res) => {
        res.message ? setter(undefined) : setter(res);
        if (setCount) {
          setCount((prev: any) => prev - 1);
        }
      })
      .catch((err) => {
        setter(undefined);
        if (setCount) {
          setCount((prev: any) => prev - 1);
        }
        // console.log('Event Graph Error: ', err)
      });
  } else {
    Rapid_fetcher(url)
      .then((res) => {
        // console.log(res);
        // console.log("\n URL",url);
        res.detail ? setter(undefined) : setter(res);
        if (setCount) {
          setCount((prev: any) => prev - 1);
        }
      })
      .catch((err) => {
        setter(undefined);
        if (setCount) {
          setCount((prev: any) => prev - 1);
        }
        // console.log('Event Graph Error: ', err)
      });
  }
};

export const getDataReducer = (
  url: string,
  dispatcher: Function,
  method?: string,
  args?: any,
  setCount?: Function,
  Loading?: boolean,
  new_data_dispatcher?: Function
) => {
  if (Loading) {
    dispatcher({ type: "Loading" });
  }
  if (method === "POST") {
    Rapid_fetcher(url, method, args)
      .then((res) => {
        res.message
          ? dispatcher({ type: "Error", payload: res.message })
          : dispatcher({ type: "Fetched", payload: res });
        if (new_data_dispatcher) {
          res.message
            ? new_data_dispatcher({ type: "Error", payload: res.message })
            : new_data_dispatcher({ type: "Fetched", payload: res });
        }
        if (setCount) {
          setCount((prev: any) => prev - 1);
        }
      })
      .catch((err) => {
        dispatcher({ type: "Error", payload: err });
        if (new_data_dispatcher) {
          new_data_dispatcher({ type: "Error", payload: err });
        }
        if (setCount) {
          setCount((prev: any) => prev - 1);
        }
      });
  } else {
    Rapid_fetcher(url)
      .then((res) => {
        // console.log("REQUEST ", res);
        res.message
          ? dispatcher({ type: "Error", payload: res.message })
          : dispatcher({ type: "Fetched", payload: res });
        if (new_data_dispatcher) {
          res.message
            ? new_data_dispatcher({ type: "Error", payload: res.message })
            : new_data_dispatcher({ type: "Fetched", payload: res });
        }
        if (setCount) {
          setCount((prev: any) => prev - 1);
        }
      })
      .catch((err) => {
        dispatcher({ type: "Error", payload: err });
        if (new_data_dispatcher) {
          new_data_dispatcher({ type: "Error", payload: err });
        }
        if (setCount) {
          setCount((prev: any) => prev - 1);
        }
      });
  }
};

export const getReduceLiveData = (
  hostIdentifier: string,
  query: string,
  dispatcher: Function,
  maxReq: number
) => {
  dispatcher({ type: "Loading" });
  // let formData = new FormData();
  // formData.append("hosts", hostIdentifier);
  // formData.append("query", query);
  const dataObj = {
    "hosts": hostIdentifier,
    "query": query
  }
  if (maxReq) {
    fetch
      (SCHEDULE_QUERY_URL,{
        method:"POST",
        headers: {
          Authorization: "Token " + localStorage.getItem("Token"),
          "Content-type": "application/json",
        },
        body: JSON.stringify(dataObj),

      })
      .then((res) => res.json())
      .then((result) => {
        // console.log("Schedule res: ",result);
      
        setTimeout(() => {
            scheduleReduceQueriesStatus(
              result.data,
              hostIdentifier,
              query,
              dispatcher,
              maxReq
            );
        }, 10000);
      })
      .catch((err) => {
        console.log(err);
      });
  } else {
    dispatcher({
      type: "Error",
      payload: { status: "Failed", message: "Data not available" },
    });
  }
  maxReq--;
};

const scheduleReduceQueriesStatus = (
  configIdlist: any,
  hostIdentifier: string,
  query: string,
  dispatcher: Function,
  maxReq: number
) => {
  dispatcher({ type: "Loading" });
  let configVal: any = Object.keys(configIdlist).map(
    (key) => configIdlist[key]
  );
  // let formData = new FormData();
  // formData.append("config_id_list", configVal);
  const dataObj = {
    "config_id_list" : configVal
  }
  if (maxReq) {
    // console.log(maxReq);
    fetch
      (QUERY_STATUS_URL, {
        method: "POST",
        headers: {
          Authorization: "Token " + localStorage.getItem("Token"),
          "Content-type": "application/json",
        },
        body: JSON.stringify(dataObj),
      })
      .then((res) => res.json())
      .then((result)=>{
        // console.log("Status Res: ", result);
        const id = Object.keys(result)[0];
        if(result[id].sent){
          // console.log("Sent: ",id, true);
          let successIds: string;
          successIds = Object.keys(result).join(',');
          // console.log("Success Ids: ", successIds);

          setTimeout(() => {
            scheduleReduceQueriesResponse(
              successIds,
              hostIdentifier,
              query,
              dispatcher,
              maxReq
            );
          }, 2000);
        }
      })
      .catch((err) => {
        getReduceLiveData(hostIdentifier, query, dispatcher, maxReq);
        console.log(err);
      });
  } else {
    dispatcher({
      type: "Error",
      payload: { status: "Failed", message: "Data not available" },
    });
  }
  maxReq--;
};

const scheduleReduceQueriesResponse = (
  configIdlist: any,
  hostIdentifier: string,
  query: string,
  dispatcher: Function,
  maxReq: number
) => {
  dispatcher({ type: "Loading" });
  // let formData = new FormData();
  const dataObj = {
    "config_id_list": configIdlist
  }
  // formData.append("config_id_list", successIds);
  if (maxReq) {
    fetch
      (QUERY_RESPONSE_URL, {
        method: "POST",
        headers: {
          Authorization: "Token " + localStorage.getItem("Token"),
          "Content-type": "application/json",
        },
        body: JSON.stringify(dataObj),
      })
      .then((res) => res.json())
      .then((result:any) => {
        // console.log("Response Res: ", result);
        let finalData: any ;
        if(configIdlist.split(',').length > 1){
          // console.log("response: ", result['combined']);
          // finalData = JSON.parse(result['combined']);
          // finalData = Object.keys(result).map((key) => JSON.parse(result[key]));
          finalData = result;
          dispatcher({ type: "Fetched", payload: finalData });
        }else{
          finalData = Object.keys(result).map((key) => result[key]);
          dispatcher({ type: "Fetched", payload: JSON.parse(finalData[0]) });
        }
        // console.log("Final Data: ",finalData)
      })
      .catch((err) => {
        dispatcher({ type: "Error", payload: err });
        getReduceLiveData(hostIdentifier, query, dispatcher, maxReq);
      });
  } else {
    dispatcher({
      type: "Error",
      payload: { status: "Failed", message: "Data not available" },
    });
  }
  maxReq--;
};

export const getLiveData = (
  hostIdentifier: string,
  query: string,
  setter: Function,
  loading?: Function
) => {
  // console.log('Selected Node ', selectedNodeIdentifier)
  // console.log(SCHEDULE_QUERY_URL)
  // let hostsIdentifier : any = nodes.map((item:any) => item.host_identifier)
  const dataObj = {
    "hosts": hostIdentifier,
    "query": query
  }
  fetch
  (SCHEDULE_QUERY_URL,{
    method:"POST",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-type": "application/json",
    },
    body: JSON.stringify(dataObj),

  })
  .then((res) => res.json())
  .then((result) => {
      setTimeout(() => {
        scheduleQueriesStatus(
          result.data,
          hostIdentifier,
          query,
          setter,
          loading
        );
    }, 10000);
  })
  .catch((err) => {
    console.log(err);
  });
};

const scheduleQueriesStatus = (
  configIdlist: any,
  hostIdentifier: string,
  query: string,
  setter: Function,
  loading?: Function
) => {
  let configVal: any = Object.keys(configIdlist).map(
    (key) => configIdlist[key]
  );
  const dataObj = {
    "config_id_list" : configVal
  }
  fetch
  (QUERY_STATUS_URL, {
    method: "POST",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-type": "application/json",
    },
    body: JSON.stringify(dataObj),
  })
  .then((res) => res.json())
  .then((result)=>{
    const id = Object.keys(result)[0];
    if(result[id].sent){
      // console.log("Sent: ",id, true);
      let successIds: string;
      successIds = Object.keys(result).join(',');
      // console.log("Success Ids: ", successIds);

      setTimeout(() => {
        scheduleQueriesResponse(
          successIds,
          hostIdentifier,
          query,
          setter,
          loading
        );
      }, 2000);
    }
  })
    .catch((err) => {
      getLiveData(hostIdentifier, query, setter);
      console.log(err);
    });
};

const scheduleQueriesResponse = (
  configIdlist: any,
  hostIdentifier: string,
  query: string,
  setter: Function,
  loading?: Function
) => {
  const dataObj = {
    "config_id_list": configIdlist
  }
  fetch
  (QUERY_RESPONSE_URL, {
    method: "POST",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-type": "application/json",
    },
    body: JSON.stringify(dataObj),
  })
  .then((res) => res.json())
  .then((result) => {
    // console.log("Response Res: ", result);
    let finalData: any = Object.keys(result).map((key) => result[key]);
    setter(JSON.parse(finalData[0]));
     if (loading !== undefined) {
        loading(false);
      }
    })
    .catch((err) => {
      setter(undefined);
      getLiveData(hostIdentifier, query, setter);
      console.log(err);
    });
};
// Event Metadata API Method
export const fetchEventMetaData = (eventId: number) => {
  // console.log(`Event ID : ${eventId}`)
  return fetch(`${EVENTS_QUERY_URL}?id=${eventId}`, {
    method: "GET",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-Type": "application/json",
    },
  })
    .then((res) => {
      // console.log(res);
      return res.json();
    })
    .catch((err) => console.log(err));
};

export const updateMe = (payload: any, url: string, setter?: Function) => {
  // console.log("Payload:",payload);
  return fetch(url, {
    method: "POST",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-Type": "application/json",
    },
    body: JSON.stringify(payload),
  })
    .then((res) => res.json())
    .then((res) =>{
      if (setter !== undefined) {
        // console.log("Res message: ", res.message)
        setter(res);
      }
      // console.log(`Success of update`, res);
    })
    .catch((err) => {
      console.log(err);
    });
};

export const deleteMe = (payload: any, url: string, setter?: Function) => {
  // let formData = new FormData();
  const headers = {
      Authorization: "Token " + localStorage.getItem("Token"),
    }

  // console.log(data);
  // formData.append("data", JSON.stringify(payload));
  axios
    .delete(url, { headers : headers, data: payload })
    .then((res) => {
      if (setter !== undefined) {
        setter(res.data);
      }
      console.log(`Success of Delete`, res.data);
      return res.data;
    })
    .catch((err) => {
      console.log(err);
    });
};
// Event Update API Method
export const updateEvent = (eventDetails: any) => {
  console.log(eventDetails);
  return fetch(EVENTS_UPDATE_URL, {
    method: "POST",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-Type": "application/json",
    },
    body: JSON.stringify(eventDetails),
  })
    .then((res) => {
      // console.log(res);
      return res.json();
    })
    .catch((err) => console.log(err));
};

// rule builder page related api's
export const conditionTableData = () => {
  return axios
    .get(CONDITION_RULE_DATA, {
      headers: {
        Authorization: "Token " + localStorage.getItem("Token"),
        "Content-type": "multipart/form-data",
      },
    })
    .then((res) => {
      return res.data;
    })
    .catch((err) => console.log(err));
};

export const Polling = (
  URL: string,
  Setter: Function,
  Interval: number,
  Limit: number
) => {
  const [semaphore, setSemaphore] = useState<number>(0);
  useInterval(() => {
    if (semaphore < Limit) {
      setSemaphore((prev) => prev + 1);
      getData(URL, Setter, "GET", {}, setSemaphore);
    }
  }, Interval);
};

interface State {
  state: INITIAL_STATE_TYPE;
  setter: Function;
}

export const PollingReducer = (
  URL: string,
  state: State,
  Interval: number,
  Limit: number,
  Method?: "GET" | "POST",
  Args?: any,
  notification?: boolean,
  setNotification?: Function,
  notificationFactor?: string
) => {
  const [semaphore, setSemaphore] = useState<number>(0);
  const [currentData, setCurrentData] = useReducer(reducerFunc, INITIAL_STATE);
  useInterval(() => {
    // console.log(state.state)
    if (semaphore < Limit) {
      if (currentData.Data === undefined) {
        setSemaphore((prev) => prev + 1);
        getDataReducer(
          URL,
          state.setter,
          Method,
          Args,
          setSemaphore,
          false,
          setCurrentData
        );
      } else {
        // if(_.differenceWith(currentData.Data, state.state.Data, _.isEqual).length>0)
        if (
          _.differenceWith(currentData.Data, state.state.Data, _.isEqual)
            .length > 0
        ) {
          if (notification && setNotification && notificationFactor) {
            if (
              currentData.Data[0][notificationFactor] !==
              state.state.Data[0][notificationFactor]
            ) {
              setNotification(
                (prev: number) =>
                  prev +
                  _.differenceWith(
                    currentData.Data,
                    state.state.Data,
                    _.isEqual
                  ).length
              );
            }
          }
          var prevData: Array<Object> = state.state.Data;
          prevData = prevData.concat(
            _.differenceWith(currentData.Data, state.state.Data, _.isEqual)
          );
          state.setter({ type: "Fetched", payload: prevData });
        }
        setSemaphore((prev) => prev + 1);
        getDataReducer(URL, setCurrentData, Method, Args, setSemaphore, false);
      }
    }
  }, Interval);
};

export const sendRuleBuilderData = (args: any, URL: string) => {
  return fetch(URL, {
    method: "POST",
    headers: {
      Authorization: "Token " + localStorage.getItem("Token"),
      "Content-Type": "application/json",
    },
    body: JSON.stringify(args),
  })
    .then((res) => {
      return res.json();
    })
    .catch((err) => console.log(err));
};
