 void connect_wifi(){
bool connected = false;
while (!connected) {
  for (int i = 0; i < wifiCount; i++) {
    WiFi.begin(ssids[i], passwords[i]);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thu ket noi:");
    lcd.setCursor(0, 1);
    lcd.print(ssids[i]);

    dotCount = 0;
    int retry = 0;

    while (WiFi.status() != WL_CONNECTED && retry < 20) {
      lcd.setCursor(15, 1);
      lcd.print(".");
      dotCount++;
      if (dotCount > 3) {
        lcd.setCursor(13, 1);
        lcd.print("   ");
        dotCount = 0;
      }
      delay(500);
      retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      break;
    }
  }

  if (!connected) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Loi ket noi WiFi");
    delay(3000);  // Chờ 3 giây rồi thử lại
  }

}

 }
 