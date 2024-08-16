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

@api_view(['GET'])
def configList(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                configurl = settings.url+"config/list"
                page = requests.get(configurl, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                return Response(data, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['GET'])
def tableList(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                configurl = settings.url+"config/table_name"
                page = requests.get(configurl, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                return Response(data, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)

@api_view(['POST'])
def configAdd(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                configurl = settings.url+"config/add"
                try:
                    data =  request.data["data"]
                    data=json.loads(data)
                except:
                    data =  request.data
                    data=json.loads(data)
                data["config"]=data["query"]
                data=json.dumps(data)
                page = requests.post(configurl, data=data, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                return Response(data, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)


@api_view(['POST'])
def configUpdate(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                configurl = settings.url+"config/update"
                try:
                    data =  request.data["data"]
                    data=json.loads(data)
                except:
                    data =  request.data
                    data=json.loads(data)
                data["schedule"]=data["query"]["schedule"]
                data=json.dumps(data)
                page = requests.post(configurl, data=data, headers=settings.inspector, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                return Response(data, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)


@api_view(['DELETE'])
def configDelete(request):
    try:
        user = request.user
        if user.is_authenticated:
            try:
                delcon = settings.url+"config/delete"
                try:
                    data =  request.data["data"]
                    data =  json.loads(data)
                except:
                    data =  request.data
                    data =  json.loads(data)
                data['id']=int(data['id'])
                data=json.dumps(data)
                page = requests.delete(delcon, headers=settings.inspector, data=data, verify=False)
                page = page.json()
                page = json.dumps(page)
                data = json.loads(page)
                return Response(data, status=status.HTTP_200_OK)
            except Exception as e:
                print(Style.BRIGHT + Back.BLACK + Fore.RED + f"\n[-] Exception: {e}\n")
                return Response({"message":"Something went Wrong"}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)
    except Http404:
        return Response({"message": "User is not authenticated"}, status=status.HTTP_403_FORBIDDEN)