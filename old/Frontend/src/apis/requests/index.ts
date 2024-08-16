import { GET_USER_URL,LOGIN_GET_TOKEN_URL,RENEW_TOKEN_URL, REGISTER_GET_TOKEN_URL, EVENTS_GRAPH_URL, EVENTS_QUERY_URL, EVENTS_UPDATE_URL } from "../urls";
import { SCHEDULE_QUERY_URL, QUERY_STATUS_URL, QUERY_RESPONSE_URL } from "../urls";

import axios, { Axios } from "axios";
import { stringify } from "querystring";


export const requests = {};

// Login API Method
export const doSignIn = (signinData : any) => {
    // console.log(signinData)
    return fetch(LOGIN_GET_TOKEN_URL, {
        method: "POST",
        headers: {
            'Content-Type':'application/json'
        },
        body:JSON.stringify(signinData)
    })
    .then(res => {
        // console.log(res);
        return res.json();
    })
    .catch(err => console.log(err))
}

export const validateSession = () => {
  // console.log(signinData)
  return fetch(GET_USER_URL, {
      method: "GET",
      headers: {
          Authorization: "Token " + localStorage.getItem("Token"),
          'Content-Type':'application/json'
      }
  })
  .then(res => {
      // console.log(res.status);
      return res.status === 401 ? false : true
  }).then(res => {
      return res
  }).catch(err => console.log(err))
}

export const renewToken = (signinData : any) => {
  return fetch(RENEW_TOKEN_URL, {
      method: "POST",
      headers: {
          'Content-Type':'application/json'
      },
      body:JSON.stringify(signinData)
  })
  .then(res => {
      return res.json();
  })
  .catch(err => console.log(err))
}

export const getGeoIp=(ip:string)=>{
  // ip = '182.57.16.34';
  const options = {
    method: 'GET',
    headers: {
      contentType: 'application/json',
      dataType: 'json'
    }
  };
  fetch('https://ipwho.is/' + ip, options)
	.then(response => response.json())
	.then(response => console.log(response))
	.catch(err => console.error(err));
}

// Register API Method
export const doSignUp = (signupData : any) => {
    // console.log(signupData)
    return fetch(REGISTER_GET_TOKEN_URL, {
        method: "POST",
        headers: {
            'Content-Type':'application/json'
        },
        body:JSON.stringify(signupData)
    })
    .then(res => {
        // console.log(res);
        return res.json();
    })
    .catch(err => console.log(err))
}

// Event Graph API Method
export const fetchEventGraph = (eventId: number) => {
    // console.log(`Event ID : ${eventId}`)
    return fetch(`${EVENTS_GRAPH_URL}?id=${eventId}`, {
        method: "GET",
        headers: {
            Authorization: "Token " + localStorage.getItem("Token"),
            'Content-Type':'application/json'
        }
    })
    .then(res => {
        // console.log(res);
        return res.json();
    })
    .catch(err => console.log(err))
}

export const Rapid_fetcher = (url:string,method?:string,args?:any) => {
  if(method==="POST")
  {
    // let formData = new FormData();
    // Object.keys(args).forEach(key => {
    //   formData.append(String(key), String(args[key]));
    // })
    let formData = new FormData();
    formData.append('data', JSON.stringify(args));
    return axios.post(
      url,
      formData,
      {
        headers: {
            "Authorization": "Token " + localStorage.getItem("Token"),
            "Content-type": "multipart/form-data",
        },                    
      }
      )
      .then(res => {
        // console.log(res.data)
        // console.log(`Success of query response`, res.data); 
          // let finalData : any =  Object.keys(res.data).map((key) => res.data[key])
          // return JSON.parse(res.data)
          return res.data;
      })
      .catch(err => console.log(err))
  }
  else
  {
    return fetch(`${url}`, {
          method: "GET",
          headers: {
              Authorization: "Token " + localStorage.getItem("Token"),
              'Content-Type':'application/json'
          }
      })
      .then(res => {
          return res.json();
      })
      .catch(err => console.log(err))
  }
}

export const getData = (url:string,setter:Function,method?:string,args?:any) => {
  if(method==="POST")
  {
    Rapid_fetcher(url,method,args)
      .then((res) => {
        res.message ? setter(undefined) : setter(res)
      })
      .catch((err) => {
        setter(undefined)
        // console.log('Event Graph Error: ', err)
      })
  }
  else
  {
    Rapid_fetcher(url)
      .then((res) => {
        res.detail ? setter(undefined) : setter(res)
      })
      .catch((err) => {
        setter(undefined)
        // console.log('Event Graph Error: ', err)
      })
  }
  }

export const getLiveData = (hostIdentifier:string, query:string, setter:Function,loading?:Function) => {
    // console.log('Selected Node ', selectedNodeIdentifier)
    // console.log(SCHEDULE_QUERY_URL)
    // let hostsIdentifier : any = nodes.map((item:any) => item.host_identifier)
    let formData = new FormData();
    formData.append('hosts', hostIdentifier);  
    formData.append('query', query);
    axios.post(
      SCHEDULE_QUERY_URL,
      formData,
      {
        headers: {
            "Authorization": "Token " + localStorage.getItem("Token"),
            "Content-type": "multipart/form-data",
        },                    
      }
    )
    .then(res => {
      // console.log(`Success of query schedule`, res.data);
      setTimeout(() => {
        scheduleQueriesStatus(res.data, hostIdentifier, query, setter,loading)
      }, 3000)
    })
    .catch(err => {
        // getLiveData(hostIdentifier, query, setter);
      console.log(err);
    })
  }

  const scheduleQueriesStatus = (configIdlist:any, hostIdentifier:string, query:string, setter:Function,loading?:Function) => {
    let configVal : any =  Object.keys(configIdlist).map((key) => configIdlist[key])
    let formData = new FormData();
    formData.append('config_id_list', configVal);
    axios.post(
      QUERY_STATUS_URL,
      formData,
      {
        headers: {
          "Authorization": "Token " + localStorage.getItem("Token"),
          "Content-type": "multipart/form-data",
        },                    
      }
    )
    .then(res => {
      // console.log(`Success of query status`, res.data);
      setTimeout(() => {
        scheduleQueriesResponse(res.data, hostIdentifier, query, setter,loading)
      }, 3000);
    })
    .catch(err => {
        getLiveData(hostIdentifier, query, setter);
        console.log(err);
    })
  }

  const scheduleQueriesResponse = (configIdlist:any, hostIdentifier:string, query:string, setter:Function,loading?:Function) => {
    let successIds : any = []
    for (var key in configIdlist) {
      if (configIdlist.hasOwnProperty(key)) {
        // if(configIdlist[key].sent) successIds.push(key)
        successIds.push(key)
      }
    }
    // console.log('cofig id for response api : ', successIds)
    let formData = new FormData();
    formData.append('config_id_list', successIds);
    axios.post(
      QUERY_RESPONSE_URL,
      formData,
      {
        headers: {
          "Authorization": "Token " + localStorage.getItem("Token"),
          "Content-type": "multipart/form-data",
        },                    
      }
    )
    .then(res => {
      // console.log(`Success of query response`, res.data);
      let finalData : any =  Object.keys(res.data).map((key) => res.data[key])
      setter(JSON.parse(finalData[0]))
      // try{
      // }
      // catch{
      //   setter(undefined)
      // }
      if(loading!==undefined)
      {
        loading(false)
      }
    })
    .catch(err => {
      setter(undefined)
      getLiveData(hostIdentifier, query, setter);
      console.log(err);
    })
  }
// Event Metadata API Method
export const fetchEventMetaData = (eventId: number) => {
    // console.log(`Event ID : ${eventId}`)
    return fetch(`${EVENTS_QUERY_URL}?id=${eventId}`, {
        method: "GET",
        headers: {
            Authorization: "Token " + localStorage.getItem("Token"),
            'Content-Type':'application/json'
        }
    })
    .then(res => {
        // console.log(res);
        return res.json();
    })
    .catch(err => console.log(err))
}

export const updateMe = (payload:any, url:string, setter?:Function) => {
    let formData = new FormData();
    formData.append('data', JSON.stringify(payload));
    axios.post(
      url,
      formData,
      {
        headers: {
          "Authorization": "Token " + localStorage.getItem("Token"),
          "Content-type": "multipart/form-data",
        },                    
      }
    )
    .then(res => {
      if(setter!==undefined)
      {
        setter(res.data)
      }
      console.log(`Success of update`, res.data);
    })
    .catch(err => {
        console.log(err);
    })
}

export const deleteMe = (payload:any, url:string, setter?:Function) => {
  let formData = new FormData();
  const headers={
    "Authorization": "Token " + localStorage.getItem("Token")
  }
  formData.append('data', JSON.stringify(payload));
  axios.delete(
    url,
    {headers, data:formData}
  )
  .then(res => {
    if(setter!==undefined)
    {
      setter(res.data)
    }
    console.log(`Success of Delete`, res.data);
  })
  .catch(err => {
      console.log(err);
  })
}
// Event Update API Method
export const updateEvent = (eventDetails : any) => {
    console.log(eventDetails)
    return fetch(EVENTS_UPDATE_URL, {
        method: "POST",
        headers: {
            Authorization: "Token " + localStorage.getItem("Token"),
            'Content-Type':'application/json'
        },
        body:JSON.stringify(eventDetails)
    })
    .then(res => {
        // console.log(res);
        return res.json();
    })
    .catch(err => console.log(err))
}