#ifndef CHASSIS_H
#define CHASSIS_H

#include "Arduino.h"
#include "Wheel.h"
#include "math.h"

class Chassis{
    public:
    Chassis(Wheel*, Wheel*);
    void forwardKinematics();
    void updatePose();
    float getX();
    float getY();
    float getTheta();

    private:
    Wheel* left_wheel;
    Wheel* right_wheel;
    float v_x=0.0; // mm/s
    float omega=0.0; // rad/s
    float x=0.0; // mm
    float y=0.0; // mm
    float theta=0.0; // rad
    unsigned long now=0; // mu_s
    unsigned long last=0;
    int dt=0;
    const float car_radius=45.5; // mm
    
};

#endif