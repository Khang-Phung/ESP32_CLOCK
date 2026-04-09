enum PopupType { NONE, TIM, NGUNGON, REMINDER };
PopupType CurrentPopup = NONE;

void handle_popup()
{
  unsigned long currentMillis = millis();
  // ==================== POPUP: TIM ====================
  if (Show_Tim) {
    if (CurrentPopup != TIM) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cho C Iuuu timmm");
      lcd.setCursor(0, 1);
      lcd.print("Tim ne :");
      lcd.write(0);
      lcd.print("x");
      lcd.print(tim);
      CurrentPopup = TIM;
      tone_buzzed = false; // reset âm thanh mỗi popup mới
    }

    if (!tone_buzzed) {
      playTingTingNonBlocking();
    }

    if (currentMillis - DisplayStartTime > DisplayDuration) {
      Show_Tim = false;
      CurrentPopup = NONE;
      lcd.clear();
      tim = 0;
      ON_V1 = false;
      Blynk.virtualWrite(V10, ON_V1);
    }

    return;
  }

  // ==================== POPUP: NGUNGON ====================
  if (Show_ngungon) {
    if (CurrentPopup != NGUNGON) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Chi Iu ngu ngon");
      lcd.write(0);
      lcd.print("x");
      lcd.print(tim);
      CurrentPopup = NGUNGON;
      tone_buzzed = false;
    }

    if (!tone_buzzed) {
      playTwinkleTheme();
    }

    if (isPlayingTwinkle == false) {
      Show_ngungon = false;
      CurrentPopup = NONE;
      lcd.clear();
      ON_V2 = false;
      Blynk.virtualWrite(V10, ON_V2);
    }

    return;
  }

  // ==================== POPUP: REMINDER ====================
  if (Show_reminder) {
    if (CurrentPopup != REMINDER) {
      lcd.clear();
      tone_buzzed = false;
      char timeStr[17];
      lcd.setCursor(5, 0);
      if (Gio_hientai != 25) {
        sprintf(timeStr, "%02d:%02d", Gio_hientai, Phut_hientai);
        lcd.print(timeStr);
      } else {
        lcd.print("free");
      }
      CurrentPopup = REMINDER;
    }

    if (currentMillis - Lasttime_Scroll >= Scroll_duration) {
      Lasttime_Scroll = currentMillis;
      displaySmart(1, Title_calendar_hientai);
    }

    if (!tone_buzzed) {
      playBeepBoopNonBlocking();
    }

    if (currentMillis - DisplayStartTime > DisplayDuration_reminder) {
      Show_reminder = false;
      CurrentPopup = NONE;
      lcd.clear();
      notification = false;
      reminder_repeat = false;
    }

    return;
  }

  // ==================== KIỂM TRA NHẮC LỊCH LẦN ĐẦU ====================
  if (Gio == Gio_temp && Phut == Phut_temp && Giay == 0 && Calendar_hientai_empty == 0) {
    reminder_repeat_start = millis();
    reminder();
  }

  // ==================== NHẮC LỊCH LẶP LẠI SAU MỖI 15 PHÚT ====================
  // Chỉ khi giờ hiện tại đã qua thời gian nhắc chính thức
  if ((Gio > Gio_hientai || (Gio == Gio_hientai && Phut >= Phut_hientai)) && Calendar_hientai_empty == 0) {
    if (currentMillis - reminder_repeat_start > reminderDuration_repeat) {
      reminder_repeat_start = millis();
      reminder_repeat = true;
      reminder();
    }
  }
}
