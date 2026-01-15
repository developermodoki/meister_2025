#pragma once
#include <M5CoreS3.h>


void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(9600);

  CoreS3.Display.setTextSize(1);
  CoreS3.Display.setTextFont(&fonts::lgfxJapanGothic_32);
}

void Main();

void loop() {
  unsigned long currentTime = millis();
  static unsigned long previousTime = 0;

  if(currentTime - previousTime >= 2000) {
    previousTime = currentTime;
    Main();
  }
} 

char flag = 2;
void Main() {
  if(flag == 1) {
    CoreS3.Display.fillScreen(TFT_BLACK);
    CoreS3.Display.setCursor(10, 50);
    CoreS3.Display.println("ステータス: 正常");
    CoreS3.Display.setCursor(10, 80);
    CoreS3.Display.printf("気温差: %0.2f °C", 0.223);
    CoreS3.Display.setCursor(10, 120);
    CoreS3.Display.printf("湯の温度: %0.1f °C", 40.00);
  }
  else if(flag == 2) {
    CoreS3.Display.fillScreen(TFT_WHITE);
    CoreS3.Display.setTextColor(TFT_RED);
    CoreS3.Display.setCursor(10, 50);
    CoreS3.Display.println("警告: example");
  }

}
