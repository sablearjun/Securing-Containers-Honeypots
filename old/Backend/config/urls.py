from django.urls import path
# from knox import views as knox_views
from . import views

urlpatterns = [
    path('list/', views.configList, name='configList'),
    path('add/', views.configAdd, name='configAdd'),
    path('update/', views.configUpdate, name='configUpdate'),
    path('delete/', views.configDelete, name='configDelete'),
    path('table_name/', views.tableList, name='tableList'),   
]