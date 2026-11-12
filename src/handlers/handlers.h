#pragma once
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "functions/functions.h"
#include "display/display.h"
#include "wifi_conn/wifi_conn.h"
#include "functions/functions.h"

void handleLineQuery();
void handleRectQuery();
void handleCircleQuery();
void handleTriangleQuery();
void handlePixelQuery();
void handleTextQuery();
void handleClearQuery();