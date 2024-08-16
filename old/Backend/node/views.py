from django.shortcuts import render
from django.contrib.auth import user_logged_out
from django.contrib.auth.models import User
from django.http import Http404
# from knox import crypto
from matplotlib.font_manager import json_dump
import auth.settings as settings
import requests
import json
from requests.structures import CaseInsensitiveDict
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.authtoken.serializers import AuthTokenSerializer
# from knox.auth import AuthToken, TokenAuthentication
from rest_framework.views import APIView
from colorama import init, Fore, Back, Style
# Create your views here.

init(autoreset=True)

@api_view(['POST'])
def HistoricalData(request):
    try:
        data = request.data
        data = json.loads(data.get('data'))
        try:
            nodeurl = settings.url+"node/historical_data"
            dist = {"query":data.get('query')}
            dist=json.dumps(dist)
            page = requests.post(nodeurl, data=dist, headers=settings.inspector, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            return Response(data, status=status.HTTP_200_OK)
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
        try:
            params = request.data["data"]
            pid = json.loads(params)["pid"]
            name = json.loads(params)["NameOfContainer"]
        except:
            pid = request.data["pid"]
            name = request.data["NameOfContainer"]
            params = json.dumps(request.data)

        #Forward Process Tree CodeBlock
        try :
            eventurl = settings.url+"event/event_forward_process_tree"
            page = requests.post(eventurl, data=params, headers=settings.inspector, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            try:
                ForwardGraph=makeTree(data,pid)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
                ForwardGraph=data
        except:
            ForwardGraph=[]
        FinalGraph=[{
                "name": name,
                "children":ForwardGraph,
                }]
        return Response(FinalGraph, status=status.HTTP_200_OK)
    except Exception as e:
        print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception in Main Block: {e}\n")
        return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)

@api_view(['POST'])
def GraphTable(request):
    try:
        try:
            params = request.data["data"]
            pid = json.loads(params)["pid"]
            name = json.loads(params)["NameOfContainer"]
        except:
            pid = request.data["pid"]
            name = request.data["NameOfContainer"]
            params = json.dumps(request.data)

        #Forward Process Tree CodeBlock
        eventurl = settings.url+"event/event_forward_process_tree"
        page = requests.post(eventurl, data=params, headers=settings.inspector, verify=False)
        page = page.json()
        page = json.dumps(page)
        data = json.loads(page)
        ForwardGraph=data
        return Response(ForwardGraph, status=status.HTTP_200_OK)
    except Exception as e:
        print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception in Main Block: {e}\n")
        return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
