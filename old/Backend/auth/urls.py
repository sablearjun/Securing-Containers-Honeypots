from django.contrib import admin
from django.urls import path, include

urlpatterns = [
    path('admin/', admin.site.urls),
    path('node/', include('node.urls')),
    path('config/', include('config.urls')),
    path('dashboard/', include('dashboard.urls')),
    path('event/', include('event.urls')),
    path('scheduled_queries/', include('ad_hoc.urls'))
]
