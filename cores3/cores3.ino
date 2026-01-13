#include <M5CoreS3.h>


void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  int textsize = CoreS3.Display.height() / 60;
  if (textsize == 0) {
      textsize = 1;
  }
  CoreS3.Display.setTextSize(textsize);
}

void loop() {
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 50);
  M5.Display.println("Boston");
  delay(2000);
} 