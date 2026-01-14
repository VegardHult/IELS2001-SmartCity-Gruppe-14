#include "navigation.h"
#include "utility.h"
#include "locate.h"

// Number of encoder clicks from intersection found to intersection
int clicksToIntersection = 100;

// Speed settings
int defaultSpeed = 100;
int defaultTurnSpeed = 50;
int emergencySpeed = 200;
int emergencyTurnSpeed = emergencySpeed / defaultSpeed * defaultTurnSpeed;


void driveClicks(int cL, int cR, int speed) {
    intPair readClicks{};
    intPair countClicks{};

    readClicks = readEncoders();
    driveBattery(readClicks);

    int dirL = cL / abs(cL);
    int dirR = cR / abs(cR);

    motors.setSpeeds(speed * dirL, speed * dirR);

    while (countClicks.int1 < cL && countClicks.int2 < cR) {
        readClicks = readEncoders();
        countClicks.int1 += readClicks.int1;
        countClicks.int2 += readClicks.int2;
    }

    driveBattery(countClicks);
}

void makeTurn(int degrees, int speed) {
    int startAcc = readAccelerometer();

    // Constrain degrees to -180 -> 180
    if (degrees > 180) {
        degrees -= 360;
    } else if (degrees < -180) {
        degrees += 360; 
    }

    // Set direction
    int dirL;
    int dirR;
    if (degrees > 0) {
        dirL = 1;
        dirR = -1;
    } else if (degrees < 0) {
        dirL = -1;
        dirR = 1;
    }

    // Start motors
    motors.setSpeeds(speed * dirL, speed * dirR);

    // Check if turn is finished
    while(true) {
        int currentAcc = readAccelerometer() - startAcc;
        if (abs(currentAcc) > abs(degrees)) {
            break;
        }
    
    }
}

void navigateGrid (nextAction a) {

    int speed;
    int turnSpeed;

    // Set speed based on mode
    switch (mode)
    {
    case PATROL:
        /* code */
        speed = defaultSpeed;
        turnSpeed = defaultTurnSpeed;
        break;
    
    EMERGENCY:
        /* code */
        speed = emergencySpeed;
        turnSpeed = emergencyTurnSpeed;
        break;
    }    

    // Drive to intersection
    driveClicks(clicksToIntersection, clicksToIntersection, speed);

    // Action at intersection
    switch (a)
    {
    // Drive straight
    case S:
        /* code */
        // Drive past intersection
        driveClicks(clicksToIntersection, clicksToIntersection, speed);
        break;

    // Turn 180 degrees
    case T:
        /* code */
        break;

    // Turn left
    case L:
        /* code */
        // 
        break;
        
    // Turn right
    case R:
        /* code */
        break;    
    }

    // updateLocation();
}


