#include "MQTT.h"
#include "EspMQTTClient.h"
#include "PubSubClient.h"
#include "utility.h"

#ifndef MQTT_IP
#define MQTT_IP "192.168.137.74"
#endif

#ifndef SSID
#define SSID "TORBJØRNSPC 4789"
#endif

#ifndef WiFi_pass
#define WiFi_pass "(75R413b"
#endif

extern String nextAction;
extern String mode;
extern bool mqttFlag;

extern String wireMessageSend;
extern String wireActionRecieve;
extern String wireBatteryRecieve;

extern int bilID;
String hostname = "car" + random(0, 1024);

EspMQTTClient mqtt(
    SSID,
    WiFi_pass,
    MQTT_IP,
    hostname
);

// Alt som skal skje når bilen har koblet til serveren. Funksjonsnavnet kan IKKE endres.
void onConnectionEstablished(){
    Serial.println("Koblet til WiFi");

    mqtt.subscribe("program/assignID", [](const String &payload){
        bilID = payload;
        mqtt.unsubscribe("program/assignID");
    });
    
    Serial.println("Venter på ID");
    // Vent til vi får en ID 
    while(bilID == NULL){
        mqtt.loop();
    }

    Serial.print("Fått ID: "); Serial.println(bilID);

    String handlingTopic = "car" + bilID + "/nextAction";
    mqtt.subscribe(handlingTopic, [](const String &payload){nextAction = payload.charAt(0); mqttFlag = true;});
    String handlingTopic = "car" + bilID + "/mode";
    mqtt.subscribe(handlingTopic, [](const String &payload){mode = payload.charAt(0); mqttFlag = true;})
}
