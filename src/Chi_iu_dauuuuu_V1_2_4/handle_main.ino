
void handle_main() {
  ///////////////////////////Main Program////////////////////////////
  unsigned long currentMillis = millis();
  if (Program_sel == 0) {


       if (enable_display == true)
    {
      lcd.clear();
    }

    if (currentMillis - lastUpdateTime >= updateInterval || enable_display == true) {
      lastUpdateTime = currentMillis;
      enable_display = false;
      char dateStr[17];
      char timeStr[17];
      sprintf(dateStr, "%02d/%02d/%04d", dayVal, monthVal, yearVal);
      sprintf(timeStr, "%02d:%02d:%02d", Gio, Phut, Giay);
      lcd.setCursor(0, 0);
      //lcd.print(temperatureC);
      lcd.setCursor(3, 0);
      lcd.print(dateStr);
      lcd.setCursor(4, 1);
      lcd.print(timeStr);
      lcd.setCursor(15, 0);
      if (rssi <= -80) {
        lcd.print("◼");  // 1 vạch Wi-Fi
      } else if (rssi <= -70) {
        lcd.print("▯");  // 2 vạch Wi-Fi
      } else {
        lcd.print("⬜");  // 3 vạch Wi-Fi
      }
      //lcd.setCursor(12, 1);
      //lcd.print("V1.2");
    }
  }
  if (Program_sel == 1) {
    if (Gio_hientai != Last_Gio_hientai && Phut_hientai != Last_Phut_hientai || enable_display == true) {
      enable_display = false;
      lcd.clear();
      char timeStr[17];
      if (Gio_hientai != 25) {
        sprintf(timeStr, "%02d:%02d", Gio_hientai, Phut_hientai);
        lcd.setCursor(5, 0);
        lcd.print(timeStr);
      } else {
        lcd.setCursor(5, 0);
        lcd.print("free");
      }
      displaySmart(1, Title_calendar_hientai);
      Last_Gio_hientai = Gio_hientai;
      Last_Phut_hientai = Phut_hientai;
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
