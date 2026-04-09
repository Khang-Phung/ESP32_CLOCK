#define DS18B20_PIN D5  // GPIO
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

#define BLYNK_FIRMWARE_VERSION "1.2.4"  

#include <time.h>
#include <ESP8266WiFi.h>  //wifi
//#include <ESP8266WebServer.h>
#include <Wire.h>                //I2C
#include <LCDI2C_Multilingual.h> // Include the whole header file
#include <NTPClient.h>           // thư viện đồng hồ
#include <WiFiUdp.h>             // thư viện đồng hồ luôn
#include <TimeLib.h>             // Thêm thư viện này để dễ xử lý ngày tháng
#include <SPI.h>                 //thư viện giao tiếp spi
#include <SD.h>                  //thư viện sd card
#include <BlynkSimpleEsp8266.h>  // Thư viện Blynk
#include <FirebaseESP8266.h>     // thư viện giao tiếp firebase

#include <ESPAsyncTCP.h>            // ✅ Dùng cho ESP8266 (thay vì AsyncTCP của ESP32)
#include <ESPAsyncWebServer.h>      // ✅ Dùng được cho cả ESP32 và ESP8266
#include <DNSServer.h>
#include <LittleFS.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 14;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

float temperatureC = 0 ;


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


byte name0x4[] = { B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00011 };
byte name0x5[] = { B00000, B00000, B00001, B00111, B00111, B11111, B11111, B11111 };
byte name0x6[] = { B00000, B01111, B11111, B11111, B11111, B11111, B11111, B00000 };
byte name0x7[] = { B00000, B11000, B11111, B11111, B11111, B11111, B11111, B00111 };
byte name0x8[] = { B00000, B00011, B11111, B11111, B11111, B11111, B11111, B11100 };
byte name0x9[] = { B00000, B11110, B11111, B11111, B11111, B11111, B11111, B00000 };
byte name0x10[] = { B00000, B00000, B10000, B11100, B11100, B11111, B11111, B11111 };
byte name0x11[] = { B00000, B00000, B00000, B00000, B00000, B00000, B10000, B11000 };
byte name1x4[] = { B00011, B00001, B00000, B00000, B00000, B00000, B00000, B00000 };
byte name1x5[] = { B11111, B11111, B11111, B01111, B00111, B00001, B00000, B00000 };
byte name1x6[] = { B11111, B11111, B11111, B11111, B11111, B11111, B01111, B00000 };
byte name1x7[] = { B11000, B11111, B11111, B11111, B11111, B11111, B11111, B00000 };
byte name1x8[] = { B00011, B11111, B11111, B11111, B11111, B11111, B11111, B00000 };
byte name1x9[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11110, B00000 };
byte name1x10[] = { B11111, B11111, B11111, B11110, B11100, B10000, B00000, B00000 };
byte name1x11[] = { B11000, B10000, B00000, B00000, B00000, B00000, B00000, B00000 };


byte Bname0x5[] = { B00000, B00000, B00000, B00001, B00111, B01111, B01111, B01111 };
byte Bname0x6[] = { B00111, B01111, B11111, B11111, B11111, B11111, B11111, B00000 };
byte Bname0x7[] = { B11100, B11100, B11111, B11111, B11111, B00111, B00000, B00000 };
byte Bname0x8[] = { B00111, B00111, B11111, B11111, B11111, B11100, B00000, B00000 };
byte Bname0x9[] = { B11100, B11110, B11111, B11111, B11111, B11111, B11111, B00000 };
byte Bname0x10[] = { B00000, B00000, B00000, B10000, B11100, B11110, B11110, B11110 };
byte Bname1x5[] = { B01111, B00111, B00111, B00111, B00001, B00000, B00000, B00000 };
byte Bname1x6[] = { B11111, B11111, B11111, B11111, B11111, B00111, B00000, B00000 };
byte Bname1x7[] = { B11110, B11110, B11111, B11111, B11111, B11111, B11111, B00111 };
byte Bname1x8[] = { B01111, B01111, B11111, B11111, B11111, B11111, B11111, B11100 };
byte Bname1x9[] = { B11111, B11111, B11111, B11111, B11111, B11100, B00000, B00000 };
byte Bname1x10[] = { B11110, B11100, B11100, B11100, B10000, B00000, B00000, B00000 };*/

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
const char* ssids[] = {"leonmusic", "The PTKs_2.4Ghz", "Leonstudio" };
const char* passwords[] = {"leonhouse", "khanhphung0807", ""};

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


LCDI2C_Vietnamese lcd(0x27, 16, 2);
DNSServer dns;
AsyncWebServer server(80);


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
bool Show_hun = false;
bool Show_reminder = false;
///// hiển thị và âm thanh
bool LCD_Displayed = false;  // Biến cờ để kiểm tra đã clear chưa
bool tone_buzzed = true;
char Lip_status = 0;
bool showLIP1 = true;
bool showLIP2 = true;
///// biến led ảo
bool ON_V1 = false;  //V10
bool ON_V2 = false;  //V11
bool ON_V3 = false;  //V12
bool notification = false;
///// biến thời gian
static bool first_display = true;
static unsigned long last_update_time = 0;
unsigned long DisplayStartTime = 0;
const unsigned int DisplayDuration = 3000;
const unsigned int DisplayDuration_reminder = 7000;
const unsigned long reminderDuration_repeat = 1200000;
unsigned long pre_time_audio = 0;
unsigned long lastUpdateEpoch = 0;
unsigned long lastUpdateTime = 0;
unsigned long lastUpdateTime_blynk = 0;
unsigned long lastUpdateTime_firebase = 0;
unsigned long Lasttime_Scroll = 0;
unsigned long reminder_repeat_start = 0;
unsigned long pretime_lip1 = 0;
unsigned long pretime_lip2 = 0;
unsigned long fps_last = 0;
const unsigned int Scroll_duration = 300;  // tốc độ cuộn (ms)
const unsigned int updateInterval = 1000;
const unsigned int updateInterval_blynk = 1000;      // Cập nhật mỗi 1 giây hiển thị đồng hồ
const unsigned int updateInterval_firebase = 30000;  // Cập nhật mỗi 30 giây firebase
const unsigned int AudioInterval = 300;              // phát âm thanh khoảng 0.3
unsigned long tingStart = 0;
char tingStep = 0;
bool tingPlaying = false;
bool reminder_repeat = true;

unsigned long noteStart = 0;
char twinkleNote = 0;
bool isPlayingTwinkle = true;
bool noteisPlayingTwinkle = false;

///// biến nút vật lý
bool PRESS = false;
bool HOLD = false;
char Program_sel = 0;
char last_program_sel = -1;  // khởi đầu khác để đảm bảo lần đầu in ra
bool enable_display = false;
bool sound_button = false;
bool Button_press = false;

enum PopupType { NONE, TIM, NGUNGON, REMINDER, HUN};
PopupType CurrentPopup = NONE;

bool wifiConnecting = false;
bool wifiConnected = false;
bool wifiFailed = false;
unsigned long wifiTryStart = 0;




void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  //sensors.begin();
  Wire.begin();
  Wire.setClock(400000);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
   lcd.print("Chờ xíu nha! ...");
  if (!LittleFS.begin()) {
  Serial.println("LittleFS mount failed!");
} else {
  Serial.println("LittleFS mounted.");
}

  Serial.println("Listing files on LittleFS:");
Dir dir = LittleFS.openDir("/");
while (dir.next()) {
  Serial.println(dir.fileName());
}

  wifi_set_phy_mode(PHY_MODE_11G);  // 802.11g (6-54 Mbps)	Tốc độ trung bình, phổ biến
  connect_wifi();
  lcd.setCursor(0, 0);
  lcd.print("Kết nối xong òi!");
  lcd.setCursor(0, 1);
  lcd.print("Đang xử lý      ");
  ssid = WiFi.SSID().c_str();
  pass = WiFi.psk().c_str();
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
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
  get_calendar_firebase();
  Gio_temp = Gio_tieptheo;
  Phut_temp = Phut_tieptheo;
  Title_calendar_temp = Title_calendar_tieptheo;
  lcd.clear();
}

void loop() {
  Blynk.run();          // Chạy blynk
  checkWiFi();          // kiểm tra kết nối wifi
  timeClient.update();  // cập nhật thời gian
   if (millis() - lastUpdateEpoch >= updateInterval ){
    lastUpdateEpoch = millis();
  time_t rawTime = timeClient.getEpochTime();
        Gio = hour(rawTime);
        Phut = minute(rawTime);
        Giay = second(rawTime);
        dayVal = day(rawTime);
        monthVal = month(rawTime);
        yearVal = year(rawTime);
  rssi = WiFi.RSSI(); 
  }
//////////////////////////////////////////////
if (Phut == 0 && Giay == 0 )
  switch (Gio) {
    case 3:
      ESP.restart();
      break;
    case 13:
      ESP.restart();
      break;
    default:
      break;
  }
/////////////////////////////////////////////////
  // Gọi kiểm tra nút nhấn

  if (Serial.available() > 0) {  // Nếu có dữ liệu được gửi đến
    char key = Serial.read();    // Đọc ký tự từ Serial

    if (key == 'a') {
    Button_press = true;
    }
  }

  if (debounceButton(BUTTON_PIN)||Button_press ) {  //check nút
    Button_press = false;
    enable_display = true;
    Program_sel = Program_sel + 1;
    if (Program_sel > 2) Program_sel = 0;  // quay lại chế độ 0
    Serial.println("Nút được nhấn (nhấn nhanh)");
    sound_button = true;
  }

  

    if(sound_button){
      playButtonPressTone();
    }
  
      // Thêm hành động của bạn ở đây

  if (WiFi.status() == WL_CONNECTED) {

    handle_update();
    handle_popup();
    if (Show_Tim || Show_ngungon || Show_reminder ||Show_hun){
      return;
    }
    handle_main();
  


  }  //vòng có kết nối wifi
}  //loop