//Arduino Nano normal Code - SmartElex motor driver,RS422 data Tx - Rx

//Motor Driver Pins:
/*
  DIR1 8
  PWM1 10
  
  DIR2 12
  PWM2 11
*/


//Other Global Variables:
String buf;   //Controller serial data
#define TIP 9 //Endobot LED pin

void setup()
{
  Serial.begin (9600); //Send & Receive serial data to RS422 module & for serial monitor debug

  TCCR1B = TCCR1B & B11111000 | B00000001; //PWM freq setting for motor control
  TCCR2B = TCCR2B & B11111000 | B00000001; //PWM freq setting for motor control

  //Motor Driver Pins:
  pinMode(8,OUTPUT);       // dir1
  pinMode(12,OUTPUT);      // dir2
  pinMode(10,OUTPUT);      // pwm1
  pinMode(11,OUTPUT);      // pwm2
  
  //Other Pins:
  pinMode(TIP, OUTPUT); //Endbot LED TIP122
}


void loop() 
{ 
   //Data Transfer Variables:
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
   }

   else if(FB < 500 && (LR >= 490 && LR <= 520) && (sh1 == 1 && sh2 == 1)) //Backward Movement
   {
     analogWrite(10,mappedB);
     analogWrite(11,mappedB);
     digitalWrite(8,1);
     digitalWrite(12,1);
   }

   if(LR > 520 && (FB >= 500 && FB <= 540) && (sh1 == 1 && sh2 == 1)) //Right Turn
   {
     analogWrite(10,mappedR);
     analogWrite(11,mappedR);
     digitalWrite(8,1);
     digitalWrite(12,0); 
   }

   else if(LR < 490 && (FB >= 500 && FB <= 540) && (sh1 == 1 && sh2 == 1)) //Left Turn
   {
     analogWrite(10,mappedL);
     analogWrite(11,mappedL);
     digitalWrite(8,0);
     digitalWrite(12,1); 
   }

   if((sh1 == 0 && sh2 == 1) && (FB >= 500 && FB <= 540) && (LR >= 490 && LR <= 520)) //Forward Switch
   {
     analogWrite(10,255);
     analogWrite(11,255);
     digitalWrite(8,0);
     digitalWrite(12,0);
   }
   
   else if((sh1 == 1 && sh2 == 0) && (FB >= 500 && FB <= 540) && (LR >= 490 && LR <= 520)) //Backward Switch
   {
     analogWrite(10,255);
     analogWrite(11,255);
     digitalWrite(8,1);
     digitalWrite(12,1);
   }

   if((LR >= 490 && LR <= 520) && (FB >= 500 && FB <= 540) && ((sh1 == 1 && sh2 == 1) || (sh1 == 0 && sh2 == 0))) //Idle Condition
   {
     analogWrite(10,0);
     analogWrite(11,0);
   }
   
   Serial.flush();
}
