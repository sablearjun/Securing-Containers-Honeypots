from django.http.response import JsonResponse
from django.shortcuts import render, redirect, resolve_url
from django.contrib.auth.models import User, auth
from django.contrib import messages
from django.http import Http404, HttpResponse,HttpRequest
from django.core.paginator import Paginator
import requests
import json
import ast
import time
from datetime import datetime
import requests
from requests.structures import CaseInsensitiveDict
import math

# Server settings :

# Jwt Token verification block

inspector = CaseInsensitiveDict()
inspector["Accept"] = "application/json"
inspector["Authorization"] = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJBcmp1biBTYWJsZSIsInRva2VuIjoicXdlcnR5IiwidXNlciI6InNhYmxlYXJqdW4ifQ.vkTPf7XV5BK2UBYCGm3TQDHhZ1zVe8Tan311LkMFBpo"

# Jwt Token endblock
# API URL
url = 'https://s3.ieor.iitb.ac.in:1235/'


# Local Settings :

# Jwt Token verification block

# inspector = CaseInsensitiveDict()
# inspector["Accept"] = "application/json"
# inspector["Authorization"] = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJtYW5hZ2VyIiwidG9rZW4iOiJxd2VydHkiLCJ1c2VyIjoia25pOWh0In0.83-jWdvW2Jt3xPpBAwvIbMup5Fpy8zUyomf4lTnh8BI"

# Jwt Token endblock
# Local URL
# url = 'https://localhost:1235/'
cpu = [0, 0, 0, 0, 0]
mem = [0, 0, 0, 0, 0]


def node(request):
    try:
        if request.user.is_anonymous:
            return render(request, 'login_signin.html')
        is_active = request.GET.get('is_active')
        if is_active:
            key = "nodes"
            nodeurl = url+"node/list?is_active="+is_active
            page = requests.get(url=nodeurl, headers=inspector, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            if key in data:
                data = data["nodes"]
                database = {"nodelist": data}
                return render(request, 'index.html', database)
            else:
                return render(request, 'index.html', {"message": "No Result Found"})
        else:
            key = "nodes"
            nodeurl = url+"node/list"
            page = requests.get(nodeurl, headers=inspector, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            if "nodes" in data:
                data = data["nodes"]
                database = {"nodelist": data}
                return render(request, 'index.html', database)
            else:
                return render(request, 'index.html', {"message": "No Node is Registered Yet"})
    except:
        return render(request, 'serverDown.html')


def events(request):
    try:
        if request.user.is_anonymous:
            return render(request, 'login_signin.html')
        request.session['page'] = 1
        if request.method == "POST":
            request.session['res'] = 0
            request.session['hostid'] = False
            request.session['isopen'] = "static"
            request.session['startTime'] = False
            request.session['endTime'] = False
            hostidentifier = str(request.POST.get('hostidentifier')).strip()
            isopen = request.POST.get('isopen')
            if isopen == "1":
                isopen = True
            elif isopen == "0":
                isopen = False
            key = "events"
            # Time pannel
            startdate = str(request.POST.get('start_date'))
            request.session['stdate'] = startdate
            starttime = str(request.POST.get('start_time'))
            request.session['sttime'] = starttime
            enddate = str(request.POST.get('end_date'))
            request.session['eddate'] = enddate
            endtime = str(request.POST.get('end_time'))
            request.session['edtime'] = endtime
            if hostidentifier and hostidentifier != "" and hostidentifier != " " and hostidentifier != "None" and isopen != "null" and isopen != "none" and starttime and startdate and startdate != "None" and starttime != "None" and endtime and enddate and enddate != "None" and endtime != "None":
                eventurl = url+"event/list"
                dateTime = startdate.split("-")
                b = starttime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                startTime = int(time.mktime(d.timetuple()))
                dateTime = enddate.split("-")
                b = endtime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                endTime = int(time.mktime(d.timetuple()))
                request.session['hostid'] = hostidentifier
                request.session['isopen'] = str(isopen)
                request.session['startTime'] = startTime
                request.session['endTime'] = endTime
                dist = {"host_identifier": str(hostidentifier), "is_open": isopen, "start_time": str(
                    startTime), "end_time": str(endTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "hostid": str(hostidentifier), "is_open": isopen, "stdate": str(request.session.get('stdate')), "sttime": str(
                        request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif hostidentifier and hostidentifier != "" and hostidentifier != " " and hostidentifier != "None" and starttime and startdate and startdate != "None" and starttime != "None" and endtime and enddate and enddate != "None" and endtime != "None":
                eventurl = url+"event/list"
                dateTime = startdate.split("-")
                b = starttime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                startTime = int(time.mktime(d.timetuple()))
                dateTime = enddate.split("-")
                b = endtime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                endTime = int(time.mktime(d.timetuple()))
                request.session['hostid'] = hostidentifier
                request.session['startTime'] = startTime
                request.session['endTime'] = endTime
                dist = {"host_identifier": str(hostidentifier), "start_time": str(
                    startTime), "end_time": str(endTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "hostid": str(hostidentifier), "stdate": str(request.session.get('stdate')), "sttime": str(
                        request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif starttime and startdate and startdate != "None" and starttime != "None" and endtime and enddate and enddate != "None" and endtime != "None" and isopen != "null" and isopen != "none":
                eventurl = url+"event/list"
                dateTime = startdate.split("-")
                b = starttime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                startTime = int(time.mktime(d.timetuple()))
                dateTime = enddate.split("-")
                b = endtime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                endTime = int(time.mktime(d.timetuple()))
                request.session['isopen'] = str(isopen)
                request.session['startTime'] = startTime
                request.session['endTime'] = endTime
                dist = {"is_open": isopen, "start_time": str(
                    startTime), "end_time": str(endTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "is_open": isopen, "stdate": str(request.session.get('stdate')), "sttime": str(request.session.get(
                        'sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif starttime and startdate and startdate != "None" and starttime != "None" and hostidentifier and hostidentifier != "" and hostidentifier != " " and isopen != "null" and isopen != "none":
                eventurl = url+"event/list"
                dateTime = startdate.split("-")
                b = starttime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                startTime = int(time.mktime(d.timetuple()))
                request.session['hostid'] = hostidentifier
                request.session['isopen'] = str(isopen)
                request.session['startTime'] = startTime
                dist = {"host_identifier": str(
                    hostidentifier), "start_time": str(startTime), "is_open": isopen}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "hostid": str(hostidentifier), "is_open": isopen, "stdate": str(
                        request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif endtime and enddate and enddate != "None" and endtime != "None" and hostidentifier and hostidentifier != "" and hostidentifier != " " and isopen != "null" and isopen != "none":
                eventurl = url+"event/list"
                dateTime = enddate.split("-")
                b = endtime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                endTime = int(time.mktime(d.timetuple()))
                request.session['hostid'] = hostidentifier
                request.session['isopen'] = str(isopen)
                request.session['endTime'] = endTime
                dist = {"host_identifier": str(
                    hostidentifier), "end_time": str(endTime), "is_open": isopen}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "hostid": str(hostidentifier), "is_open": isopen, "eddate": str(
                        request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif starttime and startdate and startdate != "None" and starttime != "None" and endtime and enddate and enddate != "None" and endtime != "None":
                dateTime = startdate.split("-")
                b = starttime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                startTime = int(time.mktime(d.timetuple()))
                dateTime = enddate.split("-")
                b = endtime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                endTime = int(time.mktime(d.timetuple()))
                eventurl = url+"event/list"
                request.session['endTime'] = endTime
                request.session['startTime'] = startTime
                dist = {"start_time": str(startTime), "end_time": str(endTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "stdate": str(request.session.get('stdate')), "sttime": str(request.session.get(
                        'sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif starttime and startdate and startdate != "None" and starttime != "None" and hostidentifier and hostidentifier != "" and hostidentifier != " ":
                eventurl = url+"event/list"
                dateTime = startdate.split("-")
                b = starttime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                startTime = int(time.mktime(d.timetuple()))
                request.session['hostid'] = hostidentifier
                request.session['startTime'] = startTime
                dist = {"host_identifier": str(
                    hostidentifier), "start_time": str(startTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "hostid": str(hostidentifier), "stdate": str(request.session.get(
                        'stdate')), "sttime": str(request.session.get('sttime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif hostidentifier and hostidentifier != "" and hostidentifier != " " and hostidentifier != "None" and isopen != "null" and isopen != "none":
                eventurl = url+"event/list"
                request.session['hostid'] = hostidentifier
                request.session['isopen'] = str(isopen)
                dist = {"host_identifier": str(
                    hostidentifier), "is_open": isopen}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "hostid": str(
                        hostidentifier), "is_open": isopen, "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": "for Host name : "+str(hostidentifier)})
            elif hostidentifier and hostidentifier != "" and hostidentifier != " " and endtime and enddate and enddate != "None" and endtime != "None":
                eventurl = url+"event/list"
                dateTime = enddate.split("-")
                b = endtime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                endTime = int(time.mktime(d.timetuple()))
                request.session['hostid'] = hostidentifier
                request.session['endTime'] = endTime
                dist = {"host_identifier": str(
                    hostidentifier), "end_time": str(endTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "hostid": str(hostidentifier), "eddate": str(request.session.get(
                        'eddate')), "edtime": str(request.session.get('edtime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif isopen != "null" and isopen != "none" and starttime and startdate and startdate != "None" and starttime != "None":
                eventurl = url+"event/list"
                dateTime = startdate.split("-")
                b = starttime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                startTime = int(time.mktime(d.timetuple()))
                request.session['isopen'] = str(isopen)
                request.session['startTime'] = startTime
                dist = {"is_open": isopen, "start_time": str(startTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "is_open": isopen, "stdate": str(request.session.get(
                        'stdate')), "sttime": str(request.session.get('sttime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif isopen != "null" and isopen != "none" and endtime and enddate and enddate != "None" and endtime != "None":
                eventurl = url+"event/list"
                dateTime = enddate.split("-")
                b = endtime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                endTime = int(time.mktime(d.timetuple()))
                request.session['endTime'] = endTime
                request.session['isopen'] = str(isopen)
                dist = {"is_open": isopen, "end_time": str(endTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "is_open": isopen, "eddate": str(request.session.get(
                        'eddate')), "edtime": str(request.session.get('edtime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif starttime and startdate and startdate != "None" and starttime != "None":
                dateTime = startdate.split("-")
                b = starttime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                startTime = int(time.mktime(d.timetuple()))
                eventurl = url+"event/list"
                request.session['startTime'] = startTime
                dist = {"start_time": str(startTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "stdate": str(request.session.get(
                        'stdate')), "sttime": str(request.session.get('sttime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif endtime and enddate and enddate != "None" and endtime != "None":
                dateTime = enddate.split("-")
                b = endtime.split(":")
                for i in b:
                    dateTime.append(i)
                d = datetime(int(dateTime[0]), int(dateTime[1]), int(
                    dateTime[2]), int(dateTime[3]), int(dateTime[4]))
                endTime = int(time.mktime(d.timetuple()))
                eventurl = url+"event/list"
                request.session['endTime'] = endTime
                dist = {"end_time": str(endTime)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "eddate": str(request.session.get(
                        'eddate')), "edtime": str(request.session.get('edtime')), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
            elif hostidentifier and hostidentifier != "" and hostidentifier != " ":
                eventurl = url+"event/list"
                request.session['hostid'] = hostidentifier
                dist = {"host_identifier": str(hostidentifier)}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "hostid": str(
                        hostidentifier), "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": "for Host name : "+str(hostidentifier)})
            elif isopen != "null" and isopen != "none":
                eventurl = url+"event/list"
                request.session['isopen'] = str(isopen)
                dist = {"is_open": isopen}
                dist = json.dumps(dist)
                dist = json.loads(dist)
                page = requests.get(url=eventurl, headers=inspector,
                                    params=dist, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data,
                                "is_open": isopen, "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": "if : "+str(isopen)})
            elif request.method == "POST":
                key = "events"
                eventurl = url+"event/list"
                page = requests.get(eventurl, headers=inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                if key in data:
                    res = data["no_of_records"]
                    data = data["events"]
                    tot = res/100
                    request.session['res'] = tot = math.ceil(tot)
                    database = {"eventlist": data, "res": res, "tot": tot}
                    return render(request, 'Events.html', database)
                else:
                    return render(request, 'Events.html', {"hostid": " "})
        else:
            request.session['res'] = 0
            request.session['page'] = 1
            request.session['hostid'] = False
            request.session['isopen'] = "static"
            request.session['startTime'] = False
            request.session['endTime'] = False
            key = "events"
            eventurl = url+"event/list"
            page = requests.get(eventurl, headers=inspector, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            if key in data:
                res = data["no_of_records"]
                data = data["events"]
                tot = res/100
                request.session['res'] = tot = math.ceil(tot)
                database = {"eventlist": data, "res": res, "tot": tot}
                return render(request, 'Events.html', database)
            else:
                return render(request, 'Events.html', {"hostid": " "})
    except:
        return render(request, 'serverDown.html')


def config(request):
    try:
        if request.user.is_anonymous:
            return render(request, 'login_signin.html')
        key = "config_queries"
        configurl = url+"config/list"
        page = requests.get(configurl, headers=inspector, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            data = data["config_queries"]
            database = {"configlist": data}
            return render(request, 'config.html', database)
        else:
            return render(request, 'config.html')
    except:
        return render(request, 'serverDown.html')


def addcon(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    if request.method == "POST":
        querydata = {}
        count = 0
        conaddurl = url+"config/add"
        config_name = str(request.POST.get('config_name'))
        pro_events = bool(request.POST.get('pro_events'))
        pro_file_events = bool(request.POST.get('pro_file_events'))
        pro_list = bool(request.POST.get('pro_list'))
        soc_events = bool(request.POST.get('soc_events'))
        usr_events = bool(request.POST.get('usr_events'))
        process_open_pipes = bool(request.POST.get('process_open_pipes'))
        process_open_files = bool(request.POST.get('process_open_files'))
        listening_ports = bool(request.POST.get('listening_ports'))
        routes = bool(request.POST.get('routes'))
        logged_in_users = bool(request.POST.get('logged_in_users'))
        arp_cache = bool(request.POST.get('arp_cache'))
        suid_bin = bool(request.POST.get('suid_bin'))
        process_open_sockets = bool(request.POST.get('process_open_sockets'))
        users = bool(request.POST.get('users'))
        groups = bool(request.POST.get('groups'))
        process_envs = bool(request.POST.get('process_envs'))
        memory_info = bool(request.POST.get('memory_info'))
        cpu_time = bool(request.POST.get('cpu_time'))
        if pro_events:
            count = count+1
            int1 = int(request.POST.get('interval1'))
            querydata["process_events"] = {
                "interval": int1, "query": "select * from process_events"}
        if pro_file_events:
            count = count+1
            int2 = int(request.POST.get('interval2'))
            querydata["process_file_events"] = {
                "interval": int2, "query": "select * from process_file_events"}
        if pro_list:
            count = count+1
            int3 = int(request.POST.get('interval3'))
            snap = request.POST.get('snap')
            if snap == "true":
                snap = 1
            else:
                snap = 0
            querydata["process_list"] = {
                "interval": int3, "query": "select * from processes", "snapshot": bool(snap)}
        if soc_events:
            count = count+1
            int4 = int(request.POST.get('interval4'))
            querydata["socket_events"] = {
                "interval": int4, "query": "select * from socket_events"}
        if usr_events:
            count = count+1
            int5 = int(request.POST.get('interval5'))
            querydata["user_events"] = {
                "interval": int5, "query": "select * from user_events"}
        if process_open_pipes:
            count = count+1
            int6 = int(request.POST.get('interval6'))
            snap1 = request.POST.get('snap1')
            if snap1 == "true":
                snap1 = 1
            else:
                snap1 = 0
            querydata["process_open_pipes"] = {
                "interval": int6, "query": "select * from process_open_pipes", "snapshot": bool(snap1)}
        if process_open_files:
            count = count+1
            int7 = int(request.POST.get('interval7'))
            snap2 = request.POST.get('snap2')
            if snap2 == "true":
                snap2 = 1
            else:
                snap2 = 0
            querydata["process_open_files"] = {
                "interval": int7, "query": "select * from process_open_files", "snapshot": bool(snap2)}
        if listening_ports:
            count = count+1
            int8 = int(request.POST.get('interval8'))
            snap3 = request.POST.get('snap3')
            if snap3 == "true":
                snap3 = 1
            else:
                snap3 = 0
            querydata["listening_ports"] = {
                "interval": int8, "query": "select * from listening_ports", "snapshot": bool(snap3)}
        if routes:
            count = count+1
            int9 = int(request.POST.get('interval9'))
            snap4 = request.POST.get('snap4')
            if snap4 == "true":
                snap4 = 1
            else:
                snap4 = 0
            querydata["routes"] = {
                "interval": int9, "query": "select * from routes", "snapshot": bool(snap4)}
        if logged_in_users:
            count = count+1
            int10 = int(request.POST.get('interval10'))
            snap5 = request.POST.get('snap5')
            if snap5 == "true":
                snap5 = 1
            else:
                snap5 = 0
            querydata["logged_in_users"] = {
                "interval": int10, "query": "select * from logged_in_users", "snapshot": bool(snap5)}
        if arp_cache:
            count = count+1
            int11 = int(request.POST.get('interval11'))
            snap6 = request.POST.get('snap6')
            if snap6 == "true":
                snap6 = 1
            else:
                snap6 = 0
            querydata["arp_cache"] = {
                "interval": int11, "query": "select * from arp_cache", "snapshot": bool(snap6)}
        if suid_bin:
            count = count+1
            int12 = int(request.POST.get('interval12'))
            snap7 = request.POST.get('snap7')
            if snap7 == "true":
                snap7 = 1
            else:
                snap7 = 0
            querydata["suid_bin"] = {
                "interval": int12, "query": "select * from suid_bin", "snapshot": bool(snap7)}
        if process_open_sockets:
            count = count+1
            int13 = int(request.POST.get('interval13'))
            snap8 = request.POST.get('snap8')
            if snap8 == "true":
                snap8 = 1
            else:
                snap8 = 0
            querydata["process_open_sockets"] = {
                "interval": int13, "query": "select * from process_open_sockets", "snapshot": bool(snap8)}
        if users:
            count = count+1
            int14 = int(request.POST.get('interval14'))
            snap9 = request.POST.get('snap9')
            if snap9 == "true":
                snap9 = 1
            else:
                snap9 = 0
            querydata["users"] = {
                "interval": int14, "query": "select * from users", "snapshot": bool(snap9)}
        if groups:
            count = count+1
            int15 = int(request.POST.get('interval15'))
            snap10 = request.POST.get('snap10')
            if snap10 == "true":
                snap10 = 1
            else:
                snap10 = 0
            querydata["groups"] = {
                "interval": int15, "query": "select * from groups", "snapshot": bool(snap10)}
        if process_envs:
            count = count+1
            int16 = int(request.POST.get('interval16'))
            snap11 = request.POST.get('snap11')
            if snap11 == "true":
                snap11 = 1
            else:
                snap11 = 0
            querydata["process_envs"] = {
                "interval": int16, "query": "select * from groups", "snapshot": bool(snap11)}
        if memory_info:
            count = count+1
            int17 = int(request.POST.get('interval17'))
            querydata["memory_info"] = {
                "interval": int17, "query": "select * from memory_info"}
        if cpu_time:
            count = count+1
            int18 = int(request.POST.get('interval18'))
            querydata["cpu_time"] = {
                "interval": int18, "query": "select * from cpu_time"}
        queryAPI = {"schedule": querydata}
        queryAPI = {"config": queryAPI, "query_name": config_name}
        queryAPI = json.dumps(queryAPI)
        if count > 0:
            response = requests.post(
                conaddurl, headers=inspector, data=queryAPI, verify=False)
        # return redirect(request.META['HTTP_REFERER'])
            return redirect('/config')
        else:
            return render(request, 'addcon.html', {"alert": "Something went wrong ...!"})
    else:
        return render(request, 'addcon.html')


def upcon_view(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    key = "config_queries"
    configurl = url+"config/list"
    id = request.GET.get('id')
    dist = {"id": id}
    page = requests.get(configurl, headers=inspector,
                        params=dist, verify=False)
    page = page.json()
    page = json.dumps(page)
    data = json.loads(page)
    if key in data:
        data = data["config_queries"]
        res = json.dumps(data[0]['query'])
        res = ast.literal_eval(res)
        res = json.loads(res)
        database = {"editcon": data, "data": res}
        return render(request, 'upcon.html', database)
    else:
        return render(request, 'upcon.html')
    # return redirect(request.META['HTTP_REFERER'])


def upcon(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    if request.method == "POST":
        querydata = {}
        count = 0
        conaddurl = url+"config/update"
        id = request.POST.get('conid')
        config_name = str(request.POST.get('config_name'))
        pro_events = bool(request.POST.get('pro_events'))
        pro_file_events = bool(request.POST.get('pro_file_events'))
        pro_list = bool(request.POST.get('pro_list'))
        soc_events = bool(request.POST.get('soc_events'))
        usr_events = bool(request.POST.get('usr_events'))
        process_open_pipes = bool(request.POST.get('process_open_pipes'))
        process_open_files = bool(request.POST.get('process_open_files'))
        listening_ports = bool(request.POST.get('listening_ports'))
        routes = bool(request.POST.get('routes'))
        logged_in_users = bool(request.POST.get('logged_in_users'))
        arp_cache = bool(request.POST.get('arp_cache'))
        suid_bin = bool(request.POST.get('suid_bin'))
        process_open_sockets = bool(request.POST.get('process_open_sockets'))
        users = bool(request.POST.get('users'))
        groups = bool(request.POST.get('groups'))
        process_envs = bool(request.POST.get('process_envs'))
        memory_info = bool(request.POST.get('memory_info'))
        cpu_time = bool(request.POST.get('cpu_time'))
        if pro_events:
            count = count+1
            int1 = int(request.POST.get('interval1'))
            querydata["process_events"] = {
                "interval": int1, "query": "select * from process_events"}
        if pro_file_events:
            count = count+1
            int2 = int(request.POST.get('interval2'))
            querydata["process_file_events"] = {
                "interval": int2, "query": "select * from process_file_events"}
        if pro_list:
            count = count+1
            int3 = int(request.POST.get('interval3'))
            snap = request.POST.get('snap')
            if snap == "True":
                snap = 1
            else:
                snap = 0
            querydata["process_list"] = {
                "interval": int3, "query": "select * from processes", "snapshot": bool(snap)}
        if soc_events:
            count = count+1
            int4 = int(request.POST.get('interval4'))
            querydata["socket_events"] = {
                "interval": int4, "query": "select * from socket_events"}
        if usr_events:
            count = count+1
            int5 = int(request.POST.get('interval5'))
            querydata["user_events"] = {
                "interval": int5, "query": "select * from user_events"}
        if process_open_pipes:
            count = count+1
            int6 = int(request.POST.get('interval6'))
            snap1 = request.POST.get('snap1')
            if snap1 == "true":
                snap1 = 1
            else:
                snap1 = 0
            querydata["process_open_pipes"] = {
                "interval": int6, "query": "select * from process_open_pipes", "snapshot": bool(snap1)}
        if process_open_files:
            count = count+1
            int7 = int(request.POST.get('interval7'))
            snap2 = request.POST.get('snap2')
            if snap2 == "true":
                snap2 = 1
            else:
                snap2 = 0
            querydata["process_open_files"] = {
                "interval": int7, "query": "select * from process_open_files", "snapshot": bool(snap2)}
        if listening_ports:
            count = count+1
            int8 = int(request.POST.get('interval8'))
            snap3 = request.POST.get('snap3')
            if snap3 == "true":
                snap3 = 1
            else:
                snap3 = 0
            querydata["listening_ports"] = {
                "interval": int8, "query": "select * from listening_ports", "snapshot": bool(snap3)}
        if routes:
            count = count+1
            int9 = int(request.POST.get('interval9'))
            snap4 = request.POST.get('snap4')
            if snap4 == "true":
                snap4 = 1
            else:
                snap4 = 0
            querydata["routes"] = {
                "interval": int9, "query": "select * from routes", "snapshot": bool(snap4)}
        if logged_in_users:
            count = count+1
            int10 = int(request.POST.get('interval10'))
            snap5 = request.POST.get('snap5')
            if snap5 == "true":
                snap5 = 1
            else:
                snap5 = 0
            querydata["logged_in_users"] = {
                "interval": int10, "query": "select * from logged_in_users", "snapshot": bool(snap5)}
        if arp_cache:
            count = count+1
            int11 = int(request.POST.get('interval11'))
            snap6 = request.POST.get('snap6')
            if snap6 == "true":
                snap6 = 1
            else:
                snap6 = 0
            querydata["arp_cache"] = {
                "interval": int11, "query": "select * from arp_cache", "snapshot": bool(snap6)}
        if suid_bin:
            count = count+1
            int12 = int(request.POST.get('interval12'))
            snap7 = request.POST.get('snap7')
            if snap7 == "true":
                snap7 = 1
            else:
                snap7 = 0
            querydata["suid_bin"] = {
                "interval": int12, "query": "select * from suid_bin", "snapshot": bool(snap7)}
        if process_open_sockets:
            count = count+1
            int13 = int(request.POST.get('interval13'))
            snap8 = request.POST.get('snap8')
            if snap8 == "true":
                snap8 = 1
            else:
                snap8 = 0
            querydata["process_open_sockets"] = {
                "interval": int13, "query": "select * from process_open_sockets", "snapshot": bool(snap8)}
        if users:
            count = count+1
            int14 = int(request.POST.get('interval14'))
            snap9 = request.POST.get('snap9')
            if snap9 == "true":
                snap9 = 1
            else:
                snap9 = 0
            querydata["users"] = {
                "interval": int14, "query": "select * from users", "snapshot": bool(snap9)}
        if groups:
            count = count+1
            int15 = int(request.POST.get('interval15'))
            snap10 = request.POST.get('snap10')
            if snap10 == "true":
                snap10 = 1
            else:
                snap10 = 0
            querydata["groups"] = {
                "interval": int15, "query": "select * from groups", "snapshot": bool(snap10)}
        if process_envs:
            count = count+1
            int16 = int(request.POST.get('interval16'))
            snap11 = request.POST.get('snap11')
            if snap11 == "true":
                snap11 = 1
            else:
                snap11 = 0
            querydata["process_envs"] = {
                "interval": int16, "query": "select * from groups", "snapshot": bool(snap11)}
        if memory_info:
            count = count+1
            int17 = int(request.POST.get('interval17'))
            querydata["memory_info"] = {
                "interval": int17, "query": "select * from memory_info"}
        if cpu_time:
            count = count+1
            int18 = int(request.POST.get('interval18'))
            querydata["cpu_time"] = {
                "interval": int18, "query": "select * from cpu_time"}
        queryAPI = {"id": id, "schedule": querydata,"query_name":config_name}
        queryAPI = json.dumps(queryAPI)
        if count > 0:
            requests.post(conaddurl, headers=inspector,
                          data=queryAPI, verify=False)
        # return redirect(request.META['HTTP_REFERER'])
        return redirect('/config')
    else:
        return render(request, 'upcon.html')


def nodecon(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    hostID = request.GET.get('host')
    config_id = request.GET.get('config')
    data = {"config_id": str(config_id), "host_identifier": str(hostID)}
    data = json.dumps(data)
    nodeconURL = url+"node/config"
    mess = requests.post(nodeconURL, headers=inspector,
                         data=data, verify=False)
    mess = mess.json()
    mess = json.dumps(mess)
    mess = json.loads(mess)
    return JsonResponse({"message": mess})


def node_extended(request):
    try:
        sec = False
        if request.user.is_anonymous:
            return render(request, 'login_signin.html')
        tm = "offline"
        hostID = request.GET.get('host_id')
        request.session['hostID'] = hostID
        queryurl = url+"node/query"+"?host_identifier="+str(hostID)
        page = requests.get(queryurl, headers=inspector, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        data = data["nodes"][0]
        rein = data["reenroll_allowed"]
        time = data["last_seen"]
        time = ast.literal_eval(time)
        key = "config_queries"
        configurl = url+"config/list"
        page1 = requests.get(configurl, headers=inspector, verify=False)
        page1 = page1.json()
        page1 = json.dumps(page1)
        data1 = json.loads(page1)
        if key in data1:
            data1 = data1["config_queries"]
        if tm in time:
            sec = time["offline"]
        if rein == 't':
            rein = "checked"
        else:
            rein = ""
        if sec:
            database = {"hostINFO": data, "rein": rein,
                        "sec": sec, "config": data1}
        else:
            database = {"hostINFO": data, "rein": rein, "config": data1}
        return render(request, 'node_extended.html', database)
    except:
        return render(request, 'node_extended.html', {"not_found": 1})


def graph(request):
    severity = {"0": 0, "1": 0, "2": 0, "3": 0, "4": 0}
    memory = "memory_health"
    processing = "cpu_health"
    seve = "severity"
    hostID = request.session.get('hostID')
    queryurl = url+"node/query"+"?host_identifier="+str(hostID)
    page = requests.get(queryurl, headers=inspector, verify=False)
    page = page.json()
    page = json.dumps(page)
    data = json.loads(page)
    if memory in data:
        mem_total = data["memory_health"][0]["memory_total"]
        mem_used = data["memory_health"][0]["memory_free"]
        mem.insert(0,100-(int(mem_used)/int(mem_total))*100)
        if len(mem) > 5:
            mem.pop()
    if processing in data:
        cpudata = float(data["cpu_health"][0]["average_CPU_load"])
        cpu.insert(0, cpudata)
        if len(cpu) > 5:
            cpu.pop()
    if seve in data:
        seve = data["severity"]
        for i in seve:
            severity[i["severity_level"]] = int(i["severity_count"])
        severity = list(severity.values())
    return JsonResponse({"mem": mem, "cpu": cpu, "severity": severity})


def loginuser(request):
    if request.method == "POST":
        user = auth.authenticate(
            request, username=request.POST['username'], password=request.POST['password'])
        if user is not None:
            auth.login(request, user)
            return redirect('/home')
        else:
            messages.warning(request, 'Invalid credential,Try again')
            return redirect(request.META['HTTP_REFERER'])
    else:
        raise Http404("access denied")


def logout_user(request):
    auth.logout(request)
    return redirect('/home')


def reIN(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    reIn = request.GET.get('reIN')
    hostID = request.GET.get('host_id')
    if reIn == 'on':
        reIn = True
    else:
        reIn = False
    data = {"host_identifier": str(hostID), "reenroll_allowed": reIn}
    data = json.dumps(data)
    reINurl = url+"node/reenroll"
    requests.post(reINurl, headers=inspector, data=data, verify=False)
    return redirect(request.META['HTTP_REFERER'])


def delete(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    hostID = request.GET.get('host_id')
    delnode = url+"node/delete"
    dist = {"host_identifier": str(hostID)}
    dist = json.dumps(dist)
    requests.delete(delnode, headers=inspector, data=dist, verify=False)
    return redirect('/home')


def makeclose(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    eventid = request.GET.get('event_id')
    remark = request.GET.get('commenttxt')
    data = {"event_id": int(eventid), "is_open": False, "remarks": str(remark)}
    data = json.dumps(data)
    IsOpen = url+"event/update"
    requests.post(IsOpen, headers=inspector, data=data, verify=False)
    return redirect(request.META['HTTP_REFERER'])


def makeopen(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    eventid = request.GET.get('event_id')
    data = {"event_id": int(eventid), "is_open": True}
    data = json.dumps(data)
    IsOpen = url+"event/update"
    requests.post(IsOpen, headers=inspector, data=data, verify=False)
    return redirect(request.META['HTTP_REFERER'])


def pagup(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    key = "events"
    request.session['page'] = int(request.session['page']) + 1
    pageno = int(request.session.get('page'))
    if pageno > int(request.session.get('res')):
        pageno = pageno-1
        request.session['page'] = pageno-1
    res = int(100)
    hostidentifier = request.session.get('hostid')
    isopen = request.session.get('isopen')
    startTime = request.session.get('startTime')
    endTime = request.session.get('endTime')
    if hostidentifier and isopen != "static" and startTime and endTime:
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"host_identifier": str(hostidentifier), "is_open": isopen, "start_time": str(
            startTime), "end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(hostidentifier), "is_open": isopen, "stdate": str(request.session.get('stdate')), "sttime": str(
                request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif hostidentifier and startTime and endTime:
        eventurl = url+"event/list"
        dist = {"host_identifier": str(hostidentifier), "start_time": str(
            startTime), "end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(hostidentifier), "stdate": str(request.session.get('stdate')), "sttime": str(
                request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime and endTime and isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"is_open": isopen, "start_time": str(
            startTime), "end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = int(ress/100)
            database = {"eventlist": data, "page": pageno, "is_open": isopen, "stdate": str(request.session.get('stdate')), "sttime": str(
                request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime and hostidentifier and isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"host_identifier": str(
            hostidentifier), "start_time": str(startTime), "is_open": isopen, "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(hostidentifier), "stdate": str(
                request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "is_open": isopen, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif endTime and hostidentifier and isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"host_identifier": str(
            hostidentifier), "end_time": str(endTime), "is_open": isopen, "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(hostidentifier), "eddate": str(request.session.get(
                'eddate')), "edtime": str(request.session.get('edtime')), "is_open": isopen, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime and endTime:
        eventurl = url+"event/list"
        dist = {"start_time": str(startTime), "end_time": str(
            endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "stdate": str(request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime and hostidentifier:
        eventurl = url+"event/list"
        dist = {"host_identifier": str(
            hostidentifier), "start_time": str(startTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(
                hostidentifier), "stdate": str(request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif hostidentifier and isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"host_identifier": str(
            hostidentifier), "is_open": isopen, "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "hostid": str(hostidentifier), "is_open": isopen, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": "for Host name : "+str(hostidentifier)})
    elif hostidentifier and endTime:
        eventurl = url+"event/list"
        dist = {"host_identifier": str(
            hostidentifier), "end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "hostid": str(hostidentifier), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif isopen != "static" and startTime:
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"is_open": isopen, "start_time": str(
            startTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "is_open": isopen, "stdate": str(request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif isopen != "static" and endTime:
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"is_open": isopen, "end_time": str(
            endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "is_open": isopen, "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime:
        eventurl = url+"event/list"
        dist = {"start_time": str(startTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "stdate": str(request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif endTime:
        eventurl = url+"event/list"
        dist = {"end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data,
                        "page": pageno, "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif hostidentifier:
        eventurl = url+"event/list"
        dist = {"host_identifier": str(
            hostidentifier), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "hostid": str(hostidentifier), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": "for Host name : "+str(hostidentifier)})
    elif isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"is_open": isopen, "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "is_open": isopen, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": "iF : "+str(isopen)})
    else:
        key = "events"
        eventurl = url+"event/list"
        dist = {"pageno": pageno, "rows": res}
        page = requests.get(eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data,
                        "page": pageno, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})


def pagdn(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    key = "events"
    request.session['page'] = int(request.session['page']) - 1
    pageno = int(request.session.get('page'))
    if pageno <= 0:
        pageno = 1
        request.session['page'] = 1
    res = int(100)
    hostidentifier = request.session.get('hostid')
    isopen = request.session.get('isopen')
    startTime = request.session.get('startTime')
    endTime = request.session.get('endTime')
    if hostidentifier and isopen != "static" and startTime and endTime:
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"host_identifier": str(hostidentifier), "is_open": isopen, "start_time": str(
            startTime), "end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(hostidentifier), "is_open": isopen, "stdate": str(request.session.get('stdate')), "sttime": str(
                request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif hostidentifier and startTime and endTime:
        eventurl = url+"event/list"
        dist = {"host_identifier": str(hostidentifier), "start_time": str(
            startTime), "end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(hostidentifier), "stdate": str(request.session.get('stdate')), "sttime": str(
                request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime and endTime and isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"is_open": isopen, "start_time": str(
            startTime), "end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "is_open": isopen, "stdate": str(request.session.get('stdate')), "sttime": str(
                request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime and hostidentifier and isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"host_identifier": str(
            hostidentifier), "start_time": str(startTime), "is_open": isopen, "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(hostidentifier), "stdate": str(
                request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "is_open": isopen, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif endTime and hostidentifier and isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"host_identifier": str(
            hostidentifier), "end_time": str(endTime), "is_open": isopen, "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(hostidentifier), "eddate": str(request.session.get(
                'eddate')), "edtime": str(request.session.get('edtime')), "is_open": isopen, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime and endTime:
        eventurl = url+"event/list"
        dist = {"start_time": str(startTime), "end_time": str(
            endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "stdate": str(request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime and hostidentifier:
        eventurl = url+"event/list"
        dist = {"host_identifier": str(
            hostidentifier), "start_time": str(startTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno, "hostid": str(hostidentifier), "stdate": str(
                request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif hostidentifier and isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"host_identifier": str(
            hostidentifier), "is_open": isopen, "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "hostid": str(hostidentifier), "is_open": isopen, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": "for Host name : "+str(hostidentifier)})
    elif hostidentifier and endTime:
        eventurl = url+"event/list"
        dist = {"host_identifier": str(
            hostidentifier), "end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "hostid": str(hostidentifier), "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif isopen != "static" and startTime:
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"is_open": isopen, "start_time": str(
            startTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "is_open": isopen, "stdate": str(request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif isopen != "static" and endTime:
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"is_open": isopen, "end_time": str(
            endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "is_open": isopen, "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif startTime:
        eventurl = url+"event/list"
        dist = {"start_time": str(startTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "stdate": str(request.session.get('stdate')), "sttime": str(request.session.get('sttime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif endTime:
        eventurl = url+"event/list"
        dist = {"end_time": str(endTime), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data,
                        "page": pageno, "eddate": str(request.session.get('eddate')), "edtime": str(request.session.get('edtime')), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})
    elif hostidentifier:
        eventurl = url+"event/list"
        dist = {"host_identifier": str(
            hostidentifier), "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "hostid": str(hostidentifier), "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": "for Host name : "+str(hostidentifier)})
    elif isopen != "static":
        eventurl = url+"event/list"
        isopen = bool(isopen)
        dist = {"is_open": isopen, "pageno": pageno, "rows": res}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        page = requests.get(url=eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data, "page": pageno,
                        "is_open": isopen, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": "iF : "+str(isopen)})
    else:
        key = "events"
        eventurl = url+"event/list"
        dist = {"pageno": pageno, "rows": res}
        page = requests.get(eventurl, headers=inspector,
                            params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            ress = data["no_of_records"]
            data = data["events"]
            tot = ress/100
            tot = math.ceil(tot)
            database = {"eventlist": data,
                        "page": pageno, "res": ress, "tot": tot}
            return render(request, 'Events.html', database)
        else:
            return render(request, 'Events.html', {"hostid": " "})


def delcon(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    id = request.GET.get('id')
    delcon = url+"config/delete"
    dist = {"id": int(id)}
    dist = json.dumps(dist)
    requests.delete(delcon, headers=inspector, data=dist, verify=False)
    return redirect(request.META['HTTP_REFERER'])

def addhawk_page(request):
    try:
        if request.user.is_anonymous:
            return render(request, 'login_signin.html')
        key = "nodes"
        nodeurl = url+"node/list"
        page = requests.get(nodeurl, headers=inspector, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            data_all = data["nodes"]
            # for displaying only active nodes
            data_active = []
            for node in data_all:
                if node['last_seen'] == "{'online':'Online'}":
                    data_active.append(node)
            database = {"hostlist": data_active}
            return render(request, 'addhawk.html', database)
        else:
            return render(request, 'addhawk.html')
    except:
        return render(request, 'serverDown.html')

def addhawk(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    hawkurl = url+"scheduled_queries/schedule"
    # print(f"\n\n\n{request.COOKIES}\n\n");
    hostides = []
    query = request.POST.get("query")
    hosts = str(request.POST.get("hosts"))
    hosts = hosts.split(",")
    for i in hosts:
        hostides.append({"host_identifier": i})
    hosts = hostides
    queryinit = {"hosts": hosts, "query": query}
    queryinit = json.dumps(queryinit)
    req_response = requests.post(
        hawkurl, headers=inspector, data=queryinit, verify=False)
    req_response = req_response.json()
    req_response = json.dumps(req_response)
    req_response = json.loads(req_response)
    req_response["csrftoken"]=request.COOKIES["csrftoken"];
    return JsonResponse(req_response)

def addhawK(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    hawkurl = url+"scheduled_queries/schedule"
    # print(f"\n\n\n{request.COOKIES}\n\n");
    hostides = []
    query = request.POST.get("query")
    hosts = str(request.POST.get("hosts"))
    hosts = hosts.split(",")
    for i in hosts:
        hostides.append({"host_identifier": i})
    hosts = hostides
    queryinit = {"hosts": hosts, "query": query}
    queryinit = json.dumps(queryinit)
    req_response = requests.post(
        hawkurl, headers=inspector, data=queryinit, verify=False)
    req_response = req_response.json()
    req_response = json.dumps(req_response)
    req_response = json.loads(req_response)
    return JsonResponse(req_response)


def addhawkStatus(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    id_dict = request.POST.get("config_ids")
    id_dict = json.loads(id_dict)
    query_ids = []
    for key in id_dict.keys():
        query_ids.append(id_dict[key])
    query_string = ",".join([str(i) for i in query_ids])
    req_data = {"config_id_list": query_string}
    req_data = json.dumps(req_data)
    print(f"\n\n\n{req_data}\n\n\n")
    hawkresurl = url+"scheduled_queries/status"
    query_resp = requests.post(
        hawkresurl, headers=inspector, data=req_data, verify=False)
    query_resp = query_resp.json()
    query_resp = json.dumps(query_resp)
    query_resp = json.loads(query_resp)
    cond = False
    try:
        for key in query_resp.keys():
            if query_resp[key]["sent"] == True:
                cond = True
        ret_dict = {}
        if cond:
            ret_dict = {"status": "ready"}
        else:
            ret_dict = {"status": "wait"}
    except:
        ret_dict = {"status": "wait"}
    return JsonResponse(ret_dict)

def addhawKstatus(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    id_dict = request.POST.get("config_ids")
    req_data = {"config_id_list": id_dict}
    req_data = json.dumps(req_data)
    hawkresurl = url+"scheduled_queries/status"
    query_resp = requests.post(
        hawkresurl, headers=inspector, data=req_data, verify=False)
    query_resp = query_resp.json()
    query_resp = json.dumps(query_resp)
    query_resp = json.loads(query_resp)
    cond = False
    try:
        for key in query_resp.keys():
            if query_resp[key]["sent"] == True:
                cond = True
        ret_dict = {}
        if cond:
            ret_dict = {"status": "ready"}
        else:
            ret_dict = {"status": "waiting"}
    except:
        ret_dict = {"status": "wait"}
    return JsonResponse(ret_dict)

def addhawkResults(request):
    print("Request in Res")
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    nodeurl = url+"node/list"
    page = requests.get(nodeurl, headers=inspector, verify=False)
    page = page.json()
    page = json.dumps(page)
    data = json.loads(page)
    database = {}
    key = "nodes"
    host_id_name_map = {}
    if key in data:
        data_all = data["nodes"]
        # for displaying only active nodes
        data_active = []
        for node in data_all:
            if node['last_seen'].find('online') != -1:
                host_id_name_map[node['host_identifier']] = node['hostname']
                data_active.append(node)
        database = {"hostlist": data_active}

    req_response = request.POST.get("config_ids")
    req_response = json.loads(req_response)
    query_list = []
    query_ids = []
    for key in req_response.keys():
        host_id = key.split(':')[0]
        query_list.append((host_id, req_response[key]))
        query_ids.append(req_response[key])

    hawkresurl = url+"scheduled_queries/response"
    query_string = ",".join([str(i) for i in query_ids])
    req_data = {"config_id_list": query_string}
    req_data = json.dumps(req_data)
    query_resp = None
    query_resp = requests.post(
        hawkresurl, headers=inspector, data=req_data, verify=False)
    query_resp = query_resp.json()
    query_resp = json.dumps(query_resp)
    query_resp = json.loads(query_resp)
    if query_resp != None:
        print("val found")

    result_list = []
    try:
        for key in query_resp.keys():
            q_host_id = [i[0] for i in query_list if str(i[1]) == key][0]
            q_host_name = host_id_name_map[q_host_id]
            result_list.append(
                {"hostname": q_host_name, "data": query_resp[key]})
    except:
        result_list.append(
            {"hostname": "", "data": "{\"Message\":\"No Result Found or Result not prepared\"}"})
    database["query_resp"] = result_list
    print(f"\n\n\n{database}\n\n\n")
    return JsonResponse(database)

def addhawKresults(request):
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    id_dict = request.POST.get("config_ids")
    req_data = {"config_id_list": id_dict}
    req_data = json.dumps(req_data)
    hawkresurl = url+"scheduled_queries/response"
    query_resp = requests.post(
        hawkresurl, headers=inspector, data=req_data, verify=False)
    query_resp = query_resp.json()
    query_resp = json.dumps(query_resp)
    query_resp = json.loads(query_resp)
    return JsonResponse({"data":query_resp})

def blocklist(request):
    try:
        if request.user.is_anonymous:
            return render(request, 'login_signin.html')
        return render(request, 'blocklist.html')
    except:
        return render(request, 'serverDown.html')


def stats(request):
    severity = {"0": 0, "1": 0, "2": 0, "3": 0, "4": 0}
    database={0:{},1:{},2:{},3:{},4:{}}
    key='node_severity_count'
    # try:
    if request.user.is_anonymous:
        return render(request, 'login_signin.html')
    if request.method == "POST":
        order=request.POST.get('order')
        result=request.POST.get('result')
        dist={"level":str(order),"topn":int(result)}
        dist = json.dumps(dist)
        dist = json.loads(dist)
        severityUrl=url+"stat/max_severity_node_count"
        page = requests.get(url=severityUrl, headers=inspector, params=dist, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        if key in data:
            data=data['node_severity_count']
            for i in data:
                database[int(i['severity'])][i['hostname']]=int(i['count'])
    severityGraph = url+"stat/node_rule_count"
    page = requests.get(url=severityGraph, headers=inspector, verify=False)
    page = page.json()
    page = json.dumps(page)
    data = json.loads(page)
    for i in data:
        for j in i["severity"]:
            severity[j["severity"]] = int(severity[j["severity"]])+int(j["severity_count"])
    severityGraph = list(severity.values())
    return render(request, 'stats.html', {"severity": severityGraph,"database":database})
    # except:
    #     return render(request, 'serverDown.html')


def statistics(request):
    nodeavail = url+"stat/nodes_count"
    nodeoffon = url+"stat/active_nodes_count"
    page = requests.get(url=nodeavail, headers=inspector, verify=False)
    page = page.json()
    page = json.dumps(page)
    data = json.loads(page)

    page1 = requests.get(url=nodeoffon, headers=inspector, verify=False)
    page1 = page1.json()
    page1 = json.dumps(page1)
    data1 = json.loads(page1)

    data = data["nodes"]
    data1 = data1["nodes"]
    off = data1["offline"]
    on = data1["online"]
    data = data["count"]

    return JsonResponse({"count": data, "off": off, "on": on})
