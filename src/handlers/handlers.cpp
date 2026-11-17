#include "handlers.h"
#include "functions/functions.h"

void handleLineQuery() {
    enableDisplay(true);

    if (!server.hasArg("x1") || !server.hasArg("y1") ||
        !server.hasArg("x2") || !server.hasArg("y2")) {
        sendBadRequest("Missing coordinates");
        return;
    }

    int x1 = getIntArg("x1");
    int y1 = getIntArg("y1");
    int x2 = getIntArg("x2");
    int y2 = getIntArg("y2");

    uint16_t color = getColor(
        server.arg("color").length()
            ? server.arg("color") : "FFFFFF"
    );

    int thickness = constrain(getIntArg("thickness", 1), 1, 20);

    bool mostlyHorizontal = abs(x2 - x1) >= abs(y2 - y1);

    // Center the stack of lines on the requested coordinates
    int start = -(thickness - 1) / 2;

    for (int i = 0; i < thickness; i++) {
        int o = start + i;
        if (mostlyHorizontal) {
            // offset vertically
            tft.drawLine(x1, y1 + o, x2, y2 + o, color);
        } else {
            // offset horizontally
            tft.drawLine(x1 + o, y1, x2 + o, y2, color);
        }
    }

    sendOK();
}


void handleRectQuery() {
    enableDisplay(true);

    if (!server.hasArg("x") || !server.hasArg("y") ||
        !server.hasArg("w") || !server.hasArg("h")) {
        sendBadRequest("Missing rectangle dimensions");
        return;
    }

    int x = getIntArg("x");
    int y = getIntArg("y");
    int w = getIntArg("w");
    int h = getIntArg("h");

    if (w <= 0 || h <= 0) {
        sendBadRequest("Invalid dimensions");
        return;
    }

    uint16_t color = getColor(
        server.hasArg("color") ? server.arg("color") : "FFFFFF"
    );

    if (getBoolArg("fill")) {
        tft.fillRect(x, y, w, h, color);
    } else {
        int thickness = constrain(getIntArg("thickness", 1), 1, 20);

        for (int i = 0; i < thickness; i++) {
            if (w - 2 * i <= 0 || h - 2 * i <= 0)
                break;

            tft.drawRect(x + i, y + i, w - 2 * i, h - 2 * i, color);
        }
    }

    sendOK();
}



void handleCircleQuery() {
    enableDisplay(true);

    if (!server.hasArg("x") || !server.hasArg("y") ||
        !server.hasArg("r")) {
        sendBadRequest("Missing circle parameters");
        return;
    }

    int x = getIntArg("x");
    int y = getIntArg("y");
    int r = getIntArg("r");

    if (r <= 0) {
        sendBadRequest("Invalid radius");
        return;
    }

    uint16_t color = getColor(
        server.hasArg("color") ? server.arg("color") : "FFFFFF"
    );

    if (getBoolArg("fill")) {
        tft.fillCircle(x, y, r, color);
    } else {
        int thickness = constrain(getIntArg("thickness", 1), 1, 20);

        for (int i = 0; i < thickness && r - i > 0; i++) {
            tft.drawCircle(x, y, r - i, color);
        }
    }

    sendOK();
}


void handleTriangleQuery() {
    enableDisplay(true);

    if (!server.hasArg("x1") || !server.hasArg("y1") ||
        !server.hasArg("x2") || !server.hasArg("y2") ||
        !server.hasArg("x3") || !server.hasArg("y3")) {
        sendBadRequest("Missing triangle coordinates");
        return;
    }

    int x1 = getIntArg("x1"), y1 = getIntArg("y1");
    int x2 = getIntArg("x2"), y2 = getIntArg("y2");
    int x3 = getIntArg("x3"), y3 = getIntArg("y3");

    uint16_t color = getColor(
        server.hasArg("color") ? server.arg("color") : "FFFFFF"
    );

    if (getBoolArg("fill")) {
        tft.fillTriangle(x1, y1, x2, y2, x3, y3, color);
    } else {
        int thickness = constrain(getIntArg("thickness", 1), 1, 20);

        drawThickLine(x1, y1, x2, y2, thickness, color);
        drawThickLine(x2, y2, x3, y3, thickness, color);
        drawThickLine(x3, y3, x1, y1, thickness, color);

        // Fill the notches at the corners so joins look clean
        if (thickness > 1) {
            int cr = thickness / 2;
            tft.fillCircle(x1, y1, cr, color);
            tft.fillCircle(x2, y2, cr, color);
            tft.fillCircle(x3, y3, cr, color);
        }
    }

    sendOK();
}

void handlePixelQuery() {
    enableDisplay(true);

    if (!server.hasArg("x") || !server.hasArg("y")) {
        sendBadRequest("Missing coordinates");
        return;
    }

    tft.drawPixel(
        getIntArg("x"),
        getIntArg("y"),
        getColor(server.hasArg("color") ? server.arg("color") : "FFFFFF")
    );

    sendOK();
}




void handleTextQuery() {
    enableDisplay(true);

    if (!server.hasArg("x") || !server.hasArg("y") ||
        !server.hasArg("data")) {
        sendBadRequest("Missing text parameters");
        return;
    }

    int x = getIntArg("x");
    int y = getIntArg("y");
    int size = constrain(getIntArg("size", 1), 1, 5);

    String data = server.arg("data");
    uint16_t color = getColor(
        server.hasArg("color") ? server.arg("color") : "FFFFFF"
    );

    //tft.setTextSize(size);
    int lineHeight;

    if (size == 1) {
        tft.setFont(&FreeSans9pt7b);
        lineHeight = 13;
    } else if (size == 2) {
        tft.setFont(&FreeSans12pt7b);
        lineHeight = 18;
    } else if (size == 3) {
        tft.setFont(&FreeSans18pt7b);
        lineHeight = 26;
    }

    tft.setTextColor(color);
    tft.setTextWrap(false);

    
    int maxWidth = tft.width() - x;

    if (maxWidth <= 0) {
        sendBadRequest("X is outside the screen");
        return;
    }

    y = y + lineHeight;

    // Without wrap: all text in one row.
    if (!getBoolArg("wrap", true)) {
        tft.setCursor(x, y);
        tft.print(data);
        sendOK();
        return;
    }

    String line = "";
    int start = 0;

    while (start < data.length()) {
        // Skip space at the start of row.
        while (start < data.length() && data[start] == ' ')
            start++;

        if (start >= data.length())
            break;

        int end = data.indexOf(' ', start);

        if (end == -1)
            end = data.length();

        String word = data.substring(start, end);
        String candidate = line.length() ? line + " " + word : word;

        if (line.length() && measureTextWidth(candidate) > maxWidth) {
            tft.setCursor(x, y);
            tft.print(line);

            y += lineHeight + 3;
            line = word;
        } else {
            line = candidate;
        }

        start = end + 1;
    }

    if (line.length()) {
        tft.setCursor(x, y);
        tft.print(line);
    }

    sendOK();
}


void handleClearQuery() {
    uint16_t color = getColor(
        server.hasArg("color") ? server.arg("color") : "000000"
    );

    tft.fillScreen(color);

    if (color == 000000) {
        enableDisplay(false);
    } else {
        enableDisplay(true);
    }
    
    sendOK();
}
