void playTwinkleTheme() {
  unsigned long now = millis();
  if (isPlayingTwinkle) {
    if (twinkleNote == 0 || now - noteStart >= TwinkleStar_duration[twinkleNote - 1]) {
      if (twinkleNote < sizeof(TwinkleStar) / sizeof(TwinkleStar[0])) {
        if (TwinkleStar[twinkleNote] > 0) {
          tone(SPEAKER_PIN, TwinkleStar[twinkleNote]);
          noteStart = now;
          twinkleNote++;
        } else {
          noTone(SPEAKER_PIN);
          noteStart = now;
          twinkleNote++;
        }
      } else {
        noTone(SPEAKER_PIN);
        isPlayingTwinkle = false;
      }
    }
  }
}
void playTingTingNonBlocking() {
  unsigned long now = millis();

  if (!tingPlaying) {
    tingPlaying = true;
    tingStep = 0;
    tingStart = now;
  }

  switch (tingStep) {
    case 0:
      tone(SPEAKER_PIN, 1500);  // Ting 1
      tingStart = now;
      tingStep = 1;
      Serial.print("[DEBUG] millis: ");
Serial.println(millis());
      break;
    case 1:
      if (now - tingStart >= 100) {
        noTone(SPEAKER_PIN);
        tingStart = now;
        tingStep = 2;
        Serial.print("[DEBUG] millis: ");
Serial.println(millis());
      }
      break;
    case 2:
      if (now - tingStart >= 100) {
        tone(SPEAKER_PIN, 1800);  // Ting 2
        tingStart = now;
        tingStep = 3;
        Serial.print("[DEBUG] millis: ");
Serial.println(millis());
      }
      break;
    case 3:
      if (now - tingStart >= 100) {
        noTone(SPEAKER_PIN);
        tingPlaying = false;  // Kết thúc
        tone_buzzed = true;
         Serial.print("[DEBUG] millis: ");
Serial.println(millis());
      }
      break;
  }
}

void playBeepBoopNonBlocking() {
  unsigned long now = millis();

  if (!tingPlaying) {
    tingPlaying = true;
    tingStep = 0;
    tingStart = now;
  }

  switch (tingStep) {
    case 0:
      tone(SPEAKER_PIN, 1000);  // Beep 1
      tingStart = now;
      tingStep = 1;
      break;
    case 1:
      if (now - tingStart >= 120) {
        noTone(SPEAKER_PIN);
        tingStart = now;
        tingStep = 2;
      }
      break;
    case 2:
      if (now - tingStart >= 80) {
        tone(SPEAKER_PIN, 1200);  // Beep 2
        tingStart = now;
        tingStep = 3;
      }
      break;
    case 3:
      if (now - tingStart >= 100) {
        noTone(SPEAKER_PIN);
        tingStart = now;
        tingStep = 4;
      }
      break;
    case 4:
      if (now - tingStart >= 80) {
        tone(SPEAKER_PIN, 900);  // Boop
        tingStart = now;
        tingStep = 5;
      }
      break;
    case 5:
      if (now - tingStart >= 150) {
        noTone(SPEAKER_PIN);
        tingPlaying = false;
        tone_buzzed = true;
      }
      break;
  }
}

void playButtonPressTone() {
  unsigned long now = millis();

  if (!tingPlaying) {
    tingPlaying = true;
    tingStep = 0;
    tingStart = now;
  }

  switch (tingStep) {
    case 0:
      tone(SPEAKER_PIN, 1500);  // Âm cao rõ ràng
      tingStart = now;
      tingStep = 1;
      break;
    case 1:
      if (now - tingStart >= 50) {
        noTone(SPEAKER_PIN);
        tingPlaying = false;
        tone_buzzed = true;
        sound_button = false;
      }
      break;
  }
}