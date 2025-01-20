#ifndef ESCAPE_H
#define ESCAPE_H

#include <Arduino.h>
#include "Wheel.h"
#include "VL53L0X.h"

class Escape {
public:
    Escape(Wheel &L_wheel, Wheel &R_wheel, VL53L0X sensor_M, VL53L0X sensor_R, VL53L0X sensor_L);
    bool check(VL53L0X sensor_M);
    void escape_quarter(double r);
    void ReverseAndRotate();

private:
    static int rotate_time;
    static double vel_const;
    static const double car_width;
    const int v_0;

    Wheel L_wheel;
    Wheel R_wheel;
    VL53L0X sensor_M;
    VL53L0X sensor_R;
    VL53L0X sensor_L;
};

#endif
