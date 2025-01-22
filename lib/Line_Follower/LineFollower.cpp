#include "LineFollower.h"

QTRSensors qtr;

LineFollower::LineFollower(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4, uint8_t p5, uint8_t p6, uint8_t p7) : stop_cross(false), stop_t_shape(false), stop_superstar(false){

  qtr.setTypeRC();
  uint8_t pinArray[] = {p0, p1, p2, p3, p4, p5, p6, p7};
  qtr.setSensorPins(pinArray, sizeof(pinArray) / sizeof(pinArray[0]));
  
  for (uint8_t i = 0; i < 250; i++) {
    qtr.calibrate();
    delay(20);
  }
}


void LineFollower::PrintLineStatus(LineStatus status){
    switch(status){
        case Linedata:
            //qtr.read(sensorValues);
            for(uint8_t i=0; i<8; i++){
                Serial.print(sensorValues[i]);
                Serial.print("  ");
            }
            Serial.print("\n");
            break;
        case motorSpeedinfo:
            Serial.print("LMSpeed: ");
            Serial.print(LMSpeed);
            Serial.print("  ");
            Serial.print("RMSpeed: ");
            Serial.print(RMSpeed);
            Serial.print("\n");
            break;
        case Custom:
            uint16_t sensor[8];
            int16_t position = qtr.readLineBlack(sensor);
            Serial.println(position);
            break;
    }
}

void LineFollower::PID(double KP, double KD, int V0 , int duration , StopMode mode){
    if((stop_cross && mode == StopMode::cross) || (stop_t_shape && mode == StopMode::t_shape) || (stop_superstar && mode == StopMode::superstar)){
        stop();
        return;
    }
    static double lastError = 0;
    double error = 0;
    qtr.read(sensorValues);
    int cntblack = 0;
    int cntwhite = 0;
    for(int i=0; i<8; i++){
        if(sensorValues[i]>=1000){
            cntblack++;
            error += (i-3.5)*10;
        }
        else if(sensorValues[i]<=200){
            cntwhite++;
        }
    }
    if((mode == StopMode::t_shape && stop_cross && cntblack>=2 && (sensorValues[7]>=1000 || sensorValues[6]>=1000)) || duration>=stop_t_shape_duration){
        stop_t_shape = true;
        return;
    }
    if(cntblack >= 5 && mode == StopMode::superstar)
        stop_superstar = true;
        
    if((cntblack >= 3 || ((cntblack>=2) && (sensorValues[7]>=1000 && sensorValues[6]>=1000)) || cntwhite>5) || duration>=stop_cross_duration){
        if(duration>=6000){
            stop_cross = true;
            return;
        }
    }
    //
    //double nonLinearGain = (abs(error) < 20) ? 1.5 : 1.0;
    double motorSpeed = KP * error + KD * (error - lastError);
    //Serial.println(error);

    lastError = error;

    RMSpeed = V0 - motorSpeed;
    LMSpeed = V0 + motorSpeed;

    if (LMSpeed < 0) { LMSpeed = 0; }
    if (RMSpeed < 0) { RMSpeed = 0; }
}

void LineFollower::stop(){
    LMSpeed = 0;
    RMSpeed = 0;
}