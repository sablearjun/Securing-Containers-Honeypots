import pandas as pd
import json
import DB.db_evn as DB
from EventRules.plugin_main import ProcessEventProcessor
from time import sleep

conn = DB.Connect()


def Engine():
    # def get_rows_from_df(bpf_df):
    while True:
        # Fetching from database
        cursor = conn.cursor()
        cursor.execute("Select * FROM bpf_process_events LIMIT 0")
        colnames = [desc[0] for desc in cursor.description]

        sql = """Select * from bpf_process_events
                    where (is_processed_py is not true)
                    order by id asc limit 100"""

        cursor.execute(sql)

        results = cursor.fetchall()
        bpf_process_df = pd.DataFrame(results, columns=colnames)
        conn.commit()

        # Enine Code Start here
        global_dict = {}
        for j, row in bpf_process_df.iterrows():
            local_dict = {}
            row_dict = dict(row)

            process_tree = row_dict["process_tree"]
            local_dict["row_data"] = row_dict

            try:
                local_dict["process_tree"] = json.loads(process_tree)
            except:
                local_dict["process_tree"] = {}
            global_dict[row["id"]] = local_dict
            record_id = 0
            record_id = local_dict["row_data"]["id"]
            event_id = 0
            event_id = local_dict["row_data"]["id"]

            for f, value_list in ProcessEventProcessor("Linux").items():
                if len(value_list) == 4:
                    boolean, string = f(local_dict)
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f.__name__
                        print(code)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        path = local_dict["row_data"]["path"]
                        cwd = local_dict["row_data"]["cwd"]
                        duration = local_dict["row_data"]["duration"]
                        cmdline = local_dict["row_data"]["cmdline"]
                        eid = local_dict["row_data"]["eid"]
                        md5 = local_dict["row_data"]["md5"]
                        sha1 = local_dict["row_data"]["sha1"]
                        sha256 = local_dict["row_data"]["sha256"]
                        
                        is_container_process = local_dict["row_data"]["is_container_process"]
                        container_age = local_dict["row_data"]["container_age"]
                        container_name = local_dict["row_data"]["container_name"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "extra_metadata": string,
                            "parents": a,
                            "tid": tid,
                            "pid": pid,
                            "parent": parent,
                            "uid": uid,
                            "gid": gid,
                            "cid": cid,
                            "exit_code": exit_code,
                            "probe_error": probe_error,
                            "syscall": syscall,
                            "path": path,
                            "cwd": cwd,
                            "cmdline": cmdline,
                            "eid": eid,
                            "md5": md5,
                            "sha1": sha1,
                            "sha256": sha256,
                            "is_container_process": is_container_process,
                            "container_age": container_age,
                            "container_name": container_name,
                            "event_id": event_id,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                    severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                    (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Process Alert', %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()
                elif len(value_list) == 7:
                    try:
                        func = json.loads(value_list[4])
                    except:
                        func = value_list[4]
                    exec(func)
                    sleep(2)
                    boolean, string = eval(f + "(local_dict)")
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f
                        print(code)
                        print(os_name)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        tid = local_dict["row_data"]["tid"]
                        pid = local_dict["row_data"]["pid"]
                        parent = local_dict["row_data"]["parent"]
                        uid = local_dict["row_data"]["uid"]
                        gid = local_dict["row_data"]["gid"]
                        cid = local_dict["row_data"]["cid"]
                        exit_code = local_dict["row_data"]["exit_code"]
                        probe_error = local_dict["row_data"]["probe_error"]
                        syscall = local_dict["row_data"]["syscall"]
                        path = local_dict["row_data"]["path"]
                        cwd = local_dict["row_data"]["cwd"]
                        duration = local_dict["row_data"]["duration"]
                        cmdline = local_dict["row_data"]["cmdline"]
                        eid = local_dict["row_data"]["eid"]
                        md5 = local_dict["row_data"]["md5"]
                        sha1 = local_dict["row_data"]["sha1"]
                        sha256 = local_dict["row_data"]["sha256"]
                        is_container_process = local_dict["row_data"]["is_container_process"]
                        container_age = local_dict["row_data"]["container_age"]
                        container_name = local_dict["row_data"]["container_name"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "extra_metadata": string,
                            "parents": a,
                            "tid": tid,
                            "pid": pid,
                            "parent": parent,
                            "uid": uid,
                            "gid": gid,
                            "cid": cid,
                            "exit_code": exit_code,
                            "probe_error": probe_error,
                            "syscall": syscall,
                            "path": path,
                            "cwd": cwd,
                            "cmdline": cmdline,
                            "eid": eid,
                            "md5": md5,
                            "sha1": sha1,
                            "sha256": sha256,
                            "is_container_process": is_container_process,
                            "container_age": container_age,
                            "container_name": container_name,
                            "event_id": event_id,
                        }
                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                    severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                    (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Process Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()

            cursor = conn.cursor()
            cursor.execute("Select * FROM bpf_process_events LIMIT 0")
            colnames = [desc[0] for desc in cursor.description]
            cursor.execute(
                "update bpf_process_events set is_processed_py = true where id = %s",
                [record_id],
            )

            conn.commit()


def soc_Engine():
    # def get_rows_from_df(bpf_df):
    while True:
        # Fetching from database
        cursor = conn.cursor()
        cursor.execute("Select * FROM bpf_socket_events LIMIT 0")
        colnames = [desc[0] for desc in cursor.description]

        sql = """Select * from bpf_socket_events
                where (is_processed_py is not true)
                order by id asc limit 100"""

        cursor.execute(sql)

        results = cursor.fetchall()
        bpf_socket_df = pd.DataFrame(results, columns=colnames)
        conn.commit()

        # Enine Code Start here
        global_dict = {}
        for j, row in bpf_socket_df.iterrows():
            local_dict = {}
            row_dict = dict(row)

            process_tree = row_dict["process_tree"]
            local_dict["row_data"] = row_dict

            try:
                local_dict["process_tree"] = json.loads(process_tree)
            except:
                local_dict["process_tree"] = {}
            global_dict[row["id"]] = local_dict
            record_id = 0
            record_id = local_dict["row_data"]["id"]
            event_id = 0
            event_id = local_dict["row_data"]["id"]

            for f, value_list in ProcessEventProcessor("Linux").items():
                if len(value_list) == 4:
                    boolean, string = f(local_dict)
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f.__name__
                        print(code)
                        print(os_name)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        path = local_dict["row_data"]["path"]
                        uid = local_dict["row_data"]["uid"]
                        gid = local_dict["row_data"]["gid"]
                        cid = local_dict["row_data"]["cid"]
                        exit_code = local_dict["row_data"]["exit_code"]
                        probe_error = local_dict["row_data"]["probe_error"]
                        parent = local_dict["row_data"]["parent"]
                        pid = local_dict["row_data"]["pid"]
                        syscall = local_dict["row_data"]["syscall"]
                        fd = local_dict["row_data"]["fd"]
                        family = local_dict["row_data"]["family"]
                        type = local_dict["row_data"]["type"]
                        protocol = local_dict["row_data"]["protocol"]
                        local_address = local_dict["row_data"]["local_address"]
                        remote_address = local_dict["row_data"]["remote_address"]
                        local_port = local_dict["row_data"]["local_port"]
                        remote_port = local_dict["row_data"]["remote_port"]
                        duration = local_dict["row_data"]["duration"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "path": path,
                            "extra_metadata": string,
                            "parents": a,
                            "parent": parent,
                            "pid": pid,
                            "syscall": syscall,
                            "uid": uid,
                            "gid": gid,
                            "cid": cid,
                            "exit_code": exit_code,
                            "probe_error": probe_error,
                            "fd": fd,
                            "family": family,
                            "type": type,
                            "protocol": protocol,
                            "local_address": local_address,
                            "remote_address": remote_address,
                            "local_port": local_port,
                            "remote_port": remote_port,
                            "duration": duration,
                        }
                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Socket Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()
                elif len(value_list) == 7:
                    try:
                        func = json.loads(value_list[4])
                    except:
                        func = value_list[4]
                    exec(func)
                    sleep(2)
                    boolean, string = eval(f + "(local_dict)")
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f
                        print(code)
                        print(os_name)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        path = local_dict["row_data"]["path"]
                        uid = local_dict["row_data"]["uid"]
                        gid = local_dict["row_data"]["gid"]
                        cid = local_dict["row_data"]["cid"]
                        exit_code = local_dict["row_data"]["exit_code"]
                        probe_error = local_dict["row_data"]["probe_error"]
                        parent = local_dict["row_data"]["parent"]
                        pid = local_dict["row_data"]["pid"]
                        syscall = local_dict["row_data"]["syscall"]
                        fd = local_dict["row_data"]["fd"]
                        family = local_dict["row_data"]["family"]
                        type = local_dict["row_data"]["type"]
                        protocol = local_dict["row_data"]["protocol"]
                        local_address = local_dict["row_data"]["local_address"]
                        remote_address = local_dict["row_data"]["remote_address"]
                        local_port = local_dict["row_data"]["local_port"]
                        remote_port = local_dict["row_data"]["remote_port"]
                        duration = local_dict["row_data"]["duration"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "path": path,
                            "extra_metadata": string,
                            "parents": a,
                            "parent": parent,
                            "pid": pid,
                            "syscall": syscall,
                            "uid": uid,
                            "gid": gid,
                            "cid": cid,
                            "exit_code": exit_code,
                            "probe_error": probe_error,
                            "fd": fd,
                            "family": family,
                            "type": type,
                            "protocol": protocol,
                            "local_address": local_address,
                            "remote_address": remote_address,
                            "local_port": local_port,
                            "remote_port": remote_port,
                            "duration": duration,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Socket Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()

            cursor = conn.cursor()
            cursor.execute("Select * FROM bpf_socket_events LIMIT 0")
            colnames = [desc[0] for desc in cursor.description]
            cursor.execute(
                "update bpf_socket_events set is_processed_py = true where id = %s",
                [record_id],
            )

            conn.commit()


def file_Engine():
    # def get_rows_from_df(bpf_df):
    while True:
        # Fetching from database
        cursor = conn.cursor()
        cursor.execute("Select * FROM bpf_file_events LIMIT 0")
        colnames = [desc[0] for desc in cursor.description]

        sql = """Select * from bpf_file_events
                where (is_processed_py is not true)
                order by id asc limit 100"""

        cursor.execute(sql)

        results = cursor.fetchall()
        bpf_file_df = pd.DataFrame(results, columns=colnames)
        conn.commit()

        # Enine Code Start here
        global_dict = {}
        for j, row in bpf_file_df.iterrows():
            local_dict = {}
            row_dict = dict(row)

            process_tree = row_dict["process_tree"]
            local_dict["row_data"] = row_dict

            try:
                local_dict["process_tree"] = json.loads(process_tree)
            except:
                local_dict["process_tree"] = {}
            global_dict[row["id"]] = local_dict
            record_id = 0
            record_id = local_dict["row_data"]["id"]
            event_id =0
            event_id = local_dict["row_data"]["id"]

            for f, value_list in ProcessEventProcessor("Linux").items():
                if len(value_list) == 4:
                    boolean, string = f(local_dict)
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f.__name__
                        print(code)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        tid = local_dict["row_data"]["tid"]
                        pid = local_dict["row_data"]["pid"]
                        parent = local_dict["row_data"]["parent"]
                        uid = local_dict["row_data"]["uid"]
                        gid = local_dict["row_data"]["gid"]
                        cid = local_dict["row_data"]["cid"]
                        exit_code = local_dict["row_data"]["exit_code"]
                        probe_error = local_dict["row_data"]["probe_error"]
                        syscall = local_dict["row_data"]["syscall"]
                        path = local_dict["row_data"]["path"]
                        file_path = local_dict["row_data"]["file_path"]
                        flags = local_dict["row_data"]["flags"]
                        duration = local_dict["row_data"]["duration"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "extra_metadata": string,
                            "parents": a,
                            "tid": tid,
                            "pid": pid,
                            "parent": parent,
                            "uid": uid,
                            "gid": gid,
                            "cid": cid,
                            "exit_code": exit_code,
                            "probe_error": probe_error,
                            "syscall": syscall,
                            "path": path,
                            "file_path": file_path,
                            "flags": flags,
                            "duration": duration,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'File Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()
                elif len(value_list) == 7:
                    try:
                        func = json.loads(value_list[4])
                    except:
                        func = value_list[4]
                    exec(func)
                    sleep(2)
                    boolean, string = eval(f + "(local_dict)")
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f
                        print(code)
                        print(os_name)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        tid = local_dict["row_data"]["tid"]
                        pid = local_dict["row_data"]["pid"]
                        parent = local_dict["row_data"]["parent"]
                        uid = local_dict["row_data"]["uid"]
                        gid = local_dict["row_data"]["gid"]
                        cid = local_dict["row_data"]["cid"]
                        exit_code = local_dict["row_data"]["exit_code"]
                        probe_error = local_dict["row_data"]["probe_error"]
                        syscall = local_dict["row_data"]["syscall"]
                        path = local_dict["row_data"]["path"]
                        file_path = local_dict["row_data"]["file_path"]
                        flags = local_dict["row_data"]["flags"]
                        duration = local_dict["row_data"]["duration"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "extra_metadata": string,
                            "parents": a,
                            "tid": tid,
                            "pid": pid,
                            "parent": parent,
                            "uid": uid,
                            "gid": gid,
                            "cid": cid,
                            "exit_code": exit_code,
                            "probe_error": probe_error,
                            "syscall": syscall,
                            "path": path,
                            "file_path": file_path,
                            "flags": flags,
                            "duration": duration,
                        }
                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values 
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'File Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()

            cursor = conn.cursor()
            cursor.execute("Select * FROM bpf_file_events LIMIT 0")
            colnames = [desc[0] for desc in cursor.description]
            cursor.execute(
                "update bpf_file_events set is_processed_py = true where id = %s",
                [record_id],
            )

            conn.commit()


def win_process_Engine():
    # def get_rows_from_df(bpf_df):
    while True:
        # Fetching from database
        cursor = conn.cursor()
        cursor.execute("Select * FROM win_process_events LIMIT 0")
        colnames = [desc[0] for desc in cursor.description]

        sql = """Select * FROM win_process_events   
                where (is_processed_py is not true)
                order by id asc limit 100"""

        cursor.execute(sql)

        results = cursor.fetchall()
        win_process_df = pd.DataFrame(results, columns=colnames)
        conn.commit()

        # Enine Code Start here
        global_dict = {}
        for j, row in win_process_df.iterrows():
            local_dict = {}
            row_dict = dict(row)

            process_tree = row_dict["process_tree"]
            local_dict["row_data"] = row_dict

            try:
                local_dict["process_tree"] = json.loads(process_tree)
            except:
                local_dict["process_tree"] = {}
            global_dict[row["id"]] = local_dict
            record_id = 0
            record_id = local_dict["row_data"]["id"]
            event_id = 0
            event_id = local_dict["row_data"]["id"]

            for f, value_list in ProcessEventProcessor("Windows").items():
                if len(value_list) == 4:
                    boolean, string = f(local_dict)
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f.__name__
                        print(code)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        action = local_dict["row_data"]["action"]
                        pid = local_dict["row_data"]["pid"]
                        process_guid = local_dict["row_data"]["process_guid"]
                        path = local_dict["row_data"]["path"]
                        cmdline = local_dict["row_data"]["cmdline"]
                        parent_pid = local_dict["row_data"]["parent_pid"]
                        parent_process_guid = local_dict["row_data"][
                            "parent_process_guid"
                        ]
                        parent_path = local_dict["row_data"]["parent_path"]
                        owner_uid = local_dict["row_data"]["owner_uid"]
                        eventid = local_dict["row_data"]["eventid"]
                        sha256 = local_dict["row_data"]["sha256"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "action": action,
                            "pid": pid,
                            "process_guid": process_guid,
                            "path": path,
                            "cmdline": cmdline,
                            "parent_pid": parent_pid,
                            "parent_process_guid": parent_process_guid,
                            "parent_path": parent_path,
                            "owner_uid": owner_uid,
                            "eventid": eventid,
                            "sha256": sha256,
                            "extra_metadata": string,
                            "parents": a,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Process Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()
                elif len(value_list) == 7:
                    try:
                        func = json.loads(value_list[4])
                    except:
                        func = value_list[4]
                    exec(func)
                    sleep(2)
                    boolean, string = eval(f + "(local_dict)")
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f
                        print(code)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        action = local_dict["row_data"]["action"]
                        pid = local_dict["row_data"]["pid"]
                        process_guid = local_dict["row_data"]["process_guid"]
                        path = local_dict["row_data"]["path"]
                        cmdline = local_dict["row_data"]["cmdline"]
                        parent_pid = local_dict["row_data"]["parent_pid"]
                        parent_process_guid = local_dict["row_data"][
                            "parent_process_guid"
                        ]
                        parent_path = local_dict["row_data"]["parent_path"]
                        owner_uid = local_dict["row_data"]["owner_uid"]
                        eventid = local_dict["row_data"]["eventid"]
                        sha256 = local_dict["row_data"]["sha256"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "action": action,
                            "pid": pid,
                            "process_guid": process_guid,
                            "path": path,
                            "cmdline": cmdline,
                            "parent_pid": parent_pid,
                            "parent_process_guid": parent_process_guid,
                            "parent_path": parent_path,
                            "owner_uid": owner_uid,
                            "eventid": eventid,
                            "sha256": sha256,
                            "extra_metadata": string,
                            "parents": a,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Process Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()

            cursor = conn.cursor()
            cursor.execute("Select * FROM win_process_events LIMIT 0")
            colnames = [desc[0] for desc in cursor.description]
            cursor.execute(
                "update win_process_events set is_processed_py = true where id = %s",
                [record_id],
            )

            conn.commit()


def win_socket_Engine():
    # def get_rows_from_df(bpf_df):
    while True:
        # Fetching from database
        cursor = conn.cursor()
        cursor.execute("Select * FROM win_socket_events LIMIT 0")
        colnames = [desc[0] for desc in cursor.description]

        sql = """Select * from win_socket_events
                where (is_processed_py is not true)
                order by id asc limit 100"""

        cursor.execute(sql)

        results = cursor.fetchall()
        win_socket_df = pd.DataFrame(results, columns=colnames)
        conn.commit()

        # Enine Code Start here
        global_dict = {}
        for j, row in win_socket_df.iterrows():
            local_dict = {}
            row_dict = dict(row)

            process_tree = row_dict["process_tree"]
            local_dict["row_data"] = row_dict

            try:
                local_dict["process_tree"] = json.loads(process_tree)
            except:
                local_dict["process_tree"] = {}
            global_dict[row["id"]] = local_dict
            record_id = 0
            record_id = local_dict["row_data"]["id"]
            event_id = 0
            event_id = local_dict["row_data"]["id"]

            for f, value_list in ProcessEventProcessor("Windows").items():
                if len(value_list) == 4:
                    boolean, string = f(local_dict)
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f.__name__
                        print(code)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        action = local_dict["row_data"]["action"]
                        eid = local_dict["row_data"]["eid"]
                        time = local_dict["row_data"]["time"]
                        utc_time = local_dict["row_data"]["utc_time"]
                        pid = local_dict["row_data"]["pid"]
                        process_guid = local_dict["row_data"]["process_guid"]
                        process_name = local_dict["row_data"]["process_name"]
                        family = local_dict["row_data"]["family"]
                        protocol = local_dict["row_data"]["protocol"]
                        local_address = local_dict["row_data"]["local_address"]
                        remote_address = local_dict["row_data"]["remote_address"]
                        local_port = local_dict["row_data"]["local_port"]
                        remote_port = local_dict["row_data"]["remote_port"]
                        eventid = local_dict["row_data"]["eventid"]
                        parent_pid = local_dict["row_data"]["parent_pid"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "action": action,
                            "eid": eid,
                            "time": time,
                            "utc_time": utc_time,
                            "pid": pid,
                            "process_guid": process_guid,
                            "process_name": process_name,
                            "family": family,
                            "protocol": protocol,
                            "local_address": local_address,
                            "remote_address": remote_address,
                            "local_port": local_port,
                            "remote_port": remote_port,
                            "eventid": eventid,
                            "parent_pid": parent_pid,
                            "extra_metadata": string,
                            "parents": a,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Socket Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()
                elif len(value_list) == 7:
                    try:
                        func = json.loads(value_list[4])
                    except:
                        func = value_list[4]
                    exec(func)
                    sleep(2)
                    boolean, string = eval(f + "(local_dict)")
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f
                        print(code)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        action = local_dict["row_data"]["action"]
                        eid = local_dict["row_data"]["eid"]
                        time = local_dict["row_data"]["time"]
                        utc_time = local_dict["row_data"]["utc_time"]
                        pid = local_dict["row_data"]["pid"]
                        process_guid = local_dict["row_data"]["process_guid"]
                        process_name = local_dict["row_data"]["process_name"]
                        family = local_dict["row_data"]["family"]
                        protocol = local_dict["row_data"]["protocol"]
                        local_address = local_dict["row_data"]["local_address"]
                        remote_address = local_dict["row_data"]["remote_address"]
                        local_port = local_dict["row_data"]["local_port"]
                        remote_port = local_dict["row_data"]["remote_port"]
                        eventid = local_dict["row_data"]["eventid"]
                        parent_pid = local_dict["row_data"]["parent_pid"]

                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "action": action,
                            "eid": eid,
                            "time": time,
                            "utc_time": utc_time,
                            "pid": pid,
                            "process_guid": process_guid,
                            "process_name": process_name,
                            "family": family,
                            "protocol": protocol,
                            "local_address": local_address,
                            "remote_address": remote_address,
                            "local_port": local_port,
                            "remote_port": remote_port,
                            "eventid": eventid,
                            "parent_pid": parent_pid,
                            "extra_metadata": string,
                            "parents": a,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Socket Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()

            cursor = conn.cursor()
            cursor.execute("Select * FROM win_socket_events LIMIT 0")
            colnames = [desc[0] for desc in cursor.description]
            cursor.execute(
                "update win_socket_events set is_processed_py = true where id = %s",
                [record_id],
            )

            conn.commit()


def win_file_Engine():
    # def get_rows_from_df(bpf_df):
    while True:
        # Fetching from database
        cursor = conn.cursor()
        cursor.execute("Select * FROM win_file_events LIMIT 0")
        colnames = [desc[0] for desc in cursor.description]

        sql = """Select * from win_file_events
                where (is_processed_py is not true)
                order by id asc limit 100"""

        cursor.execute(sql)

        results = cursor.fetchall()
        win_file_df = pd.DataFrame(results, columns=colnames)
        conn.commit()

        # Enine Code Start here
        global_dict = {}
        for j, row in win_file_df.iterrows():
            local_dict = {}
            row_dict = dict(row)

            process_tree = row_dict["process_tree"]
            local_dict["row_data"] = row_dict

            try:
                local_dict["process_tree"] = json.loads(process_tree)
            except:
                local_dict["process_tree"] = {}
            global_dict[row["id"]] = local_dict
            record_id = 0
            record_id = local_dict["row_data"]["id"]
            event_id = 0
            event_id = local_dict["row_data"]["id"]

            for f, value_list in ProcessEventProcessor("Windows").items():
                if len(value_list) == 4:
                    boolean, string = f(local_dict)
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f.__name__
                        print(code)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        action = local_dict["row_data"]["action"]
                        eid = local_dict["row_data"]["eid"]
                        target_path = local_dict["row_data"]["target_path"]
                        md5 = local_dict["row_data"]["md5"]
                        sha256 = local_dict["row_data"]["sha256"]
                        hashed = local_dict["row_data"]["hashed"]
                        uid = local_dict["row_data"]["uid"]
                        utc_time = local_dict["row_data"]["utc_time"]
                        time = local_dict["row_data"]["time"]
                        pe_file = local_dict["row_data"]["pe_file"]
                        pid = local_dict["row_data"]["pid"]
                        process_guid = local_dict["row_data"]["process_guid"]
                        process_name = local_dict["row_data"]["process_name"]
                        eventid = local_dict["row_data"]["eventid"]
                        parent_pid = local_dict["row_data"]["parent_pid"]
                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "eid": eid,
                            "target_path": target_path,
                            "md5": md5,
                            "sha256": sha256,
                            "hashed": hashed,
                            "uid": uid,
                            "utc_time": utc_time,
                            "time": time,
                            "pe_file": pe_file,
                            "pid": pid,
                            "process_guid": process_guid,
                            "action": action,
                            "eventid": eventid,
                            "parent_pid": parent_pid,
                            "process_name": process_name,
                            "extra_metadata": string,
                            "parents": a,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'File Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()
                elif len(value_list) == 7:
                    try:
                        func = json.loads(value_list[4])
                    except:
                        func = value_list[4]
                    exec(func)
                    sleep(2)
                    boolean, string = eval(f + "(local_dict)")
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        event_pid = local_dict["row_data"]["pid"]
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f
                        print(code)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        action = local_dict["row_data"]["action"]
                        eid = local_dict["row_data"]["eid"]
                        target_path = local_dict["row_data"]["target_path"]
                        md5 = local_dict["row_data"]["md5"]
                        sha256 = local_dict["row_data"]["sha256"]
                        hashed = local_dict["row_data"]["hashed"]
                        uid = local_dict["row_data"]["uid"]
                        utc_time = local_dict["row_data"]["utc_time"]
                        time = local_dict["row_data"]["time"]
                        pe_file = local_dict["row_data"]["pe_file"]
                        pid = local_dict["row_data"]["pid"]
                        process_guid = local_dict["row_data"]["process_guid"]
                        process_name = local_dict["row_data"]["process_name"]
                        eventid = local_dict["row_data"]["eventid"]
                        parent_pid = local_dict["row_data"]["parent_pid"]
                        a = local_dict["process_tree"]
                        event_metadata = {
                            "unixtime": unixtime,
                            "eid": eid,
                            "target_path": target_path,
                            "md5": md5,
                            "sha256": sha256,
                            "hashed": hashed,
                            "uid": uid,
                            "utc_time": utc_time,
                            "time": time,
                            "pe_file": pe_file,
                            "pid": pid,
                            "process_guid": process_guid,
                            "action": action,
                            "eventid": eventid,
                            "parent_pid": parent_pid,
                            "process_name": process_name,
                            "extra_metadata": string,
                            "parents": a,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'File Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()

            cursor = conn.cursor()
            cursor.execute("Select * FROM win_file_events LIMIT 0")
            colnames = [desc[0] for desc in cursor.description]
            cursor.execute(
                "update win_file_events set is_processed_py = true where id = %s",
                [record_id],
            )

            conn.commit()


def chrome_ext_Engine():
    # def get_rows_from_df(bpf_df):
    while True:
        # Fetching from database
        cursor = conn.cursor()
        cursor.execute("Select * FROM chrome_extensions LIMIT 0")
        colnames = [desc[0] for desc in cursor.description]

        sql = """Select * FROM chrome_extensions   
                where (is_processed_py is not true)
                order by id asc limit 100"""

        cursor.execute(sql)

        results = cursor.fetchall()
        chrome_ext_df = pd.DataFrame(results, columns=colnames)
        conn.commit()

        # Enine Code Start here
        global_dict = {}
        for j, row in chrome_ext_df.iterrows():
            local_dict = {}
            row_dict = dict(row)

            local_dict["row_data"] = row_dict

            global_dict[row["id"]] = local_dict
            record_id = 0
            record_id = local_dict["row_data"]["id"]
            event_id = 0
            event_id = local_dict["row_data"]["id"]

            rule_list = []
            rule_list.append(ProcessEventProcessor("Linux"))
            rule_list.append(ProcessEventProcessor("Windows"))
            for processor in rule_list:
                for f, value_list in processor.items():
                    if len(value_list) == 4:
                        boolean, string = f(local_dict)
                        if boolean == True:
                            print(record_id)
                            isAlert = True
                            severity = value_list[0]
                            technique = value_list[1]
                            technique_id = value_list[2]
                            link = value_list[3]
                            os_name = value_list[6]                        
                            code = f.__name__
                            print(code)
                            host_identifier = local_dict["row_data"]["host_identifier"]
                            hostname = local_dict["row_data"]["hostname"]
                            unixtime = local_dict["row_data"]["unixtime"]
                            browser_type = local_dict["row_data"]["browser_type"]
                            uid = local_dict["row_data"]["uid"]
                            name = local_dict["row_data"]["name"]
                            profile = local_dict["row_data"]["profile"]
                            profile_path = local_dict["row_data"]["profile_path"]
                            referenced_identifier = local_dict["row_data"][
                                "referenced_identifier"
                            ]
                            identifier = local_dict["row_data"]["identifier"]
                            version = local_dict["row_data"]["version"]
                            description = local_dict["row_data"]["description"]
                            default_locale = local_dict["row_data"]["default_locale"]
                            current_locale = local_dict["row_data"]["current_locale"]
                            update_url = local_dict["row_data"]["update_url"]
                            author = local_dict["row_data"]["author"]
                            persistent = local_dict["row_data"]["persistent"]
                            path = local_dict["row_data"]["path"]
                            permissions = local_dict["row_data"]["permissions"]
                            optional_permissions = local_dict["row_data"][
                                "optional_permissions"
                            ]
                            manifest_hash = local_dict["row_data"]["manifest_hash"]
                            referenced = local_dict["row_data"]["referenced"]
                            from_webstore = local_dict["row_data"]["from_webstore"]
                            state = local_dict["row_data"]["state"]
                            install_time = local_dict["row_data"]["install_time"]
                            install_timestamp = local_dict["row_data"][
                                "install_timestamp"
                            ]
                            key = local_dict["row_data"]["key"]

                            event_metadata = {
                                "unixtime": unixtime,
                                "browser_type": browser_type,
                                "uid": uid,
                                "name": name,
                                "profile": profile,
                                "profile_path": profile_path,
                                "referenced_identifier": referenced_identifier,
                                "identifier": identifier,
                                "version": version,
                                "description": description,
                                "default_locale": default_locale,
                                "current_locale": current_locale,
                                "update_url": update_url,
                                "author": author,
                                "persistent": persistent,
                                "path": path,
                                "permissions": permissions,
                                "optional_permissions": optional_permissions,
                                "manifest_hash": manifest_hash,
                                "referenced": referenced,
                                "from_webstore": from_webstore,
                                "state": state,
                                "install_time": install_time,
                                "install_timestamp": install_timestamp,
                                "key": key,
                                "extra_metadata": string,
                            }

                            cursor = conn.cursor()
                            cursor.execute("Select * FROM alerts LIMIT 0")
                            colnames = [desc[0] for desc in cursor.description]

                            sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                    severity, is_open, alert_type, technique, technique_id, link, event_id, os_name) values
                                    (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Other Alert', %s, %s, %s, %s, %s)"""
                            insert_sql = (
                                host_identifier,
                                hostname,
                                "",
                                unixtime,
                                code,
                                json.dumps(event_metadata),
                                isAlert,
                                severity,
                                technique,
                                technique_id,
                                link,
                                event_id,
                                os_name,
                            )
                            cursor.execute(sql, insert_sql)
                            conn.commit()
                    elif len(value_list) == 7:
                        try:
                            func = json.loads(value_list[4])
                        except:
                            func = value_list[4]
                        if "process_tree" in func:
                            break
                        else:
                            exec(func)
                    sleep(2)
                    boolean, string = eval(f + "(local_dict)")
                    if boolean == True:
                        print(record_id)
                        isAlert = True
                        severity = value_list[0]
                        technique = value_list[1]
                        technique_id = value_list[2]
                        link = value_list[3]
                        os_name = value_list[6]
                        code = f
                        print(code)
                        host_identifier = local_dict["row_data"]["host_identifier"]
                        hostname = local_dict["row_data"]["hostname"]
                        unixtime = local_dict["row_data"]["unixtime"]
                        browser_type = local_dict["row_data"]["browser_type"]
                        uid = local_dict["row_data"]["uid"]
                        name = local_dict["row_data"]["name"]
                        profile = local_dict["row_data"]["profile"]
                        profile_path = local_dict["row_data"]["profile_path"]
                        referenced_identifier = local_dict["row_data"][
                            "referenced_identifier"
                        ]
                        identifier = local_dict["row_data"]["identifier"]
                        version = local_dict["row_data"]["version"]
                        description = local_dict["row_data"]["description"]
                        default_locale = local_dict["row_data"]["default_locale"]
                        current_locale = local_dict["row_data"]["current_locale"]
                        update_url = local_dict["row_data"]["update_url"]
                        author = local_dict["row_data"]["author"]
                        persistent = local_dict["row_data"]["persistent"]
                        path = local_dict["row_data"]["path"]
                        permissions = local_dict["row_data"]["permissions"]
                        optional_permissions = local_dict["row_data"][
                            "optional_permissions"
                        ]
                        manifest_hash = local_dict["row_data"]["manifest_hash"]
                        referenced = local_dict["row_data"]["referenced"]
                        from_webstore = local_dict["row_data"]["from_webstore"]
                        state = local_dict["row_data"]["state"]
                        install_time = local_dict["row_data"]["install_time"]
                        install_timestamp = local_dict["row_data"]["install_timestamp"]
                        key = local_dict["row_data"]["key"]

                        event_metadata = {
                            "unixtime": unixtime,
                            "browser_type": browser_type,
                            "uid": uid,
                            "name": name,
                            "profile": profile,
                            "profile_path": profile_path,
                            "referenced_identifier": referenced_identifier,
                            "identifier": identifier,
                            "version": version,
                            "description": description,
                            "default_locale": default_locale,
                            "current_locale": current_locale,
                            "update_url": update_url,
                            "author": author,
                            "persistent": persistent,
                            "path": path,
                            "permissions": permissions,
                            "optional_permissions": optional_permissions,
                            "manifest_hash": manifest_hash,
                            "referenced": referenced,
                            "from_webstore": from_webstore,
                            "state": state,
                            "install_time": install_time,
                            "install_timestamp": install_timestamp,
                            "key": key,
                            "extra_metadata": string,
                        }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Other Alert', %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()

                cursor = conn.cursor()
                cursor.execute("Select * FROM chrome_extensions LIMIT 0")
                colnames = [desc[0] for desc in cursor.description]
                cursor.execute(
                    "update chrome_extensions set is_processed_py = true where id = %s",
                    [record_id],
                )

                conn.commit()


def processes_Engine():
    # def get_rows_from_df(bpf_df):
    while True:
        # Fetching from database
        cursor = conn.cursor()
        cursor.execute("Select * FROM processes LIMIT 0")
        colnames = [desc[0] for desc in cursor.description]

        sql = """Select * FROM processes   
                where (is_processed_py is not true)
                order by id asc limit 100"""

        cursor.execute(sql)

        results = cursor.fetchall()
        processes_df = pd.DataFrame(results, columns=colnames)
        conn.commit()

        # Enine Code Start here
        global_dict = {}
        for j, row in processes_df.iterrows():
            local_dict = {}
            row_dict = dict(row)

            local_dict["row_data"] = row_dict

            global_dict[row["id"]] = local_dict
            record_id = 0
            record_id = local_dict["row_data"]["id"]
            event_id = 0
            event_id = local_dict["row_data"]["id"]

            rule_list = []
            rule_list.append(ProcessEventProcessor("Linux"))
            rule_list.append(ProcessEventProcessor("Windows"))
            for processor in rule_list:
                for f, value_list in processor.items():
                    if len(value_list) == 4:
                        boolean, string = f(local_dict)
                        if boolean == True:
                            print(record_id)
                            isAlert = True
                            event_pid = local_dict["row_data"]["pid"]
                            severity = value_list[0]
                            technique = value_list[1]
                            technique_id = value_list[2]
                            link = value_list[3]
                            os_name = value_list[6]
                            code = f.__name__
                            print(code)
                            host_identifier = local_dict["row_data"]["host_identifier"]
                            hostname = local_dict["row_data"]["hostname"]
                            unixtime = local_dict["row_data"]["unixtime"]
                            pid = local_dict["row_data"]["pid"]
                            name = local_dict["row_data"]["name"]
                            name = local_dict["row_data"]["name"]
                            path = local_dict["row_data"]["path"]
                            cmdline = local_dict["row_data"]["cmdline"]
                            state = local_dict["row_data"]["state"]
                            cwd = local_dict["row_data"]["cwd"]
                            root = local_dict["row_data"]["root"]
                            uid = local_dict["row_data"]["uid"]
                            gid = local_dict["row_data"]["gid"]
                            egid = local_dict["row_data"]["egid"]
                            suid = local_dict["row_data"]["suid"]
                            sgid = local_dict["row_data"]["sgid"]
                            on_disk = local_dict["row_data"]["on_disk"]
                            wired_size = local_dict["row_data"]["wired_size"]
                            resident_size = local_dict["row_data"]["resident_size"]
                            total_size = local_dict["row_data"]["total_size"]
                            user_time = local_dict["row_data"]["user_time"]
                            system_time = local_dict["row_data"]["system_time"]
                            disk_bytes_read = local_dict["row_data"]["disk_bytes_read"]
                            disk_bytes_written = local_dict["row_data"][
                                "disk_bytes_written"
                            ]
                            start_time = local_dict["row_data"]["start_time"]
                            parent = local_dict["row_data"]["parent"]
                            pgroup = local_dict["row_data"]["pgroup"]
                            threads = local_dict["row_data"]["threads"]
                            nice = local_dict["row_data"]["nice"]
                            elevated_token = local_dict["row_data"]["elevated_token"]
                            secure_process = local_dict["row_data"]["secure_process"]
                            protection_type = local_dict["row_data"]["protection_type"]
                            virtual_process = local_dict["row_data"]["virtual_process"]
                            elapsed_time = local_dict["row_data"]["elapsed_time"]
                            handle_count = local_dict["row_data"]["handle_count"]
                            percent_processor_time = local_dict["row_data"][
                                "percent_processor_time"
                            ]

                            event_metadata = {
                                "unixtime": unixtime,
                                "pid": pid,
                                "name": name,
                                "path": path,
                                "cmdline": cmdline,
                                "state": state,
                                "cwd": cwd,
                                "root": root,
                                "uid": uid,
                                "gid": gid,
                                "egid": egid,
                                "suid": suid,
                                "sgid": sgid,
                                "on_disk": on_disk,
                                "wired_size": wired_size,
                                "resident_size": resident_size,
                                "total_size": total_size,
                                "user_time": user_time,
                                "system_time": system_time,
                                "disk_bytes_read": disk_bytes_read,
                                "disk_bytes_written": disk_bytes_written,
                                "start_time": start_time,
                                "parent": parent,
                                "pgroup": pgroup,
                                "threads": threads,
                                "nice": nice,
                                "elevated_token": elevated_token,
                                "secure_process": secure_process,
                                "protection_type": protection_type,
                                "virtual_process": virtual_process,
                                "elapsed_time": elapsed_time,
                                "handle_count": handle_count,
                                "percent_processor_time": percent_processor_time,
                                "extra_metadata": string,
                            }

                        cursor = conn.cursor()
                        cursor.execute("Select * FROM alerts LIMIT 0")
                        colnames = [desc[0] for desc in cursor.description]

                        sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Other Alert', %s, %s, %s, %s, %s, %s)"""
                        insert_sql = (
                            host_identifier,
                            hostname,
                            "",
                            unixtime,
                            code,
                            json.dumps(event_metadata),
                            isAlert,
                            severity,
                            technique,
                            technique_id,
                            link,
                            event_pid,
                            event_id,
                            os_name,
                        )
                        cursor.execute(sql, insert_sql)
                        conn.commit()
                    elif len(value_list) == 7:
                        try:
                            func = json.loads(value_list[4])
                        except:
                            func = value_list[4]
                        if "process_tree" in func:
                            break
                        else:
                            exec(func)
                        sleep(2)
                        boolean, string = eval(f + "(local_dict)")
                        if boolean == True:
                            print(record_id)
                            isAlert = True
                            event_pid = local_dict["row_data"]["pid"]
                            severity = value_list[0]
                            technique = value_list[1]
                            technique_id = value_list[2]
                            link = value_list[3]
                            os_name = value_list[6]
                            code = f
                            print(code)
                            host_identifier = local_dict["row_data"]["host_identifier"]
                            hostname = local_dict["row_data"]["hostname"]
                            unixtime = local_dict["row_data"]["unixtime"]
                            pid = local_dict["row_data"]["pid"]
                            name = local_dict["row_data"]["name"]
                            name = local_dict["row_data"]["name"]
                            path = local_dict["row_data"]["path"]
                            cmdline = local_dict["row_data"]["cmdline"]
                            state = local_dict["row_data"]["state"]
                            cwd = local_dict["row_data"]["cwd"]
                            root = local_dict["row_data"]["root"]
                            uid = local_dict["row_data"]["uid"]
                            gid = local_dict["row_data"]["gid"]
                            egid = local_dict["row_data"]["egid"]
                            suid = local_dict["row_data"]["suid"]
                            sgid = local_dict["row_data"]["sgid"]
                            on_disk = local_dict["row_data"]["on_disk"]
                            wired_size = local_dict["row_data"]["wired_size"]
                            resident_size = local_dict["row_data"]["resident_size"]
                            total_size = local_dict["row_data"]["total_size"]
                            user_time = local_dict["row_data"]["user_time"]
                            system_time = local_dict["row_data"]["system_time"]
                            disk_bytes_read = local_dict["row_data"]["disk_bytes_read"]
                            disk_bytes_written = local_dict["row_data"][
                                "disk_bytes_written"
                            ]
                            start_time = local_dict["row_data"]["start_time"]
                            parent = local_dict["row_data"]["parent"]
                            pgroup = local_dict["row_data"]["pgroup"]
                            threads = local_dict["row_data"]["threads"]
                            nice = local_dict["row_data"]["nice"]
                            elevated_token = local_dict["row_data"]["elevated_token"]
                            secure_process = local_dict["row_data"]["secure_process"]
                            protection_type = local_dict["row_data"]["protection_type"]
                            virtual_process = local_dict["row_data"]["virtual_process"]
                            elapsed_time = local_dict["row_data"]["elapsed_time"]
                            handle_count = local_dict["row_data"]["handle_count"]
                            percent_processor_time = local_dict["row_data"][
                                "percent_processor_time"
                            ]

                            event_metadata = {
                                "unixtime": unixtime,
                                "pid": pid,
                                "name": name,
                                "path": path,
                                "cmdline": cmdline,
                                "state": state,
                                "cwd": cwd,
                                "root": root,
                                "uid": uid,
                                "gid": gid,
                                "egid": egid,
                                "suid": suid,
                                "sgid": sgid,
                                "on_disk": on_disk,
                                "wired_size": wired_size,
                                "resident_size": resident_size,
                                "total_size": total_size,
                                "user_time": user_time,
                                "system_time": system_time,
                                "disk_bytes_read": disk_bytes_read,
                                "disk_bytes_written": disk_bytes_written,
                                "start_time": start_time,
                                "parent": parent,
                                "pgroup": pgroup,
                                "threads": threads,
                                "nice": nice,
                                "elevated_token": elevated_token,
                                "secure_process": secure_process,
                                "protection_type": protection_type,
                                "virtual_process": virtual_process,
                                "elapsed_time": elapsed_time,
                                "handle_count": handle_count,
                                "percent_processor_time": percent_processor_time,
                                "extra_metadata": string,
                            }

                            cursor = conn.cursor()
                            cursor.execute("Select * FROM alerts LIMIT 0")
                            colnames = [desc[0] for desc in cursor.description]

                            sql = """insert into alerts (host_identifier, hostname, context_data, unixtime, event_code, metadata, is_alert,
                                    severity, is_open, alert_type, technique, technique_id, link, pid, event_id, os_name) values
                                    (%s, %s, %s, %s, %s, %s, %s, %s, true, 'Other Alert', %s, %s, %s, %s, %s, %s)"""
                            insert_sql = (
                                host_identifier,
                                hostname,
                                "",
                                unixtime,
                                code,
                                json.dumps(event_metadata),
                                isAlert,
                                severity,
                                technique,
                                technique_id,
                                link,
                                event_pid,
                                event_id,
                                os_name,
                            )
                            cursor.execute(sql, insert_sql)
                            conn.commit()

                cursor = conn.cursor()
                cursor.execute("Select * FROM processes LIMIT 0")
                colnames = [desc[0] for desc in cursor.description]
                cursor.execute(
                    "update processes set is_processed_py = true where id = %s", [record_id]
                )

                conn.commit()
