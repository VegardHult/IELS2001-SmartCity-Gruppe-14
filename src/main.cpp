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

int id;

bool busy = false;
bool busy_last = false;
unsigned long elapsedTime = 0; // Millisekunder siden programmets start

// Batteri
volatile double batteryCharge = FULL_BATTERY; // Batteri
int batteryPercentage; //Batteri målt i sekunder
int battery_last;

void setup()
{
    delay(1000);
    initSensors();
    calibrateZumo();
    gyroskopInit();
    delay(2000);  

    int id = Get_car_ID();

    String subscribeTopic = "car" + String(id) + "/nextaction";
    String updateTopic = "car" + String(id) + "/action";

    writeToScreen(id, 0);
}

void loop()
{
  elapsedTime = millis();
  //fixes everyting battery-related and displays it on the screen
  updateBattery();

  // Update server by sending {battery} to car{id}/battery

  // Display battery percentage
   if (battery_last != batteryPercentage) {
      writeToScreen(String(batteryPercentage) + "%", 1);
      battery_last = batteryPercentage;

  // Run action
  busy = navigateGrid(action, mode);

  // When action is finished
  if (!busy)
  {
    busy = true;
    action = nextAction;
    nextAction = read_message(subscribeTopic); // Write function for recieving MQTT messages

    // Update server by sending {action} to car{id}/action
    }
    
    
  }

  
}