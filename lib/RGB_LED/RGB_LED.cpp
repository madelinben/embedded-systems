#include "RGB_LED.h"
#include <Arduino.h>

RGB_LED::RGB_LED() {

}

// config(12, 13, 14);
// GPIO Configuration
void RGB_LED::config(int RED_PIN, int GREEN_PIN, int BLUE_PIN) {
    this->RED_PIN = RED_PIN;
    this->GREEN_PIN = GREEN_PIN;
    this->BLUE_PIN = BLUE_PIN;
}

// setup(0, 1, 2);
// LED Channel must be less than 15.
void RGB_LED::setup(int RED_CHANNEL, int GREEN_CHANNEL, int BLUE_CHANNEL) {
    ledcAttachPin(this->RED_PIN, RED_CHANNEL);
    ledcAttachPin(this->GREEN_PIN, GREEN_CHANNEL);
    ledcAttachPin(this->BLUE_PIN, BLUE_CHANNEL);
    ledcSetup(RED_CHANNEL, this->PWM_FREQUENCY, this->PWM_RESOLUTION);
    ledcSetup(GREEN_CHANNEL, this->PWM_FREQUENCY, this->PWM_RESOLUTION);
    ledcSetup(BLUE_CHANNEL, this->PWM_FREQUENCY, this->PWM_RESOLUTION);
    this->RED_CHANNEL = RED_CHANNEL;
    this->GREEN_CHANNEL = GREEN_CHANNEL;
    this->BLUE_CHANNEL = BLUE_CHANNEL;
    this->lastTimestamp = millis();
}

void RGB_LED::displayColor(int RED_VALUE, int GREEN_VALUE, int BLUE_VALUE) {
    ledcWrite(this->RED_CHANNEL, RED_VALUE);
    ledcWrite(this->GREEN_CHANNEL, GREEN_VALUE);
    ledcWrite(this->BLUE_CHANNEL, BLUE_VALUE);
}

bool RGB_LED::intervalFulfilled(unsigned long startTimestamp, int specifiedDelay) {
    return (millis() - startTimestamp >= specifiedDelay);
}