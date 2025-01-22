#include "Line.h"

void LINE::init(){
  for(uint16_t i = 0; i < 250; i++){
    qtr.calibrate();
    delay(20);
  }
}

void LINE::PIDcontrol(const double &KP, const double &KD, const double &v0){
  static uint16_t lastError = 0;

  uint16_t sensor[8];
  uint16_t position = qtr.readLineBlack(sensor);
  for(uint16_t i = 0; i < 8; i++){
    Serial.print(sensor[i]);
    Serial.print(" ");
  }
  Serial.println();

  uint16_t error = position - 1000;
  double motorSpeed = KP * error + KD * (error - lastError);
  lastError = error;

  LMspeed = v0 + motorSpeed;
  RMspeed = v0 - motorSpeed;

  if(LMspeed < 0) LMspeed = 0;
  if(RMspeed < 0) RMspeed = 0;
}

double LINE::getLspd(){
  return LMspeed;
}

double LINE::getRspd(){
  return RMspeed;
}

void LINE::print(){
  for(uint16_t i = 0; i < SensorCount; i++){
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }
  Serial.println();
}