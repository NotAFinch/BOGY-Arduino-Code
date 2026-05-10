/*
 * @Author: ELEGOO
 * @Date: 2019-10-22 11:59:09
 * @LastEditTime: 2020-06-12 16:36:20
 * @LastEditors: Changhua
 * @Description: conqueror robot tank
 * @FilePath: 
 */
#include "ServoMotor.h"
#include <Arduino.h>
/*Servo*/

Servo myservo; // create servo object to control a servo
void ServoMotor::Init(unsigned int Position_angle=0)
{
  myservo.attach(PIN_Servo_z, 500, 2400); //500: 0 degree  2400: 180 degree
  delay(1);
  myservo.attach(PIN_Servo_z);
   myservo.write(Position_angle); //sets the servo position according to the 90（middle）
   delay(1);

}


/*0.17sec/60degree(4.8v)*/
void ServoMotor::Control(unsigned int Position_angle)
{
  myservo.attach(PIN_Servo_z);
  myservo.write(Position_angle);
  delay(350);
  myservo.detach();
}
