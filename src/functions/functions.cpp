#include "functions.h"



// Color RGB888 (RRGGBB) -> RGB565
uint16_t getColor(String hex) {
    hex.trim();

    if (hex.startsWith("#"))
        hex.remove(0, 1);

    if (hex.startsWith("0x") || hex.startsWith("0X"))
        hex.remove(0, 2);

    if (hex.length() != 6)
        return ST77XX_WHITE;

    char *end;
    unsigned long rgb = strtoul(hex.c_str(), &end, 16);

    if (*end != '\0')
        return ST77XX_WHITE;

    uint8_t r = (rgb >> 16) & 0xFF;
    uint8_t g = (rgb >> 8) & 0xFF;
    uint8_t b = rgb & 0xFF;

    return tft.color565(r, g, b);
}

bool getBoolArg(String name, bool defaultValue) {
    if (!server.hasArg(name))
        return defaultValue;

    String value = server.arg(name);
    value.toLowerCase();

    return value == "1" ||
           value == "true" ||
           value == "yes";
}

int getIntArg(String name, int defaultValue) {
    if (!server.hasArg(name))
        return defaultValue;

    return server.arg(name).toInt();
}

void sendBadRequest(const char* message) {
    server.send(400, "text/plain", message);
}

void sendOK() {
    server.send(200, "text/plain", "OK");
}
int measureTextWidth(String text) {
    int16_t x1, y1;
    uint16_t w, h;

    tft.getTextBounds(text.c_str(), 0, 0, &x1, &y1, &w, &h);
    return w;
}