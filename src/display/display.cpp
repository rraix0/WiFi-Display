#include "display.h"

#include <Fonts/FreeSans9pt7b.h>

Adafruit_ST7735 tft(&SPI, TFT_CS, TFT_DC, TFT_RST);
bool DISPLAY_STATUS = false;

void initDisplay() {

  SPI.begin(TFT_SCK, -1, TFT_MOSI, TFT_CS);  // MISO unused

  ledcAttach(TFT_BL, 5000, 8);
  ledcWrite(TFT_BL, 0);
  tft.setFont(&FreeSans9pt7b);
  tft.initR(INITR_BLACKTAB);   // Try INITR_GREENTAB / INITR_REDTAB
  tft.setRotation(DISPLAY_ROTATION);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);
  //tft.setTextSize(2);

  
  tft.enableDisplay(DISPLAY_STATUS);
}
void updateBrightness() {
  int potValue = analogRead(POT_PIN);
  int brightness = map(potValue, 0, 3300, 0, 255);

  ledcWrite(TFT_BL, brightness);
}

void enableDisplay(bool value) {
  DISPLAY_STATUS = value;
}