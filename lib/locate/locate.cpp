#include <Arduino.h>
#include <Zumo32U4.h>

Zumo32U4IMU imu;

#include "locate.h"
#include "TurnSensor.h"

void gyroskopInit(){
    turnSensorSetup();
}

void oppdaterGyro() {
    turnSensorUpdate();
}


int getDirection(){
    return ((((int32_t)turnAngle >> 16) * 360) >> 16) + 180;
}