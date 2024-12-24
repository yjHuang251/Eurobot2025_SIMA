#include "Wheel.h"
#include "Line.h"

uint8_t Wheel::ctrl_pin=0;
uint8_t Wheel::fb_pin=0;

int Wheel::ctrl_high=1500;
int Wheel::ctrl_low=20000-ctrl_high;
bool Wheel::cpin_state=LOW;
hw_timer_t* Wheel::timer=NULL;

float Wheel::duty_cycle=0;
int Wheel::period=0;
unsigned long Wheel::fb_high=0;
unsigned long Wheel::fb_low=0;

// @brief initialize
Wheel::Wheel(uint8_t control, uint8_t feedback, int tim){
    ctrl_pin=control;
    fb_pin=feedback;
    pinMode(ctrl_pin, OUTPUT);
    pinMode(fb_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(fb_pin), handlePulse, CHANGE);
    timer=timerBegin(tim, 80, true);// timer num, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
    timerAttachInterrupt(timer, &timCallback, true);
    timerAlarmEnable(timer);
}

// @brief rotate by given speed
void Wheel::control(int rpm){
    // no load, input voltage=6.3
    /* --- first version: use delayMicroseocnds --- */
    /*
    if(rpm==0) mu_s=1500;
    else if(rpm>0){
        if(rpm>140) mu_s=1280;
        else mu_s=map(rpm, 0, 140, 1480, 1280);
    }
    else{
        if(rpm<-140) mu_s=1720;
        else mu_s=map(rpm, 0, -140, 1520, 1720);
    }

    digitalWrite(ctrl_pin, HIGH);
    delayMicroseconds(mu_s);
    digitalWrite(ctrl_pin, LOW);
    delayMicroseconds(20000-mu_s);*/

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
    timerAlarmWrite(timer, ctrl_high, true);
}

// @brief rotate to given angle by given speed
void Wheel::thetaControl(int rpm, int theta){
    // no load, input voltage=6.3
    /* --- first version: use delayMicroseocnds --- */
    /*
    if(rpm==0) mu_s=1500;
    else if(rpm>0){
        if(rpm>140) mu_s=1280;
        else mu_s=map(rpm, 1480, 1280, 0, 140);
    }
    else{
        if(rpm<-140) mu_s=1720;
        else mu_s=map(rpm, 0, -140, 1520, 1720);
    }

    digitalWrite(ctrl_pin, HIGH);
    delayMicroseconds(mu_s);
    digitalWrite(ctrl_pin, LOW);
    delayMicroseconds(20000-mu_s);
    feedback();
    if(abs(theta-getTheta())<5) if_theta=true;*/

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
    timerAlarmWrite(timer, ctrl_high, true);
}

void Wheel::ifThetaVal(bool state){
    if_theta=state;
}

int Wheel::getTheta(){
    return theta;
}

int Wheel::getAngle(){
    return angle;
}

bool Wheel::ifTheta(){
    return if_theta;
}

void IRAM_ATTR Wheel::handlePulse() {
  if(!digitalRead(fb_pin)){
    fb_high=micros();
  }
  else{
    period=int(micros()-fb_low);
    fb_low=micros();
    duty_cycle=(fb_high-fb_low)/period;
  }
}

void IRAM_ATTR Wheel::timCallback(){
    if(cpin_state==HIGH){
        cpin_state=!cpin_state;
        digitalWrite(ctrl_pin, cpin_state);
        timerAlarmWrite(timer, ctrl_low, true);
    }
    else{
        cpin_state=!cpin_state;
        digitalWrite(ctrl_pin, cpin_state);
        timerAlarmWrite(timer, ctrl_high, true);
    }
}

// @brief read duty cycle to compute angle
void Wheel::feedback(){
    theta=(UNITS_FC-1)-(float)((duty_cycle-MIN_DC)*UNITS_FC)/(MAX_DC-MIN_DC+1);
    
    if(theta<0) theta=0;
    else if(theta>(UNITS_FC-1)) theta=UNITS_FC-1;

    if((theta<Q1_MAX)&&(pre_theta>Q4_MIN)) turns++;
    else if((theta>Q4_MIN)&&(pre_theta<Q1_MAX)) turns--;

    if(turns>=0) angle=(turns*UNITS_FC)+theta;
    else if(turns<0) angle=((turns+1)*UNITS_FC)-(UNITS_FC-theta);

    pre_theta=theta;
}