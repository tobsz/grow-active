#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Preferences.h>

// const char* ssid = "Pixel 6";
// const char* password = "t6bzeq2knmyr6yc";
const char* ssid = "thepromisedLAN";
const char* password = "67192893961044349985";

// String serverName = "http://192.168.10.119:8008";
String serverName = "http://p14s:8008";
String serverReadings;
int activeMinutes = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

String httpGETRequest(String serverName) {
  WiFiClient client;
  HTTPClient http;
    
  http.begin(client, serverName);
  
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n\nConnected to WiFi network with IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      serverReadings = httpGETRequest(serverName);
      JSONVar myObject = JSON.parse(serverReadings);

      JSONVar keys = myObject.keys();
      activeMinutes = int(myObject[keys[0]]);
      Serial.print(keys[0]);
      Serial.print(" = ");
      Serial.println(activeMinutes);
    }
    lastTime = millis();
  }
}