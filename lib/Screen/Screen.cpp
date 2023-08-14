#include "Screen.h"
#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Stack.h"
#include "DHT11Module.h"
#include "rotaryEncoderModule.h"
#include "Settings.h"
#include "Globals.h"
#include <SD.h>
#include "ScreenConfig.h"

/*
 * Function:  Screen
 * --------------------
 * Screen Constructor to initialise TFT.     
 */
Screen::Screen() {

}

/*
 * Function:  setup
 * --------------------
 * Setting up and initialising the TFT.   
 */
void Screen::setup() {
    /* // const int SCREEN_CS = 5;
    // const int SD_CS = 15;
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);

    // SPI.begin();
    // digitalWrite(SD_CARD_CS, LOW);
    // SPIClass* spi = new SPIClass(HSPI); // FSPI VSPI HSPI
    // spi.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, SD_CARD_CS);
    if (!SD.begin(SD_CS, SPI)) {
        Serial.println("Failed to mount SD Card.");
    }
    // digitalWrite(SD_CARD_CS, HIGH);
    // spi->end();
    // SPI.begin(); */

    tft.init();
    tft.initDMA();
    tft.fillScreen(TFT_BLACK);

    memoryPointer[0] = (uint16_t*)buffer[0].createSprite(this->width, this->height);
    memoryPointer[1] = (uint16_t*)buffer[1].createSprite(this->width, this->height);
    buffer[0].setViewport(0, 0, this->width, this->height);
    buffer[1].setViewport(0, 0, this->width, this->height);
    buffer[0].setTextDatum(MC_DATUM);
    buffer[1].setTextDatum(MC_DATUM);

    buffer[0].fillScreen(TFT_BLUE);
    buffer[1].fillScreen(TFT_RED);
    buffer[0].fillSprite(TFT_BLUE);
    buffer[1].fillSprite(TFT_RED);

    tft.startWrite();
    this->screenStack = new Stack(10);
    this->screenStack->push("initial");
}

void Screen::updateBuffer() {
    this->tft.pushImageDMA(0, 0, this->width, this->height, memoryPointer[active]);
    active = !active;
}

/*
 * Function:  displayTempHumid
 * --------------------
 * Displays Temperature and Humidity values to screen.
 */
void Screen::displayTempHumid(){
    if(this->screenStack->peek() != "displayTempHumid"){
        this->screenStack->push("displayTempHumid");
    }

    float tempValue = getCurrentTemperatureSensorState();
    float humidValue = getCurrentHumiditySensorState();
    Serial.println(tempValue);
    Serial.println(humidValue);

    buffer[active].fillScreen(TFT_BLACK);
    const int pixelMargin = this->height / 5;
    const int charWidth = 6;

    Serial.println("Display Sensor.");
    buffer[active].setCursor(this->width/2-(6*charWidth), pixelMargin);
    buffer[active].print("Temperature");
    buffer[active].setCursor(this->width/2-(4*charWidth), pixelMargin*2);
    buffer[active].print(tempValue, 2);
    buffer[active].setCursor(this->width/2-(4*charWidth), pixelMargin*3);
    buffer[active].println("Humidity");
    buffer[active].setCursor(this->width/2-(3*charWidth), pixelMargin*4);
    buffer[active].print(humidValue, 2);
    updateBuffer();
}

/*
 * Function:  displayMenu
 * --------------------
 * Outputs menu to screen.
 */
void Screen::displayMenu(char menuInput[][Screen::maxItemSize], int menuLength, int initialCursor){
    if(this->screenStack->peek() != "displayMenu"){
        this->screenStack->push("displayMenu");
    }

    buffer[active].fillScreen(TFT_BLACK);
    const int fontSize = 1;
    auto fsMultiplier = fontSize;
    fsMultiplier *= 10;

    const int charWidth = 6;

    buffer[active].setCursor(2*charWidth, initialCursor*fsMultiplier);

    Serial.println("Output Menu");

    for(int i = 0; i< menuLength; i++){
        int counter = i + 1;
        if(i == this->arrowY && this->arrowY >= 0)
        {
            if(i == 0){
                buffer[active].setCursor(0, initialCursor*fsMultiplier);
            }
            else{
                buffer[active].setCursor(0,i*fsMultiplier);
            }
            buffer[active].print("> ");
        }
        else if (this->arrowY <= 0) {
            this->arrowY = 0;
            Serial.println("Menu pointer less than 0.");
        } else if(this->arrowY >= menuLength){
            this->arrowY = menuLength - 1;
            Serial.println("Menu pointer greater than item size.");
        }

        buffer[active].println(menuInput[i]);
        buffer[active].setCursor(2*charWidth, counter*fsMultiplier);
    }
    updateBuffer();
}

void Screen::displaySetTempHumid() {
    buffer[active].fillScreen(ST7735_BLACK);
    buffer[active].setCursor(this->width/2,this->width/2);
    buffer[active].println(this->tempValue);
    updateBuffer();
}