#ifndef NAVIGATION_H
#define NAVIGATION_H
// Your declarations here
#include <Arduino.h>
#include <Zumo32U4.h>

#include "battery.h"

extern Zumo32U4Motors motors;

void driveClicks(int cL, int cR, int speed);

void makeTurn(int degrees, int speed);

void navigateGrid(nextAction a);

#endif // NAVIGATION_H
