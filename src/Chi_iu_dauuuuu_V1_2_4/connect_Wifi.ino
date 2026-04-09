void connect_wifi() {
  bool connected = false;
  bool web_server_1st = true;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Xóa kết nối cũ

  // 1. Thử các WiFi đã lưu
  for (int i = 0; i < wifiCount; i++) {
    WiFi.begin(ssids[i], passwords[i]);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thử kết nối:");
    lcd.setCursor(0, 1);
    lcd.print(ssids[i]);

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry++ < 15) {
      delay(500);
      lcd.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      return;
    }
  }

  // 2. Bật chế độ AP để người dùng cấu hình
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Chờ xíu nhe");
  lcd.setCursor(0, 1);
  lcd.print("Đang mở server á");

  WiFi.disconnect(true);  // Ngắt tất cả
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ap_ssid,ap_pass);

  dns.start(53, "*", WiFi.softAPIP()); // Bắt DNS captive

  Serial.print("ESP32 AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Trang chính
  server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  // Trả danh sách WiFi
server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
  int n = WiFi.scanNetworks();
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  response->print("[");
  for (int i = 0; i < n && i < 5; i++) {
    if (i > 0) response->print(",");
    response->printf("{\"ssid\":\"%s\",\"rssi\":%d,\"secure\":%s}",
                     WiFi.SSID(i).c_str(),
                     WiFi.RSSI(i),
                     WiFi.encryptionType(i) != ENC_TYPE_NONE ? "true" : "false");
    yield();  // giảm áp cho watchdog
  }
  response->print("]");
  request->send(response);
});



  // Nhận WiFi mới
  server.on("/wifi", HTTP_POST, [=](AsyncWebServerRequest *request) {
  
    if (!request->hasParam("ssid", true) || !request->hasParam("pass", true)) {
      request->redirect("/");
    return;
    }

     ssid = request->getParam("ssid", true)->value();
     pass = request->getParam("pass", true)->value();

    

    WiFi.begin(ssid.c_str(), pass.c_str());

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Đang kết nối ...");
    lcd.setCursor(0, 1);
    lcd.print(ssid);

    wifiConnecting = true;
  wifiConnected = false;
  wifiFailed = false;
  wifiTryStart = millis();
    // Trả về trang chờ đợi
  request->redirect("/wait");
  });
  server.on("/wait", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(LittleFS, "/Waiting.html", "text/html");
});

server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
  String status = "connecting";
  if (wifiConnected) status = "success";
  else if (wifiFailed) status = "failed";
  request->send(200, "text/plain", status);
});

server.on("/success", HTTP_GET, [](AsyncWebServerRequest *request){
   request->send(LittleFS, "/Success.html", "text/html");
});
  

  server.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dô wifi này nè:");
  lcd.setCursor(0, 1);
  lcd.print(ap_ssid);

  // 3. Chờ kết nối hoặc nhập lại nếu sai
  unsigned long Timeout = millis();
  while (true) {
    if (millis() - Timeout > 450000)
    {ESP.restart();}
    dns.processNextRequest();
    yield();
    delay(10);  // hoặc delay(1) là được rồi

    if (wifiConnecting) {
      if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
      wifiConnecting = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Đc òi nè!");
        lcd.setCursor(0, 1);
        lcd.print("Iuuuuuuuu");
        delay(3000);
        WiFi.softAPdisconnect(true);
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), pass.c_str());
        break;
      }

      // Nếu quá 10s mà không kết nối được
      if (millis() - wifiTryStart > 5000) {
         wifiFailed = true;
      wifiConnecting = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sai mật khẩu!");
        delay(2000);

        // Bật lại AP cho người dùng nhập lại
        WiFi.disconnect(true);
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ap_ssid,ap_pass);
        dns.start(53, "*", WiFi.softAPIP());
       lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nhập lại ii");

        wifiConnecting = false;
      }
    }
  }
}