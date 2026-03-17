#include "gfx.h"
#include "img.h"
#include <Fonts/FreeSans12pt7b.h>

void setup()
{
    Serial.begin(115200);
    while (!Serial) {
    }

    initDisplay(3);

    do {
        display.fillScreen(GxEPD_WHITE);
        display.drawBitmap(0, 0, epd_bitmap_IMG_0603, 528, 880, GxEPD_BLACK);

    } while (display.nextPage());
}

void loop()
{
}