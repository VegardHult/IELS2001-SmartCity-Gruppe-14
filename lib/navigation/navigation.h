#ifndef NAVIGATION_H
#define NAVIGATION_H
// Your declarations here
#include <Arduino.h>
#include <Zumo32U4.h>

#include "battery.h"

extern Zumo32U4Motors motors;

// Navigation master function
void navigateGrid(actions nextAction);

// Follow line between intersections
bool followLine();

// Drive certain amount of clicks per motor
bool driveClicks(int cL, int cR, int speed);

// Turn certain amount of degrees on the spot
bool makeTurn(int degrees, int speed);

#endif // NAVIGATION_H
