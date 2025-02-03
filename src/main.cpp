#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"
#include "Chassis.h"

const int v_0=50;

Wheel L_wheel(2/*control*/, 15/*feedback*/, TIM_NUM_L);
Wheel R_wheel(23, 22, TIM_NUM_R);
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
  while(!Serial.available()) {} // int test=Serial.parseInt();

  L_wheel.control(v_0, 1);
  R_wheel.control(v_0, 1);
  // R_wheel.thetaControl(50, 540);

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
  // while(1){
  //   if(L_wheel.getIfData()&&R_wheel.getIfData()){
  //     L_wheel.feedback();
  //     R_wheel.feedback();
  //     // Serial.println("===================");
  //     break;
  //   }
  // }
  // while(1){}
}

void loop() {
  // put your main code here, to run repeatedly:
  if(L_wheel.getIfData()||R_wheel.getIfData()){
    // Serial.print("[left] ");
    // if(L_wheel.getIfData()){
      // Serial.print("[left] ");
      L_wheel.feedback();
    // }
    // Serial.print(" [right] ");
    // if(R_wheel.getIfData()){
      // Serial.print(" [right] ");
      R_wheel.feedback();
    // }
    // Serial.println();
    Diff.forwardKinematics();
    Diff.updatePose();
  }
  Serial.print("[ ");
  Serial.print(Diff.now/1000000.00);
  Serial.print("] : ");
  Serial.print(" x=");
  Serial.print(Diff.x);
  Serial.print(", y=");
  Serial.print(Diff.y);
  Serial.print(", theta=");
  Serial.println(Diff.theta);

  if(Diff.theta>=3.1415926||Serial.read()=='a'){
    // Serial.print("[ ");
    // Serial.print(Diff.now/1000000.00);
    // Serial.print("] : ");
    Serial.print(" x=");
    Serial.print(Diff.x);
    Serial.print(", y=");
    Serial.print(Diff.y);
    Serial.print(", theta=");
    Serial.println(Diff.theta);
    L_wheel.control(0, 1);
    R_wheel.control(0, 1);
    while(1){}
  }
  // Serial.print(". [left] ");
  // Serial.print("prev_theta=");
  // Serial.print(Diff.left_wheel->prev_theta);
  // Serial.print(", theta=");
  // Serial.print(Diff.left_wheel->theta);
  // Serial.print(", d_theta=");
  // Serial.print(Diff.left_wheel->d_theta);
  // Serial.print("linear_vel=");
  // Serial.print(Diff.left_wheel->getCarInfo().linear_vel);
  // Serial.print(". [right] ");
  // Serial.print("prev_theta=");
  // Serial.print(Diff.right_wheel->prev_theta);
  // Serial.print(", theta=");
  // Serial.print(Diff.right_wheel->theta);
  // Serial.print(", d_theta=");
  // Serial.print(Diff.right_wheel->d_theta);
  // Serial.print("linear_vel=");
  // Serial.println(Diff.right_wheel->getCarInfo().linear_vel);
  // Serial.print("v_x=");
  // Serial.print(Diff.v_x);
  // Serial.print(", v_y=");
  // Serial.print(Diff.v_y);
  // Serial.print(", omega=");
  // Serial.print(Diff.omega);
  // Serial.print(", last=");
  // Serial.print(Diff.last);
  // Serial.print(", now=");
  // Serial.print(Diff.now);
  // Serial.print(", time delay: ");
  // Serial.print(Diff.dt);
  // Serial.println(" micro seconds");
  // Serial.print(", left wheel speed: ");
  // Serial.print(Diff.left_wheel->getCarInfo().linear_vel);
  // Serial.print(", right wheel speed: ");
  // Serial.println(Diff.right_wheel->getCarInfo().linear_vel);
  // Serial.println(Diff.left_wheel->theta);
  // Serial.print("[time delay] left wheel: ");
  // Serial.print(Diff.left_wheel->dt);
  // Serial.print(", right wheel: ");
  // Serial.println(Diff.right_wheel->dt);

  // R_wheel.thetaControl(50, 540);
  // R_wheel.feedback();
  // Serial.println("k");
  // Serial.println(L_wheel.ctrl_high);
  // vTaskDelay(portMAX_DELAY);
  // if(R_wheel.deg_ok) {
  //   R_wheel.control(0);
  //   R_wheel.deg_ok=false;
  // }
}

