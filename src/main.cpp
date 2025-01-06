#include <Arduino.h>
#include "Wheel.h"
#include "Line.h"
#include "VL53L0X.h"
Wheel L_wheel(2/*control*/, 0/*feedback*/);
Wheel R_wheel(15, 0);
Line line(13, 12, 14, 27, 26);
VL53L0X sensor;
// time_t run_time;
const int v_0=20;

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // line.init();
  L_wheel.control(v_0);
  R_wheel.control(v_0);
  Wire.begin();  // 啟動I2C通訊

  sensor.setTimeout(500);  // 設定感測器超時時間
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // 在序列埠監控視窗顯示測距值
  Serial.print(sensor.readRangeSingleMillimeters());
  // 若發生超時（感測器沒有回應），則顯示“TIMEOUT”。
  if(sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  if(sensor.readRangeSingleMillimeters() <= 150){
    
    L_wheel.control(0);
    R_wheel.control(0);
  
  }else{
  Serial.println();
  line.PIDcontrol();
  L_wheel.control(-1*(v_0*line.getLspd()));
  R_wheel.control(v_0*line.getRspd());
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