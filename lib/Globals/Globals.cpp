#include "Settings.h"
#include "Globals.h"
#include "Screen.h"
#include "SystemSelfTest.h"
// #include "Stack.h"
// #include <TFT_eSPI.h>

Settings* settings = new Settings();
Screen* screen = new Screen();
SelfTest* ErrorStatus = new SelfTest();