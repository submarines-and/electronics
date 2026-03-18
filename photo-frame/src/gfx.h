#pragma once
#include <GxEPD2_3C.h>
#include <SPIFFS.h>

//  Pins for e-ink display
#define EPD_CS 15
#define EPD_DC 27
#define EPD_RST 26
#define EPD_BUSY 25
#define EPD_SCK 13
#define EPD_MOSI 14
using Panel = GxEPD2_750c_Z90;
const uint16_t PAGE_H = 64;
GxEPD2_3C<Panel, PAGE_H> display(Panel(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

/**
 * Run to init display
 *
 * Resolution ：528(H)x880(V) Pixel
 * Rotation (according to current frame mount): 3 = portrait, 4 = landscape
 */
void initDisplay(int16_t rotation)
{
    pinMode(0, INPUT_PULLUP);
    SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);
    display.init(0, true, 2, false);
    display.setRotation(rotation);
}

void drawText(int16_t x, int16_t y, const String& s, const GFXfont* f, uint16_t color)
{
    display.setFont(f);
    display.setTextColor(color);
    display.setCursor(x, y);
    display.print(s);
}

int16_t textWidth(const String& s, const GFXfont* f)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.setFont(f);
    display.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
    return w;
}

String clipToWidth(const String& s, const GFXfont* f, int16_t maxW, bool ellipsis = true)
{
    if (maxW <= 0)
        return "";
    if (textWidth(s, f) <= maxW)
        return s;
    if (!ellipsis) {
        String out = s;
        while (out.length() > 1 && textWidth(out, f) > maxW)
            out.remove(out.length() - 1);
        return out;
    }
    const String dots = "...";
    const int16_t dotsW = textWidth(dots, f);
    if (dotsW >= maxW)
        return "";
    String out = s;
    while (out.length() > 1 && textWidth(out + dots, f) > maxW)
        out.remove(out.length() - 1);
    return out + dots;
}

void drawRight(int16_t rightX, int16_t baselineY, const String& s, const GFXfont* f, uint16_t color)
{
    drawText(rightX - textWidth(s, f), baselineY, s, f, color);
}

inline void thickH(int x1, int y, int x2, uint16_t c)
{
    display.drawLine(x1, y, x2, y, c);
    display.drawLine(x1, y + 1, x2, y + 1, c);
}

inline void thickV(int x, int y1, int y2, uint16_t c)
{
    display.drawLine(x, y1, x, y2, c);
    display.drawLine(x + 1, y1, x + 1, y2, c);
}

void drawDegreeMark(int16_t cx, int16_t cy, int16_t outerR, uint16_t color)
{
    // E-ink can render 1px outlines very faintly; use a filled ring for contrast.
    outerR = max<int16_t>(2, outerR);
    display.fillCircle(cx, cy, outerR, color);
    int16_t innerR = outerR - 2;
    if (innerR > 0)
        display.fillCircle(cx, cy, innerR, GxEPD_WHITE);
}

void drawCenterLine(int16_t baselineY, const String& s, const GFXfont* f, uint16_t color, int16_t borderMargin)
{
    display.setFont(f);
    int16_t x1, y1;
    uint16_t w, h;
    int16_t availableWidth = display.width() - 2 * borderMargin;
    String clipped_s = clipToWidth(s, f, availableWidth, false);
    display.getTextBounds(clipped_s, 0, 0, &x1, &y1, &w, &h);
    int16_t x = (display.width() - (int16_t)w) / 2;
    display.setTextColor(color);
    display.setCursor(x, baselineY);
    display.print(clipped_s);
}

int16_t lineHeight(const GFXfont* f)
{
    display.setFont(f);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds("Hg", 0, 0, &x1, &y1, &w, &h);
    return (int16_t)h + 6;
}

void renderMessage(const String& msg, const GFXfont* font, int16_t borderMargin)
{
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);
        drawCenterLine(display.height() / 2, msg, font, GxEPD_BLACK, borderMargin);
    } while (display.nextPage());
}

uint16_t read16(File& f)
{
    uint16_t result;
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}
uint32_t read32(File& f)
{
    uint32_t result;
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}

void drawFromFile(const char* filename)
{
    Serial.print("Rendering...");
    display.setFullWindow();
    display.firstPage();

    do {
        Serial.print(".");

        // check if file exists
        File file = SPIFFS.open(filename, "r");
        if (!file) {
            Serial.println("");
            Serial.println("File does not exist");
            return;
        }

        // check if valid file
        if (read16(file) != 0x4D42) {
            Serial.println("");
            Serial.println("Not a bmp file");
            file.close();
            return;
        }

        file.seek(10);
        uint32_t offset = read32(file); // Start of pixel data
        file.seek(18);
        uint32_t width = read32(file);
        int32_t height = (int32_t)read32(file);
        file.seek(28);
        uint16_t depth = read16(file); // Bits per pixel (1 or 24)

        // bmp stored upside down
        bool flip = (height > 0);
        uint32_t absHeight = abs(height);
        uint32_t rowSize = (width * depth / 8 + 3) & ~3; // Rows are 4-byte padded

        // pixel loop
        for (uint32_t i = 0; i < absHeight; i++) {
            uint32_t row = flip ? (absHeight - 1 - i) : i;
            file.seek(offset + (row * rowSize));

            for (uint32_t j = 0; j < width; j++) {
                uint16_t color = GxEPD_WHITE;

                if (depth == 24) {
                    uint8_t b = file.read();
                    uint8_t g = file.read();
                    uint8_t r = file.read();
                    // Simple thresholding for 3-color (Red/Black/White)
                    if (r > 200 && g < 100 && b < 100)
                        color = GxEPD_RED;
                    else if (r < 128 && g < 128 && b < 128)
                        color = GxEPD_BLACK;
                }
                else if (depth == 1) {
                    // Read 1-bit monochrome data
                    static uint8_t b;
                    if (j % 8 == 0)
                        b = file.read();
                    color = (b & (0x80 >> (j % 8))) ? GxEPD_WHITE : GxEPD_BLACK;
                }

                // GxEPD2 paged loop only renders pixels in the current page buffer
                display.drawPixel(j, i, color);
            }
        }

        file.close();
    } while (display.nextPage());

    Serial.println("");
}
