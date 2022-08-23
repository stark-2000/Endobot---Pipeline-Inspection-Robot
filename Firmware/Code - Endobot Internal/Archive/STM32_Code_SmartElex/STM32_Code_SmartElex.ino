//STM32 Board with SmartElex Motor Driver - basic and RS422

HardwareSerial Serial1(USART3);

String buf;

//Motor Driver Pins:
#define DIR1 PE12
#define PWM1 PE13
#define DIR2 PE11
#define PWM2 PE14

//Endobot LED Pin:
#define TIP PB12


void setup()
{
  Serial1.setRx(PD9);
  Serial1.setTx(PD8);
  Serial1.begin (9600);
  
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(TIP, OUTPUT);
}


void loop() 
{ 
   
   int a = 0, b = 0, c = 0, sh = 0, sh1 = 0, sh2 = 0;
   char x[10] = "0", y[10] = "0", z[10] = "0";
   int h = 0;
   int FB, LR, LEDpot;

   if (Serial.available ())                  // if any data came to the port
   {
       delay (5);                            // wait a bit for the entire data packet to be accepted by the port
       buf = Serial.readStringUntil('\n');   //Read serial data until null character
       Serial.println(buf);                  //Print data on serial monitor
   }

   a = buf[0] - '0';
   //Serial.println(a);
   
   b = buf[1] - '0';
   //Serial.println(b);
   
   c = buf[2] - '0';
   //Serial.println(c);
   //delay(1000);

   
   for(int j = 3; j<= (a+2); j++)
   {
     x[h] = buf[j];
     h++;
   }
   h = 0;
   //Serial.println(x);
   FB = atoi(x);                    //Forward/Backward Joystick Value
   //Serial.println(FB);
   
   
   for(int j = (3+a); j<= (b+a+2); j++)
   {
     y[h] = buf[j];
     h++;
   }
   h = 0;
   //Serial.println(y);
   LR = atoi(y);                    //Left/Right Joystick Value
   //Serial.println(LR);

   
   for(int j = (3+a+b); j<= (c+b+a+2); j++)
   {
     z[h] = buf[j];
     h++;
   }
   //Serial.println(z);
   LEDpot = atoi(z);                //Endobot LED Pot Value
   //Serial.println(LEDpot);
   

   sh = buf[a+b+c+3] - '0';
   //Serial.println(sh);
   sh1 = buf[a+b+c+4] - '0';
   //Serial.println(sh1);
   sh2 = buf[a+b+c+5] - '0';
   //Serial.println(sh2);


   int mappedF = map(FB, 540, 1023, 0, 255);  
   //Serial.println(mappedF);
   int mappedB = map(FB, 500, 70, 0, 255);
   //Serial.println(mappedB);
   int mappedL = map(LR, 490, 192, 0, 255);
   //Serial.println(mappedL);
   int mappedR = map(LR, 520, 1023, 0, 255); 
   //Serial.println(mappedR);
   int mappedz = map(LEDpot, 0, 1023, 0, 255);

   if(sh == 0)
   {
     analogWrite(TIP, mappedz);
   }
    
   if(FB > 540 && (LR >= 490 && LR <= 520) && (sh1 == 1 && sh2 == 1)) //Forward Movement
   {
     analogWrite(PWM1, mappedF);
     analogWrite(PWM2, mappedF);
     digitalWrite(DIR1, HIGH);
     digitalWrite(DIR2, LOW);
   }

   else if(FB < 500 && (LR >= 490 && LR <= 520) && (sh1 == 1 && sh2 == 1)) //Reverse Movement
   {
     analogWrite(PWM1, mappedB);
     analogWrite(PWM2, mappedB);
     digitalWrite(DIR1, LOW);
     digitalWrite(DIR2, HIGH);
   }

   if(LR > 520 && (FB >= 500 && FB <= 540) && (sh1 == 1 && sh2 == 1)) //Right Turn
   {
     analogWrite(PWM1, 180);
     analogWrite(PWM2, 180);
     digitalWrite(DIR1, LOW);
     digitalWrite(DIR2, LOW);
   }

   else if(LR < 490 && (FB >= 500 && FB <= 540) && (sh1 == 1 && sh2 == 1)) //Left Turn
   {
     analogWrite(PWM1, 180);
     analogWrite(PWM2, 180);
     digitalWrite(DIR1, HIGH);
     digitalWrite(DIR2, HIGH);
   }

   if((sh1 == 0 && sh2 == 1) && (FB >= 500 && FB <= 540) && (LR >= 490 && LR <= 520)) //Forward Switch
   {
     analogWrite(PWM1, 255);
     analogWrite(PWM2, 255);
     digitalWrite(DIR1, HIGH);
     digitalWrite(DIR2, LOW);
   }
   
   else if((sh1 == 1 && sh2 == 0) && (FB >= 500 && FB <= 540) && (LR >= 490 && LR <= 520)) //Backward Switch
   {
     analogWrite(PWM1, 255);
     analogWrite(PWM2, 255);
     digitalWrite(DIR1, LOW);
     digitalWrite(DIR2, HIGH);
   }

   if((LR >= 490 && LR <= 520) && (FB >= 500 && FB <= 540) && ((sh1 == 1 && sh2 == 1) || (sh1 == 0 && sh2 == 0))) //Idle Condition
   {
     analogWrite(PWM1, 0);
     analogWrite(PWM2, 0);
   }

}
