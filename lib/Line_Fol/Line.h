#ifndef LINE_H
#define LINE_H
#include <Arduino.h>
#include <QTRSensors.h>
#include <vector>
#include <iostream>
#define SensorCount 8

typedef class LINE{
    public:
        LINE () : LMspeed(0), RMspeed(0){
            // for(uint8_t i = 0; i < SensorCount; i++){
            //     std::cout<<pins[i]<<" ";
            // }
            // std::cout<<'\n';
            qtr.setTypeAnalog();
            qtr.setSensorPins((const uint8_t[]){35,32,33,25,26,27,14,12}, SensorCount);
        }
        void init();
        void PIDcontrol(const double &KP, const double &KD, const double &v0);
        void print();
        double getLspd();
        double getRspd();
        ~LINE(){};
    private:
        QTRSensors qtr;
        uint16_t sensorValues[SensorCount];
        double LMspeed, RMspeed;
}Line;

#endif