#include "Escape.h"

Escape::Escape(Wheel &L_wheel, Wheel &R_wheel, VL53L0X sensor_M, VL53L0X sensor_R, VL53L0X sensor_L)
    : L_wheel(L_wheel), R_wheel(R_wheel), sensor_M(sensor_M), sensor_R(sensor_R), sensor_L(sensor_L) , v_0(10) {}

bool Escape::check(VL53L0X sensor_M) {
    if (sensor_M.readRangeSingleMillimeters() <= 150 && sensor_R.readRangeSingleMillimeters() <= 150 && sensor_L.readRangeSingleMillimeters() <= 150) {
        ReverseAndRotate();
        escape_quarter(0.6);
        return true;
    }else if(sensor_M.readRangeSingleMillimeters() <= 150 && sensor_R.readRangeSingleMillimeters() <= 150){
        ReverseAndRotate();
        escape_quarter(0.4);
        return true;
    }else if(sensor_M.readRangeSingleMillimeters() <= 150 && sensor_L.readRangeSingleMillimeters() <= 150){
        ReverseAndRotate();
        escape_quarter(0.4);
        return true;
    }else{
        ReverseAndRotate();
        escape_quarter(0.2);
        return true;
    }
    return false;
}

void Escape::escape_quarter(double r) {
    for (int i = 0; i <= r * vel_const / v_0 * 200; i++) {
        if (check(sensor_M)) {
            return;
        }
        L_wheel.control(-1 * (r - car_width) * v_0);
        R_wheel.control((r + car_width) * v_0);
        Serial.println("quarter");
    }
}

void Escape::ReverseAndRotate() {
    for (int i = 0; i <= 100 / 4 / 2; i++) {
        L_wheel.control(-v_0);
        R_wheel.control(v_0);
        Serial.println(i);
    }
    for (int i = 0; i <= rotate_time / 2; i++) {
        L_wheel.control(-v_0);
        R_wheel.control(-v_0);
        Serial.println("rotate");
    }
}

int Escape::rotate_time = 40;
double Escape::vel_const = 20;
const double Escape::car_width = 0.2;
