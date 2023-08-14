#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H
#define ENCODER_CONTACT_DEBOUNCE 2 //Debounce Sensitivity AB PIN
#define ENCODER_BUTTON_DEBOUNCE 300 //Debounce Sensiticity BTN PIN
#include <Arduino.h>


enum EncoderMovement
{
    EncoderNoMovement = 0,
    EncoderClockwiseMovement = 1,
    EncoderAntiClockwiseMovement = 2,
    EncoderButtonClicked = 3 
};

void setupEncoderGPIOPins(void);
void setupPinInterupts(void);
void setupTimer(void);
EncoderMovement getEncoderState(void);
EncoderMovement getEncoderButtonState(void);

#endif