char buffer [100];

void setup () 
{
  //Serial1.begin(9600);
  Serial2.begin(9600);
  Serial.begin(9600);
}

 
void loop () 
{
    Serial.print ("hello"); // send text
    delay (500);

    int i = 0;
    if (Serial2.available ()) // if any data came to the port
    {
        delay (5); // wait a bit for the entire data packet to be accepted by the port
        while (Serial2.available ()) 
        {
            buffer [i ++] = Serial2.read (); // read data and write it to the buffer
        }
    }
    if (i> 0) // if there is something in the buffer
    {
        Serial.println(buffer);
    }

    delay(500);
}
