#ifndef RGB_STATUS_MODULE_H
#define RGB_STATUS_MODULE_H
class RGBStatus
{
    public:
        void setupRGBIndicator();
        void changeRGBLEDColour();
        void displayColour(int R, int G, int B);
        bool checkRGBStatus ();
        //static void toggleRGBPin(void *pvParameters,RGBStatus* rgb);
        RGBStatus(float minTemp, float maxTemp, float maxHumid, float minHumid);
        RGBStatus();
        void toggleStart();
        bool ToggledOn;
    private:
        enum RGBEnum
        {
            AllGood,
            TemperatureThresholdExceeded,
            HumidityThresholdExceeded,
            BothThresholdsExceeded
        };
        int RED_PIN = GPIO_NUM_12;
        int GREEN_PIN = GPIO_NUM_27;
        int BLUE_PIN = GPIO_NUM_14;
        int PWM_FREQ = 5000;
        int PWM_RES = 8;
        int RED_CHANNEL = 1;
        int GREEN_CHANNEL = 2;
        int BLUE_CHANNEL = 3;
        float minTemp;
        float maxTemp;
        float maxHumid;
        float minHumid;
        RGBEnum status;
};
   
#endif