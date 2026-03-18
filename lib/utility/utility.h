#ifndef UTILITY_H
#define UTILITY_H
// Your declarations here
#include <Arduino.h>
#include <Zumo32U4.h>
#include <Wire.h>

enum actions {
    S, // Straight
    T, // Turn 180 degrees
    L, // Turn left
    R,  // Turn right
    I // Idle
};

enum modes {
    D, // Patrol driving speed
    E, // Emergency, siren and higher speed
    C // Charge battery when on charging station
};

struct wireDataStruct {
    actions action;
    modes mode;
};

String handleWire(int addr, String data);

extern Zumo32U4Encoders encoders;
extern actions action;

extern modes mode;

struct intPair {
    int int1;
    int int2;
};

// Read encoder values
intPair readEncoders();

// Send wire message to ESP-32
String sendWire(int addr, String data);

// Request wire from ESP-32
String requestWire (int addr);

// Get string from action
String actionToString(actions action);

// Get string from mode
String modeToString(modes mode);

// Get action from string
actions stringToAction(String str);

// Get mode from string
modes stringToMode(String str);

// Parse data from wire
wireDataStruct parseWire(String recievedData);

#endif // UTILITY_H

