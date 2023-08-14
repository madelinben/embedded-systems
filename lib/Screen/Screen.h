#ifndef SCREEN_H_
#define SCREEN_H_
#include <TFT_eSPI.h>
#include "Stack.h"
#include "Settings.h"

class Screen {
    private:
        TFT_eSPI tft = TFT_eSPI();
        TFT_eSprite buffer[2] = {TFT_eSprite(&tft), TFT_eSprite(&tft)};
        uint16_t* memoryPointer[2];
        bool active = 0;
        int width = 128;
        int height = 128;
    public:
        static const int maxItemSize = 20;
        Stack* screenStack;
        int arrowY = 0;
        int tempValue = 0;
        Screen();
        void setup();
        void updateBuffer();
        void debugTimer();
        void displayMenu(char menuInput[][Screen::maxItemSize], int menuLength, int initialCursor);
        void displayTempHumid();
        void displaySetTempHumid();
};

#endif /* SCREEN_H_ */