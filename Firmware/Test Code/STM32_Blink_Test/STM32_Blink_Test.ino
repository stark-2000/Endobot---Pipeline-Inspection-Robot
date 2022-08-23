void setup() 
{
  pinMode(PE12, OUTPUT);
}

void loop() 
{
  digitalWrite(PE12, HIGH);   
  delay(3000);                       
  digitalWrite(PE12, LOW);    
  delay(3000);                       
}
