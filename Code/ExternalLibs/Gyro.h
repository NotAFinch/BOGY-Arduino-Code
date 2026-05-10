////////////////////////////////////////////
//Klasse zum Auslesen des Gyroskops zur Bestimmung des Drehwinkels
//
//Zunächst muss im main Programm in der Setup-Schleife einmal Init und Calibration aufgerufen. Dabei muss der Roboter bereits in Ausgangslage sein (und still stehen).
//Anschließend kann mit GetAngles die Drehung um die z-Achse relativ zur Ausgangslage bestimmt werden. 

#ifndef _Gyro_H_
#define _Gyro_H_
#include <Arduino.h>
class Gyro
{
public:
  bool Init(void);
  bool Calibration(void);
  bool GetAngles(float *Yaw);

public:
  //int16_t ax, ay, az, gx, gy, gz;
  int16_t gz;
  //float pith, roll, yaw;
  unsigned long now, lastTime = 0;
  float dt;      
  float agz = 0; 
  long gzo = 0;  
};

extern Gyro gyro;
#endif
