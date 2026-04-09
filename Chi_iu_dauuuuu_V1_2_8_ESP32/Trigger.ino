
BLYNK_WRITE(V1) {  // khi có sự kiện kích hoạt hiển thị pop up
  int value = param.asInt();
  if (value == 1 && (CurrentPopup == NONE || CurrentPopup == TIM)) {
    if (CurrentPopup == TIM) {
      CurrentPopup = NONE; // reset trạng thái tim lại
      tone_buzzed = false; // reset âm thanh đang phát
    }
    Show_Tim = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tim = tim + 1;
    Serial.print(tim);
    tone_buzzed = false;
    ON_V1 = true;
    enable_display = true;
    Blynk.virtualWrite(V10, ON_V1);
  }
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  if (value == 1 && CurrentPopup == NONE) {
    Show_ngungon = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    isPlayingTwinkle = true;
    twinkleNote = 0;
    noteStart = millis();
    tone_buzzed = false;
    ON_V2 = true;
    enable_display = true;
  }
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  if (value == 1 && (CurrentPopup == NONE || CurrentPopup == HUN)) {
    if (CurrentPopup == HUN && Lip_status==2) {
      Lip_status = 1;
      CurrentPopup = NONE;
      tone_buzzed = false;
    }
    Show_hun = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tone_buzzed = false;
    ON_V3 = true;
    enable_display = true;
    //Button_press = true;
  }

}

void reminder() {
  if (CurrentPopup == NONE) {
    Show_reminder = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tone_buzzed = false;
    notification = true;
    enable_display = true;
    Giay = Giay + 1;  // điều kiện để tránh lặp lại
  }

  if (Calendar_tieptheo_empty) {
    Gio_temp = 25;
    Phut_temp = 61;
    return;
  }

  if(reminder_repeat != true){
  Gio_hientai = Gio_temp;
  Phut_hientai = Phut_temp;
  Gio_temp = Gio_tieptheo;
  Phut_temp = Phut_tieptheo;
  Title_calendar_hientai = Title_calendar_temp;
  Title_calendar_temp = Title_calendar_tieptheo;
  }
}

void khang_reminder() {
  if (CurrentPopup == NONE) {
    Show_khang_reminder = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tone_buzzed = false;
    enable_display = true;
    Giay = Giay + 1;  // điều kiện để tránh lặp lại
  }
}