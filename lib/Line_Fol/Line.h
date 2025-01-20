
#include <Arduino.h>

#define K_P 10
#define K_D 0.1

typedef class Line{
    public:
    Line(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    void init();
    void PIDcontrol();
    int getLspd();
    int getRspd();
    int val[5]={0};
    bool stop = false;
    int lastblack;
    int last_lastblack;
    
    private:
    int l_spd=0;
    int r_spd=0;
    int error_now=0;
    int error_last=0;
    int d_error=0;
    int threshold[5]={400, 400, 400, 400, 400};
    uint8_t pins[5]={0};
    
}Line;