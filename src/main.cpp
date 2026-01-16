#include <Arduino.h>
#include <Zumo32U4.h>

#include "battery.h"
#include "display.h"
#include "locate.h"
#include "MQTT.h"
#include "navigation.h"
#include "startSequence.h"
#include "utility.h"

// Setup ZUMO modules
Zumo32U4Encoders encoders;
Zumo32U4Motors motors;

modes mode = PATROL;
actions action = I;

bool wait = false;

void setup() {
  // put your setup code here, to run once:
  gyroskopInit();
  delay(2000);
}

void loop() {

  oppdaterGyro();
  // put your main code here, to run repeatedly:
  int degreesLeftTurn = 84;
  int speed = 200;

  if (makeTurn(degreesLeftTurn, speed)) {
    motors.setSpeeds(0, 0);
    delay(1000);
  }

}