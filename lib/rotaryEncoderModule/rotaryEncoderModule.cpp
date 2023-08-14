#include <rotaryEncoderModule.h>
#include <Arduino.h>
#include <Ticker.h>

#define SIG_QUEUE_DEPTH 4

enum EncoderSigTypes 
{
    EncNoSig = 0,
    EncA_PlusSig = 1,
    EncA_MinusSig = 3,
};

int encoderPinA = GPIO_NUM_36;
int encoderPinB = GPIO_NUM_39;
int encoderButtonPin = GPIO_NUM_34;

volatile int debounceCountLowA;
volatile int debounceCountHighA;
volatile int debounceCountHighC;
volatile bool pinAState;
volatile bool pinStateChanged;
volatile bool newEncoderState;
volatile bool buttonClicked;

volatile EncoderMovement encoderState = EncoderNoMovement;
volatile EncoderMovement prevEncoderState = EncoderNoMovement;

hw_timer_t * timer = NULL;

void pinAChanged(void);
void buttonPressed(void);
void IRAM_ATTR timerOneISR(void);

/*
 * Function:  setupEncoderGPIOPins
 * --------------------
 * Function that initialises the variables, and sets up the GPIO for the encoder       
 */

void setupEncoderGPIOPins(void)
{
    debounceCountLowA = 0;
    debounceCountHighA = 0;
    debounceCountHighC = 0;
    pinAState = false;
    pinStateChanged = false;
    buttonClicked = false;
    newEncoderState = false;
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);
    pinMode(encoderButtonPin, INPUT);
}

/*
 * Function:  setupTimer
 * --------------------
 *  Function that sets up the timer that is used for debouncing the input from the encoder.
 *  This timer calls timerOneIRS every 100uS for added resolution.
 */

void setupTimer(void)
{
    // Configure Prescaler to 80, as our timer runs @ 80Mhz
	// Giving an output of 240,000,000 / 240 = 1,000,000 ticks / second
	timer = timerBegin(0, 80, true);  
    timerAttachInterrupt(timer, &timerOneISR, true);
	timerAlarmWrite(timer, 100, true);			
	timerAlarmEnable(timer);   
}

/*
 * Function:  setupPinInterrupts
 * --------------------
 * Function that sets up and starts the inputs for the selected GPIO
 */

void setupPinInterupts(void)
{
    attachInterrupt(encoderPinA, pinAChanged, CHANGE);
    attachInterrupt(encoderButtonPin, buttonPressed, CHANGE);
}

/*
 * Function:  getEncoderButtonState
 * --------------------
 * Function that returns the state of the encoder button.
 * If the button was pressed when the state was read, it is then set to false so as to allow for it to be updated by another button request.     
 */

EncoderMovement getEncoderButtonState(void)
{
    if (buttonClicked == true)
    {
        buttonClicked = false;
        return(EncoderButtonClicked);
    }
    else
    {
        return(EncoderNoMovement);
    }
}

/*
 * Function:  getEncoderState
 * --------------------
 * Function that returns the state of the encoder only if the previous state is the same.   
 * This allows for slightly increased movement accuracy and is very necessary          
 */

EncoderMovement getEncoderState(void)
{
    if(newEncoderState==true)
    {    
        newEncoderState = false;
        if(encoderState == prevEncoderState)
        {
            prevEncoderState = encoderState;
            return(encoderState);
        }
        else
        {
            prevEncoderState = encoderState;
            return(EncoderNoMovement);
        }
    }
    else
    {
        return(EncoderNoMovement);
    }
}

/*
 * Function:  updateSigQueue
 * --------------------
 * Function that is called by the timer which checks if there has been a change of state on    
 * the encoder inputs, and states which direction the encoder is travelling.  
 * Only update this if the newEncoderState is false.           
 */

void updateSigQueue(EncoderSigTypes newSignal)
{
    if(newEncoderState ==false)
    {
        if(newSignal==EncA_PlusSig)
        {
            if(digitalRead(encoderPinB) == LOW)    
            {
                encoderState= EncoderAntiClockwiseMovement;
            }
            else
            {
                encoderState = EncoderClockwiseMovement;
            }
            newEncoderState = true;
        }
    }
}

/*
 * Function:  TimerOneISR
 * --------------------
 *  Function that is called by the PIN Interrupt setup earlier which debounces the pin state
 *  and updates the Menu signal queue
 */

void IRAM_ATTR timerOneISR(void)
{
    /* deal with pins going low */
    if(debounceCountLowA)
    {
        debounceCountLowA=debounceCountLowA-1;
        if(debounceCountLowA==0)
        {// timer has timed out pin a has been low long enoug
            updateSigQueue(EncA_MinusSig);
        }
    }
    /* deal with pins going high */
    if(debounceCountHighA)
    {
        debounceCountHighA=debounceCountHighA-1;
        if(debounceCountHighA==0)
        {// timer has timed out pin a has been high long enough
            updateSigQueue(EncA_PlusSig);
        }
    }
    if(debounceCountHighC)
    {
        debounceCountHighC=debounceCountHighC-1;
        if(debounceCountHighC==0)
        {// timer has timed out pin c has been high long enough
            buttonClicked = true;
        }
    }
}

/*
 * Function:  pinAChanged
 * --------------------
 * Function that is linked to the pin interupt for the encoder A GPIO, activated when a change occurs, sets the debounce to start.
 */

void pinAChanged(void)
{
    if (digitalRead(encoderPinA) == HIGH)
    {
        debounceCountHighA = ENCODER_CONTACT_DEBOUNCE;
    }
    else
    {
        debounceCountLowA = ENCODER_CONTACT_DEBOUNCE;
    }
}

/*
 * Function:  buttonChanged
 * --------------------
 * Function that is linked to the pin interupt for the button GPIO, activated when a change occurs, sets the debounce to start.
 */

void buttonPressed(void)
{
    if (digitalRead(encoderButtonPin) == HIGH)
    {
        debounceCountHighC = ENCODER_BUTTON_DEBOUNCE;
    }
}

