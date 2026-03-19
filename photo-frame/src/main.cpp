#include "config-wifi.h"
#include "gfx.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Fonts/FreeSans12pt7b.h>
#include <SPIFFS.h>
#include <WiFi.h>

AsyncWebServer server(80);
QueueHandle_t longOperationQueue;
const char* tmpFilename = "/tmp.bmp";

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
    }

    Serial.println("Mounting file system...");
    if (!SPIFFS.begin(true)) {
        Serial.println("File system mount failed");
        return;
    }

    // erase all files
    SPIFFS.format();

    // init display early for debug messages
    Serial.println("Starting display...");
    initDisplay(3);

    // connect to wifi
    Serial.println("Connecting to wifi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        do {
            drawText(70, 140, "Error when connecting to wifi!", &FreeSans12pt7b, GxEPD_BLACK);
        } while (display.nextPage());

        return;
    }

    Serial.println(WiFi.localIP());

    // setup queue for later batch processing
    longOperationQueue = xQueueCreate(10, sizeof(char*));

    // curl -v -F "data=@/Users/submarines/Downloads/x.bmp" http://192.168.176.177/upload
    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest* request) { request->send(200, "text/plain", "OK"); }, [](AsyncWebServerRequest* request, String fileName, size_t index, uint8_t* data, size_t len, bool final) {
        Serial.printf("Upload[%s]: start=%u, len=%u, final=%d\n", tmpFilename, index, len, final);

        // first cycle = create and open file
        if (!index) {
            if (SPIFFS.exists(tmpFilename)) {
                SPIFFS.remove(tmpFilename);
            }

            request->_tempFile = SPIFFS.open(tmpFilename, "w");
        }

        // append current round of bytes to file
        if (len) {
            request->_tempFile.write(data, len);
        }

        // all done
        if (final) {
            Serial.printf("UploadEnd: %s, %u B\n", tmpFilename, index + len);
            request->_tempFile.close();

            // flag for later procesing of new image (handled in loop function)
            if (xQueueSend(longOperationQueue, (void*)&tmpFilename, (TickType_t)10) != pdPASS) {
                request->send(500);
            }
            else {
                request->send(200, "text/plain", "OK");
            }
        }
        //
    });

    /*
    // Debug echo file endpoint
    server.on("/download", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, tmpFilename, "img/bmp");
    });
    */

    server.begin();
    Serial.println("Ready!");
}

void loop()
{
    // handle queued operations
    if (uxQueueMessagesWaiting(longOperationQueue)) {
        const char* filePath;
        if (xQueueReceive(longOperationQueue, &(filePath), (TickType_t)10)) {
            Serial.print("Processing entry in image queue: ");
            drawFromFile(tmpFilename);
            Serial.println("Done!");
        }
    }
}
