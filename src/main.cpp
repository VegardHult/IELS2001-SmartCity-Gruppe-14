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

void setup() {
    calibrateZumo();
    Serial.begin(9600);
}

void loop() {
    followLine();
    if (paKryss()) {
        Serial.println("Kryss detected!");
        delay(1000); // Pause for a second to avoid multiple detections
    }
    
}