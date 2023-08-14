#include <Arduino.h>
#include "Settings.h"
#include <memory>
#include <EEPROM.h>

void Settings::setDefaultValues(){
    this->minHumidity = this->defaultMinHumidity;
    this->maxHumidity = this->defaultMaxHumidity;
    this->minTemperature = this->defaultMinTemperature;
    this->maxTemperature = this->defaultMaxTemperature;
}

Settings::Settings(){
    EEPROM.begin(EEPROM_SIZE);
    this->setDefaultValues();
    this->getUserSettings();
}

void Settings::getUserSettings() {
    float target;
    int addressPointer = 0;
    addressPointer += sizeof(this->minHumidity);
    this->maxHumidity = EEPROM.read(addressPointer);
    addressPointer += sizeof(this->maxHumidity);
    this->minTemperature = EEPROM.read(addressPointer);
    addressPointer += sizeof(this->minTemperature);
    this->maxTemperature = EEPROM.read(addressPointer);
}

void Settings::updateUserSettings() {
    int addressPointer = 0;
    EEPROM.write(addressPointer, this->minHumidity);
    addressPointer += sizeof(this->minHumidity);
    EEPROM.write(addressPointer, this->maxHumidity);
    addressPointer += sizeof(this->maxHumidity);
    EEPROM.write(addressPointer, this->minTemperature);
    addressPointer += sizeof(this->minTemperature);
    EEPROM.write(addressPointer, this->maxTemperature);
    addressPointer += sizeof(this->maxTemperature);
    EEPROM.commit();
}