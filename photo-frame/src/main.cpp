#include "config-wifi.h"
#include "gfx.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Fonts/FreeSans12pt7b.h>
#include <SPIFFS.h>
#include <WiFi.h>

AsyncWebServer server(80);
QueueHandle_t longOperationQueue;

void setup()
{

    Serial.begin(9600);
    while (!Serial) {
    }

    Serial.println("");
    Serial.println("Mounting file system...");
    if (!SPIFFS.begin(true)) {
        Serial.println("File system mount Failed");
        return;
    }
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
    longOperationQueue = xQueueCreate(10, sizeof(String*));

    // curl -v -F "data=@/Users/submarines/Downloads/x.bmp" http://192.168.176.177/upload
    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest* request) { request->send(200, "text/plain", "OK"); }, [](AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
                  Serial.printf("Upload[%s]: start=%u, len=%u, final=%d\n", filename.c_str(), index, len, final);

        // open the file on first call and store the file handle in the request object
                  if (!index) {
            SPIFFS.remove("tmp.bmp");
            request->_tempFile = SPIFFS.open("/tmp.bmp", "w");
                  }

        // stream the incoming chunk to the opened file
                  if (len) {
            request->_tempFile.write(data, len);
                  }

                  if (final) {
                      Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
            request->_tempFile.close();

                      // flag for later procesing of new image
            if (xQueueSend(longOperationQueue, (void*)&filename, (TickType_t)10) != pdPASS) {
                          request->send(500);
                      }
                      else {
                          request->send(200, "text/plain", "OK");
                      }
                  }
                  //
              });

    server.begin();

    /*
    do {
        display.fillScreen(GxEPD_BLACK);
        display.drawBitmap(0, 0, epd_bitmap_, 528, 880, GxEPD_WHITE);
    } while (display.nextPage());
    */

    Serial.println("Ready!");
}

void loop()
{
    if (uxQueueMessagesWaiting(longOperationQueue)) {
        String* filename;
        if (xQueueReceive(longOperationQueue, &(filename), (TickType_t)10)) {
            Serial.println("Processing entry in image queue...");

            File file = SPIFFS.open("/x.bmp", "r");
            size_t size = file.size();
            uint8_t* buffer = (uint8_t*)malloc(size);

            if (buffer) {
                file.read(buffer, size);
                Serial.printf("Read %u bytes successfully\n", size);

            do {
                display.fillScreen(GxEPD_BLACK);
                    display.drawBitmap(0, 0, buffer, 528, 880, GxEPD_WHITE);
            } while (display.nextPage());

                free(buffer);
                Serial.println("Done!");
            }
            else {
                Serial.println("Failed to allocate memory for buffer");
            }
        }
    }
}