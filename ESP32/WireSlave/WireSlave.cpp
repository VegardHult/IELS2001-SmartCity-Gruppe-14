#include "WireSlave.h"
#include "utility.h"

extern String nextAction;
extern String mode;
extern bool mqttFlag;

extern String wireMessageSend;
extern String wireActionRecieve;
extern String wireBatteryRecieve;

extern int bilID;

// Callback for request from master
void requestEvent() {
  String response = nextAction + mode;
  // Convert String to c-string and send
  Wire.write(response.c_str()); 
  mqttFlag = false;
}

// Callback for data recieved from master
void receiveEvent(int howMany) {
    String recievedData = "";
  while (Wire.available()) {
    char c = Wire.read();
    recievedData += c;
  }

  // Send completed action to server
  if (recievedData.charAt(0) == 'A') {
    nextAction = recievedData.charAt(1);
    mqtt.publish("car" + String(bilID) + "/action", wireActionRecieve.substring(1));
  }
  // Send battery data to server
  else if (recievedData.charAt(0) == 'B') {
    nextAction = recievedData.substring(1);
    mqtt.publish("car" + String(bilID) + "/battery", wireBatteryRecieve.substring(1));
  }
}