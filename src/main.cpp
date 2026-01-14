#include "battery.h"
#include "display.h"
#include "locate.h"
#include "MQTT.h"
#include "navigation.h"
#include "startSequence.h"
#include "utility.h"

#include <Arduino.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders;
Zumo32U4Motors motors;

modes mode = PATROL;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}