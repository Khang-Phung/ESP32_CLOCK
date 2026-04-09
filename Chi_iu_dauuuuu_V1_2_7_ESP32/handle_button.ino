void handle_button() {
  ButtonState state = checkButton(BUTTON_PIN);

  if (Serial.available() > 0) {  // Nếu có dữ liệu được gửi đến
    char key = Serial.read();    // Đọc ký tự từ Serial

    if (key == 'a') {
      state = BUTTON_PRESSED;
    }
  }

  if (state == BUTTON_PRESSED) {
    // Nhấn nhanh
    Serial.println("Nút được nhấn nhanh (press)");
    Program_sel++;
    if (Program_sel > 3) Program_sel = 0;
    enable_display = true;
    sound_button = true;
  } else if (state == BUTTON_HELD) {
    // Giữ nút lâu
    Serial.println("Nút được giữ lâu (hold)");
    backlightState = !backlightState;

    if (backlightState) {
      lcd.backlight();
    } else {
      lcd.noBacklight();
    }
    enable_display = true;
    sound_button = true;
  }

  bool shouldSave = false;


  if (Program_sel != last_saved_program) {
    shouldSave = true;
  }

  if (backlightState != last_saved_Backlight) {
    shouldSave = true;
  }

  if (shouldSave) {
    prefs.begin("config", false);  // mở 1 lần

    if (Program_sel != last_saved_program) {
      prefs.putInt("Program_sel", Program_sel);
      last_saved_program = Program_sel;
    }

    if (backlightState != last_saved_Backlight) {
      prefs.putBool("backlightState", backlightState);  // dùng putBool cho đúng kiểu
      last_saved_Backlight = backlightState;
    }

    prefs.end();  // đóng 1 lần
  }

  if (sound_button) {
    playButtonPressTone();
  }
}


ButtonState checkButton(uint8_t pin) {
  static bool lastReading = HIGH;
  static bool stableState = HIGH;
  static uint32_t lastDebounceTime = 0;
  static uint32_t pressStartTime = 0;
  static bool holdTriggered = false;

  const uint16_t debounceDelay = 50;
  const uint16_t holdDelay = 500;

  bool reading = digitalRead(pin);

  // Debounce
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > debounceDelay) {
    if (reading != stableState) {
      stableState = reading;

      if (stableState == LOW) {
        // Bắt đầu nhấn
        pressStartTime = millis();
        holdTriggered = false;
      } else {
        // Nút được thả ra
        if (!holdTriggered) {
          return BUTTON_PRESSED;
        }
      }
    }

    // Nếu đang giữ và đủ lâu → hold
    if (stableState == LOW && !holdTriggered && (millis() - pressStartTime >= holdDelay)) {
      holdTriggered = true;
      return BUTTON_HELD;
    }
  }

  lastReading = reading;
  return BUTTON_NONE;
}