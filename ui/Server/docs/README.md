## API Documentation

Welcome to the API documentation for the Vajra Tool's backend, which is written in Node.js. This documentation provides an overview of the various modules, endpoints, and example queries available within the backend system.

## Modules and Endpoints

The backend system is organized into 10 modules, each containing specific endpoints to handle different functionalities. Below is a summary of the modules and their corresponding endpoints:

- `/node` : Node APIs
- `/event` : Event APIs
- `/auth` : User Authentication APIs
- `/api` : User Management APIs
- `/backend_rules` : Backend Rule APIs
- `/blogs` : Blog APIs
- `/config` : Config APIs
- `/dashboard` : Dashboard APIs
- `/ip`: IP APIs
- `/yara` : Yara APIs
- `/threat_hunting` : Threat Hunting APIs
- `/python_rules` : Python Rule APIs
- `/scheduled_queries` : Scheduled Query APIs
- `/rule_builder` : Rule Builder APIs
- `/settings` : Settings APIs

Base Url: `http://localhost:4000/api/`

##### Note:
>`Login` and `Register` are the only open endpoints i.e requires no Authentication.
> All the other endpoints in Auth as well as other modules require authentication and hence are closed endpoints.
>
>Closed endpoints require a valid Token to be included in the header of the request. A Token can be acquired from the Login endpoint API.
>
>The header to be provided with each request is:
>
> | Header | value     |
> | ---- | -------- |
> | Authorization | Token \{\{token\}\} |
### Auth Module

---

Endpoints for user authentication and authorization.

<!-- Login -->
<details>
 <summary>Login :
 <code>
  POST <b>/auth/login</b></code></summary>

##### Description
This endpoint serves the purpose of allowing registered users to obtain an access token, which is a secure authorization token required to access the tool's various functionalities and resources.

Upon successful registration and account creation, users need to provide their credentials through this endpoint to obtain a valid access token. This access token will act as a form of digital identification, granting users access to specific parts of the Vajra Tool based on their role and permissions.

##### Request Body params

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | username | required | string |
> | password | required | string |

##### Request Body Example

```json
{
    "username":"abcd",
    "password":"1234"
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ "message" : "Token successfully created", "data": {...}}`|
> | `400`     | `application/json`         | `{"code":"400","message":"Token can't be created"}` |
> | `500`     | `application/json`   | `{ "message": "Database Error" }`|

##### Response Example

```json
{
    "message": "Token successfully created.",
    "data": {
        "user_data": {
            "id": 1,
            "username": "abcd",
            "email": "abcd@gmail.com",
            "role": "ADMINISTRATOR"
        },
        "token": "[valid token]",
        "renew_access_token": "[valid renew access token]"
    }
}
```
##### Example cURL

> ```javascript
>curl -X POST -H "Content-Type: application/json" --data '{"username":"admin","password":"admin"}' http://localhost:4000/api/auth/login
> ```

</details>

<!-- Register -->
<details>
 <summary>Register User :
 <code>POST <b>/auth/register</b></code></summary>

##### Description
This endpoint facilitates the process of registering a new user within the system, allowing them to create an account and gain access to the tool's functionalities.

When new users want to start using the Vajra Tool, they need to create an account through this endpoint. Registering a user involves providing essential information such as a username, password, email, name, and optionally specifying their role within the system.

##### Request Body params

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | username | required | string |
> | password | required | string |
> | passwordConfirm | required | string |
> | email | required | string |
> | name | required | string |
> | role | optional | string ['ADMINISTRATOR' \| 'STANDARD'] |

##### Req body Example

```json
{
    "username":"lmn",
    "password":"qwerty",
    "passwordConfirm": "qwerty",
    "email":"lmn@gmail.com",
    "name":"LMN",
    "role":"ADMINISTRATOR"
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message": "User successfully created."}`|
> | `400`     | `application/json`         | `{"errorMessage":"Email 'lmn@gmail.com' already exists"}` |
> | `500`     | `application/json`   | `{ "message": "Database Error" }`|

##### Response Example

```json
{
    "message": "Token successfully created.",
    "data": {
        "user_data": {
            "id": 1,
            "username": "abcd",
            "email": "abcd@gmail.com",
            "role": "ADMINISTRATOR"
        },
        "token": "[valid token]",
        "renew_access_token": "[valid renew access token]"
    }
}
```

##### Example cURL

> ```javascript
>curl -X POST -H "Content-Type: application/json" --data '{"username":"admin","password":"admin","passwordConfirm": "admin","email":"admin@gmail.com","name":"Admin","role":"ADMINISTRATOR"}' http://localhost:4000/api/auth/register
> ```

</details>

<!-- Renew Access Token -->
<details>
 <summary> Renew Access Token :
 <code>POST <b>/auth/renew-access-token</b></code></summary>

##### Description
This endpoint allows registered users to renew their access tokens, ensuring continued secure access to the website's functionalities.

Access tokens issued by the Vajra website have a finite duration of validity for security reasons. After a certain period, an access token expires and requires renewal to maintain access. The POST /auth/renew-access-token endpoint offers a way for users to extend the validity of their access tokens without the need for re-authentication.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/auth/renew-access-token
> ```

</details>

<!-- Logout -->
<details>
 <summary> Logout :
 <code>POST <b>/auth/logout</b></code></summary>

##### Description
This endpoint provides registered users with a secure and controlled way to log out of the system, effectively terminating their active session.

When users want to end their session and ensure that their access to the Vajra UI's resources is properly terminated, they can use the POST /auth/logout endpoint. This action is essential for security reasons as it prevents unauthorized access to the user's account and data after they have logged out

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | -|

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/auth/logout
> ```

</details>

### Users Module

---

Endpoints to manage user accounts and roles.

<!-- Users list -->
<details>
 <summary> User List :
 <code>GET <b>/api/user_list</b></code></summary>

##### Description
This endpoint allows authorized users to fetch a comprehensive list of all users who are registered within the Vajra Tool's ecosystem. This list includes details about each user, which can be useful for administrative purposes, understanding user demographics, and monitoring user activity.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ "message" : "List of users", "data": [{..}, {...}]}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Example
```json
{
    "message": "List of users.",
    "data": [
        {
            "id": 1,
            "email": "yogeshjadhav@gmail.com",
            "password": "$2a$08$1nL9HeQsn57L0W5vQ1eh7O4Q/F2ATgntUrkKrZPxjxvdLjtRBZJEe",
            "username": "kni9ht",
            "name": "Yogesh J",
            "role": "ADMINISTRATOR",
            "language": "en-US",
            "created_at": "2023-07-24T09:45:42.806Z",
            "updated_at": "2023-07-24T09:45:42.806Z"
        },
        {
            "id": 2,
            "email": "gaurishewale@gmail.com",
            "password": "$2a$08$G31wyyO7M9EmDBY3MA.qkez6/pLynD0Y5oQagkHf4gEZEJAzehDXq",
            "username": "gaurishewale",
            "name": "Gauri S",
            "role": "ADMINISTRATOR",
            "language": "en-US",
            "created_at": "2023-07-24T09:46:13.453Z",
            "updated_at": "2023-07-24T09:46:13.453Z"
        }
    ]
}
```

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/user_list
> ```
</details>
<!--  Team List -->
<details>
 <summary>Team List :
 <code>GET <b>/api/team_list</b></code></summary>


##### Description
This endpoint serves as a means for authorized users to fetch an inclusive list of all teams that have been registered within the Vajra Tool's ecosystem. The information contained in this list offers insights into the various teams, their members, and their respective roles, which is invaluable for effective team management and collaboration.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | -| - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message": "List of teams", "data": [{..}, {...}]}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Example
```json
{
    "message": "List of teams",
    "data": [
        {
            "id": 1,
            "name": "Team 1",
            "admin_ids": "1,2",
            "member_ids": "2,3,4",
            "permissions": ""
        },
        {
            "id": 2,
            "name": "Team 2",
            "admin_ids": "1,2",
            "member_ids": "2,4",
            "permissions": ""
        }
    ]
}
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/api/team_list
> ```
</details>

<!-- Team Member List -->
<details>
 <summary>Team Members List :
 <code>GET <b>/api/team_member_list</b></code></summary>

##### Description
This endpoint serves as a dedicated way for authorized users to retrieve an all-inclusive list of members belonging to a particular team within the Vajra Tool's ecosystem. The information provided by this list aids in understanding the composition of the team, their roles, and responsibilities, promoting effective team collaboration.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | team_id | required | integer |

##### Request Body example

```json
{
    "team_id" : 1
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message": "List of members", "data": [{..}]}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Example
```json
{
    "message": "List of members",
    "data": [
        [
            {
                "id": 2,
                "email": "gaurishewale@gmail.com",
                "password": "$2a$08$G31wyyO7M9EmDBY3MA.qkez6/pLynD0Y5oQagkHf4gEZEJAzehDXq",
                "username": "gaurishewale",
                "name": "Gauri S",
                "role": "ADMINISTRATOR",
                "language": "en-US",
                "created_at": "2023-07-24T09:46:13.453Z",
                "updated_at": "2023-07-24T09:46:13.453Z"
            }
        ],
        [
            {
                "id": 3,
                "email": "azeez@gmail.com",
                "password": "$2a$08$HjXyBE9z7pAT2woWo06y5OsSNQOvrK8uqa7zO7M7MS2bt7NF43LE2",
                "username": "azeez7",
                "name": "Azeez Dandawala",
                "role": "ADMINISTRATOR",
                "language": "en-US",
                "created_at": "2023-07-25T09:40:02.162Z",
                "updated_at": "2023-07-25T09:40:02.162Z"
            }
        ],
        [
            {
                "id": 4,
                "email": "devesh@gmail.com",
                "password": "$2a$08$AhaZBtiOHqzIt5puqqBzwOYyvpCo5YKC5sp7oHo9WxaJf5sm0jcj2",
                "username": "devesh",
                "name": "Devesh Sawant",
                "role": "STANDARD",
                "language": "en-US",
                "created_at": "2023-07-25T09:40:48.015Z",
                "updated_at": "2023-07-25T09:40:48.015Z"
            }
        ]
    ]
}
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/api/team_member_list
> ```

</details>

<!-- Team admin list -->
<details>
 <summary>Team Admins List
 <code>GET <b>/api/team_admin_list</b></code></summary>

##### Description
This endpoint offers an efficient way for authorized users to obtain a detailed list of administrators who hold elevated roles (admins) within a particular team. This information is crucial for understanding team leadership and ensuring proper delegation of tasks.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | team_id | required | integer |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message": "List of admins", "data": [{..}]}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "message": "List of admins",
    "data": [
        [
            {
                "id": 1,
                "email": "yogeshjadhav@gmail.com",
                "password": "$2a$08$1nL9HeQsn57L0W5vQ1eh7O4Q/F2ATgntUrkKrZPxjxvdLjtRBZJEe",
                "username": "kni9ht",
                "name": "Yogesh J",
                "role": "ADMINISTRATOR",
                "language": "en-US",
                "created_at": "2023-07-24T09:45:42.806Z",
                "updated_at": "2023-07-24T09:45:42.806Z"
            }
        ],
        [
            {
                "id": 2,
                "email": "gaurishewale@gmail.com",
                "password": "$2a$08$G31wyyO7M9EmDBY3MA.qkez6/pLynD0Y5oQagkHf4gEZEJAzehDXq",
                "username": "gaurishewale",
                "name": "Gauri S",
                "role": "ADMINISTRATOR",
                "language": "en-US",
                "created_at": "2023-07-24T09:46:13.453Z",
                "updated_at": "2023-07-24T09:46:13.453Z"
            }
        ]
    ]
}
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/api/team_admin_list
> ```

</details>

<!-- Make Team -->
<details>
 <summary>Make Team : <code>POST <b>/api/make_team</b></code></summary>

##### Description
This endpoint is useful for authorized users to initiate the creation of a new team within the Vajra Tool's ecosystem. The process involves specifying the team's name, administrators, members, and permissions, allowing for the establishment of a collaborative group focused on specific tasks and objectives.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | name | required | string |
> | admins | required | array<integer>|
> | members | required | array<integer>|
> | permissions | required | array<string>|

##### Request Body Example
```json
{
    "name":"Team 1",
    "admins": [1,2],
    "members":[2,3,4],
    "permissions":[]
}
```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `"message": "Team creation successfull"`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/api/make_team
> ```

</details>

<!-- Add Member -->
<details>
 <summary>Add Member : <code>POST <b>/api/add_member</b></code></summary>

##### Description
This endpoint provides a streamlined way for authorized users to add new members to an already established team within the Vajra Tool's ecosystem. The process involves specifying the team to which members will be added and providing the member IDs to be appended to the team's membership.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | append | required | boolean |
> | team_id | required | integer |
> | member_ids | required | array<integer>|

##### Request Body Example
```json
{
    "append":false,
    "team_id":1,
    "member_ids":[2,3]
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `"message": "member addition successfull"`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/api/add_member
> ```
</details>

<!-- Add Admin -->
<details>
 <summary>Add Admin : <code>POST <b>/api/add_admin</b></code></summary>

##### Description
This endpoint allows authorized users to add new administrators to a team or update the existing list of administrators. Administrators have enhanced permissions and responsibilities within the team, making this functionality important for effective team management.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | append | required | boolean |
> | team_id | required | integer |
> | admin_ids | required | array<integer>|

##### Request Body Example
```json
{
    "append":false,
    "team_id":2,
    "admin_ids":[2,5]
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `"message": "admin addition successfull"`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/api/add_admin
> ```
</details>


<!-- Edit Permission -->
<details>
 <summary>Edit Permission : <code>POST <b>/api/edit_permission</b></code></summary>

##### Description
This endpoint offers an effective way for authorized users to modify the permissions assigned to a particular team within the Vajra Tool's ecosystem. Permissions define what actions team members can perform, and this functionality ensures that access and actions are aligned with the team's objectives and responsibilities.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | append | required | boolean |
> | team_id | required | integer |
> | permissions | required | array<string>|

##### Request Body Example
```json
{
    "append":false,
    "team_id":1,
    "permissions":["READ", "WRITE"]
}
```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `"message": "Permission update successfull"`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |


##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/api/edit_permission
> ```

</details>

Assign Event -->
<details>
 <summary><code>POST <b>/api/assign_event</b></code></summary>

##### Description
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/assign_event
> ```

</details>

### Backend Rules

<!-- Backend Rules -->
<details>
 <summary>Backend Rules List : <code>GET <b>/backend_rules/list</b></code></summary>
##### Description
This endpoint offers a streamlined approach for authorized users to obtain a list of backend rules present in the Vajra.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message: "Backend Rules", data: [{..},{..}]"}`     |
> | `500`     | `application/json`         | `Internal Server Error` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
"message": "Backend Rules",
    "data": [
        {
            "id": 1,
            "tactic": "PRIVILEGE ESCALATION, PERSISTENCE",
            "technique": "KERNAL_MODULES_EXTENSIONS_MODPROBE",
            "technique_id": "T1547.006",
            "link": "",
            "os_name": "Linux",
            "severity": "Medium"
        },
        {
            "id": 2,
            "tactic": "PRIVILEGE ESCALATION,PERSISTENCE",
            "technique": "KERNAL_MODULES_EXTENSIONS_INSMOD",
            "technique_id": "T1547.006",
            "link": "",
            "os_name": "Linux",
            "severity": "Medium"
        },
        {
            "id": 3,
            "tactic": "PRIVILEGE ESCALATION, PERSISTENCE",
            "technique": "KERNAL_MODULES_EXTENSIONS_LSMOD",
            "technique_id": "T1547.006",
            "link": "",
            "os_name": "Linux",
            "severity": "Medium"
        },
        {
            "id": 4,
            "tactic": "PRIVILEGE ESCALATION, PERSISTENCE",
            "technique": "KERNAL_MODULES_EXTENSIONS_RMMOD",
            "technique_id": "T1547.006",
            "link": "",
            "os_name": "Linux",
            "severity": "Medium"
        }
    ]
}
```

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json"  http://localhost:4000/api/backend_rules/list
> ```

</details>

### Blog Module

---

Endpoints to manage Blogs settings.


<!-- Blogs List -->
<details>
 <summary>Blogs List : <code>GET <b>/blogs/list</b></code></summary>

##### Description
This endpoint offers a streamlined approach for authorized users to obtain a list of blogs present within the Vajra Tool's ecosystem. Blogs play vital role in information sharing of latest tech updates and news.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message: "Blogs", data: [{..},{..}]"}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
"message": "Blogs",
    "data": [
            {
                "id": "1",
                "name": "test",
                "content":"This is test",
                "image": "https://getvajra.com/static/media/mainpagedashboard.6b64a18e13849889e13d.png",
                "links":"",
            },
            {
                "id": "2",
                "name": "testa",
                "content":"This is test A",
                "image": "https://getvajra.com/static/media/mainpagedashboard.6b64a18e13849889e13d.png",
                "links":"",
            },
                        {
                "id": "3",
                "name": "testb",
                "content":"This is test C",
                "image": "https://getvajra.com/static/media/mainpagedashboard.6b64a18e13849889e13d.png",
                "links":"",
            },
        ]
}
```

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json"  http://localhost:4000/api/blogs/list
> ```

</details>

<!-- Add Blog -->
<details>
 <summary>Add Blog : <code>POST <b>/blogs/add</b></code></summary>

##### Description
This endpoint provides an efficient mechanism for authorized users to introduce new blogs into the Vajra Tool's ecosystem.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | name | required | string |
> | content | required | string |
> | image | required | string |
> | link | required | string |

##### Request Body Example
```json
{
    "name":"test",
    "content":"testing",
    "image":"https://getvajra.com/static/media/mainpagedashboard.6b64a18e13849889e13d.png",
    "link":"getvajra.com"
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `"message": "Addition of blog with name test successful`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/blog/add
> ```

</details>

<!-- Blog Update -->

<details>
 <summary>Blog Config : <code>POST <b>/blogs/update</b></code></summary>

##### Description
This endpoint is an aspect of the Vajra's Blog functionality. It empowers authorized users to modify and update existing blogs, allowing for fine-tuning of the tool's behavior based on changing requirements.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | id   | required | integer |
> | name | required | string |
> | content | required | string |
> | image | required | string |
> | link | required | string |



##### Request Body Example
```json
{
    "id": "35",
    "name":"test",
    "content":"This is test content",
    "image":"https://getvajra.com/static/media/mainpagedashboard.6b64a18e13849889e13d.png",
    "link":"getvajra.com"
}
```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"status": "Success", "message": "Updation of blog with id 35 successful"`     |
> | `200`     | `application/json` | `{"status": "Fail", "message": "Blog with id 35 not found. Updation not successful"}` |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/blogs/update
> ```

</details>

<!-- Blog Delete-->
<details>
 <summary>Delete Blog : <code>DELETE <b>/blogs/delete</b></code></summary>

##### Description
This endpoint provides a straightforward means for authorized users to initiate the removal of a blog from the Vajra's website. By utilizing this functionality, users can ensure that obsolete or unnecessary blogs are eliminated.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | id | required | integer |

##### Request Body Example
```json
{
    "id" : 30
}

```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"status": "Success", "message": "Deletion of blog with id 2 successful"`     |
> | `200`     | `application/json` | `{"status": "Fail", "message": "Blog with id 2 not found. Deletion not successful"}` |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X DELETE -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/blogs/delete
> ```

</details>

### Config Module

---

Endpoints to manage configuration settings.
<!-- Config Table Name -->
<details>
 <summary>List Config Tables : <code>GET <b>/config/table_name</b></code></summary>

##### Description
This endpoint is for authorized users to retrieve a list of tables used in configuration queries within the system. By listing these tables, users can better understand how the system stores and manages configuration-related data.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ "message" : "Table list Queries", "data" : [{..}, {..}]}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "message": "Table list Queries",
    "data": [
        {
            "table_name": "process_open_pipes"
        },
        {
            "table_name": "process_open_files"
        },
        {
            "table_name": "listening_ports"
        },
        {
            "table_name": "routes"
        },
        {
            "table_name": "logged_in_users"
        },
        {
            "table_name": "arp_cache"
        },
        {
            "table_name": "suid_bin"
        },
        {
            "table_name": "process_open_sockets"
        },
        {
            "table_name": "users"
        },
        {
            "table_name": "groups"
        },
        {
            "table_name": "process_events"
        },
        {
            "table_name": "user_events"
        },
        {
            "table_name": "processes"
        },
        {
            "table_name": "process_file_events"
        },
        {
            "table_name": "socket_events"
        },
        {
            "table_name": "cpu_time"
        },
        {
            "table_name": "memory_info"
        },
        {
            "table_name": "process_envs"
        },
        {
            "table_name": "bpf_socket_events"
        },
        {
            "table_name": "bpf_process_events"
        },
        {
            "table_name": "bpf_file_events"
        },
        {
            "table_name": "interface_addresses"
        },
        {
            "table_name": "docker_events"
        },
        {
            "table_name": "chrome_extensions"
        }
    ]
}
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/config/table_name
> ```

</details>

<!-- Config List -->
<details>
 <summary>Config Queries List : <code>GET <b>/config/list</b></code></summary>

##### Description
This endpoint offers a streamlined approach for authorized users to obtain a detailed list of configuration queries present within the Vajra Tool's ecosystem. Configuration queries play a vital role in retrieving and manipulating configuration data that governs the behavior of the tool.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message: "Config Queries", data: [{..},{..}]"}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
"message": "Config Queries",
    "data": [
        {
            "id": "1",
            "query": "{\"schedule\":{\"process_envs\":{\"interval\":2,\"query\":\"select * from process_envs\", \"snapshot\": false},\"process_open_pipes\":{\"interval\":2,\"query\":\"select * from process_open_pipes\", \"snapshot\": false},\"process_open_files\":{\"interval\":2,\"query\":\"select * from process_open_files\", \"snapshot\": false},\"listening_ports\":{\"interval\":2,\"query\":\"select * from listening_ports\", \"snapshot\": false},\"routes\":{\"interval\":5,\"query\":\"select * from routes\", \"snapshot\": false},\"logged_in_users\":{\"interval\":5,\"query\":\"select * from logged_in_users\", \"snapshot\": false},\"arp_cache\":{\"interval\":5,\"query\":\"select * from arp_cache\", \"snapshot\": false},\"suid_bin\":{\"interval\":20,\"query\":\"select * from suid_bin\", \"snapshot\": false}, \"process_open_sockets\":{\"interval\":20,\"query\":\"select * from process_open_sockets\", \"snapshot\": false}, \"users\":{\"interval\":20,\"query\":\"select * from users\", \"snapshot\": false},\"groups\":{\"interval\":20,\"query\":\"select * from groups\", \"snapshot\": false}, \"process_events\":{\"interval\":10,\"query\":\"select * from process_events\", \"snapshot\": false}, \"user_events\":{\"interval\":10,\"query\":\"select * from user_events\", \"snapshot\": false}, \"socket_events\":{\"interval\":10,\"query\":\"select * from socket_events\", \"snapshot\": false}}},",
            "query_name": "all queries"
        },
        {
            "id": "2",
            "query": "{\"schedule\":{\"process_open_pipes\":{\"interval\":240,\"query\":\"select * from process_open_pipes\",\"snapshot\":false}}}",
            "query_name": "process_open_pipes"
        },
        {
            "id": "3",
            "query": "{\"schedule\":{\"process_open_files\":{\"interval\":240,\"query\":\"select * from process_open_files\",\"snapshot\":false}}}",
            "query_name": "process_open_files"
        }
        ]
}
```

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/config/list
> ```

</details>

<!-- Add Config -->
<details>
 <summary>Add Config : <code>POST <b>/config/add</b></code></summary>

##### Description
This endpoint provides an efficient mechanism for authorized users to introduce new configuration queries into the Vajra Tool's ecosystem. Configuration queries dictate which all data needs to be stored in fleet database from a particular node, allowing users to tailor its behavior to meet their organization's needs.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | query_name | required | string |
> | query      | required | JSON object -'\{ "schedule" : \{ "\<table_name\>" : \{"interval" : \<integer\>, "query" : \<string\>, "snapshot" : \<boolean\>\}\}\}' |

##### Request Body Example
```json
{
    "query_name": "testing_query1",
    "query": {
        "schedule": {
            "process_open_pipes": {
                "interval": 5,
                "query": "select * from users;",
                "snapshot": false
            }
        }
    }
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `"message": "Addition of config query with query_name testing_query2 successfull`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/config/add
> ```

</details>

<!-- Config Update -->

<details>
 <summary>Update Config : <code>POST <b>/config/update</b></code></summary>

##### Description
This endpoint is a pivotal aspect of the Vajra Tool's configuration management system. It empowers authorized users to modify and update existing configuration queries, allowing for fine-tuning of the tool's behavior based on changing requirements.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | id   | required | integer |
> | query_name | required | string |
> | query      | required | JSON object -'\{ "schedule" : \{ "\<table_name\>" : \{"interval" : \<integer\>, "query" : \<string\>, "snapshot" : \<boolean\>\}\}\}' |


##### Request Body Example
```json
{
    "id": "35",
    "query_name": "test",
    "query": {
        "schedule": {
            "process_events": {
                "interval": 10,
                "query": "select * from process_events"
            },
            "process_file_events": {
                "interval": 10,
                "query": "select * from process_file_events"
            },
            "process_list": {
                "interval": 60,
                "query": "select * from processes",
                "snapshot": true
            },
            "socket_events": {
                "interval": 10,
                "query": "select * from socket_events"
            },
            "user_events": {
                "interval": 10,
                "query": "select * from user_events"
            }
        }
    }
}
```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `"message": "Updation of config query with id 35 successful"`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/config/update
> ```

</details>

<!-- Config Delete-->
<details>
 <summary>Delete Config : <code>DELETE <b>/config/delete</b></code></summary>

##### Description
This endpoint provides a straightforward means for authorized users to initiate the removal of a configuration query from the Vajra Tool's ecosystem. By utilizing this functionality, users can ensure that obsolete or unnecessary queries are eliminated, contributing to a more organized and efficient configuration management process.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | id | required | integer |

##### Request Body Example
```json
{
    "id" : 30
}

```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `"message": "Config query with id 30 successful"`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X DELETE -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/config/delete
> ```

</details>

### Dashboard

---

Endpoints to retrieve various dashboard metrics and insights.
<!-- Alert Count -->
<details>
 <summary>Alert Count : <code>GET <b>/dashboard/alert_count</b></code></summary>

##### Description
This endpoint provides an at-a-glance summary of the top 10 alerts within the Vajra Tool's ecosystem. By returning the alerts along with their corresponding counts, users can swiftly identify the most prominent security concerns that require immediate attention.

##### Query Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | host_identifier | optional | string |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"[EVENT NAME]": <integer>}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "FILE DELETION SINGLE FILE": 1463,
    "EVENT_TRIGGERED_EXECUTION_TRAP": 1362,
    "OVERWRITE_FILE_WITH_DD": 790,
    "NATIVE_API": 664,
    "UDEVADM_PRIVILEGED_COMMAND": 276,
    "INJECT_LD_PRELOAD": 254,
    "FILE DELETION ENTIRE FOLDER": 244,
    "ABUSE_ELEVATION_CONTROL_MECHANISM_SUDO_CACHING_FILE": 214,
    "MODIFY_SYSTEM_PROCESS_SYSTEMD_SERVICE_FILE": 214,
    "SYSTEM_NETWORK_CONNECTIONS_DISCOVERY": 197
}
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/dashboard/alert_count
> ```

</details>

<!-- Events Count -->
<details>
 <summary>Events Count : <code>GET <b>/dashboard/events_count</b></code></summary>

##### Description
This endpoint offers a concise overview of the count of open and closed events within the Vajra Tool's ecosystem. By returning this information, users can promptly understand the status of security incidents and gauge the effectiveness of incident response efforts.

##### Query Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | host_identifier | optional | string |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `[{ "count" : <integer>, "name" : <integer>}]`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
[
    {
        "count": 6722,
        "name": "Open"
    },
    {
        "count": 4,
        "name": "Close"
    }
]
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/dashboard/events_count
> ```

</details>

<!-- High risk count -->
<details>
 <summary>High Risk Count : <code>GET <b>/dashboard/high_risk_count</b></code></summary>

##### Description
The endpoint offers authorized users insights into the top "n" high-risk machines, along with the number of alerts generated from them. This information aids in identifying and prioritizing potential security threats.

##### Query Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | level | optional | string ["DESC" \| "ASC"]|
> | topn | optional | integer |
> | severity | optional | integer |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `[{ "host_identifier" : <string>, "hostname" : <string>, "count" : <integer>} , ...]`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
[
    {
        "host_identifier": "20190727-5c87-9c38-1fb2-5c879c381fb6",
        "hostname": "sablearjun.com",
        "count": 3674
    }
]
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/dashboard/high_risk_count
> ```

</details>

<!-- Mitre Analysis -->
<details>
 <summary>Mitre Analysis : <code>GET <b>/dashboard/mitre_analysis</b></code></summary>

##### Description
This endpoint offers an insightful overview of MITRE ATT&CK® framework-based attacks being triggered within the Vajra Tool's ecosystem. By returning information about different attack techniques and their quantities of occurence, users can effectively assess the prevalence of various attack vectors and potential risks.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | host_identifier | optional | string |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"[EVENT NAME]": <integer>}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "COMMAND AND CONTROL": 1,
    "DEFENCE EVASION": 180,
    "PRIVILEGE ESCALATION": 2075,
    "IMPACT": 803,
    "PERSISTENCE": 1653,
    "EXECUTION": 787,
    "INITIAL ACCESS": 254,
    "DISCOVERY": 293,
    "COLLECTION": 35,
    "Unnamed": 2434
}
```

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/dashboard/mitre_analysis
> ```

</details>

<!-- Node Alert Count -->
<details>
 <summary>Node Alert Count : <code>GET <b>/dashboard/node_alert_count</b></code></summary>

##### Description
This endpoint offers a consolidated overview of node information along with the number of alerts associated with each node within the Vajra Tool's ecosystem. By presenting this data, users can quickly identify nodes that are generating a significant number of alerts and take appropriate actions.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `[{..}]`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
[
    {
        "id": "3",
        "config_id": "17,18,20,21,22,23",
        "host_identifier": "20190727-5c87-9c38-1fb2-5c879c381fb6",
        "os_arch": "x86_64",
        "os_build": "unknown",
        "os_major": "22",
        "os_minor": "10",
        "os_name": "Ubuntu",
        "os_platform": "ubuntu",
        "osquery_version": "5.0.1.1",
        "node_key": "ETPVzxlFrXL8b7mtKLHI",
        "node_invalid": false,
        "reenroll_allowed": true,
        "hardware_vendor": "LENOVO",
        "hardware_model": "81NE",
        "hardware_version": "Lenovo IdeaPad S540-15IWL D",
        "hostname": "sablearjun.com",
        "enrolled_on": "1686327293",
        "last_seen": "1687420108",
        "hardware_cpu_logical_core": "8",
        "hardware_cpu_type": "x86_64",
        "hardware_physical_memory": "20708769792",
        "count": "3674"
    },
    {
        "id": "6",
        "config_id": "22,20,21,18,17,23",
        "host_identifier": "03345eaa-ffa5-ef99-e85d-04421a2d938c",
        "os_arch": "x86_64",
        "os_build": "unknown",
        "os_major": "22",
        "os_minor": "04",
        "os_name": "Ubuntu",
        "os_platform": "ubuntu",
        "osquery_version": "5.0.1.1",
        "node_key": "BWDxS22JjzhMaiRrV41m",
        "node_invalid": false,
        "reenroll_allowed": true,
        "hardware_vendor": "ASUS",
        "hardware_model": "System Product Name",
        "hardware_version": "System Version",
        "hostname": "IITB",
        "enrolled_on": "1686909292",
        "last_seen": "1687159362",
        "hardware_cpu_logical_core": "16",
        "hardware_cpu_type": "x86_64",
        "hardware_physical_memory": "16530235392",
        "count": "1107"
    }
]
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/dashboard/node_alert_count
> ```

</details>

<!-- Nodes Count -->
<details>
 <summary>Node Count : <code>GET <b>/dashboard/nodes_count</b></code></summary>

##### Description
This endpoint offers a straightforward method for authorized users to obtain a count of nodes \[offline, online\] within the Vajra Tool's ecosystem. By returning this information, users can swiftly gauge the extent of the network's reach and the number of nodes that require monitoring and management.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | ``     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/dashboard/node_count
> ```

</details>

<!-- OS Count -->

<details>
 <summary>OS Count : <code>GET <b>/dashboard/os_count</b></code></summary>

##### Description
This endpoint offers a succinct overview of the count of different operating system types within the Vajra Tool's ecosystem. It provides authorized users with a summary of the different operating system (OS) types present within the network, aiding in understanding the diversity of systems in use.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `[{ "count": <integer>, "name" : <string>}]`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
[
    {
        "count": 5,
        "name": "Windows"
    },
    {
        "count": 3,
        "name": "Linux"
    }
]
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/dashboard/os_count
> ```

</details>

<!-- Severity Count -->

<details>
 <summary>Severity Count : <code>GET <b>/dashboard/severity_count</b></code></summary>

##### Description
This endpoint offers insights into the severity levels of security incidents within the Vajra Tool's ecosystem. By returning information about different severity levels and their counts, users can effectively assess the overall risk landscape and prioritize incident response efforts.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | host_identifier | optional | string |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `[{ "severity" : <integer>, "value" : <integer> }]`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
[
    {
        "severity": 4,
        "value": 6726
    }
]
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/dashboard/severity_count
> ```

</details>

<!-- Weekly Alert Count -->

<details>
 <summary>Weekly Alert Count : <code>GET <b>/dashboard/weekly_alert_count</b></code></summary>

##### Description
This endpoint offers insights into the weekly distribution of alert counts within the Vajra Tool's ecosystem. By returning information about the number of alerts recorded each week, users can identify patterns, spikes, or changes in security incidents over time.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | host_identifier | optional | string |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/dashboard/weekly_alert_count
> ```

</details>



### Nodes

---

Endpoints related to managing nodes.

<!-- Node List  -->
<details>
 <summary>Node List : <code>GET <b>/node/list</b></code></summary>

##### Description
Returns list of nodes
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/node/list
> ```

</details>

<!-- Node Query -->

<details>
 <summary>Node Query : <code>GET <b>/node/query</b></code></summary>

##### Description
Returns additional information on nodes
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/node/query
> ```

</details>

<!-- Node Memory Health -->
<details>
 <summary>Node Memory Health : <code>GET <b>/node/memory_health</b></code></summary>

##### Description
Returns information regarding node's memory health
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/node/memory_health
> ```

</details>

<!-- Historical Data -->
<details>
 <summary>Node Historical Data : <code>POST <b>/node/historical_data</b></code></summary>

##### Description
Allows execution of query on existing data
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/node/historical_data
> ```

</details>

<!-- Node reenroll -->
<details>
 <summary>Node reenroll : <code>POST <b>/node/reenroll</b></code></summary>

##### Description
Permits user to reenroll a node
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/node/reenroll
> ```

</details>

<!-- Node config -->
<details>
 <summary>Node Config : <code>POST <b>/node/config</b></code></summary>

##### Description
Allows modification of node configuration
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/node/config
> ```

</details>

### IP
<!-- IP add file-->

<details>
 <summary>IP add File : <code>POST <b>/ip/file_upload</b></code></summary>

##### Description
Takes .csv file as an input and adds ips present in csv to database table
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | file | required | .csv file |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ status: "Success", message: 'File uploaded successfully', file: <fileDetails>, newIps }`     |
> | `500`     | `application/json`         | `{ status: "Fail", message: "Error in adding Ip file. Internal Server Error" }` |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/ip/file_upload
> ```

</details>

<!-- Ip FilesList -->
<details>
 <summary>Ip Files List : <code>GET <b>/ip/list</b></code></summary>

##### Description
This endpoint offers a streamlined approach for authorized users to obtain a list of Malicious IPs CSV files uploaded by users in the Vajra.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message: "Ip files", data: [{..},{..}]"}`     |
> | `500`     | `application/json`         | `{ status: 'Fail', message: 'Error in listing IP files. Internal Server Error' }` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
"message": "Ip files",
    "data": [
        {
            "name": "testabc.csv",
            "uploadedBy": "gaurishewale",
            "size": 38,
            "modifiedAt": 1700643272450,
            "uploadedAt": 1700643272450
        },
        {
            "name": "t2.csv",
            "uploadedBy": "gaurishewale",
            "size": 48,
            "modifiedAt": 1700627264728,
            "uploadedAt": 1700627264728
        },
        {
            "name": "t1.csv",
            "uploadedBy": "test",
            "size": 34,
            "modifiedAt": 1699507273052,
            "uploadedAt": 1699507273052
        }
    ]
}
```

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json"  http://localhost:4000/api/ip/list
> ```

</details>

### YARA
<!-- IP add file-->

<details>
 <summary>Yara add File : <code>POST <b>/yara/file_upload</b></code></summary>

##### Description
Takes .yar file as an input and stores it on server side
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | file | required | .yar file |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ status: "Success", message: 'File uploaded successfully', file: <fileDetails>, newIps }`     |
> | `500`     | `application/json`         | `{ status: "Fail", message: "Error in adding Yara file. Internal Server Error" }` |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/yara/file_upload
> ```

</details>

<!-- Yara FilesList -->
<details>
 <summary>Yara Files List : <code>GET <b>/yara/list</b></code></summary>

##### Description
This endpoint offers a streamlined approach for authorized users to obtain a list of YARA files uploaded by users in the Vajra.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message: "Yara files", data: [{..},{..}]"}`     |
> | `500`     | `application/json`         | `{ status: 'Fail', message: 'Error in listing Yara files. Internal Server Error' }` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
"message": "Yara files",
    "data": [
        {
            "name": "test.yar",
            "platform": "windows",
            "uploadedBy": "gaurishewale",
            "size": 0,
            "modifiedAt": 1700047930605,
            "uploadedAt": 1700047930605
        },
        {
            "name": "MALW_Athena.yar",
            "platform": "windows,linux",
            "uploadedBy": "gaurishewale",
            "size": 3149,
            "modifiedAt": 1700044372011,
            "uploadedAt": 1700044372011
        }
    ]
}
```

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json"  http://localhost:4000/api/blogs/list
> ```

</details>

### Event

---

Endpoints for event-related operations

<!-- Event List -->
<details>
 <summary>Event list : <code>GET <b>/event/list</b></code></summary>

##### Description
Returns a list of events
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/event/list
> ```

</details>

<!-- Events Update -->
<details>
 <summary>Event Update : <code>POST <b>/event/update</b></code></summary>

##### Description
Allows User to set Event status to close
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/event/update
> ```

</details>

<!-- Event Query -->
<details>
 <summary>Event Query : <code>GET <b>/event/query</b></code></summary>

##### Description
Returns additional information about event
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | event_id | required | number |
> | host_identifier | required | string |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/event/query
> ```

</details>

<!-- Event Graph -->
<details>
 <summary>Event Graph : <code>GET <b>/event/ept</b></code></summary>

##### Description
Returns Detection Graph and signal table of event
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/event/ept
> ```

</details>

### Threat hunting module

---

Endpoints for conducting threat hunting operations.

<!-- Tactic List -->

<details>
 <summary>Tactic List : <code>GET <b>/threat_hunting/tactic</b></code></summary>

##### Description
This endpoint offers a streamlined approach for authorized users to access a list of tactics within the Vajra Tool's threat hunting ecosystem. Tactics provide insight into the various methods that threat actors might employ to compromise network security.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"message":"Tactic List", "data" : [..]}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/threat_hunting/tactic
> ```

</details>

<!-- Technique List -->
<details>
 <summary>Technique List : <code>GET <b>/threat_hunting/technique</b></code></summary>

##### Description
This endpoint enables authorized users to access a comprehensive list of techniques associated with a specific tactic, aiding in the identification and understanding of potential threat vectors. Techniques provide insight into the specific methods that threat actors might use to execute various tactics and compromise network security.
##### Query Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | tactic | required | string |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `[{..},{..}]`     |
> | `400`     | `application/json`         | `{"status":"Fail","message":"Tactic Name should be present"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/threat_hunting/technique
> ```

</details>

<!-- Sub Technique -->
<details>
 <summary>Sub Technique List : <code>GET <b>/threat_hunting/sub_technique</b></code></summary>

##### Description
This endpoint enables authorized users to access a comprehensive list of sub-techniques associated with a specific technique and tactic, enhancing the granularity of threat analysis. Sub-techniques provide detailed insight into specific variations of techniques that threat actors might employ to compromise network security.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | tactic | required | string |
> | technique | required | string |
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `Configuration created successfully`     |
> | `400`     | `application/json`         | `{"status":"Fail","message":"Tactic name and technique name should be present"}` |
> | `500`     | `application/json`  | `{"message": 'An error occurred while retrieving sub-techniques'}` None                                     |

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/threat_hunting/sub_technique
> ```

</details>


### Python Rules Module

---

Endpoints to manage and interact with Python-based security rules.

<!-- Python Rules Table List -->
<details>
 <summary>Table list : <code>GET <b>/python_rules/table_list</b></code></summary>

##### Description
The endpoint allows authorized users to retrieve a list of columns present in specific tables [Eg. bpf_file_events, bpf_socket_events, bpf_process_events], enhancing the development and customization of python-based security rules.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ "data" : [{...}]}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "data": [
        {
            "table_name": "bpf_process_events",
            "table_label": "bpfProcessEvents",
            "columns": [
                {
                    "column_name": "is_processed_rule_builder",
                    "column_label": "ID"
                },
                {
                    "column_name": "parent",
                    "column_label": "Host Identifier"
                },...
            ]
        },
        {
            "table_name": "bpf_socket_events",
            "table_label": "bpfSocketEvents",
            "columns": [
                {
                    "column_name": "is_processed_rule_builder",
                    "column_label": "ID"
                },
                {
                    "column_name": "protocol",
                    "column_label": "Host Identifier"
                },...
            ]
        },
        {
            "table_name": "bpf_file_events",
            "table_label": "bpfFileEvents",
            "columns": [
                {
                    "column_name": "id",
                    "column_label": "ID"
                },
                {
                    "column_name": "gid",
                    "column_label": "Host Identifier"
                },...
            ]
        }
    ]
}
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/python_rules/table_list
> ```

</details>

<!-- Python Rules List -->
<details>
 <summary>Python Rules List : <code>GET <b>/python_rules/list_python_rule</b></code></summary>

##### Description
The endpoint enables authorized users to access a comprehensive list of existing python rules, facilitating management and visibility of these custom security rules.
Python rules are custom security rules written in Python that allow users to define specific logic for identifying and responding to security threats.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ "message" : "Python rules", "data" : [{...}]}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "message": "Python Rules",
    "data": [
        {
            "id": "5",
            "name": "apt_testing",
            "rule": "# This is the Sample Rule \n#   Don't forget to :\n#       1. Wrap all this in Definition/Function\n#       2. Return :\n#           a. Boolean   (Which determines weather to trigger the rule or not)\n#           b. String    (Which display some context about the rule triggered)\n\ndef apt_testing(output):\n    print(\"Testing2\")\n    return False, ss",
            "severity": 2,
            "technique": "Initial Access",
            "technique_id": "",
            "link": "vajra.com",
            "comment": "Test",
            "os_name": "Linux",
            "created": "1690545249782",
            "last_modified": "1690545249782",
            "rule_written_by": "gaurishewale"
        },
    ]
}
```

##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/python_rules/list_python_rule
> ```

</details>

<!-- Add Python Rule -->
<details>
 <summary>Python Rule Add : <code>POST <b>/python_rules/add_python_rule</b></code></summary>

##### Description
This endpoint provides a user-friendly mechanism for authorized users to add new python rules within the Vajra Tool's ecosystem, empowering them to define custom security logic for identifying and responding to potential threats.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | name | required | string |
> | rule | required | string |
> | severity | required | integer |
> | technique | required | string |
> | technique_id | optional | string |
> | link | optional | string |
> | comment | optional | string |
> | os_name | required | string |
> | rule_written_by | required | string

##### Request Body Example
```json
{
    "name": "test2",
    "rule": "# This is the Sample Rule \n#   Don't forget to :\n#       1. Wrap all this in Definition/Function\n#       2. Return :\n#           a. Boolean   (Which determines weather to trigger the rule or not)\n#           b. String    (Which display some context about the rule triggered)\n\ndef test2(output):\n    print(\"Test\")\n    return False, ss",
    "severity": 2,
    "technique": "Execution",
    "technique_id": "",
    "link": "getvajra.com",
    "comment": "test1",
    "os_name": "Linux",
    "rule_written_by": "gaurishewale"
}
```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `201`     | `application/json` | `{ "status" : "Success" , message : "Rule added successfully"}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/python_rules/add_python_rule
> ```

</details>

<!-- Python Rules Update -->

<details>
 <summary>Python Rule Update : <code>POST <b>/python_rules/update_python_rule</b></code></summary>

##### Description
The endpoint empowers authorized users to update existing python rules, enabling them to refine and adapt their custom security logic to evolving threats and requirements.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | id   | required | integer |
> | name | required | string |
> | rule | required | string |
> | severity | required | integer |
> | technique | required | string |
> | technique_id | optional | string |
> | link | optional | string |
> | comment | optional | string |
> | os_name | required | string |
> | rule_written_by | required | string

##### Request Body Example
```json
{
    "name": "test2",
    "rule": "def test2(output):\n    print(\"Test\")\n    return False, ss",
    "severity": 2,
    "technique": "Execution",
    "technique_id": "",
    "link": "getvajra.com",
    "comment": "test2",
    "os_name": "Linux",
    "rule_written_by": "gaurishewale"
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"status": "Success", "message" : "Rule updated successfully"}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/python_rules/update_python_rule
> ```

</details>

<!-- Python Rules Delete-->
<details>
 <summary>Python Rule Delete : <code>DELETE<b>/python_rules/delete_python_rule</b></code></summary>

##### Description
 This endpoint provides authorized users with the capability to remove existing python rules, allowing for the management and maintenance of custom security logic.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | id | required | integer |

##### Request Body Example
```json
{
    "id" : "6"
}
```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{"status" : "Success", "message": "Rule deleted successfully"}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Example cURL

> ```javascript
>  curl -X DELETE -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/python_rules/delete_python_rule
> ```

</details>

### Rule Builder Module

---

Endpoints to create, update, and manage dynamic security rules using a visual interface.

<!-- Rule Builder Rules List -->
<details>
 <summary>Rules List : <code>GET <b>/rule_builder/rule_list</b></code></summary>

##### Description
This endpoint provides an intuitive way for authorized users to access a list of rule builder rules within the Vajra Tool's ecosystem. Rule builder rules enable users to define and customize security logic using a visual interface, enhancing the flexibility and adaptability of threat detection and response strategies.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | - | - |

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ message: "Rule Builder Rules Queries",  "data" : [ {...}. {...}]}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "message": "Rule Builder Rules Queries",
    "data": []
}
```
##### Example cURL

> ```javascript
>  curl -X GET -H "Content-Type: application/json" http://localhost:4000/api/rule_builder/rule_list
> ```

</details>

<!-- Rule Builder Add Rule -->
<details>
 <summary>Add Rule : <code>POST <b>/rule_builder/rule_add</b></code></summary>

##### Description
This endpoint empowers authorized users to create and add new dynamic security rules, enhancing their ability to define and adapt threat detection and response strategies.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | name | required | string |
> | description | required | string |
> | rule_builder | required | string |
> | severity | required | string |
> | tactics | required | string |
> | technique_id | required | string |
> | link | optional | string |
> | type | required | string |
> | platform | required | string |
> | rule_written_by | required | string
> | alerters | optional | string |

##### Request Body Example
```json
{
    "name": "test123pqr",
    "description": "test",
    "alerters": [
        {
            "value": "email",
            "label": "email"
        }
    ],
    "severity": 1,
    "platform": [
        {
            "value": "Darwin",
            "label": "Darwin"
        },
        {
            "value": "Linux",
            "label": "Linux"
        }
    ],
    "type": "Default",
    "tactics": "",
    "technique_id": "",
    "link": "link",
    "rule_builder": {
        "operator": "AND",
        "queries": [
            {
                "tableName": "bpf_process_events",
                "columnName": "path",
                "conditionType": "equal",
                "value": "/root/",
                "dataType": "string",
                "id": "2b9c4e89-69b3-43c2-b4fb-b5601e88c66f"
            }
        ]
    },
    "rule_written_by": "gaurishewale"
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{status: "Success", message: "Rule Builder Rule <rulename> added successfully"}`     |
> | `500`     | `application/json`         | `{ status: "Fail", message: "Error in adding rule to rule builder due to internal server error" }` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "status": "Success",
    "message": "Rule Builder Rule test123pqr added successfully"
}
```
##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/rule_builder/rule_add
> ```

</details>

<!-- Rule Builder Rule Update -->

<details>
 <summary>Rule Update : <code>POST <b>/rule_builder/rule_update</b></code></summary>

##### Description
This endpoint gives authorized users a convenient way to modify and update existing rules within the Vajra Tool's rule builder ecosystem

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | None | required | object (JSON or YAML) |

##### Request Body Example
```json
{
    "name": "test123456",
    "description": "testing",
    "alerters": [
        {
            "value": "email",
            "label": "email"
        }
    ],
    "severity": 3,
    "platform": [
        {
            "value": "Darwin",
            "label": "Darwin"
        },
        {
            "value": "Linux",
            "label": "Linux"
        }
    ],
    "type": "Default",
    "tactics": [],
    "technique_id": "",
    "link": "link",
    "rule_builder": {
        "operator": "AND",
        "queries": [
            {
                "tableName": "bpf_process_events",
                "columnName": "path",
                "conditionType": "equal",
                "value": "/root/",
                "dataType": "string",
                "id": "2b9c4e89-69b3-43c2-b4fb-b5601e88c66f"
            },
            {
                "tableName": "bpf_process_events",
                "columnName": "id",
                "conditionType": "not_equal",
                "value": "1234",
                "dataType": "string",
                "id": "7af9d451-2a83-4244-8635-9fcf6db11bac"
            }
        ]
    },
    "id": "293",
    "rule_written_by": "gaurishewale"
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ "status": "Success", "message": "Updation of rule builder rule with id 293 successful" }`     |
> | `500`     | `application/json`         | `{ status: "Fail", message: "Error in updating rule builder rule due to internal server error" }` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{ "status": "Success", "message": "Updation of rule builder rule with id 293 successful" }
```
##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/rule_builder/rule_update
> ```

</details>

<!-- Rule Builder Rule Delete-->
<details>
 <summary>Rule Delete : <code>DELETE<b>/rule_builder/rule_delete</b></code></summary>

##### Description
This endpoint provides authorized users with the capability to remove existing dynamic security rules, allowing for efficient management and maintenance of security logic.

##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | id | required | string |

##### Request Body Example
```json
{
    "id": 3
}
```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ status: "Success", message : `Deletion of rule builder rule with id ${id} successful`}`     |
> | `200`     | `application/json` | `{ status: "Fail", message : `Rule Builder Rule with id ${id} not found. Deletion not successful`}`
> | `400`     | `application/json`         | `{status: "Fail",message: "Id is required. Bad request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |
> | `500`     | `application/json` | `{ status: "Fail", message: "Internal Server Error" }` |

##### Response Body Example
```json
{
    "status": "Success",
    "message": "Deletion of rule builder rule with id 293 successful"
}
```
##### Example cURL

> ```javascript
>  curl -X DELETE -H "Content-Type: application/json" --data @delete.json http://localhost:4000/api/rule_builder/rule_delete
> ```

</details>

### Scheduled Queries Module

---

Endpoints to schedule and manage data of live / collected queries.
<!-- Scheduled Queries Schedule -->
<details>
 <summary>Schedule Query : <code>POST <b>/scheduled_queries/schedule</b></code></summary>

##### Description
This endpoint provides authorized users with a mechanism to schedule queries within the Vajra Tool's ecosystem. Scheduled queries are designed to automatically run predefined queries on specified hosts enabling organizations to gather relevant information without manual intervention.


##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | hosts | required | string \[comma separated host-identifiers\]|
> | query | required | string |

##### Request body Example
```json
{
    "hosts" : "4C4C4544-0050-3510-805A-B7C04F315133,B0D2A257-A0BE-11EB-80EF-38F3AB8C1B10",
    "query" : "select * from memory_info;"
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ "[host]" : id <string>}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "B0D2A257-A0BE-11EB-80EF-38F3AB8C1B10:0": "27757",
    "4C4C4544-0050-3510-805A-B7C04F315133:0": "27758"
}
```
##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/scheduled_queries/status
> ```

</details>

<!-- Scheduled Queries Status -->
<details>
 <summary>Schedule Query Status : <code>POST <b>/scheduled_queries/scheduled_queries/status</b></code></summary>

##### Description
This endpoint offers authorized users a convenient way to monitor and check the status of scheduled queries within the Vajra Tool's ecosystem. Scheduled queries are automated processes that periodically run predefined queries on specified hosts to collect relevant data.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | config_id_list | required | string \[comma separated query ids\]|


##### Request Body Example
```json
{
    "config_id_list" : "27757,27758"
}
```
##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ "id: { "sent" : <boolean>, "send_time" : <string> }"}`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "277757": {
        "sent": true,
        "send_time": "169230597105"
    },
    "277758": {
        "sent": true,
        "send_time": "169230597135"
    }
}
```
##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/scheduled_queries/response
> ```
</details>

<!-- Scheduled Queries Response -->

<details>
 <summary>Scheduled Query Response : <code>POST <b>/scheduled_queries/response</b></code></summary>

##### Description
This endpoint allows authorized users to retrieve the responses generated by scheduled queries, providing valuable insights into the collected data and analysis results.
##### Parameters

> | name | type     | data type             |
> | ---- | -------- | --------------------- |
> | config_id_list | required | string \[comma separated query ids\] |

##### Request Body Example
```json
{
    "config_id_list" : "27757,27758"
}
```

##### Responses

> | http code | content-type               | response                                 |
> | --------- | -------------------------- | ---------------------------------------- |
> | `200`     | `application/json` | `{ "id" : "[...]" }`     |
> | `400`     | `application/json`         | `{"code":"400","message":"Bad Request"}` |
> | `405`     | `text/html;charset=utf-8`  | None                                     |

##### Response Body Example
```json
{
    "27757": "[]",
    "27758": "[]",
}
```

##### Example cURL

> ```javascript
>  curl -X POST -H "Content-Type: application/json" --data @post.json http://localhost:4000/api/scheduled_queries/response
> ```

</details>