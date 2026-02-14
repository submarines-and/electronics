#if defined(ESP8266)
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#elif defined(ESP32)
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include "config-wifi.h"
#include <ElegantOTA.h>

#if defined(ESP8266)
ESP8266WebServer server(80);
#elif defined(ESP32)
WebServer server(80);
#endif

void setup(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    server.on("/", []() {
        server.send(200, "text/plain", "/update");
    });

    ElegantOTA.begin(&server);
    server.begin();

    pinMode(13, OUTPUT);
}

void loop(void)
{
    server.handleClient();
    ElegantOTA.loop();

    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
}