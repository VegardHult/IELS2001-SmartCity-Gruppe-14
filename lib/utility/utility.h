#ifndef UTILITY_H
#define UTILITY_H
// Your declarations here
#include <Arduino.h>
#include <Zumo32U4.h>

extern Zumo32U4Encoders encoders;
extern actions action = D;

enum actions {
    D, // Drive, line following
    S, // Straight
    T, // Turn 180 degrees
    L, // Turn left
    R,  // Turn right
    I
};

enum modes {
    PATROL,
    EMERGENCY
};

extern modes mode;

struct intPair {
    int int1;
    int int2;
};

// Read encoder values
intPair readEncoders();

#endif // UTILITY_H

