#define FULL_BATTERY 100

#include <Arduino.h>
#include <Zumo32U4.h>

//Egne biblioteker
#include "battery.h"
#include "lineFollowing.h"
#include "display.h"
#include "navigation.h"
#include "startSequence.h"
#include "utility.h"
#include "locate.h"
#include "WireMaster.h"

// Setup ZUMO modules
Zumo32U4Buzzer buzzer;
Zumo32U4OLED display;
Zumo32U4Encoders encoders;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4IMU imu;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonC buttonC;

//Egne biblioteker
#include "battery.h"
#include "lineFollowing.h"
#include "display.h"

modes mode = P;
actions action = I;
actions nextAction = action;

// int id;
int addr = 0x45;

bool busy = false;
bool recieve = true;
unsigned long elapsedTime = 0; // Millisekunder siden programmets start

// Batteri
volatile double batteryCharge = FULL_BATTERY; // Batteri
int batteryPercentage; //Batteri målt i sekunder
int battery_last;

int i;
void setup()
{
  Serial.begin(9600);
  delay(4000);
  i = 0;
  Serial.println("Starter setup");
  initSensors();
  //calibrateZumo();
  //gyroskopInit();
  delay(2000);
    
  Wire.begin();
  Serial.println("Setup ferdig");
  //Forhindrer at I2C bussen låses når modemet enda starter
  while(!buttonC.getSingleDebouncedPress()){}
  // int id = Get_car_ID();

  // Write id to screen, not currently functional
  // writeToScreen(String(id), 0);
}

int angle;

void loop()
{
  Serial.println(i);
  delay(100);
  i+=1;

  //elapsedTime = millis();

  //testNavigation();

  /*
    TODO: Må finne en måte for bilen å ikke fryse når den forespør data mens ESPen settes opp. Knapp C er tilgjengelig
  

  String data = "";
  Serial.println("Forspør data");
  Wire.requestFrom(0x45, 2);
  Serial.println("Data forespurt");
  while(Wire.available()){
    Serial.println("Data tilgjengelig");
    data += Wire.read();
  }
  Serial.println(data);
  

  //fixes everyting battery-related and displays it on the screen
  //updateBattery();

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
  */
}