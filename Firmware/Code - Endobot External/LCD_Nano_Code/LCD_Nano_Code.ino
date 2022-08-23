//Global Variables:
String T1, T2, T3, EN, Dir;
float prev_m = 0;

//********************************************************************LCD Display Global Requirements*****************************************************************************//
//Header Files:
#include <Adafruit_GFX.h>         // Core graphics library
#include <Adafruit_ILI9341.h>     // Hardware-specific library
#include <SdFat.h>                // SD Card & FAT filesystem library
#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
#include <Adafruit_ImageReader.h> // Image-reading functions

//Interface Pins:
#define TFT_DC 9  //Data/Command line of Display
#define TFT_CS 10 //LCD Display chip select pin
#define TFT_RST 8 //LCD Display reset pin
#define SD_CS  4  //SD Card chip select pin
//Use default hardware SPI pins for MOSI, MISO and SCK - varies for different Arduino
//Arduino Mega SPI HW - MOSI (51), MISO (50) & SCK (52)
//Arduino Uno/Nano SPI HW - MOSI (11), MISO (12) & SCK (13)

#define USE_SD_CARD // Comment out this line to load from SPI/QSPI flash instead of SD card
#if defined(USE_SD_CARD)
  SdFat SD;  // SD card filesystem
  Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys
#else
  // SPI or QSPI flash filesystem (i.e. CIRCUITPY drive)
  #if defined(__SAMD51__) || defined(NRF52840_XXAA)
    Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS,
      PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
  #else
    #if (SPI_INTERFACES_COUNT == 1)
      Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
    #else
      Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
    #endif
  #endif
  Adafruit_SPIFlash    flash(&flashTransport);
  FatFileSystem        filesys;
  Adafruit_ImageReader reader(filesys); // Image-reader, pass in flash filesys
#endif

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
ImageReturnCode stat;  // Status from image-reading functions (success or failure)

//****************************************************************************************************************************************************************************//

//Received Data Decoding Function:
void string_processor(String data)
{
  int index_1 = data.indexOf("a"); //stores the address of "a" in index_1
  int index_2 = data.indexOf("b");
  int index_3 = data.indexOf("c");
  int index_4 = data.indexOf("d");
  int index_5 = data.indexOf("e");
 
  T1 = (data.substring(0, index_1)); //reads the string data from 0 to index_1 & stores in T1 (motor 1 temp)      
  T2 = (data.substring(index_1+1, index_2));  
  T3 = (data.substring(index_2+1, index_3));
  EN = (data.substring(index_3+1, index_4));
  Dir = (data.substring(index_4+1, index_5));
}

void setup() 
{
  Serial.begin(9600); //To receive data from Mega & for serial monitor debug

  //Display:
  LCD_Display_ini(); //Display initialisation function
  tft.setRotation(2); //fxn to set display direction 
  Serial.print(F("Loading LCD_Boot_Image to screen..."));
  stat = reader.drawBMP("/LCD_Boot_Image.bmp", tft, 0, 0); //Load full-screen BMP file 'LCD_Boot_Image.bmp' at position (0,0) (top left)
  //Notice the 'reader' object performs this, with 'tft' as an argument.
  reader.printStatus(stat);  //Displays status of printed image (success or failure)
  delay(3000); //Show image for 3 seconds
}


void loop() 
{
  if(Serial.available() > 2)
  {
    String data = Serial.readStringUntil('\n');
    string_processor(data); //Fxn to process received string data
  }
  
  if (millis() > prev_m + 2000)
  {
    tft.fillScreen(ILI9341_BLACK); //fxn to fill screen with black color
    tft.setRotation(1); //fxn to set display direction
    tft.setTextColor(ILI9341_WHITE); //fxn to set text color
    tft.setTextSize(3); //fxn to set text size
    tft.setCursor(0, 1); //fxn to set cursor size
    
    if( T1.toFloat() > 70) //Motor 1 Temp Check & Display
    {
      tft.setTextColor(ILI9341_RED); 
      tft.print("Motor 1 Temp:");
      tft.print(T1);
      tft.println(" C");
      tft.setTextColor(ILI9341_WHITE);
    }
    else
    {
      tft.print("Motor 1 Temp:");
      tft.print(T1);
      tft.println(" C");
    } 
    
    if( T2.toFloat() > 70) //Motor 2 Temp Check & Display
    {
      tft.setTextColor(ILI9341_RED); 
      tft.print("Motor 2 Temp:");
      tft.print(T2);
      tft.println(" C");
      tft.setTextColor(ILI9341_WHITE);
    }
    else
    {
      tft.print("Motor 2 Temp:");
      tft.print(T2);
      tft.println(" C");
    }

    tft.print("Endobot Temp:"); //Endobot Chamber Temp
    tft.print(T3);
    tft.println(" C");
    
    tft.print("Distance: "); //Encoder Distance display
    tft.print(EN);
    tft.println(Dir);
    
    prev_m = millis();
  }


int LCD_Display_ini()
{
  tft.begin(); //Initialize screen

  //The Adafruit_ImageReader constructor call (above, before setup()) accepts an uninitialized SdFat or FatFileSystem object. 
  //This MUST BE INITIALIZED before using any of the image reader functions!
  //Below code is for initialising flash memory & SD card with error reporting
  
  Serial.print(F("Initializing filesystem..."));
  #if defined(USE_SD_CARD)
    //SD card is pretty straightforward, a single call..
    if(!SD.begin(SD_CS, SD_SCK_MHZ(25))) //ESP32 requires 25 MHz limit
    { 
      Serial.println(F("SD begin() failed"));
      for(;;); //Fatal error, do not continue
    }
  #else
    //SPI or QSPI flash requires two steps, one to access the bare flash memory itself, then the second to access the filesystem within..
    if(!flash.begin()) 
    {
      Serial.println(F("flash begin() failed"));
      for(;;);
    }
    if(!filesys.begin(&flash)) 
    {
      Serial.println(F("filesys begin() failed"));
      for(;;);
    }
  #endif
      Serial.println(F("OK!")); 
}
