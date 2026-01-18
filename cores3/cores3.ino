#pragma once
#include <M5CoreS3.h>
#include <WiFi.h>
#include <time.h>
#include <ArduinoMqttClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <cmath>
#include <string>

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
#define WARN_TOPIC "hidden"


#define TEMPSENSOR_PORT 14


OneWire ds(TEMPSENSOR_PORT);
DallasTemperature sensors(&ds);

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  CoreS3.Power.begin();

  Serial.begin(9600);

  sensors.begin();

  CoreS3.Display.setTextSize(1);
  CoreS3.Display.setTextFont(&fonts::lgfxJapanGothic_32);

  CoreS3.Speaker.setVolume(64);

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

  /* BEGIN MQTT-CONNECTION */
  Serial.println("Connecting to MQTT broker...");
  if(!mqttClient.connect(MQTT_URL, MQTT_PORT)) {
    Serial.print("failed to connect MQTT broker, error code: ");
    Serial.println(mqttClient.connectError());
    while(1);
  }
  Serial.println("Connected to MQTT broker\n");
  mqttClient.subscribe(TOPIC_NAME_1);
  mqttClient.subscribe(TOPIC_NAME_0);
  /* END MQTT-CONNECTION */

}


void loop() {
  mqttClient.poll();
  unsigned long currentTime = millis();
  static unsigned long previousTime = 0;

  unsigned long currentTime2 = millis();
  static unsigned long previousTime2 = 0;

  static float temp_1 = 0;
  static float temp_0 = 0;
  char flag = 1;
  float diff;
  static bool isWarn = 0;

  int msgSize = mqttClient.parseMessage();

  if(msgSize) {
    String topic = mqttClient.messageTopic();
    Serial.print("Received a message from:");
    Serial.println(topic);

    String payload = "";
    while (mqttClient.available()) {
      payload += (char)mqttClient.read();
    }
    payload.trim();

    Serial.println(payload);

    if(topic.equals(TOPIC_NAME_1)) {
      temp_1 = payload.toFloat();
      Serial.println(temp_1);
    }
    else if(topic.equals(TOPIC_NAME_0)) {
      temp_0 = payload.toFloat();
      Serial.println(temp_0);
    }
  }


  if(currentTime - previousTime >= 1000) {
    previousTime = currentTime;
    sensors.requestTemperatures();

    float wTemp = sensors.getTempCByIndex(0);
    Serial.print("Water temperature is: ");
    Serial.println(wTemp);


    diff = fabs(temp_1 - temp_0);
    Serial.print("dif: ");
    Serial.println(diff);

    if(diff >= 10.0 && wTemp >= 42) {
      isWarn = 1;
      flag = 2;
      CoreS3.Speaker.tone(4000);
    }
    else if(diff >= 10.0) {
      isWarn = 1;
      flag = 3;
      CoreS3.Speaker.tone(4000);
    }
    else if(wTemp >= 42) {
      isWarn = 1;
      flag = 2;
      CoreS3.Speaker.tone(4000);
    }
    else {
      isWarn = 0;
      flag = 1;
      CoreS3.Speaker.stop();
    }

    Display(flag, diff, wTemp);
  }

  if(currentTime2 - previousTime2 >= 1000 * 10) {
    previousTime2 = currentTime2;
    if(isWarn) {
      mqttClient.beginMessage(WARN_TOPIC);
      mqttClient.print("WARNING");
      mqttClient.endMessage();
    }
  }


}


void Display(char flag, float absTemp, float wTemp) {
  if(flag == 1) {
    CoreS3.Display.fillScreen(TFT_BLACK);
    CoreS3.Display.setTextColor(TFT_WHITE);
    CoreS3.Display.setCursor(10, 50);
    CoreS3.Display.println("ステータス: 正常");
    CoreS3.Display.setCursor(10, 80);
    CoreS3.Display.printf("気温差: %0.2f °C", absTemp);
    CoreS3.Display.setCursor(10, 120);
    CoreS3.Display.printf("湯の温度: %0.1f °C", wTemp); 
  }
  else if(flag == 2) {
    CoreS3.Display.fillScreen(TFT_WHITE);
    CoreS3.Display.setTextColor(TFT_RED);
    CoreS3.Display.setCursor(10, 50);
    CoreS3.Display.print("警告: お湯の温度が高すぎます");
    CoreS3.Display.setCursor(10, 120);
    CoreS3.Display.print("41°C以下に下げてください"); 
  }
  else if(flag == 3) {
    CoreS3.Display.fillScreen(TFT_WHITE);
    CoreS3.Display.setTextColor(TFT_RED);
    CoreS3.Display.setCursor(10, 50);
    CoreS3.Display.print("警告: 部屋間の気温差が10°C以上です");
    CoreS3.Display.setCursor(10, 120);
    CoreS3.Display.printf("気温差: %0.1f °C", absTemp);
  }

}
