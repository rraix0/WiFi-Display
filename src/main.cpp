#include "wifi_conn/wifi_conn.h"
#include <FastLED.h>
#include <esp_sleep.h>
#include <Arduino.h>
#include <display/display.h>
#include <config/config.h>






void setup() {
  Serial.begin(115200);
  Serial.println("STARTING");
  pinMode(TFT_BL, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  

  bool on_screen = false;

  if (digitalRead(SWITCH) == LOW) {
    on_screen = true;
  }


  initDisplay();

  if (on_screen) {
    Serial.println("ON");
    Serial.println(DISPLAY_STATUS);
    tft.enableDisplay(true);
    updateBrightness();
    tft.fillScreen(ST77XX_BLACK);
    tft.setFont(&FreeSans18pt7b);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(35, 94);
    tft.print("ON");

    delay(1000);
    tft.fillScreen(ST77XX_BLACK);
  }

enableDisplay(false);

esp_deep_sleep_enable_gpio_wakeup(1ULL << SWITCH, ESP_GPIO_WAKEUP_GPIO_LOW);

  
  setup_wifi_connection();
  delay(200);
}
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) |
           ((g & 0xFC) << 3) |
           (b >> 3);
}

void loop() {
    EVERY_N_SECONDS(5) {
        wifi_connection();
    }

EVERY_N_MILLIS(100) {
    if (digitalRead(SWITCH) == LOW) {
        delay(40);

        if (digitalRead(SWITCH) == LOW) {
            DISPLAY_STATUS = !DISPLAY_STATUS;

            Serial.println(DISPLAY_STATUS);

            int timer = 0;

            while (digitalRead(SWITCH) == LOW) {
                delay(10);
                timer ++;
                if (timer == 100) {
                    tft.enableDisplay(true);
                    tft.fillScreen(ST77XX_BLACK);
                    tft.setFont(&FreeSans18pt7b);
                    tft.setTextColor(ST7735_WHITE);
                    tft.setCursor(28, 94);
                    tft.print("OFF");
                    delay(50);
                    updateBrightness();
                    Serial.println("Turning off in 3 seconds.");
                    delay(3000);
                    tft.fillScreen(ST77XX_BLACK);
                    Serial.println("OFF");
                    esp_deep_sleep_start();
                }
            }
        }
    }

    if (DISPLAY_STATUS) {
        tft.enableDisplay(true);
        delay(50);
        updateBrightness();
    } else {
        ledcWrite(TFT_BL, 0);
        tft.enableDisplay(false);
    }
}


    server.handleClient();
    
    
    delay(30);
}