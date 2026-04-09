void checkButton(uint8_t pin, void (*onPress)(), void (*onHold)(), unsigned long holdThreshold = 1000) {
  static bool lastState = HIGH;
  static unsigned long lastDebounce = 0;
  static unsigned long pressTime = 0;
  static bool waitingRelease = false;

  bool reading = digitalRead(pin);

  if (reading != lastState) {
    lastDebounce = millis();
  }

  if (millis() - lastDebounce > 50) {
    if (!waitingRelease && lastState == HIGH && reading == LOW) {
      pressTime = millis();
      waitingRelease = true;
    }

    if (waitingRelease && lastState == LOW && reading == HIGH) {
      unsigned long duration = millis() - pressTime;
      waitingRelease = false;

      if (duration >= holdThreshold) {
        onHold();
      } else {
        onPress();
      }
    }
  }

  lastState = reading;
}

void onPress() {
  PRESS = true;  // cờ báo có nút nhấn
  // lcd.noBacklight(); hoặc lcd.backlight();
}

void onHold() {
  HOLD = true;  // cờ báo có nút giữ
}


bool debounceButton(uint8_t pin) {
  static uint32_t lastDebounceTime = 0;
  static bool lastButtonState = HIGH;
  static bool buttonPressed = false;
  const uint16_t debounceDelay = 50;  // ms

  bool reading = digitalRead(pin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && !buttonPressed) {
      buttonPressed = true;
      lastButtonState = reading;
      return true;  // Có nhấn mới
    } else if (reading == HIGH) {
      buttonPressed = false;
    }
  }

  lastButtonState = reading;
  return false;  // Không nhấn mới
}