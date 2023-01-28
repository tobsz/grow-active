# How to create and refresh OAuth2 tokens?

Fitbit provides a public web API for fetching data collected by their the smartwatches (e.g. Fitbit Sense). To get access to the *Heart Rate Time Series* API endpoint, it is necessary to first authorize using the *OAuth 2.0 Authorization Framework*, which can be done as follows:

1. Register a new app at [dev.fitbit.com/new](dev.fitbit.com/new). Most fields can be filled with placeholders, but it's important to use *Personal* as the *OAuth 2.0 Application Type*, and to specify an HTTPS *Redirect URL* (e.g. [https://localhost:8080](https://localhost:8080), which does not necessarily to be implemented/reachable yet). As the *Default Access Type* *"Read Only"* should suffice.
2. After registering the new app, one can go to [dev.fitbit.com/apps](dev.fitbit.com/apps) and click the newly created app. On the next page there are two important tokens the **OAuth 2.0 Client ID** and the **Client Secret**. Store these somewhere and go to and follow [Fitbit's OAuth 2.0 Tutorial](https://dev.fitbit.com/build/reference/web-api/troubleshooting-guide/oauth2-tutorial/).
3. Following along the tutorial will create you two tokens which are important for the continous access the the endpoints, the **Access Token** and the **Refresh Token**
4. With the **Access Token** one can retrieve the heart rate data with  
   ```
   curl -i -H "Authorization: Bearer <Access Token>"  \
   https://api.fitbit.com/1/user/-/activities/heart/date/today/1d.json
   ```  
   The response will be a JSON containing the desired data.
5. The **Access Token** is only valid for 8 hours, afterwards it's necessary to generate a new pair of **Access & Refresh Token** using the **Refresh Token** with:  
   ```
   curl -i -X POST \
   https://api.fitbit.com/oauth2/token \
   -H "Authorization: Basic <Basic Token>"  \
   -H "Content-Type: application/x-www-form-urlencoded"  \
   --data "grant_type=refresh_token"  \
   --data "client_id=<ClientID>" \
   --data "<Refresh Token>"
   ```
   where the **Basic Token** is generated with `base64encode(client_id + ":" + client_secret)`.
