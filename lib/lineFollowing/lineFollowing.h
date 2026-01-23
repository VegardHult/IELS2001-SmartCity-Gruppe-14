#ifndef LINEFOLLOWING_H
#define LINEFOLLOWING_H

extern Zumo32U4LineSensors lineSensors;
extern Zumo32U4Motors motors;

void calibrateSensors();
void followLine();
bool paKryss();

#endif // LINEFOLLOWING_H
