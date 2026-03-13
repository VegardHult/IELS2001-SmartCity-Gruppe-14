#ifndef WIREMASTER_H
#define WIREMASTER_H
// Your declarations here
#include <Arduino.h>
#include <Zumo32U4.h>
#include <Wire.h>

String sendWire(int addr, String data);

String requestWire (int addr);

#endif // WIREMASTER_H