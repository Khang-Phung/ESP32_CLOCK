

void handle_update() {
unsigned long currentMillis = millis();
    //////////////////////////Update firebase///////////////////////////////
    if (currentMillis - lastUpdateTime_firebase >= updateInterval_firebase) {
      lastUpdateTime_firebase = currentMillis;
      get_calendar_firebase();
      if (!Calendar_hientai_empty && (Gio < Gio_hientai || (Gio == Gio_hientai && Phut <= Phut_hientai))) {
  // Lịch hiện tại vẫn hợp lệ và chưa đến → chọn hiện tại
  Gio_temp = Gio_hientai;
  Phut_temp = Phut_hientai;
  Title_calendar_temp = Title_calendar_hientai;
} else if (!Calendar_tieptheo_empty) {
  // Lịch hiện tại hết hạn hoặc giả → chọn tiếp theo nếu có
  Gio_temp = Gio_tieptheo;
  Phut_temp = Phut_tieptheo;
  Title_calendar_temp = Title_calendar_tieptheo;
} else {
  // Không có lịch gì → dùng giả trị 25
  Gio_temp = 25;
  Phut_temp = 61;
  Title_calendar_temp = "Khong co lich";
}
    }
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

    //sensors.requestTemperatures(); 
    //temperatureC = sensors.getTempCByIndex(0);
}
