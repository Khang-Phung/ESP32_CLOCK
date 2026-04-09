
void handle_popup() {
  unsigned long currentMillis = millis();
  // ==================== POPUP: TIM ====================
  if (Show_Tim) {
    if (CurrentPopup != TIM) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cho Chị Iu timmm");
      lcd.setCursor(0, 1);
      lcd.print("Tim nè ❤x");
      lcd.print(tim);
      CurrentPopup = TIM;
      tone_buzzed = false;  // reset âm thanh mỗi popup mới
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
  }

  // ==================== POPUP: NGUNGON ====================
  if (Show_ngungon) {
    if (CurrentPopup != NGUNGON) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Chị Iu ngủ ngon❤");
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
  }

  // ==================== POPUP: Hun ====================
  if (Show_hun) {
    if (CurrentPopup != HUN) {
      Lip_status = 1;
      pretime_lip1 = currentMillis;
      CurrentPopup = HUN;
      tone_buzzed = false;  // reset âm thanh mỗi popup mới

      LIP2();
    }

    if (Lip_status == 1) {
      if (currentMillis - pretime_lip1 >= 350) {
        // pretime_lip2 = currentMillis;
        Lip_status = 2;
        LIP1();
      }
    }



    if (!tone_buzzed) {
      playTingTingNonBlocking();
    }

    if (currentMillis - DisplayStartTime > DisplayDuration) {
      Show_hun = false;
      CurrentPopup = NONE;
      lcd.clear();
      ON_V3 = false;
    }
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
  }
  ////

  // ==================== POPUP: KHANG_REMINDER====================

  const char* messages1[] = {
    "Sáng sớm òi❤",      // MORNING
    "Tới giờ trưa òi❤",  // NOON
    "Chiều òi á❤",       // EVENING
    "Chuẩn bị ngủ nè❤",  // NIGHT
  };

  const char* messages2[] = {
    "Chị Iu dậy sớm i",  // MORNING
    "Ăn trưa ii nè",     // NOON
    "Nghỉ ngơi chút i",  // EVENING
    "Ngủ ngonnnn",       // NIGHT
  };
  const char* messages3[] = {
    "Hôm nay nóng he",  // MORNING
    "Nóng quá ò",       // NOON
    "Uống nước nhe",    // EVENING
    "Tối mà nóng quó",  // NIGHT
  };
  const char* messages4[] = {
    "Se lanh se lạnh",  // MORNING
    "Lạnhhh",           // NOON
    "Chìu nay lạnh",    // EVENING
    "Đắp mền dô ii",    // NIGHT
  };
  if (Show_khang_reminder) {
    if (CurrentPopup != KHANG_REMINDER) {  // chỉ in nếu là mốc hợp lệ

      lcd.clear();
      lcd.setCursor(0, 0);
      // ///////thông điệp dòng 1 cố định
      lcd.print(messages1[currentPeriod]);  // Lấy thông điệp từ mảng
      lcd.setCursor(0, 1);

      // ///////thông điệp dòng 2
      if (outdoor_temp > 33.3) {
        lcd.print(messages3[currentPeriod]);  // nóng
      } else if (outdoor_temp > 25) {
        lcd.print(messages2[currentPeriod]);  // Btg
      } else {
        lcd.print(messages4[currentPeriod]);  // Lạnh
      }


      CurrentPopup = KHANG_REMINDER;
      tone_buzzed = false;
    }

    if (!tone_buzzed) {
      playBeepBoopNonBlocking();
    }

    if (currentMillis - DisplayStartTime > DisplayDuration_reminder) {
      Show_khang_reminder = false;
      CurrentPopup = NONE;
      currentPeriod = NONE_TIME;
      lcd.clear();
    }
  }


  // ==================== Nhắc nhở cá nhân ====================
  if (Giay == 0) {
    if (Gio == 7 && Phut == 0) {
      currentPeriod = MORNING;
    } else if (Gio == 11 && Phut == 30) {
      currentPeriod = NOON;
    } else if (Gio == 17 && Phut == 30) {
      currentPeriod = EVENING;
    } else if (Gio == 23 && Phut == 30) {
      currentPeriod = NIGHT;
    }
  }
  if (currentPeriod != NONE_TIME && Show_khang_reminder == false) {
    khang_reminder();
  }

  // ==================== KIỂM TRA NHẮC LỊCH LẦN ĐẦU ====================
  if (Gio == Gio_temp && Phut == Phut_temp && Giay == 0 && Calendar_hientai_empty == 0) {
    reminder_repeat_start = millis();
    reminder_repeat = false;
    reminder();
  }

  // ==================== NHẮC LỊCH LẶP LẠI SAU MỖI 15 PHÚT ====================
  // Chỉ khi giờ hiện tại đã qua thời gian nhắc chính thức
  if (!Calendar_hientai_empty && (Gio > Gio_hientai || (Gio == Gio_hientai && Phut >= Phut_hientai))) {
    if (currentMillis - reminder_repeat_start > reminderDuration_repeat) {
      reminder_repeat_start = millis();
      reminder_repeat = true;
      reminder();
    }
  }
}
