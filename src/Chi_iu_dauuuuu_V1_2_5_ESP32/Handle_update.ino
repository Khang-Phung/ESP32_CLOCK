
 
void handle_update() {
unsigned long currentMillis = millis();
    //////////////////////////Update firebase///////////////////////////////
    get_calendar_firebase();
    //////////////////////////Update blynk///////////////////////////////
    if (currentMillis - lastUpdateTime_blynk >= updateInterval_blynk) {
      lastUpdateTime_blynk = currentMillis;
      Blynk.virtualWrite(V0, rssi);
      /*Serial.print(Gio_hientai);
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
      Serial.println(notification);*/
    }
}

void getWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=Hanoi,vn&appid=YOUR_API_KEY&units=metric");
    
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      float temp = doc["main"]["temp"]; // Nhiệt độ
      const char* weather = doc["weather"][0]["description"]; // Mô tả

      Serial.printf("Nhiet do ngoai troi: %.1f°C\n", temp);
      Serial.printf("Thoi tiet: %s\n", weather);
    } else {
      Serial.println("Loi khi goi API!");
    }

    http.end();
  }
}
