const unsigned long wifiRetryInterval = 5000; // 5 giây thử lại
void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.begin(ssid, pass);
    int retries = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mất kết nối òi!!");
    lcd.setCursor(0, 1);
    lcd.print("Đang kết nối lại");
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
      char dot_pos;
      dot_pos = 13 + dotCount;
      lcd.setCursor(dot_pos, 1);  // vị trí cuối dòng
      lcd.print(".");
      dotCount++;
      if (dotCount > 3) {  // sau 3 chấm thì xóa và quay lại
        lcd.setCursor(13, 1);
        lcd.print("   ");  // xóa 3 dấu chấm cũ
        dotCount = 0;
      }

      retries++;
    }
    if (WiFi.status() != WL_CONNECTED) {

      lcd.setCursor(0, 0);
      lcd.print("Mất lun òi");
      lcd.setCursor(0, 1);
      lcd.print("Xíu cắm lại ii");
    }
  }
}

void checkAndReconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    // Thử ping Google DNS để chắc chắn có internet
    WiFiClient client;
    if (client.connect("8.8.8.8", 53)) {
      client.stop();
      Serial.println("Wi-Fi & Internet OK");
      return;
    }
  }

    // Nếu chưa fully connected
  unsigned long now = millis();
  if (now - lastWiFiAttempt >= wifiRetryInterval) {
    lastWiFiAttempt = now;
    Serial.println("Chưa fully connected, reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, pass);
  }
}

void checkAndReconnectWiFi_1st() {
  unsigned long lastUpdate_dot = 0;
  byte dotCount = 0;
  while (true) {
    bool internetOk = false;

    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      if (client.connect("8.8.8.8", 53)) { // ping Google DNS
        client.stop();
        internetOk = true;
      }
    }

    if (internetOk) {
      Serial.println("Wi-Fi & Internet OK");
      break; // ra ngoài, tiếp tục chương trình
    }

    // Hiển thị dấu chấm khi đang chờ kết nối
    if (millis() - lastUpdate_dot >= 500) { // 0.5s cập nhật một lần
      lastUpdate_dot = millis();
      dotCount = (dotCount + 1) % 4;

      lcd.setCursor(0, 0);
      lcd.print("Đợi xíu         ");
      lcd.setCursor(0, 1);
      lcd.print("Kiểm tra mạng  ");
      for (byte i = 0; i < dotCount; i++) lcd.print(".");
    }

    // Thử reconnect sau mỗi khoảng thời gian
    if (millis() - lastWiFiAttempt >= wifiRetryInterval) {
      lastWiFiAttempt = millis();
      Serial.println("Chưa fully connected, reconnecting...");
      WiFi.disconnect();
      WiFi.begin(ssid, pass);
    }
  }
}

