#include "config-wifi.h"
#include "gfx.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Fonts/FreeSans12pt7b.h>
#include <WiFi.h>

// http://192.168.176.177
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>File Upload</title>
</head>
<body>
    <h2>Upload a file</h2>
    <form action="/upload" method="post" enctype="multipart/form-data">
        <input type="file" name="fileToUpload" id="fileToUpload">
        <input type="submit" value="Upload File" name="submit">
    </form>
</body>
</html>
)rawliteral";

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
    }

    // init display early for debug messages
    initDisplay(3);

    do {
        display.fillScreen(GxEPD_WHITE);
        drawText(70, 100, "Connecting to wifi...", &FreeSans12pt7b, GxEPD_BLACK);
    } while (display.nextPage());

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {

        do {
            drawText(70, 140, "Error when connecting to wifi!", &FreeSans12pt7b, GxEPD_BLACK);
        } while (display.nextPage());

        return;
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", index_html);
    });

    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest* request) { request->send(200); }, [](AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
        display.fillScreen(GxEPD_WHITE);
        display.drawBitmap(0, 0, data, 528, 880, GxEPD_BLACK);
        //
    });

    server.begin();

    do {
        drawText(70, 100, "Connected, waiting for image!", &FreeSans12pt7b, GxEPD_BLACK);
    } while (display.nextPage());
}

void loop()
{
}