//Arduino Mega Code - Controller Arduino Pro mini, RS422 TX - RX, LCD Arduino nano

void setup() 
{
  Serial.begin(9600);  //for serial monitor debug
  Serial1.begin(9600); //Controller Arduino Nano UART1
  Serial2.begin(9600); //RS422 module UART2
  Serial3.begin(9600); //LCD Display Arduino Nano UART3
}

void loop() 
{
  if (Serial1.available()) //if any data came to the port
  {
    Serial.println("Data received from Controller");
    delay (5);  //wait a bit for the entire data packet to be accepted by the port
    String buf = Serial1.readStringUntil('\n'); //Read serial data until null character
    Serial.println(buf);
    Serial2.println(buf);  //Send received serial data to RS422 module
    Serial.println("Data sent to Endobot");
  }
  
  if (Serial2.available()) //if any data came to the port
  {
    Serial.println("Data received from Endobot");
    delay (5);  //wait a bit for the entire data packet to be accepted by the port 
    String data = Serial2.readStringUntil('\n'); //Read serial data until null character
    Serial.println(data);
    Serial3.println(data+'\n'); //Send received serial data to LCD nano
    Serial.println("Data sent to LCD Display");
  }
  
  Serial1.flush(); //Clears serial port
  Serial2.flush();
  Serial3.flush();
}
