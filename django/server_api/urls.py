# from django.conf.urls import url
from django.urls import path, include
from .views import (
    ActivityView,
    ActivityTodayView,
)

urlpatterns = [
    path('activity', ActivityView.as_view()),
    path('today', ActivityTodayView.as_view())
]