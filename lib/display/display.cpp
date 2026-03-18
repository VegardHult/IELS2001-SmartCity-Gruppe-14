#include <Arduino.h>
#include <Zumo32U4.h>
#include "display.h"
#include "utility.h"

static bool textVisible = false;
static unsigned long lastBlink = 0;
static const unsigned long blinkInterval = 400;


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
void updateDisplay(modes currentMode) {
    if (currentMode == E) {
        if (millis() - lastBlink >= blinkInterval)
        {
            lastBlink = millis();
            textVisible = !textVisible;

            display.clear();

            if (textVisible)
            {
                display.print("EMERGNCY");
            }
        }
    } else {
        display.clear();
        display.print(modeToString(currentMode));
        textVisible = true;
        lastBlink = millis();
    }
}