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
      lcd.print("Mất lun òi");
      lcd.setCursor(0, 1);
      lcd.print("Xíu cắm lại ii");
    }
  }
}