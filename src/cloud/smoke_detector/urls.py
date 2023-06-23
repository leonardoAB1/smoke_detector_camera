from django.urls import path
from . import views

app_name = 'smoke_detector'

urlpatterns = [
    path('', views.index, name='index'),
    path('login/', views.login, name='login'),
    path('profile/', views.profile, name='profile'),
    path('setting/', views.setting, name='setting'),
    path('stadistics/', views.stadistics, name='stadistics'),
    path('transation-detail/', views.transation_detail, name='transation_detail'),
]
