//Arduino Nano 33 BLE/BLE Sense Code - SmartElex motor driver,RS422 data Tx - Rx,Encoder & Temperature Sensor

#include "mbed.h" //basic mbed functions

#define PWM_PIN_1      9  //PWM1 - Motor 1
#define PWM_PIN_2      10 //PWM2 - Motor 2
#define PWM_FREQUENCY  20000 //20KHz 
//20KHz Max freq supported by SmartElex
//100KHz Max freq supported by Pololu G2

mbed::PwmOut pwmPin1(digitalPinToPinName(PWM_PIN_1)); //Pin mapping
mbed::PwmOut pwmPin2(digitalPinToPinName(PWM_PIN_2)); //Pin mapping

//Motor Driver Pins:
#define DIR1 8
#define DIR2 11

//Global Variables - Encoder:
const int encoderA = 2;
const int encoderB = 12;
int flag = 0;
bool EA, EB;

//Thermister Global Variables:
#define RT0 10000 //Ω  //Thermister Resistance at 25°C 
#define B 3977   // K //B-Value Material Constant 
#define VCC 5    //Supply voltage to Thermister
#define R 1000   //R=1KΩ, Resistance connected in series with Thermister 
float RT, VR, ln, TX, T0, VRT;

//Other Global Variables:
String buf;   //Controller Serial data
#define TIP 3 //Endobot LED pin

//Fxn to read & calculate motor Temperature:
float temp_read(int tempPin)
{
  VRT = analogRead(tempPin);         //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;                    //Voltage across resistance in series with thermister
  RT = VRT / (VR / R);               //Resistance of RT (Thermister Resistance)

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0)));  //Temperature from thermistor

  TX = TX - 273.15;                  //Conversion to Celsius
  return TX; 
}

void setup()
{
  Serial1.begin(9600); //Send & Receive Serial data to RS422 module
  Serial.begin(9600);  //for Serial monitor debug

  T0 = 25 + 273.15;  //Temperature T0 from datasheet, conversion from Celsius to kelvin
  //Ref temp of thermister, 10K resistance at 25°C

  pwmPin1.period(1.0 / PWM_FREQUENCY); //PWM time period
  pwmPin2.period(1.0 / PWM_FREQUENCY); //PWM time period 

  //Motor Driver Pins:
  pinMode(DIR1,OUTPUT); 
  pinMode(DIR2,OUTPUT); 
  
  //Other Pins:
  pinMode(encoderA,INPUT); //Encoder channel A
  pinMode(encoderB,INPUT); //Encoder channel B
  pinMode(A2,INPUT); //Temp Sensor 1 - Motor 1
  pinMode(A3,INPUT); //Temp Sensor 2 - Motor 2
  pinMode(A4,INPUT); //Temp Sensor 3 - Indoor Temp
  pinMode(TIP,OUTPUT); //Endbot LED TIP122
}


void loop() 
{ 
   float T1 = temp_read(A2); //Motor 1 Temp
   float T2 = temp_read(A3); //Motor 2 Temp
   float T3 = temp_read(A4); //Indoor Temp

   //Data Transfer Variables - Mega to Nano:
   int a = 0, b = 0, c = 0, sh = 0, sh1 = 0, sh2 = 0;
   char x[10] = "0", y[10] = "0", z[10] = "0";
   int h = 0;
   int FB, LR, LEDpot;

   //Received Data Decryption - Mega to Nano:
   if (Serial1.available()) // if any data came to the port
   {
     delay(50);  // wait a bit for the entire data packet to be accepted by the port
     buf = Serial1.readStringUntil('\n');  //Read Serial1 data until null character
     Serial.println(buf);
   }
   a = buf[0] - '0';
   b = buf[1] - '0';
   c = buf[2] - '0';

   for(int j = 3; j<= (a+2); j++)
   {
     x[h] = buf[j];
     h++;
   }
   h = 0;
   FB = atoi(x);   //Forward/Backward Joystick Value
   
   for(int j = (3+a); j<= (b+a+2); j++)
   {
     y[h] = buf[j];
     h++;
   }
   h = 0;
   LR = atoi(y);   //Left/Right Joystick Value

   for(int j = (3+a+b); j<= (c+b+a+2); j++)
   {
     z[h] = buf[j];
     h++;
   }
   LEDpot = atoi(z); //Endobot LED Pot Value
   
   sh = buf[a+b+c+3] - '0';
   sh1 = buf[a+b+c+4] - '0';
   sh2 = buf[a+b+c+5] - '0';

   float mappedF = map(FB, 540, 1023, 0, 100);  //Forward PWM Value
   float mappedB = map(FB, 500, 70, 0, 100);    //Backward PWM Value
   float mappedL = map(LR, 490, 192, 0, 100);   // Left PWM Value
   float mappedR = map(LR, 520, 1023, 0, 100);  // Right PWM Value
   int mappedz = map(LEDpot, 0, 1023, 0, 255);  //Endobot LED PWM Value

   if(sh == 0) //Endobot LED Control PWM
   {
     analogWrite(TIP, mappedz);
   }
   else if (sh == 1) 
   {
     analogWrite(TIP, 0);
   }

   if(FB > 540 && (LR >= 490 && LR <= 520) && (sh1 == 1 && sh2 == 1)) //Forward Movement
   {
     pwmPin1.write(mappedF / 100);
     pwmPin2.write(mappedF / 100);
     digitalWrite(DIR1,0);
     digitalWrite(DIR2,1);
     flag = 10;
   }

   else if(FB < 500 && (LR >= 490 && LR <= 520) && (sh1 == 1 && sh2 == 1)) //Backward Movement
   {
     pwmPin1.write(mappedB / 100);
     pwmPin2.write(mappedB / 100);
     digitalWrite(DIR1,1);
     digitalWrite(DIR2,0);
     flag = 01;
   }

   if(LR > 520 && (FB >= 500 && FB <= 540) && (sh1 == 1 && sh2 == 1)) //Right Turn
   {
     pwmPin1.write(mappedR / 100);
     pwmPin2.write(mappedR / 100);
     digitalWrite(DIR1,0);
     digitalWrite(DIR2,0);  
     flag = 0;
   }

   else if(LR < 490 && (FB >= 500 && FB <= 540) && (sh1 == 1 && sh2 == 1)) //Left Turn
   {
     pwmPin1.write(mappedL / 100);
     pwmPin2.write(mappedL / 100);
     digitalWrite(DIR1,1);
     digitalWrite(DIR2,1);
     flag = 0;
   }

   if((sh1 == 0 && sh2 == 1) && (FB >= 500 && FB <= 540) && (LR >= 490 && LR <= 520)) //Forward Switch
   {
     pwmPin1.write(0.5);
     pwmPin2.write(0.5);
     digitalWrite(DIR1,0);
     digitalWrite(DIR2,1);
     flag = 10;
   }
   
   else if((sh1 == 1 && sh2 == 0) && (FB >= 500 && FB <= 540) && (LR >= 490 && LR <= 520)) //Backward Switch
   {
     pwmPin1.write(0.5);
     pwmPin2.write(0.5);
     digitalWrite(DIR1,1);
     digitalWrite(DIR2,0);
     flag = 01;
   }

   if((LR >= 490 && LR <= 520) && (FB >= 500 && FB <= 540) && ((sh1 == 1 && sh2 == 1) || (sh1 == 0 && sh2 == 0))) //Idle Condition
   {
     pwmPin1.write(0);
     pwmPin2.write(0);
     flag = 0;
   }

   EA = digitalRead(encoderA);
   EB = digitalRead(encoderB);

   //Data Transfer - Nano to Mega:     
   Serial1.println(String(T1)+"a"+String(T2)+"b"+String(T3)+"c"+String(EA)+String(EB)+"d"+String(flag)+"e"); //Serial data transferred to mega for LCD display
   Serial.println(String(T1)+"a"+String(T2)+"b"+String(T3)+"c"+String(EA)+String(EB)+"d"+String(flag)+"e");  //for serial monitor debug
   Serial1.flush();
}
