#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"

Wheel L_wheel(2/*control*/, 0/*feedback*/, 0/*timer*/);
Wheel R_wheel(15, 0, 1);
Line line(13, 25, 14, 27, 26);

// time_t run_time;
const int v_0=0;

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  line.init();
  L_wheel.control(v_0);
  R_wheel.control(v_0);
}

void loop() {
  // put your main code here, to run repeatedly:
  // line.PIDcontrol();
  // L_wheel.control(-1*(v_0*line.getLspd()));
  // R_wheel.control(v_0*line.getRspd());
  
  L_wheel.control(70);
}

// put function definitions here: