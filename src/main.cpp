#include <Arduino.h>
#include <Zumo32U4.h>

#include "battery.h"
#include "display.h"
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

modes mode = D;
actions action = I;
actions nextAction = action;

// int id;
int addr = 100;

bool busy = false;
bool recieve = true;
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

    // int id = Get_car_ID();

    // Write id to screen, not currently functional
    // writeToScreen(String(id), 0);
}

void loop()
{
  elapsedTime = millis();
  //fixes everyting battery-related and displays it on the screen
  updateBattery();

  // Write id to screen, not currently functional
  // writeToScreen("ID: " + String(id) + " " + String(mode), 0);

  // Run action
  busy = navigateGrid(action, mode);

  // When action is finished
  if (!busy)
  {
    // Send finished action to ESP-32
    String actionString = actionToString(action);
    sendWire(addr, actionString);
    // Assign next action to action
    action = nextAction;
    // Get next action from ESP-32
    String wireString = requestWire(addr); // Write function for recieving MQTT messages
    wireDataStruct wireData = parseWire(wireString);
    nextAction = wireData.action;
    mode = wireData.mode;
    
  }
}