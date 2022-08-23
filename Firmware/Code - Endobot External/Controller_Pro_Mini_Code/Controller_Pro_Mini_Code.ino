//Interface Pins:
#define FB A4 //Forward/Backward Joystick
#define LR A3 //Left/Right Joystick
#define LP A1 //Endobot LED Pot
#define ELED 11 //Endobot LED On/Off
#define FH 12 //Forward position hold
#define RH 5 //Reverse position hold
#define POTL 10 //Endobot LED pot's backlit LED


//Global Variables:
int xpot, ypot, LEDpot;
int LEDpos, FHold, RHold;


void setup() 
{
  Serial.begin (9600); //Serial Data Transfer (Pro Mini to Mega)
  pinMode(ELED, INPUT_PULLUP);
  pinMode(FH, INPUT_PULLUP);
  pinMode(RH, INPUT_PULLUP);
  pinMode(POTL, OUTPUT);
}


void loop() 
{
  int count1 = 0, count2 = 0, count3 = 0;
 
  xpot = analogRead(FB);
  //Serial.println(xpot);
  delay(100);
  count1=(xpot==0)?1:log10(xpot)+1; 
  
  ypot = analogRead(LR);
  //Serial.println(ypot);
  delay(100);
  count2=(ypot==0)?1:log10(ypot)+1; 

  LEDpot = analogRead(LP);
  //Serial.println(LEDpot);
  delay(100);
  count3=(LEDpot==0)?1:log10(LEDpot)+1; 

  LEDpos = digitalRead(ELED);
  FHold = digitalRead(FH);
  RHold = digitalRead(RH);

  int mapped = map(LEDpot, 0, 1023, 0, 255);
  if(LEDpos == 0)
  {
    analogWrite(POTL, mapped);
  }

  else if(LEDpos == 1)
  {
    digitalWrite(POTL, LOW);
  }
  
  Serial.print(count1);
  Serial.print(count2);
  Serial.print(count3);
  Serial.print(xpot);
  Serial.print(ypot);
  Serial.print(LEDpot);
  
  Serial.print(LEDpos);
  Serial.print(FHold);
  Serial.println(RHold);
  
  //delay(500);
}
