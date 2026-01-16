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

// Global variables used in naigation logic
int startDirection = 0;
bool actionFinished = false;
bool busy = false;
intPair countClicks{};

// 
static int degreesTurnAround = 180;
static int degreesLeftTurn = -90;
static int degreesRightTurn = 90;

// Navigation master function
void navigateGrid (actions nextAction) {

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
    
    case EMERGENCY:
        /* code */
        speed = emergencySpeed;
        turnSpeed = emergencyTurnSpeed;
        break;
    }    

    

    // Drive to intersection
    driveClicks(clicksToIntersection, clicksToIntersection, speed);

    // Action at intersection
    switch (action)
    {
    // Drive straight to intersection
    case D:
        /*code*/
        if (!followLine()) {actionFinished = false;}
        else {actionFinished = true; action = nextAction;}
        break;
    // Drive straight on intersection
    case S:
        /* code */
        // Drive past intersection
        if (!driveClicks(clicksToIntersection, clicksToIntersection, speed)) {actionFinished = true;}
        else {actionFinished = false; action = nextAction;}
        break;

    // Turn 180 degrees
    case T:
        /* code */
        if (!makeTurn(degreesTurnAround, speed)) {actionFinished = true;}
        else {actionFinished = false; action = nextAction;}
        break;

    // Turn left
    case L:
        /* code */
        if (!makeTurn(degreesLeftTurn, speed)) {actionFinished = true;}
        else {actionFinished = false; action = nextAction;}
        break;
        
    // Turn right
    case R:
        /* code */
        if (!makeTurn(degreesRightTurn, speed)) {actionFinished = true;} 
        else {actionFinished = false; action = nextAction;}
        break;
    
    // Idle
    case I:
        
        break;
    }

    // updateLocation();
}

// Follow line between intersections
bool followLine() {
    
}

// Drive certain amount of clicks per motor
bool driveClicks(int cL, int cR, int speed) {
    intPair readClicks{};

    if (!busy) {
        // Reset click count
        countClicks.int1 = 0;
        countClicks.int2 = 0;

        // Set motor direction
        int dirL = cL / abs(cL);
        int dirR = cR / abs(cR);

        // Read and reset encoder clicks from encoders
        readClicks = readEncoders();
        // Update battery value from clicks
        driveBattery(readClicks);

        // Start motors
        motors.setSpeeds(speed * dirL, speed * dirR);
        // Update busy value
        busy = true;
    } else {
        // Read and reset clicks from encoders
        readClicks = readEncoders();
        // Update battery value from clicks
        driveBattery(readClicks);
        // Count clicks
        countClicks.int1 += readClicks.int1;
        countClicks.int2 += readClicks.int2;
        
        // Check if click count is over target click count
        if (countClicks.int1 >= cL && countClicks.int2 >= cR) {
            // Return true if done driving
            return true;
        }
    }
    // Return false if not done driving
    return false;
}

// Turn certain amount of degrees on the spot
bool makeTurn(int degrees, int speed) {

    // Only run on start of turn
    if(!busy) {
        // Get facing direction at start of turn
        startDirection = getDirection();

        // Set motor direction
        int dirL;
        int dirR;
        if (degrees > 0) {dirL = 1; dirR = -1;}
        else if (degrees < 0) {dirL = -1; dirR = 1;} 
        else {dirL = 1; dirR = 1;}

        // Start motors
        motors.setSpeeds(speed * dirL, speed * dirR);
        busy = true;
    } 
    // Run while turning, return true when done turning
    else {
        int currentDirection = getDirection() - startDirection;
        if (abs(currentDirection) >= abs(degrees)) {
            return true;
        }
    }
    // Return false if not done turning
    return false;
}
