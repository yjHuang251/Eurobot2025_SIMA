#ifndef LINE_H
#define LINE_H
#include <Arduino.h>
#include <QTRSensors.h>
#include <vector>
#define SensorCount 8

typedef class LINE{
    public:
        LINE (std::vector<uint16_t> pins) : LMspeed(0), RMspeed(0){
            uint16_t cnt = 0;
            for(auto i : pins){
                sensorValues[cnt] = (const uint16_t)i;
                cnt++;
            }
        }
        void init();
        void PIDcontrol(const double &KP, const double &KD, const double &v0);
        void print();
        double getLspd();
        double getRspd();
        ~LINE(){free(sensorValues);}
    private:
        QTRSensors qtr;
        uint16_t sensorValues[SensorCount];
        double LMspeed, RMspeed;
}Line;

#endif