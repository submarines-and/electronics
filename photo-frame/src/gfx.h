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

void drawFromFile(const char* filePath, bool colorCount = 2)
{
    Serial.print("Rendering...");
    display.setFullWindow();
    display.firstPage();

    do {
        Serial.print(".");

        // check if file exists
        File file = SPIFFS.open(filePath, "r");
        if (!file) {
            Serial.println("");
            Serial.println("File does not exist");
            Serial.println(filePath);
            return;
        }

        // check if valid file
        if (read16(file) != 0x4D42) {
            Serial.println("");
            Serial.println("Not a bmp file");
            Serial.println(filePath);
            file.close();
            return;
        }

        file.seek(10);
        uint32_t offset = read32(file); // start of pixel data
        file.seek(18);
        uint32_t width = read32(file);
        int32_t height = (int32_t)read32(file);
        file.seek(28);
        uint16_t depth = read16(file); // Bits per pixel (1 or 24)

        bool flip = (height > 0); // bmp stored upside down
        uint32_t absHeight = abs(height);
        uint32_t rowSize = (width * depth / 8 + 3) & ~3; // each row is 4-byte padded

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

                    // convert RGB value to greyscale
                    uint8_t greyValue = (uint8_t)((r * 77 + g * 150 + b * 29) >> 8);

                    if (colorCount == 3) {
                        // select color based on arbitrary range for 3 color
                        if (greyValue < 86) {
                            color = GxEPD_BLACK;
                        }
                        else if (greyValue < 171) {
                            color = GxEPD_RED;
                        }
                    }
                    else {
                        // split 50/50 for 2 color
                        if (greyValue < 128) {
                            color = GxEPD_BLACK;
                        }
                        else {
                            color = GxEPD_WHITE;
                        }
                    }
                }
                else if (depth == 1) {
                    // 1-bit monochrome data
                    static uint8_t b;
                    if (j % 8 == 0) {
                        b = file.read();
                    }

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