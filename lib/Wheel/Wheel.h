#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>
#include <iostream>

#define UNITS_FC 360
#define DUTY_SCALE 1000
#define MAX_DC 971
#define MIN_DC 29
#define Q1_MAX 90
#define Q4_MIN 270
#define WEL_RADIUS 30

enum TIM_NUMBER{
    TIM_NUM_L=0,
    TIM_NUM_R=2
};

typedef struct Wheel_Info{
    time_t stamp;
    float linear_vel;
}Wheel_Info;

class Wheel{
    public:
    Wheel(uint8_t, uint8_t);
    uint8_t EXTIpin();
    hw_timer_t* getTim();
    Wheel_Info getCarInfo();
    void init(uint8_t);
    void control(int);
    void thetaControl(int, int);
    bool handlePulse();
    void sendPulse();
    void feedback();

    private:
    uint8_t ctrl_pin;
    uint8_t fb_pin;

    int mu_s=1500;
    volatile bool cpin_state;
    volatile int ctrl_high=0;
    volatile int ctrl_low=0;
    hw_timer_t *timer=NULL;
    portMUX_TYPE timer_mux=portMUX_INITIALIZER_UNLOCKED;

    volatile unsigned long fb_falling=0;
    volatile unsigned long fb_rising=0;
    volatile int fb_high=0;
    volatile int period=0;
    volatile float duty_cycle=0.0;
    
    const float duty_scale=1000.00;
    int turns=0;
    float theta=0.0; // [0,2*pi)
    int angle=0; // accumulated
    int pre_theta=0;
    int dt=0;
    Wheel_Info info;
    
};

#endif