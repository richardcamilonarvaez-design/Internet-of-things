from django.db import models

# Create your models here.

class Sensor(models.Models):
    name=models.CharField(max_length=50)
