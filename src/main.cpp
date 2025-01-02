#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"

Wheel L_wheel(2/*control*/, 15/*feedback*/, 0/*timer*/);
// Wheel R_wheel(15, 0, 1);
// Line line(13, 25, 14, 27, 26);

time_t run_time;
// const int v_0=30;
int k=0;
int last_read=0;
// portMUX_TYPE timer_mux=portMUX_INITIALIZER_UNLOCKED;
// volatile unsigned long fb_high=0;
// volatile unsigned long fb_low=0;
// volatile int period=0;
// volatile float duty_cycle=0.0;

// put function declarations here:

// void IRAM_ATTR handlePulse(){
//     portENTER_CRITICAL_ISR(&timer_mux);

//     if(!digitalRead(15)){
//         fb_high=micros();
//     }
//     else{
//         period=int(micros()-fb_low);
//         fb_low=micros();
//         duty_cycle=float(fb_high-fb_low)/float(period);
//     }
//     portEXIT_CRITICAL_ISR(&timer_mux);
// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // line.init();
  // L_wheel.control(v_0);
  // R_wheel.control(v_0);
  // pinMode(15, INPUT);
  // attachInterrupt(digitalPinToInterrupt(15), handlePulse, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  // line.PIDcontrol();
  // L_wheel.control(-1*(v_0*line.getLspd()));
  // R_wheel.control(v_0*line.getRspd());
  // Serial.println(digitalRead(15));
  L_wheel.control(30);
  // if(abs(digitalRead(15)-last_read)==1) Serial.println("pin change");
  // last_read=digitalRead(15);
  Serial.println(L_wheel.n);
}

// put function definitions here: