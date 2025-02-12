#include "Wheel.h"
// #include "Line.h"

// @brief initialize
Wheel::Wheel(uint8_t control, uint8_t feedback, int tim):
    ctrl_pin(control), fb_pin(feedback){
        pinMode(ctrl_pin, OUTPUT);
        pinMode(fb_pin, INPUT);
        timer=timerBegin(tim, 80, true);// timer num, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
        timerAlarmEnable(timer);
    }

uint8_t Wheel::EXTIpin(){
    return digitalPinToInterrupt(fb_pin);
}

hw_timer_t* Wheel::getTim(){
    return timer;
}

Wheel_Info Wheel::getWelInfo(){
    return info;
}

bool Wheel::getIfData(){
    return if_data;
}

// @brief rotate by given speed
void Wheel::control(int rpm, int direction){
    // no load, input voltage=6.3
    dir=direction;
    if(rpm==0) ctrl_high=1500;
    else {
        if(dir==1){
            if(rpm>=140) ctrl_high=1280;
            else ctrl_high=1480-rpm*float(1480-1280)/140.0;
        }
        else if(dir==-1){
            if(rpm>=140) ctrl_high=1720;
            else ctrl_high=1525+abs(rpm)*float(1720-1525)/140.0;
        }
    }
    ctrl_low=20000-ctrl_high;
    cpin_state=LOW;
    if(mu_s!=ctrl_high){
        timerAlarmWrite(timer, ctrl_high, true);
        mu_s=ctrl_high;
    }
}


/*
 *   ___
 *  |   |          |
 *  |   |__________|
 *      ^false     ^true
 */
// @brief called in pin change interrupt cllback, detect rising/falling edge
void Wheel::handlePulse(){
    portENTER_CRITICAL_ISR(&timer_mux);

    if(digitalRead(fb_pin)){
        period=int(micros()-fb_rising);
        fb_high=int(fb_falling-fb_rising);
        fb_rising=micros();
        if_data=true;
    }
    else{
        fb_falling=micros();
        if_data=false;
    }

    portEXIT_CRITICAL_ISR(&timer_mux);
}

// @brief called in timer callback, for sending high/low signal
void Wheel::sendPulse(){
    portENTER_CRITICAL_ISR(&timer_mux);

    cpin_state=!cpin_state;
    digitalWrite(ctrl_pin, cpin_state);
    timerAlarmWrite(timer, cpin_state?ctrl_high: ctrl_low, true);

    portEXIT_CRITICAL_ISR(&timer_mux);
}

// @brief calculate duty cycle, theta and veocity. Get time stamp
void Wheel::feedback(){
    duty_cycle=float(fb_high)/float(period);
    theta=(float)((duty_cycle*duty_scale-MIN_DC)*UNITS_FC)/(MAX_DC-MIN_DC+1);
    
    if(theta<0) theta=0;
    if(theta>(UNITS_FC-1)) theta=UNITS_FC-1;

    if((theta<Q1_MAX)&&(prev_theta>Q4_MIN)) turns++;
    else if((theta>Q4_MIN)&&(prev_theta<Q1_MAX)) turns--;

    if(turns>=0) angle=(turns*UNITS_FC)+theta;
    else if(turns<0) angle=((turns+1)*UNITS_FC)-(UNITS_FC-theta);

    // filter measurement noise
    if(min(abs(theta-prev_theta), abs(360-(theta-prev_theta)))<=1.0) {
        prev_theta=theta;
        return;
    }

    // angle limit checking 
    // check whether direction of angle difference is the same as given direction
    if(dir*(theta-prev_theta)<0){
        if(dir==1) d_theta=max(theta-prev_theta, 360+(theta-prev_theta));
        else if(dir==-1) d_theta=-min(theta-prev_theta, 360-(theta-prev_theta));
    }
    else d_theta=theta-prev_theta;

    dt=int(micros()-info.stamp);
    info.stamp=micros();

    // filter unreasonable velocity
    if(!(abs(wel_radius*d_theta*pi*1000000/(dt*180.0))-abs(info.linear_vel)>=80)) {
        info.linear_vel=wel_radius*d_theta*pi*1000000/(dt*180.0);
    }

    prev_theta=theta;
}