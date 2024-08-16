# name="spawn_shell"
# rule='''
# def chrome_shell(output):
#     if (True):
#         ss = 'spawn_shell triggered'
#         print(ss)
#         return True, ss
#     ss = 'spawn_shell not triggred'
#     return False, ss''' 

rule='''def win_chrome_shell(output):
        process_event = output['row_data']
        if (process_event['permissions'] == 'all'):
            ss = "[" + str(process_event['path']) + "]"
            print(ss)
            return True, ss
        ss = ''
        return False, ss''' ,

# ---------> Using API <------------
import requests
import json
from requests.structures import CaseInsensitiveDict
data={
        "name": "win_chrome_shell",
        "rule": rule ,
        "severity": 3,
        "technique": "Initial Access",
        "technique_id": "",
        "link": "No Link"
}   

# data = {
#         "name": "ash_spawn_shell",
#         "rule": "def ash_spawn_shell(output):\n    process_event = output['row_data']\n    process_tree = output['process_tree']\n    if (Condition):\n        ss = \"[\" + str(process_event['path']) + \" (pid: \" + str(process_event['pid']) + \")]\"\n        print(ss)\n        return True, ss\n    ss = ''\n    return False, ss",
#         "severity": 3,
#         "technique": "Initial Access",
#         "technique_id": "",
#         "link": "No Link",
#         "comment": "test comment",
#         "os_name": "kali"
#     }

# inspector = CaseInsensitiveDict()
# inspector["Accept"] = "application/json"
# inspector["Authorization"] = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOiIxOTUyODczNzEyIiwiaWF0IjoiMTYzNzM0MjE2NCIsImlzcyI6Im1hbmFnZXIiLCJ0b2tlbiI6InF3ZXJ0eSIsInVzZXIiOiJrbmk5aHQifQ.myVa8hBifhC-LgE4rb-WfuEFFSVzAfFt7XGQ1xdkr1g"

# hawkurl = "https://localhost:1235/config/rule_add"
# queryinit = json.dumps(data)
# page = requests.post(hawkurl, headers=inspector, data=queryinit, verify=False)

# ---------------> Directly using Database <------------
import psycopg2
    
conn = psycopg2.connect(
        host="localhost",
        port = '5432',
        database="fleet_",
        user="vajra",
        password="admin")
# Inserting into the table
conn.autocommit = True
cursor = conn.cursor()
cursor.execute("INSERT INTO python_rules(id, name, rule, severity, technique, technique_id, link, comment, os_name) VALUES (%s,%s,%s,%s,%s,%s,%s,%s,%s)",(2,data['name'],data['rule'],4, 'technique', 'T1011', 'http://dgyauwdgbawdg', 'test comment', 'Windows'))

conn.commit()
print("Records inserted........")

# Closing the connection
conn.close()

