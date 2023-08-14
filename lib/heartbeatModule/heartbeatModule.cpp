#include <Arduino.h>
#include "heartbeatModule.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

void changeHeartbeatState(void *pvParameters);

const int Stack_Size = 10;
const int blinkLED = 32;
bool previousState;

/*
 * Function:  setupHeartbeatGPIO
 * --------------------
 * Function that initialised the GPIO selected for use as a heartbeat output.               
 */

void setupHeartbeatGPIO(void)
{
    pinMode(blinkLED, OUTPUT);
    digitalWrite(blinkLED, LOW);
    previousState = false;
}

/*
 * Function:  setupHeartbeatInterrupt
 * --------------------
 * Creates the timer, settings up the callback function and the name of the timer.    
 * The callback function services the heartbeat output for the device.      
 * The stackdepth 500 seems to be the Minimum value to enable the task to function           
 */

void setupHeartbeatInterrupt(void)
{
    xTaskCreate(changeHeartbeatState, "Blink", 750, NULL, 1, NULL);
}

/*
 * Function:  changeHeartbeatState
 * --------------------
 * A function that is called via the callback timer periodically to service the heartbeat output.                     
 */

void changeHeartbeatState(void *pvParameters)
{
    while(1)
    {
        if (previousState)
        {
            digitalWrite(blinkLED, LOW);
            previousState = false;
        }
        else
        {
            digitalWrite(blinkLED, HIGH);
            previousState = true;
        }
        vTaskDelay(1000);
    }
        
}