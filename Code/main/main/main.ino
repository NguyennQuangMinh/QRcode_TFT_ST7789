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
#define TXD1 0    
#define RXD1 1   
HardwareSerial mySerial(1);  // UART1

// Adafruit_ST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk,
//                 int8_t rst = -1);
//Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS,TFT_DC,TFT_MOSI,TFT_SCLK,TFT_RST);
Adafruit_ST7789 display=Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);
QRcode_ST7789 qrcode(&display);

bool qrDisplayed = false;

// viết chữ lên màn hình TFT_ST7789
void testdrawtext(char *text, uint16_t color, int16_t x, int16_t y,  uint8_t SizeText) {
    display.setRotation(0);
    display.setCursor(x, y);
    display.setTextColor(color);
    display.setTextSize(SizeText);
    display.setTextWrap(true);
    display.print(text);
}

// hiển thị QR_code
void Draw_QRcode(char *text){
    display.fillScreen(ST77XX_WHITE); // Xóa màn hình
    //
    testdrawtext("IOT SOLUTION", ST77XX_BLUE, 10, 10, 3);
    // Khởi tạo QR code
    qrcode.init();
    qrcode.create(text, 45, 5);
    //
    testdrawtext("SCAN THE QR CODE", ST77XX_GREEN, 20, 290, 2);
}

boolean SPIFFSInited = false;
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
    fileIndex += "HOME_SCREEN_PinHigh.jpg";
  //   fileIndex = "iot.jpg";
    // Serial.println("fileIndex : " + fileIndex);
    drawFSJpeg(fileIndex.c_str(), 0, 0);
    delay(5000);

    // HomeScreen Pin medium
    fileIndex = "/";
    // if (imageIndex < 10) {
    //   fileIndex += "0" + String(imageIndex);
    // } else {
    //   fileIndex += "" + String(imageIndex);
    // }
    fileIndex += "HOME_SCREEN_PinMedium.jpg";
  //   fileIndex = "iot.jpg";
    // Serial.println("fileIndex : " + fileIndex);
    drawFSJpeg(fileIndex.c_str(), 0, 0);
    delay(5000);

    // imageIndex++;

        // HomeScreen Pin medium
    fileIndex = "/";
    // if (imageIndex < 10) {
    //   fileIndex += "0" + String(imageIndex);
    // } else {
    //   fileIndex += "" + String(imageIndex);
    // }
    fileIndex += "HOME_SCREEN_PinLow.jpg";
  //   fileIndex = "iot.jpg";
    // Serial.println("fileIndex : " + fileIndex);
    drawFSJpeg(fileIndex.c_str(), 0, 0);
    // delay(5000);

    // imageIndex++;
  }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART giao tiếp với Hercules
    Serial.println("ESP32C3 UART Ready");

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
    if (mySerial.available()) {
        // Nhận chuỗi từ máy tính
        String message = mySerial.readStringUntil('\n');
        message.trim();  // Xóa khoảng trắng đầu/cuối

        // Kiểm tra lệnh AT+STR_DISPLAY
        if (message.startsWith("AT+STR_DISPLAY=")) {
          // Cắt phần sau dấu '='
          String args = message.substring(15);  // sau "AT+STR_DISPLAY="

          int comma1 = args.indexOf(',');
          int comma2 = args.indexOf(',', comma1 + 1);

          if (comma1 > 0 && comma2 > comma1) {
            // Bóc tách các thành phần
            int y = args.substring(0, comma1).toInt();
            int fontSize = args.substring(comma1 + 1, comma2).toInt();
            String text = args.substring(comma2 + 1);

            // Debug
            char buffer[100];
            sprintf(buffer, "Parsed Y=%d, Size=%d, Text=%s", y, fontSize, text.c_str());
            mySerial.println(buffer);

            // Hiển thị lên màn hình
            display.fillScreen(ST77XX_WHITE);  // Xóa màn hình
            testdrawtext((char*)text.c_str(), ST77XX_BLUE, 10, y, fontSize);
            delay(10000);// chờ 10 giây
            HomeScreen(); // quay lại màn hình chính
          } else {
            mySerial.println("Lỗi định dạng chuỗi lệnh AT.");
          }
        }else if(!qrDisplayed && message.length() > 0) {
            // Gửi xác nhận lại về máy tính (tùy chọn)
            mySerial.println("QR RECEIVED");
            Serial.println("Displaying QR...");

            // Chuyển String → char[] để vẽ QR
            char qrChar[300];
            message.toCharArray(qrChar, sizeof(qrChar));

            // Hiển thị QR code từ chuỗi nhận được
            Draw_QRcode(qrChar);
            qrDisplayed = true;

            // Đợi 10 giây
            delay(10000);

            // Quay lại màn hình chính
            HomeScreen();
            qrDisplayed = false;
        }
  }
}


