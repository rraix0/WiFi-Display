#pragma once
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "display/display.h"
#include "wifi_conn/wifi_conn.h"

uint16_t getColor(String hex);
bool getBoolArg(String name, bool defaultValue = false);
int getIntArg(String name, int defaultValue = 0);
void sendBadRequest(const char* message);
void sendOK();
int measureTextWidth(String text);