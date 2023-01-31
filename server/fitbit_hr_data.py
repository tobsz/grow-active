import json
import requests

def get_active_minutes():
    with open("tokens/tokens.json") as f:
        token_dict = json.load(f)
    access_token = token_dict['access_token']

    date: str = "today"
    headers = {
        "Authorization": f"Bearer {access_token}",
    }
    response = requests.get(f"https://api.fitbit.com/1/user/-/activities/heart/date/{date}/1d.json", headers=headers)
    print(response.status_code)
    hr_dict = json.loads(response.content)
    # print(hr_dict)
    activity_minutes = hr_dict \
        .get('activities-heart')[0] \
        .get('value') \
        .get('customHeartRateZones')[1] \
        .get('minutes')

    print(f"active minutes: {activity_minutes}")
    return activity_minutes

if __name__ == "__main__":
    print(get_active_minutes())
