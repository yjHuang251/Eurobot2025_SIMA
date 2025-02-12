#include "Chassis.h"

Chassis::Chassis(Wheel* left_wheel_, Wheel* right_wheel_)
    :left_wheel(left_wheel_), right_wheel(right_wheel_)
    , x(0.0), y(0.0), theta(0.0), v_x(0.0), omega(0.0){}

void Chassis::forwardKinematics(){
    v_x=((-1)*left_wheel->getWelInfo().linear_vel+right_wheel->getWelInfo().linear_vel)*0.5;
    omega=(left_wheel->getWelInfo().linear_vel+right_wheel->getWelInfo().linear_vel)*0.5/car_radius;
}

void Chassis::updatePose(){
    now=max(left_wheel->getWelInfo().stamp, right_wheel->getWelInfo().stamp);
    dt=float(int(now-last));

    theta+=omega*dt/1000000.00;
    if(theta<=0.001) theta=0;
    x+=v_x*cos(theta)*dt/1000000.00;
    y+=v_x*sin(theta)*dt/1000000.00;

    last=now;
}

float Chassis::getX(){
    return x;
}

float Chassis::getY(){
    return y;
}

float Chassis::getTheta(){
    return theta;
}
