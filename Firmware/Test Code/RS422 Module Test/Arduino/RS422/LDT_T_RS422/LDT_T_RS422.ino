char buffer [100];

void setup () 
{
    Serial.begin (9600);
}

 
void loop () 
{
    Serial.write ("hello"); // send text
    delay (500);

    int i = 0;
    if (Serial.available ()) // if any data came to the port
    {
        delay (5); // wait a bit for the entire data packet to be accepted by the port
        while (Serial.available ()) 
        {
            buffer [i ++] = Serial.read (); // read data and write it to the buffer
        }
    }
    if (i> 0) // if there is something in the buffer
    {
        buffer [i ++] = '\ 0'; // convert the buffer contents to a string by adding a null character
        if (strcmp (buffer, "ping")) // if the accepted string is equal to the ping text
        {
            Serial.println("ping");
        }
    }
}
