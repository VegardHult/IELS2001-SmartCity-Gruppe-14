#include "lineFollowing.h"
#include <Zumo32U4.h>
#include <Arduino.h>


#define NUM_SENSORS 5

unsigned int lineSensorValues[NUM_SENSORS];
int error;
int lastError = 0;

void calibrateSensors()
{
    // Calibrate the line sensors by taking multiple readings
    for (uint16_t i = 0; i < 120; i++)
    {
        if (i > 30 && i <= 90)
        {
            motors.setSpeeds(-200, 200);
        }
        else
        {
            motors.setSpeeds(200, -200);
        }

        lineSensors.calibrate();
        delay(10);
    }
    motors.setSpeeds(0, 0);
}

void followLine()
{
    int position = lineSensors.readLine(lineSensorValues);
    error = position - 2000;

    int correction = 0.2*error + 1.5*(error-lastError);


    int leftSpeed = 200 + correction;
    int rightSpeed = 200 - correction;

    leftSpeed = constrain(leftSpeed,0,200);
    rightSpeed = constrain(rightSpeed,0,200);

    motors.setSpeeds(leftSpeed, rightSpeed);
    lastError = error;
}

bool paKryss()
{
    const unsigned int SENSOR_THRESHOLD = 700;
    int activeSensors = 0;
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (lineSensorValues[i] > SENSOR_THRESHOLD)
        {
            activeSensors++;
        }
    }
    return activeSensors >= 3;
}
