from django.shortcuts import render
from django.contrib.auth import user_logged_out
from django.contrib.auth.models import User
from django.http import Http404
# from knox import crypto
from rx import catch
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

init(autoreset=True)


@api_view(['GET'])
def nodes_count(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                dashUrl = settings.url+"dashboard/nodes_count"
                page = requests.get(dashUrl, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                data = data["nodes"]
                res={}
                res["data"]=[data["total"],data["online"],data["offline"]]
                res["labels"]=["Total","Online","Offline"]
                return Response(res, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK +
                      Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message": "Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['GET'])
def events_count(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                dashUrl = settings.url+"dashboard/events_count"
                page = requests.get(dashUrl, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                data = data["events"]
                res=[]
                res.append({"name":"Open","value":data["open"]})
                res.append({"name":"Close","value":data["close"]})
                return Response(res, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK +
                      Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message": "Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['GET'])
def os_count(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                dashUrl = settings.url+"dashboard/os_count"
                page = requests.get(dashUrl, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                data = data["os_count"]
                res=[]
                for i in data:
                    res.append({"name":i["os_name"], "count":i["count"]})
                    
                # res.append({"name":"Windows", "count":0})
                # res.append({"name":"Mac-OS", "count":0})
                return Response(res, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK +
                      Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message": "Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['GET'])
def severity_count(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                dashUrl = settings.url+"dashboard/severity_count"
                page = requests.get(dashUrl, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                data = data["severity_count"]
                severity=["None", "Information", "Low", "Medium", "High"]
                tmp={}
                res=[]
                def check(k):
                    if k in tmp.keys():
                        return tmp[k]
                    else:
                        return 0
                for i in data:
                    tmp[severity[int(i["severity"])-1]]=i["count"]
                for i in severity:
                    res.append({"name":i, "value":check(i)})
                return Response(res, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK +
                      Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message": "Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['GET'])
def alert_count(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                dashUrl = settings.url+"dashboard/alert_count"
                page = requests.get(dashUrl, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                data = data["alert_count"]
                res={}
                labels=[]
                dt=[]
                for i in data:
                    labels.append(i["event_code"])
                    dt.append(i["count"])
                res["labels"]=labels
                res["data"]=dt
                return Response(res, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK +
                      Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message": "Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['GET'])
def high_risk_count(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                args=dict(request.GET)
                dashUrl = settings.url+"dashboard/high_risk_count"
                page = requests.get(dashUrl, params=args, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                try:
                    data = data["high_risk_count"]
                    res={}
                    labels=[]
                    dt=[]
                    for i in data:
                        labels.append(i["hostname"])
                        dt.append(i["count"])
                    res["labels"]=labels
                    res["data"]=dt
                except:           
                    res=data
                return Response(res, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK +
                      Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message": "Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['GET'])
def weekly_alerts_count(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                dashUrl = settings.url+"dashboard/weekly_alerts_count"
                page = requests.get(dashUrl, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                lst=[]
                for i in data.keys():
                    data[i]["name"]=i
                    lst.append(data[i])
                return Response(lst, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK +
                      Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message": "Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['GET'])
def node_alert_count(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                dashUrl = settings.url+"dashboard/node_alert_count"
                page = requests.get(dashUrl, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                return Response(data, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK +
                      Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message": "Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)