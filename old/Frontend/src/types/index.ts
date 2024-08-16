export interface toggle_state{
  event_id:string;
  remarks:string;
  state:string;
}
export interface User_Info{
  username:string;
  email:string;
  first_name:string;
  last_name:string;
}
export interface Event {
  address:string;
  event_code: string;
  host_identifier: string;
  os_name: string;
  os_platform: string;
  osquery_version: string;
  id: string;
  is_alert: string;
  is_open: string;
  severity: string;
  metadata: string;
  context_data:string
  unixtime: string;
  hostname: string;
  technique:string;
  technique_id:string;
  link:string;
}

export interface bpf_process_events{
action:string;
added_capabilities:string;
cmdline:string;
container_id:string;
container_image:string;
container_name:string;
dropped_capabilities:string;
from_column:string;
host_identifier:string;
hostname:string;
id:string;
path:string;
pid:string;
port_binding:string;
privileged:string;
scope:string;
status:string;
time:string;
type:string;
}

export interface Node {
  enrolled_on: string;
  hardware_cpu_logical_core: string;
  hardware_cpu_type: string;
  hardware_model: string;
  hardware_physical_memory: string;
  hardware_vendor: string;
  hardware_version: string;
  host_identifier: string;
  hostname: string;
  status: string;
  last_seen:number;
  node_invalid: string;
  node_key: string;
  os_arch: string;
  os_build: string;
  os_major: string;
  os_minor: string;
  os_name: string;
  os_platform: string;
  osquery_version: string;
  reenroll_allowed: string;
}

export interface newTeam{
  name:string,
  member_ids:Array<number>,
  permissions:Array<number>
}

export interface HistoricData {
  action: string;
  description: string;
  directory: string;
  gid: string;
  gid_signed: string;
  host_identifier: string;
  id: string;
  is_hidden: string;
  shell: string;
  type: string;
  uid: string;
  uid_signed: string;
  username: string;
  uuid: string;
}

export interface deleteConf{
  config_id:number,
  config_name:string
}

export interface graphFilerts{
  file_event:boolean,
  socket_event:boolean
}