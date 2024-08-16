"""Os_query URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.urls import path
from . import views

urlpatterns = [
    path('', views.node),
    path('home/', views.node, name='home'),
    path('stats/', views.stats, name='stats'),
    path('events/', views.events, name='events'),
    path('nodeEX/', views.node_extended, name='info'),
    path('loginuser/', views.loginuser),
    path('logoutuser/', views.logout_user),
    path('reIN/', views.reIN, name='reIN'),
    path('config/', views.config, name='config'),
    path('makeclose/', views.makeclose, name='makeclose'),
    path('makeopen/', views.makeopen, name='makeopen'),
    path('delete/', views.delete, name='delete'),
    path('addcon/', views.addcon, name='addcon'),
    path('pagup/', views.pagup, name='pagup'),
    path('pagdn/', views.pagdn, name='pagdn'),
    path('delcon/', views.delcon, name='delcon'),
    path('upcon_view/', views.upcon_view, name='upcon_view'),
    path('upcon/', views.upcon, name='upcon'),
    path('addhawk/', views.addhawk, name='addhawk'),
    path('addhawk_page/', views.addhawk_page, name='addhawk_page'),
    path('blocklist/', views.blocklist, name='blocklist'),
    path('addhawK/', views.addhawK, name='addhawK'),
    path('addhawkStatus/', views.addhawkStatus, name='addhawkStatus'),
    path('addhawKstatus/', views.addhawKstatus, name='addhawKstatus'),
    path('addhawkResults/', views.addhawkResults, name='addhawkResults'),
    path('addhawKresults/', views.addhawKresults, name='addhawKresults'),
    path('graph/', views.graph, name='graph'),
    path('statistics/', views.statistics,name='statistics'),
    path('nodecon/', views.nodecon ,name='nodecon')
]
