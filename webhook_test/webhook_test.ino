#pragma once
#include <M5CoreS3.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <time.h>

#define SSID "KOKI08-DYNABOOK 1420"
#define WIFI_PASS "653R0o0<"
//↑dummy ssid&pass

#define NTP_1 "ntp.nict.jp"
#define NTP_2 "ntp1.noc.titech.ac.jp"
#define NTP_3 "ntp.jst.mfeed.ad.jp"

#define DISCORD_URL ""



void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  CoreS3.Power.begin();

  Serial.begin(9600);


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

// https://qiita.com/Kento210/items/ca90bc981b1353d7e9ef#3-post%E3%82%92%E5%AE%9F%E8%A3%85%E3%81%97%E3%81%A6%E3%81%BF%E3%82%8B
void loop() {  
  WiFiClientSecure client;
  client.setInsecure();
  
  if(client.connect("discord.com", 443)) {
    String postData = "{\"content\": \"hello\"}";
    String request = "POST " + String(DISCORD_URL) + " HTTP/1.1\r\n" +
                     "Host: discord.com" + "\r\n" +
                     "Content-Type: application/json\r\n" +
                     "Connection: close\r\n" +
                     "Content-Length: " + String(postData.length()) + "\r\n\r\n" +
                     postData;
    
    client.print(request);
    Serial.println("Request sent");
    while (client.connected() || client.available()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("body :");
      }
      if (line.length() > 0) {
        Serial.println(line);
      }
    }
    client.stop();
  }
  else {
    Serial.println("failed");
  }

  delay(5000);



}
