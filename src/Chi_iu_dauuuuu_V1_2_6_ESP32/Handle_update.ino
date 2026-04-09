
 
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
        Serial.print("Nhiet do: ");
        Serial.println(temp);
        waitingForConversion = false;  // Đã xong, reset cờ
      }
}
