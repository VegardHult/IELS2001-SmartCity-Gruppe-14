#include <Arduino.h>

#include "MQTT.h"
#include "WireSlave.h"
#include "utility.h"

int bilID;

char nextAction = 'I';
char mode = 'D';
bool mqttFlag = false;

// Format [action, mode]
String wireMessageSend = "ID";
String wireActionReceive = "AI";
String wireBatteryReceive = "B100";

void setup()
{
  
}

void loop()
{
    mqtt.loop();
}