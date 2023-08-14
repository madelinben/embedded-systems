#ifndef SETTINGS_H
#define SETTINGS_H
#include <Arduino.h>
#include <memory>

class Settings
{
    private:
        const int EEPROM_SIZE = 4;
    public:
        int minHumidity;
        int maxHumidity;
        int minTemperature;
        int maxTemperature;
        const int defaultMinHumidity = 0;
        const int defaultMaxHumidity = 100;
        const int defaultMinTemperature = 5;
        const int defaultMaxTemperature = 30;
        void setDefaultValues();
        void getUserSettings();
        void updateUserSettings();
        Settings();
        enum SettingAttr{
            MinHumid,
            MaxHumid,
            MinTemp,
            MaxTemp
        };
};


#endif /* SETTINGS_H */