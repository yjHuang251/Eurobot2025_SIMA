#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"

Wheel L_wheel(2/*control*/, 15/*feedback*/, 0/*timer*/);
// Wheel R_wheel(23, 22, 1);
// Line line(13, 25, 14, 27, 26);

// put function declarations here:

void IRAM_ATTR isr(){
    L_wheel.handlePulse();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // line.init();
  // L_wheel.control(v_0);
  // R_wheel.control(v_0);
  // pinMode(15, INPUT);
  attachInterrupt(L_wheel.EXTIpin(), &isr, CHANGE);
  // Serial.println(L_wheel.fb_pin);
}

void loop() {
  // n++;
  // put your main code here, to run repeatedly:
  // line.PIDcontrol();
  // L_wheel.control(-1*(v_0*line.getLspd()));
  // R_wheel.control(v_0*line.getRspd());
  // Serial.println(digitalRead(15));
  L_wheel.control(20);
  // if(abs(digitalRead(15)-last_read)==1) Serial.println("pin change");
  // last_read=digitalRead(15);
  Serial.println("test");
  // Serial.println(n);
  // if(!n%10) Serial.println("ok");
  L_wheel.feedback();
  // Serial.println(L_wheel.fb_high);
  // Serial.println(L_wheel.fb_low);
  // Serial.println(L_wheel.period);
}

// put function definitions here: