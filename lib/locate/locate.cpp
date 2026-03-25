#include <Arduino.h>
#include <Zumo32U4.h>

#include "locate.h"
#include "TurnSensor.h"

void gyroskopInit(){
    turnSensorSetup();
}

void oppdaterGyro() {
    turnSensorUpdate();
}

void resetGyro() {
    turnSensorReset();
}


int getDirection(){
    turnSensorUpdate();
    int32_t turnSensorRead = (int32_t)((int64_t)turnAngle * 180 / 0x80000000);
    if (turnSensorRead > 180) turnSensorRead -= 360;
    if (turnSensorRead < -180) turnSensorRead += 360;
    return turnSensorRead;
}