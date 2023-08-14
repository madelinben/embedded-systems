#include <Arduino.h>
#include <heartbeatModule.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include <rotaryEncoderModule.h>
#include <Adafruit_Sensor.h>
#include "Screen.h"
#include "Stack.h"
#include <stdlib.h>
#include <DHT11Module.h>
#include <RGBStatusModule.h>
#include "Settings.h"
#include "Globals.h"
#include "Logger.h"
#include "RTClib.h"
#include "rtc.h"
#include "WiFiSetup.h"

char menu[][Screen::maxItemSize] = {"Display Readings", "Set Min Humidity", "Set Max Humidity", "Set Min Temperature", "Set Max Temperature", "BACK"};
RGBStatus* RGBIndicator = new RGBStatus(5.00, 50.00, 0.00, 80.00); //Make sure to pass the settings thresholds to the RGB status module.
TaskHandle_t RGBFlashingHandle = NULL;
Settings::SettingAttr settingsEnum;

static void toggleRGBPin(void * pvParameters)
{
  while (1)
  {
    if (RGBIndicator->ToggledOn)
    {
        RGBIndicator->ToggledOn = false;
        RGBIndicator->displayColour(0, 0, 0); //RGBFLASH should not return a value.
    }
    else
    {
        RGBIndicator->ToggledOn = true;
        RGBIndicator->displayColour(256, 0, 0);
        vTaskDelay(250);
        RGBIndicator->displayColour(0, 0, 256);
    }
    vTaskDelay(250);
  }
}

void setup() {
  Serial.begin(115200);
  setCpuFrequencyMhz(240);
  setupHeartbeatGPIO();
  setupHeartbeatInterrupt();
  setupEncoderGPIOPins();
  setupTimer();
  setupPinInterupts();
  // setupWiFi();
  screen->setup();
  startDHT11();
  screen->displayMenu(menu, sizeof(menu)/Screen::maxItemSize, 0);
  bool success = startRTC();
  if (!success)
  {
    ErrorStatus->RTCError = true;
    Serial.println("Error starting RTC.");
  }
  if (ErrorStatus->RTCError == false)
  {
    syncCurrentDateTime(DateTime(F(__DATE__), F(__TIME__))); //This needs to be adjusted to the time of the WIFI Internet date time
    Serial.println("Synced time.");
  }
  RGBIndicator->setupRGBIndicator();
  setupLogger();
  Serial.println("Setup Complete.");
}

void loop() {
  EncoderMovement encoderState = getEncoderState();
  EncoderMovement buttonState = getEncoderButtonState();
  if (encoderState == EncoderAntiClockwiseMovement)
  {
    if(screen->screenStack->peek() == "displayMenu"){
      screen->arrowY -= 1;
      screen->displayMenu(menu, sizeof(menu)/Screen::maxItemSize, 0);
    } else if ((screen->screenStack->peek() == "setMinHumid") || (screen->screenStack->peek() == "setMaxHumid") || (screen->screenStack->peek() == "setMinTemp") || (screen->screenStack->peek() == "setMaxTemp")) {
      screen->tempValue -= 1;
      switch(settingsEnum){
        case Settings::SettingAttr::MinHumid:
          if(screen->tempValue < settings->defaultMinHumidity){
            screen->tempValue = settings->defaultMinHumidity;
          }
          break;
        case Settings::SettingAttr::MinTemp:
          if(screen->tempValue < settings->defaultMinTemperature){
            screen->tempValue = settings->defaultMinTemperature;
          }
          break;
        case Settings::SettingAttr::MaxHumid:
          if(screen->tempValue <= settings->minHumidity){
            screen->tempValue = settings->minHumidity + 1;
          }
          break;
        case Settings::SettingAttr::MaxTemp:
          if(screen->tempValue <= settings->minTemperature){
            screen->tempValue = settings->minTemperature + 1;
          }
          break;
      }
      screen->displaySetTempHumid();
    }
  }
  if ((encoderState == EncoderClockwiseMovement))
  {
    if(screen->screenStack->peek() == "displayMenu"){
      screen->arrowY += 1;
      screen->displayMenu(menu, sizeof(menu)/Screen::maxItemSize, 0);
    } else if ((screen->screenStack->peek() == "setMinHumid") || (screen->screenStack->peek() == "setMaxHumid") || (screen->screenStack->peek() == "setMinTemp") || (screen->screenStack->peek() == "setMaxTemp")) {
      screen->tempValue += 1;
      switch(settingsEnum){
        case Settings::SettingAttr::MinHumid:
          if(screen->tempValue >= settings->maxHumidity){
            screen->tempValue = settings->maxHumidity - 1;
          }
          break;
        case Settings::SettingAttr::MinTemp:
          if(screen->tempValue >= settings->maxTemperature){
            screen->tempValue = settings->maxTemperature - 1;
          }
          break;
        case Settings::SettingAttr::MaxHumid:
          if(screen->tempValue > settings->defaultMaxHumidity){
            screen->tempValue = settings->defaultMaxHumidity;
          }
          break;
        case Settings::SettingAttr::MaxTemp:
          
          if(screen->tempValue > settings->defaultMaxTemperature){
            screen->tempValue = settings->defaultMaxTemperature;
          }
          break;
      }
      screen->displaySetTempHumid();
    }
  }

  if (buttonState == EncoderButtonClicked)
  { 
    if((screen->screenStack->peek() == "displayTempHumid") || (screen->screenStack->peek() == "displaySetTempHumid")){
      screen->screenStack->pop();
      screen->displayMenu(menu, sizeof(menu)/Screen::maxItemSize, 0);
    }
    else if (screen->arrowY == 0 && screen->screenStack->peek() == "displayMenu") {
      screen->screenStack->pop();
      screen->displayTempHumid();
    } else if (screen->arrowY == 1) {
      if (screen->screenStack->peek() == "displayMenu") {
        screen->tempValue = settings->minHumidity;
        if (screen->screenStack->peek() != "setMinHumid") {
          screen->screenStack->push("setMinHumid");
          settingsEnum = Settings::SettingAttr::MinHumid;
        }
        screen->displaySetTempHumid();
      } else if (screen->screenStack->peek() == "setMinHumid") {
        settings->minHumidity = screen->tempValue;
        settings->updateUserSettings();
        screen->screenStack->pop();
        screen->displayMenu(menu, sizeof(menu)/Screen::maxItemSize, 0);
      }
    } else if (screen->arrowY == 2) {
      if (screen->screenStack->peek() == "displayMenu") {
        screen->tempValue = settings->maxHumidity;
        if (screen->screenStack->peek() != "setMaxHumid") {
          screen->screenStack->push("setMaxHumid");
          settingsEnum = Settings::SettingAttr::MaxHumid;
        }
        screen->displaySetTempHumid();
      } else if (screen->screenStack->peek() == "setMaxHumid") {
        settings->maxHumidity = screen->tempValue;
        settings->updateUserSettings();
        screen->screenStack->pop();
        screen->displayMenu(menu, sizeof(menu)/Screen::maxItemSize, 0);
      }
    } else if (screen->arrowY == 3) {
      if (screen->screenStack->peek() == "displayMenu") {
        screen->tempValue = settings->minTemperature;
        if (screen->screenStack->peek() != "setMinTemp") {
          screen->screenStack->push("setMinTemp");
          settingsEnum = Settings::SettingAttr::MinTemp;
        }
        screen->displaySetTempHumid();
      } else if (screen->screenStack->peek() == "setMinTemp") {
        settings->minTemperature = screen->tempValue;
        settings->updateUserSettings();
        screen->screenStack->pop();
        screen->displayMenu(menu, sizeof(menu)/Screen::maxItemSize, 0);
      }
    } else if (screen->arrowY == 4) {
      if (screen->screenStack->peek() == "displayMenu") {
        screen->tempValue = settings->maxTemperature;
        if (screen->screenStack->peek() != "setMaxTemp") {
          screen->screenStack->push("setMaxTemp");
          settingsEnum = Settings::SettingAttr::MaxTemp;
        }
        screen->displaySetTempHumid();
      } else if (screen->screenStack->peek() == "setMaxTemp") {
        settings->maxTemperature = screen->tempValue;
        settings->updateUserSettings();
        screen->screenStack->pop();
        screen->displayMenu(menu, sizeof(menu)/Screen::maxItemSize, 0);
      }
    }
  }
  bool flashing = RGBIndicator->checkRGBStatus();
  if (flashing && RGBFlashingHandle == NULL)
  {
    xTaskCreate(toggleRGBPin, "RGBFlash", 10000, NULL, 1, &RGBFlashingHandle);
  }
  else
  {
    if(RGBFlashingHandle != NULL)
    {
      vTaskDelete(RGBFlashingHandle);
    }
  }
}