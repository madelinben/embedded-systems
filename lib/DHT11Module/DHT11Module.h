#include <Arduino.h>
#ifndef DHT11_MODULE_H
#define DHT11_MODULE_H

void startDHT11(void);
float getCurrentTemperatureSensorState(void);
float getCurrentHumiditySensorState(void);

//std::shared_ptr<Settings> settings = nullptr;

#endif