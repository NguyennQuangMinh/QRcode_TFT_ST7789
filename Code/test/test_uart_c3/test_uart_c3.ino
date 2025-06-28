#define TXD1 7   // TX từ ESP32C3 sang máy tính
#define RXD1 6   // RX từ máy tính về ESP32C3

HardwareSerial mySerial(1);  // UART1

void setup() {
  Serial.begin(115200);  // Debug qua USB
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART giao tiếp với Hercules
  
  Serial.println("ESP32C3 UART Ready");
}

void loop() {
  if (mySerial.available()) {
    String message = mySerial.readStringUntil('\n');
    message.trim();
    Serial.println("Received from PC: " + message);

    if (message == "1") {
      mySerial.println("2");  // Gửi lại số 2 cho Hercules
      Serial.println("Sent: 2");
    }
  }
}
