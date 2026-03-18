#include "buzzer.h"
#include <Arduino.h>
#include <Zumo32U4.h>

Zumo32U4Buzzer buzzer;

static bool sirenOn = false;
static const char sirenSound[] = "l8 a f";

void startSiren()
{
  sirenOn = true;
  buzzer.play(sirenSound);
}

void updateSiren()
{
  if (sirenOn && !buzzer.isPlaying())
  {
    buzzer.play(sirenSound);
  }
}

void stopSiren()
{
  sirenOn = false;
  buzzer.stopPlaying();
}