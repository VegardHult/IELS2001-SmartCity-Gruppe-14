#ifndef __BATTERY__
#define __BATTERY__

#include <Arduino.h>
#include <Zumo32U4.h>

extern Zumo32U4Encoders encoders;

// Battery variables
// Maximal battery level
#ifndef FULL_BATTERY
#define FULL_BATTERY 100
#endif

// Battery cost of driving, percentage per wheel rotation
#ifndef DRIVE_COST_ROTATION
#define DRIVE_COST_ROTATION 1
#endif

// Battery cost of running, percentage per minute
#ifndef IDLE_COST_MINUTE
#define IDLE_COST_MINUTE 60
#endif

// Idle cost time interval, 1 second
#ifndef IDLE_TIME
#define IDLE_TIME 1000
#endif

// Elapsed time since start of program, global variable
extern unsigned long elapsedTime;
// Current battery charge, global variable
extern volatile double batteryCharge;
// Current battery charge converted to in 0-100, global variable
extern int batteryPercentage;

// Subtract battery charge cost of driving
void driveBattery();

// Subtract battery charge cost of idle
void idleBattery();

// Calculate percentage from batteryCharge, and write to screen
void displayBatteryPercentage();

// Runs all functions above to update battery state and display
void updateBattery();

#endif // __BATTERY__