#include "SPI.h"
#include "Adafruit_ILI9341.h"


#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_MISO 12     

//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() 
{
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 

  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  
  tft.begin();
}


void loop(void) 
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(3);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
  tft.setCursor(0, 0);
  tft.println("Hello World !");
  delay(1000);
}
