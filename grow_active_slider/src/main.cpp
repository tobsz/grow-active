#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char *ssid = "thepromisedLAN";
const char *password = "67192893961044349985";

const char *serverName = "http://p14s:8000/activity";

const int BUFLEN = 10;
int buf[BUFLEN];
int bufStart = 0;
int bufEnd = 1;

void putCB(int item) {
  buf[bufStart++] = item;
  bufStart %= BUFLEN;
}

int getCB() {
  int item = buf[bufEnd++];
  bufEnd %= BUFLEN;
  return item;
}




void httpPost(int minutesVal) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/json");

    String buffer = "{\"minutes\":" + String(minutesVal) + "}";
    int httpResponseCode = http.POST(buffer);
    Serial.printf("HTTP response code: %d\n", httpResponseCode);
    http.end();
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected to WiFi network with IP address:");
  Serial.println(WiFi.localIP());

  pinMode(A0, INPUT);
}

void loop() {
  int pot = analogRead(A0) / 34;
  putCB(pot);
  Serial.println(pot);
  if (pot == getCB()) {
    httpPost(pot);
    int curPot = pot;
    while (pot == curPot) {
      pot = analogRead(A0) / 34;
      delay(100);
    }
  }
  delay(100);
}