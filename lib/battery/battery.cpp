#include "battery.h"
#include "display.h"


/*
Disse er i headerfilen definert som extern, og forventer da at variablene settes i en annen fil. Jeg 
har bare kommentert de vekk, men de erklæres i headerfilen og initsialiseres i main.cpp
*/

// Set battery charge to full at start of program
//volatile double batteryCharge = FULL_BATTERY;
// Set battery percentage to full at start of program
//int batteryPercentage = 100;

// Last time of idleBattery() update
unsigned long lastIdleUpdate = 0;

// Update battery charge when wheel encoders move, interrupts loop()
void driveBattery() {
    int leftEncoderCount = encoders.getCountsAndResetLeft();
    int rightEncoderCount = encoders.getCountsAndResetRight();
  
    int encodersCount = abs(leftEncoderCount) + abs(rightEncoderCount);
    batteryCharge -= encodersCount * DRIVE_COST_ROTATION/900;
}

// Update battery charge based on elapsed time
void idleBattery() {
    // Subtracts IDLE_COST from batteryCharge if IDLE_TIME has passed since last update.
    if (elapsedTime > (lastIdleUpdate + IDLE_TIME)) {
        lastIdleUpdate = elapsedTime;
        batteryCharge -= IDLE_COST_MINUTE/60;
    }
}

void displayBatteryPercentage() {
    // Calculate percentage from current charge and fullBattery value
    int newPercentage = round(100*batteryCharge/FULL_BATTERY);
    // If percentage has changed, update value and screen
    if (batteryPercentage != newPercentage) {
        batteryPercentage = newPercentage;
        String batteryString = String(batteryPercentage) + "%";
        
        // Update screen
        writeToScreen(batteryString, 0);
    }
}


void updateBattery() {
    // Subtracts IDLE_COST from batteryCharge every IDLE_TIME ms
    idleBattery();
    // Subtracts DRIVE_COST from batteryCharge for every encoder pulse
    driveBattery();

    // Stops battery charge from going below zero
    if (batteryCharge < 0) {
        batteryCharge = 0;
    }

    displayBatteryPercentage();
}