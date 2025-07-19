#include <Arduino.h>
#include "stdio.h"
#include "stdint.h"
#include "cstring"
#include <FS.h>

#include <Adafruit_GFX.h>      
#include <Adafruit_ST7789.h>   
#include <SPI.h>
#include <qrcode_st7789.h>
#include <LittleFS.h> // file LittelFS
#include <JPEGDecoder.h> // JPEG decoder library

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

// pin UART
// #define TXD1 0    
// #define RXD1 1   
// HardwareSerial mySerial(0);  // UART1

// Adafruit_ST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk,
//                 int8_t rst = -1);
//Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS,TFT_DC,TFT_MOSI,TFT_SCLK,TFT_RST);
Adafruit_ST7789 display=Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);
QRcode_ST7789 qrcode(&display);

bool qrDisplayed = false;
boolean SPIFFSInited = false;

extern void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
extern void drawFSJpeg(const char *filename, int xpos, int ypos);
extern void jpegRender(int xpos, int ypos);
extern void jpegInfo();

void testdrawtext(const char *text, uint16_t color, int16_t x, int16_t y,  uint8_t SizeText);
void Draw_QRcode(char* text);
void HomeScreen();
void handleATCommand(String message);



void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    //mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART giao tiếp với Hercules
    // Serial.println("ESP32C3 UART Ready");

    // Khởi tạo SPI và màn hình
    SPI.begin(TFT_SCLK, TFT_RST, TFT_MOSI, TFT_CS);
    display.init(240, 320);
    display.invertDisplay(0);  // tránh đảo màu
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);
    if (!LittleFS.begin(1)) {
      Serial.println("LittleFS Mount Failed");
      return;
    }
    listDir(LittleFS, "/", 3);
    SPIFFSInited = true;
    HomeScreen();

}

void loop() {
  if (Serial.available()) {
      // Nhận chuỗi từ máy tính
      String message = Serial.readStringUntil('\n');
      Serial.println("Received: " + message);  // Debug dòng này
      handleATCommand(message);
  }
}

// viết chữ lên màn hình TFT_ST7789
void testdrawtext(const char *text, uint16_t color, int16_t x, int16_t y,  uint8_t SizeText) {
    display.setRotation(0);
    display.setCursor(x, y);
    display.setTextColor(color);
    display.setTextSize(SizeText);
    display.setTextWrap(true);
    display.print(text);
}

// hiển thị QR_code
void Draw_QRcode(char* text){
    display.fillScreen(ST77XX_WHITE); // Xóa màn hình
    //
    testdrawtext("IOT SOLUTION", ST77XX_BLUE, 10, 10, 3);
    // Khởi tạo QR code
    qrcode.init();
    qrcode.create(text, 45, 5);
    //
    testdrawtext("SCAN THE QR CODE", ST77XX_GREEN, 20, 290, 2);
}


void HomeScreen(){
  if (SPIFFSInited) {

    // if (imageIndex > 45) {
    //   imageIndex = 0;
    // }

    String fileIndex = "/";

    // if (imageIndex < 10) {
    //   fileIndex += "0" + String(imageIndex);
    // } else {
    //   fileIndex += "" + String(imageIndex);
    // }
    fileIndex += "HomeScreen.jpg";
  //   fileIndex = "iot.jpg";
    // Serial.println("fileIndex : " + fileIndex);
    drawFSJpeg(fileIndex.c_str(), 0, 0);
    // delay(5000);
  }
}

void handleATCommand(String message) {
  message.trim();

  // ===== Lệnh hiển thị chữ =====
  if (message.startsWith("AT+STR_DISPLAY=")) {
    String args = message.substring(15);  // Bỏ "AT+STR_DISPLAY="
    int comma1 = args.indexOf(',');
    int comma2 = args.indexOf(',', comma1 + 1);

    if (comma1 > 0 && comma2 > comma1) {
      int y = args.substring(0, comma1).toInt();
      int fontSize = args.substring(comma1 + 1, comma2).toInt();
      String text = args.substring(comma2 + 1);

      // Debug
      char buffer[100];
      sprintf(buffer, "Parsed Y=%d, Size=%d, Text=%s", y, fontSize, text.c_str());
      Serial.println(buffer);

      // Hiển thị chữ
      display.fillScreen(ST77XX_WHITE);
      testdrawtext((char*)text.c_str(), ST77XX_BLUE, 10, y, fontSize);
    } else {
      Serial.println("Lỗi định dạng lệnh AT+STR_DISPLAY");
    }
    return; // Đã xử lý lệnh
  }

  // ===== Lệnh hiển thị ảnh =====
  if (message.startsWith("AT+PIC_DISPLAY=")) {
    String fileName = message.substring(15);  // Bỏ "AT+PIC_DISPLAY="
    fileName.trim();
    fileName.replace("\"", "");

    Serial.println("Displaying image: " + fileName);
    String fileIndex = "/";
    fileIndex += fileName;
    // Hiển thị ảnh tại góc trên trái
    drawFSJpeg(fileIndex.c_str(), 0, 0);
    return; // Đã xử lý lệnh
  }
}

