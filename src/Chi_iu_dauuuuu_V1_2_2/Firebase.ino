void get_calendar_firebase() {
   Firebase.ready();  // <-- rất quan trọng
  // Gọi thử đọc dữ liệu từ Realtime Database
  if (Firebase.getString(fbdo, "/reminders/event0/time")) {
    Time_calendar_temp = fbdo.stringData();
    Gio_hientai = Time_calendar_temp.substring(0, Time_calendar_temp.indexOf(":")).toInt();
    Phut_hientai = Time_calendar_temp.substring(Time_calendar_temp.indexOf(":") + 1).toInt();
    Calendar_hientai_empty = false;
    if (Gio_hientai == 25) { Calendar_hientai_empty = true; }
  } else {
    Serial.print(" Lỗi Firebase: ");
    Serial.println(fbdo.errorReason());
    Gio_hientai = 25;
    Phut_hientai = 61;
    Calendar_hientai_empty = true;
  }
  if (Firebase.getString(fbdo, "/reminders/event1/time")) {

    Time_calendar_temp = fbdo.stringData();
    Gio_tieptheo = Time_calendar_temp.substring(0, Time_calendar_temp.indexOf(":")).toInt();
    Phut_tieptheo = Time_calendar_temp.substring(Time_calendar_temp.indexOf(":") + 1).toInt();
    Calendar_tieptheo_empty = false;
    if (Gio_tieptheo == 25) { Calendar_tieptheo_empty = true; }
  } else {
    Serial.print(" Lỗi Firebase: ");
    Serial.println(fbdo.errorReason());
    Gio_tieptheo = Gio_hientai;
    Phut_tieptheo = Phut_hientai;
    Calendar_tieptheo_empty = true;
  }
  if (Firebase.getString(fbdo, "/reminders/event0/title")) {
    Title_calendar_hientai = fbdo.stringData();
    //char myCharArray[Title_calendar.length() + 1]; // +1 cho ký tự null kết thúc
    //Title_calendar.toCharArray(myCharArray, sizeof(myCharArray));
  }
  if (Firebase.getString(fbdo, "/reminders/event1/title")) {
    Title_calendar_tieptheo = fbdo.stringData();
    //char myCharArray[Title_calendar.length() + 1]; // +1 cho ký tự null kết thúc
    //Title_calendar.toCharArray(myCharArray, sizeof(myCharArray));
  }
}

void syncTimeAndInitFirebase(FirebaseConfig &config, FirebaseAuth &auth) {
  // Cấu hình múi giờ VN (UTC+7), máy chủ NTP
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
                timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);

  // Khởi tạo Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}