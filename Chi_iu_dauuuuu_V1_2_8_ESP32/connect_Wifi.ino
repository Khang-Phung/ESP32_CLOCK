void connect_wifi() {
  bool connected = false;

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
    while (WiFi.status() != WL_CONNECTED && retry++ < WIFI_RETRY_COUNT) {
      delay(WIFI_TIMEOUT_MS);
      lcd.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      return; // Thành công → thoát
    }
  }

  // 2. Không thành công → bật AP + Captive Portal
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mở sever nè");
  lcd.setCursor(0, 1);
  lcd.print("Đợi xíuu...");

  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ap_ssid, ap_pass);

  dns.start(53, "*", WiFi.softAPIP()); // Bắt DNS captive portal

  Serial.print("ESP32 AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Route chính
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  // Route trả danh sách WiFi
  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
    int n = WiFi.scanNetworks();
    String json = "[";
    int count = 0;

    for (int i = 0; i < n && count < 15; i++) {
      String ssidFound = WiFi.SSID(i);
      if (ssidFound.length() == 0) continue;
      if (count > 0) json += ",";
      int rssi = WiFi.RSSI(i);
      bool secure = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);

      json += "{";
      json += "\"ssid\":\"" + ssidFound + "\",";
      json += "\"rssi\":" + String(rssi) + ",";
      json += "\"secure\":" + String(secure ? "true" : "false");
      json += "}";

      count++;
      yield();
    }

    json += "]";
    WiFi.scanDelete();
    request->send(200, "application/json", json);
  });

  // Nhận WiFi mới
  server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (!request->hasParam("ssid", true) || !request->hasParam("pass", true)) {
      request->redirect("/");
      return;
    }

    ssid = request->getParam("ssid", true)->value();
    pass = request->getParam("pass", true)->value();

    WiFi.begin(ssid.c_str(), pass.c_str());

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Đang kết nối...");
    lcd.setCursor(0, 1);
    lcd.print(ssid);

    wifiConnecting = true;
    wifiConnected = false;
    wifiFailed = false;
    wifiTryStart = millis();

    request->redirect("/wait");
  });

  // Trang chờ
  server.on("/wait", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/Waiting.html", "text/html");
  });

  // Trạng thái
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    String status = "connecting";
    if (wifiConnected) status = "success";
    else if (wifiFailed) status = "failed";
    request->send(200, "text/plain", status);
  });

  // Thành công
  server.on("/success", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/Success.html", "text/html");
  });

  // Captive portal: bất kỳ request nào cũng trả về index.html
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.begin();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dô wifi này nè:");
  lcd.setCursor(0, 1);
  lcd.print(ap_ssid);

  // 3. Vòng lặp Captive Portal
  unsigned long Timeout = millis();
  while (true) {
    if (millis() - Timeout > AP_TIMEOUT_MS) {
      ESP.restart();
    }

    dns.processNextRequest();
    delay(100);

    if (wifiConnecting) {
      if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        wifiConnecting = false;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kết nối OK!");
        delay(2000);

        WiFi.softAPdisconnect(true);
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), pass.c_str());
        break;
      }

      // Nếu quá thời gian thử kết nối mới
      if (millis() - wifiTryStart > WIFI_RETRY_NEW) {
        wifiFailed = true;
        wifiConnecting = false;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sai mật khẩu á!");
        delay(2000);

        // Bật lại AP để nhập lại
        WiFi.disconnect(true);
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ap_ssid, ap_pass);
        dns.start(53, "*", WiFi.softAPIP());
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nhập lại ii...");
      }
    }
  }
}