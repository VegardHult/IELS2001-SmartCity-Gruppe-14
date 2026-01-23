#ifndef LINEFOLLOWING_H
#define LINEFOLLOWING_H

#include <Arduino.h>
#include <Zumo32U4.h>

extern Zumo32U4LineSensors lineSensors;
extern Zumo32U4Motors motors;

// Egen linjeleser
int readSensors(unsigned int *sensor_values, unsigned char readMode, unsigned char white_line);

// Håndterer all linjefølging
void followLine(int max_speed);

#endif // LINEFOLLOWING_H
