#include "lineFollowing.h"

#define MIDDLE_OF_LINE 2000
#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS]; 

int error;
int lastError = 0;

//endre disse for å tune styringen
#define PROPORTIONAL_CONSTANT 0.2
#define DERIVATIVE_CONSTANT 1.5

extern int _lastValue;

int locateLine (unsigned int sensor_values[]) {
    int _numSensors = 3;

    unsigned char i, on_line = 0;
    unsigned long avg; // this is for the weighted total, which is long
                       // before division
    unsigned int sum; // this is for the denominator which is <= 64000

    avg = 0;
    sum = 0;

    for(i=1;i<(_numSensors+1);i++) {
        int value = sensor_values[i];
        // keep track of whether we see the line at all
        if(value > 200) {
            on_line = 1;
        }

        // only average in values that are above a noise threshold
        if(value > 50) {
            avg += (long)(value) * (i * 1000);
            sum += value;
        }
    }

    if(!on_line)
    {
        // If it last read to the left of center, return 0.
        if(_lastValue < (_numSensors-1)*1000/2)
            return 0;

        // If it last read to the right of center, return the max.
        else
            return (_numSensors-1)*1000;

    }

    _lastValue = avg/sum;

    return _lastValue;
}


static void readSensors(){
    //tall mellom 0 og 4000
    lineSensors.readLine(lineSensorValues); 
    int position = locateLine(lineSensorValues);
    //tall mellom -2000 og 2000
    error = position - MIDDLE_OF_LINE;
}

static int directionChange(){
    //PID kontrolleringsformel
    int value = PROPORTIONAL_CONSTANT*error + DERIVATIVE_CONSTANT*(error - lastError);
    lastError = error;
    return value;
}

void followLine(int max_speed){
    readSensors();
    int speedDifference = directionChange();

    int leftSpeed = max_speed + speedDifference;
    int rightSpeed = max_speed - speedDifference;

    leftSpeed = constrain(leftSpeed,0,max_speed);
    rightSpeed = constrain(rightSpeed,0,max_speed);
    motors.setSpeeds(leftSpeed, rightSpeed);
}

/*
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
*/