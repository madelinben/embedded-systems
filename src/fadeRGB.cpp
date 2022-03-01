#include <Arduino.h>
#include "RGB_LED.h"

RGB_LED* LED = new RGB_LED();

// Basic Task Scheduler
const int DELAY_MS = 5;
unsigned long lastTimestamp;

void setup() {
    Serial.begin(115200);

    LED->config(12, 13, 14);
    LED->setup(0, 1, 2);

    lastTimestamp = millis();
}

void loop() {
    unsigned int colour[3]; //RGB Value Array
    colour[0] = 255; // RED
    colour[1] = 0; // GREEN
    colour[2] = 0; // BLUE
    
    // Cross-Fade Intensity Value
    for (int dec = 0; dec < 3; dec += 1) {
        int inc = dec == 2 ? 0 : dec + 1;
        for(int i = 0; i < 255; i += 1) {
            colour[dec] -= 1;
            colour[inc] += 1;
            
            if (LED->intervalFulfilled(lastTimestamp, DELAY_MS)) {
                LED->displayColor(colour[0], colour[1], colour[2]);
                lastTimestamp = millis();
            }
        }
    }
}