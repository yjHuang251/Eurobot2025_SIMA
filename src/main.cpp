#include <Arduino.h>
#include "Wheel.h"
#include "Chassis.h"

const float KP=35.0;

const int final_pt=3;
float points[final_pt][3]={{0.0, 0.0, 0},
                    {1450.0, 600.0, 50.0},
                    {1900.0, 150.0, 30.0}};
int to_pt=0;

const float v_0=30.0;
bool if_rotate=false;

struct Vector{
  double i;
  double j;
};

// Vector car_vec;

Wheel L_wheel(2/*control*/, 15/*feedback*/, TIM_NUM_L);
Wheel R_wheel(33, 32, TIM_NUM_R);
Chassis Diff(&L_wheel, &R_wheel);

void IRAM_ATTR LhandlePulse(){
  L_wheel.handlePulse();
}

void IRAM_ATTR RhandlePulse(){
  R_wheel.handlePulse();
}

void IRAM_ATTR LtimCallback(){
  L_wheel.sendPulse();
}

void IRAM_ATTR RtimCallback(){
  R_wheel.sendPulse();
}

// @brief distance between two points
double difference(float to_x, float to_y, float at_x, float at_y){
  return sqrt(pow(to_x-at_x, 2)+pow(to_y-at_y, 2));
}

// @brief calculate unit vector of two points
Vector normalize(float to_x, float to_y, float at_x, float at_y){
  Vector unit_vector;
  double distance=difference(to_x, to_y, at_x, at_y);
  unit_vector.i=(to_x-at_x)/distance;
  unit_vector.j=(to_y-at_y)/distance;
  return unit_vector;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  L_wheel.control(0, 1);
  R_wheel.control(0, 1);

  attachInterrupt(L_wheel.EXTIpin(), &LhandlePulse, CHANGE);
  attachInterrupt(R_wheel.EXTIpin(), &RhandlePulse, CHANGE);

  timerAttachInterrupt(L_wheel.getTim(), &LtimCallback, true);
  timerAttachInterrupt(R_wheel.getTim(), &RtimCallback, true);

  // wait until both wheels get feedback data
  while(1){
    if(L_wheel.getIfData()&&R_wheel.getIfData()){
      L_wheel.feedback();
      R_wheel.feedback();
      Diff.forwardKinematics();
      Diff.updatePose();
      break;
    }
  }
  L_wheel.control(v_0, -1);
  R_wheel.control(v_0, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(L_wheel.getIfData()||R_wheel.getIfData()){
    L_wheel.feedback();
    R_wheel.feedback();
    Diff.forwardKinematics();
    Diff.updatePose();
  }

  if(difference(points[to_pt][0], points[to_pt][1], Diff.getX(), Diff.getY())<=points[to_pt][2]){
    // Serial.print(" x=");
    // Serial.print(Diff.getX());
    // Serial.print(", y=");
    // Serial.print(Diff.getY());
    // Serial.print(", theta=");
    // Serial.println(Diff.getTheta());
    Serial.println("POINT ARRIVED");
    L_wheel.control(0, -1);
    R_wheel.control(0, 1);
    // vTaskDelay(1000/portTICK_PERIOD_MS);
    to_pt++;
    if_rotate=true;
    if(to_pt==final_pt) vTaskDelay(portMAX_DELAY);
  }

  Vector car_vec=normalize(points[to_pt][0], points[to_pt][1], Diff.getX(), Diff.getY());
  // Serial.print("car_vec length ");
  // Serial.print(sqrt(pow(car_vec.i, 2)+pow(car_vec.j, 2)));
  // Serial.print(", heading length ");
  // Serial.println(sqrt(pow(Diff.getHeading().i, 2)+pow(Diff.getHeading().j, 2)));
  double cross_product=Diff.getHeading().i*car_vec.j-Diff.getHeading().j*car_vec.i;

  if(if_rotate){
    Serial.println("rotating...");
    L_wheel.control(2, 1);
    R_wheel.control(2, 1);
    if(cross_product<0.087) if_rotate=false;
  }
  else {
    if(abs(cross_product)>0.035){
    // if(v_0-5*cross_product>0&&v_0+5*cross_product<=140){
      L_wheel.control(v_0-KP*cross_product, -1);
      R_wheel.control(v_0+KP*cross_product, 1);
    // }
    }
    else { // if(abs(cross_product)>0.035&&!if_rotate){
      L_wheel.control(v_0, -1);
      R_wheel.control(v_0, 1);
    }
  }
  // else if(cross_product<-0.002&&if_rotate){
  //   // if(v_0-5*cross_product>0&&v_0+5*cross_product<=140){
  //     L_wheel.control(v_0-KP*cross_product, -1);
  //     R_wheel.control(v_0+KP*cross_product, 1);
  //   // }
  // }

  Serial.print(to_pt);
  Serial.print(", x=");
  Serial.print(Diff.getX());
  Serial.print(", y=");
  Serial.print(Diff.getY());
  Serial.print(", theta=");
  Serial.println(Diff.getTheta());

}
