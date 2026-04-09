void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    int retries = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mat ket noi oi!!");
    lcd.setCursor(0, 1);
    lcd.print("Dang ket noi ");
    while (WiFi.status() != WL_CONNECTED && retries < 10) {
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
      lcd.print("Mat ket noi oiii");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
  }
}