#include <Arduino.h>
#include "RGBStatusModule.h"
#include <DHT11Module.h>

bool ToggledOn;

RGBStatus::RGBStatus()
{

}

RGBStatus::RGBStatus(float minTemp, float maxTemp, float minHumid, float maxHumid)
{
    this->minTemp = minTemp;
    this->maxTemp = maxTemp;
    this->minHumid = minHumid;
    this->maxHumid = maxHumid;
}

void RGBStatus::setupRGBIndicator(void)
{
    ledcAttachPin(RED_PIN, RED_CHANNEL);
    ledcAttachPin(GREEN_PIN, GREEN_CHANNEL);
    ledcAttachPin(BLUE_PIN, BLUE_CHANNEL);
    ledcSetup(RED_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(GREEN_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(BLUE_CHANNEL, PWM_FREQ, PWM_RES);
    displayColour(0, 255, 0);
    this->ToggledOn = false;
    this->status = RGBEnum::AllGood;
}

void RGBStatus::displayColour(int R, int G, int B)
{
    ledcWrite(RED_CHANNEL, R);
    ledcWrite(GREEN_CHANNEL, G);
    ledcWrite(BLUE_CHANNEL, B);
}

//max humid 80%, max temp 50C
bool RGBStatus::checkRGBStatus ()
{
    RGBEnum previousState = this->status;
    bool tempExceeded = false;
    bool humidExceeded = false;

    float currentTemperature = getCurrentTemperatureSensorState();
    float currentHumidity = getCurrentHumiditySensorState();
    if (currentTemperature > maxTemp || currentTemperature < minTemp )
    {
        displayColour(255, 0, 0);
        tempExceeded = true;
        this-> status = RGBEnum::TemperatureThresholdExceeded;
    }
    if (currentHumidity > maxHumid || currentHumidity < minHumid)
    { 
        humidExceeded = true;
        if (tempExceeded == true)
        {
            displayColour(255, 0, 255);
            this->status = RGBEnum::BothThresholdsExceeded;
        }
        else 
        {
            displayColour(0, 0, 255);
            this->status = RGBEnum::HumidityThresholdExceeded;
        }
    }
    if (tempExceeded == false && humidExceeded == false)
    {
        this->status = RGBEnum::AllGood;
        displayColour(0, 255, 0);
    }
    if(previousState != this->status){
        switch(this->status)
        {        
            case 0:
                Serial.println("System within threshold parameters.");
                break;
            case 1:
                Serial.println("Temperature Threshold Exceeded.");
                break;
            case 2:
                Serial.println("Humidity Threshold Exceeded.");
                break;
            case 3:
                Serial.println("Both Thresholds Exceeded.");
                break;        
            default:
                Serial.println("RGB Status Exception. Invalid Status.");
                break;
        }
    }
    if (tempExceeded && humidExceeded)
    {
        return true;
    }
    else
    {
        return false;
    }
}