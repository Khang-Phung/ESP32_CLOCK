
BLYNK_WRITE(V1) {  // khi có sự kiện kích hoạt hiển thị pop up
  int value = param.asInt();
  if (value == 1 && ON_V2 != 1 && ON_V3 != 1 && notification != 1) {
    if (ON_V1) {
      CurrentPopup = NONE;
      tone_buzzed = false;
    }
    Show_Tim = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tim = tim + 1;
    Serial.print(tim);
    tone_buzzed = false;
    ON_V1 = true;
    Blynk.virtualWrite(V10, ON_V1);
  }
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  if (value == 1 && ON_V1 != 1 && ON_V3 != 1 && notification != 1) {
    if (ON_V2) {
      CurrentPopup = NONE;
      tone_buzzed = false;
    }
    Show_ngungon = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    isPlayingTwinkle = true;
    twinkleNote = 0;
    noteStart = millis();
    tone_buzzed = false;
    ON_V2 = true;
  }
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  if (value == 1 && ON_V2 != 1 && ON_V3 != 1 && notification != 1) {
   /* Show_quane = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tone_buzzed = false;
    ON_V3 = true;*/
    Button_press = true;
  }

}

void reminder() {
  if (ON_V1 != 1 && ON_V2 != 1 && ON_V3 != 1) {
    Show_reminder = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tone_buzzed = false;
    notification = true;
    Giay = Giay + 1;  // điều kiện để tránh lặp lại
  }

  if (Calendar_tieptheo_empty) {
    Gio_temp = 26;
    Phut_temp = 61;
    return;
  }

  if(reminder_repeat != 1){
  Gio_hientai = Gio_temp;
  Phut_hientai = Phut_temp;
  Gio_temp = Gio_tieptheo;
  Phut_temp = Phut_tieptheo;
  Title_calendar_hientai = Title_calendar_temp;
  Title_calendar_temp = Title_calendar_tieptheo;
  }
}