import requests
import json
import base64
def get_access_token():
    with open(r'D:\Conda_Projects\ARapp-main\cfehome\tokens.json') as f:
        token_dict = json.load(f)

    client_id: str = token_dict['client_id']
    client_secret: str = token_dict['client_secret']
    access_token: str = token_dict['access_token']
    refresh_token: str = token_dict['refresh_token']
    # basic_token is necessary for the authorization for token refresh request
    basic_token: str = base64.b64encode(f"{client_id}:{client_secret}".encode()).decode()
    # print(basic_token)
    header = {
        "Authorization": f"Basic {basic_token}",
        "Content-Type": "application/x-www-form-urlencoded",
    }

    data = {
        "grant_type": "refresh_token",
        "client_id": client_id,
        "refresh_token": refresh_token,
    }

    response_dict = json.loads(requests.post("https://api.fitbit.com/oauth2/token", headers=header, data=data).content)
    #print(response_dict)
    access_token = response_dict['access_token']
    refresh_token = response_dict['refresh_token']
    # print(refresh_token)

    token_dict.update({'refresh_token': refresh_token, 'access_token': access_token})

    with open(r"D:\Conda_Projects\ARapp-main\cfehome\tokens.json", "w") as f:
        json.dump(token_dict, f, ensure_ascii=False, indent=4)

    return access_token

get_access_token()