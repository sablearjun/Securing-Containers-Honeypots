Node reenroll
/node/reenroll
Request type: POST
Parameter: JSON request containing fields (host identifier (string), re-enroll (boolean))
Response: JSON response showing success, or failure

https://localhost:1235/node/reenroll

in body,
format JSON
{
    "host_identifier" : "be6283e7-1094-5d4f-adf1-231caca7a01f",
    "reenroll_allowed" : true
}

NOTE:- reenroll allowed value NOT to be in "".
---------------------------------------------
Node Query
/node/query
Request type: GET
Parameter: host Value: host identifier string
Response: All details about given node

https://localhost:1235/node/query?host_identifier=be6283e7-1094-5d4f-adf1-231caca7a01f

----------------------------------------------
Node List
/node/list
Request type: GET
Parameters: None
Response: List of all nodes with all related information in JSON format

https://localhost:1235/node/list

--------------------------------------------------
Node Delete
/node/delete
Request type: DELETE
Parameter: host Value: host identifier string
Response: JSON response showing success, or failure

https://localhost:1235/node/delete?host_identifier=dummybe6283e7-1094-5d4f-adf1-2301f 

test query to insert dummy node

insert into nodes (config_id, host_identifier, os_arch, os_build, os_major, os_minor, os_name, os_platform, osquery_version, node_key, node_invalid,reenroll_allowed) values ('1,2,3', 'dummybe6283e7-1094-5d4f-adf1-2301f', 'x86', 'adas', '95', '55', 'dev', 'dev', '4.6.0', 'qwerty', 'f', 'f');

-------------------------------------------------------

-------------------------------------------------------
Events List
/event/list
Request type: GET
Parameters: None
Response: List of all events with all related information in JSON format

https://localhost:1235/event/list

-------------------------------------------------------
Event list 
/event/list
Request type: GET
Parameters: filter Value: JSON containing at least one field from [host_identifier, is_open, start_time, end_time]
Response: List of all events filtered using filter (where host id == host_identifier AND, isopen = is_open, AND event time >= start_time AND event time <= end_time)
boolean value can be given as true/t 

host_identifier| is_open | event_time

	https://localhost:1234/event/list
---
0000	https://localhost:1234/event/list?pageno=1&rows=100
---
0001	https://localhost:1234/event/list?end_time=1612791666&pageno=1&rows=100
---
0010	https://localhost:1234/event/list?start_time=1612789606&pageno=1&rows=100
---
0011	https://localhost:1234/event/list?start_time=1612789606&end_time=1612791666&pageno=1&rows=100
---
0100	https://localhost:1234/event/list?is_open=f&pageno=1&rows=100
---
0101	https://localhost:1234/event/list?is_open=f&end_time=1612791666&pageno=1&rows=100
---
0110	https://localhost:1234/event/list?is_open=f&start_time=1612789606&pageno=1&rows=100
---
0111	https://localhost:1234/event/list?is_open=f&start_time=1612789606&end_time=1612791666&pageno=1&rows=100
---
1000	https://localhost:1234/event/list?host_identifier=72236c40-22e4-c74b-b79f-c6a0e7ddda80&pageno=1&rows=100
---
1001	https://localhost:1234/event/list?host_identifier=72236c40-22e4-c74b-b79f-c6a0e7ddda80&end_time=1612791666&pageno=1&rows=100
---
1010	https://localhost:1234/event/list?host_identifier=72236c40-22e4-c74b-b79f-c6a0e7ddda80&start_time=1612789606&pageno=1&rows=100
---
1011	https://localhost:1234/event/list?host_identifier=72236c40-22e4-c74b-b79f-c6a0e7ddda80&start_time=1612789606&end_time=1612791666&pageno=1&rows=100
---
1100	https://localhost:1234/event/list?host_identifier=72236c40-22e4-c74b-b79f-c6a0e7ddda80&is_open=f&pageno=1&rows=100
---
1101	https://localhost:1234/event/list?host_identifier=72236c40-22e4-c74b-b79f-c6a0e7ddda80&is_open=f&end_time=1612791666&pageno=1&rows=100
---
1110	https://localhost:1234/event/list?host_identifier=72236c40-22e4-c74b-b79f-c6a0e7ddda80&is_open=f&start_time=1612789606&pageno=1&rows=100
---
1111	https://localhost:1234/event/list?host_identifier=72236c40-22e4-c74b-b79f-c6a0e7ddda80&is_open=f&start_time=1612789606&end_time=1612791666&pageno=1&rows=100
---
---------------------------------------------------------------
Event Query
/event/query
Request type: GET
Parameter: event_id Value: some numerical ID
Response: All details about given event (where event_id == id from events table)

https://localhost:1235/event/query?id=73

-------------------------------------------------------------
Event Update
/event/update
Request type: POST
Parameter: JSON request containing fields (event_id, is_open)
Response: JSON response showing success, or failure

https://localhost:1235/event/update

in body,
format JSON

{
    "event_id" : 73,
    "is_open" : true,
    "remarks" : "Remarks you want to add"
}

-------------------------------------------------------------------

------------------------------------------------------------------

Config List
/config/list
Request type: GET
Parameters: None
Response: List of all config with all related information in JSON format

https://localhost:1235/config/list

-------------------------------------------------------------------
Config Delete
/config/delete
Request type: DELETE
Parameter: id
Response: JSON response showing success, or failure

https://localhost:1235/config/delete

in body,
format JSON

{
    "id" : 4
}

test query to insert dummy config

insert into config_queries (id, query) values (4, '{\"schedule\":{\"process_events\":{\"interval\":10,\"query\":\"select * from process_events\"},\"process_file_events\":{\"interval\":10,\"query\":\"select * from process_file_events\"},\"process_list\":{\"interval\":60,\"query\":\"select * from processes\",\"snapshot\":true},\"socket_events\":{\"interval\":10,\"query\":\"select * from socket_events\"},\"user_events\":{\"interval\":10,\"query\":\"select * from user_events\"}}}');

---------------------------------------------------------------
Config Update
/config/update
Request type: POST
Parameter: JSON request containing fields (query, id)
Response: JSON response showing success, or failure

https://localhost:1235/config/update

in body,
format JSON

{
    "id":"3",
    "schedule": {
        "process_events": {
            "interval": 10,
            "query": "select * from process_events"
        },
        "process_list": {
            "interval": 60,
            "query": "select * from processes",
            "snapshot": true
        },
        "user_events": {
            "interval": 10,
            "query": "select * from user_events"
        }
    }
}
---------------------------------------------------------------
Config Add
/config/add
Request type: POST
Parameter: JSON request containing fields (query)
Response: JSON response showing success, or failure

https://localhost:1235/config/add

in body,
format JSON

{
    "schedule": {
        "process_events": {
            "interval": 10,
            "query": "select * from process_events"
        },
        "process_list": {
            "interval": 60,
            "query": "select * from processes",
            "snapshot": true
        },
        "user_events": {
            "interval": 10,
            "query": "select * from user_events"
        }
    }
}
------------------------------------------------------------------------
------------------------------------------------------------------------

JWT

1) for temporary purpose make token online in which add custom claim as token and user as below.
	example-
	
		payload
		{
		  "iss": "devesh",
		  "iat": 1619007062,
		  "exp": 1650543062,
		  "aud": "s3.ieor.iitb.ac.in",
		  "sub": "jrocket@example.com",
		  "user": "devesh10s",
		  "token": "osquery"
		}

2) enter new values in data base.
	example-
		 username  |              secret              |  token  | issuer 
		-----------+----------------------------------+---------+--------
		 devesh10s | qwertyuiopasdfghjklzxcvbnm123456 | osquery | devesh	


3) put the jwt token in authorization header (in Postman use bearer token). 
	example-
								eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJkZXZlc2giLCJpYXQiOjE2MTkwMDcwNjIsImV4cCI6MTY1MDU0MzA2MiwiYXVkIjoiczMuaWVvci5paXRiLmFjLmluIiwic3ViIjoianJvY2tldEBleGFtcGxlLmNvbSIsInVzZXIiOiJkZXZlc2gxMHMiLCJ0b2tlbiI6Im9zcXVlcnkifQ.X08PxKHGs-ArVHfQCGVtKs-GNOIXE5wW5POl5zJC8b8

