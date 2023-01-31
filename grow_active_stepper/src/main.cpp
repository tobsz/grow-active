#include <Arduino.h>
#include <AccelStepper.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <WiFi.h>

const int STEP = 19;
const int ENABLE = 22;
const int DIR = 23;

// const char* ssid = "Pixel 6";
// const char* password = "t6bzeq2knmyr6yc";
const char* ssid = "S8";
const char* password = "internetsucks";
String serverName = "http://192.168.43.205:8000/activity";

String serverReadings;
int activeMinutes = 0;
int currentActiveMinutes = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

Preferences preferences;
AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR);

String httpGETRequest(String serverName) {
  WiFiClient client;
  HTTPClient http;
  Serial.printf("Start http request.\n");
    
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

void printPosition() {
  Serial.printf("Current position: %d\n", stepper.currentPosition());
}

void goToPosition(long pos) {
  // Serial.printf("Move to new position. Current position is %ld\n", stepper.currentPosition());
  stepper.enableOutputs();
  delay(50);
  stepper.moveTo(pos);
  stepper.runToPosition();
  printPosition();
  preferences.begin("stepper", false);
  preferences.putLong("position", stepper.currentPosition());
  stepper.disableOutputs();
  delay(50);
  delay(3000);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n\nConnected to WiFi network with IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  connectToWiFi();

  preferences.begin("stepper", true);
  long current_position = preferences.getLong("position", 0L);
  preferences.end();
  delay(1000);
  stepper.setCurrentPosition(current_position);
  Serial.printf("Starting position: %ld\n", stepper.currentPosition());

  stepper.setEnablePin(ENABLE);
  stepper.setPinsInverted(false, false, true);
  stepper.setMaxSpeed(1000.0F);
  stepper.setAcceleration(100.0F);
  delay(1000);
}

void fetchAndMaybeMove() {
  if (WiFi.status() == WL_CONNECTED) {
    serverReadings = httpGETRequest(serverName);
    JSONVar myObject = JSON.parse(serverReadings);

    JSONVar keys = myObject.keys();
    activeMinutes = int(myObject[keys[0]]);
    Serial.println(activeMinutes);
    // if (activeMinutes != currentActiveMinutes) {
      goToPosition(activeMinutes * 100);
    //   currentActiveMinutes = activeMinutes;
    // }
  }
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    fetchAndMaybeMove();
    lastTime = millis();
  }
}