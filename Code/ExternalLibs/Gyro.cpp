#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "Gyro.h"

#include <stdio.h>
#include <math.h>

MPU6050 accelgyro;
Gyro gyro;


bool Gyro::Init(void)
{
  Wire.begin();
  uint8_t chip_id = 0x00;
  uint8_t cout = 0;
  do
  {
    chip_id = accelgyro.getDeviceID();
    Serial.print("MPU6050_chip_id: ");
    Serial.println(chip_id);
    delay(10);
    cout += 1;
    if (cout > 10)
    {
      return true;
    }
  } while (chip_id == 0X00 || chip_id == 0XFF); //确保从机设备在线（强行等待 获取 ID ）
  accelgyro.initialize();
  // unsigned short times = 100; //采样次数
  // for (int i = 0; i < times; i++)
  // {
  //   gz = accelgyro.getRotationZ();
  //   gzo += gz;
  // }
  // gzo /= times; //计算陀螺仪偏移
  return false;
}
bool Gyro::Calibration(void)
{
  unsigned short times = 100; //采样次数
  for (unsigned int i = 0; i < times; i++)
  {
    gz = accelgyro.getRotationZ();
    gzo += gz;
  }
  gzo /= times; //计算陀螺仪偏移

  // gzo = accelgyro.getRotationZ();
  return false;
}
bool Gyro::GetAngles(float *Yaw)
{
  unsigned long now = millis();   //当前时间(ms)
  dt = (now - lastTime) / 1000.0; //微分时间(s)
  lastTime = now;                 //上一次采样时间(ms)
  gz = accelgyro.getRotationZ();
  float gyroz = -(gz - gzo) / 131.0 * dt; //z轴角速度
  if (fabs(gyroz) < 0.05)
  {
    gyroz = 0.00;
  }
  agz += gyroz; //z轴角速度积分
  *Yaw = agz;
  return false;
}
