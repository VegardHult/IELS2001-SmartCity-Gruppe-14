#include <Arduino.h>
#include <Zumo32U4.h>
#include "startSequence.h"

/*extren*/ Zumo32U4LineSensors lineSensors;
/*extren*/ Zumo32U4Motors motors;

unsigned int lineSensorValues[5];
unsigned long elapsedTime = 0;
unsigned long timeWhenCrossedLine = 0;

// skru på sensorer
void initSensors()
{
    lineSensors.initFiveSensors();
}

void trackLinePosition()
{
    lineSensors.readLine(lineSensorValues);
    /*for(int i=0;i<5;i++){
         Serial.print(lineSensorValues[i]);
    }
    Serial.println("-------------");*/

    if (lineSensorValues[2] = 1000)
    {
        timeWhenCrossedLine = elapsedTime;
    }
}

void stopOnLine()

{
    int startTimeFindLine = elapsedTime;
    while ((elapsedTime - startTimeFindLine) < timeWhenCrossedLine)
    {
        motors.setSpeeds(-200, 200);
    }
    motors.setSpeeds(0,0);
}

//noe feil med logikken i tiden på denne
void spinZumo()
{
    elapsedTime = millis();
    for (int i = 0; i < 40; i++)
    {

        motors.setSpeeds(200, -200);

        lineSensors.calibrate();
        trackLinePosition();
        delay(10);
    }
    
    stopOnLine();
}