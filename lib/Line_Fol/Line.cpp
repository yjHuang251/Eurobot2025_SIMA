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
  for(int i=0;i<5;i++) {
    val[i]=analogRead(pins[i]);
  }
  divide_line=((val[0]+val[2]+val[4])/3+(val[1]+val[3])/2)/2;
}

void Line::PIDcontrol(){
  for(int i=0;i<5;i++){
    val[i]=analogRead(pins[i]);
    val[i]=(val[i]>divide_line)?0:1;
  }

  if(val[0]==0 && val[1]==1 && val[2]==0 && val[3]==1 && val[4]==0){
    error_now = 0;
   }
  else if(val[0]==1 && val[1]==0 && val[2]==1 && val[3]==0 && val[4]==0){
    error_now = 1;
    d_error = error_now - error_last;
    l_spd = K_P * error_now - K_D * d_error;
  }
  else if(val[0]==0 && val[1]==1 && val[2]==0 && val[3]==0 && val[4]==0){
    error_now = 2;
    d_error = error_now - error_last;
    l_spd = K_P * error_now - K_D * d_error;
  }
  else if(val[0]==1 && val[1]==0 && val[2]==0 && val[3]==0 && val[4]==0){
    error_now = 3;
    d_error = error_now - error_last;
    l_spd = K_P * error_now - K_D * d_error;
  }
  else if(val[0]==0 && val[1]==0 && val[2]==1 && val[3]==0 && val[4]== 1){
    error_now = -1;
    d_error = error_now - error_last;
    r_spd = -K_P * error_now + K_D * d_error;
  }
  else if(val[0]==0 && val[1]==0 && val[2]==0 && val[3]==1 && val[4]==0){
    error_now = -2;
    d_error = error_now - error_last;
    r_spd = -K_P * error_now + K_D * d_error;
  }
  else if(val[0]==0 && val[1]==0 && val[2]==0 && val[3]==0 && val[4]==1){
    error_now = -3;
    d_error = error_now - error_last;
    r_spd = -K_P * error_now + K_D * d_error;
  }
  error_last=error_now;
}

int Line::getLspd(){
  return l_spd;
}

int Line::getRspd(){
  return r_spd;
}

/*
int r_spd = 0;
int l_spd = 0;

float K_P = 0.5;
float K_D = 0.1;
int error_now = 0;
int error_last = 0;


int IN1 = 8;        //L298N IN1 連接 Arduino pin 10
int IN2 = 9;        //L298N IN2 連接 Arduino pin 11
int IN3 = 10;         //L298N IN3 連接 Arduino pin 6
int IN4 = 12;         //L298N IN4 連接 Arduino pin 5

int Sensor1 = 1;
int Sensor2 = 2;
int Sensor3 = 3;
int Sensor4 = 4;
int Sensor5 = 5;

//從左至右
int S1;
int S2;
int S3;
int S4;
int S5;

int divide_line = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  

  pinMode(1,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);

  Stop();
  delay(500);
}*/

/*
void loop() {
  // put your main code here, to run repeatedly:
  S1 = analogRead(1);
  S2 = analogRead(2);
  S3 = analogRead(3);
  S4 = analogRead(4);
  S5 = analogRead(5);
  divide_line = ((S1+ S3 + S5) / 3 + (S2 + S4) / 2) / 2;//這邊一開始定義一遍就行
  while(1){
    S1 = analogRead(1);
    S2 = analogRead(2);
    S3 = analogRead(3);
    S4 = analogRead(4);
    S5 = analogRead(5);

    error_last = error_now;
    error_now = 0;
    d_error = 0;

    if(S1 > divide_line){
      S1 = 0;
    }
    else{
      S1 = 1;
    }
    if(S2 > divide_line){
      S2 = 0;
    }
    else{
      S2 = 1;
    }
    if(S3 > divide_line){
      S3 = 0;
    }
    else{
      S3 = 1;
    }
    if(S4 > divide_line){
      S4 = 0;
    }
    else{
      S4 = 1;
    }
    if(S5 > divide_line){
      S5 = 0;
    }
    else{
      S5 = 1;
    }

    
    if(S1==0 && S2==1 && S3==0 && S4==1 && S5==0){
      error_now = 0;
    }
    else if(S1==1 && S2==0 && S3==1 && S4==0 && S5==0){
      error_now = 1;
      d_error = error_now - error_last;
      l_spd = K_P * error_now - K_D * d_error;
    }
    else if(S1==0 && S2==1 && S3==0 && S4==0 && S5==0){
      error_now = 2;
      d_error = error_now - error_last;
      l_spd = K_P * error_now - K_D * d_error;
    }
    else if(S1==1 && S2==0 && S3==0 && S4==0 && S5==0){
      error_now = 3;
      d_error = error_now - error_last;
      l_spd = K_P * error_now - K_D * d_error;
    }
    else if(S1==0 && S2==0 && S3==1 && S4==0 && S5==1){
      error_now = -1;
      d_error = error_now - error_last;
      r_spd = -K_P * error_now + K_D * d_error;
    }
    else if(S1==0 && S2==0 && S3==0 && S4==1 && S5==0){
      error_now = -2;
      d_error = error_now - error_last;
      r_spd = -K_P * error_now + K_D * d_error;
    }
    else if(S1==0 && S2==0 && S3==0 && S4==0 && S5==1){
      error_now = -3;
      d_error = error_now - error_last;
      r_spd = -K_P * error_now + K_D * d_error;
    }
  

  }
  

}*/