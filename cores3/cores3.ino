#pragma once
#include <M5CoreS3.h>
#include <WiFi.h>
#include <time.h>
#include <ArduinoMqttClient.h>

#define SSID "KOKI08-DYNABOOK 1420"
#define WIFI_PASS "653R0o0<"
//↑dummy ssid&pass

#define NTP_1 "ntp.nict.jp"
#define NTP_2 "ntp1.noc.titech.ac.jp"
#define NTP_3 "ntp.jst.mfeed.ad.jp"

#define MQTT_PORT 1883
#define MQTT_URL "test.mosquitto.org"
#define TOPIC_NAME_0 "hidden"
#define TOPIC_NAME_1 "hidden"

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


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
  }

  /* END WIFI-CONNECTION */

  /* BEGIN MQTT-CONNECTION */
  Serial.println("Connecting to MQTT broker...");
  if(!mqttClient.connect(MQTT_URL, MQTT_PORT)) {
    Serial.print("failed to connect MQTT broker, error code: ");
    Serial.println(mqttClient.connectError());
    
    while(1);
  }
  Serial.println("Connected to MQTT broker\n");
  /* END MQTT-CONNECTION */
}

void Main();

void loop() {
  mqttClient.poll();
  unsigned long currentTime = millis();
  static unsigned long previousTime = 0;

  if(currentTime - previousTime >= 2000) {
    previousTime = currentTime;
    Main();
  }
} 

void Main() {
  CoreS3.Display.fillScreen(TFT_BLACK);
  CoreS3.Display.setCursor(10, 50);
  CoreS3.Display.println("ステータス: 正常");
  CoreS3.Display.setCursor(10, 80);
  CoreS3.Display.printf("気温差: %0.2f °C", 0.223);
  CoreS3.Display.setCursor(10, 120);
  CoreS3.Display.printf("お湯の温度: %0.1f °C", 40.00);

}
