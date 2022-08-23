/*
  Pins -- Arduino Nano (ble, every, normal)
  DIR1 8
  PWM1 10
  
  DIR2 12
  PWM2 11
*/

#include "mbed.h"

#define PWM_PIN_1      9
#define PWM_PIN_2      10
#define PWM_FREQUENCY  20000

mbed::PwmOut pwmPin1(digitalPinToPinName(PWM_PIN_1));
mbed::PwmOut pwmPin2(digitalPinToPinName(PWM_PIN_2));

void setup()
{
  pwmPin1.period(1.0 / PWM_FREQUENCY);
  pwmPin2.period(1.0 / PWM_FREQUENCY);
  
  pinMode(8,OUTPUT);    //dir1
  pinMode(11,OUTPUT);   //dir2
  
  pinMode(9,OUTPUT);   //pwm1
  pinMode(10,OUTPUT);   //pwm2

  pinMode (A0, INPUT);
  Serial.begin(9600);
}


void loop()
{
  //Serial.println(analogRead(A0));
  //int a = analogRead(A0);
  //float dutyCycle = map(a,0,1024,0,100);
  
  float dutyCycle = 8;
  Serial.println(dutyCycle);
  Serial1.println(dutyCycle);
 
  pwmPin1.write(dutyCycle/100);
  pwmPin2.write(dutyCycle/100);
  digitalWrite(8,1);
  digitalWrite(11,0);
}
