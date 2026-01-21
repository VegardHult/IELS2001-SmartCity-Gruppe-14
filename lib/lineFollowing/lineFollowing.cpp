#include "lineFollowing.h"
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;

#define NUM_SENSORS 5

unsigned int lineSensorValues[NUM_SENSORS];
int error;
int lastError = 0;


void calibrateSensors() {
    // Calibrate the line sensors by taking multiple readings
    for (uint16_t i = 0; i < 120; i++) {
        if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void followLine(){
    int position = lineSensors.readLine(lineSensorValues);
    error = position - 2000; 
    const float Kp = 0.1f;
    int correction = (int)(Kp * error);

    int leftSpeed = 200 + correction;
    int rightSpeed = 200 - correction;

    if(leftSpeed > 400) leftSpeed = 400;
    if (leftSpeed < -400) leftSpeed = -400;
    if (rightSpeed > 400) rightSpeed = 400;
    if (rightSpeed < -400) rightSpeed = -400;
    motors.setSpeeds(leftSpeed, rightSpeed);
    lastError = error;

}

bool paKryss(){
    const unsigned int SENSOR_THRESHOLD = 700;
    int activeSensors = 0;
    for (int i = 0; i < NUM_SENSORS; i++) {
        if (lineSensorValues[i] > SENSOR_THRESHOLD) {
            activeSensors++;
        }
    }
    return activeSensors >= 3;

}
