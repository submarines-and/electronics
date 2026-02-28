#include "ota.h"
#include "config-wifi.h"
#include <ElegantOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define WIFI_CONNECTING_FEEDBACK 10

void OTA::setup()
{
    pinMode(WIFI_CONNECTING_FEEDBACK, OUTPUT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // from "config-wifi.h, see readme file for contents"

    Serial.println("OTA.setup");
    Serial.print("Connecting to wifi ");
    Serial.print(WIFI_SSID);
    Serial.print(" ");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");

        // blink build in led while connecting to wifi
        digitalWrite(WIFI_CONNECTING_FEEDBACK, HIGH);
        delay(500);
        digitalWrite(WIFI_CONNECTING_FEEDBACK, LOW);
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