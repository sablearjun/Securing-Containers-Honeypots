import json
import ast
from django import template
from django.shortcuts import render, redirect
from django.contrib.auth.models import User, auth
from django.contrib import messages
from django.http import Http404
from django.core.paginator import Paginator
import requests
import random

register = template.Library()


@register.inclusion_tag("json.html")
def pretty_json(value):
    max_int = 9223372036854775807
    page = json.dumps(value, indent=4)
    page = json.loads(page)
    res = ast.literal_eval(page)
    parents = res["parents"][0]
    _id = random.randint(1, max_int)
    return {
        "res": res,
        "parents": parents,
        "id": _id
    }


@register.inclusion_tag("jsonified.html")
def jsonified(value):
    page = json.dumps(value)
    res = ast.literal_eval(page)
    data = json.loads(res)
    data = data["schedule"]
    # print(f"\n\n\n{res}\n\n\n")
    return {
        "res": data
    }
