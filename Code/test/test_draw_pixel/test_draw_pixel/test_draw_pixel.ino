#include <Adafruit_GFX.h>      
#include <Adafruit_ST7789.h>   
#include <SPI.h>
#include <qrcode_st7789.h>

#include <LittleFS.h>

// JPEG decoder library
#include <JPEGDecoder.h>
#include "font.h"


// pin TFT_ST7789
// #define TFT_CS      5
// #define TFT_RST     0 
// #define TFT_DC      1
// #define TFT_MOSI 	4  
// #define TFT_SCLK 	2

#define TFT_CS      5
#define TFT_RST     6
#define TFT_DC      8
#define TFT_MOSI 	9  
#define TFT_SCLK 	7

#define TFT_BACKLIGHT -1
Adafruit_ST7789 display=Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);

// viết chữ lên màn hình TFT_ST7789
void testdrawtext(char *text, uint16_t color, int16_t x, int16_t y,  uint8_t SizeText) {
    display.setRotation(0);
    display.setCursor(x, y);
    display.setTextColor(color);
    display.setTextSize(SizeText);
    display.setTextWrap(true);
    display.print(text);
}
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Starting...");

    // mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART giao tiếp với Hercules
    Serial.println("ESP32C3 UART Ready");

    // Khởi tạo SPI và màn hình
    SPI.begin(TFT_SCLK, TFT_RST, TFT_MOSI, TFT_CS);
    display.init(240, 320);
    display.invertDisplay(0);  // tránh đảo màu
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);
    display.fillScreen(ST77XX_WHITE);  // Xóa màn hình
    
    display.drawPixel(5, 15, ST77XX_RED);
    put_String(10,180,"TRẦN VĂN THẮNG",&Font3); // font2 chuan

}

void loop() {
  // put your main code here, to run repeatedly:

}
