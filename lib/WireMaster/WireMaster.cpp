#include "WireMaster.h"

#include <Arduino.h>
#include <Zumo32U4.h>
#include <Wire.h>

String sendWire(int addr, String data) {
    // Send data to ESP-32
    Wire.beginTransmission(addr);
    Wire.write(data.c_str());
    Wire.endTransmission();
}

String requestWire (int addr) {
    // Request next action from ESP-32
    Wire.requestFrom(addr, 10); // Request 2 bytes (one character long string)
    String recievedData = ""; // Clear existing data
    while (Wire.available()) {
        char c = Wire.read();
        if (c == '\0') break; // Stop reading at the terminator
        recievedData += c;
    }
    return recievedData;
}