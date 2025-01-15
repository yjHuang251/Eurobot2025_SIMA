#ifndef CHASSIS_H
#define CHASSIS_H

#include "Arduino.h"
#include "Wheel.h"
#include "math.h"

#define CAR_RADIUS 100

class Chassis{
    public:
    Chassis(Wheel, Wheel);
    void forwardKinematics();
    void updatePose();

    private:
    Wheel left_wheel;
    Wheel right_wheel;

    float v_x=0.0;
    float v_y=0.0;
    float omega=0.0;
    float x=0.0;
    float y=0.0;
    float theta=0.0;

    time_t now=0;
    time_t last=0;
    int dt=0;
};

#endif