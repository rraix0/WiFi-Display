#pragma once

// WiFi setup
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// WiFi Configuration
#define WIFI_LOCAL_IP IPAddress(192, 168, 0, 35) // Ip will be used to send HTTP requests. Remember to configure DHCP on your router, otherwise this ip might be assigned to other devices!
#define WIFI_GATEWAY IPAddress(192, 168, 0, 1)
#define WIFI_SUBNET IPAddress(255, 255, 255, 0)
#define WIFI_PRIMARY_DNS IPAddress(1, 1, 1, 1)
#define WIFI_SECONDARY_DNS IPAddress(1, 0, 0, 1)
#define HTTP_SERVER_PORT 80

// Display size + driver settings
#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT  160
#define DISPLAY_ROTATION 2 // 2 -> portrait upside down
#define TFT_INIT_TYPE INITR_BLACKTAB //If doesn't work try INITR_GREENTAB / INITR_REDTAB or check out Adafruit_GFX docs for Adafruit_ST7735

// Switch - ON / OFF / SLEEP
#define SWITCH 2

// TFT display pins
#define TFT_SCK  4   // GPIO4 (SCK)
#define TFT_MOSI 5   // GPIO5 (MOSI)
#define TFT_CS   7   // GPIO7 (CS)
#define TFT_DC   3   // GPIO3 (DC)
#define TFT_RST  1   // GPIO1 (TX)
#define TFT_BL   10  // GPIO10 (BL)

// Potentiometer pin - brightness
#define POT_PIN  0  //GPIO0