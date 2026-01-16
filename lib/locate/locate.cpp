#include "locate.h"

Zumo32U4IMU imu;

#include "TurnSensor.h"


void gyroskopInit(){
    turnSensorSetup();
}


int getVinkel(){
    return (((int32_t)turnAngle >> 16) * 360) >> 16;
}