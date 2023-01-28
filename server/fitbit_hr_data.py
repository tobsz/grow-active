import fitbit

def get_active_minutes():
    client_id = "2397W8"
    client_secret = "5cd1462e3f53b47c65d18e2cf5d3b063"
    access_token = "eyJhbGciOiJIUzI1NiJ9.eyJhdWQiOiIyMzk3VzgiLCJzdWIiOiJCREZZOFoiLCJpc3MiOiJGaXRiaXQiLCJ0eXAiOiJhY2Nlc3NfdG9rZW4iLCJzY29wZXMiOiJyY2YgcmhyIHJhY3QiLCJleHAiOjE2NzQwNDQ2OTMsImlhdCI6MTY3NDAxNTg5M30.IWm2CMq9GbHvqEJJWnZvtT2gmriwX1gZLTX4K_otwmo"
    refresh_token = "dfbad91bc69c10f80f8f1d74df8ecd91036eccae727cf817f17bcbd21f9f342a"

    client = fitbit.Fitbit(
        client_id,
        client_secret,
        access_token=access_token,
        refresh_token=refresh_token
    )

    # date = "today"
    date = "2023-01-17"

    url = f"{client.API_ENDPOINT}/1/user/-/activities/heart/date/{date}/1d.json"

    hr_data = client.make_request(url)
    return hr_data \
        .get('activities-heart')[0] \
        .get('value') \
        .get('customHeartRateZones')[1] \
        .get('minutes')

if __name__ == "__main__":
    print(get_active_minutes())
