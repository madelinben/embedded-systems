#include <Arduino.h>

// GPIO Configuration
const int RED_PIN = 12;
const int GREEN_PIN = 13;
const int BLUE_PIN = 14;

// PWM Properties
const int RED_CHANNEL = 0;
const int GREEN_CHANNEL = 1;
const int BLUE_CHANNEL = 2;

const int PWM_FREQUENCY = 5000;
const int PWM_RESOLUTION = 8;

// Millis Timeout
const int DELAY_MS = 5;
unsigned long lastTimestamp;

boolean intervalFulfilled(unsigned long startTimestamp, int specifiedDelay) {
	return (millis() - startTimestamp >= specifiedDelay);
}

void displayColor(int R, int G, int B) {
  ledcWrite(RED_CHANNEL, R);
  ledcWrite(GREEN_CHANNEL, G);
  ledcWrite(BLUE_CHANNEL, B);
}

void setup() {
    Serial.begin(115200);
    
    ledcAttachPin(RED_PIN, RED_CHANNEL);
    ledcAttachPin(GREEN_PIN, GREEN_CHANNEL);
    ledcAttachPin(BLUE_PIN, BLUE_CHANNEL);

    ledcSetup(RED_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(GREEN_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(BLUE_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);

    lastTimestamp = millis();
}

void loop() {
  unsigned int colour[3]; //RGB
  colour[0] = 255;
  colour[1] = 0;
  colour[2] = 0;

  // Cross-Fade Intensity Value
  for (int dec = 0; dec < 3; dec += 1) {
    int inc = dec == 2 ? 0 : dec + 1;
    for(int i = 0; i < 255; i += 1) {
      colour[dec] -= 1;
      colour[inc] += 1;

      if (intervalFulfilled(lastTimestamp, DELAY_MS)) {
        displayColor(colour[0], colour[1], colour[2]);
        lastTimestamp = millis();
      }
    }
  }
}