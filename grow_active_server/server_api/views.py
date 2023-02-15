from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from .models import Activity
from .serializers import ActivitySerializer
from datetime import date


class ActivityView(APIView):
    def get(self, request, *args, **kwargs):
        """
        List all stored activity minutes
        """
        activities = Activity.objects
        serializer = ActivitySerializer(activities, many = True)
        return Response(serializer.data, status=status.HTTP_200_OK)
    
    def post(self, request, *args, **kwargs):
        """
        Create or update the minutes for the current date
        """
        data = {
            'date': date.today(),
            # 'date': request.data.get('date'),
            'minutes': request.data.get('minutes'),
        }

        try:
            # update branch
            activity = Activity.objects.get(pk=data['date'])
            serializer = ActivitySerializer(activity, data=data)
        except Activity.DoesNotExist:
            # create branch
            serializer = ActivitySerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data, status=status.HTTP_201_CREATED)
        
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

class ActivityTodayView(APIView):
    def get(self, request, *args, **kwargs):
        """
        Get the activity minutes of the current day
        """
        activity = Activity.objects.get(date=date.today())
        print(activity)
        serializer = ActivitySerializer(activity)
        print(serializer.data)
        return Response(serializer.data, status=status.HTTP_200_OK)