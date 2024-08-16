import requests
import json

# URL to send the GET request to
url = "http://localhost:4000/event/ept"

# JSON body to send with the GET request
# Linux

#  7326495 Infinite loop event_id linux
#  62623  Alert ID
request_body = {
    "id":7326495,
    "host_identifier":"29adb7f9-12fa-cccb-41e0-04421a2d9324",
    "socket_event" : False,
    "file_event" : False,
    "os_name":"Linux",
    "alert_type":"File Alert"
}
# request_body = {
#     "id":1027,
#     "host_identifier":"B0D2A257-A0BE-11EB-80EF-38F3AB8C1B10",
#     "socket_event" : False,
#     "file_event" : False,
#     "os_name":"Windows",
#     "alert_type":"Process Alert"
# }

# Send the GET request with the JSON body
response = requests.post(url, json=request_body)

# Check if the request was successful (status code 200)
if response.status_code == 200:
    # Print the response content (assuming it's JSON)
    response_json = response.json()
    print("Response:")
    print(json.dumps(response_json, indent=4))
else:
    res = response.json()
    # print(f"Request failed with status code: {res.status_code}")
    print(res)
    print("Error", res.message)
