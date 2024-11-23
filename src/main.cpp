#include <Arduino.h>
#include "Wheel.h"

time_t run_time;
Wheel wheel(17/*control*/, 18/*feedback*/);

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  run_time=millis();
  while(1){
    wheel.control(140);
    if((millis()-run_time)>5000) break;
    Serial.println(wheel.getTheta());
  }
  delay(500);
  run_time=millis();
  while(1){
    wheel.control(-100);
    if((millis()-run_time)>3000) break;
    Serial.println(wheel.getTheta());
  }

  while(1){
    wheel.control(50, 120);
    if(!wheel.ifTheta()){
      wheel.ifThetaVal(false);
      break;
    }
    Serial.println(wheel.getTheta());
  }
  delay(2000);
}

// put function definitions here: