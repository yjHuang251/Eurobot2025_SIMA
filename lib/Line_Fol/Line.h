#ifndef LINE_H
#define LINE_H

#include <Arduino.h>

typedef class Line{
    public:
    Line(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    void init();
    void PIDcontrol();
    int getLspd();
    int getRspd();
   
    
    private:
    int l_spd=0;
    int r_spd=0;
    uint8_t pins[5]={0};
    bool val[5]={0};
    
}Line;

#endif