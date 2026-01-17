#pragma once
#include <M5CoreS3.h>

#define SOUND_PIN 37

void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(9600);
  pinMode(SOUND_PIN, OUTPUT);

}

void Main(bool soundFlag);

void loop() {
  unsigned long currentTime = millis();
  static unsigned long previousTime = 0;
  static bool soundFlag = 1;

  if(currentTime - previousTime >= 2000) {
    previousTime = currentTime;
    soundFlag = !soundFlag;
    Main(soundFlag);
  }
  
} 

void Main(bool soundFlag) {
  digitalWrite(SOUND_PIN, (soundFlag ? HIGH : LOW));
}