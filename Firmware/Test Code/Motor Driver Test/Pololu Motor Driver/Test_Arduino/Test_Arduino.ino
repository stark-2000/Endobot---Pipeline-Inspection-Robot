void setup()
{
/*
  Pins -- Arduino Nano (ble, every, normal)
  DIR1 8
  PWM1 10
  
  DIR2 12
  PWM2 11
  
  M1 FAULT 3 
  M2 FAULT 5
  
  M1 SLEEP 2
  M2 SLEEP 4
*/

  pinMode( 3,INPUT_PULLUP); // m1f
  pinMode( 5,INPUT_PULLUP); // m2f
  
  pinMode( 2,OUTPUT);       // m1s
  pinMode( 4,OUTPUT);       // m2s
  
  pinMode( 8,OUTPUT);       // dir1
  pinMode( 12,OUTPUT);      // dir2
  
  pinMode( 10,OUTPUT);      // pwm1
  pinMode( 11,OUTPUT);      // pwm2
  
  digitalWrite(2,HIGH);
  digitalWrite(4,HIGH);
  
  //pinMode(14,INPUT);    //POT
  //Serial.begin(115200);
}


void loop() 
{ 
   //float sp = map(analogRead(A0),0,1024,0,255);
   float sp = 255;
  
   analogWrite(10,sp);
   digitalWrite(8,0);
   analogWrite(11,sp);
   digitalWrite(12,1);
   Serial.println(sp);
   delay(10);
}
