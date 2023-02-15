from rest_framework import serializers
from .models import Activity

class ActivitySerializer(serializers.ModelSerializer):
    class Meta:
        model = Activity
        fields = ["date", "minutes"]
    
    def update(self, instance: Activity, validated_data):
        instance.date = validated_data.get('date', instance.date)
        instance.minutes = validated_data.get('minutes', instance.minutes)
        instance.save()
        return super().update(instance, validated_data)