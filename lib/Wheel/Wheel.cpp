#include "Wheel.h"
#include "Line.h"
/*
void IRAM_ATTR handlePulse() {
  if( digitalRead(fb_pin) ){
    timerhigh = micros();
  }
  else{
    period = micros() - timerlow;
    timerlow = micros();
    dutycycle = ( timerlow - timerhigh ) / period;
  }
}*/

// @brief initialize
Wheel::Wheel(uint8_t control, uint8_t feedback){
    ctrl_pin=control;
    fb_pin=feedback;
    pinMode(ctrl_pin, OUTPUT);
    pinMode(fb_pin, INPUT);
    // attachInterrupt(digitalPinToInterrupt(fb_pin), handlePulse, CHANGE);

}

// @brief rotate by given speed
void Wheel::control(int rpm){
    // no load, input voltage=6.3
    if(rpm==0) mu_s=1500;
    else if(rpm>0){
        if(rpm>140) mu_s=1280;
        else mu_s=map(rpm, 0, 140, 1480, 1280);
    }
    else{
        if(rpm<-140) mu_s=1720;
        else mu_s=map(rpm, 0, -140, 1520, 1720);
    }
    time_t time1 = micros();
    
        digitalWrite(ctrl_pin, HIGH);
        delayMicroseconds(mu_s);
        digitalWrite(ctrl_pin, LOW);
        delayMicroseconds(20000-mu_s);
    
}

Wheel& Wheel::operator=(const Wheel& other) {
    if (this == &other) {
        return *this; 
    }

    
    this->ctrl_pin = other.ctrl_pin;
    this->fb_pin = other.fb_pin;
    this->mu_s = other.mu_s;
    this->if_theta = other.if_theta;
    this->turns = other.turns;
    this->duty_cycle = other.duty_cycle;
    this->theta = other.theta;
    this->angle = other.angle;
    this->pre_theta = other.pre_theta;
    this->t_high = other.t_high;
    this->t_low = other.t_low;
    this->k = other.k;

    return *this;
}


// @brief rotate to given angle by given speed
// void Wheel::control(int rpm, int theta){
//     // no load, input voltage=6.3
//     if(rpm==0) mu_s=1500;
//     else if(rpm>0){
//         if(rpm>140) mu_s=1280;
//         else mu_s=map(rpm, 1480, 1280, 0, 140);
//     }
//     else{
//         if(rpm<-140) mu_s=1720;
//         else mu_s=map(rpm, 0, -140, 1520, 1720);
//     }

//     digitalWrite(ctrl_pin, HIGH);
//     delayMicroseconds(mu_s);
//     digitalWrite(ctrl_pin, LOW);
//     delayMicroseconds(20000-mu_s);
//     feedback();
//     if(abs(theta-getTheta())<5) if_theta=true;
// }

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

// @brief read duty cycle to compute angle
void Wheel::feedback(){
    // while(1){
        t_high=pulseIn(fb_pin, HIGH);
        t_low=pulseIn(fb_pin, LOW);
        // Serial.println(k++);
        // if(t_high!=0&&t_low!=0) break;
    // }
    duty_cycle=(float)DUTY_SCALE*t_high/(float)(t_high+t_low);
    // Serial.print("duty cycle: ");
    // Serial.println(/*duty_scale*/t_low/*(t_high+t_low)*/);
    theta=(UNITS_FC-1)-(float)((duty_cycle-MIN_DC)*UNITS_FC)/(MAX_DC-MIN_DC+1);
    
    if(theta<0) theta=0;
    else if(theta>(UNITS_FC-1)) theta=UNITS_FC-1;

    if((theta<Q1_MAX)&&(pre_theta>Q4_MIN)) turns++;
    else if((theta>Q4_MIN)&&(pre_theta<Q1_MAX)) turns--;

    if(turns>=0) angle=(turns*UNITS_FC)+theta;
    else if(turns<0) angle=((turns+1)*UNITS_FC)-(UNITS_FC-theta);

    pre_theta=theta;
    // Serial.print("theta: ");
    // Serial.println(theta);
}