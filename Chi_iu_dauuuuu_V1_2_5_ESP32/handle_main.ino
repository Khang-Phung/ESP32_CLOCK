
void handle_main() {
  ///////////////////////////Main Program////////////////////////////
  unsigned long currentMillis = millis();
  if (Program_sel == 0) {


    if (enable_display == true) {
      lcd.clear();
    }

    if (currentMillis - lastUpdateTime >= updateInterval || enable_display == true) {
      lastUpdateTime = currentMillis;
      enable_display = false;
      char line1[17];
      char line2[17];
      sprintf(line1, " %s  %02d/%02d/%04d ", weekdays[Thu], dayVal, monthVal, yearVal);
      sprintf(line2, "%02d:%02d:%02d  %.1f"
                     "°C",
              Gio, Phut, Giay, temp);  // \xDF là ký tự độ °

      lcd.setCursor(0, 0);
      lcd.print(line1);
      lcd.setCursor(0, 1);
      lcd.print(line2);

      /*     lcd.setCursor(15, 0);
      if (rssi <= -80) {
        lcd.print("◼");  // 1 vạch Wi-Fi
      } else if (rssi <= -70) {
        lcd.print("▯");  // 2 vạch Wi-Fi
      } else {
        lcd.print("⬜");  // 3 vạch Wi-Fi
      }*/
      //lcd.setCursor(12, 1);
      //lcd.print("V1.2");
      // In ra Serial để debug
      Serial.print("Ngay: ");
      Serial.println(line1);

      Serial.print("Gio: ");
      Serial.println(line2);

      // Nếu không đang chờ thì bắt đầu yêu cầu đo
      if (!waitingForConversion) {
        sensor.requestTemperatures();     // Gửi lệnh đo (non-blocking)
        lastRequestTime = currentMillis;  // Ghi lại thời gian bắt đầu
        waitingForConversion = true;      // Đặt cờ đang đợi
      }

      // Nếu đang chờ, kiểm tra đủ thời gian chưa
      if (waitingForConversion && currentMillis - lastRequestTime >= conversionDelay) {
        temp = sensor.getTempCByIndex(0);  // Lấy nhiệt độ
        Serial.print("Nhiet do: ");
        Serial.println(temp);
        waitingForConversion = false;  // Đã xong, reset cờ
      }
    }
  }
  if (Program_sel == 1) {
    if (currentMillis - lastUpdateTime >= 1000 || enable_display == true) {
      lastUpdateTime = currentMillis;
      enable_display = false;
      lcd.clear();
      char timeStr[17];
      if (Gio_hientai != 25) {
        sprintf(timeStr, "%02d:%02d", Gio_hientai, Phut_hientai);
        lcd.setCursor(5, 0);
        lcd.print(timeStr);
        // ➕ Serial debug
        Serial.print("[LCD] Thoi gian: ");
        Serial.println(timeStr);
      } else {
        lcd.setCursor(5, 0);
        lcd.print("free");
        // ➕ Serial debug
        Serial.println("[LCD] free mode");
      }
      displaySmart(1, Title_calendar_hientai);
      // ➕ Serial debug
      Serial.print("[LED] Title: ");
      Serial.println(Title_calendar_hientai);
      //Last_Gio_hientai = Gio_hientai;
      //Last_Phut_hientai = Phut_hientai;
    }
  }

  if (Program_sel == 2) {

    if (enable_display == true)

    {
      lcd.print("█");  // bar 1
      lcd.print("▓");  // bar 2
      lcd.print("▒");  // bar 3
      lcd.print("░");  // bar 4
      lcd.print("▄");  // bar 5
      lcd.print("▌");  // bar 6
      lcd.print("▐");  // bar 7
      lcd.print("▀");  // bar 8
    }
    if (currentMillis - lastUpdateTime >= updateInterval || enable_display == true) {
      lastUpdateTime = currentMillis;
      enable_display = false;
      int Gio_chuc = Gio / 10;
      int Gio_dv = Gio % 10;
      int Phut_chuc = Phut / 10;
      int Phut_dv = Phut % 10;

      printNumber(Gio_chuc, 0);
      printNumber(Gio_dv, 3);
      lcd.setCursor(6, 0);
      lcd.print("  ");
      lcd.setCursor(6, 1);
      lcd.print("  ");
      lcd.setCursor(9, 0);
      lcd.print(" ");
      lcd.setCursor(9, 1);
      lcd.print(" ");
      printNumber(10, 8);
      printNumber(Phut_chuc, 10);
      printNumber(Phut_dv, 13);
    }
  }
}
