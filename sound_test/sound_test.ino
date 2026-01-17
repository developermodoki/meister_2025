#pragma once
#include <M5CoreS3.h>


void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  CoreS3.Power.begin();
  Serial.begin(9600);

  CoreS3.Speaker.setVolume(64);

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
  if(soundFlag) {
    CoreS3.Speaker.tone(4000);
  }
  else {
    CoreS3.Speaker.stop();
  }
}