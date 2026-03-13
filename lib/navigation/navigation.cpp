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

// Degrees for turns
static int degreesTurnAround = 174;
static int degreesLeftTurn = -84;
static int degreesRightTurn = 84;

extern bool recieve;

// Navigation master function
bool navigateGrid (actions nextAction, modes mode) {

    int speed;
    int turnSpeed;

    // Set speed based on mode
    switch (mode)
    {
        // Default
        case D:
            speed = defaultSpeed;
            turnSpeed = defaultTurnSpeed;
            break;
        
        // Emergency
        case E:
            speed = emergencySpeed;
            turnSpeed = emergencyTurnSpeed;
            break;
    }    

    // Action at intersection
    switch (action)
    {
    // Drive straight to intersection
    case S:
        busy = followLine();
        // Start driving to intersection centre when intersetion found
        if (!busy) {
            action = D;
            // Start driving to intersection center, to keep busy true
            busy = driveClicks(clicksToIntersection, clicksToIntersection, speed);
        }
        break;

    // Drive to intersection centre
    case D:
        busy = driveClicks(clicksToIntersection, clicksToIntersection, speed);
        break;

    // Turn 180 degrees
    case T:
        busy = makeTurn(degreesTurnAround, turnSpeed);
        break;

    // Turn left
    case L:
        busy = makeTurn(degreesLeftTurn, turnSpeed);
        break;
        
    // Turn right
    case R:
        busy = makeTurn(degreesRightTurn, turnSpeed);
        break;
    
    // Idle
    case I:
        busy = false;
        // Sleep?
        break;
    }
    return busy;
}

// Follow line between intersections
bool followLine() {
    busy = false;
    return busy;
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
            // Set not busy when done driving
            busy = false;
        }
    }
    // Return false if not done driving
    return busy;
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
        if (degrees > 0) {dirL = -1; dirR = 1;}
        else if (degrees < 0) {dirL = 1; dirR = -1;} 
        else {dirL = 1; dirR = 1;}

        // Start motors
        motors.setSpeeds(speed * dirL, speed * dirR);

        busy = true;
    } 
    // Run while turning, return false when done turning
    else {
        int currentDirection = (getDirection() - startDirection);
        if (currentDirection > 180) {currentDirection -= 360;} else if (currentDirection < -179) {currentDirection += 360;}
        Serial.println(currentDirection);
        // Set busy to false when reached rotation goal
        if (degrees >= 0) {
            // For positive angle changes
            if (currentDirection >= degrees) {
                busy = false;
            }
        // For negative angle changes
        } else {
            if (currentDirection <= degrees) {
                busy = false;
            }
        }
        
    }
    // Return busy
    return busy;
}
