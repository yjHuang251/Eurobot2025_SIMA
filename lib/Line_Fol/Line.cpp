#include "Line.h"

Line::Line(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4){
  pins[0]=p0;
  pins[1]=p1;
  pins[2]=p2;
  pins[3]=p3;
  pins[4]=p4;
  for(int i=0;i<5;i++) {
    pinMode(pins[i], INPUT);
  }
}

void Line::init(){
  for(int i=0;i<5;i++) {
    val[i]=digitalRead(pins[i]);
  }
}

void Line::PIDcontrol(){
  for(int i=0;i<5;i++){
    val[i]=digitalRead(pins[i]);
  } 
  l_spd = 1;
  r_spd = 1;
  
  if(val[0]&&!val[1]) r_spd += 2;
  else if((val[0]&&val[1])||(!val[0]&&val[1])) r_spd += 1;
  if((val[3]&&!val[4])||(val[3]&&val[4])) l_spd += 1;
  else if(!val[3]&&val[4]) l_spd += 2;
}

int Line::getLspd(){
  return l_spd;
}

int Line::getRspd(){
  return r_spd;
}
