#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"

Wheel L_wheel(2/*control*/, 0/*feedback*/);
Wheel R_wheel(15, 0);
Line line(13, 12, 14, 27, 26);

// time_t run_time;
const int v_0=20;

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // line.init();
  L_wheel.control(v_0);
  R_wheel.control(v_0);
}

void loop() {
  // put your main code here, to run repeatedly:
  line.PIDcontrol();
  L_wheel.control(-1*(v_0*line.getLspd()));
  R_wheel.control(v_0*line.getRspd());
  /*for(int i=0;i<5;i++){
    Serial.print(line.val[i]);
    Serial.print(" ");
  }
  Serial.println();*/
  // Serial.print("left: ");
  // Serial.println(v_0+line.getLspd());
  // Serial.print("right: ");
  // Serial.println(v_0+line.getRspd());
}

// put function definitions here: