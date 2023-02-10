#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

enum direction_t {
  DOWN,
  UP,
};

struct valueAndDir {
  int value;
  direction_t dir;
};

direction_t dir = UP;
int lastVal;

const char *ssid = "thepromisedLAN";
const char *password = "67192893961044349985";
const char *serverName = "http://grow-active:8000/activity";
// const char *ssid = "Pixel 6";
// const char *password = "t6bzeq2knmyr6yc";
// const char *serverName = "http://192.168.82.119:8000/activity";

const size_t BUFLEN = 10;
struct valueAndDir buffer[BUFLEN];
size_t bufStart;
size_t bufEnd;

void resetCB() {
  for (size_t i = 0; i < BUFLEN; ++i) {
    buffer[i].value = -1;
    buffer[i].dir = UP;
  }
  bufStart = 0;
  bufEnd = 1;
}

void putCB(int item, direction_t dir) {
  buffer[bufStart].value = item;
  buffer[bufStart].dir = dir;
  ++bufStart %= BUFLEN;
}

valueAndDir getCB() {
  valueAndDir valAndDir = buffer[bufEnd];
  ++bufEnd %= BUFLEN;
  return valAndDir;
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

int readValue() {
  // int factor = 34; // for 30 steps
  int factor = 169; // for 7 steps
  return analogRead(A0) / factor * 5;
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
  resetCB();
  lastVal = -1;
}

void loop() {
  int value = readValue();
  if (value > lastVal && dir == DOWN) {
    dir = UP;
    resetCB();
  } else if (value < lastVal && dir == UP) {
    dir = DOWN;
    resetCB();
  }
  valueAndDir vad = getCB();
  putCB(value, dir);
  Serial.println(value);
  if (value == vad.value && value == lastVal && dir == vad.dir) {
    httpPost(value);
    while (value == readValue()) {
      delay(50);
    }
  } else {
    delay(150);
  }
  lastVal = value;
}