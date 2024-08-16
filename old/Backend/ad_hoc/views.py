from django.shortcuts import render
from django.contrib.auth import user_logged_out
from django.contrib.auth.models import User
from django.http import Http404
# from knox import crypto
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
def scheduleQuery(request):
    try:
        user = request.user
        data = request.data
        try:
            hawkurl = settings.url+"scheduled_queries/schedule"
            hostides = []
            query = data["query"]
            hosts = str(data["hosts"]).split(",")
            for i in hosts:
                hostides.append({"host_identifier": i})
            hosts = hostides
            queryinit = {"hosts": hosts, "query": query}
            queryinit = json.dumps(queryinit)
            page = requests.post(hawkurl, headers=settings.inspector, data=queryinit, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            return Response(data, status=status.HTTP_200_OK)
        except Exception as e:
            print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
            return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['POST'])
def scheduleQueryStatus(request):
    try:
        user = request.user
        try:
            hawkresurl = settings.url+"scheduled_queries/status"
            req_data = json.dumps(request.data)
            page = requests.post(hawkresurl, headers=settings.inspector, data=req_data, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            return Response(data, status=status.HTTP_200_OK)
        except Exception as e:
            print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
            return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['POST'])
def scheduleQueryResponse(request):
    try:
        user = request.user
        data = request.data
        try:
            hawkersurl = settings.url+"scheduled_queries/response"
            req_data = {"config_id_list": data["config_id_list"]}
            req_data = json.dumps(req_data)
            page = requests.post(hawkersurl, headers=settings.inspector, data=req_data, verify=False)
            page = page.json()
            page = json.dumps(page)
            data = json.loads(page)
            return Response(data, status=status.HTTP_200_OK)
        except Exception as e:
            print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
            return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)