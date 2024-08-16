export const pythonSampleRule = `# This is the Sample Rule 
#   Don't forget to :
#       1. Wrap all this in Definition/Function
#       2. Return :
#           a. Boolean   (Which determines weather to trigger the rule or not)
#           b. String    (Which display some context about the rule triggered)

def apt_testing(output):
    process_event = output['row_data']
    process_tree = output['process_tree']
    if ((process_event['path'] in ["/usr/bin/dash","/usr/bin/zsh", '/usr/lib/postgresql/14/bin/postgres']) | (process_event['path'][-2:] == 'sh')):
        for e in process_tree:
            if e['path'] == "/usr/lib/postgresql/14/bin/postgres":
                ss = "[" + str(e['path']) + " (pid: " + str(e['pid']) +")] spawned [" + str(process_event['path']) + " (pid: " + str(process_event['pid']) + ")]"
                print(ss)
                return True, ss
    ss = ''
    return False, ss`;

export const Tactic = [
  'Initial Access',
  'Execution',
  'Persistence',
  'Privilege Escalation',
  'Defense Evasion',
  'Credential Access',
  'Discovery',
  'Lateral Movement',
  'Collection',
  'Command and Control',
  'Exfiltration',
  'Impact',
];

export const severity = [
  // {
  //   value: 0,
  //   label: 'None',
  // },
  {
    value: 25,
    label: 'Information',
  },
  {
    value: 50,
    label: 'Low',
  },
  {
    value: 75,
    label: 'Medium',
  },
  {
    value: 100,
    label: 'High',
  },
];

export const Alerters = [
  // { value: 'rsyslog', label: 'rsyslog' },
  { value: 'email', label: 'email' },
];

export const Tactics = [
  { value: 'Initial Access', label: 'Initial Access' },
  { value: 'Execution', label: 'Execution' },
  { value: 'Persistence', label: 'Persistence' },
  { value: 'Privilege Escalation', label: 'Privilege Escalation' },
  { value: 'Defense Evasion', label: 'Defense Evasion' },
  { value: 'Credential Access', label: 'Credential Access' },
  { value: 'Discovery', label: 'Discovery' },
  { value: 'Lateral Movement', label: 'Lateral Movement' },
  { value: 'Collection', label: 'Collection' },
  { value: 'Command and Control', label: 'Command and Control' },
  { value: 'Exfiltration', label: 'Exfiltration' },
  { value: 'Impact', label: 'Impact' },
];

export const queryClause = [
  { value: "equal", label: "equal" },
  { value: "not_equal", label: "not equal" },
  { value: "begins_with", label: "begins with" },
  { value: "not_begins_with", label: "doesn't begin with" },
  { value: "contains", label: "contains" },
  { value: "not_contains", label: "doesn't contain" },
  { value: "ends_with", label: "ends with" },
  { value: "not_ends_within", label: "doesn't end with" },
  { value: "is_empty", label: "is empty" }
]

export const Platforms = [
  { value: 'all', label: 'All' },
  { value: 'Darwin', label: 'Darwin' },
  { value: 'Linux', label: 'Linux' },
  { value: 'freebsd', label: 'FreeBSD' },
  { value: 'posix', label: 'POSIX Compatible' },
  { value: 'Windows', label: 'Windows' },
]