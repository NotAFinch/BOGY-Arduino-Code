#ifndef _ServoMotor_H_
#define _ServoMotor_H_

/*Servo*/
#include <Servo.h>
class ServoMotor
{
public:
  void Init(unsigned int Position_angle);

  void Control(unsigned int Position_angle);

private:
#define PIN_Servo_z 10
};

extern ServoMotor servo;

#endif
