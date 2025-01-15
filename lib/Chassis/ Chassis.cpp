#include "Chassis.h"

Chassis::Chassis(Wheel left_wheel_, Wheel right_wheel_)
    :left_wheel(left_wheel_), right_wheel(right_wheel_){}

void Chassis::forwardKinematics(){
    v_x=(left_wheel.getCarInfo().linear_vel+right_wheel.getCarInfo().linear_vel)*cos(theta)*0.5;
    v_y=(left_wheel.getCarInfo().linear_vel+right_wheel.getCarInfo().linear_vel)*sin(theta)*0.5;
    omega=(-left_wheel.getCarInfo().linear_vel+right_wheel.getCarInfo().linear_vel)*0.5/CAR_RADIUS;
}

void Chassis::updatePose(){
    now=max(left_wheel.getCarInfo().stamp, right_wheel.getCarInfo().stamp);
    dt=now-last;
    x+=v_x*dt;
    y+=v_y*dt;
    theta+=omega*dt;
}