import { BASE_URL } from "../base-urls";

//Node Page API
export const NODES_LIST_URL = BASE_URL + "/node/list/";                          //request type - GET
export const NODES_REENROLL_URL = BASE_URL + "/node/reenroll/";                  //request type - POST
export const NODES_CONFIG_URL = BASE_URL + "/node/config/";                      //request type - POST
export const NODES_QUERY_URL = BASE_URL + "/node/query";                        //request type - GET
export const NODES_MEMORY_HEALTH_URL = BASE_URL + "/node/memory_health/";        //request type - GET
export const NODES_DELETE_URL = BASE_URL + "/node/delete";                       //request type - DELETE
export const HISTORIC_DATA_URL = BASE_URL + "/node/historical_data/";            //request type - POST
export const DEMO_DATA_URL = BASE_URL + "/node/demo/";                           //request type - POST
export const NODE_PROGRAMS_LIST = BASE_URL + "/node/programs_list";              //request type - GET
export const VUL_PROGRAMS_LIST = BASE_URL + "/node/vul_programs_list";           //request type - GET

//Alert Page API
export const EVENTS_LIST_URL = BASE_URL + "/event/list/";                        //request type - GET
export const EVENTS_QUERY_URL = BASE_URL + "/event/query";                       //request type - GET
export const REFERENCE_LOG_URL = BASE_URL + "/node/log_list";              //request type - GET
export const EVENTS_UPDATE_URL = BASE_URL + "/event/update/";                    //request type - POST
export const EVENTS_GRAPH_URL = BASE_URL + "/event/ept/";                      //request type - POST
export const EVENTS_ASSIGNEE_URL = BASE_URL + "/event/assignee";

//Config Page API
export const CONFIG_LIST_URL = BASE_URL + "/config/list";                        //request type - GET
export const CONFIG_TABLE_URL = BASE_URL + "/config/table_name";                 //request type - GET
export const CONFIG_DELETE_URL = BASE_URL + "/config/delete";                    //request type - DELETE
export const CONFIG_UPDATE_URL = BASE_URL + "/config/update/";                   //request type - POST
export const CONFIG_ADD_URL = BASE_URL + "/config/add/";                         //request type - POST

//Live Query API
export const SCHEDULE_QUERY_URL = BASE_URL + "/scheduled_queries/schedule/";     //request type - POST
export const QUERY_STATUS_URL = BASE_URL + "/scheduled_queries/status/";         //request type - GET
export const QUERY_RESPONSE_URL = BASE_URL + "/scheduled_queries/response/";     //request type - GET

//User Management API
export const LOGIN_GET_TOKEN_URL=BASE_URL + "/auth/login/"                        //request type-POST
export const LOGOUT_DELETE_TOKEN_URL=BASE_URL+"/auth/logout/"                     //request type-POST
export const REGISTER_GET_TOKEN_URL=BASE_URL+'/auth/register/'                    //request type-POST
export const RENEW_TOKEN_URL=BASE_URL+'/auth/renew-access-token/'                 //request type-POST
export const GET_USER_URL=BASE_URL+'/api/user_details/'                           //request type-GET
export const DELETE_USER_URL=BASE_URL+'/api/user_delete'                          //request type-DELETE  
export const UPDATE_USER_URL =BASE_URL+'/api/user_update'                           // request type POST

//Admin Panel API
export const GET_GROUPS_URL=BASE_URL+'/api/team_list/'                           //request type-GET
export const GET_USER_LIST_URL=BASE_URL+'/api/user_list/'                        //request type-GET
export const ASSIGN_ALERT_USER_URL=BASE_URL+'/api/assign_event/'                 //request type-POST
export const CREATE_GROUP_URL=BASE_URL+'/api/make_team/'                         //request type-POST
export const UPDATE_GROUP_MEMBER=BASE_URL+'/api/add_member/';                    //request type-POST
export const DELETE_GROUP=BASE_URL+'/api/team_deletion/';                        //request type-DELETE

//Dashboard API Methods
export const NODE_COUNT_URL=BASE_URL+'/dashboard/nodes_count/'                   //request type-GET
export const EVENTS_COUNT_URL=BASE_URL+'/dashboard/events_count/'                //request type-GET
export const OS_COUNT_URL=BASE_URL+'/dashboard/os_count/'                        //request type-GET
export const SEVERITY_COUNT_URL=BASE_URL+'/dashboard/severity_count/'            //request type-GET
export const ALERT_COUNT_URL=BASE_URL+'/dashboard/alert_count/'                  //request type-GET
export const HIGH_RISK_COUNT_URL=BASE_URL+'/dashboard/high_risk_count/'          //request type-GET
export const NODE_ALERT_COUNT_URL=BASE_URL+'/dashboard/node_alert_count/'        //request type-GET
export const WEEKLY_ALERTS_COUNT_URL=BASE_URL+'/dashboard/weekly_alerts_count/'  //request type-GET
export const MITRE_ANALYSIS_URL=BASE_URL+'/dashboard/mitre_analysis/'            //request type-GET

//Threat Hunting API Methods
export const GET_TACTIC_URL=BASE_URL+'/api/tactic/'                              //request type-GET
export const GET_TECHNIQUE_URL=BASE_URL+'/api/technique/'                        //request type-GET
export const GET_SUB_TECHNIQUE_URL=BASE_URL+'/api/sub-technique/'                //request type-GET
export const GET_THREATS_URL=BASE_URL+'/api/threats/'                            //request type-POST

//Python Rule Engine
export const SUBMIT_PYTHON_RULE_URL=BASE_URL+'/python_rules/add_python_rule/'   //request type-POST
export const UPDATE_PYTHON_RULE_URL=BASE_URL+'/python_rules/update_python_rule/'
export const LIST_PYTHON_RULE_URL=BASE_URL+'/python_rules/list_python_rule/'    //request type-GET
export const DELETE_PYTHON_RULE_URL=BASE_URL+'/python_rules/delete_python_rule/'//request type-GET

// rule builder
export const CONDITION_RULE_DATA = BASE_URL+'/python_rules/table_list/';        //request type-GET
export const RULE_BUILDER_RULE_LIST = BASE_URL+'/rule_builder/rule_list/';      //request type-GET
export const RULE_BUILDER_RULE_ADD = BASE_URL+'/rule_builder/rule_add/';        //request type-POST
export const RULE_BUILDER_RULE_UPDATE = BASE_URL+'/rule_builder/rule_update/';  //request type-POST
export const RULE_BUILDER_RULE_DELETE = BASE_URL+'/rule_builder/rule_delete/';  //request type-DELETE

// Ip
export const IP_FILE_UPLOAD = BASE_URL+'/ip/file_upload';                       //request type-POST
export const IP_FILE_LIST = BASE_URL+'/ip/list';                              //request type-GET

// Yara
export const YARA_FILE_UPLOAD = BASE_URL+'/yara/file_upload';                       //request type-POST
export const YARA_FILE_LIST = BASE_URL+'/yara/list';                                //request type-GET


export const LIST_BACKEND_RULE_URL=BASE_URL+'/backend_rules/list'               //request type-GET

// Settings
export const MANUAL_DATA_PURGE = BASE_URL+'/settings/manual_purge';             //request type-DELETE
export const DATA_RETENTION_DAYS = BASE_URL+'/settings/data_retention_days/'    //request type-GET
export const DATA_RETENTION_DAYS_UPDATE = BASE_URL+'/settings/data_retention_days/update' //request type-GET

// Blogs
export const GET_BLOGS_URL = BASE_URL+'/blogs/list';                       //request type - GET
export const BLOG_DELETE_URL = BASE_URL + "/blogs/delete/";                     //request type - DELETE
export const BLOG_UPDATE_URL = BASE_URL + "/blogs/update/";                     //request type - POST
export const BLOG_ADD_URL = BASE_URL + "/blogs/add/";                           //request type - POST

export const AGENT_INSTALLER_URL = process.env.REACT_APP_AGENT_INSTALLER_URL;

