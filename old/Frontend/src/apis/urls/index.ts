import { BASE_URL } from "../base-urls";

export const NODES_LIST_URL = BASE_URL + "/node/list/"; //request type - GET
export const NODES_REENROLL_URL = BASE_URL + "/node/reenroll/"; //request type - POST
export const NODES_CONFIG_URL = BASE_URL + "/node/config/"; //request type - POST
export const NODES_QUERY_URL = BASE_URL + "/node/query/"; //request type - GET
export const NODES_MEMORY_HEALTH_URL = BASE_URL + "/node/memory_health/"; //request type - GET
export const NODES_DELETE_URL = BASE_URL + "/node/delete"; //request type - DELETE
export const EVENTS_LIST_URL = BASE_URL + "/event/list/"; //request type - GET
export const EVENTS_QUERY_URL = BASE_URL + "/event/query"; //request type - GET
export const EVENTS_UPDATE_URL = BASE_URL + "/event/update/"; //request type - POST

export const EVENTS_GRAPH_URL = BASE_URL + "/event/graph/"; //request type - POST
export const NODE_GRAPH_URL = BASE_URL + "/node/graph/"; //request type - POST
export const NODE_GRAPH_TABLE_URL = BASE_URL + "/node/graphtable/"; //request type - POST


export const CONFIG_LIST_URL = BASE_URL + "/config/list"; //request type - GET
export const CONFIG_TABLE_URL = BASE_URL + "/config/table_name"; //request type - GET
export const CONFIG_DELETE_URL = BASE_URL + "/config/delete"; //request type - DELETE
export const CONFIG_UPDATE_URL = BASE_URL + "/config/update/"; //request type - POST
export const CONFIG_ADD_URL = BASE_URL + "/config/add/"; //request type - POST
export const SCHEDULE_QUERY_URL = BASE_URL + "/scheduled_queries/schedule/"; //request type - POST
export const QUERY_STATUS_URL = BASE_URL + "/scheduled_queries/status/"; //request type - GET
export const QUERY_RESPONSE_URL = BASE_URL + "/scheduled_queries/response/"; //request type - GET
export const HISTORIC_DATA_URL = BASE_URL + "/node/historical_data/"; //request type - POST
export const LOGIN_GET_TOKEN_URL=BASE_URL + "/api/login/"//request type-POST
export const LOGOUT_DELETE_TOKEN_URL=BASE_URL+"/api/logout/"//request type-POST
export const REGISTER_GET_TOKEN_URL=BASE_URL+'/api/register/'//request type-POST
export const RENEW_TOKEN_URL=BASE_URL+'/api/renew-access-token/'//request type-POST
export const GET_USER_URL=BASE_URL+'/api/user/'//request type-GET
// export const FORWARD_PROCESS_TREE_URL=BASE_URL+'/event/event_forward_process_tree/'//request type-POST

//Admin Panel API
export const GET_GROUPS_URL=BASE_URL+'/api/team_list/'//request type-GET
export const GET_USER_LIST_URL=BASE_URL+'/api/user_list/'//request type-GET
export const ASSIGN_ALERT_USER_URL=BASE_URL+'/api/assign_event/'//request type-POST
export const CREATE_GROUP_URL=BASE_URL+'/api/make_team/'//request type-POST


//Dashboard API Methods
export const NODE_COUNT_URL=BASE_URL+'/dashboard/nodes_count/'//request type-GET
export const EVENTS_COUNT_URL=BASE_URL+'/dashboard/events_count/'//request type-GET
export const OS_COUNT_URL=BASE_URL+'/dashboard/os_count/'//request type-GET
export const SEVERITY_COUNT_URL=BASE_URL+'/dashboard/severity_count/'//request type-GET
export const ALERT_COUNT_URL=BASE_URL+'/dashboard/alert_count/'//request type-GET
export const HIGH_RISK_COUNT_URL=BASE_URL+'/dashboard/high_risk_count/'//request type-GET
export const NODE_ALERT_COUNT_URL=BASE_URL+'/dashboard/node_alert_count/'//request type-GET
export const WEEKLY_ALERTS_COUNT_URL=BASE_URL+'/dashboard/weekly_alerts_count/'//request type-GET

//Threat Hunting API Methods
export const GET_TACTIC_URL=BASE_URL+'/api/tactic/'//request type-GET
export const GET_TECHNIQUE_URL=BASE_URL+'/api/technique/'//request type-GET
export const GET_SUB_TECHNIQUE_URL=BASE_URL+'/api/sub-technique/'//request type-GET
export const GET_THREATS_URL=BASE_URL+'/api/threats/'//request type-POST
