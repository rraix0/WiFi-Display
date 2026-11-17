#pragma once
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <config/config.h>

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>

extern bool DISPLAY_STATUS;

extern Adafruit_ST7735 tft;

void initDisplay();

void updateBrightness();
void enableDisplay(bool);