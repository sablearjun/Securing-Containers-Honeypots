from django.urls import path
from . import views

urlpatterns = [
    path('list/', views.EventList),
    path('update/', views.Update),
    path('query/', views.Query),
    path('graph/', views.Graph),
    # path('event_forward_process_tree/', views.forwardGraph)
]