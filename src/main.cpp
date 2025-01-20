#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"

Wheel L_wheel(2/*control*/, 0/*feedback*/);
Wheel R_wheel(15, 0);
Line line({13, 35, 14, 27, 26});

// time_t run_time;
const double v_0=30;
const double KP = 1.0;
const double KD = 1.0;

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
  line.PIDcontrol(KP, KD, v_0);
  L_wheel.control(line.getLspd());
  R_wheel.control(line.getRspd());
} 
// put function definitions here: