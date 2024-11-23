#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>

#define UNITS_FC 360
#define DUTY_SCALE 1000
#define MAX_DC 971
#define MIN_DC 29
#define Q1_MAX 90
#define Q4_MIN 270

typedef class Wheel{
    public:
    Wheel(uint8_t, uint8_t);
    void control(int);
    void control(int, int);
    void ifThetaVal(bool);
    int getTheta();
    int getAngle();
    bool ifTheta();
    void feedback();

    private:
    // void feedback();

    uint8_t ctrl_pin=0;
    uint8_t fb_pin=0;

    int mu_s=1500;
    bool if_theta=false;
    
    const float duty_scale=1000.00;
    int turns=0;
    float duty_cycle=0;
    int theta=0;
    int angle=0;
    int pre_theta=0;
    unsigned long t_high;
    unsigned long t_low;

    int k=0;

}Wheel;

#endif