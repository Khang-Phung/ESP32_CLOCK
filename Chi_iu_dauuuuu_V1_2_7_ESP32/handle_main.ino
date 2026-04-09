
void handle_main() {
  ///////////////////////////Main Program////////////////////////////
  unsigned long currentMillis = millis();
  if (Program_sel == 0) {


    if (enable_display == true||first_time==true) {
      first_time=false;
      lcd.clear();
    }

    if (currentMillis - lastUpdateTime >= updateInterval || enable_display == true) {
      lastUpdateTime = currentMillis;
      enable_display = false;
      char line1[17];
      char line2[17];
      sprintf(line1, "%s   %02d/%02d/%04d", weekdays[Thu], dayVal, monthVal, yearVal);
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
      //Serial.print("Ngay: ");
      //Serial.println(line1);

      //Serial.print("Gio: ");
      //Serial.println(line2);
      get_temp(currentMillis);
    }
  }
  if (Program_sel == 1) {
    if (currentMillis - lastUpdateTime >= 1000 || enable_display == true||first_time==true) {
      lastUpdateTime = currentMillis;
      enable_display = false;
      first_time=false;
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

    if (enable_display == true||first_time==true)

    { first_time=false;
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

  if (Program_sel == 3) {

    if (enable_display == true||first_time==true)
    {
      lcd.clear();
       lcd.createChar(0, House);
       lcd.createChar(1, City);
       lcd.createChar(2, Humidity);
       lcd.createChar(3, Degree);
       lcd.createChar(4, Rain);
       lcd.createChar(5, Cloud);
       lcd.createChar(6, Sun);
       lcd.createChar(7, Moon);
       first_time=false;

        // Sau khi cả hai có dữ liệu, hiển thị:
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.print(temp);  // Có thể thêm 1 số lẻ nếu thích
      lcd.write(byte(3));
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.write(byte(1));
      lcd.print(outdoor_temp);
      lcd.write(byte(3));
      lcd.print("C");
      lcd.setCursor(11, 1);
      
      lcd.write(byte(2));
      lcd.print(outdoor_humidity);
      lcd.print("%");

      lcd.setCursor(11, 0);
      //String symbol = getWeatherSymbol(weather_desc);  // Trả về 🌧
      getWeatherSymbol(weather_desc);
    }
    if (currentMillis - last_weather >= 30000|| enable_display == true) {
      last_weather = currentMillis;
      enable_display = false;
      get_weather_firebase();
      get_temp(currentMillis);
      // Sau khi cả hai có dữ liệu, hiển thị:
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.print(temp);  // Có thể thêm 1 số lẻ nếu thích
      lcd.write(byte(3));
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.write(byte(1));
      lcd.print(outdoor_temp);
      lcd.write(byte(3));
      lcd.print("C");
      lcd.setCursor(11, 1);
      
      lcd.write(byte(2));
      lcd.print(outdoor_humidity);
      lcd.print("%");

      lcd.setCursor(11, 0);
      //String symbol = getWeatherSymbol(weather_desc);  // Trả về 🌧
      getWeatherSymbol(weather_desc);
      // Debug ra Serial nếu muốn
      Serial.print("Nhiet do trong: ");
      Serial.println(temp);
      Serial.print("Nhiet do ngoai: ");
      Serial.println(outdoor_temp);
    }
  }
}
