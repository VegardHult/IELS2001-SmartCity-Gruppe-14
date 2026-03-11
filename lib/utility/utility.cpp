#include "utility.h"

// Read encoder values
intPair readEncoders() {
    
    intPair readClicks{};

    readClicks.int1 = encoders.getCountsAndResetLeft();
    readClicks.int2 = encoders.getCountsAndResetRight();

    return readClicks;
}

String handleWire(int addr, String data) {
    // Send data to ESP-32
    Wire.beginTransmission(addr);
    Wire.write(data);
    Wire.endTransmission();
    
    // Request next action from ESP-32
    Wire.requestFrom(addr, 2); // Request 2 bytes (one character long string)
    String receivedData = ""; // Clear existing data
    while (Wire.available()) {
        char c = Wire.read();
        if (c == '\0') break; // Stop reading at the terminator
        receivedData += c;
    }

    return recievedData;
}