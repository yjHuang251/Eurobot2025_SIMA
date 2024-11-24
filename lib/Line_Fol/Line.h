
#include <Arduino.h>

#define K_P 0.5
#define K_D 0.1

typedef class Line{
    public:
    Line(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    void PIDcontrol();
    int getLspd();
    int getRspd();
    
    private:
    int l_spd=0;
    int r_spd=0;
    int error_now=0;
    int error_last=0;
    int d_error=0;
    int divide_line=0;
    int val[5]={0};
    uint8_t pins[5]={0};
    
}Line;