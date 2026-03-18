#include "config-wifi.h"
#include "gfx.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Fonts/FreeSans12pt7b.h>
#include <WiFi.h>

// http://192.168.176.177
AsyncWebServer server(80);
QueueHandle_t longOperationQueue;

void setup()
{

    Serial.begin(9600);
    while (!Serial) {
    }

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
    longOperationQueue = xQueueCreate(10, sizeof(AsyncWebServerRequest*));

    // curl -v -F "data=@/Users/submarines/Downloads/x.bmp" http://192.168.176.177/upload
    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest* request) {
        // response already set ?
        if (request->getResponse()) {
            // 400 No Content-Length
            return;
        }

        // nothing uploaded ?
        if (!request->_tempObject) {
            return request->send(400, "text/plain", "Nothing uploaded");
        }

        uint8_t* buffer = reinterpret_cast<uint8_t*>(request->_tempObject);
        // process the buffer

        delete buffer;
        request->_tempObject = nullptr;

        request->send(200, "text/plain", "OK");
        //
    },
              [](AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
                  Serial.printf("Upload[%s]: start=%u, len=%u, final=%d\n", filename.c_str(), index, len, final);

                  // first pass ?
                  if (!index) {
                      // Note: using content type to determine size is not reliable!
                      size_t size = request->header("Content-Length").toInt();
                      if (!size) {
                          request->send(400, "text/plain", "No Content-Length");
                      }
                      else {
                          Serial.printf("Allocating buffer of %u bytes\n", size);
                          uint8_t* buffer = new (std::nothrow) uint8_t[size];
                          if (!buffer) {
                              // not enough memory
                              Serial.println("Out of memory!");
                              request->abort();
                              return;
                          }

                          request->_tempObject = buffer;
                      }
                  }

                  if (len) {
                      memcpy(reinterpret_cast<uint8_t*>(request->_tempObject) + index, data, len);
                  }

                  if (final) {
                      Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);

                      // flag for later procesing of new image
                      if (xQueueSend(longOperationQueue, (void*)&request, (TickType_t)10) != pdPASS) {
                          request->send(500);
                      }
                      else {
                          request->send(200, "text/plain", "OK");
                      }
                  }
                  //
              });

    server.begin();
    Serial.println("Waiting for image!");
}

void loop()
{
    if (uxQueueMessagesWaiting(longOperationQueue)) {
        AsyncWebServerRequest* request;
        if (xQueueReceive(longOperationQueue, &(request), (TickType_t)10)) {
            Serial.println("Processing entry in image queue...");

            do {
                display.fillScreen(GxEPD_WHITE);
                //      display.drawBitmap(0, 0, request->_tempObject, 528, 880, GxEPD_BLACK);
                drawText(70, 100, "Meanwhile in Vienna", &FreeSans12pt7b, GxEPD_RED);
            } while (display.nextPage());
        }
    }
}