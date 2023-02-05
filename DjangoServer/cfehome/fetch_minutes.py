import json
import base64
from cfehome.refreshToken import get_access_token

def fetch_data():

    import requests

    date: str = "today"
    headers = {
        "Authorization": f"Bearer {get_access_token()}",
    }
    response = requests.get(f"https://api.fitbit.com/1/user/-/activities/heart/date/{date}/1d.json", headers=headers)
    #print(response.status_code)
    hr_dict = json.loads(response.content)
    #print(hr_dict)
    activity_minutes = hr_dict.get('activities-heart')[0].get('value').get('customHeartRateZones')[1].get('minutes')

    print(activity_minutes)
    return activity_minutes

fetch_data()