// Config thiết bị
#if defined(ARDUINO_ESP32_DEV)
// Đây là ESP32 WROOM
#define DS18B20_PIN 4
#define SPEAKER_PIN 25
#define BUTTON_PIN 12

#elif defined(ARDUINO_ESP32C3_DEV)
// Đây là ESP32-C3 Mini
#define DS18B20_PIN 3
#define SPEAKER_PIN 1
#define BUTTON_PIN 0

#else
#error "Board chưa hỗ trợ, kiểm tra define"
#endif

// Config wifi
#define WIFI_TIMEOUT_MS     500   // Thời gian chờ mỗi lần thử (ms)
#define WIFI_RETRY_COUNT    20    // Số lần thử mỗi mạng
#define AP_TIMEOUT_MS       450000 // Thời gian chờ ở chế độ AP (ms)
#define WIFI_RETRY_NEW      5000  // Thời gian thử kết nối khi nhập mới (ms)


// Config Blynk
#define BLYNK_TEMPLATE_ID "TMPL6VbXWYoFM"
#define BLYNK_TEMPLATE_NAME "SERVO"
#define BLYNK_AUTH_TOKEN "JACftJ323ry267oWPifsUDsCYksb3ZRy"
#define BLYNK_FIRMWARE_VERSION "1.2.8"

// Firebase Config
#define API_KEY "AIzaSyAgUSv_T8CoRsp5cikxfQ447i8EZm5sCVQ"
#define DATABASE_URL "https://esp-reminder-default-rtdb.asia-southeast1.firebasedatabase.app/"  // KHÔNG có https://
// User Firebase Auth
#define USER_EMAIL "khangphung05@gmail.com"
#define USER_PASSWORD "ThaiKhang0510"


#include <time.h>
#include <WiFi.h>  //wifi
#include <WebServer.h>
#include <Wire.h>                 //I2C
#include <LCDI2C_Multilingual.h>  // Include the whole header file
#include <NTPClient.h>            // thư viện đồng hồ
#include <WiFiUdp.h>              // thư viện đồng hồ luôn
#include <TimeLib.h>              // Thêm thư viện này để dễ xử lý ngày tháng
#include <SPI.h>                  //thư viện giao tiếp spi
#include <SD.h>                   //thư viện sd card
#include <BlynkSimpleEsp32.h>     // Thư viện Blynk
#include <FirebaseESP32.h>        // thư viện giao tiếp firebase

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include <LittleFS.h> //thư viện flash card để lưu hình ảnh âm thanh

#include <HTTPClient.h>   // Dùng để gửi request HTTP GET/POST
#include <ArduinoJson.h>  // Dùng để parse JSON trả về từ API

#include <OneWire.h> // Giao tiếp Onewire
#include <DallasTemperature.h>  // Giao tiếp DS18b20
#include <Preferences.h> //thư viện flash card để lưu vào vùng nhớ nội
#include <WiFiClient.h> //thư viện để ping đến gg

Preferences prefs;

OneWire oneWire(DS18B20_PIN);        // Đối tượng OneWire
DallasTemperature sensor(&oneWire);  // Giao tiếp DS18B20

/*
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

*/

byte name0x6[] = { B00000, B00000, B00001, B00011, B00111, B01111, B11111, B11110 };
byte name0x7[] = { B00000, B11000, B11110, B11111, B11111, B11111, B11111, B00011 };
byte name0x8[] = { B00000, B00011, B01111, B11111, B11111, B11111, B11111, B11000 };
byte name0x9[] = { B00000, B00000, B10000, B11000, B11100, B11110, B11111, B01111 };
byte name1x6[] = { B11001, B11111, B01111, B00111, B00011, B00001, B00000, B00000 };
byte name1x7[] = { B11100, B11111, B11111, B11111, B11111, B11111, B01111, B00000 };
byte name1x8[] = { B00111, B11111, B11111, B11111, B11111, B11111, B11110, B00000 };
byte name1x9[] = { B10011, B11111, B11110, B11100, B11000, B10000, B00000, B00000 };

byte Bname0x6[] = { B00000, B00000, B00001, B00011, B00111, B00111, B01111, B11111 };
byte Bname0x7[] = { B00000, B11110, B11111, B11111, B11111, B11111, B11101, B11100 };
byte Bname0x8[] = { B00000, B01111, B11111, B11111, B11111, B11111, B10111, B00111 };
byte Bname0x9[] = { B00000, B00000, B10000, B11000, B11100, B11100, B11110, B11111 };
byte Bname1x6[] = { B11111, B00111, B00011, B00001, B00000, B00000, B00000, B00000 };
byte Bname1x7[] = { B11110, B11111, B11111, B11111, B11111, B00111, B00000, B00000 };
byte Bname1x8[] = { B01111, B11111, B11111, B11111, B11111, B11100, B00000, B00000 };
byte Bname1x9[] = { B11111, B11100, B11000, B10000, B00000, B00000, B00000, B00000 };

byte House[] = { B00100, B01110, B11111, B11111, B11111, B11111, B11011, B11011 };
byte City[] = { B01110, B01010, B01110, B01010, B01110, B11010, B11111, B11111 };
byte Humidity[] = { B00000, B00100, B01110, B11111, B11111, B11111, B11111, B01110 };
byte Cloud[] = { B00110, B01111, B11111, B00000, B01110, B11110, B11111, B00000 };
byte Lightning[] = { B00011, B00110, B01100, B11100, B11110, B00110, B01100, B11000 };
byte Sun[] = { B00000, B10001, B01110, B11111, B11111, B11111, B01110, B10001 };
byte Rain[] = { B01110, B11111, B00000, B01001, B10010, B00000, B00100, B01000 };
byte Degree[] = { B00000, B01110, B01010, B01110, B00000, B00000, B00000, B00000 };
byte Moon[] = { B00110, B01100, B11000, B11000, B11000, B11000, B01110, B00111 };

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
// Danh sách SSID và PASSWORD
//const char* ssids[] = {"leonmusic", "The PTKs_2.4Ghz", "", "SSID4"};
//const char* passwords[] = {"leonhouse", "khanhphung0807", "", "PASS4"};
const char* ssids[] = { "leonmusic", "The PTKs_2.4Ghz", "Leonstudio" };
const char* passwords[] = { "leonhouse", "khanhphung0807", "" };

String ssid;
String pass;

const char* ap_ssid = "Bé Khang";
const char* ap_pass = "iuiuiuiu";
int wifiCount = sizeof(ssids) / sizeof(ssids[0]);

char dotCount = 0;


int rssi;
bool connected = false;
//const char* ssid = "";
//const char* password = "";
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
//int Last_Gio_hientai;
//int Last_Phut_hientai;

String Title_calendar_tieptheo;
int Gio_tieptheo;  //tiếp theo
int Phut_tieptheo;

String Title_calendar_temp;
int Gio_temp;  // tạm
int Phut_temp;
bool calendar_saved = false;
bool Calendar_hientai_empty = false;
bool Calendar_tieptheo_empty = false;

const char* weekdays[] = {
  "", "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"
};



LCDI2C_Vietnamese lcd(0x27, 16, 2);
DNSServer dns;
AsyncWebServer server(80);


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "162.159.200.1", 7 * 3600, 60000);


char Gio;
char Phut;
char Giay;
char Thu;
char dayVal;
char monthVal;
int yearVal;


///// biến message
bool Show_Tim = false;
bool Show_ngungon = false;
bool Show_hun = false;
bool Show_reminder = false;
bool Show_khang_reminder = false;
///// hiển thị và âm thanh
bool LCD_Displayed = false;  // Biến cờ để kiểm tra đã clear chưa
bool tone_buzzed = true;
char Lip_status = 0;
bool showLIP1 = true;
bool showLIP2 = true;
bool backlightState = true;
///// biến led ảo
bool ON_V1 = false;  //V10
bool ON_V2 = false;  //V11
bool ON_V3 = false;  //V12
bool notification = false;
///// biến thời gian
bool timeSynced = false;
static bool first_display = true;
static unsigned long last_update_time = 0;
unsigned long DisplayStartTime = 0;
const unsigned int DisplayDuration = 3000;
const unsigned int DisplayDuration_reminder = 10000;
const unsigned long reminderDuration_repeat = 1200000;
unsigned long pre_time_audio = 0;
unsigned long lastReset = 0;
unsigned long lastUpdateTime = 0;
unsigned long lastUpdateTime_blynk = 0;
unsigned long lastUpdateTime_firebase = 0;
unsigned long Lasttime_Scroll = 0;
unsigned long reminder_repeat_start = 0;
unsigned long pretime_lip1 = 0;
unsigned long pretime_lip2 = 0;
unsigned long fps_last = 0;
unsigned long lastWiFiAttempt = 0;
unsigned long lastNTPAttempt = 0;
unsigned long lastWiFiCheck = 0;
const unsigned int Scroll_duration = 300;  // tốc độ cuộn (ms)
const unsigned int updateInterval = 200; // tốc độ cập nhật của main prog 0
const unsigned int updateInterval_blynk = 1000;              // Cập nhật mỗi 1 giây 
const unsigned int updateInterval_firebase = 30000;          // Cập nhật mỗi 30 giây firebase
const unsigned int AudioInterval = 300;                      // phát âm thanh khoảng 0.3
const unsigned long SYNC_EVERY_24H = 24UL * 60 * 60 * 1000;  // 24 giờ
const unsigned long WIFI_CHECK_INTERVAL = 30000;              // 30 giây
unsigned long syncInterval = 2000;                           // 2 giây ban đầu


unsigned long tingStart = 0;
char tingStep = 0;
bool tingPlaying = false;
bool reminder_repeat = true;

unsigned long noteStart = 0;
char twinkleNote = 0;
bool isPlayingTwinkle = false;


///// biến nút vật lý
bool PRESS = false;
bool HOLD = false;
char Program_sel = 0;
char last_program_sel = -1;  // khởi đầu khác để đảm bảo lần đầu in ra
bool enable_display = false;
bool sound_button = false;
bool Button_press = false;
char last_saved_program = -1;
bool last_saved_Backlight = false;
bool first_time = true;

enum ButtonState {
  BUTTON_NONE = 0,
  BUTTON_PRESSED,
  BUTTON_HELD
};

enum FbStep {
  FB_IDLE,
  FB_GET_EVENT0_TIME,
  FB_GET_EVENT1_TIME,
  FB_GET_EVENT0_TITLE,
  FB_GET_EVENT1_TITLE,
  FB_DONE
};
FbStep fbStep = FB_GET_EVENT0_TIME;

enum PopupType { NONE,
                 TIM,
                 NGUNGON,
                 REMINDER,
                 HUN,
                 KHANG_REMINDER };
PopupType CurrentPopup = NONE;

enum TimeOfDay {
  MORNING,
  NOON,
  EVENING,
  NIGHT,
  NONE_TIME
};

TimeOfDay currentPeriod = NONE_TIME;

bool wifiConnecting = false;
bool wifiConnected = false;
bool wifiFailed = false;
unsigned long wifiTryStart = 0;

//DS18b20
unsigned long lastRequestTime = 0;
unsigned long last_weather = 0;
bool waitingForConversion = false;
const int resolution = 11;  // Chọn độ phân giải (9 đến 12)
unsigned int conversionDelay;
float temp;
float outdoor_temp;
int outdoor_humidity;
String weather_desc;

//Debug
int a, b, c, d, e, f, g, h, i, j, k;
int serialNumber = -1;   // Số đọc được, -1 nghĩa là chưa có số mới
String buffer = "";      // Bộ nhớ tạm để ghép số



void setup() {

  prefs.begin("config", true);                   // true: chỉ đọc
  Program_sel = prefs.getInt("Program_sel", 0);  // 0 là mặc định
  prefs.end();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  sensor.begin();
  sensor.setResolution(resolution);
  // Tính thời gian cần đợi theo datasheet
  sensor.setWaitForConversion(false);
  sensor.requestTemperatures();
  Wire.begin();
  Wire.setClock(400000);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("   Đồng hồ TK   ");
  lcd.setCursor(0, 1);
  lcd.print("         V");
  lcd.print(BLYNK_FIRMWARE_VERSION);
  delay(5000);

  /////////////////////////////////////////////////
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed!");
  } else {
    Serial.println("LittleFS mounted.");

    Serial.println("Listing files on LittleFS:");
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file) {
      Serial.println(file.name());
      file = root.openNextFile();
    }
  }
  ////////////////////////////////////////////////////
  connect_wifi();
  ssid = WiFi.SSID().c_str();
  pass = WiFi.psk().c_str();
  delay(1000);
  checkAndReconnectWiFi_1st();
  // Đến đây là fully connected
  lcd.setCursor(0, 0);
  lcd.print("Kết nối xong òi!");
  lcd.setCursor(0, 1);
  lcd.print("Đang xử lý      ");
  Blynk.begin(auth, ssid, pass);
  lcd.setCursor(0, 1);
  lcd.print("Đang xử lý .");
  timeClient.begin();
  lcd.setCursor(0, 1);
  lcd.print("Đang xử lý ..");

  // Cấu hình xác thực người dùng Firebase
  auth_firebase.user.email = USER_EMAIL;
  auth_firebase.user.password = USER_PASSWORD;

  // Cấu hình Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Khởi tạo Firebase
  syncTimeAndInitFirebase(config, auth_firebase);
  lcd.setCursor(0, 1);
  lcd.print("Đang xử lý ...");

  // lấy thông tin trước khi vào loop
  while (fbStep != FB_IDLE) {
    get_calendar_firebase();  // Sẽ đi từ FB_IDLE → ... → FB_DONE → FB_IDLE

    delay(10);  // Cho CPU thở tí, tránh freeze
  }
  get_weather_firebase();
  temp = sensor.getTempCByIndex(0);
  //Gio_temp = Gio_tieptheo;
  //Phut_temp = Phut_tieptheo;
  //Title_calendar_temp = Title_calendar_tieptheo;
  lcd.clear();
}

void loop() {
  unsigned long loopStart = millis();  // Bắt đầu đo toàn vòng loop
  Serial.println("=== LOOP START ===");

  unsigned long t1 = millis();
  Blynk.run();  // Chạy blynk
  Serial.printf("[DEBUG] Blynk.run() mất %lu ms\n", millis() - t1);
  a = millis() - t1;
  t1 = millis();
  time_sync();
  b = millis() - t1;
  
  
  /* //Khúc restart
  if (Phut == 0 && Giay == 0) {
    switch (Gio) {
      case 3:
      case 13:
        Serial.println("[DEBUG] Restart ESP");
        ESP.restart();
        break;
    }
  }*/

  // nút nhấn
  t1 = millis();
  handle_button();
  Serial.printf("[DEBUG] handle_button() mất %lu ms\n", millis() - t1);
  c = millis() - t1;
  
  
  
  // Nếu có WiFi
  if (WiFi.status() == WL_CONNECTED) {
    t1 = millis();
    handle_update();
    Serial.printf("[DEBUG] handle_update() mất %lu ms\n", millis() - t1);
    d = millis() - t1;
    t1 = millis();
    handle_popup();
    Serial.printf("[DEBUG] handle_popup() mất %lu ms\n", millis() - t1);
    e = millis() - t1;
    if (Show_Tim || Show_ngungon || Show_reminder || Show_hun|| Show_khang_reminder) {
      Serial.println("[DEBUG] Có popup đang hiển thị => bỏ qua handle_main()");
      e = 0;
      return;
    }
    t1 = millis();
    handle_main();
    Serial.printf("[DEBUG] handle_main() mất %lu ms\n", millis() - t1);
    f = millis() - t1;
    // Chỉ check WiFi mỗi WIFI_CHECK_INTERVAL
    if (millis() - lastWiFiCheck >= WIFI_CHECK_INTERVAL) {
      lastWiFiCheck = millis();
      t1 = millis();
      checkAndReconnectWiFi();
      Serial.printf("[DEBUG] checkAndReconnectWiFi() mất %lu ms\n", millis() - t1);
      g = millis() - t1;
    } else {
      Serial.printf("[DEBUG] checkAndReconnectWiFi() đang đợi ");
      g = 0;
    }
    yield();  // Cho ESP nghỉ xíu
      
      
      //// debug
while (Serial.available() > 0) {
    char c = Serial.read();

    if (c >= '0' && c <= '9') {         // Nếu là ký tự số
      if (buffer.length() < 3) {        // Giới hạn tối đa 3 chữ số
        buffer += c;
      }
    } 
    else if (c == '\n' || c == '\r') {  // Kết thúc khi xuống dòng
      if (buffer.length() > 0) {
        serialNumber = buffer.toInt();  // Chuyển thành số nguyên
        Serial.print("Nhận được: ");
        Serial.println(serialNumber);
        buffer = "";                    // Xóa buffer để đọc số mới
      }
    }
    else {
      buffer = ""; // Nếu gặp ký tự không hợp lệ thì reset
    }
  }

      //// debug
    Serial.printf("=== LOOP END - Tổng thời gian: %lu ms ===\n", millis() - loopStart);
    h = millis() - loopStart;

  

  }
}