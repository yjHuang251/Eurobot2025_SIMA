#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"
#include "Chassis.h"

const int v_0=40;

Wheel L_wheel(2/*control*/, 15/*feedback*/, TIM_NUM_L);
Wheel R_wheel(33, 32, TIM_NUM_R);
Chassis Diff(&L_wheel, &R_wheel);
// Line line(13, 25, 14, 27, 26);

void IRAM_ATTR LhandlePulse(){
  L_wheel.handlePulse();
}

void IRAM_ATTR RhandlePulse(){
  R_wheel.handlePulse();
}

void IRAM_ATTR LtimCallback(){
  L_wheel.sendPulse();
}

void IRAM_ATTR RtimCallback(){
  R_wheel.sendPulse();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // line.init();
  // while(!Serial.available()) {} // int test=Serial.parseInt();

  L_wheel.control(0, 1);
  R_wheel.control(0, 1);
  // L_wheel.thetaControl(10, 1, 90);
  // R_wheel.thetaControl(10, 1, 90);

  attachInterrupt(L_wheel.EXTIpin(), &LhandlePulse, CHANGE);
  attachInterrupt(R_wheel.EXTIpin(), &RhandlePulse, CHANGE);

  timerAttachInterrupt(L_wheel.getTim(), &LtimCallback, true);
  timerAttachInterrupt(R_wheel.getTim(), &RtimCallback, true);
  // delay(1000);
  // Diff.forwardKinematics();
  // Diff.updatePose();
  // Serial.print(" x=");
  // Serial.print(Diff.x);
  // Serial.print(", y=");
  // Serial.print(Diff.y);
  // Serial.print(", theta=");
  // Serial.println(Diff.theta);
  while(1){
    if(L_wheel.getIfData()&&R_wheel.getIfData()){
      L_wheel.feedback();
      R_wheel.feedback();
      // Serial.println("===================");
      Diff.forwardKinematics();
      Diff.updatePose();
      break;
    }
  }
  L_wheel.control(v_0, -1);
  R_wheel.control(v_0, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  // vTaskDelay(portMAX_DELAY);
  // if(L_wheel.getIfData()||R_wheel.getIfData()){
    L_wheel.feedback();
    R_wheel.feedback();
    Diff.forwardKinematics();
    Diff.updatePose();
  // }
  // Serial.print("[ ");
  // Serial.print(Diff.now/1000000.00);
  // Serial.print("] : ");
  Serial.print(" x=");
  Serial.print(Diff.x);
  Serial.print(", y=");
  Serial.print(Diff.y);
  Serial.print(", theta=");
  Serial.println(Diff.theta);

  if(abs(Diff.x-500)<=3||Diff.x>500+2/*||Serial.available()*/){
  // if(Diff.x>=WEL_RADIUS*0.5*PI){ // wheel rotate 90 deg
  // if(abs(Diff.theta-3.1415926535)<=0.1||Diff.theta>3.1415926535+0.05){ // theta
    Serial.print(" x=");
    Serial.print(Diff.x);
    Serial.print(", y=");
    Serial.print(Diff.y);
    Serial.print(", theta=");
    Serial.println(Diff.theta);
    L_wheel.control(0, -1);
    R_wheel.control(0, 1);
    while(1){}
  }

  // if(Diff.theta>=3.1415926||Serial.read()=='a'){
  //   // Serial.print("[ ");
  //   // Serial.print(Diff.now/1000000.00);
  //   // Serial.print("] : ");
  //   Serial.print(" x=");
  //   Serial.print(Diff.x);
  //   Serial.print(", y=");
  //   Serial.print(Diff.y);
  //   Serial.print(", theta=");
  //   Serial.println(Diff.theta);
  //   L_wheel.control(0, 1);
  //   R_wheel.control(0, 1);
  //   while(1){}
  // }
}

