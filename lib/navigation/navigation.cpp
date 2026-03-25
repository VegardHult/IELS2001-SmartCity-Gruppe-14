#include "navigation.h"
#include "utility.h"
#include "locate.h"
#include "lineFollowing.h"

// Number of encoder clicks from intersection found to intersection
int clicksToIntersection = 600;

// Speed settings
int defaultSpeed = 200;
int defaultTurnSpeed = 150;
int emergencySpeed = 300;
int emergencyTurnSpeed = emergencySpeed / defaultSpeed * defaultTurnSpeed;

// Global variables used in navigation logic
int startDirection = 0;
bool actionFinished = false;
bool followingLine = false;
extern bool recieve;
extern bool busy;

intPair countClicks{};

// Degrees for turns
static int degreesTurnAround = 174;
static int degreesLeftTurn = 84;
static int degreesRightTurn = -84;



// Navigation master function
bool navigateGrid (actions action, modes mode) {

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

        // Charging
        case C:
            speed = 0;
            turnSpeed = 0;
            break;
    }    

    // Action at intersection
    switch (action)
    {
    // Drive straight to intersection
    case S:
        if (followingLine) {
            followingLine = false;//followLine(speed);
        } else {
            busy = driveClicks(clicksToIntersection, clicksToIntersection, speed);
            followingLine = true;
        }
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
        motors.setSpeeds(0, 0);
        // Sleep?
        break;
    }
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
        driveBattery(abs(readClicks.int1) + abs(readClicks.int2));

        // Start motors
        motors.setSpeeds(speed * dirL, speed * dirR);
        busy = true;
    } else {
        // Read and reset clicks from encoders
        readClicks = readEncoders();
        // Update battery value from clicks
        driveBattery(abs(readClicks.int1) + abs(readClicks.int2));
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
        resetGyro();

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
        int currentDirection = getDirection();
        // Set busy to false when reached rotation goal
        if (abs(currentDirection) >= abs(degrees)) {
            busy = false;
        }
    }
    // Return busy
    return busy;
}

actions test_action = R;

void testAction() {
    if (test_action == R) {
        busy = makeTurn(-90, 150);
        if (!busy) {
            test_action = S;
        }
    } else if (test_action == S) {
        busy = driveClicks(500, 500, 150);
        if (!busy) {
            test_action = R;
        }
    }
    
    if (!busy) {
    motors.setSpeeds(0,0);
    delay(1000);
  }
}


void testNavigation() {
    actions actionList[] = {L, R, L, T, S};
    modes modeList[] = {D, D, E, D, E};

    int a = 0;

    while (true) {
        busy = navigateGrid(actionList[a], modeList[a]);

        if (!busy) {
            a += 1;
            motors.setSpeeds(0,0);
            delay(1000);
        }

        if (a >= int(sizeof(actionList)/sizeof(actionList[0]))) {break;}
    }
}
