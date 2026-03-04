#include "battery.h"
#include <Arduino.h>
#include <Zumo32U4.h>

Zumo32U4OLED display;

#define driveCost 1
#define idleCost 1
#define idleTime 1000

int batteryCharge = fullBattery;
int batteryPercentage = 100;

unsigned long lastIdleUpdate = 0;

// Update battery charge when wheel encoders move, interrupts loop()
void driveBattery() {
    batteryCharge -= driveCost;
}

// Update battery charge based on elapsed time
void idleBattery(unsigned long elapsedTime) {
    if (elapsedTime > lastIdleUpdate + idleTime) {
        lastIdleUpdate = elapsedTime;
        batteryCharge -= idleCost;
    }
}

void calculatePercentage() {
    // Calculate percentage from current charge and fullBattery value
    int newPercentage = round(100*batteryCharge/fullBattery);
    // If percentage has changed, update value and screen
    if (batteryPercentage != newPercentage) {
        batteryPercentage = newPercentage;
        // Update screen
        updateScreen();
    }
}

void updateScreen() {
    // Clear the screen
    display.clear();
    
    // Print a string
    display.print(batteryPercentage + "%");
}


void updateBattery(unsigned long elapsedTime) {
    idleBattery(elapsedTime);
    calculatePercentage();
}