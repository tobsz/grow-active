from django.http import JsonResponse
import json
from .models import ActivityData
#from .serializers import ActivityDataSerializer
from cfehome.fetch_minutes import fetch_data

def Activity(request):
    #activity=ActivityData.objects.all()
    #serializer=ActivityDataSerializer(activity,many=True)
    #print(serializer.data)
    return JsonResponse({"minutes":fetch_data()%31+9},safe=False)
    
