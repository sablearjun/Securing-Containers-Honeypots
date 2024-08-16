from django.urls import path
# from knox import views as knox_views
from . import views

urlpatterns = [
    path('historical_data/', views.HistoricalData),
    path('graph/', views.Graph),
    path('graphtable/', views.GraphTable),
]