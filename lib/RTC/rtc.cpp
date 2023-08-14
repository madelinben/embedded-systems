#include "rtc.h"
#include "RTClib.h"
#include "Wire.h"


RTC_DS1307 rtc;

/*
 * Function:  getRTCCurrentDateTime
 * --------------------
 * Gets the current date and time in a date time structure.   
 */
DateTime getRTCCurrentDateTime(void)
{
    DateTime currentTime = rtc.now();
    return(currentTime);
}

/*
 * Function:  syncCurrentDateTime
 * --------------------
 * overrides the current date and time recorded on the RTC. 
 */
void syncCurrentDateTime(DateTime currentDateTime)
{
    rtc.adjust(currentDateTime);
}

/*
 * Function:  startRTC
 * --------------------
 * Starts the RTC module, returning a true if it succeeded or false if it failed.
 */
bool startRTC(void)
{
    bool successful = rtc.begin();
    return (successful);
}