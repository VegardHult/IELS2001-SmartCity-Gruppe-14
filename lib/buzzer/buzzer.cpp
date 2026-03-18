#include "buzzer.h"
#include <Arduino.h>
#include <Zumo32U4.h>

Zumo32U4Buzzer sirene;

static bool sirenOn = false;
static const char sirenSound[] = "l4 a f";

void startSiren()
{
  sirenOn = true;
  sirene.play(sirenSound);
}

void updateSiren()
{
  if (sirenOn && !sirene.isPlaying())
  {
    sirene.play(sirenSound);
  }
}

void stopSiren()
{
  sirenOn = false;
  sirene.stopPlaying();
}