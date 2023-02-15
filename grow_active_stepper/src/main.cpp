#include <Arduino.h>
#include <AccelStepper.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <WiFi.h>

const int STEP = 19;
const int ENABLE = 22;
const int DIR = 23;

// const char *ssid = "thepromisedLAN";
// const char *password = "67192893961044349985";
// const char *url = "http://growactive:8000/today";
const char* ssid = "Pixel 6";
const char* password = "t6bzeq2knmyr6yc";
const char *url = "https://werkverzeichnis.orijauch.de/today";
// const char* ssid = "S8";
// const char* password = "internetsucks";
// String serverName = "http://192.168.82.119:8000/today";

String serverReadings;
int activeMinutes = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

Preferences preferences;
AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR);

String httpGETRequest() {
  HTTPClient http;
  String payload = "{}"; 

  http.begin(url);

  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    payload = http.getString();
  } else {
    Serial.printf("Error code: %d\n", httpResponseCode);
  }
  http.end();

  return payload;
}

void printPosition() {
  Serial.printf("Current position: %d\n", stepper.currentPosition());
}

void goToPosition(long pos) {
  Serial.printf("Move to new position: %ld\n", pos);
  stepper.enableOutputs();
  delay(50);
  stepper.moveTo(pos);
  stepper.runToPosition();
  printPosition();
  preferences.begin("stepper", false);
  preferences.putLong("position", stepper.currentPosition());
  stepper.disableOutputs();
  Serial.println("Arrived.");
  delay(50);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected to WiFi network with IP address: ");
  Serial.println(WiFi.localIP());
}

void fetchAndMaybeMove() {
  if (WiFi.status() == WL_CONNECTED) {
    int currentMinutes = activeMinutes;
    serverReadings = httpGETRequest();
    JSONVar myObject = JSON.parse(serverReadings);
    JSONVar keys = myObject.keys();
    activeMinutes = int(myObject[keys[1]]);
    Serial.println(activeMinutes);
    if (activeMinutes != currentMinutes) {
      goToPosition(activeMinutes * 69);
    }
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();

  preferences.begin("stepper", true);
  long current_position = preferences.getLong("position", 0L);
  preferences.end();
  delay(500);
  stepper.setCurrentPosition(current_position);
  Serial.printf("Starting position: %ld\n", stepper.currentPosition());

  stepper.setEnablePin(ENABLE);
  stepper.setPinsInverted(true, false, true);
  stepper.setMaxSpeed(100.0F);
  stepper.setAcceleration(10.0F);
  delay(500);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    fetchAndMaybeMove();
    lastTime = millis();
  }
}