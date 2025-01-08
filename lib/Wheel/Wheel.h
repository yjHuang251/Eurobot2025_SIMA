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

class Wheel{
    public:
    Wheel(uint8_t, uint8_t, int);
    uint8_t EXTIpin();
    void control(int);
    void thetaControl(int, int);
    void handlePulse();
    static void IRAM_ATTR timCallback();
    
    void feedback();

    private:
    // static Wheel* instance[2];
    // static int instance_cnt;

    static uint8_t ctrl_pin;
    static uint8_t fb_pin;

    int mu_s=1500;
    static bool cpin_state;
    static volatile int ctrl_high;
    static volatile int ctrl_low;
    static hw_timer_t *timer;
    static portMUX_TYPE timer_mux;

    volatile unsigned long fb_falling=0;
    volatile unsigned long fb_rising=0;
    volatile int fb_high=0;
    volatile int period=0;
    volatile float duty_cycle=0.0;
    volatile bool if_data=false;
    
    const float duty_scale=1000.00;
    int turns=0;
    int theta=0;
    int angle=0;
    int pre_theta=0;
    
};

#endif