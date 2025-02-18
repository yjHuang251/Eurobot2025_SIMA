// #include <Arduino.h>
// #include "Wheel.h"
// #include "Chassis.h"

// float init_point[2]={0.0, 0.0};
// float goal_point[2]={0.0, 0.0};

// const float v_0=35.0;

// Wheel L_wheel(2/*control*/, 15/*feedback*/, TIM_NUM_L);
// Wheel R_wheel(33, 32, TIM_NUM_R);
// Chassis Diff(&L_wheel, &R_wheel);

// void IRAM_ATTR LhandlePulse(){
//   L_wheel.handlePulse();
// }

// void IRAM_ATTR RhandlePulse(){
//   R_wheel.handlePulse();
// }

// void IRAM_ATTR LtimCallback(){
//   L_wheel.sendPulse();
// }

// void IRAM_ATTR RtimCallback(){
//   R_wheel.sendPulse();
// }

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);

//   L_wheel.control(0, 1);
//   R_wheel.control(0, 1);

//   attachInterrupt(L_wheel.EXTIpin(), &LhandlePulse, CHANGE);
//   attachInterrupt(R_wheel.EXTIpin(), &RhandlePulse, CHANGE);

//   timerAttachInterrupt(L_wheel.getTim(), &LtimCallback, true);
//   timerAttachInterrupt(R_wheel.getTim(), &RtimCallback, true);

//   // wait until both wheels get feedback data
//   while(1){
//     if(L_wheel.getIfData()&&R_wheel.getIfData()){
//       L_wheel.feedback();
//       R_wheel.feedback();
//       Diff.forwardKinematics();
//       Diff.updatePose();
//       break;
//     }
//   }
//   // L_wheel.control(v_0, -1);
//   // R_wheel.control(v_0, 1);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if(L_wheel.getIfData()||R_wheel.getIfData()){
//     L_wheel.feedback();
//     R_wheel.feedback();
//     Diff.forwardKinematics();
//     Diff.updatePose();
//   }
//   if(abs(Diff.getY()-goal_point[1])>=1){
//     L_wheel.control(v_0+0.5*(Diff.getY()-goal_point[1]), -1);
//     R_wheel.control(v_0-0.5*(Diff.getY()-goal_point[1]), 1);
//   }

//   // Serial.print("left wheel speed: ");
//   // Serial.print(L_wheel.getWelInfo().linear_vel);
//   // Serial.print(", right wheel speed: ");
//   // Serial.print(R_wheel.getWelInfo().linear_vel);
//   Serial.print(", x=");
//   Serial.print(Diff.getX());
//   Serial.print(", y=");
//   Serial.print(Diff.getY());
//   Serial.print(", theta=");
//   Serial.println(Diff.getTheta());

//   if(abs(Diff.getX()-goal_point[0])<=3||Diff.getX()>goal_point[0]+2){
//   // if(Diff.x>=WEL_RADIUS*0.5*PI){ // wheel rotate 90 deg
//   // if(abs(Diff.theta-3.1415926535)<=0.1||Diff.theta>3.1415926535+0.05){ // theta
//     Serial.print(" x=");
//     Serial.print(Diff.getX());
//     Serial.print(", y=");
//     Serial.print(Diff.getY());
//     Serial.print(", theta=");
//     Serial.println(Diff.getTheta());
//     L_wheel.control(0, -1);
//     R_wheel.control(0, 1);
//     while(1){}
//   }
// }
