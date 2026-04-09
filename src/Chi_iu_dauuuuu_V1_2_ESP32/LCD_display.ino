// Ham hien thi chuoi voi logic can giua neu ngan, cuon neu dai
void displaySmart(int row, String text) {
  if (text.length() <= 16) {
    // --- Căn giữa ---
    int startCol = (16 - text.length()) / 2;
    lcd.setCursor(0, row);
    lcd.print("                ");  // Xóa dòng
    lcd.setCursor(startCol, row);
    lcd.print(text);
  } else {
    // --- Cuộn ---
    String subText = text.substring(scrollIndex);
    if (subText.length() > 16) {
      subText = subText.substring(0, 16);
    }
    lcd.setCursor(0, row);
    lcd.print("                ");  // Xóa dòng
    lcd.setCursor(0, row);
    lcd.print(subText);

    scrollIndex++;
    if (scrollIndex > text.length()) {
      scrollIndex = 0;
    }
  }
}