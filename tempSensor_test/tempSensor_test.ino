#pragma once
#include <M5CoreS3.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPSENSOR_PORT 14

OneWire ds(TEMPSENSOR_PORT);
DallasTemperature sensors(&ds);

void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(9600);
  sensors.begin();
}


void loop() {
  unsigned long currentTime = millis();
  static unsigned long previousTime = 0;

  if(currentTime - previousTime >= 2000) {
    previousTime = currentTime;
    Main();
  }
  
} 

void Main() {
  sensors.requestTemperatures();

  float temp = sensors.getTempCByIndex(0);
  Serial.print("Water temperature is: ");
  Serial.println(temp);

}
