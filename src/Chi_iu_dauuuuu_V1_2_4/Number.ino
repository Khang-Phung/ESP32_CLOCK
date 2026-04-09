void custom0(int col)
{ 
  lcd.setCursor(col, 0); 
  lcd.print("▓");   // bar 2
  lcd.print("▀");   // bar 8
  lcd.print("█");   // bar 1
  lcd.setCursor(col, 1); 
  lcd.print("▓");   
  lcd.print("▌");   // bar 6
  lcd.print("█");   
}

void custom1(int col)
{
  lcd.setCursor(col, 0);
  lcd.print("  █"); 
  lcd.setCursor(col, 1);
  lcd.print("  █"); 
}

void custom2(int col)
{
  lcd.setCursor(col, 0);
  lcd.print("▄"); // bar 5
  lcd.print("▒"); // bar 3
  lcd.print("█"); 
  lcd.setCursor(col, 1);
  lcd.print("▓"); // bar 2
  lcd.print("▌"); // bar 6
  lcd.print("▌"); 
}

void custom3(int col)
{
  lcd.setCursor(col, 0);
  lcd.print("▄"); 
  lcd.print("▒"); 
  lcd.print("█"); 
  lcd.setCursor(col, 1);
  lcd.print("▐"); // bar 7
  lcd.print("▌"); 
  lcd.print("█"); 
}

void custom4(int col)
{
  lcd.setCursor(col, 0);
  lcd.print("▓"); 
  lcd.print("▌"); 
  lcd.print("█"); 
  lcd.setCursor(col, 1);
  lcd.print("  █"); 
}

void custom5(int col)
{
  lcd.setCursor(col, 0);
  lcd.print("▓"); 
  lcd.print("▒"); 
  lcd.print("░"); // bar 4
  lcd.setCursor(col, 1);
  lcd.print("▐"); 
  lcd.print("▌"); 
  lcd.print("█"); 
}

void custom6(int col)
{
  lcd.setCursor(col, 0);
  lcd.print("▓"); 
  lcd.print("▒"); 
  lcd.print("░"); 
  lcd.setCursor(col, 1);
  lcd.print("▓"); 
  lcd.print("▌"); 
  lcd.print("█"); 
}

void custom7(int col)
{
  lcd.setCursor(col, 0);
  lcd.print("▓"); 
  lcd.print("▀");  // bar 8
  lcd.print("█"); 
  lcd.setCursor(col, 1);
  lcd.print("  █"); 
}

void custom8(int col)
{
  lcd.setCursor(col, 0); 
  lcd.print("▓"); 
  lcd.print("▒"); 
  lcd.print("█"); 
  lcd.setCursor(col, 1); 
  lcd.print("▓"); 
  lcd.print("▌"); 
  lcd.print("█"); 
}

void custom9(int col)
{
  lcd.setCursor(col, 0); 
  lcd.print("▓"); 
  lcd.print("▒"); 
  lcd.print("█"); 
  lcd.setCursor(col, 1); 
  lcd.print("▐"); 
  lcd.print("▌"); 
  lcd.print("█"); 
}

void custom10(int col)
{
  lcd.setCursor(col, 0); 
  lcd.print("▌"); 
  lcd.setCursor(col, 1); 
  lcd.print("▌");
}
