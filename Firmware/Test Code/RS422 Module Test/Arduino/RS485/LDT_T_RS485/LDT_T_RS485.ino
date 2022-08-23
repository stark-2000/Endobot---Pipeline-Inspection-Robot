#define SerialTxControl 2 // Pin D2 will switch receiver / transmitter mode
#define RS485Transmit HIGH
#define RS485Receive LOW

void setup (void) 
{
    Serial.begin (9600); 
    pinMode (SerialTxControl, OUTPUT);
    digitalWrite (SerialTxControl, RS485Transmit); // put the device into transmitter mode
}
 
void loop (void) 
{
    Serial.print ("ping"); // send text
    delay (500);
}
