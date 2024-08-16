from django.urls import path
# from knox import views as knox_views
from . import views

urlpatterns = [
    path('schedule/', views.scheduleQuery, name='schedule_query'),
    path('status/', views.scheduleQueryStatus, name='schedule_query_status'),
    path('response/', views.scheduleQueryResponse, name='schedule_query_response'),
]