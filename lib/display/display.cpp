#include <Arduino.h>
#include <Zumo32U4.h>
#include "display.h"


void clearScreen() {
    // Clear screen
    display.clear();
}

void writeToScreen(String str, int line) {
    // Write to specific line
    display.gotoXY(0, line);
    
    // Print a batteryCPercentage to screen
    display.print(str + "              ");
}