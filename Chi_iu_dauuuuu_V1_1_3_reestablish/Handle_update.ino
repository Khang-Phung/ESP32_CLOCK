

void handle_update() {
unsigned long currentMillis = millis();
    //////////////////////////Update firebase///////////////////////////////
    if (currentMillis - lastUpdateTime_firebase >= updateInterval_firebase) {
      lastUpdateTime_firebase = currentMillis;
      get_calendar_firebase();
      if (Gio < Gio_hientai || (Gio == Gio_hientai && Phut <= Phut_hientai)) {
        Gio_temp = Gio_hientai;
        Phut_temp = Phut_hientai;
        Title_calendar_temp = Title_calendar_hientai;
      } else {
        Gio_temp = Gio_tieptheo;
        Phut_temp = Phut_tieptheo;
        Title_calendar_temp = Title_calendar_tieptheo;
      }
    }
    //////////////////////////Update blynk///////////////////////////////
    if (currentMillis - lastUpdateTime_blynk >= updateInterval_blynk) {
      lastUpdateTime_blynk = currentMillis;
      Blynk.virtualWrite(V0, rssi);
      Serial.print(Gio_hientai);
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
      Serial.println(notification);
    }
}
