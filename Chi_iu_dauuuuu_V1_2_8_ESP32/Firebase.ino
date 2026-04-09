void get_calendar_firebase() {
   switch (fbStep) {
    case FB_IDLE:
      if (millis() - lastUpdateTime_firebase >= updateInterval_firebase && Firebase.ready()) {
        fbStep = FB_GET_EVENT0_TIME;
      }
      break;

    case FB_GET_EVENT0_TIME:
      if (Firebase.getString(fbdo, "/reminders/event0/time")) {
        Time_calendar_temp = fbdo.stringData();
        Gio_hientai = Time_calendar_temp.substring(0, Time_calendar_temp.indexOf(":")).toInt();
        Phut_hientai = Time_calendar_temp.substring(Time_calendar_temp.indexOf(":") + 1).toInt();
        Calendar_hientai_empty = (Gio_hientai == 25);
      } else {
        Gio_hientai = 25;
        Phut_hientai = 61;
        Calendar_hientai_empty = true;
      }
      fbStep = FB_GET_EVENT1_TIME;
      break;

    case FB_GET_EVENT1_TIME:
      if (Firebase.getString(fbdo, "/reminders/event1/time")) {
        Time_calendar_temp = fbdo.stringData();
        Gio_tieptheo = Time_calendar_temp.substring(0, Time_calendar_temp.indexOf(":")).toInt();
        Phut_tieptheo = Time_calendar_temp.substring(Time_calendar_temp.indexOf(":") + 1).toInt();
        Calendar_tieptheo_empty = (Gio_tieptheo == 25);
      } else {
        Gio_tieptheo = Gio_hientai;
        Phut_tieptheo = Phut_hientai;
        Calendar_tieptheo_empty = true;
      }
      fbStep = FB_GET_EVENT0_TITLE;
      break;

    case FB_GET_EVENT0_TITLE:
      if (Firebase.getString(fbdo, "/reminders/event0/title")) {
        Title_calendar_hientai = fbdo.stringData();
      }
      fbStep = FB_GET_EVENT1_TITLE;
      break;

    case FB_GET_EVENT1_TITLE:
      if (Firebase.getString(fbdo, "/reminders/event1/title")) {
        Title_calendar_tieptheo = fbdo.stringData();
      }
      fbStep = FB_DONE;
      break;

    case FB_DONE:
      lastUpdateTime_firebase = millis();
      fbStep = FB_IDLE;

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
      break;
  }
}

void get_weather_firebase() {
  if (!Firebase.ready()) return;

  if (Firebase.getFloat(fbdo, "/weather/outdoor_temp")) {
    outdoor_temp = fbdo.floatData();
  }

  if (Firebase.getInt(fbdo, "/weather/humidity")) {
    outdoor_humidity = fbdo.intData();
  }

  if (Firebase.getString(fbdo, "/weather/weather_desc")) {
    weather_desc = fbdo.stringData();
  }
}


void syncTimeAndInitFirebase(FirebaseConfig &config, FirebaseAuth &auth) {
/*  // Cấu hình múi giờ VN (UTC+7), máy chủ NTP
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // Đợi đến khi hệ thống có thời gian hợp lệ
  Serial.print("⏳ Đang đồng bộ NTP...");
  time_t now = time(nullptr);
  while (now < 100000) {  // Dưới ngưỡng này xem như chưa có giờ
    delay(200);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("\n✅ Đồng bộ NTP thành công!");
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  Serial.printf("⏰ Giờ hệ thống: %02d:%02d:%02d %02d/%02d/%04d\n", 
                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, 
                timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);*/

  // Khởi tạo Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}