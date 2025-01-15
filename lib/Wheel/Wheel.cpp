#include "Wheel.h"
// #include "Line.h"

// @brief initialize
Wheel::Wheel(uint8_t control, uint8_t feedback):
    ctrl_pin(control), fb_pin(feedback){}

uint8_t Wheel::EXTIpin(){
    return digitalPinToInterrupt(fb_pin);
}

hw_timer_t* Wheel::getTim(){
    return timer;
}

Wheel_Info Wheel::getCarInfo(){
    return info;
}

void Wheel::init(uint8_t tim){
    pinMode(ctrl_pin, OUTPUT);
    pinMode(fb_pin, INPUT);
    timer=timerBegin(tim, 80, true);// timer num, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
    timerAlarmEnable(timer);
}

// @brief rotate by given speed
void Wheel::control(int rpm){
    // no load, input voltage=6.3

    if(rpm==0) ctrl_high=1500;
    else if(rpm>0){
        if(rpm>140) ctrl_high=1280;
        else ctrl_high=1480-rpm*(1480-1280)/140;
    }
    else{
        if(rpm<-140) ctrl_high=1720;
        else ctrl_high=1520+rpm*(1720-1520)/140;
    }
    ctrl_low=20000-ctrl_high;
    cpin_state=LOW;
    if(mu_s!=ctrl_high){
        timerAlarmWrite(timer, ctrl_high, true);
        mu_s=ctrl_high;
    }
}

// @brief rotate to given angle by given speed
void Wheel::thetaControl(int rpm, int theta){
    // no load, input voltage=6.3

    if(angle>=theta) return;

    if(rpm==0) ctrl_high=1500;
    else if(rpm>0){
        if(rpm>140) ctrl_high=1280;
        else ctrl_high=map(rpm, 1480, 1280, 0, 140);
    }
    else{
        if(rpm<-140) ctrl_high=1720;
        else ctrl_high=map(rpm, 0, -140, 1520, 1720);
    }
    ctrl_low=20000-ctrl_high;
    cpin_state=LOW;
    if(mu_s!=ctrl_high){
        timerAlarmWrite(timer, ctrl_high, true);
        mu_s=ctrl_high;
    }

    if(handlePulse()){
        feedback();
    }
}

bool Wheel::handlePulse(){
    portENTER_CRITICAL_ISR(&timer_mux);

    if(digitalRead(fb_pin)){
        period=int(micros()-fb_rising);
        fb_high=int(fb_falling-fb_rising);
        fb_rising=micros();
        return true;
    }
    else{
        fb_falling=micros();
        return false;
    }

    portEXIT_CRITICAL_ISR(&timer_mux);
}

void Wheel::sendPulse(){
    portENTER_CRITICAL_ISR(&timer_mux);

    cpin_state=!cpin_state;
    digitalWrite(ctrl_pin, cpin_state);
    timerAlarmWrite(timer, cpin_state?ctrl_high: ctrl_low, true);
    timerAlarmEnable(timer);

    portEXIT_CRITICAL_ISR(&timer_mux);
}

// @brief read duty cycle to compute angle
void Wheel::feedback(){
    duty_cycle=float(fb_high)/float(period);
    theta=(float)((duty_cycle*duty_scale-MIN_DC)*UNITS_FC)/(MAX_DC-MIN_DC+1);
    
    if(theta<0) theta=0;
    else if(theta>(UNITS_FC-1)) theta=UNITS_FC-1;

    if((theta<Q1_MAX)&&(pre_theta>Q4_MIN)) turns++;
    else if((theta>Q4_MIN)&&(pre_theta<Q1_MAX)) turns--;

    if(turns>=0) angle=(turns*UNITS_FC)+theta;
    else if(turns<0) angle=((turns+1)*UNITS_FC)-(UNITS_FC-theta);

    pre_theta=theta;
    dt=int(micros()-info.stamp);
    info.stamp=micros();
    info.linear_vel=WEL_RADIUS*(theta-pre_theta)/dt;
}