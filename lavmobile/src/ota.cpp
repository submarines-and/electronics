#include "ota.h"
#include "config-wifi.h"
#include <Arduino.h>
#include <ElegantOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>

void OTA::setup()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // from "config-wifi.h, see readme file for contents"

#ifdef DEBUG
    Serial.print("Connecting to wifi ");
    Serial.print(WIFI_SSID);
    Serial.print(" ");
#endif

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // redirect on root
    server.on("/", [this]() {
        server.send(200, "text/plain", "/update");
    });

    ElegantOTA.begin(&server);
    server.begin();
}

void OTA::checkForUpdate()
{
    server.handleClient();
    ElegantOTA.loop();
}