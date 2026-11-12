#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>
#include "esp_wifi.h"
#include "esp_sleep.h"
#include "handlers/handlers.h"

#define CONNECTION_TIMEOUT 5
#define DEEP_SLEEP_DURATION_NO_WIFI 10
#define DEEP_SLEEP_DURATION 10


extern const char* wifi_ssid;
extern const char* wifi_password;

extern IPAddress local_IP;
extern IPAddress gateway;
extern IPAddress subnet;
extern IPAddress primaryDNS;
extern IPAddress secondaryDNS;

extern WebServer server;

void wifi_connection();
void handleLedQuery();
void handleAlertQuery();
void setup_wifi_connection();