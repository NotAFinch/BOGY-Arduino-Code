
#ifndef _Ultraschall_H_
#define _Ultraschall_H_
#include <Arduino.h>

class Ultraschall
{
public:
  void Init(void);
  void Get(uint16_t *ULTRASONIC_Get);
  void Test(void);

private:
#define TRIG_PIN 13      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 12      // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
};

extern Ultraschall ultraschall;
#endif

//https://arduinogetstarted.com/tutorials/arduino-ultrasonic-sensor
