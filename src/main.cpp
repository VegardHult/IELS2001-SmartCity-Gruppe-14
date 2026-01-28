#include <Zumo32U4.h>
#include <Arduino.h>
#include <Zumo32U4.h>
#include "battery.h"
#include "display.h"
#include "MQTT.h"
#include "navigation.h"
#include "startSequence.h"
#include "utility.h"
#include "lineFollowing.h"

Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;

int _lastValue = 0;

void setup()
{
    delay(1000);
    initSensors();
    calibrateZumo();
}

void loop()
{
    followLine(200, 0.4, 1.5);
}