#include <Arduino.h>
#include <Zumo32U4.h>

#include "battery.h"
#include "display.h"
#include "MQTT.h"
#include "navigation.h"
#include "startSequence.h"
#include "utility.h"
#include "lineFollowing.h"

// Setup ZUMO modules
Zumo32U4Buzzer buzzer;
Zumo32U4OLED display;
Zumo32U4Encoders encoders;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;

//Egne biblioteker
#include "battery.h"
#include "lineFollowing.h"
#include "display.h"

modes mode = PATROL;
actions action = I;
actions nextAction = action;

bool busy = false;
bool busy_last = false;

void setup()
{
    delay(1000);
    initSensors();
    calibrateZumo();
    gyroskopInit();
    delay(2000);  

    // Get car ID
}

void loop()
{
  // Run action
  busy = navigateGrid(action);

  // When action is finished
  if (!busy)
  {
    busy = true;
    action = nextAction;
    nextAction = read_message(); // Write function for recieving MQTT messages

    // Update server by sending {action} to car{id}/action

  }
}