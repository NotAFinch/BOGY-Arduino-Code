#include "Gyro.h"
#include "Motor.h"
#include "ServoMotor.h"
#include "Ultraschall.h"
#include "IRremote.h"
#include "smartcarDef.h"
#include "DriverFunctions.h"
#include "MPU6050.h"

IRrecv irRecv(9);
decode_results irInput;
uint8_t IRData;
bool remoteMode = true;
int motorBaseSpeed = 130; // max speed (rfn ?)
float speedMod;  //(0 = min; 1 = max)
int maxSpeedDistance = 150; //cm 
int maxGroundDistance = 650;
unsigned int compareMinDiff = 30;
bool trigTest;

uint8_t detectCycles = 0;
uint8_t minCycles = 5;
uint8_t speed = 130; // 0 - 255
bool moveEnable = false;

bool gZMD = false;
uint16_t ZMCounter = 0;
int input;

enum servoPostions {
  L = 0,
  LC = 55,
  C = 90,
  RC = 125,
  R = 180 
};

enum movementConfigurations {
  stop,
  forward,
  slowForward,
  left,
  sharpLeft,
  right,
  sharpRight,
  backward,
  backLeft,
  backRight,
};

const char *moveTypes[] = {
  "stop",
  "forward",
  "slowForward",
  "left",
  "sharpLeft",
  "right",
  "sharpRight",
  "backward",
  "backLeft",
  "backRight",
};

movementConfigurations curMC;

static uint16_t distance;
static uint16_t *getDistance = &distance;
static uint16_t pastDistance;
static uint16_t *getPastDistance = &pastDistance;
float yaw;
float *getYaw = &yaw;
float targetYaw;

int loopCounter = 0;

Motor motor;
RBGLED rgbled;
MPU6050 mpu;

bool gyroStatus = false;
unsigned int servoAngle = 90;

bool Debug = true;
bool masterToggle = true;
int serialIn;

void setup() {
  //Init
  Serial.begin(9600);
  delay(250);
  Serial.println("RGBLED Init");
  rgbled.RBGLED_Init(15);
  rgbled.RBGLED_Set(100, 1, CRGB::Black);
  rgbled.RBGLED_Set(250, 1, CRGB::Red);

  Serial.print("Gyro Init: ");
  mpu.initialize();
  mpu.setIntZeroMotionEnabled(true);
  gyroStatus = gyro.Init();
  
  Serial.print("Motor Init: ");
  motor.Init();
  
  Serial.print("Servo Init: ");
  servo.Init(servoAngle);
  
  Serial.print("Ultrasonic Init & Test: ");
  ultraschall.Init();
  ultraschall.Test();
  
  Serial.println("TrackDectect Init:");
  InitTrackDetect();
  
  rgbled.RBGLED_Set(200, 1, CRGB::Black);
  Serial.println("Init Done!");
  rgbled.RBGLED_Set(1000, 1, CRGB::Green);
  
  Serial.print("Calibrating Gyro ...");
  gyro.Calibration();
  mpu.setZeroMotionDetectionThreshold(100);
  mpu.setZeroMotionDetectionDuration(15);
  Serial.println("Done"),
  rgbled.RBGLED_Set(200, 1, CRGB::Black);

  irRecv.enableIRIn();
  Serial.println("IRRecv Enabled");

  rgbled.RBGLED_Set(250, 1, CRGB::Blue);
  Serial.println("------------------------------------\n      Setup Done!\n------------------------------------\n");
  delay(200);
  masterToggle = false;
  rgbled.RBGLED_Set(1000, 1, CRGB::Red);
  Serial.println("MasterToggle off");
}

void loop() {
  delay(5);  
  IRData = IRRead();
  cmdRemoteExec();
  if (masterToggle == true){
    remoteMove(IRData);
    if (moveEnable == true) {
      curMC = avoidanceDetect();
    }
    //rgbled.RBGLED_Set(1, 1, CRGB::Blue);
    getSpeedModLinear();
    getGyroData();
  }
  else {
    rgbled.RBGLED_Set(2, 1, CRGB::Red);
    curMC = stop;
  }

  smartDrive(curMC);
  if (gZMD) {
    ZMCounter++;
  }
  if (ZMCounter > 300) {
    Serial.println("ZeroMotion");
    ZMCounter = 0;
    trigTest = true;
    rgbled.RBGLED_Set(500, 1, CRGB::Black);
  }

  if (Serial.available() > 0) {
    float number = Serial.parseFloat();

    Serial.print("received: ");
    Serial.println(number);
    turnByAngle(number);
  }
}

void getGyroData(void){
  gZMD = mpu.getZeroMotionDetected();
  gyro.GetAngles(getYaw);
  //Serial.println((float)yaw);
}

uint8_t IRRead(void) {
  uint8_t ret = 0;
  if (irRecv.decode(&irInput))
  {
    rgbled.RBGLED_Set(250, 1, CRGB::Green);
    
    switch (irInput.value)
    {
    case aRECV_upper:
    case bRECV_upper:
      /* code */ ret = 1;
      break;
    case aRECV_lower:
    case bRECV_lower:
      /* code */ ret = 2;
      break;
    case aRECV_Left:
    case bRECV_Left:
      /* code */ ret = 3;
      break;
    case aRECV_right:
    case bRECV_right:
      /* code */ ret = 4;
      break;
    case aRECV_ok:
    case bRECV_ok:
      /* code */ ret = 5;
      break;

    case aRECV_1:
    case bRECV_1:
      /* code */ ret = 6;
      break;
    case aRECV_2:
    case bRECV_2:
      /* code */ ret = 7;
      break;
    case aRECV_3:
    case bRECV_3:
      /* code */ ret = 8;
      break;
    case aRECV_4:
    case bRECV_4:
      /* code */ ret = 9;
      break;
    case aRECV_5:
    case bRECV_5:
      /* code */ ret = 10;
      break;
    case aRECV_6:
    case bRECV_6:
      /* code */ ret = 11;
      break;
    case aRECV_7:
    case bRECV_7:
      /* code */ ret = 12;
      break;
    case aRECV_8:
    case bRECV_8:
      /* code */ ret = 13;
      break;
    case aRECV_9:
    case bRECV_9:
      /* code */ ret = 14;
      break;
    case aRECV_A:
    case bRECV_A:
      /* code */ ret = 15;
      break;
    case aRECV_0:
    case bRECV_0:
      /* code */ ret = 16;
      break;
    case aRECV_B:
    case bRECV_B:
      /* code */ ret = 17;
      break;
    default:
      // *IRrecv_Get = 5;
      irRecv.resume();
      return 0;
      break;
    }
    irRecv.resume();
    Serial.print("pressed: ");
    Serial.println(int(ret)) ;
    return ret;
  }
  else
  {
    return 42;
  }
}

void remoteMove(uint8_t Data) {
  if (getIsGrounded() == true){
    if (remoteMode) {
      int motorLowSpeed = (motorBaseSpeed * 0.5);
      int motorHighSpeed = (motorBaseSpeed * 1.5);
      switch(Data){
        case 1:
          motor.Control(true,motorBaseSpeed,true,motorBaseSpeed,true);
          break;
        case 2:
          motor.Control(true,0,true,0,true);
          break;
        case 3:
          motor.Control(true,motorHighSpeed,true,motorLowSpeed,true);
          break;
        case 4:
          motor.Control(true,motorLowSpeed,true,motorHighSpeed,true);
          break;
      }
    }
  }
  else {
    motor.Control(true,0,true,0,true);
  }
}

void cmdRemoteExec(void){
  switch(IRData){
    case 1:
      //do
      break;
    case 2:
      //do
      break;
    case 3:
      //do
      break;
    case 4:
      //do
      break;
    case 5:
      Serial.println(getTrackLeft());
      Serial.println(getTrackCenter());
      Serial.println(getTrackRight());
      Serial.print("Avr: ");
      Serial.println((getTrackLeft() + getTrackCenter() + getTrackRight() ) / 3);
      break;
    case 6:
      //do
      break;
    case 7:
      Serial.print("Distance: ");
      ultraschall.Test();
      Serial.print("SpeedMod: ");
      Serial.println(speedMod);
      Serial.print("SpeedSuggest: ");
      Serial.println(int((speed * speedMod) + 40));
      break;
    case 8:
      if (speed < 220) {
        speed += 10;
      }
      break;
    case 9:
      turnByAngle(180.0);
      break;
    case 10:
      moveEnable = !moveEnable;
      break;
    case 11:
      getGyroData();
      break;
    case 12:
      speed = 50;
      break;
    case 13:
      Serial.println(compareDistancePosition(LC,RC));
      Serial.print("Center:");
      getDistancePostion(C,getDistance);
      Serial.println(distance);
      Serial.print("LC:");
      getDistancePostion(LC,getDistance);
      Serial.println(distance);
      Serial.print("RC:");
      getDistancePostion(RC,getDistance);
      Serial.println(distance);
      break;
    case 14:
      //do
      break;
    case 15:
      if (speed > 10){
        speed -= 10;
      }   
      break; 
    case 16:
      masterToggle = !masterToggle;
      Serial.print("MasterToggle = ");
      Serial.println(masterToggle);
      motor.Control(true,0,true,0,false);
      break;
    case 17:
      turnByAngle(720.0);
      break;  
  }
}

void InitTrackDetect(void){
  pinMode(PIN_TRACKL, INPUT);
  pinMode(PIN_TRACKC, INPUT);
  pinMode(PIN_TRACKR, INPUT);
  return;
}

int getTrackLeft(void){return analogRead(PIN_TRACKL);}
int getTrackCenter(void){return analogRead(PIN_TRACKC);}
int getTrackRight(void){return analogRead(PIN_TRACKR);}

bool getSpeedModLinear(void){
  static uint16_t distance;
  static uint16_t *getDistance = &distance;
  int speedModTemp;
  ultraschall.Get(getDistance);
  speedModTemp = map(distance,0,maxSpeedDistance,30,100);
  speedMod = (float)speedModTemp / 100;
}

bool getIsGrounded(void){
  //Sum of Distances / Num Distances
  float avrDistance = ((getTrackLeft() + getTrackCenter() + getTrackRight() ) / 3);
  rgbled.RBGLED_Set(1, 1, CRGB::Blue);
  if (avrDistance < maxGroundDistance){
    return true;
  }
  else {
    return false;
    Serial.print("not grounded!");
  }
}

movementConfigurations avoidanceDetect(void){
  static movementConfigurations avoidSuggest;
  static uint8_t stopCycles;
  static uint8_t checkCycle;
  uint8_t compareReturn;
  
  if (trigTest) {
     goto detectCycleLabel;
     trigTest = false;
  }
  
  if (detectCycles >= minCycles){
    detectCycles = 0;
    getDistancePostion(C,getDistance);
    rgbled.RBGLED_Set(200, 1, CRGB::Yellow);
    if (distance < 25) {
      avoidSuggest = slowForward;
      rgbled.RBGLED_Set(200, 1, CRGB::Red2);
      if (distance < 12) {
        avoidSuggest = stop;
        stopCycles++;
        rgbled.RBGLED_Set(200, 1, CRGB::Red1);
        if (stopCycles > 1) {
          compareReturn = compareDistancePosition(L,R);
          if (compareReturn == 1) {avoidSuggest = backRight;} 
          else if (compareReturn == 0) {avoidSuggest = backLeft;}
          else {avoidSuggest = backLeft;}
          detectCycles = -15;
        }
        return avoidSuggest;
      }
    } else if (distance < 75) {
      detectCycleLabel:
      stopCycles = 0;
      rgbled.RBGLED_Set(300, 1, CRGB::Orange);
      compareReturn = compareDistancePosition(LC,RC);
      if (compareReturn < 2){
        if (compareReturn){avoidSuggest = right;} 
        else {avoidSuggest = left;}
      }
      if (abs(pastDistance - distance) < 3) {
        rgbled.RBGLED_Set(500, 1, CRGB::Red3);
        compareReturn = compareDistancePosition(L,R);
        if ( compareReturn == 1){avoidSuggest = sharpRight;} 
        else if (compareReturn == 0){avoidSuggest = sharpLeft;}
      }
    } else {
      avoidSuggest = forward;
      checkCycle++;
      if (checkCycle > 10) {
        checkCycle = 0;
        goto detectCycleLabel;
      }
      rgbled.RBGLED_Set(100, 1, CRGB::Purple); 
    }
  }
  if (avoidSuggest != forward) {
    Serial.print("OAS: ");
    Serial.println(moveTypes[avoidSuggest]);
  }
  detectCycles++;
  return avoidSuggest;
}

int compareDistancePosition(servoPostions A,servoPostions B){
  int ret = 0;
  getDistancePostion(A,getPastDistance);
  getDistancePostion(B,getDistance);
  servo.Control(C);
  if ((abs(pastDistance - distance)) > compareMinDiff) {
    ret = pastDistance >= distance;
  } else {
    //Serial.println("Minum not Reached ");
    ret = 2;
  }
  return ret;
}
void getDistancePostion(servoPostions pos,uint16_t *getter){
  servo.Control(pos);
  ultraschall.Get(getter);
}

void smartDrive(movementConfigurations Suggest){ //switch Case
  const bool CLED = true;
  uint8_t speedBaseValue = int((speed * speedMod) + 60);
  uint8_t speedBaseValueBack = int((speed * speedMod) + 80);
  
  if (getIsGrounded() == true && moveEnable == true) {
    const float multHigh = 1.25;
    const float multLow = 0.75;
    switch(Suggest){
      case 0:
        motor.Control(true,0,true,0,CLED);
        break;
      case 1:
        motor.Control(true,speedBaseValue,true,speedBaseValue,CLED);
        break;
      case 2:
        motor.Control(true,speedBaseValue*0.5,true,speedBaseValue*0.5,CLED);
        break;
      case 3:
        motor.Control(true,speedBaseValue*multHigh,true,speedBaseValue*multLow,CLED);
        break;
      case 4:
        turnByAngle(-85);
        break;
      case 5:
        motor.Control(true,speedBaseValue*multLow,true,speedBaseValue*multHigh,CLED);
        break;
      case 6:
        turnByAngle(+85);
        break;
      case 7:
        motor.Control(false,speedBaseValueBack,false,speedBaseValueBack,CLED);
        break;
      case 8:
        turnByAngle(-35);
        motor.Control(false,speedBaseValueBack*multLow,false,speedBaseValueBack*multHigh,CLED);
        break;
      case 9:
        turnByAngle(+35);
        motor.Control(false,speedBaseValueBack*multHigh,false,speedBaseValueBack*multLow,CLED);
        break;
    }
  }

  if (!moveEnable) {
    motor.Control(true,0,true,0,CLED);
  }
}

void turnByAngle(float angle) {
  targetYaw = targetYaw + angle;

  while (targetYaw != yaw && abs(targetYaw - yaw) > 2) {
    gyro.GetAngles(getYaw);
    static uint8_t turnSpeed = uint8_t((abs(targetYaw - yaw) * 3) + 20);
    if (targetYaw > yaw){
      motor.Control(false,turnSpeed,true,turnSpeed,true); //right
    }
    else if (targetYaw < yaw){
      motor.Control(true,turnSpeed,false,turnSpeed,true); //left  
    }
    else {
      motor.Control(true,0,true,0,true);
    }
    Serial.print(yaw);
    Serial.print(" <-> ");
    Serial.println(targetYaw);
  }
  motor.Control(true,0,true,0,true);
}