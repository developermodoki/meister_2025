#pragma once
#include <DHT.h>
#include <WiFi.h>
#include <time.h>


//#define SSID "KOKI08-DYNABOOK 1420"
//#define WIFI_PASS "653R0o0<"
//dummy ssid&pass
#define DHT_INPUT_PIN 33
#define DHT_TYPE DHT11
#define NTP_1 "ntp.nict.jp"
#define NTP_2 "ntp1.noc.titech.ac.jp"
#define NTP_3 "ntp.jst.mfeed.ad.jp"


DHT dht(DHT_INPUT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

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
      if(i <= 3) break;
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
}

void loop() {
  delay(2000);
  Serial.println("Reading Temperature.");

  float humid = dht.readHumidity();
  float temp = dht.readTemperature();

  
  if (isnan(humid) || isnan(temp)) {
    Serial.println("Failed reading");
    return;
  }

  Serial.printf("Temp: %.2f\n", temp);
  Serial.printf("Humid: %.2f\n", humid);
  
}
