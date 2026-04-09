/*void displaySmart(int row, String content) {
  static unsigned long scrollStart = 0;
  static int scrollPos = 0;
  static String lastContent = "";

  int charCount = getUTF8CharCount(content);

  // Nếu nội dung thay đổi → reset cuộn
  if (content != lastContent) {
    scrollStart = millis();
    scrollPos = 0;
    lastContent = content;
  }

  lcd.setCursor(0, row);
  lcd.print("                "); // clear line

  // Nếu nội dung ngắn hơn hoặc bằng 16 ký tự → canh giữa
  if (charCount <= 16) {
    int padding = (16 - charCount) / 2;
    lcd.setCursor(padding, row);
    lcd.print(content);
    return;
  }

  // Nếu nội dung dài hơn → cuộn
  if (millis() - scrollStart > 500) { // delay cuộn
    scrollStart = millis();

    String scrollStr = cutUTF8Substring(content, scrollPos, 16);
    lcd.setCursor(0, row);
    lcd.print(scrollStr);

    scrollPos++;
    if (scrollPos > charCount) {
      scrollPos = 0;
    }
  }
}



// Đếm số ký tự thực sự (Unicode)
int getUTF8CharCount(const String &str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if ((str[i] & 0xC0) != 0x80) count++; // byte đầu của UTF-8
  }
  return count;
}

// Cắt chuỗi Unicode từ vị trí `start` lấy `charLimit` ký tự
String cutUTF8Substring(const String &str, int start, int charLimit) {
  String result = "";
  int count = 0, realPos = 0, added = 0;

  // Di chuyển đến vị trí `start` theo ký tự (không phải byte)
  while (realPos < str.length() && count < start) {
    if ((str[realPos] & 0xC0) != 0x80) count++;
    realPos++;
  }

  // Lấy `charLimit` ký tự tiếp theo
  while (realPos < str.length() && added < charLimit) {
    if ((str[realPos] & 0xC0) != 0x80) added++;
    result += str[realPos];
    realPos++;
  }

  return result;
}*/
void displaySmart(int row, String content) {
  lcd.setCursor(0, row);
  lcd.print("                "); // Clear dòng

  int visibleLength = getUTF8CharCount(content);
  if (visibleLength <= 16) {
    int padding = (16 - visibleLength) / 2;
    lcd.setCursor(padding, row);
    lcd.print(content);
  } else {
    String trimmed = cutUTF8String(content, 14); // Giữ trọn 14 ký tự
    lcd.setCursor(0, row);
    lcd.print(trimmed + "..");
  }
}

// Đếm số ký tự Unicode (UTF-8)
int getUTF8CharCount(const String &str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if ((str[i] & 0xC0) != 0x80) count++; // đếm các byte đầu của ký tự UTF-8
  }
  return count;
}
String cutUTF8String(const String &str, int charLimit) {
  String result = "";
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if ((str[i] & 0xC0) != 0x80) count++;
    if (count > charLimit) break;
    result += str[i];
  }
  return result;
}


