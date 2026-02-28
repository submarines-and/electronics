#include "ota.h"
#include "config-wifi.h"
#include <ElegantOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define FEEDBACK_LED 10

void OTA::setup()
{
    pinMode(FEEDBACK_LED, OUTPUT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // from "config-wifi.h, see readme file for contents"

    Serial.println("OTA.setup");
    Serial.print("Connecting to wifi ");
    Serial.print(WIFI_SSID);
    Serial.print(" ");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");

        // blink build in led while connecting to wifi
        rgbLedWrite(FEEDBACK_LED, 64, 0, 0);
        delay(500);

        rgbLedWrite(FEEDBACK_LED, 0, 0, 0);
        delay(500);
    }

    Serial.println("Connected!");
    Serial.println(WiFi.localIP());

    ElegantOTA.begin(&server);
    server.begin();
}

void OTA::checkForUpdate()
{
    server.handleClient();
    ElegantOTA.loop();
}