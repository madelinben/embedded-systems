#include <Arduino.h>
#include "DHT11Module.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include <DHT.h>
#include "Settings.h"
#include "RGBStatusModule.h"
#include "Globals.h"

#include "Screen.h"
#include "Stack.h"
#include <TFT_eSPI.h>

#define DHTTYPE DHT11
const int DHT11Pin = GPIO_NUM_0;

DHT dht(DHT11Pin, DHTTYPE);

float CurrentTemperature;
float CurrentHumidity;
void updateCurrentSensorState(void *pvParameters);

/*
 * Function:  startDHT11
 * --------------------
 * Initialises the DHT11 and starts the Task to service the DHT11.             
 */
void startDHT11(void)
{
    dht.begin();
    CurrentTemperature = 0;
    CurrentHumidity = 0;
   // startStatusIndicatorServices();
    xTaskCreate(updateCurrentSensorState, "DHT11", 10000, NULL, 1, NULL);
}

/* void startStatusIndicatorServices()
{
    settings = new Settings();
    settings->setupDefaultValues();
    RGBIndicator = new RGBStatus();
    RGBIndicator->setupRGBIndicator();
} */

/*
 * Function:  updateCurrentSensorState
 * --------------------
 * Function that is called by a task, which gets the DHT11 temperature and humidity, and updates the current state of temp and humid if valid.           
 */
void updateCurrentSensorState(void *pvParameters)
{
    while(1)
    {
        bool sensorUpdate = true;
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();
        if (isnan(humidity) == false)
        {
            CurrentHumidity = humidity;
        } else {
            sensorUpdate = false;
        }
        if(isnan(temperature) == false)
        {
            CurrentTemperature = temperature;
        } else {
            sensorUpdate = false;
        }
        if ((screen->screenStack->peek() == "displayTempHumid") && sensorUpdate) {
            screen->displayTempHumid();
        }
        vTaskDelay(1000);
    }
} 

/*
 * Function:  getCurrentTemperatureSensorState
 * --------------------
 * returns the float of the current sensor temperature.              
 */
/* void getCurrentTemperatureSensorState(int *temperature)
{
    *temperature = CurrentTemperature;
} */

/*
 * Function:  getCurrentHumiditySensorState
 * --------------------
 * returns the float of the current sensor humidity.           
 */
/* void getCurrentHumiditySensorState(int *humidity)
{
    *humidity = CurrentHumidity;
} */

float getCurrentTemperatureSensorState(void)
{
    return CurrentTemperature;
}

float getCurrentHumiditySensorState(void)
{
    return CurrentHumidity;
}