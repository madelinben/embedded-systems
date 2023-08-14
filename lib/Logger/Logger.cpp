#include "Logger.h"
#include <Arduino.h>

#include <EEPROM.h>
#include <SPIFFS.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <time.h>
#include <ArduinoJson.h>

#include "Globals.h"
#include "DHT11Module.h"
#include "rtc.h"
#include "ScreenConfig.h"

#include <WiFi.h>
#include <HTTPClient.h>

#define CS_PIN 5
const char* path = "/logs/data.txt";
const char* url = "";
DynamicJsonDocument documentJson(1024);

unsigned long getCurrentEpoch() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return 0;
    }
    time(&now);
    return now;
}

void listDir(const char* path) {
    Serial.printf("Directory: %s\n", path);
    File root = SPIFFS.open(path);
    if (!(root || root.isDirectory())) {
        Serial.println("Failed to open directory.");
    } else {
        File file = root.openNextFile();
        while (file) {
            if (file.isDirectory()) {
                Serial.print("Dir: ");
                Serial.println(file.name());
            } else {
                Serial.print("File: ");
                Serial.print(file.name());
                Serial.print(", Size: ");
                Serial.println(file.size());
            }
            file = root.openNextFile();
        }
    }
}

void readFile(const char* path) {
    Serial.printf("Reading file: %s\n", path);
    File file = SPIFFS.open(path);
    if (file) {
        Serial.print("Read from file: ");
        while (file.available()) {
            Serial.write(file.read());
        }
        file.close();
    } else {
        Serial.println("Failed to open file.");
    }
}

void writeFile(const char* path, const char* buffer) {
    Serial.printf("Writing file: %s\n", path);
    File file = SPIFFS.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file.");
    } else {
        if (file.print(buffer)) {
            Serial.println("Finished writing content to file.");
        } else {
            Serial.println("Failed to write content to file.");
        }
        file.close();
    }
}

void appendFile(const char* path, const char* buffer) {
    Serial.printf("Appending to file: %s\n", path);
    File file = SPIFFS.open(path, FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file.");
    } else {
        if (file.print(buffer)) {
            Serial.println("Finished appending content to file.");
        } else {
            Serial.println("Failed to append content to file.");
        }
        file.close();
    }
}

void deleteFile(const char* path) {
    Serial.printf("Deleting file: %s\n", path);
    if(SPIFFS.remove(path)){
        Serial.println("File deleted.");
    } else {
        Serial.println("Failed to delete file.");
    }
}

void formatJsonPayload() {
        JsonObject dataObject = documentJson.createNestedObject("data");
        dataObject.createNestedArray("readings");
        DateTime currentTimestamp = getRTCCurrentDateTime();
        char timestampBuffer[32]; // YYYY-MM-DD hh:mm:ss
        sprintf(timestampBuffer, "%04d-%02d-%02d %02d:%02d:%02d", currentTimestamp.year(), currentTimestamp.month(), currentTimestamp.day(), currentTimestamp.hour(), currentTimestamp.minute(), currentTimestamp.second());
        dataObject["timestamp"] = timestampBuffer;
        dataObject["selfTest"]["Dht11"] = false;
        dataObject["selfTest"]["RotaryEncoder"] = false;
        dataObject["selfTest"]["Screen"] = false;
        dataObject["selfTest"]["SdCard"] = false;
        dataObject["selfTest"]["Ldr"] = false;
        dataObject["selfTest"]["Rtc"] = false;

        // serializeJson(documentJson, Serial);
        // Serial.println(documentJson.memoryUsage());
}

static void logLatestState(void* pvParameters) {
    while(1) {
        vTaskDelay(10000 / portTICK_PERIOD_MS);

        // unsigned long currentEpoch = getCurrentEpoch();
        float currentTemperature = getCurrentTemperatureSensorState();
        float currentHumidity = getCurrentHumiditySensorState();
        DateTime currentTimestamp = getRTCCurrentDateTime();
        char timestampBuffer[32]; // YYYY-MM-DD hh:mm:ss
        sprintf(timestampBuffer, "%04d-%02d-%02d %02d:%02d:%02d", currentTimestamp.year(), currentTimestamp.month(), currentTimestamp.day(), currentTimestamp.hour(), currentTimestamp.minute(), currentTimestamp.second());
        JsonObject readingObject = documentJson["data"]["readings"].createNestedObject();
        readingObject["temperature"] = currentTemperature;
        readingObject["humidity"] = currentHumidity;
        readingObject["timestamp"] = timestampBuffer;
        documentJson["data"]["selfTest"]["Dht11"] = ErrorStatus->DHT11Error;
        documentJson["data"]["selfTest"]["RotaryEncoder"] = ErrorStatus->RotaryEncoderError;
        documentJson["data"]["selfTest"]["Screen"] = ErrorStatus->ScreenError;
        documentJson["data"]["selfTest"]["SdCard"] = ErrorStatus->SDCardError;
        documentJson["data"]["selfTest"]["Ldr"] = ErrorStatus->LRDError;
        documentJson["data"]["selfTest"]["Rtc"] = ErrorStatus->RTCError;

        // serializeJson(documentJson, Serial);
        // Serial.println(documentJson.memoryUsage());

        char formattedString[100] = ""; /* 35 */
        snprintf_P(formattedString, sizeof(formattedString), PSTR("%.2f, %.2f, %lu\r\n"), currentTemperature, currentHumidity, timestampBuffer);
        // Serial.println(formattedString);

        /* digitalWrite(TFT_CS, HIGH);
        digitalWrite(SD_CS, LOW);

        // SPISettings tftTransation(2700000, MSBFIRST, SPI_MODE1);
        SPISettings sdTransaction(1000000, LSBFIRST, SPI_MODE3);
        SPI.beginTransaction(sdTransaction); */

        /* if (!SD.begin(SD_CS, SPI)) {
            Serial.println("Failed to mount SD Card.");
        } else {
            if (SD.exists("/logs/data.txt")) {
                appendFile("/logs/data.txt", formattedString);
            } else {
                writeFile("/logs/data.txt", "Temperature, Humidity, Timestamp\r\n");
            }
        } */

        if (!SPIFFS.begin(true)) {
            Serial.println("Failed to mount SD Card.");
        } else {
            if (SPIFFS.exists("/logs/data.txt")) { // (!File f = SPIFFS.open("/logs/data.txt"))
                appendFile("/logs/data.txt", formattedString);
            } else {
                writeFile("/logs/data.txt", "Temperature, Humidity, Timestamp\r\n");
            }
        }

        readFile("/logs/data.txt");

        /* // SPI.endTransaction(); 
        digitalWrite(SD_CS, HIGH);
        digitalWrite(TFT_CS, LOW); */
    }
}

static void updateServer(void* pvParameters) {
    while(1) {
        vTaskDelay(30000 / portTICK_PERIOD_MS);

        // serializeJson(documentJson, Serial);
        // Serial.println(documentJson.memoryUsage());

        HTTPClient http;
        WiFiClient client;
        String payload;
        serializeJson(documentJson, payload);
        http.begin(client, url);
        http.addHeader("Content-Type", "application/json");
        http.POST(payload);
        Serial.println(http.getString());

        documentJson["data"]["readings"].clear();
        documentJson["data"]["timestamp"] = nullptr;
        documentJson["data"]["selfTest"]["Dht11"] = false;
        documentJson["data"]["selfTest"]["RotaryEncoder"] = false;
        documentJson["data"]["selfTest"]["Screen"] = false;
        documentJson["data"]["selfTest"]["SdCard"] = false;
        documentJson["data"]["selfTest"]["Ldr"] = false;
        documentJson["data"]["selfTest"]["Rtc"] = false;
        
        http.end();
    }
}

/*
 * Function:  getTempHumid
 * --------------------
 * Gets the temperature and humidity values from thr DHT11.   
 */
static void formatDebugString(void* pvParameters) {
    while(1) {
        float temp = getCurrentTemperatureSensorState();
        float humid = getCurrentHumiditySensorState();
        Serial.println("Debug String: ");
        Serial.print("Temperature: "); Serial.print(temp, 2); Serial.println("*C");
        Serial.print("Humidity: "); Serial.print(humid, 2); Serial.println("%");
        Serial.print("System Status: ");
        if (((settings->minTemperature < temp) && (temp < settings->maxTemperature)) && ((settings->minHumidity < humid) && (humid < settings->maxHumidity))) {
            Serial.println("System Ok.");
        } else if (!(((temp > settings->minTemperature) && (temp < settings->maxTemperature)) && ((humid > settings->minHumidity) && (humid < settings->maxHumidity)))) {
            Serial.println("Both Invalid.");
        } else if (!((temp > settings->minTemperature) && (temp < settings->maxTemperature))) {
            Serial.println("Temperature Invalid.");
        } else if (!((humid > settings->minHumidity) && (humid < settings->maxHumidity))) {
            Serial.println("Humidity Invalid.");
        } else {
            Serial.println("Error validating sensor readings.");
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void setupLogger() {
    formatJsonPayload();

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
    } else {
        Serial.print("SD Card Type: ");
        if (cardType == CARD_MMC) {
            Serial.println("MMC");
        } else if (cardType == CARD_SD) {
            Serial.println("SDSC");
        } else if (cardType == CARD_SDHC) {
            Serial.println("SDHC");
        } else {
            Serial.println("UNKNOWN");
        }

        uint64_t cardSize = SD.cardSize() / (1024 * 1024);
        Serial.printf("SD Card Size: %lluMB\n", cardSize);
    }

    xTaskCreate(formatDebugString, "DebugString", 10000, NULL, 1, NULL);
    xTaskCreate(logLatestState, "SensorLog", 10000, NULL, 1, NULL);
    // xTaskCreate(updateServer, "ServerUpdate", 10000, NULL, 1, NULL);
    Serial.println("Logger Initialised.");
}