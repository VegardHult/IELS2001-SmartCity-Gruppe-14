#include <Arduino.h>
#include <Zumo32U4.h>
#include "startSequence.h"


unsigned int lineValues[5];

// skru på sensorer
void initSensors()
{
    lineSensors.initFiveSensors();
}

//spinner bilen helt til den finne linja igjen
void stopOnLine()
{
    while (lineValues[2] != 1000)
    {
        lineSensors.readLine(lineValues);
        motors.setSpeeds(-200, 200);
        delay(10);
    }

    motors.setSpeeds(0,0);
}

//spinner bilen og kalibrerer sensorene
void calibrateZumo()
{
    for (int i = 0; i < 40; i++)
    {
        lineSensors.calibrate();
        motors.setSpeeds(200, -200);
        delay(10);
    }
    motors.setSpeeds(0,0);
    //kalibrering ferdig -> spin tilbake til linja
    stopOnLine();
}