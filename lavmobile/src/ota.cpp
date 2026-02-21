#include "ota.h"
#include "config-wifi.h"
#include <Arduino.h>
#include <ElegantOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define WIFI_CONNECTING_FEEDBACK 13

void OTA::setup()
{
    pinMode(WIFI_CONNECTING_FEEDBACK, OUTPUT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // from "config-wifi.h, see readme file for contents"

#ifdef DEBUG
    Serial.println("OTA.setup");
    Serial.print("Connecting to wifi ");
    Serial.print(WIFI_SSID);
    Serial.print(" ");
#endif

    while (WiFi.status() != WL_CONNECTED) {

#ifdef DEBUG
        Serial.print(".");
#endif

        // blink build in led while connecting to wifi
        digitalWrite(WIFI_CONNECTING_FEEDBACK, HIGH);
        delay(500);
        digitalWrite(WIFI_CONNECTING_FEEDBACK, LOW);
        delay(500);
    }

#ifdef DEBUG
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());
#endif

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