#pragma once
#include <GxEPD2_3C.h>
#include <SPIFFS.h>

//  pins for e-ink display
#define EPD_CS 15
#define EPD_DC 27
#define EPD_RST 26
#define EPD_BUSY 25
#define EPD_SCK 13
#define EPD_MOSI 14

//  Init displayed in paged mode (due to lack of ram)
GxEPD2_3C<GxEPD2_750c_Z90, 128> display(GxEPD2_750c_Z90(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

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

void drawFromFile(const char* filePath, bool useThreeColors = false)
{

    // Initial file check to get dimensions
    File file = SPIFFS.open(filePath, "r");
    if (!file || read16(file) != 0x4D42) {
        Serial.println("\nFile error");
        if (file)
            file.close();
        return;
    }

    file.seek(18);
    uint32_t width = read32(file);
    int32_t height = (int32_t)read32(file);
    file.seek(10);
    uint32_t offset = read32(file);
    file.seek(28);
    uint16_t depth = read16(file);
    file.close();

    uint32_t absHeight = abs(height);
    uint32_t rowSize = (width * depth / 8 + 3) & ~3;
    bool flip = (height > 0);

    Serial.println("File stats:");
    Serial.printf("Height: %d, Width: %d, Depth: %d \n", absHeight, width, depth);

    // Error buffers (2 bytes per pixel width)
    int16_t* currentRowErr = (int16_t*)malloc(width * sizeof(int16_t));
    int16_t* nextRowErr = (int16_t*)malloc(width * sizeof(int16_t));

    Serial.print("Rendering...");
    display.setFullWindow();
    display.firstPage();

    do {

        // Reset error buffers for each page refresh
        memset(currentRowErr, 0, width * sizeof(int16_t));
        memset(nextRowErr, 0, width * sizeof(int16_t));

        file = SPIFFS.open(filePath, "r");

        for (uint32_t i = 0; i < absHeight; i++) {

            // BMPs are bottom-up; read top-down for dither flow
            uint32_t bmpRow = flip ? (absHeight - 1 - i) : i;
            file.seek(offset + (bmpRow * rowSize));

            for (uint32_t j = 0; j < width; j++) {
                int16_t gray;
                if (depth == 24) {
                    uint8_t b = file.read();
                    uint8_t g = file.read();
                    uint8_t r = file.read();
                    gray = (r * 77 + g * 150 + b * 29) >> 8;

                    // increase brightness when using 3 colors
                    // it still looks pretty bad, but it gives it a very artsy feel
                    if (useThreeColors) {
                        float normalized = gray / 255.0;
                        gray = (uint8_t)(pow(normalized, 0.5) * 255.0); // Gamma 0.5
                    }
                }
                else {
                    // 1-bit logic: treat as 0 or 255
                    static uint8_t b_byte;
                    if (j % 8 == 0)
                        b_byte = file.read();
                    gray = (b_byte & (0x80 >> (j % 8))) ? 255 : 0;
                }

                // dithering
                int16_t pixelWithErr = gray + currentRowErr[j];
                uint16_t color;
                int16_t actualGray;

                // 3 color logic
                if (useThreeColors) {
                    if (pixelWithErr < 85) {
                        color = GxEPD_BLACK;
                        actualGray = 0;
                    }
                    else if (pixelWithErr < 170) {
                        color = GxEPD_RED;
                        actualGray = 127; // Mid-point target for Red
                    }
                    else {
                        color = GxEPD_WHITE;
                        actualGray = 255;
                    }
                }
                else {
                    // Threshold to Black or White
                    color = (pixelWithErr < 128) ? GxEPD_BLACK : GxEPD_WHITE;
                    actualGray = (color == GxEPD_BLACK) ? 0 : 255;
                }

                // for some reason, the screen is refusing to draw the bottom part of the image
                // I don't know if this is a memory issue or (more likely) an issue in the rendering code
                // luckily, I can center the image for it to look better, sometimes it even fits inside of the passepartout, in my case approx 750px tall.
                // calc is (display height - image height post resize) / 2
                int16_t yOffset = (GxEPD2_750c_Z90::WIDTH - absHeight) / 2; //::WIDTH used because frame is sideways
                display.drawPixel(j, i + yOffset, color);

                // Calculate error and diffuse to neighbors
                int16_t err = pixelWithErr - actualGray;
                if (j + 1 < width)
                    currentRowErr[j + 1] += err * 7 / 16;
                if (i + 1 < absHeight) {
                    if (j > 0)
                        nextRowErr[j - 1] += err * 3 / 16;
                    nextRowErr[j] += err * 5 / 16;
                    if (j + 1 < width)
                        nextRowErr[j + 1] += err * 1 / 16;
                }
            }

            // Prepare for next row: move nextRow errors up and clear
            memcpy(currentRowErr, nextRowErr, width * sizeof(int16_t));
            memset(nextRowErr, 0, width * sizeof(int16_t));
        }

        file.close();
        Serial.print(".");

    } while (display.nextPage());

    free(currentRowErr);
    free(nextRowErr);
    Serial.println("\nRendering complete!");
}