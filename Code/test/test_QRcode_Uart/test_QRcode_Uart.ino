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

#define TFT_BACKLIGHT -1

// pin UART
#define TXD1 7   
#define RXD1 6   
HardwareSerial mySerial(1);  // UART1

Adafruit_ST7789 display = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);
QRcode_ST7789 qrcode(&display);


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
    testdrawtext("LPBLOA000XXX", ST77XX_BLUE, 10, 10, 3);
    // Khởi tạo QR code
    qrcode.init();
    qrcode.create(text, 45, 5);
    //
    testdrawtext("SCAN THE QR CODE", ST77XX_GREEN, 20, 290, 2);
}

void Home_screen(){
    display.fillScreen(ST77XX_WHITE); // Xóa màn hình
    //trang chủ
    testdrawtext("WELCOME TO...", ST77XX_BLUE, 5, 130, 3);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART giao tiếp với Hercules
    Serial.println("ESP32C3 UART Ready");

    // Khởi tạo SPI và màn hình
    SPI.begin(TFT_SCLK, TFT_RST, TFT_MOSI, TFT_CS);
    display.init(240, 320);
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);
    Home_screen();

}

bool qrDisplayed = false;

void loop() {
    if (mySerial.available()) {
        // Nhận chuỗi từ máy tính
        String message = mySerial.readStringUntil('\n');
        message.trim();  // Xóa khoảng trắng đầu/cuối
        Serial.println("Received QR string: " + message);

        if (!qrDisplayed && message.length() > 0) {
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
            Home_screen();
            qrDisplayed = false;
        }
    }

}
