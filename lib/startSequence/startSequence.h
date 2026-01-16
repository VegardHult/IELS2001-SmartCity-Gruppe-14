#ifndef STARTSEQUENCE_H
#define STARTSEQUENCE_H

//tenker at disse kan være definert i main-fila
extern Zumo32U4LineSensors lineSensors;
extern Zumo32U4Motors motors;

//starte sensorene
void initSensors();
//kalibrerer og finner tilbake til linja
void calibrateZumo();

#endif // STARTSEQUENCE_H
