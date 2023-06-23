from django.shortcuts import render

def index(request):
    return render(request, 'smoke_detector/index.html')

def login(request):
    return render(request, 'smoke_detector/login.html')

def profile(request):
    return render(request, 'smoke_detector/profile.html')

def setting(request):
    return render(request, 'smoke_detector/setting.html')

def stadistics(request):
    return render(request, 'smoke_detector/stadistics.html')

def transaction_detail(request):
    return render(request, 'smoke_detector/transaction-detail.html')
