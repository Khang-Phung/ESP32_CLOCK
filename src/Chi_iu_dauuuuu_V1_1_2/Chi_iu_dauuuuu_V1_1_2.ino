
#define SPEAKER_PIN D7  // GPIO
#define BUTTON_PIN D6
#define BLYNK_TEMPLATE_ID "TMPL6VbXWYoFM"
#define BLYNK_TEMPLATE_NAME "SERVO"
#define BLYNK_AUTH_TOKEN "S5n6HhkqlOtx1RcXUh7hmELjaoTiJTOz"
// Firebase Config
#define API_KEY "AIzaSyAgUSv_T8CoRsp5cikxfQ447i8EZm5sCVQ"
#define DATABASE_URL "https://esp-reminder-default-rtdb.asia-southeast1.firebasedatabase.app/"  // KHÔNG có https://

// User Firebase Auth
#define USER_EMAIL "khangphung05@gmail.com"
#define USER_PASSWORD "ThaiKhang0510"




#include <ESP8266WiFi.h>  //wifi
#include <ESP8266WebServer.h>
#include <Wire.h>                //I2C
#include <LiquidCrystal_I2C.h>   //thư viện LCD
#include <NTPClient.h>           // thư viện đồng hồ
#include <WiFiUdp.h>             // thư viện đồng hồ luôn
#include <TimeLib.h>             // Thêm thư viện này để dễ xử lý ngày tháng
#include <SPI.h>                 //thư viện giao tiếp spi
#include <SD.h>                  //thư viện sd card
#include <BlynkSimpleEsp8266.h>  // Thư viện Blynk
#include <FirebaseESP8266.h>     // thư viện giao tiếp firebase


///// trái tim LCD
byte trai_tim[] = {
  B00000,
  B11011,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

byte vachwifi_3[] = {
  B00001,
  B00001,
  B00001,
  B00101,
  B00101,
  B10101,
  B10101,
  B10101
};

byte vachwifi_2[] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B00100,
  B10100,
  B10100,
  B10100
};

byte vachwifi_1[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10000,
  B10000,
  B10000
};



// biến LCD
int scrollIndex = 0;

// Nốt và thời gian tương ứng (ms)
const int TwinkleStar[] = {
  262, 0, 262, 0, 392, 0, 392, 0, 440, 0, 440, 0, 392, 0, 0,
  349, 0, 349, 0, 330, 0, 330, 0, 294, 0, 294, 0, 262, 0, 0
};

const int TwinkleStar_duration[] = {
  150, 100, 150, 100, 150, 100, 150, 100, 150, 100, 200, 100, 300, 100, 100,  // Twinkle Twinkle Little Star
  150, 100, 150, 100, 150, 100, 150, 100, 150, 100, 200, 100, 300, 100, 100   // How I wonder what you are
};
///// biến Wifi
const char* ssid = "The PTKs_2.4Ghz";
const char* password = "khanhphung0807";
int rssi;
//const char* ssid = "leonmusic";
//const char* password = "leonhouse";
char auth[] = BLYNK_AUTH_TOKEN;

// Firebase
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth_firebase;
String Title_calendar;
String Time_calendar;
String Time_calendar_temp;
int tim;

String Title_calendar_hientai;
int Gio_hientai;  // hiện tại
int Phut_hientai;
int Last_Gio_hientai;
int Last_Phut_hientai;

String Title_calendar_tieptheo;
int Gio_tieptheo;  //tiếp theo
int Phut_tieptheo;

String Title_calendar_temp;
int Gio_temp;  // tạm
int Phut_temp;
bool calendar_saved = false;
bool Calendar_hientai_empty = false;
bool Calendar_tieptheo_empty = false;


LiquidCrystal_I2C lcd(0x27, 16, 2);
ESP8266WebServer server(80);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000);


char Gio;
char Phut;
char Giay;
char dayVal;
char monthVal;
int yearVal;


///// biến message
bool Show_Tim = false;
bool Show_ngungon = false;
bool Show_quane = false;
bool Show_reminder = false;
///// hiển thị và âm thanh
bool LCD_Displayed = false;  // Biến cờ để kiểm tra đã clear chưa
bool tone_buzzed = true;
///// biến led ảo
bool ON_V1 = false;  //V10
bool ON_V2 = false;  //V11
bool ON_V3 = false;  //V12
bool notification = false;
///// biến thời gian
unsigned long DisplayStartTime = 0;
const unsigned int DisplayDuration = 3000;
const unsigned int DisplayDuration_reminder = 7000;
const unsigned long reminderDuration_repeat = 1200000;
unsigned long pre_time_audio = 0;
unsigned long lastUpdateTime = 0;
unsigned long lastUpdateTime_blynk = 0;
unsigned long lastUpdateTime_firebase = 0;
unsigned long Lasttime_Scroll = 0;
unsigned long reminder_repeat_start = 0;
const unsigned int Scroll_duration = 300;  // tốc độ cuộn (ms)
const unsigned int updateInterval = 1000;
const unsigned int updateInterval_blynk = 1000;      // Cập nhật mỗi 1 giây hiển thị đồng hồ
const unsigned int updateInterval_firebase = 30000;  // Cập nhật mỗi 30 giây firebase
const unsigned int AudioInterval = 300;              // phát âm thanh khoảng 0.3
unsigned long tingStart = 0;
char tingStep = 0;
bool tingPlaying = false;
bool reminder_repeat = false;

unsigned long noteStart = 0;
char twinkleNote = 0;
bool isPlayingTwinkle = true;
bool noteisPlayingTwinkle = false;

///// biến nút vật lý
bool PRESS = false;
bool HOLD = false;
char Program_sel = 0;
char last_program_sel = -1;  // khởi đầu khác để đảm bảo lần đầu in ra
char dotCount = 0;
bool enable_display = false;
bool sound_button = false;



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
      break;
    case 1:
      if (now - tingStart >= 100) {
        noTone(SPEAKER_PIN);
        tingStart = now;
        tingStep = 2;
      }
      break;
    case 2:
      if (now - tingStart >= 100) {
        tone(SPEAKER_PIN, 1800);  // Ting 2
        tingStart = now;
        tingStep = 3;
      }
      break;
    case 3:
      if (now - tingStart >= 100) {
        noTone(SPEAKER_PIN);
        tingPlaying = false;  // Kết thúc
        tone_buzzed = true;
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

void get_calendar_firebase() {
  // Gọi thử đọc dữ liệu từ Realtime Database
  if (Firebase.getString(fbdo, "/reminders/event0/time")) {
    Time_calendar_temp = fbdo.stringData();
    Gio_hientai = Time_calendar_temp.substring(0, Time_calendar_temp.indexOf(":")).toInt();
    Phut_hientai = Time_calendar_temp.substring(Time_calendar_temp.indexOf(":") + 1).toInt();
    Calendar_hientai_empty = false;
    if (Gio_hientai == 25) { Calendar_hientai_empty = true; }
  } /*else {
    Serial.print(" Lỗi Firebase: ");
    //Serial.println(fbdo.errorReason());
    Gio_hientai = 25;
    Phut_hientai = 61;
    Calendar_hientai_empty = true;
  }*/
  if (Firebase.getString(fbdo, "/reminders/event1/time")) {

    Time_calendar_temp = fbdo.stringData();
    Gio_tieptheo = Time_calendar_temp.substring(0, Time_calendar_temp.indexOf(":")).toInt();
    Phut_tieptheo = Time_calendar_temp.substring(Time_calendar_temp.indexOf(":") + 1).toInt();
    Calendar_tieptheo_empty = false;
    if (Gio_tieptheo == 25) { Calendar_tieptheo_empty = true; }
  } /*else {
    Serial.print(" Lỗi Firebase: ");
    //Serial.println(fbdo.errorReason());
    Gio_tieptheo = Gio_hientai;
    Phut_tieptheo = Phut_hientai;
    Calendar_tieptheo_empty = true;
  }*/
  if (Firebase.getString(fbdo, "/reminders/event0/title")) {
    Title_calendar_hientai = fbdo.stringData();
    //char myCharArray[Title_calendar.length() + 1]; // +1 cho ký tự null kết thúc
    //Title_calendar.toCharArray(myCharArray, sizeof(myCharArray));
  }
  if (Firebase.getString(fbdo, "/reminders/event1/title")) {
    Title_calendar_tieptheo = fbdo.stringData();
    //char myCharArray[Title_calendar.length() + 1]; // +1 cho ký tự null kết thúc
    //Title_calendar.toCharArray(myCharArray, sizeof(myCharArray));
  }
}

void setup() {

  pinMode(D6, INPUT_PULLUP);
  Serial.begin(115200);
  Wire.begin(D2, D1);
  Wire.setClock(400000);
  lcd.init();
  lcd.backlight();
  lcd.clear();


  lcd.createChar(0, trai_tim);
  lcd.createChar(1, vachwifi_1);
  lcd.createChar(2, vachwifi_2);
  lcd.createChar(3, vachwifi_3);
  lcd.setCursor(0, 0);
  lcd.print("Cho xiu nha! ...");


  wifi_set_phy_mode(PHY_MODE_11G);  // 802.11g (6-54 Mbps)	Tốc độ trung bình, phổ biến
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 1);
  lcd.print("Dang ket noi ");
  while (WiFi.status() != WL_CONNECTED) {

    lcd.setCursor(15, 1);  // vị trí cuối dòng
    lcd.print(".");
    dotCount++;
    if (dotCount > 3) {  // sau 3 chấm thì xóa và quay lại
      lcd.setCursor(13, 1);
      lcd.print("   ");  // xóa 3 dấu chấm cũ
      dotCount = 0;
    }
    delay(500);
  }

  /*Serial.println(F(" Đã kết nối!"));
  Serial.print(F("Địa chỉ IP: "));
  Serial.println(WiFi.localIP());*/

  /*server.on("/reminder", handleReminder);
  server.begin();
  Serial.println("Server da san sang.");*/
  lcd.setCursor(0, 0);
  lcd.print("Ket noi xong oi!");
  lcd.setCursor(0, 1);
  lcd.print("Dang xu ly      ");
  Blynk.begin(auth, ssid, password);
  lcd.setCursor(0, 1);
  lcd.print("Dang xu ly .");
  timeClient.begin();
  lcd.setCursor(0, 1);
  lcd.print("Dang xu ly ..");
  // Cấu hình xác thực người dùng Firebase
  auth_firebase.user.email = USER_EMAIL;
  auth_firebase.user.password = USER_PASSWORD;

  // Cấu hình Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Khởi tạo Firebase
  Firebase.begin(&config, &auth_firebase);
  Firebase.reconnectWiFi(true);
  lcd.setCursor(0, 1);
  lcd.print("Dang xu ly ...");

  // lấy thông tin trước khi vào loop
  get_calendar_firebase();
  lcd.setCursor(0, 1);
  lcd.print("Dang xu ly ....");
  Gio_temp = Gio_tieptheo;
  Phut_temp = Phut_tieptheo;
  Title_calendar_temp = Title_calendar_tieptheo;
  lcd.clear();
}

BLYNK_WRITE(V1) {  // khi có sự kiện kích hoạt hiển thị pop up
  int value = param.asInt();
  if (value == 1 && ON_V2 != 1 && ON_V3 != 1 && notification != 1) {
    if (ON_V1) {
      LCD_Displayed = false;
      tone_buzzed = false;
    }
    Show_Tim = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tim = tim + 1;
    tone_buzzed = false;
    ON_V1 = true;
    Blynk.virtualWrite(V10, ON_V1);
  }
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  if (value == 1 && ON_V1 != 1 && ON_V3 != 1 && notification != 1) {
    if (ON_V2) {
      LCD_Displayed = false;
      tone_buzzed = false;
    }
    Show_ngungon = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    isPlayingTwinkle = true;
    twinkleNote = 0;
    noteStart = millis();
    tone_buzzed = false;
    ON_V2 = true;
  }
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  if (value == 1 && ON_V2 != 1 && ON_V3 != 1 && notification != 1) {
    Show_quane = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tone_buzzed = false;
    ON_V3 = true;
  }
}

void reminder() {
  if (ON_V1 != 1 && ON_V2 != 1 && ON_V3 != 1) {
    Show_reminder = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tone_buzzed = false;
    notification = true;
    Giay = Giay + 1;  // điều kiện để tránh lặp lại
  }

  if (Calendar_tieptheo_empty) {
    Gio_temp = 26;
    Phut_temp = 61;
    return;
  }

  if(reminder_repeat != 1){
  Gio_hientai = Gio_temp;
  Phut_hientai = Phut_temp;
  Gio_temp = Gio_tieptheo;
  Phut_temp = Phut_tieptheo;
  Title_calendar_hientai = Title_calendar_temp;
  Title_calendar_temp = Title_calendar_tieptheo;
  }
}


void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    int retries = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mat ket noi oi!!");
    lcd.setCursor(0, 1);
    lcd.print("Dang ket noi ");
    while (WiFi.status() != WL_CONNECTED && retries < 10) {
      char dot_pos;
      dot_pos = 13 + dotCount;
      lcd.setCursor(dot_pos, 1);  // vị trí cuối dòng
      lcd.print(".");
      dotCount++;
      if (dotCount > 3) {  // sau 3 chấm thì xóa và quay lại
        lcd.setCursor(13, 1);
        lcd.print("   ");  // xóa 3 dấu chấm cũ
        dotCount = 0;
      }

      retries++;
    }
    if (WiFi.status() != WL_CONNECTED) {

      lcd.setCursor(0, 0);
      lcd.print("Mat ket noi oiii");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
  }
}

void loop() {
  Blynk.run();          // Chạy blynk
  checkWiFi();          // kiểm tra kết nối wifi
  timeClient.update();  // cập nhật thời gian
  rssi = WiFi.RSSI();
  // Gọi kiểm tra nút nhấn
  if (debounceButton(D6)) {  //check nút
    enable_display = true;
    Program_sel = Program_sel + 1;
    if (Program_sel > 1) Program_sel = 0;  // quay lại chế độ 0
    Serial.println("Nút được nhấn (nhấn nhanh)");
    sound_button = true;
  }
    if(sound_button){
      playButtonPressTone();
    }
  
      // Thêm hành động của bạn ở đây

  if (WiFi.status() == WL_CONNECTED) {

    unsigned long currentMillis = millis();
    //////////////////////////Update firebase///////////////////////////////
    if (currentMillis - lastUpdateTime_firebase >= updateInterval_firebase) {
      lastUpdateTime_firebase = currentMillis;
      get_calendar_firebase();
      if (Gio < Gio_hientai || (Gio == Gio_hientai && Phut <= Phut_hientai)) {
        Gio_temp = Gio_hientai;
        Phut_temp = Phut_hientai;
        Title_calendar_temp = Title_calendar_hientai;
      } else {
        Gio_temp = Gio_tieptheo;
        Phut_temp = Phut_tieptheo;
        Title_calendar_temp = Title_calendar_tieptheo;
      }
    }
    //////////////////////////Update blynk///////////////////////////////
    if (currentMillis - lastUpdateTime_blynk >= updateInterval_blynk) {
      lastUpdateTime_blynk = currentMillis;
      Blynk.virtualWrite(V0, rssi);
      Serial.print(Gio_hientai);
      Serial.print(":");
      Serial.println(Phut_hientai);
      Serial.print(Gio_tieptheo);
      Serial.print(":");
      Serial.println(Phut_tieptheo);
      Serial.print(Gio_temp);
      Serial.print(":");
      Serial.println(Phut_temp);
      Serial.print(Calendar_hientai_empty);
      Serial.print(",");
      Serial.print(Show_reminder);
      Serial.print(",");
      Serial.println(notification);
    }

    //////////////////////////POP UP///////////////////////////////
    if (Show_Tim) {          //điều kiện on
      if (!LCD_Displayed) {  //xét đã hiện thị chưa
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cho C Iuuu timmm");
        lcd.setCursor(0, 1);
        lcd.print("Tim ne :");
        lcd.write(0);
        lcd.print("x");
        lcd.print(tim);
        LCD_Displayed = true;  // Đánh dấu đã hiển thị rồi
      }

      if (currentMillis - DisplayStartTime > DisplayDuration) {  // Điều kiện ngừng pop up
        Show_Tim = false;                                        // reset điều kiện on
        LCD_Displayed = false;                                   // reset đánh dấu hiển thị
        lcd.clear();                                             // Chỉ clear lại khi hết hiển thị
        tim = 0;                                                 // reset tim
        ON_V1 = false;                                           // reset cờ on nút nhấn
        Blynk.virtualWrite(V10, ON_V1);
      }
      if (!tone_buzzed) {
        playTingTingNonBlocking();
      }

      return;  // Không vẽ thời gian khi đang hiển thị dummy
    }

    if (Show_ngungon) {
      if (!LCD_Displayed) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Chi Iu ngu ngon");
        lcd.write(0);
        lcd.print("x");
        lcd.print(tim);
        LCD_Displayed = true;  // Đánh dấu đã hiển thị rồi
      }

      if (isPlayingTwinkle == false) {
        Show_ngungon = false;
        LCD_Displayed = false;
        lcd.clear();  // Chỉ clear lại khi hết hiển thị

        ON_V2 = false;  // reset cờ on nút nhấn
        Blynk.virtualWrite(V10, ON_V2);
      }
      if (!tone_buzzed) {
        playTwinkleTheme();
      }

      return;  // Không vẽ thời gian khi đang hiển thị dummy
    }

    if (Show_reminder) {
      if (!LCD_Displayed) {
        lcd.clear();
        char timeStr[17];
        if(Gio_hientai != 25){
        sprintf(timeStr, "%02d:%02d", Gio_hientai, Phut_hientai);
        lcd.setCursor(5, 0);
        lcd.print(timeStr);}
        else {lcd.setCursor(5, 0);
        lcd.print("free");}

        
        if (currentMillis - Lasttime_Scroll >= Scroll_duration) {
          Lasttime_Scroll = currentMillis;
          displaySmart(1, Title_calendar_hientai);
        }
        LCD_Displayed = true;  // Đánh dấu đã hiển thị rồi
      }
      if (currentMillis - DisplayStartTime > DisplayDuration_reminder) {  // Điều kiện ngừng pop up
        Show_reminder = false;                                            // reset điều kiện on
        LCD_Displayed = false;                                            // reset đánh dấu hiển thị
        lcd.clear();                                                      // Chỉ clear lại khi hết hiển thị
        notification = false;                                             // reset cờ on nút nhấn
        reminder_repeat = false;
      }
      if (!tone_buzzed) {
        playBeepBoopNonBlocking();
      }
      return;  // Không vẽ thời gian khi đang hiển thị dummy
    }
          //////////////////////////POP UP///////////////////////////////
    if (Gio == Gio_temp && Phut == Phut_temp && Giay == 0 && Calendar_hientai_empty == 0)  // Pop up dạng nhắc lịch
    { reminder_repeat_start = millis();
      reminder();
    }
          //////////////////////////POP UP///////////////////////////////
    if (Gio > Gio_hientai || (Gio == Gio_hientai && Phut >= Phut_hientai)){
      if (currentMillis - reminder_repeat_start > reminderDuration_repeat){
        reminder_repeat_start = millis();
        reminder_repeat = true;
        reminder();
      }

    }

    ///////////////////////////Main Program////////////////////////////
    if (Program_sel == 0) {

      if (currentMillis - lastUpdateTime >= updateInterval || enable_display == true) {
        lastUpdateTime = currentMillis;
        enable_display = false;
        time_t rawTime = timeClient.getEpochTime();
        Gio = hour(rawTime);
        Phut = minute(rawTime);
        Giay = second(rawTime);
        dayVal = day(rawTime);
        monthVal = month(rawTime);
        yearVal = year(rawTime);

        char dateStr[17];
        char timeStr[17];
        sprintf(dateStr, "%02d/%02d/%04d", dayVal, monthVal, yearVal);
        sprintf(timeStr, "%02d:%02d:%02d", Gio, Phut, Giay);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print(dateStr);
        lcd.setCursor(4, 1);
        lcd.print(timeStr);
        lcd.setCursor(15, 0);
        if (rssi <= -80) {
          lcd.write(1);  // 1 vạch Wi-Fi
        } else if (rssi <= -70) {
          lcd.write(2);  // 2 vạch Wi-Fi
        } else {
          lcd.write(3);  // 3 vạch Wi-Fi
        }
      }
    }
    if (Program_sel == 1) {
      if (Gio_hientai != Last_Gio_hientai && Phut_hientai != Last_Phut_hientai || enable_display == true) {
        enable_display = false;
        lcd.clear();
        char timeStr[17];
         if(Gio_hientai != 25){
        sprintf(timeStr, "%02d:%02d", Gio_hientai, Phut_hientai);
        lcd.setCursor(5, 0);
        lcd.print(timeStr);}
        else {lcd.setCursor(5, 0);
        lcd.print("free");}
        if (currentMillis - Lasttime_Scroll >= Scroll_duration) {
          Lasttime_Scroll = currentMillis;
          displaySmart(1, Title_calendar_hientai);
        }
        Last_Gio_hientai = Gio_hientai;
        Last_Phut_hientai = Phut_hientai;
      }
    }


  }  //vòng có kết nối wifi
}  //loop