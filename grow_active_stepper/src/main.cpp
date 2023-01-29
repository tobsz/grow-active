#include <Arduino.h>
#include <AccelStepper.h>
#include <Preferences.h>

Preferences preferences;

const int STEP = 19;
const int ENABLE = 22;
const int DIR = 23;

AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR);

void printPosition() {
  Serial.printf("Current position: %d\n", stepper.currentPosition());
}

void goToPosition(long pos) {
  stepper.moveTo(pos);
  stepper.runToPosition();
  printPosition();
  preferences.begin("stepper", false);
  preferences.putLong("position", stepper.currentPosition());
}

void setup() {
  Serial.begin(115200);

  preferences.begin("stepper", true);
  long current_position = preferences.getLong("position", 0L);
  preferences.end();
  delay(1000);
  stepper.setCurrentPosition(current_position);
  Serial.printf("Starting position: %ld\n", stepper.currentPosition());

  stepper.setEnablePin(ENABLE);
  stepper.setPinsInverted(false, false, true);
  stepper.setMaxSpeed(1000.0F);
  stepper.setAcceleration(10.0F);
  delay(1000);
  stepper.enableOutputs();
  for (size_t i = 0; i < 5; ++i) {
    if (stepper.currentPosition() < i * 10000) {
      goToPosition(i * 10000);
    }
  }
  goToPosition(0);
}

void loop() {
}