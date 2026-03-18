#include "config-wifi.h"
#include "gfx.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Fonts/FreeSans12pt7b.h>
#include <WiFi.h>

// http://192.168.176.177
AsyncWebServer server(80);

void setup()
{

    Serial.begin(9600);
    while (!Serial) {
    }

    // init display early for debug messages
    initDisplay(3);

    // connect to wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        do {
            drawText(70, 140, "Error when connecting to wifi!", &FreeSans12pt7b, GxEPD_BLACK);
        } while (display.nextPage());

        return;
    }

    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest* request) { request->send(200); }, [](AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
        do {
            display.fillScreen(GxEPD_WHITE);
            display.drawBitmap(0, 0, data, 528, 880, GxEPD_BLACK);
            drawText(70, 100, "Meanwhile in Vienna", &FreeSans12pt7b, GxEPD_RED);
        } while (display.nextPage());
        //
    });

    server.begin();
}

void loop()
{
}