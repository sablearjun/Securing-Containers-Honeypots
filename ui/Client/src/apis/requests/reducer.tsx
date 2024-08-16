export interface INITIAL_STATE_TYPE{
    Loading:boolean,
    Data:any|undefined,
    Error:any|undefined
}

export interface Action{
    type:"Loading"|"Fetched"|"Error"|"Polling",
    payload:any
}

export const INITIAL_STATE:INITIAL_STATE_TYPE={
    Loading:false,
    Data:undefined,
    Error:undefined,
}

export const reducerFunc=(state:INITIAL_STATE_TYPE,action:Action)=>{
    switch (action.type) {
        case "Loading":
            return {
                ...state,
                Loading:true,
                Data:undefined
            }
        case "Fetched":
            return {
                ...state,
                Loading:false,
                Data:action.payload,
                Error:undefined
            }
        case "Error":
            return {
                ...state,
                Loading:false,
                Error:action.payload,
                Data:undefined
            }
        default:
            return state
    }
}