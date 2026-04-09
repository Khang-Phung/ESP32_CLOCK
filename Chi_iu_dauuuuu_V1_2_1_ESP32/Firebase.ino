void get_calendar_firebase() {
  // Gọi thử đọc dữ liệu từ Realtime Database
  if (Firebase.getString(fbdo, "/reminders/event0/time")) {
    Time_calendar_temp = fbdo.stringData();
    Gio_hientai = Time_calendar_temp.substring(0, Time_calendar_temp.indexOf(":")).toInt();
    Phut_hientai = Time_calendar_temp.substring(Time_calendar_temp.indexOf(":") + 1).toInt();
    Calendar_hientai_empty = false;
    if (Gio_hientai == 25) { Calendar_hientai_empty = true; }
  } /*else {
    Serial.print(" Lỗi Firebase: ");
    //Serial.println(fbdo.errorReason());
    Gio_hientai = 25;
    Phut_hientai = 61;
    Calendar_hientai_empty = true;
  }*/
  if (Firebase.getString(fbdo, "/reminders/event1/time")) {

    Time_calendar_temp = fbdo.stringData();
    Gio_tieptheo = Time_calendar_temp.substring(0, Time_calendar_temp.indexOf(":")).toInt();
    Phut_tieptheo = Time_calendar_temp.substring(Time_calendar_temp.indexOf(":") + 1).toInt();
    Calendar_tieptheo_empty = false;
    if (Gio_tieptheo == 25) { Calendar_tieptheo_empty = true; }
  } /*else {
    Serial.print(" Lỗi Firebase: ");
    //Serial.println(fbdo.errorReason());
    Gio_tieptheo = Gio_hientai;
    Phut_tieptheo = Phut_hientai;
    Calendar_tieptheo_empty = true;
  }*/
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