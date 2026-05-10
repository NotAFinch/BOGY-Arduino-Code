#ifndef _DriverFunctions_H_
#define _DriverFunctions_H_

#include "FastLED.h"
class RBGLED
{
public:
  void RBGLED_Init(uint8_t set_Brightness);
  void RBGLED_Set(uint16_t Duration, uint8_t Traversal_Number, CRGB colour);
  void RBGLED_Color(uint8_t LED_s, uint8_t r, uint8_t g, uint8_t b);

public:
private:
#define PIN_RBGLED 4
#define NUM_LEDS 1
public:
  CRGB leds[NUM_LEDS];
};

#endif
