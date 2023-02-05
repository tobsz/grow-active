from rest_framework import serializers
from.models import ActivityData
class ActivityDataSerializer(serializers.ModelSerializer):
    class Meta:
        model=ActivityData
        fields= ["minute_count"]