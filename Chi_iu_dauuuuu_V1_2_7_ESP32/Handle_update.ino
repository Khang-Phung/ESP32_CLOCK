
 
void handle_update() {
unsigned long currentMillis = millis();
    //////////////////////////Update firebase///////////////////////////////
    get_calendar_firebase();
    //////////////////////////Update blynk///////////////////////////////
    if (currentMillis - lastUpdateTime_blynk >= updateInterval_blynk) {
      lastUpdateTime_blynk = currentMillis;
      Blynk.virtualWrite(V0, rssi);
      /*Serial.print(Gio_hientai);
      Serial.print(":");
      Serial.println(Phut_hientai);
      Serial.print(Gio_tieptheo);
      Serial.print(":");
      Serial.println(Phut_tieptheo);
      Serial.print(Gio_temp);
      Serial.print(":");
      Serial.println(Phut_temp);
      Serial.print(Calendar_hientai_empty);
      Serial.print(",");
      Serial.print(Show_reminder);
      Serial.print(",");
      Serial.println(notification);*/
    }
}




void get_temp(unsigned long currentMillis) {
// Nếu không đang chờ thì bắt đầu yêu cầu đo
      if (!waitingForConversion) {
        sensor.requestTemperatures();     // Gửi lệnh đo (non-blocking)
        lastRequestTime = currentMillis;  // Ghi lại thời gian bắt đầu
        waitingForConversion = true;      // Đặt cờ đang đợi
      }

      // Nếu đang chờ, kiểm tra đủ thời gian chưa
      if (waitingForConversion && currentMillis - lastRequestTime >= conversionDelay) {
        temp = sensor.getTempCByIndex(0);  // Lấy nhiệt độ
        //Serial.print("Nhiet do: ");
      //  Serial.println(temp);
        waitingForConversion = false;  // Đã xong, reset cờ
      }
}

void resetVariables() {
  // Biến message
  Show_Tim = false;
  Show_ngungon = false;
  Show_hun = false;
  Show_reminder = false;

  // Hiển thị và âm thanh
  LCD_Displayed = false;
  tone_buzzed = true;
  Lip_status = 0;
  showLIP1 = true;
  showLIP2 = true;
  backlightState = true;

  // Biến led ảo
  ON_V1 = false;  // V10
  ON_V2 = false;  // V11
  ON_V3 = false;  // V12
  notification = false;

  // Âm thanh và nhạc
  tingStep = 0;
  tingPlaying = false;
  reminder_repeat = true;

  noteStart = 0;
  twinkleNote = 0;
  isPlayingTwinkle = false;

  // Biến nút vật lý
  /*PRESS = false;
  HOLD = false;
  last_saved_Backlight = false;
  Button_press = false;*/

  enable_display = false;
  sound_button = false;
  

  

  // Popup
  CurrentPopup = NONE;
}


void time_sync() {
  unsigned long now = millis();
  if (!timeSynced) {
    // Chưa sync thành công -> thử mỗi 2 giây
    if (now - lastNTPAttempt >= syncInterval) {
      lastNTPAttempt = now;
      unsigned long t1 = millis();
      if (timeClient.update()) {
        Serial.printf("[DEBUG] NTP Sync OK, mất %lu ms\n", millis() - t1);
        timeSynced = true;
        syncInterval = SYNC_EVERY_24H; // Lần sau sync 24h/lần
      } else {
        Serial.printf("[DEBUG] NTP Sync failed sau %lu ms, retry in 2s...\n", millis() - t1);
      }
    }
  } else {
    // Đã sync -> mỗi 24h sync 1 lần
    if (now - lastNTPAttempt >= syncInterval) {
      lastNTPAttempt = now;
      unsigned long t1 = millis();
      if (timeClient.update()) {
        Serial.printf("[DEBUG] NTP re-sync OK, mất %lu ms\n", millis() - t1);
      } else {
        Serial.printf("[DEBUG] NTP re-sync failed sau %lu ms, switching to fast retry\n", millis() - t1);
        timeSynced = false; // Quay lại chế độ thử nhanh 2s
        syncInterval = 2000;
      }
    }
  }

  unsigned long t1 = millis();
  time_t rawTime = timeClient.getEpochTime();
  Gio = hour(rawTime);
  Phut = minute(rawTime);
  Giay = second(rawTime);
  Thu = weekday(rawTime);
  dayVal = day(rawTime);
  monthVal = month(rawTime);
  yearVal = year(rawTime);
  rssi = WiFi.RSSI();
  Serial.printf("[DEBUG]timeClient.getEpochTime() mất %lu ms\n", millis() - t1);
  Serial.println("[DEBUG] Cập nhật biến thời gian & RSSI");
}


