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

bool busy = false;

void setup()
{
    delay(1000);
    initSensors();
    calibrateZumo();
    gyroskopInit();
    delay(2000);

    // UFERDIG

    // Assign ID from MQTT "promgram/assignId"
    while (true)
    {
      if (recieved_message)
      {
        int id = message;
        // subscribe to car{id}/nextaction
        // topic = "car{id}/action"
        break;
      }
    }
    
}

void loop()
{
  // Get MQTT message if ready
  if (!busy)
  {
    busy = true;
    actions nextMove = read_message;
  }
  
  // Run action
  if (navigateGrid(nextMove)) {
    // Send update to server
  }

}