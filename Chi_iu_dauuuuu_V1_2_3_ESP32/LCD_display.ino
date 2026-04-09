
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


