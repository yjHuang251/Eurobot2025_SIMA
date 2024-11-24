#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"

Wheel L_wheel(6/*control*/, 7/*feedback*/);
Wheel R_wheel(8, 9);
Line line(1, 2, 3, 4, 5);

time_t run_time;
const int v_0=30;

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  L_wheel.control(50);
  R_wheel.control(v_0);
}

void loop() {
  // put your main code here, to run repeatedly:
  line.PIDcontrol();
  L_wheel.control(v_0+line.getLspd());
  R_wheel.control(v_0+line.getRspd());
}

// put function definitions here: