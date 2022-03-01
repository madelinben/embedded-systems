#ifndef RGB_LED_H_
#define RGB_LED_H_
class RGB_LED {
    public:
        int RED_PIN = 12;
        int GREEN_PIN = 13;
        int BLUE_PIN = 14;
        int RED_CHANNEL = 0;
        int GREEN_CHANNEL = 1;
        int BLUE_CHANNEL = 2;
        int PWM_FREQUENCY = 5000;
        int PWM_RESOLUTION = 8;
        RBG_LED();
        void config(int RED_PIN, int GREEN_PIN, int BLUE_PIN);
        void setup(int RED_CHANNEL, int GREEN_CHANNEL, int BLUE_CHANNEL);
        void displayColor(int RED_VALUE, int GREEN_VALUE, int BLUE_VALUE);
    private:
        int DELAY_MS = 5;
        unsigned long lastTimestamp;
        bool intervalFulfilled(unsigned long startTimestamp, int specifiedDelay);
};

#endif /* RGB_LED_H_ */