#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"

Wheel L_wheel(2/*control*/, 0/*feedback*/);
Wheel R_wheel(15, 0);
Line line(13, 35, 14, 27, 26);

// time_t run_time;
const double v_0=30;
const double v_rst = 30;

double v_0r = 50;
double v_0l = 45;
const double Kpl = 5.0;
const double Kpr = 1.0;

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
  //Serial.println(line.stop);
  if(line.stop){
    L_wheel.control(0);
    R_wheel.control(0);
  }
  else{
    //Serial.println("go");
    if(line.getLspd()==1 ) v_0l += (110-v_0l)/2;
    if(line.getRspd()==1) v_0l += 60.0;
    if(v_0l>100) v_0l=100;
    //if(line.getRspd()==1) v_0r = v_rst;
    L_wheel.control(-1*(v_0l));
    R_wheel.control(v_0r);
    /*Serial.print(v_0+v_0l);
    Serial.print(" ");
    Serial.print(v_0+v_0r);
    Serial.print(" ");
    Serial.println();*/
    v_0l -= Kpl;
    v_0l = max(40.0,v_0l);
    //v_0r -= Kpr;
    v_0r = max(0.0,v_0r);
  }
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