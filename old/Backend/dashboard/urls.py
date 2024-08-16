from django.urls import path
# from knox import views as knox_views
from . import views

urlpatterns = [
    path('nodes_count/', views.nodes_count, name='nodes_count'),
    path('events_count/', views.events_count, name='events_count'),
    path('os_count/', views.os_count, name='os_count'),
    path('severity_count/', views.severity_count, name='severity_count'),
    path('alert_count/', views.alert_count, name='alert_count'),
    path('high_risk_count/', views.high_risk_count, name='high_risk_count'),
    path('weekly_alerts_count/', views.weekly_alerts_count, name='weekly_alerts_count'),
    path('node_alert_count/', views.node_alert_count, name='node_alert_count')
]
