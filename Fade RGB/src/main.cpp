#include <Arduino.h>

// GPIO Configuration
const int RED_PIN = 12;
const int GREEN_PIN = 13;
const int BLUE_PIN = 14;

// PWM Properties
const int RED_CHANNEL = 0;
const int GREEN_CHANNEL = 1;
const int BLUE_CHANNEL = 2;

const int PWM_FREQ = 5000;
const int PWM_RES = 8;

void setup() {
    Serial.begin(115200);
    
    ledcAttachPin(RED_PIN, RED_CHANNEL);
    ledcAttachPin(GREEN_PIN, GREEN_CHANNEL);
    ledcAttachPin(BLUE_PIN, BLUE_CHANNEL);

    ledcSetup(RED_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(GREEN_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(BLUE_CHANNEL, PWM_FREQ, PWM_RES);
}

void loop() {
  // put your main code here, to run repeatedly:
}