#ifndef __DISPLAY__
#define __DISPLAY__

#include <Arduino.h>
#include <Zumo32U4.h>

#include "utility.h"

extern Zumo32U4OLED display;

// Clear screen
void clearScreen();

// Write string to screen
void writeToScreen(String str, int line);

void updateDisplay(modes currentMode);

#endif // __DISPLAY__