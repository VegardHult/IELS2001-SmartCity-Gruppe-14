#include <Arduino.h>
#include <Zumo32U4.h>
#include "battery.h"
#include "display.h"
#include "locate.h"
#include "MQTT.h"
#include "navigation.h"
#include "startSequence.h"
#include "utility.h"

Zumo32U4Buzzer buzzer;

void setup() {
  Serial.begin(9600);
  initSensors();


  spinZumo();
 

  buzzer.playNote(NOTE_A(4), 500, 10);
  
}

void loop() {

 
}