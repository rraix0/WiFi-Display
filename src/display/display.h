#pragma once
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define POT_PIN  0

#define TFT_SCK  4
#define TFT_MOSI 5
#define TFT_CS   7
#define TFT_DC   3
#define TFT_RST  1
#define TFT_BL   10

extern bool DISPLAY_STATUS;

extern Adafruit_ST7735 tft;

void initDisplay();

void updateBrightness();
void enableDisplay(bool);