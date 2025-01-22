#include <Arduino.h>
#include "Wheel.h"
#include "LineFollower.h"

// This example is designed for use with six analog QTR sensors. These
// reflectance sensors should be connected to analog pins A0 to A5. The
// sensors' emitter control pin (CTRL or LEDON) can optionally be connected to
// digital pin 2, or you can leave it disconnected and remove the call to
// setEmitterPin().
//
// The main loop of the example reads the raw sensor values (uncalibrated). You
// can test this by taping a piece of 3/4" black electrical tape to a piece of
// white paper and sliding the sensor across it. It prints the sensor values to
// the serial monitor as numbers from 0 (maximum reflectance) to 1023 (minimum
// reflectance).

LineFollower lineFollower(25, 32, 33, 18, 19, 27, 14, 12);
Wheel L_wheel(2,15);
Wheel R_wheel(23,22);
LineFollower::StopMode stop_mode = LineFollower::StopMode::cross;

const int v_0 = 45;
const int KP = 2;
const int KD = 1;

int starttime;
int Duration = 0;

void setup()
{
  Serial.begin(9600);
  starttime = millis();
  //LineFollower lineFollower(35, 32, 33, 25, 26, 27, 14, 12);
  L_wheel.control(0);
  R_wheel.control(0);
  delay(2000);
}


void loop()
{
  //Serial.println("Sensor Values: ");
  //lineFollower.PrintLineStatus();
  Duration = millis() - starttime;
  //Serial.println(Duration);
  lineFollower.PID(KP, KD, v_0 , Duration, stop_mode);
  lineFollower.PrintLineStatus(LineFollower::LineStatus::Linedata);
  L_wheel.control(lineFollower.getLMSpeed());
  R_wheel.control(lineFollower.getRMSpeed());
  delay(20);
}