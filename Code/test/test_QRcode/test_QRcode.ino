#include <Adafruit_GFX.h>      
#include <Adafruit_ST7789.h>   
#include <SPI.h>
#include <qrcode_st7789.h>

// pin TFT_ST7789
#define TFT_CS      5
#define TFT_RST     0 
#define TFT_DC      1
#define TFT_MOSI 	4  
#define TFT_SCLK 	2

// pin UART
#define TXD1    21
#define RXD1    20

#define TFT_BACKLIGHT -1

Adafruit_ST7789 display = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);
QRcode_ST7789 qrcode(&display);

HardwareSerial mySerial(2);

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
    Serial.begin(115200);
    Serial.println("Starting...");

    // Khởi tạo SPI và màn hình
    SPI.begin(TFT_SCLK, TFT_RST, TFT_MOSI, TFT_CS);
    display.init(240, 320);
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);

    display.fillScreen(ST77XX_WHITE); // Xóa màn hình
    //
    testdrawtext("LPBLOA000XXX", ST77XX_BLUE, 10, 10, 3);
    // Khởi tạo QR code
    qrcode.init();
    qrcode.create("00020101021138560010A000000727012600069704490112LPBLOA0029020208QRIBFTTA53037045802VN63049F94", 45, 5);
    //
    testdrawtext("SCAN THE QR CODE", ST77XX_GREEN, 20, 290, 2);

}

void loop() {

}
