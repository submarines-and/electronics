#include "gfx.h"
#include <Fonts/FreeSans12pt7b.h>

void setup()
{
    Serial.begin(115200);
    while (!Serial) {
    }

    initDisplay();

    do {
        display.fillScreen(GxEPD_WHITE);
        drawText(BORDER_MARGIN, BORDER_MARGIN + 40, "Black text", &FreeSans12pt7b, GxEPD_BLACK);
        drawText(BORDER_MARGIN, BORDER_MARGIN + 80, "Red text", &FreeSans12pt7b, GxEPD_RED);
    } while (display.nextPage());
}

void loop()
{
}