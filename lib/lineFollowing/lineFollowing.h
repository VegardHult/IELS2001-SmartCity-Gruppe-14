#ifndef LINEFOLLOWING_H
#define LINEFOLLOWING_H

#include <Arduino.h>
#include <Zumo32U4.h>

//endre disse for å tune styringen
#define PROPORTIONAL_CONSTANT 0.4
#define DERIVATIVE_CONSTANT 1.5

extern Zumo32U4LineSensors lineSensors;
extern Zumo32U4Motors motors;

// Egen linjeleser
int readSensors(unsigned int *sensor_values, unsigned char readMode, unsigned char white_line);

// Håndterer all linjefølging
bool followLine(int max_speed, float prop_const = PROPORTIONAL_CONSTANT, float der_const = DERIVATIVE_CONSTANT);

bool paKryss();

void testLineFollowing();

#endif // LINEFOLLOWING_H
