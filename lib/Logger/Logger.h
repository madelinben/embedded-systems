#ifndef LOGGER_H_
#define LOGGER_H_
#include "Globals.h"
#include "DHT11Module.h"
#include "rtc.h"
#include <FS.h>

extern void setupLogger();

/* class Logger {
    private:
        int EEPROM_SIZE = 4;
        int CS_PIN = 5;
        const char* path = "/logs/data.txt";
        unsigned long getCurrentEpoch();
        // void logSPIFFS();
        // void logSD();
        // void formatJSON();
        // void listDir();
        // File readFile(fs::FS &fs, const char * path);
        // void writeFile(fs::FS &fs, const char* path, const char* buffer);
        // void appendFile(fs::FS &fs, const char* path, const char* buffer);
        // void deleteFile(fs::FS &fs, const char* path);
    public:
        Logger();
        // void setupWifiConnection();
        // void getUserSettings();
        // void updateUserSettings();
}; */

#endif /* LOGGER_H_ */