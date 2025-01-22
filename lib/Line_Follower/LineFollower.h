#include <Arduino.h>
#include <QTRSensors.h>

extern QTRSensors qtr;
#define NUM_SENSORS 8



typedef class LineFollower{

    public:
    enum LineStatus{
        motorSpeedinfo,
        Linedata,
        PIDinfo,
        Custom
    };
    enum StopMode{
        cross,
        t_shape,
        superstar
    };
    LineFollower(uint8_t , uint8_t , uint8_t, uint8_t , uint8_t , uint8_t, uint8_t , uint8_t);
    void PrintLineStatus(LineStatus);
    void PID(double , double , int , int duration = 6000,StopMode = cross);
    void stop();
    double getLMSpeed(){return -1*LMSpeed;}
    double getRMSpeed(){return RMSpeed;}
    
    private:
    uint16_t  sensorValues[NUM_SENSORS];
    double LMSpeed;
    double RMSpeed;
    bool stop_cross;
    bool stop_t_shape;
    bool stop_superstar;
    int cntrightblack = 0;
    const int stop_cross_duration = 8300;
    const int stop_t_shape_duration = 9500;
  



}LineFollower;