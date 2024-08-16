export interface toggle_state{
  event_id :  string;
  remarks  :  string;
  state    :  string;
}
export interface User_Info{
  username  : string;
  email     : string;
  first_name: string;
  last_name : string;
}
export interface PythonRule{
  rule_written_by : string | null;
  name            : string;
  rule            : string;
  severity        : number;
  technique       : string;
  technique_id    : string;
  link            : string;
  comment         : string;
  os_name         : string;
  mode?           : 'add'|'edit';
}
export interface Event {
  address         : string;
  event_code      : string;
  host_identifier : string;
  id              : string;
  is_alert        : string;
  is_open         : boolean;
  severity        : string;
  metadata        : string;
  context_data    : string
  unixtime        : string;
  hostname        : string;
  technique       : string;
  technique_id    : string;
  link            : string;
  rule_written_by : string;
  created         : number;
  last_modified   : number;
  action          : string;
  os_name         : string;
  alert_type      : string;
  event_id        : string;
  assigned_users  : any;
}

export interface EventExtraData{
  action:string
  address:string,
  alert_type:string,
  assign_to:string,
  context_data:string,
  event_code:string,
  host_identifier:string
  hostname:string,
  id:string,
  is_alert:string,
  is_open:string,
  link:string,
  metadata:string,
  os_name:string,
  os_platform:string,
  osquery_version:string,
  pid:string
  severity:string,
  technique:string,
  technique_id:string,
  unixtime:string
}

export interface Node {
  id                        : number;
  enrolled_on               : string;
  hardware_cpu_logical_core : string;
  hardware_cpu_type         : string;
  hardware_model            : string;
  hardware_physical_memory  : string;
  hardware_vendor           : string;
  hardware_version          : string;
  host_identifier           : string;
  hostname                  : string;
  status                    : string;
  last_seen                 : number;
  node_invalid              : string;
  node_key                  : string;
  os_arch                   : string;
  os_build                  : string;
  os_major                  : string;
  os_minor                  : string;
  os_name                   : string;
  os_platform               : string;
  osquery_version           : string;
  reenroll_allowed          : string;
}

export interface newTeam{
  name        : string,
  member_ids  : Array<number>,
  permissions : Array<number>
}

export interface HistoricData {
  action          : string;
  description     : string;
  directory       : string;
  gid             : string;
  gid_signed      : string;
  host_identifier : string;
  id              : string;
  is_hidden       : string;
  shell           : string;
  type            : string;
  uid             : string;
  uid_signed      : string;
  username        : string;
  uuid            : string;
}

export interface deleteConf{
  config_id   : number,
  config_name : string
}

export interface graphFilerts{
  file_event    : boolean,
  socket_event  : boolean
}