#include "wifi_conn/wifi_conn.h"
#include <FastLED.h>
#include <esp_sleep.h>
#include <Arduino.h>
#include <display/display.h>

#define SWITCH 18




void setup() {
  Serial.begin(115200);
  Serial.println("STARTING");
  pinMode(TFT_BL, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  //digitalWrite(TFT_BL, HIGH);

  initDisplay();


  setup_wifi_connection();
  delay(1000);
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

            while (digitalRead(SWITCH) == LOW) {
                delay(10);
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