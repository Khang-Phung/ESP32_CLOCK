#define SD_CS_PIN D8    // GPIO15
#define SPEAKER_PIN D7  // GPIO15
#define BUTTON_PIN D6
#define BLYNK_TEMPLATE_ID "TMPL6VbXWYoFM"
#define BLYNK_TEMPLATE_NAME "SERVO"
#define BLYNK_AUTH_TOKEN "S5n6HhkqlOtx1RcXUh7hmELjaoTiJTOz"

#include <ESP8266WiFi.h>         //wifi
#include <Wire.h>                //I2C
#include <LiquidCrystal_I2C.h>   //thư viện LCD
#include <NTPClient.h>           // thư viện đồng hồ
#include <WiFiUdp.h>             // thư viện đồng hồ luôn
#include <TimeLib.h>             // Thêm thư viện này để dễ xử lý ngày tháng
#include <SPI.h>                 //thư viện giao tiếp spi
#include <SD.h>                  //thư viện sd card
#include <BlynkSimpleEsp8266.h>  // Thư viện Blynk


///// trái tim LCD
byte customChar[] = {
  B00000,
  B11011,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

// Nốt và thời gian tương ứng (ms)
const int melody[] = {
  262,0, 262,0, 392,0, 392,0, 440,0, 440,0, 392,0, 0,
  349,0, 349,0, 330,0, 330,0, 294,0, 294,0, 262,0, 0
};

const int noteDurations[] = {
  150,100, 150,100, 150,100, 150,100, 150,100, 200,100, 300,100, 100,  // Twinkle Twinkle Little Star
  150,100, 150,100, 150,100, 150,100, 150,100, 200,100, 300,100, 100   // How I wonder what you are
};
///// biến Wifi
//const char* ssid = "The PTKs_2.4Ghz";
//const char* password = "khanhphung0807";
const char* ssid = "leonmusic";
const char* password = "leonhouse";
char auth[] = BLYNK_AUTH_TOKEN;

int tim;

LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000);
///// biến message
bool Show_Tim = false;
bool Show_ngungon = false;
bool Show_quane = false;
///// hiển thị và âm thanh
bool dummyDisplayed = false;  // Biến cờ để kiểm tra đã clear chưa
bool tone_enable = false;
///// biến led ảo
bool ON_V1 = false;  //V10
bool ON_V2 = false;  //V11
bool ON_V3 = false;  //V12

///// biến thời gian
unsigned long DisplayStartTime = 0;
const unsigned long DisplayDuration = 3000;
unsigned long pre_time_audio = 0;
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 1000;  // Cập nhật mỗi 1 giây
const unsigned long AudioInterval = 300;    // phát âm thanh khoảng 0.3
unsigned long tingStart = 0;
int tingStep = 0;
bool tingPlaying = false;

unsigned long noteStart  = 0;
int twinkleNote   = 0;
bool isPlayingTwinkle  = true;
bool noteisPlayingTwinkle  = false;

///// biến nút vật lý
bool PRESS = false;
bool HOLD = false;

void playTwinkleTheme() {
  unsigned long now = millis();
  if (isPlayingTwinkle ) {
    if (twinkleNote == 0 || now - noteStart >= noteDurations[twinkleNote - 1]) {
      if (twinkleNote < sizeof(melody) / sizeof(melody[0])) {
        if(melody[twinkleNote]>0){
        tone(SPEAKER_PIN, melody[twinkleNote]);
        noteStart = now;
        twinkleNote++;}
        else{
        noTone(SPEAKER_PIN); 
        noteStart = now;
        twinkleNote++; 
        }
      } else {
        noTone(SPEAKER_PIN);
        isPlayingTwinkle  = false;
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
        tone_enable = true;
      }
      break;
  }
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

void onShortPress() {
  PRESS = true;  // cờ báo có nút nhấn
  // lcd.noBacklight(); hoặc lcd.backlight();
}

void onLongHold() {
  HOLD = true;  // cờ báo có nút giữ
}

void setup() {

  pinMode(D6, INPUT_PULLUP);
  Serial.begin(115200);
  Wire.begin(D2, D1);
  Wire.setClock(400000);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.createChar(0, customChar);

  /*if (!SD.begin(SD_CS_PIN)) {
    lcd.setCursor(0, 0);
    lcd.print("SD card error!");
    while (1); // Dừng luôn nếu lỗi thẻ SD
  }*/

  WiFi.begin(ssid, password);
  Serial.print(F("Đang kết nối WiFi..."));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(F(" Đã kết nối!"));
  Serial.print(F("Địa chỉ IP: "));
  Serial.println(WiFi.localIP());

  Blynk.begin(auth, ssid, password);
  timeClient.begin();
}

BLYNK_WRITE(V1) {
  int value = param.asInt();
  if (value == 1 && ON_V2 != 1 && ON_V3 != 1) {
    if (ON_V1) {
      dummyDisplayed = false;
      tone_enable = false;
    }
    Show_Tim = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tim = tim + 1;
    tone_enable = false;
    ON_V1 = true;
    Blynk.virtualWrite(V10, ON_V1);
  }
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  if (value == 1 && ON_V1 != 1 && ON_V3 != 1) {
    if (ON_V2) {
      dummyDisplayed = false;
      tone_enable = false;
    }
    Show_ngungon = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    isPlayingTwinkle  = true;
    twinkleNote   = 0;
  noteStart   = millis();
  tone_enable = false;
    ON_V2 = true;
  }
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  if (value == 1 && ON_V2 != 1 && ON_V3 != 1) {
    Show_quane = true;
    DisplayStartTime = millis();
    pre_time_audio = millis();
    tim = tim + 1;
    tone_enable = false;
    ON_V3 = true;
  }
}

void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi mất kết nối! Đang kết nối lại..."));
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 10) {
      delay(500);
      Serial.print(".");
      retries++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(F(" Đã kết nối lại!"));
    } else {
      Serial.println(F(" Không kết nối được!"));
    }
  }
}

void loop() {
  Blynk.run();
  checkWiFi();
  timeClient.update();

  if (debounceButton(D6)) {
    static bool backlightState = true;
    backlightState = !backlightState;
    if (backlightState) {
      lcd.backlight();
    } else {
      lcd.noBacklight();
    }
  }
  unsigned long currentMillis = millis();
  /////////////////////////////////////////////////////////////////
  if (Show_Tim) {
    if (!dummyDisplayed) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cho C Iuuu timmm");
      lcd.setCursor(0, 1);
      lcd.print("Tim ne :");
      lcd.write(0);
      lcd.print("x");
      lcd.print(tim);
      dummyDisplayed = true;  // Đánh dấu đã hiển thị rồi
    }

    if (currentMillis - DisplayStartTime > DisplayDuration) {
      Show_Tim = false;
      dummyDisplayed = false;
      lcd.clear();     // Chỉ clear lại khi hết hiển thị
      tim = 0;         // reset tim
      ON_V1 = false;   // reset cờ on nút nhấn
      Blynk.virtualWrite(V10, ON_V1);
    }
    if (!tone_enable) {
      playTingTingNonBlocking();
    }

    return;  // Không vẽ thời gian khi đang hiển thị dummy
  }

   if (Show_ngungon) {
    if (!dummyDisplayed) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Chi Iu ngu ngon");
      lcd.write(0);
      lcd.print("x");
      lcd.print(tim);
      dummyDisplayed = true;  // Đánh dấu đã hiển thị rồi
    }

    if (isPlayingTwinkle  == false) {
      Show_ngungon = false;
      dummyDisplayed = false;
      lcd.clear();     // Chỉ clear lại khi hết hiển thị

      ON_V2 = false;   // reset cờ on nút nhấn
      Blynk.virtualWrite(V11, ON_V2);
    }
    if (!tone_enable) {
      playTwinkleTheme();
    }

    return;  // Không vẽ thời gian khi đang hiển thị dummy
  }
  /////////////////////////////////////////////////////////////////

  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;

    time_t rawTime = timeClient.getEpochTime();
    int Gio = hour(rawTime);
    int Phut = minute(rawTime);
    int Giay = second(rawTime);
    int dayVal = day(rawTime);
    int monthVal = month(rawTime);
    int yearVal = year(rawTime);

    char dateStr[17];
    char timeStr[17];
    sprintf(dateStr, "%02d/%02d/%04d", dayVal, monthVal, yearVal);
    sprintf(timeStr, "%02d:%02d:%02d", Gio, Phut, Giay);

    lcd.setCursor(3, 0);
    lcd.print(dateStr);
    lcd.setCursor(4, 1);
    lcd.print(timeStr);
  }
}