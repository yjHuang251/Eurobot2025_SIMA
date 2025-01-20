#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"
#include "VL53L0X.h"
#include "escape.h"

#define XSHUT1 23
#define XSHUT2 19
#define XSHUT3 18

Wheel L_wheel(2/*control*/, 0/*feedback*/);
Wheel R_wheel(15, 0);
Line line(13, 12, 14, 27, 26);
VL53L0X sensor_M;
VL53L0X sensor_R;
VL53L0X sensor_L;

Escape escape(L_wheel, R_wheel, sensor_M, sensor_R, sensor_L);
// time_t run_time;
const int v_0 = 40;
int currentState;

enum state{
  lining,
  escaping,
  toFinaling
}; 

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // line.init();
  L_wheel.control(v_0);
  R_wheel.control(v_0);
  currentState = lining;
  Wire.begin(21, 22);
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  pinMode(XSHUT3, OUTPUT);
  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  delay(10);
  digitalWrite(XSHUT1, HIGH);
  delay(10);
  sensor_L.init();
  sensor_L.setAddress(0x28);
  digitalWrite(XSHUT2, HIGH);
  delay(10);
  sensor_M.init();
  sensor_M.setAddress(0x29);
  digitalWrite(XSHUT3, HIGH);
  delay(10);
  sensor_L.init();
  sensor_R.setAddress(0x2A);
  sensor_L.setTimeout(500);
  sensor_M.setTimeout(500);
  sensor_R.setTimeout(500);
    // 設定感測器超時時間
  if (!sensor_M.init()) {
    Serial.println("Failed to detect and initialize sensor_M!");
    while (1) {}
  }
  if(!sensor_L.init()){
    Serial.println("Failed to detect and initialize sensor_L!");
    while(1){}
  }
  if(!sensor_R.init()){
    Serial.println("Failed to detect and initialize sensor_R!");
    while(1){}
  }
}



void loop() {
  
  if(currentState == lining){
  // 若發生超時（感測器沒有回應），則顯示“TIMEOUT”。
    if(sensor_M.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    if(sensor_M.readRangeSingleMillimeters() <= 150 ){
      
      L_wheel.control(0);
      R_wheel.control(0);
      currentState = escaping;
    }else{
      L_wheel.control(-v_0 *1.2);
      R_wheel.control(v_0 * 1.5);

      //Serial.println("go");
    
    }
   
    }else if(currentState == escaping){
      escape.ReverseAndRotate();
      escape.escape_quarter(0.2);
      currentState = toFinaling;
    }else{
      // goFinaling
      escape.check(sensor_M);
      L_wheel.control(-v_0 *1.2);
      R_wheel.control(v_0 * 1.5);
      Serial.println("goFinaling");
    }

  }
  

// put function definitions here: