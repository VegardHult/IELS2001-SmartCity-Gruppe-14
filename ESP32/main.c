#include <Arduino.h>
#include <Wire.h>

#include "MQTT.h"

String nextAction = "I"

void setup()
{
  
}

// Callback for request from master
void requestCallback() {
  String response = nextAction;
  // Convert String to c-string and send
  Wire.write(response.c_str()); 
  recieveActionFlag = false;
}

// Callback for data recieved from master
void receiveCallback(int howMany) {
    String recievedData = "";
  while (Wire.available()) {
    char c = Wire.read();
    recievedData += c;
  }

  // Send completed action to server
  if (recievedData.charAt(0) == "A") {
    action = recievedData.charAt(1);
    mqtt.publish("car" + bilID + "/action", action);
  }
  // Send battery data to server
  else if (recievedData.charAt(0) == "B") {
    action = recievedData.substring(1);
    mqtt.publish("car" + bilID + "/battery", action);
  }
}

void loop()
{
    mqtt.loop();
}