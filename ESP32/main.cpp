#include <Wire.h>
//#include "src/MQTT/MQTT.h"
//#include "src/ESP_utility/ESP_utility.h"
//#include "src/WireSlave/WireSlave.h"
//#include <EspMQTTClient.h>
//#include <PubSubClient.h>



#ifndef MQTT_IP
#define MQTT_IP "192.168.137.74"
#endif

#ifndef SSID
#define SSID "TORBJØRNSPC 4789"
#endif

#ifndef WiFi_pass
#define WiFi_pass "(75R413b"
#endif

String bilID;

char nextAction = 'I';
char mode = 'D';


extern bool mqttFlag;

extern String wireMessageSend;
extern String wireActionRecieve;
extern String wireBatteryRecieve;

//extern EspMQTTClient mqtt;



// Callback for request from master
void requestEvent() {
  //char response[] = {nextAction, mode};
  // Convert String to c-string and send
  Wire.write((uint8_t)nextAction);
  Wire.write((uint8_t)mode); 
  mqttFlag = false;

  Serial.println("Ble forespurt data");
}

// Callback for data recieved from master
void receiveEvent(int howMany) {
    String recievedData = "";
  while (Wire.available()) {
    char c = Wire.read();
    recievedData += c;
  }

  Serial.println(recievedData);
/*
  // Send completed action to server
  if (recievedData.charAt(0) == 'A') {
    nextAction = recievedData.charAt(1);
    mqtt.publish("car" + String(bilID) + "/action", wireActionRecieve.substring(1));
  }
  // Send battery data to server
  else if (recievedData.charAt(0) == 'B') {
    nextAction = recievedData.substring(1);
    mqtt.publish("car" + String(bilID) + "/battery", wireBatteryRecieve.substring(1));
  }*/
}



bool mqttFlag = false;

// Format [action, mode]
String wireMessageSend = "ID";
String wireActionRecieve = "AI";
String wireBatteryRecieve = "B100";




//String hostname = "car" + random(0, 1024);

/*EspMQTTClient mqtt(
    "TORBJØRNSPC 4789",
    "(75R413b",
    "192.168.137.74",
    "Bil"
);*/

void setup()
{
  Serial.begin(9600);
  Wire.begin(0x45);


  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop()
{
    //Serial.println(nextAction);
}
