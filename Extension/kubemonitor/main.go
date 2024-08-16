package main

import (
	"database/sql"
	"encoding/json"
	"fmt"
	_ "github.com/lib/pq"
	"github.com/tidwall/gjson"
	"io/ioutil"
	"k8s.io/apiserver/pkg/apis/audit/v1"
	"net/http"
	"net/url"
	"strings"
)

const (
	host     = "localhost"
	port     = 5432
	user     = "adhokshajmishra"
	password = "password123"
	dbname   = "honeypot"
)

// isPodCreation returns true if the given event is of a pod creation
func isPodCreation(event v1.Event) bool {
	return event.Verb == "create" &&
		event.Stage == v1.StageResponseComplete &&
		event.ObjectRef != nil &&
		event.ObjectRef.Resource == "pods" &&
		strings.HasSuffix(event.RequestURI, "/pods?fieldManager=kubectl-client-side-apply")
}

func isExecEvent(event v1.Event) bool {
	return event.Stage == v1.StageResponseComplete &&
		strings.Contains(event.RequestURI, "/exec?command=")
}

func isSecretListing(event v1.Event) bool {
	return event.Stage == v1.StageResponseComplete &&
		strings.Contains(event.RequestURI, "/secrets") &&
		event.Verb == "list"
}

func isIgnorable(event v1.Event) bool {
	result := false

	for _, source_ip := range event.SourceIPs {
		if source_ip == "127.0.0.1" {
			result = true
		}
	}

	return result
}

func prettyPrint(event v1.Event, stmt *sql.Stmt) {
	if isPodCreation(event) {

		payload := string(event.RequestObject.Raw[:])
		containers := gjson.Get(payload, "spec.containers")

		fmt.Println("Event: Pod creation")
		fmt.Println("URL: ", event.RequestURI)
		fmt.Println("Source IP: ", event.SourceIPs)
		fmt.Println("User-Agent: ", event.UserAgent)
		fmt.Println("Username: ", event.User.Username)
		fmt.Println("Request Timestamp:", event.RequestReceivedTimestamp)
		fmt.Println("Pod Name: ", event.ObjectRef.Name)
		fmt.Println("Pod Namespace: ", event.ObjectRef.Namespace)

		var raw_objectref string
		var raw_request_object string
		var raw_response_object string

		dump, _ := json.Marshal(event.ObjectRef)
		raw_objectref = string(dump)

		dump, _ = json.Marshal(event.RequestObject)
		raw_request_object = string(dump)

		dump, _ = json.Marshal(event.ResponseObject)
		raw_response_object = string(dump)

		containers.ForEach(func(key, value gjson.Result) bool {
			fmt.Println("Container name:", value.Get("name"))
			fmt.Println("Container image:", value.Get("image"))
			fmt.Println("Arguments:", value.Get("args").Array())
			stmt.Exec(event.Level, event.AuditID, event.Stage, event.RequestURI, event.Verb, event.User.Username,
				event.User.Groups, event.SourceIPs, event.UserAgent, event.RequestReceivedTimestamp.Unix(),
				event.ObjectRef.Name, event.ObjectRef.Namespace, value.Get("name").String(),
				value.Get("image").String(), value.Get("args").Array(), "", "", "",
				raw_objectref, raw_request_object, raw_response_object)
			return true
		})
		fmt.Println(" ")
	} else if isExecEvent(event) {
		u, err := url.ParseRequestURI(event.RequestURI)
		if err != nil {
			return
		}

		parameters, err := url.ParseQuery(u.RawQuery)
		if err != nil {
			return
		}

		fmt.Println("Event: Command execution")
		fmt.Println("URL: ", event.RequestURI)
		fmt.Println("Source IP: ", event.SourceIPs)
		fmt.Println("User-Agent: ", event.UserAgent)
		fmt.Println("Username: ", event.User.Username)
		fmt.Println("Request Timestamp:", event.RequestReceivedTimestamp)
		fmt.Println("Pod Name: ", event.ObjectRef.Name)
		fmt.Println("Pod Namespace: ", event.ObjectRef.Namespace)
		fmt.Println("Container:", parameters["container"])
		fmt.Println("Command:", parameters["command"])
		fmt.Println("Standard Output:", parameters["stdout"])
		fmt.Println("Standard Error:", parameters["stderr"])
		fmt.Println("")
	} else if isSecretListing(event) {
		fmt.Println("Event: Secret Listing")
		fmt.Println("URL: ", event.RequestURI)
		fmt.Println("Source IP: ", event.SourceIPs)
		fmt.Println("User-Agent: ", event.UserAgent)
		fmt.Println("Username: ", event.User.Username)
		fmt.Println("Request Timestamp:", event.RequestReceivedTimestamp)
		fmt.Println("Pod Name: ", event.ObjectRef.Name)
		fmt.Println("Pod Namespace: ", event.ObjectRef.Namespace)
		dump, _ := json.Marshal(event)
		fmt.Println(string(dump))
		fmt.Println("")
	} else if !isIgnorable(event) {
		fmt.Println("Non-ignored Event")
		dump, _ := json.Marshal(event)
		fmt.Println(string(dump))
		fmt.Println("")
	}
}

func main() {
	psqlconn := fmt.Sprintf("host=%s port=%d user=%s password=%s dbname=%s sslmode=disable", host, port, user, password, dbname)
	// open database
	db, err := sql.Open("postgres", psqlconn)
	if err != nil {
		panic(err)
	}

	defer db.Close()

	// check db
	err = db.Ping()
	if err != nil {
		panic(err)
	}

	stmt, err := db.Prepare("insert into k8s_events " +
		"(level, audit_id, stage, request_uri, verb, username, usergroups, source_ips, user_agent, request_time, " +
		"pod_name, pod_namespace, container_name, container_image, container_arguments, container_exec_command, " +
		"stdout, stderr, raw_object_ref, raw_request_object, raw_response_object) " +
		"values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21)")
	if err != nil {
		panic(err)
	}

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		defer r.Body.Close()
		body, err := ioutil.ReadAll(r.Body)
		if err != nil {
			http.Error(w, "can't read body", http.StatusBadRequest)
			return
		}

		var events v1.EventList
		err = json.Unmarshal(body, &events)
		if err != nil {
			http.Error(w, "failed to unmarshal audit events", http.StatusBadRequest)
			return
		}

		for _, event := range events.Items {
			prettyPrint(event, stmt)
		}
	})
	fmt.Printf("Server listening on port 8080\n")
	panic(http.ListenAndServe(":8080", nil))
}
