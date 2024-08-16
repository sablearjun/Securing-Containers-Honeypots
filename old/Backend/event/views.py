from django.shortcuts import render
from django.contrib.auth import user_logged_out
from django.contrib.auth.models import User
from django.http import Http404
import distutils.util as util
import auth.settings as settings
import requests
import json
from requests.structures import CaseInsensitiveDict
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.authtoken.serializers import AuthTokenSerializer
from rest_framework.views import APIView
from colorama import init, Fore, Back, Style
# Create your views here.

init(autoreset=True)

eventParams={}

def serialize_event(key,dist):
    if key in dist:
        eventParams[key]=dist[key]

@api_view(['GET'])
def EventList(request):
    try:
        user = request.user
        try:
            eventParams.clear()
            data=request.GET.dict()
            if len(data)>0:
                paramList=['pageno','rows','is_open','host_identifier','start_time','end_time','myalerts']
                for key in paramList:
                    serialize_event(key,data)
            eventurl = settings.url+"event/list"
            dist = json.dumps(eventParams)
            dist = json.loads(dist)
            page = requests.get(eventurl, params=dist, headers=settings.inspector, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            if "myalerts" in dist:
                try:
                    myAlertsList=assign_event.objects.filter(user_id=User.objects.get(username=user).id).values().values_list("event_id",flat=True)[0]
                except:
                    myAlertsList=[]
                if len(myAlertsList) > 0:
                    myAlerts=[]
                    for event in data["events"]:
                        if event["id"] in str(myAlertsList):
                            myAlerts.append(event)
                    return Response({"events":myAlerts,"no_of_records":len(myAlerts)}, status=status.HTTP_200_OK)
                else:
                    return Response({"events":[],"no_of_records":0}, status=status.HTTP_200_OK)
            return Response(data, status=status.HTTP_200_OK)
        except Exception as e:
            print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
            return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['POST'])
def Update(request):
    try:
        user = request.user
        try:
            data=request.data
            dist = {"event_id": data["event_id"], "is_open": data["is_open"], "remarks": data["remarks"]}
            eventurl = settings.url+"event/update"
            dist = json.dumps(dist)
            page = requests.post(eventurl, data=dist, headers=settings.inspector, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            return Response({"info":data}, status=status.HTTP_200_OK)
        except Exception as e:
            print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
            return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

def makeTree(dataSet,PID):
    tree = []
    for data in dataSet:
        if "marked" not in data:
            # if data["parent"] == PID or data["pid"] == PID:
            if data["parent"] == PID:
                data["marked"] = True
                data["name"]=data["path"]
                # if "cmdline" in data:
                #     data["name"]=data["pid"]
                # if "path" in data:
                #     data["name"]=data["pid"]
                data["attributes"]={"pid":data["pid"]}
                if "event_type" in data:
                    data["attributes"]["Event"]=data["event_type"]
                tmp=data
                if "children" not in tmp:
                    tmp["children"]=makeTree(dataSet,tmp["pid"])
                tree.append(tmp)
    return tree


@api_view(['POST'])
def Graph(request):
    try:
        user = request.user
        try:
            try:
                params = request.data["data"]
                pid = json.loads(params)["pid"]
                Id = json.loads(params)["id"]
            except:
                pid = request.data["pid"]
                Id = request.data["id"]
                params = json.dumps(request.data)

            #Forward Process Tree CodeBlock
            try :
                eventurl = settings.url+"event/event_forward_process_tree"
                page = requests.post(eventurl, data=params, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                try:
                    # tree=json.loads(data["get_forward_process_tree"][0]["forward_process_tree"])

                    ForwardGraph=makeTree(data,pid)
                except Exception as e:
                    print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
                    ForwardGraph=data
            except:
                ForwardGraph=[]

            #Backward Process Tree CodeBlock
            eventurl = settings.url+"event/query"
            page = requests.get(eventurl, params={"pid":Id}, headers=settings.inspector, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            malPro=json.loads(data["events"][0]["metadata"])
            data = json.loads(data["events"][0]["metadata"])["parents"]
            del malPro["parents"]
            data.append(malPro)
            for i in data:
                if i["pid"]==pid:
                    i["children"]=ForwardGraph
            Graph = makeTree(data,0)
            Graph = {"Graph":Graph}
            return Response(Graph, status=status.HTTP_200_OK)
        except Exception as e:
            print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception in Main Block: {e}\n")
            return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)


@api_view(['GET'])
def Query(request):
    try:
        user = request.user
        try:
            data=request.GET.dict()
            eventurl = settings.url+"event/query"
            page = requests.get(eventurl, params=data, headers=settings.inspector, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            try:
                data["events"][0]["assign_to"]=RegisterSerializer(User.objects.get(id=assign_event.objects.get(event_id=data["events"][0]["id"]).user_id)).data
            except:
                data["events"][0]["assign_to"]="Event not Assigned"
            return Response(data, status=status.HTTP_200_OK)
        except Exception as e:
            print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
            return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)