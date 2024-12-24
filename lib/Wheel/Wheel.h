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
    Wheel(uint8_t, uint8_t, int);
    void control(int);
    void thetaControl(int, int);
    void ifThetaVal(bool);
    int getTheta();
    int getAngle();
    bool ifTheta();
    static void IRAM_ATTR handlePulse();
    static void IRAM_ATTR timCallback();

    private:
    void feedback();

    static uint8_t ctrl_pin;
    static uint8_t fb_pin;

    int mu_s=1500; //
    bool if_theta=false;
    static bool cpin_state;
    static int ctrl_high;
    static int ctrl_low;
    static hw_timer_t *timer;
    
    const float duty_scale=1000.00;
    int turns=0;
    static float duty_cycle;
    int theta=0;
    int angle=0;
    int pre_theta=0;
    static int period;
    static unsigned long fb_high;
    static unsigned long fb_low;
    
}Wheel;

#endif