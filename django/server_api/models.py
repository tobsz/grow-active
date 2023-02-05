from django.db import models


class Activity(models.Model):
    date = models.DateField(auto_now_add = True, auto_now = False, blank = False, primary_key = True)
    minutes = models.IntegerField(default = 0, blank = False)

    def __str__(self):
        return f"{self.date}: {self.minutes}"