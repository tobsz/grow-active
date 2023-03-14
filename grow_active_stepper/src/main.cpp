#include <Arduino.h>
#include <AccelStepper.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <WiFiClientSecure.h>

const int STEP = 19;
const int ENABLE = 22;
const int DIR = 23;

const int FACTOR = 69;

// const char *ssid = "thepromisedLAN";
// const char *password = "67192893961044349985";
// const char *url = "https://growactive:8000/today";
// const char *ssid = "Pixel 6";
// const char *password = "t6bzeq2knmyr6yc";
const char *ssid = "CityLAB-Guest";
const char *password = "Tempelhof";
const char *url = "https://werkverzeichnis.orijauch.de/today";
// const char* ssid = "S8";
// const char* password = "internetsucks";
// String serverName = "https://192.168.82.119:8000/today";

const char *test_root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"-----END CERTIFICATE-----\n";

String serverReadings;
int activeMinutes = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

Preferences preferences;
AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR);

String httpGETRequest() {
  String payload = "{}"; 
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client->setCACert(test_root_ca);
    HTTPClient https;
    if (https.begin(*client, url)) {
      int httpResponseCode = https.GET();
      if (httpResponseCode == HTTP_CODE_OK) {
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
        payload = https.getString();
      } else {
        Serial.printf("Error code: %d\n", httpResponseCode);
      }
    }
    https.end();
  }
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
  WiFi.mode(WIFI_STA);
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
      goToPosition(activeMinutes * FACTOR);
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
  stepper.setAcceleration(50.0F);
  delay(500);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    fetchAndMaybeMove();
    lastTime = millis();
  }
}