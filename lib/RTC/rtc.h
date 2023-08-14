#ifndef RTC_H
#define RTC_H
#include "RTClib.h"

DateTime getRTCCurrentDateTime(void);
void syncCurrentDateTime(DateTime currentDateTime);
bool startRTC(void);

#endif