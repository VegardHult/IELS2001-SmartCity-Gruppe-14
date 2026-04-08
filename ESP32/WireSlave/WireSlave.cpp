#include "WireSlave.h"
#include "utility.h"

extern String nextAction;
extern String mode;
extern bool mqttFlag;

extern String wireMessageSend;
extern String wireActionReceive;
extern String wireBatteryReceive;

extern int bilID;

// Callback for request from master
void requestEvent() {
  String response = nextAction + mode;
  // Convert String to c-string and send
  Wire.write(response.c_str()); 
  mqttFlag = false;
}

// Callback for data received from master
void receiveEvent(int howMany) {
    String receivedData = "";
  while (Wire.available()) {
    char c = Wire.read();
    receivedData += c;
  }

  // Send completed action to server
  if (receivedData.charAt(0) == 'A') {
    nextAction = receivedData.charAt(1);
    mqtt.publish("car" + String(bilID) + "/action", wireActionReceive.substring(1));
  }
  // Send battery data to server
  else if (receivedData.charAt(0) == 'B') {
    nextAction = receivedData.substring(1);
    mqtt.publish("car" + String(bilID) + "/battery", wireBatteryReceive.substring(1));
  }
}