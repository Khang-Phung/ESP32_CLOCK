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