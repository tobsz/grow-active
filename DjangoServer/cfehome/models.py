from django.db import models

class ActivityData(models.Model):
    minute_count=models.IntegerField()