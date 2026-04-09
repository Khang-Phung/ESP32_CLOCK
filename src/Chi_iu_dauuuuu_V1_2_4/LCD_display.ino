
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


void LIP1(){
  lcd.createChar(0, name0x6);
  lcd.createChar(1, name0x7);
  lcd.createChar(2, name0x8);
  lcd.createChar(3, name0x9);
  lcd.createChar(4, name1x6);
  lcd.createChar(5, name1x7);
  lcd.createChar(6, name1x8);
  lcd.createChar(7, name1x9);

  // Vẽ hàng trên (row 0)
lcd.setCursor(0, 0);
lcd.print("      ");         // đệm trái
lcd.write(byte(0));          // 
lcd.write(byte(1));          // 
lcd.write(byte(2));          // 
lcd.write(byte(3));          // 
lcd.print("      ");         // đệm phải

lcd.setCursor(0, 1);
lcd.print("      ");         // đệm trái
lcd.write(byte(4));          // 
lcd.write(byte(5));          // 
lcd.write(byte(6));          // 
lcd.write(byte(7));          // 
lcd.print("      ");         // đệm phải
}

void LIP2(){
  lcd.createChar(0, Bname0x6);
  lcd.createChar(1, Bname0x7);
  lcd.createChar(2, Bname0x8);
  lcd.createChar(3, Bname0x9);
  lcd.createChar(4, Bname1x6);
  lcd.createChar(5, Bname1x7);
  lcd.createChar(6, Bname1x8);
  lcd.createChar(7, Bname1x9);

   // Vẽ hàng trên (row 0)
lcd.setCursor(0, 0);
lcd.print("      ");         // đệm trái
lcd.write(byte(0));          // 
lcd.write(byte(1));          // 
lcd.write(byte(2));          // 
lcd.write(byte(3));          // 
lcd.print("      ");         // đệm phải

lcd.setCursor(0, 1);
lcd.print("      ");         // đệm trái
lcd.write(byte(4));          // 
lcd.write(byte(5));          // 
lcd.write(byte(6));          // 
lcd.write(byte(7));          // 
lcd.print("      ");         // đệm phải

}
/*
void LIP1_test(){
  lcd.createChar(0, name0x4);
  lcd.createChar(1, name0x5);
  lcd.createChar(2, name0x6);
  lcd.createChar(3, name0x7);
  lcd.createChar(4, name0x8);
  lcd.createChar(5, name0x9);
  lcd.createChar(6, name0x10);
  lcd.createChar(7, name0x11);

  // Vẽ hàng trên (row 0)
lcd.setCursor(0, 0);
lcd.print("    ");         // đệm trái
lcd.write(byte(0));          // 
lcd.write(byte(1));          // 
lcd.write(byte(2));          // 
lcd.write(byte(3));          // 
lcd.write(byte(4));          // 
lcd.write(byte(5));          // 
lcd.write(byte(6));          // 
lcd.write(byte(7));          // 
lcd.print("    ");         // đệm phải
  lcd.createChar(0, name1x4);
  lcd.createChar(1, name1x5);
  lcd.createChar(2, name1x6);
  lcd.createChar(3, name1x7);
  lcd.createChar(4, name1x8);
  lcd.createChar(5, name1x9);
  lcd.createChar(6, name1x10);
  lcd.createChar(7, name1x11);

lcd.setCursor(0, 1);
lcd.print("    ");          //  đệm trái
lcd.write(byte(0));          //
lcd.write(byte(1));          // 
lcd.write(byte(2));          // 
lcd.write(byte(3));          // 
lcd.write(byte(4));          // 
lcd.write(byte(5));          // 
lcd.write(byte(6));          // 
lcd.write(byte(7));          // 
lcd.print("    ");      // đệm phải
}

void LIP2_test(){
  lcd.createChar(0, Bname0x5);
  lcd.createChar(1, Bname0x6);
  lcd.createChar(2, Bname0x7);
  lcd.createChar(3, Bname0x8);
  lcd.createChar(4, Bname0x9);
  lcd.createChar(5, Bname0x10);

   // Vẽ hàng trên (row 0)
lcd.setCursor(0, 0);
lcd.print("     ");         // đệm trái
lcd.write(byte(0));          // 
lcd.write(byte(1));          // 
lcd.write(byte(2));          // 
lcd.write(byte(3));          // 
lcd.write(byte(4));          // 
lcd.write(byte(5));           //
lcd.print("     ");         // đệm phải

lcd.createChar(0, Bname1x5);
  lcd.createChar(1, Bname1x6);
  lcd.createChar(2, Bname1x7);
  lcd.createChar(3, Bname1x8);
  lcd.createChar(4, Bname1x9);
  lcd.createChar(5, Bname1x10);

lcd.setCursor(0, 1);
lcd.print("     ");         // đệm trái
lcd.write(byte(0));          // 
lcd.write(byte(1));          // 
lcd.write(byte(2));          // 
lcd.write(byte(3));          // 
lcd.write(byte(4));          // 
lcd.write(byte(5));           //
lcd.print("     ");         // đệm phải

}

*/
