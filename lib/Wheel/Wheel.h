#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>
#include <iostream>

#define UNITS_FC 360
#define DUTY_SCALE 1000
#define MAX_DC 971
#define MIN_DC 29
#define Q1_MAX 0.25*UNITS_FC
#define Q4_MIN 0.75*UNITS_FC

enum TIM_NUMBER{
    TIM_NUM_L=0,
    TIM_NUM_R=2
};

typedef struct Wheel_Info{
    unsigned long stamp=0; // mu_s
    float linear_vel=0.0; // mm/s
}Wheel_Info;

class Wheel{
    public:
    Wheel(uint8_t, uint8_t, int);
    uint8_t EXTIpin();
    hw_timer_t* getTim();
    Wheel_Info getCarInfo();
    bool getIfData();
    void control(int, int);
    void thetaControl(int, int);
    void handlePulse();
    void sendPulse();
    void feedback();

    bool deg_ok=false;

    float theta=0.0; // [0, 360), deg
    int angle=0; // accumulated, deg
    float prev_theta=0.0; // deg
    int dt=0; // mu_s
    Wheel_Info info;
    float d_theta=0.0;

    private:
    // variables for setup
    uint8_t ctrl_pin;
    uint8_t fb_pin;
    hw_timer_t *timer=NULL;

    portMUX_TYPE timer_mux=portMUX_INITIALIZER_UNLOCKED;

    // variables for control
    int mu_s=1500;
    volatile bool cpin_state;
    int ctrl_high=0;
    int ctrl_low=0;
    int dir=0;
    bool if_deg=false;

    // variables for feedback interrupt
    volatile unsigned long fb_falling=0;
    volatile unsigned long fb_rising=0;
    volatile int fb_high=0;
    volatile int period=0;
    bool if_data=false;
    int spe_deg=0;
    
    // variables for feedback
    float duty_cycle=0.0;
    int turns=0;
    // float theta=0.0; // [0, 360), deg
    // int angle=0; // accumulated, deg
    // int prev_theta=0; // deg
    // int dt=0; // mu_s
    // Wheel_Info info;
    // float d_theta=0.0;
    const float duty_scale=1000.0;
    const float wel_radius=23.75; // mm
    const float pi=3.14159265358979323846;
    
};

#endif