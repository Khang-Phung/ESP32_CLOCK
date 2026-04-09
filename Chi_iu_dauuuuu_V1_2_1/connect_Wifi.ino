void connect_wifi() {
  bool connected = false;

  // 1. Thử kết nối từng WiFi đã lưu
  for (int i = 0; i < wifiCount; i++) {
    WiFi.begin(ssids[i], passwords[i]);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thu ket noi:");
    lcd.setCursor(0, 1);
    lcd.print(ssids[i]);

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry++ < 20) {
      delay(500);
      lcd.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Da ket noi!");
      delay(1000);
      return; // ✅ Thành công, thoát luôn
    }
  }

  // 2. Bật chế độ AP để cấu hình lại WiFi
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bat che do AP");
  lcd.setCursor(0, 1);
  lcd.print("WiFi_Config");

  WiFi.mode(WIFI_AP);
  WiFi.softAP("WiFi_Config");
  dns.start(53, "*", WiFi.softAPIP());

  Serial.print("ESP AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Cấu hình web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
    int n = WiFi.scanNetworks();
    String json = "[";
    for (int i = 0; i < n; i++) {
      if (i) json += ",";
      json += "\"" + WiFi.SSID(i) + "\"";
    }
    json += "]";
    request->send(200, "application/json", json);
  });

  // ✅ Xử lý nhận WiFi mới từ người dùng
  server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {
    String ssid = request->getParam("ssid", true)->value();
    String pass = request->getParam("pass", true)->value();

    request->send(200, "text/plain", "Dang ket noi toi " + ssid);

    // Bắt đầu thử kết nối
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dang ket noi...");
    lcd.setCursor(0, 1);
    lcd.print(ssid);

    // Đặt flag cho loop đợi ở dưới biết
    wifiConnecting = true;
    wifiTryStart = millis();
  });

  server.begin();

  // 3. Đợi cho đến khi WiFi thành công (do user cấu hình)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

    // Nếu đang trong giai đoạn thử kết nối mới
    if (wifiConnecting) {
      // Thử kết nối trong 10 giây
      if (millis() - wifiTryStart > 10000) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("That bai! Reset");
        delay(2000);
        ESP.restart();
      }

      // Thành công
      if (WiFi.status() == WL_CONNECTED) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Da ket noi!");
        delay(1000);
        break;
      }
    }
  }
}
