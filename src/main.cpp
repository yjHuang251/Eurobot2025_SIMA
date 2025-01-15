#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"

const int v_0=50;
volatile bool if_odom[2]={false, false};

Wheel L_wheel(2/*control*/, 15/*feedback*/);
Wheel R_wheel(23, 22);
// Line line(13, 25, 14, 27, 26);

// put function declarations here:

void IRAM_ATTR LhandlePulse();
void IRAM_ATTR RhandlePulse();
void IRAM_ATTR LtimCallback();
void IRAM_ATTR RtimCallback();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // line.init();
  L_wheel.init(TIM_NUM_L);
  R_wheel.init(TIM_NUM_R);
  L_wheel.control(v_0);
  R_wheel.control(v_0);

  attachInterrupt(L_wheel.EXTIpin(), &LhandlePulse, CHANGE);
  attachInterrupt(R_wheel.EXTIpin(), &RhandlePulse, CHANGE);

  timerAttachInterrupt(L_wheel.getTim(), &LtimCallback, true);
  timerAttachInterrupt(R_wheel.getTim(), &RtimCallback, true);

}

void loop() {
  // put your main code here, to run repeatedly:
  // line.PIDcontrol();
  // L_wheel.control(-1*(v_0*line.getLspd()));
  // R_wheel.control(v_0*line.getRspd());
  // L_wheel.control(20);
  // if(L_wheel.getIfData()){
  //   L_wheel.feedback();
  //   Serial.print("[ ");
  //   Serial.print(micros()/1000000.00);
  //   Serial.print(" s ] ");
  //   Serial.print("L wheel theta: ");
  //   Serial.println(L_wheel.theta);
  // }
  // if(R_wheel.getIfData()){
  //   R_wheel.feedback();
  //   Serial.print("[ ");
  //   Serial.print(micros()/1000000.00);
  //   Serial.print(" s ] ");
  //   Serial.print("R wheel theta: ");
  //   Serial.println(R_wheel.theta);
  // }
  if(if_odom[0]&&if_odom[1]){
    L_wheel.feedback();
    R_wheel.feedback();
    Serial.print("[ ");
    Serial.print(micros()/1000000.00);
    Serial.print(" s ] ");
    Serial.print("L wheel theta: ");
    Serial.print(L_wheel.theta);
    Serial.print(", R wheel theta: ");
    Serial.println(R_wheel.theta);
  }
  // vTaskDelay(portMAX_DELAY);
}

// put function definitions here:

void IRAM_ATTR LhandlePulse(){
  if_odom[0]=L_wheel.handlePulse();
}

void IRAM_ATTR RhandlePulse(){
  if_odom[1]=R_wheel.handlePulse();
}

void IRAM_ATTR LtimCallback(){
  L_wheel.sendPulse();
}

void IRAM_ATTR RtimCallback(){
  R_wheel.sendPulse();
}
