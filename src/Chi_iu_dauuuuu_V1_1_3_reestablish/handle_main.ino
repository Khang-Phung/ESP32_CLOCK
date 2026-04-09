
void handle_main() {
   ///////////////////////////Main Program////////////////////////////
   unsigned long currentMillis = millis();
    if (Program_sel == 0) {

      if (currentMillis - lastUpdateTime >= updateInterval || enable_display == true) {
        lastUpdateTime = currentMillis;
        enable_display = false;
        time_t rawTime = timeClient.getEpochTime();
        Gio = hour(rawTime);
        Phut = minute(rawTime);
        Giay = second(rawTime);
        dayVal = day(rawTime);
        monthVal = month(rawTime);
        yearVal = year(rawTime);

        char dateStr[17];
        char timeStr[17];
        sprintf(dateStr, "%02d/%02d/%04d", dayVal, monthVal, yearVal);
        sprintf(timeStr, "%02d:%02d:%02d", Gio, Phut, Giay);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print(dateStr);
        lcd.setCursor(4, 1);
        lcd.print(timeStr);
        lcd.setCursor(15, 0);
        if (rssi <= -80) {
          lcd.write(1);  // 1 vạch Wi-Fi
        } else if (rssi <= -70) {
          lcd.write(2);  // 2 vạch Wi-Fi
        } else {
          lcd.write(3);  // 3 vạch Wi-Fi
        }
      }
    }
    if (Program_sel == 1) {
      if (Gio_hientai != Last_Gio_hientai && Phut_hientai != Last_Phut_hientai || enable_display == true) {
        enable_display = false;
        lcd.clear();
        char timeStr[17];
         if(Gio_hientai != 25){
        sprintf(timeStr, "%02d:%02d", Gio_hientai, Phut_hientai);
        lcd.setCursor(5, 0);
        lcd.print(timeStr);}
        else {lcd.setCursor(5, 0);
        lcd.print("free");}
        if (currentMillis - Lasttime_Scroll >= Scroll_duration) {
          Lasttime_Scroll = currentMillis;
          displaySmart(1, Title_calendar_hientai);
        }
        Last_Gio_hientai = Gio_hientai;
        Last_Phut_hientai = Phut_hientai;
      }
    }
}
