//Arduino Nano normal Code - Pololu G2 motor driver,RS422 data Tx - Rx,Encoder & Temperature Sensor

//Motor Driver Pins:
/*
  DIR1 8
  PWM1 10
  
  DIR2 12
  PWM2 11
*/

//Global Variables - Encoder:
const int encoderPinA = 6;
const int encoderPinB = 7;
bool flag = 0;
bool EA, EB;

//Thermister Global Variables:
#define RT0 10000 //Ω  //Thermister Resistance at 25°C 
#define B 3977    // K //B-Value Material Constant 
#define VCC 5     //Supply voltage to Thermister
#define R 1000    //R=1KΩ, Resistance connected in series with Thermister 
float RT, VR, ln, TX, T0, VRT;

//Other Global Variables:
String buf;   //Controller serial data
#define TIP 9 //Endobot LED pin

//Fxn to read & calculate motor Temperature:
float temp_read(int tempPin)
{
  VRT = analogRead(tempPin);         //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;                    //Voltage across resistance in series with thermister
  RT = VRT / (VR / R);               //Resistance of RT (Thermister Resistance)

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor

  TX = TX - 273.15;                 //Conversion to Celsius
  return TX; 
}


void setup()
{
  Serial.begin (9600); //Send & Receive serial data to RS422 module & for serial monitor debug

  T0 = 25 + 273.15;  //Temperature T0 from datasheet, conversion from Celsius to kelvin
  //Ref temp of thermister, 10K resistance at 25°C

  TCCR1B = TCCR1B & B11111000 | B00000001; //PWM freq setting for motor control
  TCCR2B = TCCR2B & B11111000 | B00000001; //PWM freq setting for motor control

  //Motor Driver Pins:
  pinMode(8,OUTPUT);       //dir1
  pinMode(12,OUTPUT);      //dir2
  pinMode(10,OUTPUT);      //pwm1
  pinMode(11,OUTPUT);      //pwm2
  
  //Other Pins:
  pinMode (encoderPinA, INPUT);
  pinMode (encoderPinB, INPUT);
  pinMode(A2,INPUT); //Temp Sensor 1 - Motor 1
  pinMode(A3,INPUT); //Temp Sensor 2 - Motor 2
  pinMode(A4,INPUT); //Temp Sensor 3 - Indoor Temp
  pinMode(TIP, OUTPUT); //Endbot LED TIP122
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
   if (Serial.available()) // if any data came to the port
   {
     delay(50);  // wait a bit for the entire data packet to be accepted by the port
     buf = Serial.readStringUntil('\n');  //Read serial data until null character
     //Serial.println(buf);
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

   int mappedF = map(FB, 540, 1023, 0, 255);  //Forward PWM Value
   int mappedB = map(FB, 500, 70, 0, 255);    //Backward PWM Value
   int mappedL = map(LR, 490, 192, 0, 255);   // Left PWM Value
   int mappedR = map(LR, 520, 1023, 0, 255);  // Right PWM Value
   int mappedz = map(LEDpot, 0, 1023, 0, 255); //Endobot LED PWM Value

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
     analogWrite(10,mappedF);
     analogWrite(11,mappedF);
     digitalWrite(8,0);
     digitalWrite(12,0);
     flag = 1;
   }

   else if(FB < 500 && (LR >= 490 && LR <= 520) && (sh1 == 1 && sh2 == 1)) //Backward Movement
   {
     analogWrite(10,mappedB);
     analogWrite(11,mappedB);
     digitalWrite(8,1);
     digitalWrite(12,1);
     flag = 1;
   }

   if(LR > 520 && (FB >= 500 && FB <= 540) && (sh1 == 1 && sh2 == 1)) //Right Turn
   {
     analogWrite(10,mappedR);
     analogWrite(11,mappedR);
     digitalWrite(8,1);
     digitalWrite(12,0); 
     flag = 0;  
   }

   else if(LR < 490 && (FB >= 500 && FB <= 540) && (sh1 == 1 && sh2 == 1)) //Left Turn
   {
     analogWrite(10,mappedL);
     analogWrite(11,mappedL);
     digitalWrite(8,0);
     digitalWrite(12,1);
     flag = 0;
   }

   if((sh1 == 0 && sh2 == 1) && (FB >= 500 && FB <= 540) && (LR >= 490 && LR <= 520)) //Forward Switch
   {
     analogWrite(10,255);
     analogWrite(11,255);
     digitalWrite(8,0);
     digitalWrite(12,0);
     flag = 1; 
   }
   
   else if((sh1 == 1 && sh2 == 0) && (FB >= 500 && FB <= 540) && (LR >= 490 && LR <= 520)) //Backward Switch
   {
     analogWrite(10,255);
     analogWrite(11,255);
     digitalWrite(8,1);
     digitalWrite(12,1);
     flag = 1; 
   }

   if((LR >= 490 && LR <= 520) && (FB >= 500 && FB <= 540) && ((sh1 == 1 && sh2 == 1) || (sh1 == 0 && sh2 == 0))) //Idle Condition
   {
     analogWrite(10,0);
     analogWrite(11,0);
     flag = 0;
   }

   if (flag == 1)
   {
     EA = digitalRead(encoderPinA);
     EB = digitalRead(encoderPinB);
   }

   //Data Transfer - Nano to Mega:     
   Serial.println(String(T1)+"a"+String(T2)+"b"+String(T3)+"c"+String(EA)+String(EB)+"d"); //Serial data transferred to mega for LCD display
   Serial.flush();
}
