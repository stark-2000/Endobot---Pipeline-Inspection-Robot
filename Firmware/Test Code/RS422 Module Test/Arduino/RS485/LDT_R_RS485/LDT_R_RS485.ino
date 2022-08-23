#define SerialTxControl 2
#define RS485Transmit HIGH
#define RS485Receive LOW

char buffer [100];
byte state = 0;

void setup (void) 
{
    Serial.begin (9600);
    pinMode (13, OUTPUT); 
    pinMode (SerialTxControl, OUTPUT); 
    digitalWrite (SerialTxControl, RS485Receive); // put the device into receiver mode
}
 
void loop (void) 
{
    int i = 0;
    if (Serial.available ()) 
    {// if any data came to the port
        delay (5); // wait a bit for the entire data packet to be accepted by the port
        while (Serial.available ()) 
        {
            buffer [i ++] = Serial.read (); // read data and write it to the buffer
        }
    }
    if (i> 0) 
    {// if there is something in the buffer
        buffer [i ++] = '\ 0'; // convert the buffer contents to a string by adding a null character
        if (strcmp (buffer, "ping")) 
        {// if the accepted string is equal to the ping text
            digitalWrite (13, state); // blink LED
            Serial.println("yes");
            state =! state;
        }
    }
}
