#include "config-wifi.h"
#include <ElegantOTA.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

// pins
#define AIN1 A1
#define AIN2 A2

// Ota server
WebServer server(80);

void setup(void)
{
    /**
     * OTA
     */
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // from "config-wifi.h, see readme file for contents"

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    server.on("/", []() { server.send(200, "text/plain", "/update"); });

    ElegantOTA.begin(&server);
    server.begin();

    /**
     * App
     */
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
}

void loop(void)
{
    /**
     * OTA
     */
    server.handleClient();
    ElegantOTA.loop();

    /**
     * App
     */

    // FORWARD
    digitalWrite(AIN2, LOW);

    // ramp speed up
    for (int duty_cycle = 0; duty_cycle < 256; duty_cycle++) {
        analogWrite(AIN1, duty_cycle);
        delay(10);
    }
    // ramp speed down
    for (int duty_cycle = 255; duty_cycle >= 0; duty_cycle--) {
        analogWrite(AIN1, duty_cycle);
        delay(10);
    }

    // REVERSE
    digitalWrite(AIN1, LOW);
    // ramp speed up
    for (int duty_cycle = 0; duty_cycle < 256; duty_cycle++) {
        analogWrite(AIN2, duty_cycle);
        delay(10);
    }
    // ramp speed down
    for (int duty_cycle = 255; duty_cycle >= 0; duty_cycle--) {
        analogWrite(AIN2, duty_cycle);
        delay(10);
    }
}