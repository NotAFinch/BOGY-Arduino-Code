#ifndef _Motor_H_
#define _Motor_H_

#include <Arduino.h>

/*Motor*/
class Motor
{
public:
  void Init(void);

  //A = Right; B = Left
  void Control(boolean direction_A, uint8_t speed_A, 
                                     boolean direction_B, uint8_t speed_B, 
                                     boolean controlED                     
  );                                                                      
private:

#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8
#define PIN_Motor_AIN_1 7
#define PIN_Motor_STBY 3

public:
#define speed_Max 255
#define direction_just true
#define direction_back false
#define direction_void 3

#define Duration_enable true
#define Duration_disable false
#define control_enable true
#define control_disable false
};

#endif
