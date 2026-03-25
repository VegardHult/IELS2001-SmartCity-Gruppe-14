#ifndef NAVIGATION_H
#define NAVIGATION_H
// Your declarations here
#include <Arduino.h>
#include <Zumo32U4.h>

#include "battery.h"
#include "utility.h"

extern Zumo32U4Motors motors;

// Navigation master function
bool navigateGrid(actions nextAction, modes mode);

// Drive certain amount of clicks per motor
bool driveClicks(int cL, int cR, int speed);

// Turn certain amount of degrees on the spot
bool makeTurn(int degrees, int speed);

void testNavigation();

#endif // NAVIGATION_H
