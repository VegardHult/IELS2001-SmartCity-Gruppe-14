#include "buzzer.h"
#include <Arduino.h>
#include <Zumo32U4.h>

Zumo32U4Buzzer buzzer;

static const uint16_t MIN_FREQ = 600;
static const uint16_t MAX_FREQ = 1200;
static const uint16_t STEP = 10; //økning i frekvens per oppdatering i Hz
static const uint16_t DURATION = 20; //varighet per tone i ms
static const uint8_t VOLUME = 15; //maks volum

static bool sirenOn = false;
static uint16_t currentFreq = MIN_FREQ; 
static bool increasing = true; //indikerer om frekvensen øker eller minker

void startSiren() 
{
    sirenOn = true; 
    currentFreq = MIN_FREQ;
    increasing = true;
}

void sirenUpdate() 
{
    if (!sirenOn)
        return;
    
    buzzer.playFrequency(currentFreq, DURATION, VOLUME);

    if (increasing) 
    {
        currentFreq += STEP;
        if (currentFreq >= MAX_FREQ) 
        {
            currentFreq = MAX_FREQ;
            increasing = false; //start å minke frekvensen
        }
}
    else 
    {
        currentFreq -= STEP;
        if (currentFreq <= MIN_FREQ) 
        {
            currentFreq = MIN_FREQ;
            increasing = true; //start å øke frekvensen
        }
    }
}

void stopSiren() 
{
    sirenOn = false;
    buzzer.stopPlaying(); //stopp eventuelle pågående toner
}

bool isSirenOn() 
{
    return sirenOn;
}