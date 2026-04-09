
void displaySmart(int row, String content) {
  lcd.setCursor(0, row);
  lcd.print("                ");  // Clear dòng

  int visibleLength = getUTF8CharCount(content);
  if (visibleLength <= 16) {
    int padding = (16 - visibleLength) / 2;
    lcd.setCursor(padding, row);
    lcd.print(content);
  } else {
    String trimmed = cutUTF8String(content, 14);  // Giữ trọn 14 ký tự
    lcd.setCursor(0, row);
    lcd.print(trimmed + "..");
  }
}

// Đếm số ký tự Unicode (UTF-8)
int getUTF8CharCount(const String &str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if ((str[i] & 0xC0) != 0x80) count++;  // đếm các byte đầu của ký tự UTF-8
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


void LIP1() {
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
  lcd.print("      ");  // đệm trái
  lcd.write(byte(0));   //
  lcd.write(byte(1));   //
  lcd.write(byte(2));   //
  lcd.write(byte(3));   //
  lcd.print("      ");  // đệm phải

  lcd.setCursor(0, 1);
  lcd.print("      ");  // đệm trái
  lcd.write(byte(4));   //
  lcd.write(byte(5));   //
  lcd.write(byte(6));   //
  lcd.write(byte(7));   //
  lcd.print("      ");  // đệm phải
}

void LIP2() {
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
  lcd.print("      ");  // đệm trái
  lcd.write(byte(0));   //
  lcd.write(byte(1));   //
  lcd.write(byte(2));   //
  lcd.write(byte(3));   //
  lcd.print("      ");  // đệm phải

  lcd.setCursor(0, 1);
  lcd.print("      ");  // đệm trái
  lcd.write(byte(4));   //
  lcd.write(byte(5));   //
  lcd.write(byte(6));   //
  lcd.write(byte(7));   //
  lcd.print("      ");  // đệm phải
}




void getWeatherSymbol(String weatherDesc) {
  weatherDesc.toLowerCase();  // tránh viết hoa gây lỗi

  if (weatherDesc.indexOf("rain") >= 0) {
    lcd.write(byte(5));
    lcd.write(byte(4));
    lcd.write(byte(4));
    lcd.write(byte(4));
    lcd.write(byte(5));
  } else if (weatherDesc.indexOf("clouds") >= 0) {

    if (Gio > 6 && Gio < 18) {
      lcd.write(byte(5));
      lcd.write(byte(5));
      lcd.write(byte(6));
      lcd.write(byte(5));
      lcd.write(byte(5));
    } else {
      lcd.write(byte(5));
      lcd.write(byte(5));
      lcd.write(byte(7));
      lcd.write(byte(5));
      lcd.write(byte(5));
    }
  }
else if (weatherDesc.indexOf("clear") >= 0) {
  if (Gio > 6 && Gio < 18) {
    lcd.setCursor(13, 0);
    lcd.write(byte(6));
  } else {
    lcd.setCursor(13, 0);
    lcd.write(byte(7));
  }
}
else if (weatherDesc.indexOf("snow") >= 0) {
  //return "❄️";
}
else if (weatherDesc.indexOf("thunder") >= 0) {
  lcd.write(byte(5));
}
else if (weatherDesc.indexOf("mist") >= 0 || weatherDesc.indexOf("fog") >= 0) {
  //return "🌫";
}
else {
  //return "?";  // không biết loại gì
}
}


/*String getWeatherSymbol(String weatherDesc) {
  weatherDesc.toLowerCase();  // tránh viết hoa gây lỗi

  if (weatherDesc.indexOf("rain") >= 0) {
    return "⛅";  // U+26C5
  } else if (weatherDesc.indexOf("cloud") >= 0) {
    return "☁️";//U+2601
  } else if (weatherDesc.indexOf("clear") >= 0) {
    return "☀️"; //U+2600
  } else if (weatherDesc.indexOf("snow") >= 0) {
    return "❄️";
  } else if (weatherDesc.indexOf("thunder") >= 0) {
    return "⚡";//	U+26A1
  } else if (weatherDesc.indexOf("mist") >= 0 || weatherDesc.indexOf("fog") >= 0) {
    return "🌫";
  } else {
    return "?";  // không biết loại gì
  }
}*/
