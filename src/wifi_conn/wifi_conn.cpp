#include "wifi_conn.h"
#include <new>
#include <cstring>
#include "../display/display.h"

const char *wifi_ssid = WIFI_SSID;
const char *wifi_password = WIFI_PASSWORD;

WebServer server(HTTP_SERVER_PORT);

IPAddress local_IP(WIFI_LOCAL_IP);
IPAddress gateway(WIFI_GATEWAY);
IPAddress subnet(WIFI_SUBNET);
IPAddress primaryDNS(WIFI_PRIMARY_DNS);
IPAddress secondaryDNS(WIFI_SECONDARY_DNS);

constexpr int WIDTH = DISPLAY_WIDTH;
constexpr int HEIGHT = DISPLAY_HEIGHT;
int IMG_WIDTH = 128;
int IMG_HEIGHT = 160;
int POS_X = 0;
int POS_Y = 0;
constexpr size_t PIXEL_COUNT = WIDTH * HEIGHT;

uint16_t* bitmap = nullptr;

uint8_t pixelByte[2];
int byteIndex = 0;
size_t pixelIndex = 0;

void handleFileUpload() {
  enableDisplay(true);

  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    Serial.println("===== HTTP PARAMETERS =====");

Serial.printf("hasArg(w): %s\n",
              server.hasArg("w") ? "YES" : "NO");

Serial.printf("hasArg(h): %s\n",
              server.hasArg("h") ? "YES" : "NO");

Serial.printf("w: %s\n",
              server.arg("w").c_str());

Serial.printf("h: %s\n",
              server.arg("h").c_str());


    IMG_WIDTH = server.hasArg("w") ? server.arg("w").toInt() : 128;
    IMG_HEIGHT = server.hasArg("h") ? server.arg("h").toInt() : 160;
    POS_X = server.hasArg("x") ? server.arg("x").toInt() : 0;
    POS_Y = server.hasArg("y") ? server.arg("y").toInt() : 0;

    if (IMG_WIDTH < 1 || IMG_WIDTH > WIDTH ||
        IMG_HEIGHT < 1 || IMG_HEIGHT > HEIGHT ||
        POS_X < 0 || POS_Y < 0 ||
        POS_X + IMG_WIDTH > WIDTH ||
        POS_Y + IMG_HEIGHT > HEIGHT) {
        Serial.println("Invalid dimensions or position!");
        return;
    }

    memset(bitmap, 0, PIXEL_COUNT * sizeof(uint16_t));

    if (bitmap == nullptr) {
      Serial.println("Not enough memory for the bitmap!");
      return;
    }
    byteIndex = 0;
    pixelIndex = 0;

    Serial.printf("Receiving file: %s\n",
                  upload.filename.c_str());
  }
  else if (upload.status == UPLOAD_FILE_WRITE) {
    if (bitmap == nullptr) {
      return;
    }

    for (size_t i = 0; i < upload.currentSize; i++) {
      pixelByte[byteIndex++] = upload.buf[i];

      if (byteIndex == 2) {
        size_t imgPixelCount = (size_t)IMG_WIDTH * IMG_HEIGHT;

        if (pixelIndex < imgPixelCount) {
          // The file contains MSB first, followed by LSB.
          bitmap[pixelIndex] =
              ((uint16_t)pixelByte[0] << 8) |
              pixelByte[1];

          pixelIndex++;
        }

        byteIndex = 0;
      }
    }
  }


else if (upload.status == UPLOAD_FILE_END) {
    size_t expectedPixels = (size_t)IMG_WIDTH * IMG_HEIGHT;
    size_t expectedBytes = expectedPixels * sizeof(uint16_t);

    Serial.println("\n===== UPLOAD END =====");

    Serial.printf("upload.totalSize: %u B\n",
                  (unsigned int)upload.totalSize);

    Serial.printf("Received pixels: %u\n",
                  (unsigned int)pixelIndex);

    Serial.printf("Expected pixels: %u\n",
                  (unsigned int)expectedPixels);

    Serial.printf("Expected bytes: %u B\n",
                  (unsigned int)expectedBytes);

    Serial.printf("Remaining byteIndex: %d\n", byteIndex);

    Serial.printf("Bitmap memory size: %u B\n",
                  (unsigned int)(PIXEL_COUNT * sizeof(uint16_t)));

    Serial.printf("IMG_WIDTH x IMG_HEIGHT: %d x %d\n",
                  IMG_WIDTH, IMG_HEIGHT);

    Serial.printf("Pixel count check: %s\n",
                  pixelIndex == expectedPixels ? "OK" : "ERROR");

    Serial.printf("Byte count check: %s\n",
                  byteIndex == 0 ? "OK" : "ERROR");

    Serial.printf("File size check: %s\n",
                  upload.totalSize == expectedBytes ? "OK" : "ERROR");

    if (pixelIndex == expectedPixels &&
        byteIndex == 0 &&
        upload.totalSize == expectedBytes) {

        Serial.println("[OK] RGB565 bitmap ready!");

        tft.drawRGBBitmap(
            POS_X, POS_Y,
            bitmap,
            IMG_WIDTH, IMG_HEIGHT
        );
    } else {
        Serial.println("[ERROR] Invalid bitmap size!");

        Serial.printf(
            "Pixel difference: %lld\n",
            (long long)pixelIndex - (long long)expectedPixels
        );
    }
  }





  else if (upload.status == UPLOAD_FILE_ABORTED) {
    byteIndex = 0;
    pixelIndex = 0;
    tft.fillScreen(ST77XX_BLACK);

    Serial.println("Upload aborted!");
  }
}

void setup_wifi_connection() {
    bitmap = new uint16_t[PIXEL_COUNT];
    if (bitmap == nullptr) {
        Serial.println("Not enough memory for the bitmap!");
    } else {
        memset(bitmap, 0, PIXEL_COUNT * sizeof(uint16_t));
        Serial.println("Bitmap allocated!");
    }


    Serial.println("Setting up WiFi");
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("IP configuration error");
    }

    WiFi.mode(WIFI_STA); // Optional
    WiFi.begin(wifi_ssid, wifi_password);

    server.on(
      "/upload",
      HTTP_POST,
      []() {
          server.send(200, "text/plain", "OK");
      },
      handleFileUpload
    );
    server.on("/line",     HTTP_POST, handleLineQuery);
    server.on("/rect",     HTTP_POST, handleRectQuery);
    server.on("/circle",   HTTP_POST, handleCircleQuery);
    server.on("/triangle", HTTP_POST, handleTriangleQuery);
    server.on("/pixel",    HTTP_POST, handlePixelQuery);
    server.on("/text",     HTTP_POST, handleTextQuery);
    server.on("/clear",    HTTP_POST, handleClearQuery);
    server.begin();
}


void wifi_connection(){
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost, reconnecting...");
        WiFi.disconnect();
        WiFi.begin(wifi_ssid, wifi_password);

        Serial.println("\nConnecting");

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nReconnected!");
            Serial.println(WiFi.localIP());
        } else {
            Serial.println("\nCould not connect to WiFi");
        }
    }
      int sila = WiFi.RSSI();

    Serial.print("Signal strength: ");
    Serial.print(sila);
    Serial.println(" dBm");
}