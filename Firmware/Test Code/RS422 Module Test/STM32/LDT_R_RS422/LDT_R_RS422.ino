HardwareSerial Serial1(USART3);

char buffer [100];

void setup () 
{
    Serial1.setRx(PD9);
    Serial1.setTx(PD8);
    Serial1.begin (9600);
    pinMode(PE12, OUTPUT);
}

 
void loop () 
{
    Serial1.write ("ping"); // send text
    delay (500);
    
    int i = 0;
    if (Serial1.available ()) // if any data came to the port
    {
        delay (5); // wait a bit for the entire data packet to be accepted by the port
        while (Serial1.available ()) 
        {
            buffer [i ++] = Serial1.read (); // read data and write it to the buffer
        }
    }
    if (i> 0) // if there is something in the buffer
    {
        buffer [i ++] = '\ 0'; // convert the buffer contents to a string by adding a null character
        if (strcmp (buffer, "hello")) // if the accepted string is equal to the ping text
        {
            //Serial.println("hello");
            digitalWrite(PE12, HIGH);
            delay(1000);
            digitalWrite(PE12, LOW);
        }
    }
}
