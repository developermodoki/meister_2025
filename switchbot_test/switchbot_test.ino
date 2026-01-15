#pragma once
#include <M5CoreS3.h>
#include <WiFi.h>
#include <time.h>
#include <WiFiClientSecure.h>
#include "root_ca_switchbot.cpp"

#define SSID "KOKI08-DYNABOOK 1420"
#define WIFI_PASS "653R0o0<"
//↑dummy ssid&pass

#define NTP_1 "ntp.nict.jp"
#define NTP_2 "ntp1.noc.titech.ac.jp"
#define NTP_3 "ntp.jst.mfeed.ad.jp"

WiFiClient wifiClient;

void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(9600);

  CoreS3.Display.setTextSize(1);
  CoreS3.Display.setTextFont(&fonts::lgfxJapanGothic_32);

  // https://github.com/developermodoki/meister_2024/blob/main/atoms3_main/atoms3_main.ino#L205C5-L205C8
  /* BEGIN WIFI-CONNECTION */
  Serial.println("Starting Wi-Fi Connection...");
  WiFi.begin(SSID, WIFI_PASS);
  delay(2000);

  if(!WiFi.isConnected()) {
    int i = 0;
    while(WiFi.begin(SSID, WIFI_PASS) != WL_CONNECTED) {
      delay(2000);
      i++;
      if(i > 4) break;
    };
  }

  if(WiFi.isConnected()) {
    Serial.print("Wi-Fi Connected. Local IP is: ");
    Serial.println(WiFi.localIP());
    configTime(60 * 60 * 9, 0, NTP_1, NTP_2, NTP_3);
  }
  else {
    Serial.println("Wifi Connection failed.");
    while(1);
  }

  /* END WIFI-CONNECTION */
}


void Main();
String getData();

void loop() {
  unsigned long currentTime = millis();
  static unsigned long previousTime = 0;

  if(currentTime - previousTime >= 2000) {
    previousTime = currentTime;
    Main();
  }
} 

// https://qiita.com/Kento210/items/ca90bc981b1353d7e9ef
String getData() {
  WiFiClientSecure getClient;
  getClient.setCACert(root_ca);

  if(getClient.connect("api.switch-bot.com", 443)) {
    String request = "GET https://api.switch-bot.com/v1.0/devices/" + deviceId + "/status" + " HTTP/1.1\r\n" +
                  "Authorization:" + String(token) + "\r\n" + 
                  "Connection: close\r\n\r\n";
    
    getClient.print(request);
    getClient.flush();

    Serial.println(request);
    Serial.println(" ");

    String line;
    while (getClient.connected() || getClient_client.available()) {
      if (getClient.available()) {
        line = getClient.readStringUntil('\n');
      }
    }

    if(getClient) getClient.stop();
    return line;

  } 
  else {
    Serial.println("Server Connection Error");
    return "";
  }
}

void Main() {
  Serial.println(getData());
  
  CoreS3.Display.fillScreen(TFT_BLACK);
  CoreS3.Display.setCursor(10, 50);
  CoreS3.Display.printf("気温: %0.1f °C", 0.223);
  CoreS3.Display.setCursor(10, 120);
  CoreS3.Display.printf("湿度: %0.1f °C", 40.00);

}